#ifndef GAMEMESH_H
#define GAMEMESH_H
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <GameWindow/GameWindow.h>
#include <GameMath/GameMath.h>
#include <GameBoundingBox/GameBoundingBox.h>

class GameMesh
{
public:
static int MaterialIndex;

struct VertexGroupData
{
std::vector<float> Weights;
};

struct MaterialData
{
int Index;
float AmbientStrength;
int ShininessIndex;
GameMath::Vector3 DiffuseColor;
GameMath::Vector3 SpecularColor;
std::string DiffuseMap="";
std::string AmbientMap="";
std::string SpecularMap="";
MaterialData();
};

struct FaceData
{
int Indices[3];
std::vector<GameMath::Vector2*> UVCoords;
int Material_index=-1;
};

std::string Name;
std::vector<GameMath::Vector3*> Vertices;
std::vector<FaceData*> Faces;
std::vector<MaterialData*> Materials;
std::map<std::string,VertexGroupData*> VertexGroups;
GameBoundingBox* BoundingBox;
void ShowContent();
};

#endif//GAMEMESH_H
