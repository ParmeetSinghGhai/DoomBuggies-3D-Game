#ifndef GameWorld_H
#define GameWorld_H
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
#include <GameBMP/GameBMP.h>
#include <GameShader/GameShader.h>
#include <map>
#include <vector>
class GameObject;

class SkyBox
{
public:
static float vertices[];
unsigned int VAO;
unsigned int Texture;
int VAOCount;

SkyBox(std::string FolderPath);
void Draw();
};





class GameWorld
{
public:
static int Index;
int SmallestDivision=2;
int WorldVAOCount;
unsigned int WorldVAO;
float minx;
float maxx;
float miny;
float maxy;
float minz;
float maxz;
int xbound;
int zbound;

std::vector<float> WorldVBOBuffer;
std::map<int,std::map<int,GameWorldCube*>> WorldCoordMap;

GameWorld(float minx,float maxx,float miny,float maxy,float minz,float maxz);
GameWorldCube* MakeCube(int z,int x,float minx,float minz,float maxy);
void Draw();
};

class GameWorldCube
{
public:
int Index;
GameMath::Vector3 Centroid;
GameObject* Object=nullptr;
GameObject* HealthObject=nullptr;
GameObject* WeaponObject=nullptr;
GameWorldCube* Neighbours[8];
float R=0.0f;
float G=0.0f;
float B=0.0f;
float ColorEffect=1.0f;
bool ColorSwitch=true;
GameMath::Vector3 ColorTint;

unsigned int VAO=-1;
unsigned int VBO=-1;
void Load(std::vector<float>* VBOBuffer,int SmallestDivision);
void Load(int SmallestDivision);
void DrawForeground();
void Free();
};
#endif
