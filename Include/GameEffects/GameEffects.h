#ifndef GameEffects_H
#define GameEffects_H
#include <GameWindow/GameWindow.h>
#include <GameShader/GameShader.h>
#include <cstring>
#include <GameBoundingBox/GameBoundingBox.h>
#include <GameWorld/GameWorld.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <algorithm>
#include <GameBMP/GameBMP.h>
#include <GameMath/GameMath.h>
#include <GameCamera/GameCamera.h>
#include <GameSettings/GameSettings.h>
class GameEffects
{
public:
class Bullet
{
public:
static int BulletIndex;
int Index;
int ObjectIndex;

float* Vertices;
int TriangleCount;
int VertexCount;
std::string TextureLocation;
float DistanceRate;
GameMath::Vector3 CannonPosition=GameMath::Vector3(-0.2f,0.4f,0.0f);
GameMath::Vector3 CannonFront=GameMath::Vector3(-1.0f,0.0f,0.0f);
GameMath::Vector3 YAxis=GameMath::Vector3(0.0f,1.0f,0.0f);
GameMath::Matrix4x4 TranslationMatrix;
GameMath::Matrix4x4 RotationMatrix;
bool isAlive=false;

unsigned int VAO;
unsigned int VBO;
unsigned int Texture;
Bullet(int ObjectIndex);
~Bullet();
void Load();
GameMath::Matrix4x4 Draw();
};




class BulletFire
{
public:
static int BulletFireIndex;
int Index;
int ObjectIndex;

float* Vertices;
int TriangleCount;
int VertexCount;
std::string TextureLocation;
GameMath::Vector3 XAxis=GameMath::Vector3(1.0f,0.0f,0.0f);
float BulletFireRate=0.1f;
float BulletFireRotationRate=5.0f;

unsigned int VAO;
unsigned int VBO;
unsigned int Texture;
BulletFire(int ObjectIndex);
~BulletFire();
void Load();
void Draw(GameMath::Matrix4x4 BulletMatrix);
};




class Explosion
{
public:
static int ExplosionIndex;
int Index;
int ObjectIndex;

float* Vertices;
int TriangleCount;
int VertexCount;
std::string TextureLocation;
GameMath::Vector3 BBCentroid;
float ExplosionRate=0.5f;
bool isExploding=false;
bool isCameraShaking=false;
bool CameraSwitch=false;

unsigned int VAO;
unsigned int VBO;
unsigned int Texture;
Explosion(int ObjectIndex);
~Explosion();
void Load();
void Draw(GameCamera::FPS *Camera,bool* isCameraShaking_GlobalFlag);
};



class HelpSection
{
public:
float* Vertices;
int TriangleCount=0;
int VertexCount=0;
int State = 0;

std::string TextureLocations[9];
unsigned int Texture[9];
unsigned int VAO;
unsigned int VBO;

~HelpSection();
void Load();
void Draw();
};

};


#endif
