#ifndef GameBlenderImport_h
#define GameBlenderImport_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
#include <GameShader/GameShader.h>
#include <GameBMP/GameBMP.h>
#include <GameArmature/GameArmature.h>
#include <GameMesh/GameMesh.h>
#include <GameObject/GameObject.h>
#include <GameText/GameText.h>
#include <GameEffects/GameEffects.h>
class GameBlenderImport
{
public:
//***************
// MAIN FUNCTIONS
//***************
static GameObject* Import(std::string filepath);
static float* ImportEffects(std::string filepath,int* TriangleCount);


private:
//****************************************
// PRIVATE MEMBERS VARIABLES AND FUNCTIONS
//****************************************
static std::ifstream Binaryfile;
static std::string FilePath;
static char *byte;
static char *bytef;
static char *bytes;

//***************
// HELPER  FUNCTIONS
//***************
static int Geti();
static float Getf();
static std::string Gets();
static void Getv(GameMath::Vector3* vec);
static void Getbb(float* maxx,float* minx,float* maxy,float* miny,float* maxz,float* minz);
};
#endif // GameBlenderImport_h
