#ifndef GameBoundingBox_H
#define GameBoundingBox_H
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
#include <vector>
#include <map>
class GameObject;
class GameBoundingBox
{
public:
static int BoundingBoxIndex;
int Index;
int ObjectIndex;

float R=0.0f;
float G=0.0f;
float B=0.0f;

float minx=0.0f;
float maxx=0.0f;
float miny=0.0f;
float maxy=0.0f;
float minz=0.0f;
float maxz=0.0f;

float offset_maxx=0.0f;
float offset_minx=0.0f;
float offset_maxy=0.0f;
float offset_miny=0.0f;
float offset_maxz=0.0f;
float offset_minz=0.0f;

unsigned int VAO;
unsigned int VBO;
float FadeEffect=0.1f;
float ColorEffect=1.0;
bool ColorSwitch=true;
GameMath::Vector3 Centroid;
GameMath::Vector3 ColorTint;
GameMath::Matrix4x4 FinalMatrix;
GameObject* Object=nullptr;

~GameBoundingBox();
GameBoundingBox(float maxx,float minx,float maxy,float miny,float maxz,float minz,int ObjectIndex);
void CalculateXYZ();
void Load();
void DrawBackground();
void DrawForeground();


GameMath::Vector3& operator [](const int Index);
friend std::ostream& operator <<(std::ostream &out,GameBoundingBox &BB)
{
out<<"Bounding Box ::\n";
out<<" Minx="<<BB.minx<<"\n";
out<<" Maxx="<<BB.maxx<<"\n";
out<<" Miny="<<BB.miny<<"\n";
out<<" Maxy="<<BB.maxy<<"\n";
out<<" Minz="<<BB.minz<<"\n";
out<<" Maxz="<<BB.maxz<<"\n";
out<<" Centroid="<<BB.Centroid;
return out;
}

};
#endif // GameBoundingBox_H
