#include <iostream>
#include <cstdlib>
//#include <unistd.h>
#include <GameWindow/GameWindow.h>
#include <GameMath/GameMath.h>
#include <GameShader/GameShader.h>
#include <GameCamera/GameCamera.h>
#include <GameBMP/GameBMP.h>
#include <GameBlenderImport/GameBlenderImport.h>
#include <GameArmature/GameArmature.h>
#include <GameGPUObjectLoader/GameGPUObjectLoader.h>
#include <GameObject/GameObject.h>
#include <GameWorld/GameWorld.h>
#include <GameText/GameText.h>
#include <GameSettings/GameSettings.h>
#include <typeinfo>
#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif

//*******************************************************************
// PIXELMAPPING FUNCTIONS
//*******************************************************************

/*************************************************
THE PIXEL STRUCTURE CONTAINS THE FOLLOWING PROPERTIES:
1) INT TYPE - A PIXEL STRUCTURE CAN BE LINKED TO A GAMEWORLD CUBE OBJECT AND BOUNDING BOX OBJECT
              TYPE 1 - THE OBJECT LINKED TO THIS PIXEL OBJECT IS OF TYPE GAMEWORLDCUBE
              TYPE 2 - THE OBJECT LINKED TO THIS PIXEL OBJECT IS OF TYPE GAMEBOUNDINGBOX

2) VOID POINTER - THIS POINTS TO EITHER A GAMEWORLDCUBE OBJECT OR GAMEBOUNDINGBOX OBJECT
*************************************************/


/*************************************************
THIS FUNCTION DOES THE FOLLOWING:
1) CREATES A NEW PIXEL OBJECT
2) ASSIGNS THE GAMEWORLDCUBE OBJECT PASSED ON AS ARGUMENT TO THE NEW PIXEL OBJECT
3) GENERATE A UNIQUE COLOR FOR THIS PIXEL OBJECT AND CREATE UNIQUE STRING REPRESENTATIONN FOR THAT COLOR
2) SAVES THE NEW PIXEL OBJECT IN THE "PIXELMAP" USING THE STRING REPRESENTATION OF THE UNIQUE COLOR ASSIGNED ABOVE
   PIXEL MAP IS A MAP OF PIXEL OBJECTS WITH A UNIQUE STRING KEY FOR EACH PIXEL
*************************************************/
void GameWindow::PixelMapping(GameWorldCube* payload)
{
    Pixel* pixel=new Pixel();
    pixel->type=1;
    pixel->payload=(void*)payload;
    GenerateKey(pixel,&(payload->R),&(payload->G),&(payload->B));
}
/*************************************************
THIS FUNCTION DOES THE FOLLOWING:
1) CREATES A NEW PIXEL OBJECT
2) ASSIGNS THE GAMEBOUNDINGBOX OBJECT PASSED ON AS ARGUMENT TO THE NEW PIXEL OBJECT
3) GENERATE A UNIQUE COLOR FOR THIS PIXEL OBJECT AND CREATE UNIQUE STRING REPRESENTATIONN FOR THAT COLOR
2) SAVES THE NEW PIXEL OBJECT IN THE "PIXELMAP" USING THE STRING REPRESENTATION OF THE UNIQUE COLOR ASSIGNED ABOVE
   PIXEL MAP IS A MAP OF PIXEL OBJECTS WITH A UNIQUE STRING KEY FOR EACH PIXEL
*************************************************/
void GameWindow::PixelMapping(GameBoundingBox* payload)
{
    Pixel* pixel=new Pixel();
    pixel->type=2;
    pixel->payload=(void*)payload;
    GenerateKey(pixel,&payload->R,&payload->G,&payload->B);
}
/*************************************************
THIS FUNCTION CREATES A UNIQUE COLOR FOR A PIXEL OBJECT AND CREATE A UNIQUE STRING REPRESENTATION OF THAT COLOR
THAT CAN UNIQUELY IDENTIFY THAT PIXEL OBJECT. THE PROCEDURE INVOLVED IS AS FOLLOWS:

1) A COLOR IS MADE UP OF RED, GREEN AND BLUE COLOR
2) EACH COLOR COMPONENT(RGB) CAN HAVE A FLOATING VALUE FROM 0.0 TO 1.0 IN OPENGL SHADERS
3) OUTSIDE OPENGL, EACH COLOR COMPONENT CAN HAVE 255 UNIQUE VALUES BY DIVING THE COLOR COMPONENT BY 255.0
   E.G - RED CAN BE 155 WHICH WHEN DIVIDED BY 255 GIVES US = 0.607 WHICH IS BETWEEN 0.0 - 1.0
         RED CAN BE 220 WHICH WHEN DIVIDED BY 255 GIVES US = 0.862 WHICH IS BETWEEN 0.0 - 1.0
4) WE TRACK VALUES FOR EACH COLOR COMPONENTS IN VARIABLES:
   Pixel_R_Index
   Pixel_G_Index
   Pixel_B_Index
   INITIALLY THEY ARE ALL SET TO ZERO
5) RGB VALUE OF 000 IS RESERVED FOR THE WORLD BACKGROUND SPACE
6) WHENEVER A NEW PIXEL OBJECT IS NEEDED TO BE ASSIGNED A UNIQUE COLOR, THE INDIVIDUAL RGB VALUES ARE
   CONVERTED INTO STRING REPRESENTATION OF 3 DIGITS WITH LEADING ZEROS
   E.G RED COLOR 10 IS COVERTED INTO STRING "010"
       RED COLOR 220 IS COVERTED INTO STRING "220"
       RED COLOR 5 IS COVERTED INTO STRING "005"
7) THE STRING REPRENSENTATIONS OF EACH COLOR ARE COMBINED TOGETHER IN THE ORDER
   RED COLOR + GREEN COLOR + BLUE COLOR
   TO CREATE A UNIQUE STRING KEY TO BE USED AS KEY IN THE PIXELMAP FOR THE NEW PIXEL OBJECT
   E.G- RED COLOR : 5 , GREEN COLOR: 154 , BLUE COLOR : 67 TURNS INTO "005" + "154" + "067" = STRING "00515467"
8) THE RGB FLOATING VALUES OBTAINED AFTER DIVING EACH OF THEM BY 255.0
   ARE ALSO OUTPUT VIA THE FLOATING VALUE POINTERS SO THEY CAN BE ASSIGNED TO THE
   GAMEWORLDCUBE OBJECT OR THE GAMEBOUNDINGBOX OBJECT THAT THE PIXEL OBJECT WILL BE LINKED TO
8) AFTER THAT THE RGB VALUES ARE INCREMENTED IN THE FOLLOWING WAY:
   A) RED COMPONENT IS INCREMENTED IF ITS LESS THAN 255
   B) GREEN COMPONENT IS INCREMENTED IF ITS LESS THAN 255 AND RED COMPONENT IS 255. RESET RED TO 0
   C) BLUE COMPONENT IS INCREMENTED IF ITS LESS THAN 255 AND GREEN COMPONENT IS 255. RESET GREEN TO 0

   THIS ALLOWS EACH GREEN COLOR VALUE TO HAVE 255 RED COLOR VALUES AND EACH BLUE COLOR VALUE TO HAVE
   255 GREEN COLOR VALUES WHICH GIVES US A TOTAL OF = 255(RED) * 255(GREEN) * 255(BLUE) = 16581375 UNIQUE COLORS
*************************************************/
void GameWindow::GenerateKey(Pixel *pixel,float* Rf,float* Gf,float* Bf)
{
    std::string R,G,B;
    std::string zero="0";

    if(Pixel_R_Index <=9)
    R=zero+zero+std::to_string(Pixel_R_Index);
    else if(Pixel_R_Index >=10 && Pixel_R_Index <=99)
    R=zero+std::to_string(Pixel_R_Index);
    else
    R=std::to_string(Pixel_R_Index);

    if(Pixel_G_Index <=9)
    G=zero+zero+std::to_string(Pixel_G_Index);
    else if(Pixel_G_Index >=10 && Pixel_G_Index <=99)
    G=zero+std::to_string(Pixel_G_Index);
    else
    G=std::to_string(Pixel_G_Index);

    if(Pixel_B_Index <=9)
    B=zero+zero+std::to_string(Pixel_B_Index);
    else if(Pixel_B_Index >=10 && Pixel_B_Index <=99)
    B=zero+std::to_string(Pixel_B_Index);
    else
    B=std::to_string(Pixel_B_Index);

    GameWindow::PixelMap[R+G+B]=pixel;
    *Rf=(float)Pixel_R_Index/255.0f;
    *Gf=(float)Pixel_G_Index/255.0f;
    *Bf=(float)Pixel_B_Index/255.0f;

    if(Pixel_B_Index < 255)
    {
        Pixel_B_Index++;
    }
    if(Pixel_G_Index < 255 && Pixel_B_Index == 255)
    {
        Pixel_B_Index=0;
        Pixel_G_Index++;
    }
    if(Pixel_R_Index < 255 && Pixel_G_Index == 255 && Pixel_B_Index == 255)
    {
        Pixel_B_Index=0;
        Pixel_G_Index=0;
        Pixel_R_Index++;
    }
}
/*************************************************
THIS FUNCTION TAKES IN INDIVIDUAL RED , GREEN AND BLUE COLOR VALUES RANGING FROM
0 TO 255 AND CONVERTS THEM INTO A UNIQUE STRING THAT CAN BE USED TO RETRIEVE A PIXEL OBJECT FROM THE
PIXELMAP

THE INDIVIDUAL RGB VALUES ARE CONVERTED INTO STRING REPRESENTATION OF 3 DIGITS WITH LEADING ZEROS
E.G RED COLOR 10 IS COVERTED INTO STRING "010"
    RED COLOR 220 IS COVERTED INTO STRING "220"
    RED COLOR 5 IS COVERTED INTO STRING "005"

THEN THE STRING REPRENSENTATIONS OF EACH COLOR ARE COMBINED TOGETHER IN THE ORDER
RED COLOR + GREEN COLOR + BLUE COLOR
TO CREATE A UNIQUE STRING KEY TO BE USED AS KEY IN PIXELMAP
E.G- RED COLOR : 5 , GREEN COLOR: 154 , BLUE COLOR : 67 TURNS INTO "005" + "154" + "067" = STRING "00515467"
*************************************************/
std::string GameWindow::ReadKey(int Pixel_R_Index,int Pixel_G_Index,int Pixel_B_Index)
{
    std::string R,G,B;
    std::string zero="0";

    if(Pixel_R_Index <=9)
    R=zero+zero+std::to_string(Pixel_R_Index);
    else if(Pixel_R_Index >=10 && Pixel_R_Index <=99)
    R=zero+std::to_string(Pixel_R_Index);
    else
    R=std::to_string(Pixel_R_Index);

    if(Pixel_G_Index <=9)
    G=zero+zero+std::to_string(Pixel_G_Index);
    else if(Pixel_G_Index >=10 && Pixel_G_Index <=99)
    G=zero+std::to_string(Pixel_G_Index);
    else
    G=std::to_string(Pixel_G_Index);

    if(Pixel_B_Index <=9)
    B=zero+zero+std::to_string(Pixel_B_Index);
    else if(Pixel_B_Index >=10 && Pixel_B_Index <=99)
    B=zero+std::to_string(Pixel_B_Index);
    else
    B=std::to_string(Pixel_B_Index);

    return R+G+B;
}



//******************************************************************
// HEALTH BOX, WEAPON BOX AND INANIMATE OBJECT POSITIONING FUNCTIONS
//******************************************************************
GameWorldCube* HealthCube;
GameWorldCube* WeaponCube;
/*****************************************************************
THIS FUNCTION POSITIONS THE HEALTH OBJECT IN THE WORLD BY ASSIGNING IT A EMPTY GAMEWORLDCUBE

1) CHECK IF THE "HealthCube" GAMEWORLDCUBE IS SET TO NULLPTR WHICH INDICATES
   THAT THERE IS NO OTHER HEALTH BOX OBJECT CURRENTLY DIPSLAYING IN THE WORLD
2) GET RANDOMIZED KEYS VALUES X AND Z FROM RANGE 0 TO WORLD OBJECT'S XBOUND AND ZBOUND VARIABLES
   RESPECTIVELY
3) USE THE RANDOMIZED KEY VALUES TO GET A RANDOM GAMEWORLD CUBE FROM "WorldCoordMap" WHICH
   IS STILL EMPTY OR HAS NOT BE OCCUPIED BY ANOTHER OBJECT
4) ASSIGN THE HEALTH OBJECT THIS GAMEWORLDCUBE USING ITS "SourceWorldCube" PROPERTY
5) SET THE "HEALTH OBJECT" PROPERTY OF THIS GAMEWORLDCUBE TO THIS HEALTH OBJECT
6) TRANSLATE THE HEALTH OBJECT TO THE LOCATION OF THE GAMEWORLDCUBE. APPLY THE SAME TRANSLATION TO IT'S CENTROID
7) SET THE HEALTH OBJECT'S "ISALIVE" PROPERTY TO TRUE SO IT CAN BE RENDERED ON THE SCREEN
*****************************************************************/
void SpawnHealth(GameObject* Object,GameWorld *World)
{
if(HealthCube!=nullptr)
return;

GameWorldCube* WC=nullptr;
int X=0;
int Z=0;
int count=0;

while(WC==nullptr && count < 10)
{
  X= rand() % World->xbound + 0;
  while(X % 2!=0)
  X=X+1;

  Z= rand() % World->zbound + 0;
  while(Z % 2!=0)
  Z=Z+1;

  WC=(World->WorldCoordMap[Z])[X];
  if(WC->Object!=nullptr)
  WC=nullptr;

  count++;
}

if(WC==nullptr)
{
HealthCube=nullptr;
return;
}

GameMath::Vector3 LOC=WC->Centroid;
LOC.y=LOC.y + 1.0f;
Object->SourceWorldCube=WC;
WC->HealthObject=Object;
Object->BuggyFinalMatrix=GameMath::Transform::Translate(LOC);
Object->BoundingBox->Centroid.x=Object->BuggyFinalMatrix.Matrix[3][0];
Object->BoundingBox->Centroid.y=Object->BuggyFinalMatrix.Matrix[3][1];
Object->BoundingBox->Centroid.z=Object->BuggyFinalMatrix.Matrix[3][2];
Object->BoundingBox->CalculateXYZ();
Object->isAlive=true;
HealthCube=WC;
}
/*****************************************************************
THIS FUNCTION POSITIONS THE WEAPON OBJECT IN THE WORLD BY ASSIGNING IT A EMPTY GAMEWORLDCUBE

1) CHECK IF THE "WeaponCube" GAMEWORLDCUBE IS SET TO NULLPTR WHICH INDICATES
   THAT THERE IS NO OTHER WEAPON BOX OBJECT CURRENTLY DIPSLAYING IN THE WORLD
2) GET RANDOMIZED KEYS VALUES X AND Z FROM RANGE 0 TO WORLD OBJECT'S XBOUND AND ZBOUND VARIABLES
   RESPECTIVELY
3) USE THE RANDOMIZED KEY VALUES TO GET A RANDOM GAMEWORLD CUBE FROM "WorldCoordMap" WHICH
   IS STILL EMPTY OR HAS NOT BE OCCUPIED BY ANOTHER OBJECT
4) ASSIGN THE WEAPON OBJECT THIS GAMEWORLDCUBE USING ITS "SourceWorldCube" PROPERTY
5) SET THE "WEAPON OBJECT" PROPERTY OF THIS GAMEWORLDCUBE TO THIS WEAPON OBJECT
6) TRANSLATE THE WEAPON OBJECT TO THE LOCATION OF THE GAMEWORLDCUBE. APPLY THE SAME TRANSLATION TO IT'S CENTROID
7) SET THE WEAPON OBJECT'S "ISALIVE" PROPERTY TO TRUE SO IT CAN BE RENDERED ON THE SCREEN
*****************************************************************/
void SpawnWeapon(GameObject* Object,GameWorld *World)
{
if(WeaponCube!=nullptr)
return;

GameWorldCube* WC=nullptr;
int X=0;
int Z=0;
int count=0;

while(WC==nullptr && count < 10)
{
  X= rand() % World->xbound + 0;
  while(X % 2!=0)
  X=X+1;

  Z= rand() % World->zbound + 0;
  while(Z % 2!=0)
  Z=Z+1;

  WC=(World->WorldCoordMap[Z])[X];
  if(WC->Object!=nullptr)
  WC=nullptr;

  count++;
}

if(WC==nullptr)
{
WeaponCube=nullptr;
return;
}

GameMath::Vector3 LOC=WC->Centroid;
LOC.y=LOC.y + 1.0f;
Object->SourceWorldCube=WC;
WC->WeaponObject=Object;
Object->BuggyFinalMatrix=GameMath::Transform::Translate(LOC);
Object->BoundingBox->Centroid.x=Object->BuggyFinalMatrix.Matrix[3][0];
Object->BoundingBox->Centroid.y=Object->BuggyFinalMatrix.Matrix[3][1];
Object->BoundingBox->Centroid.z=Object->BuggyFinalMatrix.Matrix[3][2];
Object->BoundingBox->CalculateXYZ();
Object->isAlive=true;
WeaponCube=WC;
}
/*****************************************************************
THIS FUNCTION POSITIONS INANIMATE OBJECTS IN THE WORLD BY ASSIGNING THEM EMPTY GAMEWORLDCUBES

1) GET RANDOMIZED KEYS VALUES X AND Z FROM RANGE 0 TO WORLD OBJECT'S XBOUND AND ZBOUND VARIABLES
   RESPECTIVELY
2) USE THE RANDOMIZED KEY VALUES TO GET A RANDOM GAMEWORLD CUBE FROM "WorldCoordMap" WHICH
   IS STILL EMPTY OR HAS NOT BE OCCUPIED BY ANOTHER OBJECT
3) ASSIGN THE INANIMATE OBJECT THIS GAMEWORLDCUBE USING ITS "SourceWorldCube" PROPERTY
4) SET THE "OBJECT" PROPERTY OF THIS GAMEWORLDCUBE TO THIS INANIMATE OBJECT
5) TRANSLATE THE OBJECT TO THE LOCATION OF THE GAMEWORLDCUBE. APPLY THE SAME TRANSLATION TO IT'S CENTROID
*****************************************************************/
void SpawnInanimateObjects(GameObject* Object,GameWorld *World)
{
GameWorldCube* WC=nullptr;
int X=0;
int Z=0;
int count=0;

while(WC==nullptr && count < 10)
{
  X= rand() % World->xbound + 0;
  while(X % 2!=0)
  X=X+1;

  Z= rand() % World->zbound + 0;
  while(Z % 2!=0)
  Z=Z+1;

  WC=(World->WorldCoordMap[Z])[X];
  if(WC->Object!=nullptr)
  WC=nullptr;

  count++;
}

if(WC==nullptr)
return;

GameMath::Vector3 LOC=WC->Centroid;
Object->SourceWorldCube=WC;
WC->Object=Object;
Object->BuggyFinalMatrix=GameMath::Transform::Translate(LOC);
Object->BoundingBox->Centroid.x=Object->BuggyFinalMatrix.Matrix[3][0];
Object->BoundingBox->Centroid.y=Object->BuggyFinalMatrix.Matrix[3][1];
Object->BoundingBox->Centroid.z=Object->BuggyFinalMatrix.Matrix[3][2];
Object->BoundingBox->CalculateXYZ();
}
/*****************************************************************
THIS FUNCTION ASSIGNS GAMEEFFECTS BULLET, BULLET FIRE AND EXPLOSION OBJECTS TO A GAMEOBJECT

1) IT CREATES NEW OBJECTS OF TYPES BULLET, BULLET FIRE AND EXPLOSION
2) SETS UP THE FILEPATH TO THEIR RESPECTIVE TEXTURE FILES FROM WHICH THEIR TEXTURE ARE LOADED
3) USE THE "IMPORTEFFECTS" FUNCTION TO LOAD THEIR VERTEX DATA INTO A BUFFER
4) USE THEIR "LOAD" FUNCTION TO LOAD THEIR TEXTURES AND VERTICES IN VERTEX SHADERS
5) THE OBJECT'S _BULLET, _BULLETFIRE AND _EXPLOSION OBJECT POINTERS ARE ASSIGNED THOSE
   GAMEFFECTS RESPECTIVELY
******************************************************************/
void GenerateEffects(GameObject* object)
{
GameEffects::Bullet* bullet=new GameEffects::Bullet();
bullet->TextureLocation=GameWindow::CWD+"\\Payload\\Textures\\Bullet.bmp";
bullet->Vertices=GameBlenderImport::ImportEffects(std::string(GameWindow::CWD+"\\Payload\\Models\\Bullet.GameEffects").c_str(),&(bullet->TriangleCount));
bullet->Load();

GameEffects::BulletFire* bulletfire=new GameEffects::BulletFire();
bulletfire->TextureLocation=GameWindow::CWD+"\\Payload\\Textures\\Bullet.bmp";
bulletfire->Vertices=GameBlenderImport::ImportEffects(std::string(GameWindow::CWD+"\\Payload\\Models\\BulletFiring.GameEffects").c_str(),&(bulletfire->TriangleCount));
bulletfire->Load();

GameEffects::Explosion* explosion=new GameEffects::Explosion();
explosion->TextureLocation=GameWindow::CWD+"\\Payload\\Textures\\Explosion.bmp";
explosion->Vertices=GameBlenderImport::ImportEffects(std::string(GameWindow::CWD+"\\Payload\\Models\\Explosion.GameEffects").c_str(),&(explosion->TriangleCount));
explosion->Load();

object->_Bullet=bullet;
object->_BulletFire=bulletfire;
object->_Explosion=explosion;
}

//******************************************************************
// MAIN FUNCTION
//******************************************************************
int main()
{
//******************************************************************
//GET ALL GAME SETTINGS FOR GAME PLAY 
//******************************************************************
GameSettings::Setup();

//******************************************************************
//INITIALIZE THE GAME WINDOW THAT WILL OUTPUT THE GAME GRAPHICS
//******************************************************************
/*
OPENGL FUNCTIONS:
glViewport - glviewport() defines what part of the screen we want to render to by defining the x coordinate,
             y coordinate, width, and height of the rendering area
glEnable - enable or disable server-side gl capabilities, here we are enabling "GL_DEPTH_TEST"
glShadeModel - select flat or smooth shading.flat shading is one color for the polygon whereas smooth shading
               allows for interpolation of color values between vertices
glClearColor - specify clear values for the color buffers in rgba format. it basically sets up the
               background color in the render window
*/
GameWindow::Initialize("3D GAME USING OPENGL - KILL THAT BUGGY",100,100,800,600);
if (!GameWindow::GetStatus())
  return 0;

glViewport(0,0,GameWindow::Width,GameWindow::Height);
glEnable(GL_DEPTH_TEST);
glShadeModel(GL_FLAT);
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
std::cout<<"3D Game Window Initialized\n";

//******************************************************************
//GET CURRENT WORKING DIRECTORY SO THE PAYLOAD DIRECTORY CAN BE
//LOCATED AND ALL THE ASSETS CAN BE LOADED IN MEMORY
//********************`**********************************************
TCHAR CWD[MAX_PATH];
GetCurrentDirectory(MAX_PATH, CWD);
std::wstring wStr = CWD;
GameWindow::CWD = std::string(wStr.begin(), wStr.end());

//******************************************************************
// SET MODEL AND PROJECTION MATRICES ALONG WITH THE CAMERA OBJECT
//******************************************************************
GameCamera::FPS FPSCamera;
GameMath::Matrix4x4 Model;
GameMath::Matrix4x4 Projection=GameMath::Transform::PerspectiveView((float)GameWindow::Width/(float)GameWindow::Height,0.1f, 100.0f, 45.0f);

/******************************************************************
SETUP THE MAIN SHADER AND THE HELPER SHADER
MAIN SHADERS - RENDERS OBJECTS ON THE SCREEN USING THE FOREGROUND FRAME BUFFER SO THEY CAN BE DISPLAYED TO THE USER.
               MOST OF THESE OBJECTS WILL BE GAMEOBJECTS, GAMEWORLD, GAMEFFECTS OBJECTS LIKE BULLET AND INANIMATE OBJECTS
               LIKE ROCKS, HEALTH AND WEAPON OBJECTS

HELPER SHADER - RENDERS OBJECTS IN THE BACKGROUND USING THE BACKGROUND FRAME BUFFER. THE OBJECTS IN THE BACKGROUND
                FRAMEOBJECT ARE NEVER RENDERED ON THE SCREEN. THEY ARE USED TO ALLOW USER INTERACTION TO PROCESS.
                EACH OBJECT IN THE BACKGROUND FRAMEE BUFFER IS GIVEN A UNIQUE COLOR WHICH IS USED TO IDENTIFY THAT OBJECT
                AND THAT IS HOW THE USER INTERACTS WITH THE WORLD
******************************************************************/
GameShader MainShader;
GameShader HelperShader;
MainShader.LoadShader(std::string(GameWindow::CWD+"\\Payload\\Shaders\\MainVertex.cpp").c_str(), std::string(GameWindow::CWD+"\\Payload\\Shaders\\MainFragment.cpp").c_str(),std::string(GameWindow::CWD+"\\Payload\\Shaders\\MainGeometry.cpp").c_str());
HelperShader.LoadShader(std::string(GameWindow::CWD+"\\Payload\\Shaders\\HelperVertex.cpp").c_str(), std::string(GameWindow::CWD+"\\Payload\\Shaders\\HelperFragment.cpp").c_str(),nullptr);

//******************************************************************
//  SETUP A WORLD OBJECT
//  SETUP 10 GAME OBJECTS WHERE 8 OBJECTS ARE BUGGIES AND THE REMANINING 2 ARE HEALTH AND WEAPON OBJECT
//  SETUP 30 INANIMATE OBJECTS
//******************************************************************
MainShader.Use();
int ObjectCount=10;
int InanimateObjectCount=30;
GameObject* WorldObject;
GameObject* Objects[10];
GameObject* InanimateObject[30];

//******************************************************************
//  IMPORT BUGGY OBJECTS ALONG WITH THEIR GAME EFFECTS OBJECTS
//******************************************************************
Objects[0]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobile.GameEngine").c_str());
GenerateEffects(Objects[0]);
Objects[1]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobile.GameEngine").c_str());
GenerateEffects(Objects[1]);
Objects[2]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobile.GameEngine").c_str());
GenerateEffects(Objects[2]);
Objects[3]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobile.GameEngine").c_str());
GenerateEffects(Objects[3]);
Objects[4]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobileBad.GameEngine").c_str());
GenerateEffects(Objects[4]);
Objects[5]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobileBad.GameEngine").c_str());
GenerateEffects(Objects[5]);
Objects[6]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobileBad.GameEngine").c_str());
GenerateEffects(Objects[6]);
Objects[7]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\BatMobileBad.GameEngine").c_str());
GenerateEffects(Objects[7]);

//******************************************************************
//  IMPORT HEALTH BOX AND WEAPON BOX OBJECTS
//******************************************************************
Objects[8]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\HealthBox.GameEngine").c_str());
Objects[9]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\WeaponBox.GameEngine").c_str());

//******************************************************************
//  IMPORT 5 DIFFERENT INANIMATE OBJECTS 6 TIMES
//  TO CREATE 30 INANIMTE OBJECTS IN TOTAL
//******************************************************************
for(int i=0;i<30;)
{
InanimateObject[i++]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\Rock1.GameEngine").c_str());
InanimateObject[i++]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\Rock2.GameEngine").c_str());
InanimateObject[i++]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\Rock3.GameEngine").c_str());
InanimateObject[i++]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\Rock4.GameEngine").c_str());
InanimateObject[i++]=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\Rock5.GameEngine").c_str());
}

//******************************************************************
//  IMPORT WORLD OBJECT
//******************************************************************
WorldObject=GameBlenderImport::Import(std::string(GameWindow::CWD+"\\Payload\\Models\\World.GameEngine"));

//******************************************************************
//  LOAD 8 BUGGY OBJECTS
//  LOAD 1 HEALTH AND 1 WEAPON OBJECT
//  LOAD 30 INANIMATE OBJECTS
//  LOAD 1 WORLD OBJECT IN MAIN VERTEX SHADER
//******************************************************************
GameGPUObjectLoader::AddObject(Objects[0]);
GameGPUObjectLoader::AddObject(Objects[1]);
GameGPUObjectLoader::AddObject(Objects[2]);
GameGPUObjectLoader::AddObject(Objects[3]);
GameGPUObjectLoader::AddObject(Objects[4]);
GameGPUObjectLoader::AddObject(Objects[5]);
GameGPUObjectLoader::AddObject(Objects[6]);
GameGPUObjectLoader::AddObject(Objects[7]);
GameGPUObjectLoader::AddObject(Objects[8]);
GameGPUObjectLoader::AddObject(Objects[9]);
for(int i=0;i<30;i++)
GameGPUObjectLoader::AddObject(InanimateObject[i]);
GameGPUObjectLoader::AddObject(WorldObject);

//******************************************************************
//  LOAD THE HELP SECTION BOX WHICH IS DISPLAYED AT THE
//  BEGINNING OF THE GAME ONLY ONCE
//******************************************************************
GameEffects::HelpSection* Help=new GameEffects::HelpSection();
Help->TextureLocation=GameWindow::CWD+"\\Payload\\Textures\\HelpBoxDiffuseMap.bmp";
Help->Vertices=GameBlenderImport::ImportEffects(std::string(GameWindow::CWD+"\\Payload\\Models\\HelpBox.GameEffects").c_str(),&(Help->TriangleCount));
Help->Load();

/******************************************************************
  CREATE 30 DIFFERENT POINTLIGHTS AT RANDOMISED LOCATIONS AND
  LOAD THEM IN THE VERTEX SHADER

  THE ALGORITHM USED IS AS FOLLOWS:
  1) GET THE SIZE OF THE WORLD OBJECT IN X COORDINATES BY SUBTRACTING THE MINX FROM MAXX
  2) GET THE SIZE OF THE WORLD OBJECT IN Z COORDINATES BY SUBTRACTING THE MINZ FROM MAXZ
  3) GET RANDOMIZED VALUES FOR X RANGING FROM 0 TO SIZE_X
  4) GET RANDOMIZED VALUES FOR Z RANGING FROM 0 TO SIZE_Z
  5) GET RANDOMIZED VALUES FOR Y RANGING FROM 5 TO 20
  6) ADD THE VALUE IN (3) TO MINX TO GET THE NEW RANDOMIZED X COORDINATE
  7) ADD THE VALUE IN (4) TO MINZ TO GET THE NEW RANDOMIZED Z COORDINATE
  8) USE THE RANDOMIZED X ,Y AND Z VALUES TO POSITION THE LIGHTS
******************************************************************/
GameLight::PointLight Lights[30];
int worldsizeX=abs((int)WorldObject->Mesh.BoundingBox->maxx - (int)WorldObject->Mesh.BoundingBox->minx);
int worldsizeZ=abs((int)WorldObject->Mesh.BoundingBox->maxz - (int)WorldObject->Mesh.BoundingBox->minz);
int X,Y,Z;
for(int i=0;i<30;i++)
{
  X=WorldObject->Mesh.BoundingBox->minx + (rand() % worldsizeX + 0);
  Z=WorldObject->Mesh.BoundingBox->minz + (rand() % worldsizeZ + 0);
  Y= rand() % 20 + 5;
  Lights[i].Position.x=(float)X;
  Lights[i].Position.y=(float)Y;
  Lights[i].Position.z=(float)Z;
  GameGPUObjectLoader::AddObject(&Lights[i]);
}

//******************************************************************
//  CREATE ONE SUN LIGHT
//******************************************************************
GameLight::SunLight sun;
sun.Color=GameMath::Vector3(GameSettings::SunLightColorRed,GameSettings::SunLightColorGreen,GameSettings::SunLightColorBlue);
sun.Direction=GameMath::Vector3(0.0f,-1.0f,0.0f);
GameGPUObjectLoader::AddObject(&sun);

//******************************************************************
//  LOAD ALL 10 GAMEOBJECTS, THE WORLD OBJECT AND THE GAME LIGHTS
//  IN THE MAIN SHADER SO THEY CAN ALL BE RENDERED ON THE SCREEN
//******************************************************************
GameGPUObjectLoader::Load();

//******************************************************************
//  PIXEL MAPPING IS DONE FOR ALL THOSE OBJECTS THAT THE USER CAN
//  INTERACT WITH WHICH IS WHY ALL THE GAMEWORLDCUBE OBJECTS THAT
//  MAKE UP THE GAMEWORLD OBJECT AND THE BOUNDING BOXES OF THE
//  GAMEOBJECTS EXCLUDING INANIMATE OBJECTS ARE PIXEL MAPPED
//
//  PIXEL MAPPING IS THE PROCESS OF ASSIGNING A UNIQUE COLOR TO AN OBJECT
//  WHICH CAN BE USED TO IDENTIFY THAT OBJECT
//  THAT COLOR IS MAPPED AT THAT OBJECT AND SAVED IN A COLLECTION OR A MAP
//  LATER SO BY READING A PIXEL ON THE SCREEN AND ITS UNIQUE COLORS
//  WE CAN TELL WHICH OBJECT THE USER IS POINTING TO WITH THE MOUSE CURSOR
//******************************************************************
GameWorld World=GameWorld(WorldObject->BoundingBox->minx,WorldObject->BoundingBox->maxx,WorldObject->BoundingBox->miny,WorldObject->BoundingBox->maxy,WorldObject->BoundingBox->minz,WorldObject->BoundingBox->maxz);
GameWindow::PixelMapping(Objects[0]->BoundingBox);
GameWindow::PixelMapping(Objects[1]->BoundingBox);
GameWindow::PixelMapping(Objects[2]->BoundingBox);
GameWindow::PixelMapping(Objects[3]->BoundingBox);
GameWindow::PixelMapping(Objects[4]->BoundingBox);
GameWindow::PixelMapping(Objects[5]->BoundingBox);
GameWindow::PixelMapping(Objects[6]->BoundingBox);
GameWindow::PixelMapping(Objects[7]->BoundingBox);
GameWindow::PixelMapping(Objects[8]->BoundingBox);
GameWindow::PixelMapping(Objects[9]->BoundingBox);

//******************************************************************
//  THE INFO PANELS WHICH DISPLAY THE HEALTH AND THE WEAPON STATUS
//  ARE SETUP FOR EACH BUGGY OBJECT
//******************************************************************
GameMath::Vector3 LOC;
GameText Info[]=
{
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
GameText(GameWindow::CWD+"\\Payload\\TextTextures\\Text.bmp",&FPSCamera),
};

//******************************************************************
//  SETUP AND POSITIONING OF BUGGY OBJECTS AND CAMERA
//******************************************************************
Objects[0]->ObjectType=1;//Friendly
Objects[1]->ObjectType=1;//Friendly
Objects[2]->ObjectType=1;//Friendly
Objects[3]->ObjectType=1;//Friendly
Objects[4]->ObjectType=2;//Enemy
Objects[5]->ObjectType=2;//Enemy
Objects[6]->ObjectType=2;//Enemy
Objects[7]->ObjectType=2;//Enemy
Objects[4]->Speed=GameSettings::EnemySpeedMin;//Enemy - speed in meters per milisecond
Objects[5]->Speed=GameSettings::EnemySpeedMin;//Enemy - speed in meters per milisecond
Objects[6]->Speed=GameSettings::EnemySpeedMin;//Enemy - speed in meters per milisecond
Objects[7]->Speed=GameSettings::EnemySpeedMin;//Enemy - speed in meters per milisecond
Objects[8]->ObjectType=3;//Health Box
Objects[9]->ObjectType=4;//Weapon Box
Objects[8]->isAlive=false;//Health Box
Objects[9]->isAlive=false;//Weapon Box

//MAKE THE WORLD ROTATE AT AN INTERVAL
//MAKE THE WORLD TEXTURE CHANGE
//HAVE TCP/IP NETWORK GAMEPLAY INVOLVED
// HAVE A GAMEOVER BOX OVER IMPLEMENTED
// AND A RESET BOX IMPLEMENTED

//******************************************************************
// CALCULATING THE MID POINT OF THE WORLD OBJECT AND THEN USING
// AN OBJECT OF 6 WORLDCUBES, RANDOM POSITIONS IN THE WORLD SPACE IS
// CALCULATED FOR THE 8 BUGGIES AT WHICH THEY ARE FINALLY PLACED
// DURING THE START OF THE GAMEPLAY
//******************************************************************
int WorldMidZ=World.xbound/2;
int WorldMidX=World.zbound/2;
int Gap=6;
GameWorldCube* WRDC[8];
WRDC[0]=(World.WorldCoordMap[WorldMidZ])[WorldMidX];
WRDC[1]=(World.WorldCoordMap[WorldMidZ])[WorldMidX+Gap];
WRDC[2]=(World.WorldCoordMap[WorldMidZ+Gap])[WorldMidX];
WRDC[3]=(World.WorldCoordMap[WorldMidZ-Gap])[WorldMidX];
WRDC[4]=(World.WorldCoordMap[WorldMidZ])[WorldMidX-Gap];
WRDC[5]=(World.WorldCoordMap[WorldMidZ-Gap])[WorldMidX-Gap];
WRDC[6]=(World.WorldCoordMap[WorldMidZ+Gap])[WorldMidX+Gap];
WRDC[7]=(World.WorldCoordMap[WorldMidZ-Gap])[WorldMidX+Gap];

for(int i=0;i<ObjectCount-2;i++)
{
    WRDC[i]->Object=Objects[i];
    Objects[i]->SourceWorldCube=WRDC[i];

    LOC=WRDC[i]->Centroid;
    LOC.y=(Objects[i]->BoundingBox->maxy - Objects[i]->BoundingBox->miny)/2.0f;
    Objects[i]->BuggyTranslationMatrix=GameMath::Transform::Translate(LOC);
    Objects[i]->BuggyFinalMatrix=Objects[i]->BuggyTranslationMatrix *  Objects[i]->BuggyRotationMatrix;
    Objects[i]->BoundingBox->Centroid.x=Objects[i]->BuggyTranslationMatrix.Matrix[3][0];
    Objects[i]->BoundingBox->Centroid.y=Objects[i]->BuggyTranslationMatrix.Matrix[3][1];
    Objects[i]->BoundingBox->Centroid.z=Objects[i]->BuggyTranslationMatrix.Matrix[3][2];
    Objects[i]->BoundingBox->CalculateXYZ();
    Objects[i]->_Bullet->CannonPosition=Objects[i]->BuggyFinalMatrix *  Objects[i]->Armature.Bones["MachineGun"]->Head;
    Objects[i]->_Explosion->BBCentroid=Objects[i]->BoundingBox->Centroid;
    Objects[i]->_Explosion->BBCentroid.y=0.0f;

    //CAMERA IS PLACED AT THE MIDDLE OF THE WORLD OBJECT BUT SLIGHTLY OFFSET BY THE Z COORDINATE
    if(i==0)
    {
        LOC.z=LOC.z+10.0f;
        LOC.y=LOC.y+2.0f;
        FPSCamera.CameraPosition=LOC;
    }
}

//******************************************************************
// POSITIONING OF THE INANIMATE OBJECTS
//******************************************************************
for(int i=0;i<30;i++)
SpawnInanimateObjects(InanimateObject[i],&World);

//******************************************************************
// SETTING UP SKYBOX
//******************************************************************
SkyBox skybox(GameWindow::CWD+"\\Payload\\SkyBox\\");

//**********************************************
// BACKGROUND FRAMEBUFFER INITIALIZATION
//**********************************************
/*
OPENGL FUNCTIONS:
glGenFramebuffers - generate framebuffer object names. the names are basically integer numbers that uniquely identifies that framebuffer
glBindFramebuffer - bind a framebuffer to a framebuffer target.target must be either GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_FRAMEBUFFER.
                    you specify the framebuffer using that integer name that you got from glGenFramebuffers() function
glGenTextures - generate texture names which are integer numbers
glBindTexture - bind a named texture to a texturing target. target must be one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D,
                GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER,
                GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY
glTexImage2D - specify a two-dimensional texture image.the first argument specifies the target texture
glTexParameteri - set texture parameters
glFramebufferTexture2D - attach a level of a texture object as a logical buffer of a framebuffer object
glGenRenderbuffers - generate renderbuffer object names which are integer numbers
glBindRenderbuffer - bind a renderbuffer to a renderbuffer target
glRenderbufferStorage - establish data storage, format and dimensions of a renderbuffer object's image
glFramebufferRenderbuffer - attach a renderbuffer as a logical buffer of a framebuffer object
*/
unsigned char pixels[4] = { 0 };
unsigned int framebuffer;
glGenFramebuffers(1, &framebuffer);
glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

unsigned int textureColorbuffer;
glGenTextures(1, &textureColorbuffer);
glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GameWindow::Width, GameWindow::Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

unsigned int rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GameWindow::Width, GameWindow::Height);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
glBindFramebuffer(GL_FRAMEBUFFER,0);
//******************************************************************
// GAME LOOP VARIABLES INITIALIZATION
//******************************************************************
MSG msg;
const int DESIRED_TIME_TAKEN_PER_FRAME=1000 / 120;
DWORD FrameStartTime;
DWORD Timer;
DWORD HealthWeaponTimer=0;
DWORD AITimer=0;
bool AIUpdated=false;
POINT MousePointerInfo;
GameWindow::Pixel* PixelPointer;
GameBoundingBox* PlayerBuggyBB=nullptr;
GameBoundingBox* EnemyBuggyBB=nullptr;
GameBoundingBox* HealthWeaponBB=nullptr;
GameWorldCube* DestinationWorldCube=nullptr;
GameObject* TestObject=nullptr;
DWORD LightDimTimer=0;
float LightsDimEffect=1.0f;
bool LightHigh=false;
bool LightLow=true;
bool isCameraShaking=false;
bool HelpAcknowleged=false;
GameMath::Vector3 OriginalCameraPosition=FPSCamera.CameraPosition;

//**********************************************
//    GAME LOOP STARTS HERE
//**********************************************
while(GameWindow::KeepAlive)
{
  /********************************************
  // WINDOWS MESSAGE LOOP IS IMPLEMENTED USING THE PeekMessage() FUNCTION
  // WHICH IS RUN UNDER A WHILE LOOP WITH A TIMER IN PLACE
  // SO IF A LOT OF WINDOWS EVENT MESSAGES GET INTO THE MESSAGE QUEUE
  // THEN PeekMessage() FUNCTION CAN HANDLE AS MANY AS POSSIBLE WITHIN THE
  // DESIRED TIME IT TAKES TO RENDER ONE FRAME AFTER THAT THE GRAPHICS RENDERING TAKES PLACE
  // NO MATTER WHAT. THIS AVOIDS ANY MAJOR LAGS IN THE GRAPHICS RENDERING PROCESS
  *********************************************/
  FrameStartTime=GetTickCount();
  Timer=FrameStartTime + DESIRED_TIME_TAKEN_PER_FRAME;
  while(GetTickCount() < Timer)
  {
      if(PeekMessage(&msg,GameWindow::WindowHandle, 0, 0, PM_REMOVE))
      {
        if(msg.message == WM_QUIT || (GetKeyState(27) & 0x8000))
        GameWindow::KeepAlive=false;
        else
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
      else
      break;
  }

  //**********************************************************************
  //   CALCULATING CAMERA VIEW MATRIX
  //**********************************************************************
  FPSCamera.UpdateCameraView();

  //**************************************************************************************
  //   RE-CALCULATING PRPOJECTION MATRIX IF VIEWPORT CHANGES DUE TO RESIZING OF GAMEWINDOW
  //**************************************************************************************
  if(GameWindow::ViewPortChanged==true)
  Projection=GameMath::Transform::PerspectiveView((float)GameWindow::Width/(float)GameWindow::Height,0.1f, 1000.0f, 60.0f);

  //**************************************************************************************
  //   DISPLAYING THE HELP SECTION SO THE USER CAN READ THE INSTRUCTIONS
  //   AND THEN PRESS "ENTER" ON THE KEYBOARD TO PROCEED FURTHER
  //   UNTIL THAT HAPPENS THE GAME WILL NOT CONTINUE
  //   AFTER THE USER PRESSES THE "ENTER" KEY THEN THE GAME WILL PROCEED FURTHER
  //   AND THE CAMERA POSITION WILL BE RESTORED TO ITS ORIGINAL POSITION
  //**************************************************************************************
  if(HelpAcknowleged==false)
  {
      HelperShader.Use();
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
      glUniformMatrix4fv(3 ,1, GL_FALSE, FPSCamera.View.Addressof());
      glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());
      Help->Draw();
      SwapBuffers(GameWindow::DeviceContext);

      if(GetKeyState(13) & 0x8000)
      {
	  HelpAcknowleged=true;
	  FPSCamera.CameraPosition=OriginalCameraPosition;
	  FPSCamera.YawAngle=0.0f;
	  FPSCamera.PitchAngle=0.0f;
      }
      continue;
  }

  //**********************************************************************
  //   ENABLING THE MAIN SHADER AND RESETTING OPENGL COLOR AND DEPTH BITS
  //**********************************************************************
  /*
  OPENGL FUNCTIONS:
  glClear - clear buffers to preset values
  */
  MainShader.Use();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //**************************************************************************************
  //  UPDATING THE MODEL,VIEW AND PROJECTION MATRIX IN THE MAIN SHADER
  //  ALONG WITH THE CAMERA POSITION AND THE TOTAL NUMBER OF LIGHTS
  //**************************************************************************************
  /*
  OPENGL FUNCTIONS:
  glUniform - Specify the value of a uniform variable for the current program object or shader object bound to the
              currently bound framebuffer
  glUniformMatrix4fv - Specify the value of a uniform 4x4 matrix variable using the address of the first element of the matrix
  glUniform4f - Specify the value of a uniform 4d vector of floating point values.
  glUniform1i - Specify the value of a uniform integer
  */
  Model.Clean();
  glUniformMatrix4fv(4, 1, GL_FALSE, Model.Addressof());
  glUniformMatrix4fv(5 ,1, GL_FALSE, FPSCamera.View.Addressof());
  glUniformMatrix4fv(6, 1, GL_FALSE, Projection.Addressof());
  glUniform4f(9,FPSCamera.CameraPosition.x,FPSCamera.CameraPosition.y,FPSCamera.CameraPosition.z,1.0f);
  glUniform1i(10,GameLight::TotalLights);

  //****************************************************************************
  //  PROCESS BUGGY MOVEMENTS AND ATTACKS FOR PLAYER BUGGIES
  //  PROCESS BUGGY MOVEMENTS, ATTACKS AND UPDATES FOR AI BUGGIES
  //****************************************************************************
  for(int i=0;i<ObjectCount;i++)
  {
   if(Objects[i]->isAlive==true)
   {
       //*****************************************************************************
       //   PLAYER BUGGIES MOVE ONLY WHEN THEIR "DESTINATIONWORLDCUBE" VARIABLE IS SET
       //   PLAYER BUGGIES ATTACK ONLY WHEN THEIR "ATTACKOBJECT" VARIABLE IS SET
       //*****************************************************************************
       if(Objects[i]->ObjectType==1)
       {
	   Objects[i]->WorldMovement(GameWindow::Time_Taken_Per_Frame,&HealthCube,&WeaponCube);
	   Objects[i]->Attack(&Objects[0],ObjectCount,&InanimateObject[0],InanimateObjectCount);
       }

       /*******************************************************************************
       // AI BUGGIES FOLLOW THIS SIMPLE ALGORITHM:
       // 1) IF THEIR HEALTH IS BELOW 30 % AND A HEALTH BOX IS PRESENT THEN IT WILL GO TOWARDS THAT HEALTH BOX
       // 2) IF THEIR HEALTH IS ABOVE 30 % BUT THE AMMO IS BELOW 30 % AND A WEAPON BOX IS PRESENT THEN
       //    IT WILL GO TOWARDS THE WEAPON BOX
       // 3) IF BOTH HEALTH AND AMMO ARE OVER 30 % THEN IT WILL LOOK FOR PLAYER BUGGIES TO ATTACK
       //    USING THE FIND() FUNCTION
       // 4) IF ITS ABLE TO FIND A PLAYER BUGGY OBJECT THEN IT WILL DO THE FOLLOWING:
       //    A) IF THE TARGET IS OUT OF RANGE THEN IT WILL PURSUE THE TARGET OBJECT
       //    B) ELSE IF THE TARGET IS IN WAEPONS RANGE THEN IT WILL START ATTACKING THE
       //       TARGET OBJECT IMMEDIATELY
       // 5) THE AI BUGGY WILL INCREASE ITS SPEED BY 0.01 EVERY 20 SECONDS UNTILL IT
       //    ATTAINS THE SAME SPEED AS THE PLAYER BUGGY OBJECT WHICH IS 0.8
       *********************************************************************************/
       if(Objects[i]->ObjectType==2)
       {
          if(Objects[i]->Health <= GameSettings::EnemyHealthBeforeReload && HealthCube!=nullptr)
          Objects[i]->DestinationWorldCube=HealthCube;
          else if(Objects[i]->WeaponQuantity <= GameSettings::EnemyWeaponBeforeReload && WeaponCube!=nullptr )
          Objects[i]->DestinationWorldCube=WeaponCube;
          else if(Objects[i]->AttackObject==nullptr)
          {
            GameObject* SearchObject=Objects[i]->Find(&Objects[0],ObjectCount);
            if(SearchObject!=nullptr)
            {
               if(Objects[i]->TargetOutOfRange==true)
               Objects[i]->DestinationWorldCube=SearchObject->SourceWorldCube;
               else
               Objects[i]->AttackObject=SearchObject;
            }
          }

          if(Objects[i]->Speed < GameSettings::EnemySpeedMax &&  GetTickCount() > AITimer + GameSettings::EnemySpeedIncreaseInterval)
          {
              Objects[i]->Speed=Objects[i]->Speed + GameSettings::EnemySpeedIncreaseRate;
              AIUpdated=true;
          }

          Objects[i]->WorldMovement(GameWindow::Time_Taken_Per_Frame,&HealthCube,&WeaponCube);
          Objects[i]->Attack(&Objects[0],ObjectCount,&InanimateObject[0],InanimateObjectCount);
       }
   }
  }

  if(AIUpdated==true)
  {
      AITimer=GetTickCount();
      AIUpdated=false;
  }

  //****************************************************
  //   GENERATE HEALTH AND WEAPON OBJECTS EVERY 40 SECONDS
  //****************************************************
  if(GetTickCount() > HealthWeaponTimer + GameSettings::HealthAndWeaponSpawnTime)
  {
    SpawnHealth(Objects[8],&World);
    SpawnWeapon(Objects[9],&World);
    HealthWeaponTimer=GetTickCount();
  }

  //******************************************************
  //   *****    ******   **  *   ******   *****  *****   *
  //   *   *    *        * * *   *    *   ***    *   *   *
  //   *****    ***      *  **   *    *   *      *****   *
  //   *     *  ******   *   *   ******   *****  *     * *
  //******************************************************
  //******************************************************
  //   HAVE THE POINT LIGHTS VARY THEIR LIGHT INTENSITY
  //   BETWEEN 0.08 AND 0.9 EVERY 20 SECONDS TO CREATE
  //   A SUNRISE AND SUNSET EFFECT. THE SUNSET LASTS 10 SECONDS
  //   WHILE SUNRISE LASTS 20 SECONDS
  //******************************************************
  //SUNRISE EFFECT USING POINT LIGHTS
  if(LightLow==true && GetTickCount() > LightDimTimer + GameSettings::LampLightOnTime)
  {
      LightsDimEffect-=GameSettings::LampLightChangeSpeed;
      if(LightsDimEffect < GameSettings::LampLightMin)
      {
	      LightHigh=true;
	      LightLow=false;
	      LightDimTimer=GetTickCount();
      }
      else
      {
          for(int i=0;i<30;i++)
          Lights[i].UpdateColorXYZ(LightsDimEffect,LightsDimEffect,LightsDimEffect);
      }
  }
  //SUNSET EFFECT USING POINT LIGHTS
  else if(LightHigh==true && GetTickCount() > LightDimTimer + GameSettings::LampLightOffTime)
  {
      LightsDimEffect+= GameSettings::LampLightChangeSpeed;
      if(LightsDimEffect > GameSettings::LampLightMax)
      {
	      LightHigh=false;
          LightLow=true;
          LightDimTimer=GetTickCount();
      }
      else
      {
	      for(int i=0;i<30;i++)
	      Lights[i].UpdateColorXYZ(LightsDimEffect,LightsDimEffect,LightsDimEffect);
      }
  }
  //******************************************************************************************
  //   USING THE MAIN SHADER
  //   DRAW THE WORLD
  //   DRAW ALL THE ACTIVE GAMEOBJECTS WHICH INCLUDES 8 BUGGIES, 1 HEALTH BOX AND 1 WEAPON BOX
  //   DRAW ALL THE INANIMATE OBJECTS
  //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
  //******************************************************************************************
  WorldObject->Draw();

  for(int i=0;i<ObjectCount;i++)
  {
   if(Objects[i]->isAlive==true)
   Objects[i]->Draw();
  }

  for(int i=0;i<30;i++)
  InanimateObject[i]->Draw();

  //*********************************************
  //   USING THE HELPER SHADER
  //   DRAW THE CURRENTLY SELECTED GAMEWORLDCUBE IN YELLOW TINT
  //   DRAW THE CURRENTLY SELECTED PLAYER BUGGY  BOX IN BLUE TINT
  //   DRAW THE CURRENTLY SELECTED ENEMY BUGGY BOX IN RED TINT
  //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
  //*********************************************
  /*
  OPENGL FUNCTIONS:
  glBlendFunc - Pixels can be drawn using a function that blends the incoming (source) RGBA values with the RGBA values that
                are already in the frame buffer (the destination values). Blending is initially disabled. Use glEnable and glDisable
                with argument GL_BLEND to enable and disable blending.
                glBlendFunc defines the operation of blending for all draw buffers when it is enabled
                sfactor specifies which method is used to scale the source color components.
                dfactor specifies which method is used to scale the destination color components
  */
  HelperShader.Use();
  Model.Clean();
  glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
  glUniformMatrix4fv(3 ,1, GL_FALSE, FPSCamera.View.Addressof());
  glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if(PlayerBuggyBB!=nullptr)
  {
     if(PlayerBuggyBB->Object->isAlive==true)
     {
	 PlayerBuggyBB->Load();
	 PlayerBuggyBB->DrawForeground();
     }
  }

  if(EnemyBuggyBB!=nullptr)
  {
     if(EnemyBuggyBB->Object->isAlive==true)
     {
	 EnemyBuggyBB->Load();
	 EnemyBuggyBB->DrawForeground();
     }
  }

  if(HealthWeaponBB!=nullptr)
  {
     if(HealthWeaponBB->Object->isAlive==true)
     {
	HealthWeaponBB->Load();
	HealthWeaponBB->DrawForeground();
     }
  }

  if(DestinationWorldCube!=nullptr)
  {
      DestinationWorldCube->DrawForeground();
  }

  //******************************************
  //   USING THE HELPER SHADER
  //   DRAW THE ACTIVE BULLET OBJECT
  //   DRAW THE ACTIVE BULLETFIRE OBJECT
  //   DRAW THE ACTIVE EXPLOSION OBJECT
  //
  //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER ONLY FOR BUGGY OBJECTS
  //
  //   IN CASE OF EXPLOSION, SET THE GLOBAL "ISCAMERASHAKING" FLAG TO TRUE
  //   AND SET THE EXPLOSION OBJECT'S LOCAL "ISCAMERASHAKING" FLAG TO TRUE SO
  //   ONLY ONE EXPLOSION CAUSES THE CAMERA TO SHAKE AT A TIME. ONCE THIS
  //   EXPLOSION COMPLETES THEN THE GLOABL "ISCAMERASHAKING" FLAG WILL BE SET TO
  //   FALSE AGAIN SO SOME OTHER EXPLOSION GETS TO SHAKE THE CAMERA
  //
  //   IF ANOTHER EXPLOSION OCCURS THEN ITS LOCAL "ISCAMERASHAKING" FLAG WILL
  //   ONLY BE SET TO TRUE IF THE GLOBAL "ISCAMERASHAKING" FLAG IS SET TO FALSE
  //   THIS WILL MAKE SURE THAT ONLY ONE EXPLOSION OBJECT GETS TO MAKE THE
  //   CAMERA SHAKE.
  //******************************************
  for(int i=0;i<ObjectCount-2;i++)
  {
   if(Objects[i]->isAlive==true)
   {
     if(Objects[i]->_Bullet->isAlive==true)
     Objects[i]->_BulletFire->Draw(Objects[i]->_Bullet->Draw());
   }

   if(Objects[i]->_Explosion->isExploding==true)
   {
       if(isCameraShaking==false)
       {
	   Objects[i]->_Explosion->isCameraShaking=true;
	   isCameraShaking=true;
       }
       Objects[i]->_Explosion->Draw(&FPSCamera,&isCameraShaking);
   }
  }

  //***********************************************************************************
  //   USING THE HELPER SHADER
  //   DRAW THE HEALTH AND THE WEAPON STATUS BOXES FOR EACH ACTIVE / ALIVE BUGGY OBJECT
  //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER ONLY FOR BUGGY OBJECTS
  //***********************************************************************************
  for(int i=0;i<ObjectCount-2;i++)
  {
   if(Objects[i]->isAlive==true)
   {
       Info[i].Update(Objects[i]->Health,Objects[i]->WeaponQuantity,&(Objects[i]->BoundingBox->Centroid));
       Info[i].Draw();
   }
  }

  //*******************************************************
  //   USING THE HELPER SHADER DRAW SKYBOX AT THE VERY END
  //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
  //   SO THE SKYBOX REMAINS AHEAD OF ALL OBJECTS AND VISIBLE ALL THE TIME
  //*******************************************************
  Model.Clean();
  glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
  skybox.Draw();

  //******************************************
  //   USING THE HELPER SHADER
  //   DRAW THE WORLD OBJECT WHICH INVOLVES DRAWING THE ALL THE GAMEWORLDCUBE OBJECTS WITH UNIQUE COLORS
  //   DRAW ALL THE BOUNDING BOX OBJECTS FOR ALL THE BUGGY OBJECTS WITH UNIQUE COLORS
  //   DRAW ALL THE BOUNDING BOX OBJECTS FOR THE HEALTH AND WEAPON BOX WITH UNIQUE COLORS
  //   IN THE BACKGROUND FRAMEBUFFER
  //******************************************
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  HelperShader.Use();
  glDisable(GL_BLEND);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(GameWindow::ViewPortChanged==true)
  {
      //VIEWPORT IS A GLOBAL STATE FOR OPENGL
      //WHEN IT CHANGES, UPDATE THE BACKGROUND FRAMEBUFFER'S TEXTURE BUFFER AND DEPTH BUFFER WITH NEW WINDOW WIDTH AND HEIGHT
      glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GameWindow::Width, GameWindow::Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

      glBindRenderbuffer(GL_RENDERBUFFER, rbo);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,GameWindow::Width,GameWindow::Height);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

      GameWindow::ViewPortChanged=false;
  }

  glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
  glUniformMatrix4fv(3 ,1, GL_FALSE, FPSCamera.View.Addressof());
  glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());

  World.Draw();

  for(int i=0;i<ObjectCount;i++)
  {
    if(Objects[i]->isAlive==true)
    {
        Objects[i]->BoundingBox->Load();
        Objects[i]->BoundingBox->DrawBackground();
    }
  }

  //***************************************************
  //   READ THE PIXEL COLOR VALUES AT THE MOUSE CURSOR
  //   USE THE RGB COLOR VALUES FOR THAT PIXEL AS THE KEY FOR THE "PixelMap" COLLECTION
  //   THE OBJECT RETRIEVED OFF THE "PixelMap" COLLECTION CAN BE OF TYPE:
  //   A) NULLPTR - INDICATING THAT THE MOUSE IS POINTING AT THE WORLD BACKGROUND
  //   B) GAMEWORLDCUBE - INDICATING THAT THE MOUSE IS POINTING AT A GAMEWORLDCUBE
  //   C) BOUNDINGBOX - INDICATING THAT THE MOUSE IS POINTING AT A BOUNDING BOX OF ONE OF THE
  //                    BUGGY OBJECTS, HEALTH BOX OBJECT OR WEAPON BOX OBJECT
  //***************************************************
  /*
  OPENGL FUNCTIONS:
  glReadBuffer - glReadBuffer specifies a color buffer as the source for subsequent glReadPixels, glCopyTexImage1D,
                 glCopyTexImage2D, glCopyTexSubImage1D, glCopyTexSubImage2D, and glCopyTexSubImage3D commands
  glReadPixels - glReadPixels and glReadnPixels return pixel data from the frame buffer, starting with the pixel
                 whose lower left corner is at location (x, y), into client memory starting at location data.
                 Several parameters control the processing of the pixel data before it is placed into client memory

  WINDOWS FUNCTIONS:
  GetCursorPos - Retrieves the position of the mouse cursor, in screen coordinates.
  ScreenToClient - The function uses the window identified by the hWnd parameter and the screen coordinates given in the POINT structure
                   to compute client coordinates. It then replaces the screen coordinates with the client coordinates
  */
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  GetCursorPos(&MousePointerInfo);
  ScreenToClient(GameWindow::WindowHandle,&MousePointerInfo);
  glReadPixels(MousePointerInfo.x, GameWindow::Height - MousePointerInfo.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE,pixels);
  std::string Key=GameWindow::ReadKey(pixels[0],pixels[1],pixels[2]);
  PixelPointer=GameWindow::PixelMap[Key];

  //***************************************************
  // PROCESS USER MOUSE INPUT:
  // 1) CHECK IF USER HAS SELECTED ONE OF THE PLAYER BUGGY OBJECTS AS THOSE ARE THE ONLY OBJECTS THAT THE USER CAN CONTROL
  // 2) IF NO PLAYER BUGGY OBJECT IS SELECTED THEN
  //    A) CHECK IF THE MOUSE IS HOVERING OVER ONE OF THE PLAYER BUGGY OBJECTS. IF YES THEN HIGHLIGHT THAT OBJECT IN BLUE
  //    B) CHECK IF THE USER HAS CLICKED ON THE LEFT MOUSE BUTTON WITH THE PLAYER OBJECT HIGHLIGHTED
  //       IF YES THEN SET THIS OBJECT AS THE "PLAYERBUGGYBB" OBJECT AND QUIT AFTER RESETTING ALL "PLAYERBUGGYBB" ,"ENEMYBUGGYBB"
  //       ,"HEALTHWEAPONBB" AND "DESTINATIONWORLDCUBE" TO NULLPTR
  //
  //    IF A PLAYER BUGGY OBJECT IS ALREDY SELECTED THEN
  //    A) CHECK IF THE USER HAS PRESSED THE MOUSE RIGHT BUTTON. IF YES THEN RAISE THE "HALT" FLAG OF THE "PLAYERBUGGYBB" OBJECT
  //       AND QUIT
  //     B) IF THE USER HAS NOT PRESSED THE MOUSE RIGHT BUTTON THEN CHECK WHICH OBJECT THE MOUSE IS HOVERING OVER:
  //       i) IF THE OBJECT IS GAMEWORLDCUBE THEN HIGHLIGHT THIS OBJECT IN YELLOW
  //       ii) CHECK IF THE USER HAS PRESSED THE MOUSE LEFT BUTTON WITH THIS OBJECT HIGHLIGHTED
  //           IF YES THEN SET "PLAYERBUGGYBB" OBJECT'S DESTINATIONWORLDCUBE TO THIS WORLDCUBE AND THEN QUIT AFTER RESETTING ALL
  //            "PLAYERBUGGYBB" ,"ENEMYBUGGYBB", "HEALTHWEAPONBB" AND "DESTINATIONWORLDCUBE" TO NULLPTR
  //
  //       i) IF THE OBJECT IS A BOUNDING BOX OBJECT AND IF ITS A BUGGY OBJECT AND ITS NOT THE SAME OBJECT AS THE PLAYER BUGGY OBJECT
  //          THEN HIGHLIGHT THIS OBJECT IN RED
  //       ii) CHECK IF THE USER HAS PRESSED THE MOUSE LEFT BUTTON WITH THIS OBJECT HIGHLIGHTED
  //           IF YES THEN SET "PLAYERBUGGYBB" OBJECT'S ATTACKOBJECT TO THE HIGHLIGHTED OBJECT AND THEN QUIT AFTER RESETTING ALL
  //           "PLAYERBUGGYBB" ,"ENEMYBUGGYBB", "HEALTHWEAPONBB" AND "DESTINATIONWORLDCUBE" TO NULLPTR
  //
  //       i) IF THE OBJECT IS A BOUNDING BOX OBJECT AND ITS A WEAPON OR A HEALTH BOX THEN HIGHLIGHT THIS OBJECT IN GREEN
  //       ii) CHECK IF THE USER HAS PRESSED THE MOUSE LEFT BUTTON WITH THIS OBJECT HIGHLIGHTED
  //           IF YES THEN SET "PLAYERBUGGYBB" OBJECT'S DESTINATIONWORLDCUBE TO THIS OBJECT'S WORLDCUBE AND THEN QUIT AFTER RESETTING ALL
  //           "PLAYERBUGGYBB" ,"ENEMYBUGGYBB", "HEALTHWEAPONBB" AND "DESTINATIONWORLDCUBE" TO NULLPTR
  //***************************************************


  //PLAYER BUGGY IS NOT SELECTED SO SELECT A PLAYER BUGGY FIRST
  if(PlayerBuggyBB==nullptr)
  {
    if(PixelPointer->type==2)//MOUSE HOVERING OVER BOUNDING BOX WHICH ARE BOTH PLAYER AND AI BUGGIES
    {
      if(((GameBoundingBox*)PixelPointer->payload)->Object->ObjectType==1 && GameWindow::MouseLButtonPressed==true)//ONLY SELECT PLAYER BUGGES
      {
 	    PlayerBuggyBB=(GameBoundingBox*)PixelPointer->payload;
 	    PlayerBuggyBB->ColorTint.x=GameSettings::PlayerSelectColorRed;
 	    PlayerBuggyBB->ColorTint.y=GameSettings::PlayerSelectColorGreen;
 	    PlayerBuggyBB->ColorTint.z=GameSettings::PlayerSelectColorBlue;
        GameWindow::MouseLButtonPressed=false;
      }
    }
  }


  //PLAYER BUGGY IS SELECTED
  if(PlayerBuggyBB!=nullptr)
  {
    if(GameWindow::MouseRButtonPressed==true)//RAISING "HALT" FLAG ON THE OBJECT WITH MOUSE RIGHT CLICK
    {
	PlayerBuggyBB->Object->HALT=true;
	PlayerBuggyBB=nullptr;
	DestinationWorldCube=nullptr;
	EnemyBuggyBB=nullptr;
	HealthWeaponBB=nullptr;
	GameWindow::MouseRButtonPressed=false;
    }
    else if(PixelPointer->type==1)//MOUSE HOVERING OVER GAMEWORLDCUBE
    {
        DestinationWorldCube=(GameWorldCube*)PixelPointer->payload;
        DestinationWorldCube->Load(World.SmallestDivision);
        EnemyBuggyBB=nullptr;

        if(GameWindow::MouseLButtonPressed==true)
        {
          PlayerBuggyBB->Object->DestinationWorldCube=DestinationWorldCube;
	      PlayerBuggyBB=nullptr;
	      DestinationWorldCube=nullptr;
	      EnemyBuggyBB=nullptr;
	      HealthWeaponBB=nullptr;
	      GameWindow::MouseLButtonPressed=false;
        }
    }
    else if(PixelPointer->type==2)//MOUSE HOVERING OVER BOUNDING BOX
    {
       TestObject=((GameBoundingBox*)PixelPointer->payload)->Object;
       if(TestObject->ObjectType < 3 && TestObject->Index!=PlayerBuggyBB->Object->Index)//BOUNDING BOX IS A ENEMY BUGGY
       {
	    EnemyBuggyBB=(GameBoundingBox*)PixelPointer->payload;
	    EnemyBuggyBB->ColorTint.x=GameSettings::EnemySelectColorRed;
	    EnemyBuggyBB->ColorTint.y=GameSettings::EnemySelectColorGreen;
	    EnemyBuggyBB->ColorTint.z=GameSettings::EnemySelectColorBlue;
	    DestinationWorldCube=nullptr;

	    if(GameWindow::MouseLButtonPressed==true)
	    {
	        PlayerBuggyBB->Object->AttackObject=EnemyBuggyBB->Object;
	        PlayerBuggyBB=nullptr;
	        DestinationWorldCube=nullptr;
	        EnemyBuggyBB=nullptr;
	        HealthWeaponBB=nullptr;
            GameWindow::MouseLButtonPressed=false;
	    }
       }
       else if(TestObject->ObjectType >= 3)//BOUNDING BOX IS A HEALTH OR A WEARPON CUBE
       {
	    HealthWeaponBB=(GameBoundingBox*)PixelPointer->payload;
	    HealthWeaponBB->ColorTint.x=GameSettings::HealthWeaponSelectColorRed;
	    HealthWeaponBB->ColorTint.y= GameSettings::HealthWeaponSelectColorGreen;
	    HealthWeaponBB->ColorTint.z= GameSettings::HealthWeaponSelectColorBlue;
	    DestinationWorldCube=nullptr;
	    EnemyBuggyBB=nullptr;

	    if(GameWindow::MouseLButtonPressed==true)
            {
	        PlayerBuggyBB->Object->DestinationWorldCube=HealthWeaponBB->Object->SourceWorldCube;
	        PlayerBuggyBB=nullptr;
	        DestinationWorldCube=nullptr;
	        EnemyBuggyBB=nullptr;
	        HealthWeaponBB=nullptr;
	        GameWindow::MouseLButtonPressed=false;
	    }
       }
     }
  }



  //*********************************************************************************************************
  //  REVERTING TO THE DEFAULT FRAME BUFFER FROM THE BACKGROUND FRAME BUFFER SO IT CAN RENDERED ON THE SCREEN
  //*********************************************************************************************************
  /*
  WINDOWS FUNCTIONS:
  SwapBuffers - The SwapBuffers function exchanges the front and back buffers if the current pixel format
                for the window referenced by the specified device context includes a back buffer.
  */
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SwapBuffers(GameWindow::DeviceContext);

  //******************************************************
  //  CALCULATING THE TIME TAKEN FOR ONE FRAME TO PROCESS
  //******************************************************
  GameWindow::Time_Taken_Per_Frame=GetTickCount() - FrameStartTime;
}
//**********************************************
//          GAME LOOP ENDS HERE
//**********************************************

return 0;
}

