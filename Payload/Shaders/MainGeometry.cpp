#version 440 core
/*
A Geometry Shader (GS) is a Shader program written in GLSL that governs the processing of Primitives.
Geometry shaders reside between the Vertex Shaders (or the optional Tessellation stage) and the fixed-function Vertex Post-Processing stage.

A geometry shader is optional and does not have to be used.

Geometry shader invocations take a single Primitive as input and may output zero or more primitives.
There are implementation-defined limits on how many primitives can be generated from a single GS invocation.
GS's are written to accept a specific input primitive type and to output a specific primitive type
*/

//***********************************************************
// SPECIFY THE INPUT PRIMITIVE TYPE FOR THIS GEOMETRY SHADER
//***********************************************************
/*
Each geometry shader is designed to accept a specific Primitive type as input and to output a specific primitive type.
The accepted input primitive type is defined in the shader:

layout(input_primitive​) in;

The input_primitive​ type must match the primitive type for the vertex stream provided to the GS.
If Tessellation is enabled, then the primitive type is specified by the Tessellation Evaluation Shader's output qualifiers.
If Tessellation is not enabled, then the primitive type is provided by the drawing command that renders with this shader program.
The valid values for input_primitive​, along with the valid OpenGL primitive types or tessellation forms, are:

GL_POINTS , GL_LINES, GL_LINE_STRIP, GL_LINE_LIST, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
*/
layout(triangles) in;

//******************************************************************
// SPECIFY THE INPUT VARIABLES FROM VERTEX SHADER TO GEOMETRY SHADER
//******************************************************************
in int VMaterialIndex[];
in vec2 VTextureCoords[];
in vec4 VFragmentPosition[];

//***********************************************************
// SPECIFY THE OUTPUT PRIMITIVE TYPE FOR THIS GEOMETRY SHADER
//***********************************************************
//****** OUT Variables **********
/*
The output_primitive​ must be one of the following:

points
line_strip
triangle_strip
*/
layout(triangle_strip,max_vertices=3) out;

//******************************************************************
// SPECIFY THE OUTPUT VARIABLES FROM VERTEX SHADER TO GEOMETRY SHADER
//******************************************************************
out int FMaterialIndex;
out vec2 FTextureCoords;
out vec4 FFragmentPosition;
out vec3 Normal;

//******************************************************************
// FUNCTION DECLARATION
//******************************************************************
vec3 ComputeNormal();


void main()
{
vec3 normal=ComputeNormal();

//forward the following variables from vertex shader to the fragment shader for vertex # 1
//gl position, texture coordinates , material index and normal
gl_Position = gl_in[0].gl_Position;
FFragmentPosition=VFragmentPosition[0];
FTextureCoords = VTextureCoords[0];
FMaterialIndex=VMaterialIndex[0];
Normal=normal;
EmitVertex();

//forward the following variables from vertex shader to the fragment shader for vertex # 2
//gl position, texture coordinates , material index and normal
gl_Position = gl_in[1].gl_Position;
FFragmentPosition=VFragmentPosition[1];
FTextureCoords = VTextureCoords[1];
FMaterialIndex=VMaterialIndex[1];
Normal=normal;
EmitVertex();

//forward the following variables from vertex shader to the fragment shader for vertex # 3
//gl position, texture coordinates , material index and normal
gl_Position = gl_in[2].gl_Position;
FFragmentPosition=VFragmentPosition[2];
FTextureCoords = VTextureCoords[2];
FMaterialIndex=VMaterialIndex[2];
Normal=normal;
EmitVertex();

EndPrimitive();
}

//******************************************************************
/*
Function to calculate normal of a primitive or a triangle

the procedure involved is as follows:
1) use the fragment positions for the 3 vertices that form the primitive / triangle
2) generate a directional vector from fragment # 1 and fragment # 2
3) generate a directional vector from fragment # 1 and fragment # 3
4) do a cross product between the above two directional vectors
5) normalize the vector obtained in step (4) above
*/
//******************************************************************
vec3 ComputeNormal()
{
vec3 a =vec3(VFragmentPosition[0]) - vec3(VFragmentPosition[1]);
vec3 b =vec3(VFragmentPosition[2]) - vec3(VFragmentPosition[1]);
return normalize(cross(b, a));
}


