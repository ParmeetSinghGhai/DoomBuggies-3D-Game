#version 440 core
//*****************************************************//
// Input variables for Vertex Shader
//*****************************************************//
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
//*****************************************************//
layout (location = 2) uniform mat4 Model;
layout (location = 3) uniform mat4 View;
layout (location = 4) uniform mat4 Projection;
//*****************************************************//
//when colorflag is 0 then Color is used which is an vertex attribute
//when colorflag is 1 then SpecialColor is used
//when colorflag is 2 then Texture is used
//when colorflag is 3 then Special color is used along with xmove for health and weapon boxes
//when colorflag is 4 then SkyBox is processed
layout (location = 5) uniform int ColorFlag;
layout (location = 6) uniform vec4 SpecialColor;
//*****************************************************//

//***** Only Used by GameText  ************************//
layout (location = 7) in vec2 TextureCoords;//used by gametext info panel showing text "health" and "weapon"
layout (location = 8) uniform float xmove;//used by gametext health and weapon boxes
//*****************************************************//

out vec4 VColor;
out vec2 Texture;
out vec3 SkyBoxTexture;
void main()
{
vec3 PPosition=Position;
//here we are processing the health and weapon status boxes for buggy objects
//when drawing these boxes, the colorflag is set to 3 and the x coordinates of their
//color values are set to 2.0f as markers as colorflag=3 is used by bounding boxes
//when they are being drawn in the foreground framebuffer
//
//these health and weapon boxes are made up of 2 triangles made up of 6 vertices
//the vertices which can be translated in the x direction where marked with the x coordinate
//of their color values being 2.0f
//these vertices out of the 6 vertices present are translated in the x direction
//according to the value of the 'xmove' variable
if(ColorFlag==3 && Color[0] >= 1.0f)

{
    PPosition[0]=Position[0] + xmove;
    gl_Position=Projection * View * Model * vec4(PPosition, 1.0f);
}
else if (ColorFlag==4)//processing skybox vertices
{
    /*A cubemap used to texture a 3D cube can be sampled using the positions of the cube as the texture
    coordinates. When a cube is centered on the origin (0,0,0) each of its position vectors is also a direction
    vector from the origin. This direction vector is exactly what we need to get the corresponding texture value at
    that specific cube’s position. For this reason we only need to supply position vectors and don’t need texture
    coordinates.
    */
    SkyBoxTexture=PPosition;
    /*
    We want the skybox to be centered around the player so that no matter how far the player moves,
    the skybox won’t get any closer giving the impression the surrounding environment is extremely large.
    The current view matrix however transforms all the skybox’s positions by rotating, scaling and translating them,
    so if the player moves, the cubemap moves as well! We want to remove the translation part of the view matrix
    so movement doesn’t affect the skybox’s position vectors
    */
    mat4 ModifiedView=View;
    ModifiedView[3][0]=0.0f;
    ModifiedView[3][1]=0.0f;
    ModifiedView[3][2]=0.0f;
    ModifiedView[3][3]=1.0f;
    ModifiedView[0][3]=0.0f;
    ModifiedView[1][3]=0.0f;
    ModifiedView[2][3]=0.0f;
    vec4 SkyBoxPosition = Projection * ModifiedView * Model * vec4(PPosition, 1.0f);
    /*
    perspective division is performed after the vertex shader has run, dividing the gl_Position’s
    xyz coordinates by its w component. the z component of the resulting division is equal to that vertex’s depth value.
    Using this information we can set the z component of the output position equal to its w component which will
    result in a z component that is always equal to 1.0, because when the perspective division is applied its z
    component translates to w / w = 1.0:
    The resulting normalized device coordinates will then always have a z value equal to 1.0: the maximum
    depth value. The skybox will as a result only be rendered wherever there are no objects visible (only then it
    will pass the depth test, everything else is in front of the skybox).
    */
    gl_Position=SkyBoxPosition.xyww;
}
else
{
    //this is processed for all other objects
    gl_Position=Projection * View * Model * vec4(PPosition, 1.0f);
}

VColor=vec4(Color,1.0f);
Texture=TextureCoords;
};
