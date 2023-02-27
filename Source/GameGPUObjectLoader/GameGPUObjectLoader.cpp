#include <GameGPUObjectLoader/GameGPUObjectLoader.h>
int GameGPUObjectLoader::Global_ChildBoneIndex=0;
int GameGPUObjectLoader::Global_WeightIndex=0;
int GameGPUObjectLoader::Global_BoneIndex=0;

/**************************************************
THE GAMEGPUOBJECTLOADER LOADS GAMEOBJECTS AND GAMELIGHTS INTO THE OPENGL VERTEX SHADER
AND FRAGMENT SHADERS. THIS CLASS CONTAINS COLLECTIONS OF GAMEOBJECT AND GAMELIGHT OBJECTS
WHICH IS ITERATED OVER WHERE EACH OBJECT IS LOADED INTO THE OPENGL VERTEX SHADER

THE COLLECTION OF GAMEOBJECTS IS NAMED "OBJECTS"
THE COLLECTION OF GAMELIGHTS IS NAMED "LIGHTS"

BESIDES THAT IT SPECIFIES THE FOLLOWING DATA STRUCTURES:
1) BONE:
 int ChildStartIndex;
 int ChildEndIndex;
 int WeightStartIndex;
 int WeightEndIndex;
 float SelfMatrix[4][4];
 float InheritedMatrix[4][4];
 float OutMatrix[4][4];

2) MATERIAL:
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

3) LIGHT:
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

AND THE FOLLOWING COLLECTIONS TO SAVE OBJECTS OF THESE DATA STRUCTURES ALONG WITH OTHER DATA STRUCTURES:
1) TYPE:VECTOR OF "BONE" DATA STRUCTURE, NAME: BONEDATA
2) TYPE:VECTOR OF INTEGER, NAME: CHILDBONEDATA
3) TYPE:VECTOR OF FLOATING POINT VALUES, NAME: WEIGHTDATA
4) TYPE:VECTOR OF "MATERIAL" DATA STRUCTURE, NAME: MATERIALDATA
5) TYPE:VECTOR OF "LIGHT" DATA STRUCTURE, NAME: LIGHTDATA
**************************************************/


/************************************************
THIS FUNCTION LOADS ALL GAMEOBJECTS SAVED IN "OBJECTS" COLLECTION IN THE VERTEX SHADER.
THE PROCEDURE INVOLVED IS AS FOLLOWS:

1) CHECK THE SIZE OF THE "OBJECTS" COLLECTION
2) IF THERE ARE MORE THAN 0 GAMEOBJECTS IN THE "OBJECTS" COLLECTION. IF YES THEN PROCEED FURTHER
3) ITERATE OVER ALL THE GAMEOBJECTS SAVED IN THE "OBJECTS" COLLECTION
4) CHECK IF THE OBJECT'S MESH HAS MORE THAN 0 MATERIALS. IF YES THEN ITERATE OVER ALL THE MATERIALS
   A) CREATE A NEW MATERIAL OBJECT OF TYPE: MATERIAL
   B) LOAD THE FOLLOWING DATA FROM THE MESH'S MATERIAL ONTO THIS NEWLY CREATED MATERIAL OBJECT:
      i) AMBIENT STRENGTH
      ii) SHININESS INDEX
      iii) DIFFUSE COLOR
      iv) SPECULAR COLOR
      v) DIFFUSE MAP
      vi) SPECULAR MAP
      vii) AMBIENT MAP
   C) SAVE THIS NEW MATERIAL OBJECT IN THE "MATERIAL" COLLECTION
5) LOAD ALL THE MESH DATA FROM THE GAMEOBJECT OBJECT IN THE FOLLOWING COLLECTIONS:
   A) VECTOR OF TYPE: FLOAT, NAME: "V" , STORES VERTEX COORDINATES
   B) VECTOR OF TYPE: FLOAT, NAME: "T" , STORES UV COORDINATES
   C) VECTOR OF TYPE: INT, NAME: "M" , STORES MATERIAL INDICES
   D) VECTOR OF TYPE: INT, NAME: "VI" , STORES VERTEX INDICES

   MESH DATA STRUCTURE IS DIVIDED INTO THE FOLLOWING WAYS:
   A) IT CONTAINS FACES WHICH ARE TRIANGLES COMPOSED OF THREE VERTICES
   B) EACH VERTEX HAS A VERTEX INDEX ASSIGNED IT TO THAT UNIQUELY IDENTIFIES IT IN THE MESH
   B) EACH VERTEX HAS A UV COORDINATE ASSIGNED IT WHICH IS A 2D VECTOR COMPOSED OF X AND Y COORDINATES
   C) EACH FACE HAS A MATERIAL INDEX ASSIGNED IT TO THAT UNIQUELY INDENTIFIES THAT MATERIAL

6) COPYING OVER THE MESH' BOUNDING BOX REFERENCE OVER TO THE OBJECT'S BOUNDING BOX POINTER VARIABLE
7) AND THEN ADD A REFERENCE TO THIS GAMEOBJECT IN THIS BOUNDING BOX SO THIS GAMEOBJECT CAN BE REFERENCED DIRECLY
   FROM WITHIN THIS BOUNDING BOX
8) SET THE VERTEX ARRAY OBJECT(VAO) OF THIS GAMEOBJECT AFTER LOADING THE MESH INTO THE VERTEX SHADER
   SO THE OBJECT CAN DRAW THE ENTIRE MESH WITHOUT NEEDING THE MESH OBJECT. ALSO SET THE VAO COUNT VARIABLE TO
   THE TOTAL NUMBER OF VERTICES PRESENT IN THE MESH
9) ITERATE OVER ALL THE BONES IN THE ARMATURE OF THE BONES AND LOAD EACH BONE IN THE COLLECTION: "BONE".
   EACH BONE IS ASSIGNED A GLOBAL INDEX THAT KEEPS INTO ACCOUNT ALL THE OTHER BONES FROM ARMATURES OF OTHER OBJECTS
   EACH CHILDBONE IS ALSO KEPT TRACK OF USING A GLOBAL INDEX THAT TAKES INTO ACCOUNT THE ARMATURES OF OTHER OBJECTS
   EACH VERTEX WEIGHT OF THE VERTEX GROUP IS ALSO KEPT TRACK OF USING A GLOBAL INDEX

   WHILE ITERATING THROUGH ALL THE BONES, THE FOLLOWING PROCEDURE IS FOLLOWED:
   i) ASSIGN A GLOBAL BONE INDEX TO THE BONE THEN INCREMENT THE GLOBAL BONE INDEX BY 1
      , MARK THIS GLOBAL BONE INDEX IF THIS IS THE FIRST BONE OF THE ARMATURE
   ii) SET EMPTY MATRICES FOR THE BONE
   iii) ITERATE THROUGH ALL THE CHILD BONES, ASSIGN A GLOBAL CHILD INDEX FOR THE FIRST CHILD BONE AND MARK IT
        AND THEN INCREMENT THE GLOBAL CHILD BONE INDEX BY 1 WITH EACH CHILD BONE
   iv) MARK THE GLOBAL CHILD BONE INDEX FOR THE LAST CHILD BONE
   v) CREATE A VERTEXGROUP OBJECT FOR THIS BONE
      LOAD THE WEIGHT DATA FOR ALL VERTICES IN THIS VERTEXGROUP OBJECT
      MARK THE GLOBAL WEIGHT INDEX FOR THE FIRST WEIGHT AND THE LAST WEIGHT

10) SAVE THE BONE DATA IN THE "BONE DATA" COLLECTION
11) MARK THE GLOBAL INDEX OF THE LAST BONE FOR THIS OBJECT



THE FOLLOWING COLLECTIONS ARE THEN LOADED INTO THE VERTEX SHADER USING THE FOLLOWING OPENGL FUNCTIONS:
1) TYPE:VECTOR OF "BONE" DATA STRUCTURE, NAME: BONEDATA
2) TYPE:VECTOR OF INTEGER, NAME: CHILDBONEDATA
3) TYPE:VECTOR OF FLOATING POINT VALUES, NAME: WEIGHTDATA
4) TYPE:VECTOR OF "MATERIAL" DATA STRUCTURE, NAME: MATERIALDATA

glGenBuffers - GENERATES BUFFER OBJECTS AND ASSIGN NAMES TO THEM IN THE FORM OF INTEGERS

glBindBuffer - SETS THE CURRENT BUFFER TO THE BUFFER REFEERED TO BY THE SECOND ARGUMENT WHICH IS THE INTEGER NAME OF THE BUFFER OBJECT
               THE FIRST ARGUMENT SPECIFIES THE TYPE OF THE BUFFER BEING USED IN OUR CASE IT WAS " GL_SHADER_STORAGE_BUFFER"

glBindBufferBase - BINDS A BUFFER OBJECT TO AN INDEXED BUFFER TARGET
                   IT ACCEPTS THE FOLLOWING THREE ARGUMENTS:

		   A) TARGET - SPECIFY THE TARGET OF THE BIND OPERATION. TARGET MUST BE
		               ONE OF GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER OR GL_SHADER_STORAGE_BUFFER.
		   B) INDEX - SPECIFY THE INDEX OF THE BINDING POINT SET WITHIN THE VERTEX SHADER FOR THIS BUFFER
		   C) BUFFER - THE NAME OF A BUFFER OBJECT TO BIND TO THE SPECIFIED BINDING POINT. THIS IS THE NAME THAT
		               WE GET FROM glGenBuffers FUNCTION

glBufferData - CREATES AND INITIALIZES A BUFFER OBJECT'S DATA STORE
               IT ACCEPTS THE FOLLOWING THREE ARGUMENTS:

	       A) BUFFER - SPECIFIES THE NAME OF THE BUFFER OBJECT FOR GLNAMEDBUFFERDATA FUNCTION.
	       B) SIZE - SPECIFIES THE SIZE IN BYTES OF THE BUFFER OBJECT'S NEW DATA STORE.
	       C) DATA - SPECIFIES A POINTER TO DATA THAT WILL BE COPIED INTO THE DATA STORE FOR INITIALIZATION, OR NULL IF NO DATA IS TO BE COPIED.
	       D) USAGE - SPECIFIES THE EXPECTED USAGE PATTERN OF THE DATA STORE. THE SYMBOLIC CONSTANT MUST BE GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, OR GL_DYNAMIC_COPY.



AFTER THAT THE BUFFER NAME FOR THE BONE DATA BUFFER OBJECT "BoneData_SSBO" IS SHARED AMONG ALL GAMEOBJECTS
ALONG WITH THE BINDING POINT, WHICH IS AN INTEGER, FOR THAT BUFFER OBJECT

THE ENTIRE PROCESS IS THEN REPEATED FOR ALL THE GAMELIGHT OBJECTS

===============================================
OTHER IMPORTANT OPENGL FUNCTIONS USED HERE ARE:
===============================================

glGenVertexArrays - GENERATE VERTEX ARRAY OBJECT NAMES IN THE FORM OF INTEGER NUMBERS.A VERTEX ARRAY OBJECT (OR VAO) IS
                    AN OBJECT THAT DESCRIBES HOW THE VERTEX ATTRIBUTES ARE STORED IN A VERTEX BUFFER OBJECT (OR VBO).
                    THIS MEANS THAT THE VAO IS NOT THE ACTUAL OBJECT STORING THE VERTEX DATA, BUT THE DESCRIPTOR OF THE VERTEX DATA

glGenBuffers - GENERATE BUFFER OBJECT NAMES IN THE FORM OF INTEGER NUMBERS.A VERTEX BUFFER OBJECT (VBO) IS AN OPENGL
               FEATURE THAT PROVIDES METHODS FOR UPLOADING VERTEX DATA (POSITION, NORMAL VECTOR, COLOR, ETC.)
               TO THE VIDEO DEVICE FOR NON-IMMEDIATE-MODE RENDERING.

glBindVertexArray - GLBINDVERTEXARRAY BINDS THE VERTEX ARRAY OBJECT WITH NAME ARRAY. ARRAY IS THE NAME OF A VERTEX ARRAY OBJECT
                    PREVIOUSLY RETURNED FROM A CALL TO GLGENVERTEXARRAYS, OR ZERO TO BREAK THE EXISTING VERTEX ARRAY OBJECT BINDING.

glBindBuffer - GLBINDBUFFER BINDS A BUFFER OBJECT TO THE SPECIFIED BUFFER BINDING POINT. CALLING GLBINDBUFFER WITH TARGET SET
               TO ONE OF THE ACCEPTED SYMBOLIC CONSTANTS AND BUFFER SET TO THE NAME OF A BUFFER OBJECT BINDS THAT BUFFER OBJECT NAME TO THE TARGET.

glBufferData - GLBUFFERDATA AND GLNAMEDBUFFERDATA CREATE A NEW DATA STORE FOR A BUFFER OBJECT. IN CASE OF GLBUFFERDATA,
               THE BUFFER OBJECT CURRENTLY BOUND TO TARGET IS USED.WHILE CREATING THE NEW STORAGE, ANY PRE-EXISTING DATA STORE IS DELETED.
               THE NEW DATA STORE IS CREATED WITH THE SPECIFIED SIZE IN BYTES AND USAGE. IF DATA IS NOT NULL, THE DATA STORE IS INITIALIZED WITH DATA FROM THIS POINTER.

glVertexAttribPointer - GLVERTEXATTRIBPOINTER SPECIFIES THE LOCATION AND DATA FORMAT OF THE ARRAY OF GENERIC VERTEX ATTRIBUTES AT INDEX INDEX TO USE WHEN RENDERING.
                        SIZE SPECIFIES THE NUMBER OF COMPONENTS PER ATTRIBUTE AND MUST BE 1, 2, 3, 4, OR GL_BGRA. TYPE SPECIFIES THE DATA TYPE OF EACH COMPONENT,
                        AND STRIDE SPECIFIES THE BYTE STRIDE FROM ONE ATTRIBUTE TO THE NEXT, ALLOWING VERTICES AND ATTRIBUTES TO BE PACKED INTO A SINGLE ARRAY OR
                        STORED IN SEPARATE ARRAYS.

glEnableVertexAttribArray - GLENABLEVERTEXATTRIBARRAY AND GLENABLEVERTEXARRAYATTRIB ENABLE THE GENERIC VERTEX ATTRIBUTE ARRAY SPECIFIED BY INDEX.
                            GLENABLEVERTEXATTRIBARRAY USES CURRENTLY BOUND VERTEX ARRAY OBJECT FOR THE OPERATION

glGenTextures - GENERATE TEXTURE NAMES IN THE FORM OF INTEGER NUMBERS

glBindTexture - GLBINDTEXTURE LETS YOU CREATE OR USE A NAMED TEXTURE. CALLING GLBINDTEXTURE WITH TARGET SET TO GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D,
                GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER,
                GL_TEXTURE_2D_MULTISAMPLE OR GL_TEXTURE_2D_MULTISAMPLE_ARRAY AND TEXTURE SET TO THE NAME OF THE NEW TEXTURE BINDS THE TEXTURE NAME TO THE TARGET.
                WHEN A TEXTURE IS BOUND TO A TARGET, THE PREVIOUS BINDING FOR THAT TARGET IS AUTOMATICALLY BROKEN.

glTexParameteri - GLTEXPARAMETER AND GLTEXTUREPARAMETER ASSIGN THE VALUE OR VALUES IN PARAMS TO THE TEXTURE PARAMETER SPECIFIED AS PNAME.
                  FOR GLTEXPARAMETER, TARGET DEFINES THE TARGET TEXTURE, EITHER GL_TEXTURE_1D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY,
                  GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, OR GL_TEXTURE_RECTANGL

glTexImage2D - TEXTURING ALLOWS ELEMENTS OF AN IMAGE ARRAY TO BE READ BY SHADERS. TO DEFINE TEXTURE IMAGES, CALL GLTEXIMAGE2D.
               THE ARGUMENTS DESCRIBE THE PARAMETERS OF THE TEXTURE IMAGE, SUCH AS HEIGHT, WIDTH, WIDTH OF THE BORDER, LEVEL-OF-DETAIL NUMBER (SEE GLTEXPARAMETER),
               AND NUMBER OF COLOR COMPONENTS PROVIDED. THE LAST THREE ARGUMENTS DESCRIBE HOW THE IMAGE IS REPRESENTED IN MEMORY.

glGenerateMipmap - GLGENERATEMIPMAP AND GLGENERATETEXTUREMIPMAP GENERATES MIPMAPS FOR THE SPECIFIED TEXTURE OBJECT. FOR GLGENERATEMIPMAP, THE TEXTURE OBJECT
                   THAT IS BOUND TO TARGET.
************************************************/

void GameGPUObjectLoader::ExtractObjectData(GameObject* Object, std::vector<Material>& MaterialData, std::vector<Bone>& BoneData, std::vector<int>& ChildBoneData, std::vector<float>& WeightData)
{
    if (Object == nullptr)
        return;
    
    Bone* bone = nullptr;
    Material* material = nullptr;
    GameMesh* Mesh = nullptr;
    GameArmature* Armature = nullptr;
    
    Mesh = &Object->Mesh;
    Armature = &Object->Armature;

    //*************************************************************
    // LOAD MATERIAL DATA
    //*************************************************************
    if (Mesh->Materials.size() > 0)
    {
        for (GameMesh::MaterialData* Mat : Mesh->Materials)
        {
            material = new GameGPUObjectLoader::Material();
            material->AmbientStrength = Mat->AmbientStrength;
            material->ShininessIndex = Mat->ShininessIndex;
            material->DiffuseColor[0] = Mat->DiffuseColor.x;
            material->DiffuseColor[1] = Mat->DiffuseColor.y;
            material->DiffuseColor[2] = Mat->DiffuseColor.z;
            material->DiffuseColor[3] = 0.0f;
            material->SpecularColor[0] = Mat->SpecularColor.x;
            material->SpecularColor[1] = Mat->SpecularColor.y;
            material->SpecularColor[2] = Mat->SpecularColor.z;
            material->SpecularColor[3] = 0.0f;
            material->DiffuseMap = Object->DiffuseMap;
            material->AmbientMap = Object->AmbientMap;
            material->SpecularMap = Object->SpecularMap;
            MaterialData.push_back(*material);
        }
    }
    //*************************************************************
    // LOADING ARMATURE AND VERTEX WEIGHT DATA
    //*************************************************************
    // ASSIGNING UNIQUE INDICES TO BONES AS PER THEIR POSITION
    // IN THE BONE SHARED BUFFER OBJECT IN THE VERTEX SHADER
    for (std::map<std::string, GameArmature::Bone*>::iterator boneit = Armature->Bones.begin(); boneit != Armature->Bones.end(); boneit++)
    {
        boneit->second->Index = Global_BoneIndex;
        Global_BoneIndex++;
    }

    unsigned int BoneCount = 0;
    for (std::map<std::string, GameArmature::Bone*>::iterator boneit = Armature->Bones.begin(); boneit != Armature->Bones.end(); boneit++)
    {
        //MARK THE GLOBAL INDEX OF THE FIRST BONE FOR THIS OBJECT
        if (BoneCount == 0)
            Object->BoneStartIndex = boneit->second->Index;

        //CREATE EMPTY MATRICES FOR THE BONE
        bone = new GameGPUObjectLoader::Bone();
        GameMath::Transform::CopyMatrixtoArray(nullptr, &bone->SelfMatrix[0][0]);
        GameMath::Transform::CopyMatrixtoArray(nullptr, &bone->InheritedMatrix[0][0]);
        GameMath::Transform::CopyMatrixtoArray(nullptr, &bone->OutMatrix[0][0]);

        //MARK THE GLOBAL INDEX OF THE FIRST CHILD BONE FOR THIS OBJECT
        bone->ChildStartIndex = Global_ChildBoneIndex;
        for (std::map<std::string, GameArmature::Bone*>::iterator childit = (boneit->second)->Child.begin(); childit != (boneit->second)->Child.end(); childit++)
        {
            ChildBoneData.push_back(childit->second->Index);
            Global_ChildBoneIndex++;
        }
        //MARK THE GLOBAL INDEX OF THE LAST CHILD BONE FOR THIS OBJECT
        bone->ChildEndIndex = Global_ChildBoneIndex;

        //CREATE A VERTEX GROUP DATA FOR THIS BONE IN THIS OBJECT
        GameMesh::VertexGroupData* vertexgroup = Mesh->VertexGroups[boneit->first];

        //MARK THE GLOBAL INDEX OF THE FIRST WEIGHT DATA FOR THIS BONE IN THIS OBJECT
        bone->WeightStartIndex = Global_WeightIndex;
        for (float Weight : vertexgroup->Weights)
        {
            WeightData.push_back(Weight);
            Global_WeightIndex++;
        }
        bone->WeightEndIndex = Global_WeightIndex;
        //MARK THE GLOBAL INDEX OF THE LAST WEIGHT DATA FOR THIS BONE IN THIS OBJECT

        //SAVE THE NEW BONE IN THE COLLECTION: "BONE DATA"
        BoneData.push_back(*bone);

        //MARK THE GLOBAL INDEX OF THE LAST BONE FOR THIS OBJECT
        if (BoneCount == Armature->Bones.size() - 1)
            Object->BoneEndIndex = boneit->second->Index;

        BoneCount++;
    }

}

void GameGPUObjectLoader::LoadObjects()
{
    if (!GameObject::ALL_Objects_Loaded)
    {
        GameGPUObjectLoader::Global_ChildBoneIndex = 0;
        GameGPUObjectLoader::Global_WeightIndex = 0;
        GameGPUObjectLoader::Global_BoneIndex = 0;
        
        std::vector<Material> MaterialData;
        std::vector<Bone> BoneData;
        std::vector<int> ChildBoneData;
        std::vector<float> WeightData;
        
        for (GameObject* Object : GameObject::Players)
            ExtractObjectData(Object, MaterialData, BoneData, ChildBoneData, WeightData);
        for (GameObject* Object : GameObject::Enemies)
            ExtractObjectData(Object, MaterialData, BoneData, ChildBoneData, WeightData);
        for (GameObject* Object: GameObject::Inanimates)
            ExtractObjectData(Object, MaterialData, BoneData, ChildBoneData, WeightData);

        ExtractObjectData(GameObject::RealWorld, MaterialData, BoneData, ChildBoneData, WeightData);
        ExtractObjectData(GameObject::HealthBox, MaterialData, BoneData, ChildBoneData, WeightData);
        ExtractObjectData(GameObject::WeaponBox, MaterialData, BoneData, ChildBoneData, WeightData);
        
        //*************************
        // MATERIAL DATA BINDING
        //*************************
        if (MaterialData.size() > 0)
        {
            glGenBuffers(1, &GameObject::ALL_MaterialData_SSBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameObject::ALL_MaterialData_SSBO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GameObject::MaterialData_Binding, GameObject::ALL_MaterialData_SSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, MaterialData.size() * sizeof(Material), &MaterialData[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            std::cout << "LOADED ALL MATERIALS, TOTAL::"+std::to_string(MaterialData.size())+"\n";
            MaterialData.clear();
        }
        //*************************
        // BONE DATA BINDING
        //*************************
        if (BoneData.size() > 0)
        {
            glGenBuffers(1, &GameObject::ALL_BoneData_SSBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameObject::ALL_BoneData_SSBO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GameObject::BoneData_Binding, GameObject::ALL_BoneData_SSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, BoneData.size() * sizeof(Bone), &BoneData[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            std::cout << "LOADED ALL BONES, TOTAL::" + std::to_string(BoneData.size()) + "\n";
            BoneData.clear();
        }
        //*************************
        // CHILD DATA BINDING
        //*************************
        if (ChildBoneData.size() > 0)
        {
            glGenBuffers(1, &GameObject::ALL_ChildBoneData_SSBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameObject::ALL_ChildBoneData_SSBO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GameObject::ChildBoneData_Binding, GameObject::ALL_ChildBoneData_SSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, ChildBoneData.size() * sizeof(int), &ChildBoneData[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            std::cout << "LOADED ALL CHILD BONES, TOTAL::" + std::to_string(ChildBoneData.size()) + "\n";
            ChildBoneData.clear();
        }
        //*************************
        // WEIGHT DATA BINDING
        //*************************
        if (WeightData.size() > 0)
        {
            glGenBuffers(1, &GameObject::ALL_WeightData_SSBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameObject::ALL_WeightData_SSBO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GameObject::WeightData_Binding, GameObject::ALL_WeightData_SSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, WeightData.size() * sizeof(float), &WeightData[0], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            std::cout << "LOADED ALL WEIGHTS, TOTAL::" + std::to_string(WeightData.size()) + "\n";
            WeightData.clear();
        }
        std::cout << "ALL OBJECT DATA LOADED INTO GPU\n";
        GameObject::ALL_Objects_Loaded = true;
    }
}

void GameGPUObjectLoader::UnloadObjects()
{
    if (GameObject::ALL_Objects_Loaded)
    {
        GameGPUObjectLoader::Global_ChildBoneIndex = 0;
        GameGPUObjectLoader::Global_WeightIndex = 0;
        GameGPUObjectLoader::Global_BoneIndex = 0;

        glDeleteBuffers(1, &GameObject::ALL_MaterialData_SSBO);
        glDeleteBuffers(1, &GameObject::ALL_BoneData_SSBO);
        glDeleteBuffers(1, &GameObject::ALL_ChildBoneData_SSBO);
        glDeleteBuffers(1, &GameObject::ALL_WeightData_SSBO);
        std::cout << "ALL OBJECT DATA UNLOADED FROM GPU\n";
        GameObject::ALL_Objects_Loaded = false;
    }
}

void GameGPUObjectLoader::LoadLights()
{
    GameGPUObjectLoader::Light* light = nullptr;
    std::vector<Light> LightData;
    if (!GameLight::ALL_Lights_Loaded)
    {
        for (SunLight* sunlight : GameLight::SunLights)
        {
            light = new Light();
            light->LightType = 0;

            light->Color[0] = sunlight->Color[0];
            light->Color[1] = sunlight->Color[1];
            light->Color[2] = sunlight->Color[2];
            light->Color[3] = 0.0f;

            light->Direction[0] = sunlight->Direction[0];
            light->Direction[1] = sunlight->Direction[1];
            light->Direction[2] = sunlight->Direction[2];
            light->Direction[3] = -1.0f;

            sunlight->DataIndex = LightData.size() * sizeof(Light);
            LightData.push_back(*light);
        }
        for (PointLight* pointlight : GameLight::PointLights)
        {
            light = new Light();

            light->LightType = 1;

            light->Color[0] = pointlight->Color[0];
            light->Color[1] = pointlight->Color[1];
            light->Color[2] = pointlight->Color[2];
            light->Color[3] = 0.0f;

            light->Position[0] = pointlight->Position[0];
            light->Position[1] = pointlight->Position[1];
            light->Position[2] = pointlight->Position[2];
            light->Position[3] = 1.0f;

            light->Constant = pointlight->Constant;
            light->Linear = pointlight->Linear;
            light->Quadratic = pointlight->Quadratic;

            pointlight->DataIndex = LightData.size() * sizeof(Light);
            LightData.push_back(*light);
        }
        for (SpotLight* spotlight : GameLight::SpotLights)
        {
            light = new Light();

            light->LightType = 2;

            light->Color[0] = spotlight->Color[0];
            light->Color[1] = spotlight->Color[1];
            light->Color[2] = spotlight->Color[2];
            light->Color[3] = 0.0f;

            light->Position[0] = spotlight->Position[0];
            light->Position[1] = spotlight->Position[1];
            light->Position[2] = spotlight->Position[2];
            light->Position[3] = 1.0f;

            light->Direction[0] = spotlight->Direction[0];
            light->Direction[1] = spotlight->Direction[1];
            light->Direction[2] = spotlight->Direction[2];
            light->Direction[3] = 0.0f;

            light->Constant = spotlight->Constant;
            light->Linear = spotlight->Linear;
            light->Quadratic = spotlight->Quadratic;

            light->CutOffAngle = cos(GameMath::Transform::GetRadian(spotlight->CutOffAngle));
            light->OuterCutOffAngle = cos(GameMath::Transform::GetRadian(spotlight->OuterCutOffAngle));

            spotlight->DataIndex = LightData.size() * sizeof(Light);
            LightData.push_back(*light);
        }
        //*************************
        // Light Data Binding
        //*************************
        glGenBuffers(1, &GameLight::LightData_SSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameLight::LightData_SSBO);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GameLight::LightData_Binding, GameLight::LightData_SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, LightData.size() * sizeof(Light), &LightData[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        std::cout << "LOADED ALL LIGHTS, TOTAL::" + std::to_string(LightData.size()) + "\n";
        LightData.clear();
        GameLight::ALL_Lights_Loaded = true;
    }
}

void GameGPUObjectLoader::UnloadLights()
{
    if (GameLight::ALL_Lights_Loaded)
    {
        glDeleteBuffers(1, &GameLight::LightData_SSBO);
        std::cout << "ALL LIGHT DATA UNLOADED FROM GPU\n";
        GameLight::ALL_Lights_Loaded = false;
    }
}
