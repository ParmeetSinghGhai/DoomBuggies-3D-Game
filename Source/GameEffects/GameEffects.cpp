#include <GameEffects/GameEffects.h>
/*************************************************
THIS FUNCTION LOADS THE BULLER MESH DATA AND TEXTURE DATA IN GPU MEMORY USING THE HELPER SHADER
*************************************************/
void GameEffects::Bullet::Load()
{
unsigned int VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (TriangleCount* 3 * 5), &Vertices[0], GL_STATIC_DRAW);
VertexCount=TriangleCount * 3;

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(7);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int Width=0;
int Height=0;
unsigned char* ImageData=nullptr;
ImageData=GameBMP::LoadBMP(TextureLocation.c_str(),&Width,&Height,"Object Bullet Image");
if(ImageData!=nullptr)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);
  GameBMP::FreeBMP(ImageData);
}
}
/*************************************************
THIS FUNCTION LOADS THE BULLER FIRE MESH DATA AND TEXTURE DATA IN GPU MEMORY USING THE HELPER SHADER
*************************************************/
void GameEffects::BulletFire::Load()
{
unsigned int VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (TriangleCount * 3 * 5), Vertices, GL_STATIC_DRAW);
VertexCount=TriangleCount * 3;

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(7);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int Width=0;
int Height=0;
unsigned char* ImageData=nullptr;
ImageData=GameBMP::LoadBMP(TextureLocation.c_str(),&Width,&Height,"Object Bullet Fire Image");
if(ImageData!=nullptr)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);
  GameBMP::FreeBMP(ImageData);
}
}
/*************************************************
THIS FUNCTION LOADS THE OBJECT EXPLOSION MESH DATA AND TEXTURE DATA IN GPU MEMORY USING THE HELPER SHADER
*************************************************/
void GameEffects::Explosion::Load()
{
unsigned int VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (TriangleCount * 3 * 5), Vertices, GL_STATIC_DRAW);
VertexCount=TriangleCount * 3;

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(7);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int Width=0;
int Height=0;
unsigned char* ImageData=nullptr;
ImageData=GameBMP::LoadBMP(TextureLocation.c_str(),&Width,&Height,"Object Explosion Image");
if(ImageData!=nullptr)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);
  GameBMP::FreeBMP(ImageData);
}

}
/*************************************************
THIS FUNCTION LOADS THE HELP SECTION OBJECT MESH DATA AND TEXTURE DATA IN GPU MEMORY USING THE HELPER SHADER
*************************************************/
void GameEffects::HelpSection::Load()
{
unsigned int VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (TriangleCount* 3 * 5), &Vertices[0], GL_STATIC_DRAW);
VertexCount=TriangleCount * 3;

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(7);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int Width=0;
int Height=0;
unsigned char* ImageData=nullptr;
ImageData=GameBMP::LoadBMP(TextureLocation.c_str(),&Width,&Height,"Object Bullet Image");
if(ImageData!=nullptr)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);
  GameBMP::FreeBMP(ImageData);
}
}





/*************************************************
THIS FUNCTION ROTATES AND TRANSLATES THE BULLET OBJECT THAT IS ASSIGNED TO A BUGGY OBJECT IN THE FOLLOWING WAY:

1) IT APPLIES THE CANNON' ROTATION MATRIX SO THE BULLET IS POINTING IN THE SAME DIRECTION THAT THE BUGGY'S CANNON IS
   POINTING AT
2) THEN IT TRANSLATES THE BULLET ONE FLOATING POINT AWAY FROM THE CANNON POSITION WHICH IS THE LOCATION OF THE
   "MACHINE GUN" BONE HEAD VECTOR AFTER IT HAS TRANSLATED / ROTATED ALONG WITH BUGGY
3) AFTER THAT FROM THE ABOVE TRANSLATED POSITION, IT TRANSLATES A CERTAIN PERCENTAGE OF THE ATTACK DISTANCE
   IN THE ATTACK DIRECTION
4) THE BULLET OBJECT IS THEN RENDERED ON THE SCREEN
5) THE "ISALIVE" VARIABLE IS SET TO FALSE SO IT DOES NOT RENDER ON THE NEXT FRAME UNLESS THE ATTACK COMMAND
   HAS BEEN INITIATED
6) IT RETURNS THE MATRIX OBTAINED AT STEP (2) ABOVE SO THE BULLET FIRING OBJECT CAN ALSO FACE IN THE
   SAME DIRECTION THAT THE CANNON IS POINTING AT AND DISPLAY / RENDER AT ONE FLOATING POINT AWAY FROM THAT POSITION
*************************************************/
GameMath::Matrix4x4 GameEffects::Bullet::Draw()
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);
GameMath::Matrix4x4 T= GameMath::Transform::Translate(CannonPosition + (CannonFront * 1.0f));
glUniformMatrix4fv(2, 1, GL_FALSE,(TranslationMatrix * T * RotationMatrix).Addressof());
glUniform1i(5,2);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, VertexCount);
isAlive=false;
return T * RotationMatrix;
}
/*************************************************
THIS FUNCTION ROTATES AND SCALES THE BULLET FIRING OBJECT THAT IS ACCOMPANIED WITH THE BULLET OBJECT:

1) IT SCALES THE BULLET FIRE OBJECT ACCORDING TO THE VALUE OF THE "BulletFireRate" VARIABLES WHICH INCREMENTS
   BY 0.2 FROM 0.0 TO 1.0 AND AS SOON AS IT REACHES 1.0, IT RESETS TO 0.0
2) THEN IT ROTATES THE BULLET FIRE OBJECT ALONG THE WORLD X AXIS BY THE VALUE OF THE "BulletFireRotationRate"
   VARIABLE TO MAKE IT LOOK LIKE AS IF ITS ROTATING ABOUT ITS CENTER OF ROTATION
3) AFTER THAT IT IS TRANSLATED TO ONE FLOATING POINT AWAY FROM THE CANNON POSITION AND NO FURTHER THAN THAT
4) THE BULLET FIRE OBJECT IS THEN RENDERED ON THE SCREEN
*************************************************/
void GameEffects::BulletFire::Draw(GameMath::Matrix4x4 BulletMatrix)
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);

if(BulletFireRate>1.0f)
BulletFireRate=0.0f;
else
BulletFireRate=BulletFireRate+0.2f;

if(BulletFireRotationRate > 360.0)
BulletFireRotationRate=0.0f;
else
BulletFireRotationRate+=5.0f;

glUniformMatrix4fv(2, 1, GL_FALSE,(BulletMatrix * GameMath::Transform::RotateQ(XAxis,BulletFireRotationRate) * GameMath::Transform::Scale(BulletFireRate,BulletFireRate,BulletFireRate)).Addressof());
glUniform1i(5,2);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}
/*************************************************
THIS FUNCTION TRANSLATES AND SCALES THE EXPLOSION OBJECT:

1) IT SCALES THE EXPLOSION OBJECT ACCORDING TO THE VALUE OF THE "ExplosionRate" VARIABLES WHICH INCREMENTS
   BY 0.01 FROM 0.0 TO 1.5. THE Y COMPONENT OF THIS TRANSFORMATION INCREMENTS MORE THAN THE X AND Z COMPONENTS
   TO MAKE THE EXPLOSION EXPAND FAR MORE VERTICALLY THAN HORIZONTALLY
2) AFTER THAT IT IS TRANSLATED TO THE CENTER OF THE BUGGY OBJECT TO MAKE IT LOOK LIKE AS IF THE EXPLOSION STARTED FROM
   THE CENTER OF THE OBJECT
3) THE EXPLOSION OBJECT IS THEN RENDERED ON THE SCREEN
4) IF THE "isCameraShaking" IS SET TO TRUE THEN APPLY THE SHAKE EFFECT TO THE CAMERA
4) AS SOON AS "ExplosionRate" REACHES 1.5, THE "ISALIVE" VARIABLE IS SET TO FALSE AFTER WHICH THE EXPLOSION OBJECT
   IS NOT RENDERED ON THE SCREEN ANYMORE
   AND THE GLOBAL "ISCAMERASHAKING" FLAG IS SET TO FALSE AS WELL SO OTHER EXPLOSION OBJECTS CAN GET TO SET THIS VARIABLE
   TRUE AND APPLY SHAKING EFFECTS TO THE CAMERA POSITION AS ONLY ONE EXPLOSION OBJECT CAN DO THAT TO THE CAMERA AT A TIME
*************************************************/
void GameEffects::Explosion::Draw(GameCamera::FPS *Camera, bool* isCameraShaking_GlobalFlag)
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);

if(ExplosionRate > 1.5f)
{
    isExploding=false;
    isCameraShaking=false;
    (*isCameraShaking_GlobalFlag)=false;//SET THE GLOBAL "ISCAMERASHAKING" FLAG TO TRUE SO
                                        //OTHER EXPLOSION OBJECTS CAN MAKE THE CAMERA SHAKE AS WELL
}
else
isExploding=true;


if(CameraSwitch==false && isCameraShaking==true)
{
    Camera->CameraPosition.x+=0.5f;
    Camera->CameraPosition.y-=0.2f;
    CameraSwitch=true;
}
else if(CameraSwitch==true && isCameraShaking==true)
{
    Camera->CameraPosition.x-=0.5f;
    Camera->CameraPosition.y+=0.2f;
    CameraSwitch=false;
}

ExplosionRate=ExplosionRate+0.01f;
glUniformMatrix4fv(2, 1, GL_FALSE,(GameMath::Transform::Translate(BBCentroid) * GameMath::Transform::Scale(ExplosionRate,ExplosionRate+0.5f,ExplosionRate)).Addressof());
glUniform1i(5,2);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0,VertexCount);
}
/*************************************************
THIS FUNCTION SIMPLY DRAWS THE HELP SECTION OBJECT ON THE SCREEN
*************************************************/
void GameEffects::HelpSection::Draw()
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);
glUniform1i(5,2);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0,VertexCount);
}


