#ifndef GAMETEXT_H
#define GAMETEXT_H
#include <GameWindow/GameWindow.h>
#include <GameCamera/GameCamera.h>
#include <GameMath/GameMath.h>
#include <GameBMP/GameBMP.h>

class GameText
{
std::string TextBMP;
unsigned int Texture;
unsigned int VAO;
unsigned int VBO;
GameMath::Matrix4x4 TMatrix;
GameCamera::FPS *Camera;

unsigned int HealthVAO;
unsigned int HealthVBO;
float HealthMeter=0.0f;
GameMath::Vector3 HealthColor=GameMath::Vector3(0.0f,1.0f,0.0f);

unsigned int WeaponVAO;
unsigned int WeaponVBO;
float WeaponMeter=0.0f;
GameMath::Vector3 WeaponColor=GameMath::Vector3(0.0f,1.0f,0.0f);

public:
static int GameTextIndex;
int Index;
int ObjectIndex;
~GameText();
GameText(std::string BMPLocation,GameCamera::FPS *Camera,int ObjectIndex);
void Draw();
void Update(float Health,float Weapon,GameMath::Vector3* Location);
};



#endif // GAMETEXT_H
