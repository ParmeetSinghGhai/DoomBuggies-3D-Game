#version 440 core
/*
A Fragment Shader is the Shader stage that will process a Fragment generated by the Rasterization into a set of colors and a single depth value.
The fragment shader is the OpenGL pipeline stage after a primitive is rasterized. For each sample of the pixels covered by a primitive,
a "fragment" is generated. Each fragment has a Window Space position, a few other values, and it contains all of the interpolated
per-vertex output values from the last Vertex Processing stage.

The output of a fragment shader is a depth value, a possible stencil value (unmodified by the fragment shader), and zero or more color
values to be potentially written to the buffers in the current framebuffers.
Fragment shaders take a single fragment as input and produce a single fragment as output.

The only place where you can use a sampler is in one of the GLSL standard library's texture lookup functions.
These functions access the texture referred to by the sampler. They take a texture coordinate as parameters.
*/
//*****************************************************//
//   Lighting uniform Variables
//*****************************************************//
layout (location = 9) uniform vec4 CameraPosition;
layout (location = 10) uniform int TotalLights;
layout (location = 11) uniform int NoMaterialFlag;
//*****************************************************//
//   Texture Variables
//*****************************************************//
/*
A sampler is a set of GLSL variable types. Variables of one of the sampler types must be uniforms or as function parameters.
Each sampler in a program represents a single texture of a particular texture type. The type of the sampler corresponds to the
type of the texture that can be used by that sampler.
*/
layout (binding = 0) uniform sampler2D DiffuseMap;
layout (binding = 1) uniform sampler2D SpecularMap;
layout (binding = 2) uniform sampler2D AmbientMap;
//*****************************************************//
//   Buffer Bindings
//*****************************************************//
const int MaterialData_Binding=3;
const int LightData_Binding=4;
//*****************************************************//
//   In & Out Variables
//*****************************************************//
in vec4 FFragmentPosition;
in vec3 Normal;
in vec2 FTextureCoords;
flat in int FMaterialIndex;
out vec4 FragColor;
//*********************************************
//Material Buffer
//*********************************************
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
//Light Buffer
//*********************************************
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
//*******************************
//   Function Declaration
//*******************************
vec3 CalculateAmbientLight(vec3 LightColor,Material material);
vec3 CalculateDiffuseLight(vec3 LightColor,vec3 LightDirection,Material material);
vec3 CalculateSpecularLight(vec3 LightColor,vec3 LightDirection,vec3 ViewerDirection,Material material);
vec3 GetLightDirection(vec4 LightDirection);
vec3 GetViewerDirection();
vec4 CalculateAllLights(Material material);

void main()
{
Material material;
//if no material is provided for a primitive / triangle then a default primitive is created
//the uniform variable "NoMaterialFlag" has a value of -1 if the primitive has not been
//assigned any material

//NoMaterialFlag , value = 1(true) object has no material, value = 0(false) object has material

if (NoMaterialFlag == 1)
material = Material(1.0f, 32, 0, 0, 0, 0, 0, 0, vec4(0.5f, 0.5f, 0.5f, 0.0f), vec4(0.5f, 0.5f, 0.5f, 0.0f));
else
material = Materials[FMaterialIndex];

FragColor = CalculateAllLights(material);
};


/*
this function calculates the ambient light for an object based on the light source's color and the material of the object

it uses the opengl "texture" function that accesses the texture referred to by a sampler. it takes
a texture coordinate as parameters.

the procedure involved is as follows:
check if the object's material has a diffuse map and an ambient map if yes then use the fragment's position
to get the texture's RGB values from the diffuse map and the ambient map and then use those values in the
formulae below:

ambient= light color * diffusemap's RGB component * ambientmap's RGB component

if the diffuse map and ambient map are not present then use the materials's diffuse color and ambient strength

ambient= light color * ambient strength * diffuse colors
*/
vec3 CalculateAmbientLight(vec3 LightColor,Material material)
{
    vec3 ambient;

    if(material.AmbientMap > 0 && material.DiffuseMap > 0)
    ambient= LightColor * (texture(AmbientMap, FTextureCoords).rgb * texture(DiffuseMap, FTextureCoords).rgb);
    else if(material.DiffuseMap > 0)
    ambient= LightColor * (material.AmbientStrength * texture(DiffuseMap, FTextureCoords).rgb);
    else
    ambient= LightColor * vec3(material.AmbientStrength * material.DiffuseColor);

    return ambient;
}
/*
this function calculates the diffuse light for an object based on the light source's color , the material of the object
and the direction of the light from the object (fragment) to the light source.

it uses the opengl "texture" function that accesses the texture referred to by a sampler. it takes
a texture coordinate as parameters.

the procedure involved is as follows:
1) do a dot product between the normal of the fragment and the direction from the fragment to the light source.
2) if the dot product is less than 0.0 then cap it to 0.0
3) if material's diffuse map is preset then multiple the texture RGB value, obtained from fragment's texture position
   using the texture() function, with the dot product obtained above and the light source's color
4) if material's diffuse map is not preset then multiple the material diffuse color with the dot product obtained above
   and the light source's color
*/
vec3 CalculateDiffuseLight(vec3 LightColor,vec3 LightDirection,Material material)
{
    vec3 diffuse;

    float diff = max(dot(Normal, LightDirection), 0.0);

    if(material.DiffuseMap > 0)
    diffuse= LightColor * (diff * texture(DiffuseMap, FTextureCoords).rgb);
    else
    diffuse= LightColor * vec3(diff * material.DiffuseColor);

    return diffuse;
}
/*
this function calculates the diffuse light for an object based on the light source's color, the material of the object
,the direction of the light from the object (fragment) to the light source and the viewer direction which is direction
from the object (fragment) to the camera

it uses the opengl "texture" function that accesses the texture referred to by a sampler. it takes
a texture coordinate as parameters.

the procedure involved is as follows:
1) calculate the reflection direction which is a negative of the direction from the fragment to the light source off the
   normal of the object
2) do a dot product between the reflection direction and the direction from the fragment to the camera.
3) if the dot product is less than 0.0 then cap it to 0.0
4) raise the dot product to the power of the material's shininess index
5) if material's specular map is preset then multiple the texture RGB value, obtained from fragment's texture position
   using the texture() function, with the value obtained in step(4) and the light source's color
6) if material's specular map is not preset then multiple the material specular color with the value obtained in step(4)
   above and the light source's color
*/
vec3 CalculateSpecularLight(vec3 LightColor,vec3 LightDirection,vec3 ViewerDirection,Material material)
{
    vec3 specular;

    vec3 reflectDir = reflect(-LightDirection, Normal);
    float spec=pow(max(dot(ViewerDirection, reflectDir), 0.0), material.ShininessIndex);

    if(material.SpecularMap > 0)
    specular=LightColor * (spec * texture(SpecularMap, FTextureCoords).rgb);
    else
    specular=LightColor * vec3(spec * material.SpecularColor);

    return specular;
}
/*
this function calculates the direction from the object (fragment) to the light source

the light direction is represented by a 4d vector (x,y,z,w)

in case of directional light source, the w component is set -1.0f as the user specifies this
direction as the direction from the light source to the object while we need the direction from the
fragment to the light source so we simply negative this direction

in case of spotlights and pointlights, the position of the light source is given as argument
to this function so the direction from the fragment to the light source can be calculated
*/
vec3 GetLightDirection(vec4 LightDirection)
{
    vec3 lightDir;

    if(LightDirection.w<=0.0f)
    lightDir = normalize(-vec3(LightDirection));
    else
    lightDir = normalize(vec3(LightDirection) - vec3(FFragmentPosition));

    return lightDir;
}
/*
this function calculates the direction from the object (fragment) to the camera
using the camera position and the fragment position
*/
vec3 GetViewerDirection()
{
    vec3 viewDir = normalize(vec3(CameraPosition)-vec3(FFragmentPosition));
    return viewDir;
}
/*
this function iterates through all the light sources namely directional lights, points lights
and spot lights and calculates the combined effect of all those lights on the vertex

for directional lights, the light direction obtained from GetLightDirection() function, the light color and the material
data are fed to all the 3 functions:CalculateAmbientLight,CalculateDiffuseLight and CalculateSpecularLight

for point lights, the light direction obtained from GetLightDirection() function, the light color and the material
data are fed to all the 3 functions:CalculateAmbientLight,CalculateDiffuseLight and CalculateSpecularLight
along with the attenuation. attenuation is the value that determines the intensity of the point light at
far away distances. with point lights the light fades away as the distance between the object and the light grows
first the distance between the light source and the object is calculated and then its used in calculating
attenuation using the following formulea:
attenuation = 1.0f / light constant + light linear constant * distance + light quadratic constant * (distance * distance));

for spot lights, the same calculations and variables are involved as the point lights with one difference
where besides attenuation, the intensity value is calculated and used in all 3 functions: CalculateAmbientLight,
CalculateDiffuseLight and CalculateSpecularLight
to calculate intensity value we first calculate the dot product between the direction from light source to the object
and the direction that the spot light is pointing at. we call this angle "theta"
then intensity is calculated as the ratio of (theta - outercutoffangle) / (innercutoffangle - outercutoffangle)

the light values from all point sources are combined/added together and output from this function
which is used to color the fragment and all other fragments of the primitive
*/
vec4 CalculateAllLights(Material material)
{
  vec3 ViewerDirection=GetViewerDirection();
  vec3 AllLights=vec3(0.0f,0.0f,0.0f);
    
  for(int i=0;i<TotalLights;i++)
  {
     if(Lights[i].LightType==0)
     {
        vec3 LightDirection=GetLightDirection(Lights[i].Direction);
        vec3 Color=vec3(Lights[i].Color);
        AllLights+=CalculateAmbientLight(Color,material) + CalculateDiffuseLight(Color,LightDirection,material) + CalculateSpecularLight(Color,LightDirection,ViewerDirection,material);
     }
     else if(Lights[i].LightType==1)
     {
        vec3 LightDirection=GetLightDirection(Lights[i].Position);
        vec3 Color=vec3(Lights[i].Color);

        float distance = length(vec3(Lights[i].Position) - vec3(FFragmentPosition));
        float attenuation = 1.0f / (Lights[i].Constant + Lights[i].Linear * distance + Lights[i].Quadratic * (distance * distance));
        AllLights+=(attenuation * CalculateAmbientLight(Color,material)) + (attenuation * CalculateDiffuseLight(Color,LightDirection,material)) + (attenuation * CalculateSpecularLight(Color,LightDirection,ViewerDirection,material));
     }
     else if(Lights[i].LightType==2)
     {
        vec3 LightDirection=GetLightDirection(Lights[i].Position);
        vec3 Color=vec3(Lights[i].Color);

        float distance = length(vec3(Lights[i].Position) - vec3(FFragmentPosition));
        float attenuation = 1.0f / (Lights[i].Constant + Lights[i].Linear * distance + Lights[i].Quadratic * (distance * distance));

        float theta = dot(LightDirection,normalize(vec3(Lights[i].Direction)));
        float epsilon = Lights[i].CutOffAngle - Lights[i].OuterCutOffAngle;
        float intensity = clamp(((theta - Lights[i].OuterCutOffAngle) / epsilon), 0.0f, 1.0f);

        AllLights+=(attenuation * intensity * CalculateDiffuseLight(Color,LightDirection,material)) + (attenuation * intensity * CalculateSpecularLight(Color,LightDirection,ViewerDirection,material));
     }
     
  }
  
  return vec4(AllLights,1.0f);
}







