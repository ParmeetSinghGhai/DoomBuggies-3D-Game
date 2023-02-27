#ifndef GameObject_H
#define GameObject_H
#include <GameArmature/GameArmature.h>
#include <GameMesh/GameMesh.h>
#include <GameWindow/GameWindow.h>
#include <GameShader/GameShader.h>
#include <cstring>
#include <GameBoundingBox/GameBoundingBox.h>
#include <GameCamera/GameCamera.h>
#include <GameWorld/GameWorld.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <algorithm>
#include <GameBMP/GameBMP.h>
#include <GameEffects/GameEffects.h>
#include <GameText/GameText.h>
#include <GameBlenderImport/GameBlenderImport.h>
#include <GameSettings/GameSettings.h>
class GameObject
{
public:
static int ObjectIndex;
static unsigned int ALL_MaterialData_SSBO;
static unsigned int ALL_WeightData_SSBO;
static unsigned int ALL_ChildBoneData_SSBO;
static unsigned int ALL_BoneData_SSBO;
static int WeightData_Binding;
static int ChildBoneData_Binding;
static int BoneData_Binding;
static int MaterialData_Binding;
static bool ALL_Objects_Loaded;
static std::vector<GameObject*> Players;
static std::vector<GameObject*> Enemies;
static std::vector<GameObject*> Inanimates;
static GameObject* HealthBox;
static GameObject* WeaponBox;
static GameObject* RealWorld;
static GameObject* SelectedPlayer;
static GameObject* LastSelectedPlayer;
static GameObject* SelectedEnemy;
static GameWorldCube* SelectedWorldCube;


int VAOCount;
int Index;
GameArmature Armature;
GameMesh Mesh;
GameText* Info=nullptr;
GameBoundingBox* BoundingBox=nullptr;
GameEffects::Bullet* _Bullet=nullptr;
GameEffects::BulletFire* _BulletFire=nullptr;
GameEffects::Explosion* _Explosion=nullptr;

std::string FilePath;
unsigned int VAO = 0;
unsigned int VVBO = 0;
unsigned int VIVBO = 0;
unsigned int TVBO = 0;
unsigned int MVBO = 0;
unsigned int DiffuseMap=0;
unsigned int AmbientMap=0;
unsigned int SpecularMap=0;

int NoMaterialFlag=1;//1 = true, object has no material, 0 = false, object has material
int BoneStartIndex=0;
int BoneEndIndex=0;

//************************************
// Object Type  1=Player , 2 = Enemy , 3 = HealthBox , 4 = WeaponBox
//************************************
int ObjectType=1;
bool isAlive=true;
bool PlayerTakenControl = false;
bool HALT=false;


//************************************
// World Movement Properties
//************************************
float Speed=GameSettings::DefaultObjectSpeed;//meters per second
float SpeedConstant=((100.0f * 0.16f)/1000.0f);
GameWorldCube* DestinationWorldCube=nullptr;
GameWorldCube* SourceWorldCube=nullptr;

//************************************
// Weapon Attack, Weapon Usage and Health Property
//************************************
GameObject* AttackObject=nullptr;
bool TargetOutOfRange=true;
float WeaponQuantity=100.0f;
float Health = 100.0f;

//************************************
// Object and Cannon Movement Properties
//************************************
float TurnAngle = 0.0f;
float FrontWheelRotationAngle=1.0f;
float BackWheelRotationAngle = 1.0f;
float CannonFireRotationAngle=1.0f;
float BuggyRotationAngle=0.0f;
float CannonRotationAngle=0.0f;
float LastCannonRotationAngle = 0.0f;
float CannonRotationAngle_Buffer = 0.0f;

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
~GameObject();
GameObject* AISearchTarget();
void Load();
void Unload();
static GameWorldCube* FindEmptyNeighbour(GameWorldCube* Source);
static GameWorldCube* FindClosestEmptyNeighbour(GameWorldCube* Source, GameWorldCube* Destination);
bool CollisionDetection(GameMath::Vector3 *Point,GameBoundingBox *B2);

void FrontWheelTurn(float Angle);
void WheelRotation(bool FrontWheel_Clockwise,bool BackWheel_Clockwise,int TurnDirection);
void PlayerControlledWorldMovement(int FrameRate,GameCamera::FPS &FPSCamera);
void PlayerControlledAttack(GameCamera::FPS& FPSCamera);
void PlayerControlledCannonFiring();
void Attack();
void WorldMovement(int FrameRate);


void Destroy();
void BuggyMovement(GameMath::Vector3* Direction,float Distance);
void CannonMovement(GameMath::Vector3 Target);
void CannonFiring(float AttackDistance);
void Draw();
void SetBoneMatrix(std::string BoneName,GameMath::Matrix4x4 Mat);
};
#endif//GameObject_H


