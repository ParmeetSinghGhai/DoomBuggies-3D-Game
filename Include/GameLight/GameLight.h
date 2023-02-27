#ifndef GameLight_H
#define GameLight_H
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
#include <vector>
extern void GetDistanceValues(int Distance,float *Lin,float* Quad);



//****************************
// SUNLIGHT DATA STRUCTURE
//****************************
class SunLight
{
public:
int DataIndex;
GameMath::Vector3 Color;
GameMath::Vector3 Direction;

SunLight();
~SunLight();
void UpdateColor(GameMath::Vector3);
void UpdateDirection(GameMath::Vector3);
};

//****************************
// POINTLIGHT DATA STRUCTURE
//****************************
class PointLight
{
public:
int DataIndex;
GameMath::Vector3 Color;
GameMath::Vector3 Position;

float Constant=1.0f;
float Linear=0.09f;
float Quadratic=0.032f;

PointLight();
~PointLight();
void UpdateColor(GameMath::Vector3);
void UpdateColorXYZ(float x,float y,float z);
void UpdatePosition(GameMath::Vector3);
void UpdateDistance(int Distance);
void SetupDistance(int Distance);
};
//****************************
// SPOTLIGHT DATA STRUCTURE
//****************************
class SpotLight
{
public:
int DataIndex;
GameMath::Vector3 Color;
GameMath::Vector3 Position;
GameMath::Vector3 Direction;

float Constant=1.0f;
float Linear=0.09f;
float Quadratic=0.032f;
float CutOffAngle;
float OuterCutOffAngle;

SpotLight();
~SpotLight();
void UpdateColor(GameMath::Vector3);
void UpdatePosition(GameMath::Vector3);
void UpdateDistance(int Distance);
void UpdateDirection(GameMath::Vector3);
void UpdateCutOffAngle(float DegreeAngle);
void UpdateOuterCutOffAngle(float DegreeAngle);
void SetupDistance(int Distance);
};

//****************************
// GAMELIGHT DATA STRUCTURE
//****************************
class GameLight
{
public:
	static int LightData_Binding;
	static bool ALL_Lights_Loaded;
	static unsigned int LightData_SSBO;
	static std::vector<SunLight*> SunLights;
	static std::vector<PointLight*> PointLights;
	static std::vector<SpotLight*> SpotLights;
	static int GetTotalLightCount();
};
#endif // GameLight_H
