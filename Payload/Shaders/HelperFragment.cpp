#version 440 core
in vec4 VColor;
in vec2 Texture;
in vec3 SkyBoxTexture;

out vec4 FragColor;
layout (binding = 0) uniform sampler2D TextureMap;
layout (binding = 1) uniform samplerCube SkyBox;

layout (location = 5) uniform int ColorFlag;
layout (location = 6) uniform vec4 SpecialColor;

void main()
{
//this option is used for showing health and weapon status boxes with the "xmove" value
//making these boxes bigger or smaller
//
//for drawing bounding boxes in the foreground framebuffer usually when they are
//flashing in blue, red or green color
//
//for drawing gameworldcubes boxes in the foreground framebuffer usually when they are
//flashing in yellow color
if(ColorFlag==1 || ColorFlag==3)
{
    FragColor=SpecialColor;
}
else if(ColorFlag==2)//this option is used by gametext info panels for buggies showing the text "health" and "weapon"
                    //in black background
{
    FragColor=texture(TextureMap, Texture);
}
else if(ColorFlag==4)//this option is used for rendering skybox
{
    FragColor=texture(SkyBox, SkyBoxTexture);
}
else if(ColorFlag==0)//this is the default value and is used for rendering gameworldcubes and bounding boxes in the
                     //background framebuffer
{
    FragColor=VColor;
}

};
