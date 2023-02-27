#ifndef GameArmature_h
#define GameArmature_h
#include<GameWindow/GameWindow.h>
#include<GameMath/GameMath.h>
#include<GameShader/GameShader.h>
#include<vector>
#include<map>
#include<math.h>
class GameArmature
{
public:
static int BoneIndex;
static int ArmatureIndex;
int Index;
int ObjectIndex;

struct Bone
{
public:
int Index;
int ArmatureIndex;
std::string Name;
GameMath::Vector3 Head;
GameMath::Vector3 Tail;
GameMath::Vector3 XAxis;
GameMath::Vector3 YAxis;
GameMath::Vector3 ZAxis;
std::pair<std::string,Bone*> Parent;
std::map<std::string,Bone*> Child;
Bone(int ArmatureIndex);
~Bone();
};

std::string Name;
std::map<std::string,Bone*> Bones;

~GameArmature();
void ConnectBones();
void ShowContent();
};
#endif // GameArmature_h
