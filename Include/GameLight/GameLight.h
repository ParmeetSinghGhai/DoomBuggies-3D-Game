#ifndef GameLight_H
#define GameLight_H
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
namespace GameLight
{
extern int TotalLights;
extern void GetDistanceValues(int Distance,float *Lin,float* Quad);

//****************************
// SUNLIGHT DATA STRUCTURE
//****************************
class SunLight
{
public:
GameMath::Vector3 Color;
GameMath::Vector3 Direction;

void UpdateColor(GameMath::Vector3);
void UpdateDirection(GameMath::Vector3);

int DataIndex;
unsigned int LightData_SSBO;
int LightData_Binding;
};

//****************************
// POINTLIGHT DATA STRUCTURE
//****************************
class PointLight
{
public:
GameMath::Vector3 Color;
GameMath::Vector3 Position;

float Constant=1.0f;
float Linear=0.09f;
float Quadratic=0.032f;

void UpdateColor(GameMath::Vector3);
void UpdateColorXYZ(float x,float y,float z);
void UpdatePosition(GameMath::Vector3);
void UpdateDistance(int Distance);
void SetupDistance(int Distance);

int DataIndex;
unsigned int LightData_SSBO;
int LightData_Binding;
};
//****************************
// SPOTLIGHT DATA STRUCTURE
//****************************
class SpotLight
{
public:
GameMath::Vector3 Color;
GameMath::Vector3 Position;
GameMath::Vector3 Direction;
float Constant=1.0f;
float Linear=0.09f;
float Quadratic=0.032f;
float CutOffAngle;
float OuterCutOffAngle;

void UpdateColor(GameMath::Vector3);
void UpdatePosition(GameMath::Vector3);
void UpdateDistance(int Distance);
void UpdateDirection(GameMath::Vector3);
void UpdateCutOffAngle(float DegreeAngle);
void UpdateOuterCutOffAngle(float DegreeAngle);
void SetupDistance(int Distance);

int DataIndex;
unsigned int LightData_SSBO;
int LightData_Binding;
};
};
#endif // GameLight_H
