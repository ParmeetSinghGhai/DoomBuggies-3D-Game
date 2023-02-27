#version 440 core
/*The Vertex Shader is the programmable Shader stage in the rendering pipeline that handles
the processing of individual vertices. Vertex shaders are fed Vertex Attribute data, as specified
from a vertex array object by a drawing command. A vertex shader receives a single vertex from the vertex stream
and generates a single vertex to the output vertex stream. There must be a 1:1 mapping from input vertices to output vertices.

A vertex shader basically runs for each primitive / triangle
*/
//*****************************************************//
//   Vertex Inputs Variables
//*****************************************************//
/*
User-defined inputs for vertex shaders are called vertex attributes.
They are passed via vertex arrays to the vertex shader, usually from data stored in Buffer Objects.
*/
layout (location = 0) in vec3 Position; //vertex position coordinates
layout (location = 1) in vec2 TextureCoords;//vertex texture coordinates
layout (location = 2) in int MaterialIndex;//vertex material index for each vertex of a triangle.
                                           //although each triangle is assigned one material
layout (location = 3) in int VertexIndex;//this is the unique vertex index assigned to each vertex
//*****************************************************//
//   Object Matrix Uniform Variables
//*****************************************************//
/*
A uniform is a global Shader variable declared with the "uniform" storage qualifier.
These act as parameters that the user of a shader program can pass to that program.
Their values are stored in a program object.
Uniforms are so named because they do not change from one shader invocation to the next
within a particular rendering call thus their value is uniform among all invocations.
This makes them unlike shader stage inputs and outputs, which are often different for each invocation of a shader stage.
*/
layout (location = 4) uniform mat4 Model;//this is the model matrix that every vertex gets multiplied with
layout (location = 5) uniform mat4 View;//this is the camera view matrix that every vertex gets multiplied with
layout (location = 6) uniform mat4 Projection;//this is the projection matrix that every vertex gets multiplied with
//*****************************************************//
//   Object Bone Uniform Variables
//*****************************************************//
layout (location = 7) uniform int BoneStartIndex;//this is the bone start index and is unique for each game object
layout (location = 8) uniform int BoneEndIndex;//this is the bone end index and is unique for each game object
//*****************************************************//
//   Lighting uniform Variables
//*****************************************************//
layout (location = 9) uniform vec4 CameraPosition;//this is the camera position vector that gives out the world coordinates of camera's position
                                                  //this position vector is used in lighting calculations in fragment shader
layout (location = 10) uniform int TotalLights;//this is an integer specifying the total number of lights to be used in fragment shader
                                               //for lighting calculations
layout (location = 11) uniform int NoMaterialFlag;//this integer when set to -1 indicates that the object has no material assigned to it
                                                  //so a default material is used for that object in fragment shader
//*****************************************************//
//   Shader Storage Buffer Object Bindings
//*****************************************************//
const int WeightData_Binding=0;
const int ChildBoneData_Binding=1;
const int BoneData_Binding=2;
const int MaterialData_Binding=3;
const int LightData_Binding=4;
/*
A Shader Storage Buffer Object is a Buffer Object that is used to store and retrieve data from within the OpenGL Shading Language.
SSBOs are declared as interface blocks, using the buffer keyword.They have special layout qualifiers for specifying important
aspects of them, such a memory layout and binding qualities. If the last member variable of a shader storage block is declared
with an indeterminate array length (using []), then the size of this array is determined at the time the shader is executed.
The size is basically the rest of the buffer object range that was attached to that binding point.

each data structure used to implement these shader storage buffer objects need to have
data tighly packed in them where the total bytes for all variables contained in the structure
needs to be multiple of 2 so in cases where these data structure do not have enough variables
to make that happen, padding variables are provided for the same
*/
//*****************************************************//
//   Weights Shader Storage Buffer
//*****************************************************//
/*
this buffer is used by all the bones from all the gameobject to keep track of the weights
assigned to the vertices linked to that bone

its usage is shown with an example below:
gameobject 1 has 2 bones with 2 vertices
gameobject 2 has 3 bones with 2 vertices
the weights in this buffer will be aligned in the following way:

go=gameobject
b=bone
v=vertex
w=vertex weight

Weights[0]=go1.b1.v1.w - index 0 will be WeightStartIndex for bone 1 of gameobject 1
Weights[1]=go1.b1.v2.w - index 1 will be WeightEndIndex for bone 1 of gameobject 1
Weights[2]=go1.b2.v1.w - index 2 will be WeightStartIndex for bone 2 of gameobject 1
Weights[3]=go1.b2.v2.w - index 3 will be WeightEndIndex for bone 2 of gameobject 1
Weights[4]=go2.b1.v1.w
Weights[5]=go2.b1.v2.w
Weights[6]=go2.b2.v1.w
Weights[7]=go2.b2.v2.w
Weights[8]=go2.b3.v1.w - index 8 will be WeightStartIndex for bone 3 of gameobject 2
Weights[9]=go2.b3.v2.w - index 9 will be WeightEndIndex for bone 3 of gameobject 2
*/
layout(std430, binding = WeightData_Binding) buffer VertexWeightData
{
float Weights[];
};
//*****************************************************//
//   Child Bone Shader Storage Buffer
//*****************************************************//
/*
this buffer is also used by all the bones from all the game objects to keep track of the
child bones assigned to the bone by using the unique indices assigned to each bone

its usage is shown with an example below:
gameobject 1 has 3 bones with indices b1=1, b2=2 and b3=3.
b1 has 2 child bones with indices 1,2 and b2 has 1 child bones with index 3 and b3 has no child bones

gameobject 2 has 4 bones with indices b1=4, b2=5 , b3=6 and b4=7
b1 has 3 child bones with indices 4,5,6 , b2 and b3 have no child bones and b4 has 2 child bones with index 6,7

go=gameobject
b=bone
cbindex = childboneindex

ChildBones[0] = go1.b1.cb1index = 1
ChildBones[1] = go1.b1.cb2index = 2
ChildBones[2] = go1.b2.cb1index = 3
ChildBones[3] = go2.b1.cb1index = 4  - index 3 will be the ChildStartIndex for bone 1 of gameobject 2
ChildBones[4] = go2.b1.cb2index = 5
ChildBones[5] = go2.b1.cb3index = 6  - index 5 will be the ChildEndIndex for bone 1 of gameobject 2
ChildBones[6] = go2.b4.cb1index = 6
ChildBones[7] = go2.b4.cb2index = 7
*/
layout(std430, binding = ChildBoneData_Binding) buffer ChildBoneData
{
int ChildBones[];
};
//*****************************************************//
//   Bone Shader Storage Buffer
//*****************************************************//
/*
this buffer is also used by all the bones from all the game objects to keep track of their
child bones, vertex weights and self , inherited and out matrices

self matrix is the local transformation that is applied to the bone
inherited matrix is the transformation that it inherits from its parent bone
out matrix = bone's inherited matrix * bone's self matrix
*/
struct Bone
{
int ChildStartIndex;
int ChildEndIndex;
int WeightStartIndex;
int WeightEndIndex;
mat4 SelfMatrix;
mat4 InheritedMatrix;
mat4 OutMatrix;
};
layout(std430, binding = BoneData_Binding) buffer BoneData
{
  Bone Bones[];
};
//*********************************************
//Material Shader Storage Buffer
//*********************************************
/*
this buffer is used to store material data from all gameobjects
*/
struct Material
{
float AmbientStrength;
int ShininessIndex;
int DiffuseMap;
int AmbientMap;
int SpecularMap;
int Padding1;
int Padding2;
int Padding3;
vec4 DiffuseColor;
vec4 SpecularColor;
};
layout(std430, binding = MaterialData_Binding) buffer MaterialData
{
  Material Materials[];
};
//*********************************************
//Light Shader Storage Buffer
//*********************************************
/*
this buffer is used to store light data for all the lights used in
light calculations in fragment shader
*/
struct Light
{
int LightType;
float Constant;
float Linear;
float Quadratic;
float CutOffAngle;
float OuterCutOffAngle;
float Padding1;
float Padding2;
vec4 Color;
vec4 Position;
vec4 Direction;
};
layout(std430, binding = LightData_Binding) buffer LightData
{
  Light Lights[];
};
//*********************************************************//
//   Output variables from Vertex Shader to Geometry Shader
//*********************************************************//
out vec4 VFragmentPosition;
out vec2 VTextureCoords;
out int VMaterialIndex;
//gl_position is a built in out variable so we dont have to explicitly specify them here
//************************************************************//
//   Function that Calculates Bone Matrix Effects on Vertices
//************************************************************//
vec4 CalCulateBoneMatrix();

void main()
{
 //Order of Matrix Multiplication
 //Projection * View * Model Matrix(Translate,Rotate,Scale) * Bone Matrix(Translate,Rotate,Scale)

 VFragmentPosition=Model * CalCulateBoneMatrix();
 gl_Position = Projection * View * VFragmentPosition;

 VMaterialIndex=MaterialIndex;
 VTextureCoords=TextureCoords;
};


//************************************************************//
/*
this function calculates the effects of all the bones present in a gameobject on a vertex

the procedure involved is as follows:
1) get the local coordinates of the vertex
2) iterate through all the bones in the gameobject. with each iteration do the following:
   A) use the "bonestartindex" property to retrieve the bone data for the bone
   B) from that bone data retrieve the "weightstartindex" value
   C) using the "weightstartindex" value and the vertex index of the vertex
      retrieve the "weight" assigned to this vertex under this bone
   D) calculate the "outmatrix" of the bone by multiplying the "inherited" matrix with the bone's
      "self" matrix
   E) get the transformed vertex position by multiplying the original vertex position
      with bone's "out" matrix
   F) subtract the transformed vertex position from the original vertex position
      to get the transformation that would be applied to the original vertex position
   G) multiply the transformation obtained with the weight of this vertex to
      get the weighted transformation
   H) add this weight transformation to the original vertex position to get the original
      position of the vertex linearly interpolated
   I) apply the bone "out" matrix to all its child bones
*/
//************************************************************//

vec4 CalCulateBoneMatrix()
{
vec4 PO=vec4(Position,1.0f);
int BI=BoneStartIndex;
int CS;
int CE;
int CI;
int VI;
while(BI <= BoneEndIndex)
{
    VI=Bones[BI].WeightStartIndex + VertexIndex;
    Bones[BI].OutMatrix=Bones[BI].InheritedMatrix * Bones[BI].SelfMatrix;

    PO=PO + ((Bones[BI].OutMatrix * PO - PO) * Weights[VI]);

    CS=Bones[BI].ChildStartIndex;
    CE=Bones[BI].ChildEndIndex;
    while(CS < CE)
    {
        CI=ChildBones[CS];
        Bones[CI].InheritedMatrix=Bones[BI].OutMatrix;
        Bones[CI].OutMatrix=Bones[CI].InheritedMatrix * Bones[CI].SelfMatrix;
        CS++;
    }
    BI++;
}
return PO;
}

