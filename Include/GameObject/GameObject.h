#ifndef GameObject_H
#define GameObject_H
#include <GameArmature/GameArmature.h>
#include <GameMesh/GameMesh.h>
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
#include <GameEffects/GameEffects.h>
#include <GameBlenderImport/GameBlenderImport.h>
#include <GameSettings/GameSettings.h>
class GameObject
{
public:
static int ObjectIndex;
GameArmature Armature;
GameMesh Mesh;
GameBoundingBox* BoundingBox;
GameEffects::Bullet* _Bullet=nullptr;
GameEffects::BulletFire* _BulletFire=nullptr;
GameEffects::Explosion* _Explosion=nullptr;

int VAOCount;
int Index;

std::string FilePath;
unsigned int VAO;
unsigned int DiffuseMap=-1;
unsigned int AmbientMap=-1;
unsigned int SpecularMap=-1;

unsigned int BoneData_SSBO;
int BoneData_Binding;
int NoMaterialFlag=-1;
int BoneStartIndex=-1;
int BoneEndIndex=0;

//************************************
// Object Type  1=Player , 2 = Enemy , 3 = HealthBox , 4 = WeaponBox
//************************************
int ObjectType=1;
bool isAlive=true;
bool HALT=false;
float Health=100.0f;

//************************************
// World Movement Properties
//************************************
float Speed=GameSettings::MovementSpeed;//speed in meters per second attained from GameSettings class
float SpeedConstant=((100.0f * 0.16f)/1000.0f);
GameWorldCube* DestinationWorldCube=nullptr;
GameWorldCube* SourceWorldCube=nullptr;

//************************************
// Weapon Attack, Weapon Usage and Health Property
//************************************
GameObject* AttackObject=nullptr;
bool TargetOutOfRange=true;
float WeaponDamageRate=GameSettings::WeaponDamageRate;
float WeaponConsumptionRate=GameSettings::WeaponConsumptionRate;
float WeaponQuantity=100.0f;
float WeaponRange=GameSettings::WeaponRange;
float WeaponRestorationRate = GameSettings::WeaponRestorationRate;
float HealthRestorationRate = GameSettings::HealthRestorationRate;
std::vector<int> VisitedWorldCubes;

//************************************
// Object Movement Properties
//************************************
float WheelRotationAngle=1.0f;
float CannonFireRotationAngle=1.0f;
float BuggyRotationAngle=0.0f;
float CannonRotationAngle=0.0f;

GameMath::Matrix4x4 BuggyRotationMatrix;
GameMath::Matrix4x4 BuggyTranslationMatrix;
GameMath::Matrix4x4 BuggyFinalMatrix;
GameMath::Matrix4x4 CannonRotationMatrix;

GameMath::Vector3 BuggyFront=GameMath::Vector3(-1.0f,0.0f,0.0f);
GameMath::Vector3 CannonFront=GameMath::Vector3(-1.0f,0.0f,0.0f);

GameMath::Vector3 XAxis=GameMath::Vector3(1.0f,0.0f,0.0f);
GameMath::Vector3 YAxis=GameMath::Vector3(0.0f,1.0f,0.0f);
GameMath::Vector3 ZAxis=GameMath::Vector3(0.0f,0.0f,-1.0f);

//************************************
// Object functions
//************************************
GameObject();
GameObject* Find(GameObject** SampleObjects,int ObjectCount);
bool CollisionDetection(GameMath::Vector3 *Point,GameBoundingBox *B2);
void Attack(GameObject** SampleObjects,int ObjectCount,GameObject** SampleInanimateObjects,int InanimateObjectCount);
void WorldMovement(int FrameRate,GameWorldCube** HealthCube,GameWorldCube** WeaponCube);
void Destroy();
void BuggyMovement(GameMath::Vector3* Direction,float Distance);
void CannonMovement(GameMath::Vector3 Target);
void CannonFiring(float AttackDistance);
void Draw();
void SetBoneMatrix(std::string BoneName,GameMath::Matrix4x4 Mat);
};
#endif//GameObject_H


