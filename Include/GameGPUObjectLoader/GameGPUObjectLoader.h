#ifndef GameGPUObjectLoader_H
#define GameGPUObjectLoader_H
#include <GameObject/GameObject.h>
#include <GameLight/GameLight.h>
#include <GameBMP/GameBMP.h>

class GameGPUObjectLoader
{
private:
//**************************************
//BONES
//**************************************
struct Bone
{
 int ChildStartIndex;
 int ChildEndIndex;
 int WeightStartIndex;
 int WeightEndIndex;
 float SelfMatrix[4][4];
 float InheritedMatrix[4][4];
 float OutMatrix[4][4];
};
//**************************************
//MATERIALS
//**************************************
struct Material
{
 float AmbientStrength;
 float ShininessIndex;
 int DiffuseMap=0;
 int AmbientMap=0;
 int SpecularMap=0;
 int Padding1=0;
 int Padding2=0;
 int Padding3=0;
 float DiffuseColor[4];
 float SpecularColor[4];
};
//**************************************
//LIGHTS
//**************************************
struct Light
{
int LightType=0;
float Constant=1.0f;
float Linear=0.0f;
float Quadratic=0.0f;
float CutOffAngle=0.0f;
float OuterCutOffAngle=0.0f;
float Padding1=0.0f;
float Padding2=0.0f;
float Color[4]={0.0f,0.0f,0.0f,0.0f};
float Position[4]={0.0f,0.0f,0.0f,1.0f};
float Direction[4]={0.0f,0.0f,0.0f,0.0f};
};
static std::vector<Bone> BoneData;
static std::vector<int> ChildBoneData;
static std::vector<float> WeightData;
static std::vector<Material> MaterialData;
static std::vector<Light> LightData;


public:
static std::vector<GameObject*> Objects;
static std::map<std::string,std::vector<void*>> Lights;
static int Global_BoneIndex;
static int Global_ChildBoneIndex;
static int Global_WeightIndex;


static void AddObject(GameObject* Object);
static void AddObject(GameLight::SunLight* Light);
static void AddObject(GameLight::PointLight* Light);
static void AddObject(GameLight::SpotLight* Light);
static void Load();
};
#endif // GameGPUObjectLoader_H
