#include <GameGPUObjectLoader/GameGPUObjectLoader.h>
std::vector<GameObject*> GameGPUObjectLoader::Objects;
std::map<std::string,std::vector<void*>> GameGPUObjectLoader::Lights;

std::vector<GameGPUObjectLoader::Bone> GameGPUObjectLoader::BoneData;
std::vector<GameGPUObjectLoader::Light> GameGPUObjectLoader::LightData;
std::vector<GameGPUObjectLoader::Material> GameGPUObjectLoader::MaterialData;
std::vector<int> GameGPUObjectLoader::ChildBoneData;
std::vector<float> GameGPUObjectLoader::WeightData;

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
THIS FUNCTION SAVES A GAMEOBJECT IN THE "OBJECTS" COLLECTION OF GAMEOBJECT OBJECTS
************************************************/
void GameGPUObjectLoader::AddObject(GameObject* Object)
{
Objects.push_back(Object);
}
/************************************************
THIS FUNCTION SAVES A GAMELIGHT OF TYPE:SUNLIGHT IN THE "LIGHTS" COLLECTION OF GAMELIGHT OBJECTS
USING THE KEY "SUNLIGHT"
************************************************/
void GameGPUObjectLoader::AddObject(GameLight::SunLight* Light)
{
Lights["SunLight"].push_back((void*)Light);
}
/************************************************
THIS FUNCTION SAVES A GAMELIGHT OF TYPE:POINTLIGHT IN THE "LIGHTS" COLLECTION OF GAMELIGHT OBJECTS
USING THE KEY "POINTLIGHT"
************************************************/
void GameGPUObjectLoader::AddObject(GameLight::PointLight* Light)
{
Lights["PointLight"].push_back((void*)Light);
}
/************************************************
THIS FUNCTION SAVES A GAMELIGHT OF TYPE:SPOTLIGHT IN THE "LIGHTS" COLLECTION OF GAMELIGHT OBJECTS
USING THE KEY "SPOTLIGHT"
************************************************/
void GameGPUObjectLoader::AddObject(GameLight::SpotLight* Light)
{
Lights["SpotLight"].push_back((void*)Light);
}
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
void GameGPUObjectLoader::Load()
{
int WeightData_Binding=0;
int ChildBoneData_Binding=1;
int BoneData_Binding=2;
int MaterialData_Binding=3;
int LightData_Binding=4;

if(Objects.size() > 0)
{
//OPENGL SHARED BUFFER BINDINGS
Bone *bone=nullptr;
Material *material=nullptr;
GameMesh *Mesh=nullptr;
GameArmature *Armature=nullptr;
unsigned char *ImageData=nullptr;

int Width=0;
int Height=0;
//*************************************************************
// PROCESSING OBJECTS
//*************************************************************
for(GameObject* Object: Objects)
{
    Mesh=&Object->Mesh;
    Armature=&Object->Armature;

    //*************************************************************
    // LOAD MATERIAL DATA
    //*************************************************************
    if(Mesh->Materials.size() > 0)
    {
      for(GameMesh::MaterialData* Mat:Mesh->Materials)
      {
        material=new Material();
        material->AmbientStrength=Mat->AmbientStrength;
        material->ShininessIndex=Mat->ShininessIndex;
        material->DiffuseColor[0]=Mat->DiffuseColor.x;
        material->DiffuseColor[1]=Mat->DiffuseColor.y;
        material->DiffuseColor[2]=Mat->DiffuseColor.z;
        material->DiffuseColor[3]=0.0f;
        material->SpecularColor[0]=Mat->SpecularColor.x;
        material->SpecularColor[1]=Mat->SpecularColor.y;
        material->SpecularColor[2]=Mat->SpecularColor.z;
        material->SpecularColor[3]=0.0f;


        //*************************************************************
        // LOAD MATERIAL TEXTURE DATA
        //*************************************************************
        if(Mat->DiffuseMap!="")
        {
            glGenTextures(1, &Object->DiffuseMap);
            glBindTexture(GL_TEXTURE_2D, Object->DiffuseMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Width=0;
            Height=0;
            ImageData=nullptr;
            ImageData=GameBMP::LoadBMP((Mat->DiffuseMap).c_str(),&Width,&Height,"Object "+std::to_string(Object->Index) + " Diffuse Map");
            if(ImageData!=nullptr)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
              glGenerateMipmap(GL_TEXTURE_2D);
              GameBMP::FreeBMP(ImageData);
            }
            material->DiffuseMap=Object->DiffuseMap;
            glBindTexture(GL_TEXTURE_2D,0);
        }
        if(Mat->SpecularMap!="")
        {
            glGenTextures(1, &Object->SpecularMap);
            glBindTexture(GL_TEXTURE_2D, Object->SpecularMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Width=0;
            Height=0;
            ImageData=nullptr;
            ImageData=GameBMP::LoadBMP((Mat->SpecularMap).c_str(),&Width,&Height,"Object "+std::to_string(Object->Index) +" Specular Map");
            if(ImageData!=nullptr)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, Width, Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, ImageData);
              glGenerateMipmap(GL_TEXTURE_2D);
              GameBMP::FreeBMP(ImageData);
            }
            material->SpecularMap=Object->SpecularMap;
            glBindTexture(GL_TEXTURE_2D,0);
        }
        if(Mat->AmbientMap!="")
        {
            glGenTextures(1, &Object->AmbientMap);
            glBindTexture(GL_TEXTURE_2D, Object->AmbientMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Width=0;
            Height=0;
            ImageData=nullptr;
            ImageData=GameBMP::LoadBMP((Mat->AmbientMap).c_str(),&Width,&Height,"Object "+std::to_string(Object->Index) + " Ambient Map");
            if(ImageData!=nullptr)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
              glGenerateMipmap(GL_TEXTURE_2D);
              GameBMP::FreeBMP(ImageData);
            }
            material->AmbientMap=Object->AmbientMap;
            glBindTexture(GL_TEXTURE_2D,0);
        }

        MaterialData.push_back(*material);
      }
    }

    //*************************************************************
    // LOAD MESH DATA
    //*************************************************************
    std::vector<float> v;
    std::vector<float> t;
    std::vector<int> m;
    std::vector<int> vi;
    //ITERATE THROUGH ALL THE FACES OF THE MESH
    for(unsigned int i=0;i< Mesh->Faces.size();i++)
    {
     //ITERATE THROUGH THE 3 VERTICES OF A FACE
     for(int j=0;j<3;j++)
     {
      //*************************************************************
      // LOAD THE X , Y AND Z COORDINATE OF A VERTEX
      //*************************************************************
      v.push_back(Mesh->Vertices[Mesh->Faces[i]->Indices[j]]->x);
      v.push_back(Mesh->Vertices[Mesh->Faces[i]->Indices[j]]->y);
      v.push_back(Mesh->Vertices[Mesh->Faces[i]->Indices[j]]->z);
      //*************************************************************
      // LOAD VERTEX INDEX OF A VERTEX
      //*************************************************************
      vi.push_back(Mesh->Faces[i]->Indices[j]);
      //*************************************************************
      // LOAD UV COORDINATES OF A VERTEX
      //*************************************************************
      if(Mesh->Faces[i]->UVCoords.size() > 0)
      {
       t.push_back(Mesh->Faces[i]->UVCoords[j]->x);
       t.push_back(Mesh->Faces[i]->UVCoords[j]->y);
      }
      //*************************************************************
      // LOAD MATERIAL INDEX FOR A FACE
      //*************************************************************
      if(Mesh->Materials.size() > 0)
      m.push_back(Mesh->Materials.at(Mesh->Faces[i]->Material_index)->Index);
     }
    }

    unsigned int VAO,VVBO,VIVBO,TVBO,MVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VVBO);
    glGenBuffers(1, &VIVBO);
    glGenBuffers(1, &TVBO);
    glGenBuffers(1, &MVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), &v[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if(t.size() > 0)
    {
    glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    glBufferData(GL_ARRAY_BUFFER, t.size()  * sizeof(float), &t[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    }

    if(m.size() > 0)
    {
    glBindBuffer(GL_ARRAY_BUFFER, MVBO);
    glBufferData(GL_ARRAY_BUFFER, m.size()  * sizeof(int), &m[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(int), (void*)0);
    glEnableVertexAttribArray(2);
    Object->NoMaterialFlag=0;//SETTING THE OBJECT'S "NOMATERIALFLAG" VALUE FROM DEFAULT -1 TO 0 INDICATING THAT MATERIAL IS PRESENT
    }

    glBindBuffer(GL_ARRAY_BUFFER, VIVBO);
    glBufferData(GL_ARRAY_BUFFER, vi.size() * sizeof(float), &vi[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);

    Object->VAO=VAO;
    Object->VAOCount=vi.size();
    Object->BoundingBox=Mesh->BoundingBox;//COPYING OVER THE MESH' BOUNDING BOX OVER TO THE OBJECT'S BOUNDING BOX
    Object->BoundingBox->Object=Object;//CREATING A REFERENCE TO THIS OBJECT IN THIS OBJECT'S BOUNDING BOX.
                                       //SO THIS GAMEOBJECT CAN BE REFERENCED DIRECTLY FROM WITHIN ITS BOUNDING BOX

    //*************************************************************
    // LOADING ARMATURE AND VERTEX WEIGHT DATA
    //*************************************************************
    // ASSIGNING UNIQUE INDICES TO BONES AS PER THEIR POSITION
    // IN THE BONE SHARED BUFFER OBJECT IN THE VERTEX SHADER
    for(std::map<std::string,GameArmature::Bone*>::iterator boneit=Armature->Bones.begin();boneit!=Armature->Bones.end();boneit++)
    {
       boneit->second->Index=Global_BoneIndex;
       Global_BoneIndex++;
    }

    unsigned int BoneCount=0;
    for(std::map<std::string,GameArmature::Bone*>::iterator boneit=Armature->Bones.begin();boneit!=Armature->Bones.end();boneit++)
    {
      //MARK THE GLOBAL INDEX OF THE FIRST BONE FOR THIS OBJECT
      if(BoneCount==0)
      Object->BoneStartIndex=boneit->second->Index;

      //CREATE EMPTY MATRICES FOR THE BONE
      bone=new GameGPUObjectLoader::Bone();
      GameMath::Transform::CopyMatrixtoArray(nullptr,&bone->SelfMatrix[0][0]);
      GameMath::Transform::CopyMatrixtoArray(nullptr,&bone->InheritedMatrix[0][0]);
      GameMath::Transform::CopyMatrixtoArray(nullptr,&bone->OutMatrix[0][0]);

      //MARK THE GLOBAL INDEX OF THE FIRST CHILD BONE FOR THIS OBJECT
      bone->ChildStartIndex=Global_ChildBoneIndex;
      for(std::map<std::string,GameArmature::Bone*>::iterator childit=(boneit->second)->Child.begin();childit!=(boneit->second)->Child.end();childit++)
      {
         ChildBoneData.push_back(childit->second->Index);
         Global_ChildBoneIndex++;
      }
      //MARK THE GLOBAL INDEX OF THE LAST CHILD BONE FOR THIS OBJECT
      bone->ChildEndIndex=Global_ChildBoneIndex;

      //CREATE A VERTEX GROUP DATA FOR THIS BONE IN THIS OBJECT
      GameMesh::VertexGroupData* vertexgroup=Mesh->VertexGroups[boneit->first];

      //MARK THE GLOBAL INDEX OF THE FIRST WEIGHT DATA FOR THIS BONE IN THIS OBJECT
      bone->WeightStartIndex=Global_WeightIndex;
      for(float Weight:vertexgroup->Weights)
      {
          WeightData.push_back(Weight);
          Global_WeightIndex++;
      }
      bone->WeightEndIndex=Global_WeightIndex;
      //MARK THE GLOBAL INDEX OF THE LAST WEIGHT DATA FOR THIS BONE IN THIS OBJECT

      //SAVE THE NEW BONE IN THE COLLECTION: "BONE DATA"
      BoneData.push_back(*bone);

      //MARK THE GLOBAL INDEX OF THE LAST BONE FOR THIS OBJECT
      if(BoneCount==Armature->Bones.size()-1)
      Object->BoneEndIndex=boneit->second->Index;

      BoneCount++;
    }
std::cout<<"Object #"<<Object->Index<<" Loaded in GPU \n";
}
unsigned int MaterialData_SSBO;
unsigned int WeightData_SSBO;
unsigned int ChildBoneData_SSBO;
unsigned int BoneData_SSBO;
//*************************
// MATERIAL DATA BINDING
//*************************
glGenBuffers(1, &MaterialData_SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, MaterialData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,MaterialData_Binding,MaterialData_SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, MaterialData.size() * sizeof(Material), &MaterialData[0], GL_DYNAMIC_DRAW);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
MaterialData.clear();
//*************************
// BONE DATA BINDING
//*************************
glGenBuffers(1, &BoneData_SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, BoneData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,BoneData_Binding,BoneData_SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, BoneData.size() * sizeof(Bone), &BoneData[0], GL_DYNAMIC_DRAW);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
BoneData.clear();
//*************************
// CHILD DATA BINDING
//*************************
glGenBuffers(1, &ChildBoneData_SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, ChildBoneData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,ChildBoneData_Binding,ChildBoneData_SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, ChildBoneData.size() * sizeof(int), &ChildBoneData[0], GL_DYNAMIC_DRAW);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
ChildBoneData.clear();
//*************************
// WEIGHT DATA BINDING
//*************************
glGenBuffers(1, &WeightData_SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, WeightData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,WeightData_Binding,WeightData_SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, WeightData.size() * sizeof(float), &WeightData[0], GL_DYNAMIC_DRAW);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
WeightData.clear();
//**************************
// LINKING BONEDATA BINDING AND SSBO
//**************************
for(GameObject* Object: Objects)
{
Object->BoneData_SSBO=BoneData_SSBO;
Object->BoneData_Binding=BoneData_Binding;
}
//******************************
// END OF ALL OBJECT PROCESSING
//******************************
}

//*************************************************************
// Processing Lights
//*************************************************************
if(Lights.size() > 0)
{
    GameLight::SunLight* sunlight=nullptr;
    GameLight::PointLight* pointlight=nullptr;
    GameLight::SpotLight* spotlight=nullptr;
    Light* light=nullptr;

    for(void* L:Lights["SunLight"])
    {
      sunlight=(GameLight::SunLight*)L;
      light=new Light();

      light->LightType=0;

      light->Color[0]=sunlight->Color[0];
      light->Color[1]=sunlight->Color[1];
      light->Color[2]=sunlight->Color[2];
      light->Color[3]=0.0f;

      light->Direction[0]=sunlight->Direction[0];
      light->Direction[1]=sunlight->Direction[1];
      light->Direction[2]=sunlight->Direction[2];
      light->Direction[3]=-1.0f;

      sunlight->DataIndex=LightData.size() * sizeof(Light);
      LightData.push_back(*light);
    }

    for(void* L:Lights["PointLight"])
    {
      pointlight=(GameLight::PointLight*)L;
      light=new Light();

      light->LightType=1;

      light->Color[0]=pointlight->Color[0];
      light->Color[1]=pointlight->Color[1];
      light->Color[2]=pointlight->Color[2];
      light->Color[3]=0.0f;

      light->Position[0]=pointlight->Position[0];
      light->Position[1]=pointlight->Position[1];
      light->Position[2]=pointlight->Position[2];
      light->Position[3]=1.0f;

      light->Constant=pointlight->Constant;
      light->Linear=pointlight->Linear;
      light->Quadratic=pointlight->Quadratic;

      pointlight->DataIndex=LightData.size()  * sizeof(Light);
      LightData.push_back(*light);
    }

    for(void* L:Lights["SpotLight"])
    {
      spotlight=(GameLight::SpotLight*)L;
      light=new Light();

      light->LightType=2;

      light->Color[0]=spotlight->Color[0];
      light->Color[1]=spotlight->Color[1];
      light->Color[2]=spotlight->Color[2];
      light->Color[3]=0.0f;

      light->Position[0]=spotlight->Position[0];
      light->Position[1]=spotlight->Position[1];
      light->Position[2]=spotlight->Position[2];
      light->Position[3]=1.0f;

      light->Direction[0]=spotlight->Direction[0];
      light->Direction[1]=spotlight->Direction[1];
      light->Direction[2]=spotlight->Direction[2];
      light->Direction[3]=0.0f;

      light->Constant=spotlight->Constant;
      light->Linear=spotlight->Linear;
      light->Quadratic=spotlight->Quadratic;

      light->CutOffAngle=cos(GameMath::Transform::GetRadian(spotlight->CutOffAngle));
      light->OuterCutOffAngle=cos(GameMath::Transform::GetRadian(spotlight->OuterCutOffAngle));

      spotlight->DataIndex=LightData.size() * sizeof(Light);
      LightData.push_back(*light);
    }

    GameLight::TotalLights=LightData.size();

//*************************
// Light Data Binding
//*************************
unsigned int LightData_SSBO;
glGenBuffers(1, &LightData_SSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,LightData_Binding,LightData_SSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, LightData.size() * sizeof(Light), &LightData[0], GL_DYNAMIC_DRAW);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
LightData.clear();
//*************************
// Linking LightData SSBO
//*************************
for(void* L:Lights["SunLight"])
{
    ((GameLight::SunLight*)L)->LightData_SSBO=LightData_SSBO;
    ((GameLight::SunLight*)L)->LightData_Binding=LightData_Binding;
}

for(void* L:Lights["PointLight"])
{
    ((GameLight::PointLight*)L)->LightData_SSBO=LightData_SSBO;
    ((GameLight::PointLight*)L)->LightData_Binding=LightData_Binding;
}

for(void* L:Lights["SpotLight"])
{
    ((GameLight::SpotLight*)L)->LightData_SSBO=LightData_SSBO;
    ((GameLight::SpotLight*)L)->LightData_Binding=LightData_Binding;
}

std::cout<<"Light(s) loaded in GPU = "<<GameLight::TotalLights<<" \n";
//******************************
// End of Light Data Processing
//******************************
}

}




