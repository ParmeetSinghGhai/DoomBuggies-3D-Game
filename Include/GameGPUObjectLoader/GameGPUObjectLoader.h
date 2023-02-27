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
		int DiffuseMap = 0;
		int AmbientMap = 0;
		int SpecularMap = 0;
		int Padding1 = 0;
		int Padding2 = 0;
		int Padding3 = 0;
		float DiffuseColor[4];
		float SpecularColor[4];
	};
	//**************************************
	//LIGHTS
	//**************************************
	struct Light
	{
		int LightType = 0;
		float Constant = 1.0f;
		float Linear = 0.0f;
		float Quadratic = 0.0f;
		float CutOffAngle = 0.0f;
		float OuterCutOffAngle = 0.0f;
		float Padding1 = 0.0f;
		float Padding2 = 0.0f;
		float Color[4] = { 0.0f,0.0f,0.0f,0.0f };
		float Position[4] = { 0.0f,0.0f,0.0f,1.0f };
		float Direction[4] = { 0.0f,0.0f,0.0f,0.0f };
	};

public:
	static int Global_BoneIndex;
	static int Global_ChildBoneIndex;
	static int Global_WeightIndex;

	static void ExtractObjectData(GameObject* Object, std::vector<Material>& MaterialData, std::vector<Bone>& BoneData, std::vector<int>& ChildBoneData, std::vector<float>& WeightData);
	static void LoadObjects();
	static void UnloadObjects();
	static void LoadLights();
	static void UnloadLights();
};
#endif // GameGPUObjectLoader_H
