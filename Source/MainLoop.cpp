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
    Pixel* pixel = new Pixel();
    pixel->type = 2;
    pixel->payload = (void*)payload;
    GenerateKey(pixel, &payload->R, &payload->G, &payload->B);
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
GameWorldCube* RandomWorldPosition(GameWorld* World, int Attempts = 0)
{
    GameWorldCube* WC = nullptr;
    int X = 0;
    int Z = 0;
    int Counter = 0;
    while (WC == nullptr && Counter <= Attempts)
    {
        X = rand() % World->xbound + 0;
        while (X % 2 != 0)
            X = X + 1;

        Z = rand() % World->zbound + 0;
        while (Z % 2 != 0)
            Z = Z + 1;

        WC = (World->WorldCoordMap[Z])[X];
        if (WC != nullptr)
        {
            if (WC->Object != nullptr)
                WC = nullptr;
            else
                break;
        }
        
        if(Attempts > 0)
            Counter +=1;
    }
    return WC;
}
void SpawnHealth(GameWorld *World)
{
    if (!GameObject::HealthBox->isAlive)
    {
        GameWorldCube* WC = RandomWorldPosition(World, 10);
        if (WC == nullptr)
            return;

        GameMath::Vector3 LOC = WC->Centroid;
        LOC.y = LOC.y + 1.0f;
        GameObject::HealthBox->SourceWorldCube = WC;
        WC->HealthObject = GameObject::HealthBox;

        GameObject::HealthBox->BuggyFinalMatrix = GameMath::Transform::Translate(LOC);
        GameObject::HealthBox->BoundingBox->FinalMatrix = GameObject::HealthBox->BuggyFinalMatrix * GameMath::Transform::Scale(1.1f, 1.1f, 1.1f);
        GameObject::HealthBox->BoundingBox->Centroid.x = GameObject::HealthBox->BuggyFinalMatrix.Matrix[3][0];
        GameObject::HealthBox->BoundingBox->Centroid.y = GameObject::HealthBox->BuggyFinalMatrix.Matrix[3][1];
        GameObject::HealthBox->BoundingBox->Centroid.z = GameObject::HealthBox->BuggyFinalMatrix.Matrix[3][2];
        GameObject::HealthBox->BoundingBox->CalculateXYZ();
        GameObject::HealthBox->isAlive = true;
    }
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
void SpawnWeapon(GameWorld *World)
{
    if (!GameObject::WeaponBox->isAlive)
    {
        GameWorldCube* WC = RandomWorldPosition(World, 10);
        if (WC == nullptr)
            return;

        GameMath::Vector3 LOC = WC->Centroid;
        LOC.y = LOC.y + 1.0f;
        GameObject::WeaponBox->SourceWorldCube = WC;
        WC->WeaponObject = GameObject::WeaponBox;

        GameObject::WeaponBox->BuggyFinalMatrix = GameMath::Transform::Translate(LOC);
        GameObject::WeaponBox->BoundingBox->FinalMatrix = GameObject::WeaponBox->BuggyFinalMatrix * GameMath::Transform::Scale(1.1f, 1.1f, 1.1f);
        GameObject::WeaponBox->BoundingBox->Centroid.x = GameObject::WeaponBox->BuggyFinalMatrix.Matrix[3][0];
        GameObject::WeaponBox->BoundingBox->Centroid.y = GameObject::WeaponBox->BuggyFinalMatrix.Matrix[3][1];
        GameObject::WeaponBox->BoundingBox->Centroid.z = GameObject::WeaponBox->BuggyFinalMatrix.Matrix[3][2];
        GameObject::WeaponBox->BoundingBox->CalculateXYZ();
        GameObject::WeaponBox->isAlive = true;
    }
}


/*****************************************************************
THIS FUNCTION SETUPS ALL THE PLAYERS, ENEMIES AND LIGHTS IN THE GAME
ALONG WITH THE WORLD OBJECT
******************************************************************/
GameWorld* LoadAssets(GameCamera::FPS& FPSCamera)
{
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
//******************************************************************
//  IMPORT WORLD OBJECT
//******************************************************************
GameObject::RealWorld = GameBlenderImport::Import(std::string(GameWindow::CWD + "\\Payload\\Models\\WorldNoMaterial.GameEngine"));
GameObject::RealWorld->Load();
GameWorld* FakeWorld = new GameWorld(GameObject::RealWorld->BoundingBox->minx, GameObject::RealWorld->BoundingBox->maxx, GameObject::RealWorld->BoundingBox->miny, GameObject::RealWorld->BoundingBox->maxy, GameObject::RealWorld->BoundingBox->minz, GameObject::RealWorld->BoundingBox->maxz);
//******************************************************************
//  IMPORT PLAYER OBJECTS
//  Boudingbox for an object should always be loaded using "Load" 
//  function after "pixelmapping" has been called on it since
//  pixelmapping assigns a unique RGB value to the boundingbox that 
//  gets drawn in the background framebuffer
//******************************************************************
for (int i = 0; i < GameSettings::PlayerCount; i++)
{
    GameObject* Player = GameBlenderImport::Import(std::string(GameWindow::CWD + "\\Payload\\Models\\BatMobile.GameEngine").c_str());
    GameObject::Players.push_back(Player);
    Player->Load();
    //******************************************************************
    //  DO PIXELMAPPING FOR THE OBJECT AND THEN LOAD ITS BOUNDING BOX IN 
    //  SHADER SINCE THE PIXEL MAPPING ASSIGNS THE R,G,B VALUES TO THE 
    //  BOUNDING BOX WHICH GETS USED AS ITS COLOR IN THE BACKGROUND BUFFER
    //******************************************************************
    GameWindow::PixelMapping(Player->BoundingBox);
    Player->BoundingBox->Load();
    //******************************************************************
    //  LOAD BULLET, BULLET FIRE AND EXPLOSION EFFECTS FOR THE OBJECT
    //******************************************************************
    GameEffects::Bullet* bullet = new GameEffects::Bullet(Player->Index);
    bullet->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Bullet.bmp";
    bullet->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\Bullet.GameEffects").c_str(), &(bullet->TriangleCount));
    bullet->Load();

    GameEffects::BulletFire* bulletfire = new GameEffects::BulletFire(Player->Index);
    bulletfire->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Bullet.bmp";
    bulletfire->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\BulletFiring.GameEffects").c_str(), &(bulletfire->TriangleCount));
    bulletfire->Load();

    GameEffects::Explosion* explosion = new GameEffects::Explosion(Player->Index);
    explosion->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Explosion.bmp";
    explosion->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\Explosion.GameEffects").c_str(), &(explosion->TriangleCount));
    explosion->Load();

    Player->_Bullet = bullet;
    Player->_BulletFire = bulletfire;
    Player->_Explosion = explosion;
    //******************************************************************
    //  SET THE OBJECT TYPE AND SPEED
    //******************************************************************
    Player->ObjectType = 1;
    Player->Speed = GameSettings::PlayerSpeed;
    //******************************************************************
    //  THE INFO PANELS WHICH DISPLAY THE HEALTH AND THE WEAPON STATUS
    //  ARE SETUP FOR EACH BUGGY OBJECT
    //******************************************************************
    Player->Info = new GameText(GameWindow::CWD + "\\Payload\\TextTextures\\Text.bmp", &FPSCamera,Player->Index);
    //******************************************************************
    //  POSITIONING OF BUGGY OBJECTS AT RANDOM LOCATIONS IN SPACE
    //******************************************************************
    GameMath::Vector3 LOC;
    GameWorldCube* WC = RandomWorldPosition(FakeWorld);
    Player->SourceWorldCube = WC;
    WC->Object = Player;
    LOC = WC->Centroid;
    LOC.y = (Player->BoundingBox->maxy - Player->BoundingBox->miny) / 2.0f;
    Player->BuggyTranslationMatrix = GameMath::Transform::Translate(LOC);
    Player->BuggyFinalMatrix = Player->BuggyTranslationMatrix * Player->BuggyRotationMatrix;
    Player->BoundingBox->Centroid.x = Player->BuggyTranslationMatrix.Matrix[3][0];
    Player->BoundingBox->Centroid.y = Player->BuggyTranslationMatrix.Matrix[3][1];
    Player->BoundingBox->Centroid.z = Player->BuggyTranslationMatrix.Matrix[3][2];
    Player->BoundingBox->CalculateXYZ();
    Player->BoundingBox->FinalMatrix = Player->BuggyFinalMatrix;
    Player->_Bullet->CannonPosition = Player->BuggyFinalMatrix * Player->Armature.Bones["MachineGun"]->Head;
    Player->_Explosion->BBCentroid = Player->BoundingBox->Centroid;
    Player->_Explosion->BBCentroid.y = 0.0f;
    //******************************************************************
    //  SET THE SELECT COLOR OF THE PLAYER BUGGY'S BOUNDING BOX
    //******************************************************************
    Player->BoundingBox->ColorTint.x = GameSettings::PlayerSelectColorRed;
    Player->BoundingBox->ColorTint.y = GameSettings::PlayerSelectColorGreen;
    Player->BoundingBox->ColorTint.z = GameSettings::PlayerSelectColorBlue;
}
//******************************************************************
//  IMPORT ENEMY OBJECTS
//  Boudingbox for an object should always be loaded using "Load" 
//  function after "pixelmapping" has been called on it since
//  pixelmapping assigns a unique RGB value to the boundingbox that 
//  gets drawn in the background framebuffer
//******************************************************************
for (int i = 0; i < GameSettings::EnemyCount; i++)
{
    GameObject* Enemy = GameBlenderImport::Import(std::string(GameWindow::CWD + "\\Payload\\Models\\BatMobileBad.GameEngine").c_str());
    GameObject::Enemies.push_back(Enemy);
    Enemy->Load();
    //******************************************************************
    //  DO PIXELMAPPING FOR THE OBJECT AND THEN LOAD ITS BOUNDING BOX IN 
    //  SHADER SINCE THE PIXEL MAPPING ASSIGNS THE R,G,B VALUES TO THE 
    //  BOUNDING BOX WHICH GETS USED AS ITS COLOR IN THE BACKGROUND BUFFER
    //******************************************************************
    GameWindow::PixelMapping(Enemy->BoundingBox);
    Enemy->BoundingBox->Load();
    //******************************************************************
    //  LOAD BULLET, BULLET FIRE AND EXPLOSION EFFECTS FOR THE OBJECT
    //******************************************************************
    GameEffects::Bullet* bullet = new GameEffects::Bullet(Enemy->Index);
    bullet->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Bullet.bmp";
    bullet->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\Bullet.GameEffects").c_str(), &(bullet->TriangleCount));
    bullet->Load();

    GameEffects::BulletFire* bulletfire = new GameEffects::BulletFire(Enemy->Index);
    bulletfire->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Bullet.bmp";
    bulletfire->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\BulletFiring.GameEffects").c_str(), &(bulletfire->TriangleCount));
    bulletfire->Load();

    GameEffects::Explosion* explosion = new GameEffects::Explosion(Enemy->Index);
    explosion->TextureLocation = GameWindow::CWD + "\\Payload\\Textures\\Explosion.bmp";
    explosion->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\Explosion.GameEffects").c_str(), &(explosion->TriangleCount));
    explosion->Load();

    Enemy->_Bullet = bullet;
    Enemy->_BulletFire = bulletfire;
    Enemy->_Explosion = explosion;
    //******************************************************************
    //  SET THE OBJECT TYPE AND SPEED
    //******************************************************************
    Enemy->ObjectType = 2;
    Enemy->Speed = GameSettings::EnemySpeedMin;
    //******************************************************************
    //  THE INFO PANELS WHICH DISPLAY THE HEALTH AND THE WEAPON STATUS
    //  ARE SETUP FOR EACH BUGGY OBJECT
    //******************************************************************
    Enemy->Info = new GameText(GameWindow::CWD + "\\Payload\\TextTextures\\Text.bmp", &FPSCamera,Enemy->Index);
    //******************************************************************
    //  POSITIONING OF BUGGY OBJECTS AT RANDOM LOCATIONS IN SPACE
    //******************************************************************
    GameMath::Vector3 LOC;
    GameWorldCube* WC = RandomWorldPosition(FakeWorld);
    Enemy->SourceWorldCube = WC;
    WC->Object = Enemy;
    LOC = WC->Centroid;
    LOC.y = (Enemy->BoundingBox->maxy - Enemy->BoundingBox->miny) / 2.0f;
    Enemy->BuggyTranslationMatrix = GameMath::Transform::Translate(LOC);
    Enemy->BuggyFinalMatrix = Enemy->BuggyTranslationMatrix * Enemy->BuggyRotationMatrix;
    Enemy->BoundingBox->Centroid.x = Enemy->BuggyTranslationMatrix.Matrix[3][0];
    Enemy->BoundingBox->Centroid.y = Enemy->BuggyTranslationMatrix.Matrix[3][1];
    Enemy->BoundingBox->Centroid.z = Enemy->BuggyTranslationMatrix.Matrix[3][2];
    Enemy->BoundingBox->CalculateXYZ();
    Enemy->BoundingBox->FinalMatrix = Enemy->BuggyFinalMatrix;
    Enemy->_Bullet->CannonPosition = Enemy->BuggyFinalMatrix * Enemy->Armature.Bones["MachineGun"]->Head;
    Enemy->_Explosion->BBCentroid = Enemy->BoundingBox->Centroid;
    Enemy->_Explosion->BBCentroid.y = 0.0f;
    //******************************************************************
    //  SET THE SELECT COLOR OF THE ENEMY BUGGY'S BOUNDING BOX
    //******************************************************************
    Enemy->BoundingBox->ColorTint.x = GameSettings::EnemySelectColorRed;
    Enemy->BoundingBox->ColorTint.y = GameSettings::EnemySelectColorGreen;
    Enemy->BoundingBox->ColorTint.z = GameSettings::EnemySelectColorBlue;
}
//******************************************************************
//  IMPORT INANIMATE OBJECTS
//******************************************************************
std::string AssetFile[5];
AssetFile[0] = std::string("\\Payload\\Models\\Rock1.GameEngine");
AssetFile[1] = std::string("\\Payload\\Models\\Rock2.GameEngine");
AssetFile[2] = std::string("\\Payload\\Models\\Rock3.GameEngine");
AssetFile[3] = std::string("\\Payload\\Models\\Rock4.GameEngine");
AssetFile[4] = std::string("\\Payload\\Models\\Rock5.GameEngine");
int AssetFileCounter = 0;
for (int i = 0; i < GameSettings::InanimateCount; i++)
{
    GameObject* Inanimate;
    Inanimate = GameBlenderImport::Import(std::string(GameWindow::CWD + AssetFile[AssetFileCounter]).c_str());
    GameObject::Inanimates.push_back(Inanimate);
    Inanimate->Load();
    if (AssetFileCounter ==4)
        AssetFileCounter = 0;
    else
        AssetFileCounter += 1;
    //******************************************************************
    //  POSITIONING OF INANIMATE OBJECTS AT RANDOM LOCATIONS IN WORLD
    //******************************************************************
    GameWorldCube* WC = RandomWorldPosition(FakeWorld);
    GameMath::Vector3 LOC = WC->Centroid;
    Inanimate->SourceWorldCube = WC;
    WC->Object = Inanimate;
    Inanimate->BuggyFinalMatrix = GameMath::Transform::Translate(LOC);
    Inanimate->BoundingBox->Centroid.x = Inanimate->BuggyFinalMatrix.Matrix[3][0];
    Inanimate->BoundingBox->Centroid.y = Inanimate->BuggyFinalMatrix.Matrix[3][1];
    Inanimate->BoundingBox->Centroid.z = Inanimate->BuggyFinalMatrix.Matrix[3][2];
    Inanimate->BoundingBox->CalculateXYZ();
    Inanimate->BoundingBox->FinalMatrix = Inanimate->BuggyFinalMatrix;
}

//******************************************************************
//  IMPORT HEALTH BOX 
//  Boudingbox for an object should always be loaded using "Load" 
//  function after "pixelmapping" has been called on it since
//  pixelmapping assigns a unique RGB value to the boundingbox that 
//  gets drawn in the background framebuffer
//******************************************************************
GameObject::HealthBox = GameBlenderImport::Import(std::string(GameWindow::CWD + "\\Payload\\Models\\HealthBox.GameEngine").c_str());
GameObject::HealthBox->ObjectType = 3;
GameObject::HealthBox->isAlive = false;
GameObject::HealthBox->Load();
GameWindow::PixelMapping(GameObject::HealthBox->BoundingBox);
GameObject::HealthBox->BoundingBox->Load();
GameObject::HealthBox->BoundingBox->ColorTint.x = GameSettings::HealthSelectColorRed;
GameObject::HealthBox->BoundingBox->ColorTint.y = GameSettings::HealthSelectColorGreen;
GameObject::HealthBox->BoundingBox->ColorTint.z = GameSettings::HealthSelectColorBlue;

//******************************************************************
//  IMPORT WEAPON BOX 
//  Boudingbox for an object should always be loaded using "Load" 
//  function after "pixelmapping" has been called on it since
//  pixelmapping assigns a unique RGB value to the boundingbox that 
//  gets drawn in the background framebuffer
//******************************************************************
GameObject::WeaponBox = GameBlenderImport::Import(std::string(GameWindow::CWD + "\\Payload\\Models\\WeaponBox.GameEngine").c_str());
GameObject::WeaponBox->ObjectType = 4;
GameObject::WeaponBox->isAlive = false;
GameObject::WeaponBox->Load();
GameWindow::PixelMapping(GameObject::WeaponBox->BoundingBox);
GameObject::WeaponBox->BoundingBox->Load();
GameObject::WeaponBox->BoundingBox->ColorTint.x = GameSettings::WeaponSelectColorRed;
GameObject::WeaponBox->BoundingBox->ColorTint.y = GameSettings::WeaponSelectColorGreen;
GameObject::WeaponBox->BoundingBox->ColorTint.z = GameSettings::WeaponSelectColorBlue;

//******************************************************************
// SETUP POINTS LIGHTS AT RANDOM LOCATIONS IN WORLD
//******************************************************************
PointLight* Light;
for (int i = 0; i < GameSettings::LampCount; i++)
{
    Light = new PointLight();
    GameWorldCube* WC = RandomWorldPosition(FakeWorld);
    Light->Position.x = WC->Centroid.x;
    Light->Position.y = WC->Centroid.y + 5.0f; 
    Light->Position.z = WC->Centroid.z;
    GameLight::PointLights.push_back(Light);
}

//******************************************************************
//  SETUP ONE SUN LIGHT
//******************************************************************
SunLight* Sun = new SunLight();
std::cout << "SUN LIGHT " << std::to_string(GameSettings::SunLightColorRed) << "," << std::to_string(GameSettings::SunLightColorGreen) << "," << std::to_string(GameSettings::SunLightColorBlue) << "\n";
Sun->Color = GameMath::Vector3(GameSettings::SunLightColorRed, GameSettings::SunLightColorGreen, GameSettings::SunLightColorBlue);
Sun->Direction = GameMath::Vector3(0.0f, -1.0f, 0.0f);
GameLight::SunLights.push_back(Sun);

//******************************************************************
//  LOADING WORLDCUBE OBJECT THAT IS MEANT TO HIGHLIGHT/SHOW USER'S
//  SELECTION OF A WORLD POSITION WHEN MOVING A BUGGY TO THAT LOCATION.
//  THIS ONE WORLDCUBE OBJECT WILL TRANSLATE TO THE WORLDCUBE POSITION 
//  THAT THE USER IS POINTING AT AND WILL GLOW WITH A GLOW EFFECT
//******************************************************************
GameObject::SelectedWorldCube = new GameWorldCube();
GameObject::SelectedWorldCube->Centroid.x = GameObject::Players[0]->SourceWorldCube->Centroid.x;
GameObject::SelectedWorldCube->Centroid.y = GameObject::Players[0]->SourceWorldCube->Centroid.y;
GameObject::SelectedWorldCube->Centroid.z = GameObject::Players[0]->SourceWorldCube->Centroid.z;
GameObject::SelectedWorldCube->Load(FakeWorld->SmallestDivision);
GameObject::SelectedWorldCube->FinalMatrix = GameMath::Transform::Scale(0.0f, 0.0f, 0.0f);
GameObject::SelectedWorldCube->ColorTint.x = GameSettings::LocationSelectColorRed;
GameObject::SelectedWorldCube->ColorTint.y = GameSettings::LocationSelectColorGreen;
GameObject::SelectedWorldCube->ColorTint.z = GameSettings::LocationSelectColorBlue;

//******************************************************************
//  LOAD ALL GAMEOBJECTS IN GPU
//******************************************************************
GameGPUObjectLoader::LoadObjects();

//******************************************************************
//  LOAD ALL LIGHTS IN GPU
//******************************************************************
GameGPUObjectLoader::LoadLights();

//******************************************************************
//  RETURN NEWLY GENERATED FAKEWORLD OBJECT
//******************************************************************
return FakeWorld;
}

/*****************************************************************
THIS FUNCTION UNDOs EVERYTHING THAT THE SETUP FUNCTION HAS DONE
AND FREES ALL THE RESOURCES
******************************************************************/
void UnloadAssets(GameWorld* FakeWorld)
{
    //CLEARING PIXEL MAP
    for (std::map<std::string, GameWindow::Pixel*>::iterator it = GameWindow::PixelMap.begin(); it != GameWindow::PixelMap.end(); it++)
    delete it->second;
    GameWindow::PixelMap.clear();

    //DELETING GAMEWORLD AND ALL THE WORLDCUBE OBJECTS WITH IT 
    delete FakeWorld;

    //DELETING REAL WORLD OBJECT
    delete GameObject::RealWorld;
    GameObject::RealWorld = nullptr;

    //DELETING ALL THE PLAYERS
    for (GameObject* Player : GameObject::Players)
        delete Player;
    GameObject::Players.clear();

    //DELETING ALL THE ENEMIES
    for (GameObject* Enemy : GameObject::Enemies)
        delete Enemy;
    GameObject::Enemies.clear();

    //DELETING ALL THE INANIMATES
    for (GameObject* Inanimate : GameObject::Inanimates)
        delete Inanimate;
    GameObject::Inanimates.clear();

    //DELETING HEALTH AND WEAPON BOX
    delete GameObject::HealthBox;
    delete GameObject::WeaponBox;
    GameObject::HealthBox = nullptr;
    GameObject::WeaponBox = nullptr;

    //DELETING SUNLIGHTS
    for (SunLight* Sun : GameLight::SunLights)
        delete Sun;
    GameLight::SunLights.clear();

    //DELETING POINTLIGHTS
    for (PointLight* PointLight : GameLight::PointLights)
        delete PointLight;
    GameLight::PointLights.clear();

    //DELETING SPOTLIGHTS
    for (SpotLight* SpotLight : GameLight::SpotLights)
        delete SpotLight;
    GameLight::SpotLights.clear();

    //DELETING SELECTED WORLD CUBE OBJECT
    delete GameObject::SelectedWorldCube;
    GameObject::SelectedWorldCube = nullptr;

    //UNLOADING ALL OBJECTS AND LIGHTS FROM GPU
    GameGPUObjectLoader::UnloadObjects();
    GameGPUObjectLoader::UnloadLights();

    //RESETTING ALL THE COMPONENTS INDICES
    GameObject::ObjectIndex = 0;
    GameMesh::MeshIndex = 0;
    GameMesh::MaterialIndex = 0;
    GameArmature::ArmatureIndex = 0;
    GameArmature::BoneIndex = 0;
    GameText::GameTextIndex = 0;
    GameBoundingBox::BoundingBoxIndex = 0;
    GameEffects::Bullet::BulletIndex = 0;
    GameEffects::BulletFire::BulletFireIndex = 0;
    GameEffects::Explosion::ExplosionIndex = 0;

}


void DebugGraphics()
{
    GameWindow::Initialize("3D GAME USING OPENGL - KILL THAT BUGGY", 100, 100, 800, 600);
    if (!GameWindow::GetStatus())
        return;

    glViewport(0, 0, GameWindow::Width, GameWindow::Height);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    TCHAR CWD[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, CWD);
    std::wstring wStr = CWD;
    GameWindow::CWD = std::string(wStr.begin(), wStr.end());

    GameCamera::FPS FPSCamera;
    GameMath::Matrix4x4 Model;
    GameMath::Matrix4x4 Projection = GameMath::Transform::PerspectiveView((float)GameWindow::Width / (float)GameWindow::Height, 0.1f, 100.0f, 45.0f);

    GameShader HelperShader;
    HelperShader.LoadShader(std::string(GameWindow::CWD + "\\Payload\\Shaders\\HelperVertex.cpp").c_str(), std::string(GameWindow::CWD + "\\Payload\\Shaders\\HelperFragment.cpp").c_str(), nullptr);


    GameBoundingBox Origin(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0);
    Origin.Load();
    Origin.ColorTint.x = 1.0f;
    Origin.ColorTint.y = 1.0f;
    Origin.ColorTint.z = 0.0f;
    GameBoundingBox Sun(100.0f, -100.0f, 0.1f, -0.1f, 100.0f, -100.0f, 0);
    Sun.Load();
    Sun.ColorTint.x = 1.0f;
    Sun.ColorTint.y = 0.0f;
    Sun.ColorTint.z = 0.0f;


    GameBoundingBox Planet(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0);
    GameBoundingBox PlanetBoundingBox(1.5f, -1.5f, 0.01f, -0.01f, 1.5f, -1.5f, 0);
    Planet.Load();
    PlanetBoundingBox.Load();
    Planet.ColorTint.x = 0.0f;
    Planet.ColorTint.y = 0.0f;
    Planet.ColorTint.z = 1.0f;
    PlanetBoundingBox.ColorTint.x = 0.0f;
    PlanetBoundingBox.ColorTint.y = 1.0f;
    PlanetBoundingBox.ColorTint.z = 1.0f;
    GameMath::Matrix4x4 PlanetTranslate = GameMath::Transform::Translate(5.0f, 5.0f, -2.0f);
    Planet.Centroid = PlanetTranslate * Planet.Centroid;
    PlanetBoundingBox.FinalMatrix = GameMath::Transform::Translate(Planet.Centroid);
    GameMath::Matrix4x4 PlanetRotate;
    FPSCamera.CameraPosition = GameMath::Vector3(5.0f, 5.0f, 5.0f);
    FPSCamera.CameraPosition.y += 1.0f;
    FPSCamera.PlayerTakenControl = true;
    FPSCamera.UpdateCameraPosition();
    FPSCamera.UpdateCameraView();
    
    const int DESIRED_TIME_TAKEN_PER_FRAME = 1000 / 120;
    DWORD FrameStartTime;
    DWORD Timer;
    MSG msg;
    float Angle = 0.0f;
    while (true)
    {
        FrameStartTime = GetTickCount();
        Timer = FrameStartTime + DESIRED_TIME_TAKEN_PER_FRAME;
        while (GetTickCount() < Timer)
        {
            if (PeekMessage(&msg, GameWindow::WindowHandle, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT || (GetKeyState(27) & 0x8000))
                    GameWindow::KeepAlive = false;
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
                break;
        }

        HelperShader.Use();
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(3, 1, GL_FALSE, FPSCamera.View.Addressof());
        glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());



        if (GetKeyState(87) & 0x8000)
        {
            GameMath::Matrix4x4 T = GameMath::Transform::Translate(FPSCamera.Front * 1.0f);
            PlanetTranslate = T * PlanetTranslate;
            Planet.Centroid = T * Planet.Centroid;
            GameMath::Vector3 PlanetPosition = GameMath::Vector3(PlanetTranslate.Matrix[3][0], PlanetTranslate.Matrix[3][1], PlanetTranslate.Matrix[3][2]);
            FPSCamera.CameraPosition = T * FPSCamera.CameraPosition;
            FPSCamera.UpdateCameraView();
            std::cout << "Planet Centroid " << Planet.Centroid;
        }
        else if (GetKeyState(83) & 0x8000)
        {
            GameMath::Matrix4x4 T = GameMath::Transform::Translate(FPSCamera.Front * -1.0f);
            PlanetTranslate = T * PlanetTranslate;
            Planet.Centroid = T * Planet.Centroid;
            GameMath::Vector3 PlanetPosition = GameMath::Vector3(PlanetTranslate.Matrix[3][0], PlanetTranslate.Matrix[3][1], PlanetTranslate.Matrix[3][2]);
            FPSCamera.CameraPosition = T * FPSCamera.CameraPosition;
            FPSCamera.UpdateCameraView();
            std::cout << "Planet Centroid " << Planet.Centroid;
        }
        else if (GetKeyState(65) & 0x8000)
        {
            Angle += 1.0f;
            FPSCamera.YawAngle += 1.0f;
            FPSCamera.UpdateCameraView();

            GameMath::Vector3 PlanetPosition = GameMath::Vector3(PlanetTranslate.Matrix[3][0], PlanetTranslate.Matrix[3][1], PlanetTranslate.Matrix[3][2]);
            GameMath::Matrix4x4 RM;
            GameMath::Vector3 D = FPSCamera.CameraPosition - PlanetPosition;
            GameMath::Vector3 pivot = D.Normalize() * D.Length();
            GameMath::Matrix4x4 n = GameMath::Transform::Translate(-pivot);
            GameMath::Matrix4x4 p = GameMath::Transform::Translate(pivot);
            PlanetRotate = p * GameMath::Transform::RotateQ('Y', Angle) * n;
            Planet.Centroid = FPSCamera.CameraPosition + (FPSCamera.Front * D.Length());
            Planet.Centroid.y -= 1.0f;
            std::cout << "Planet Centroid " << Planet.Centroid;
        }
        else if (GetKeyState(68) & 0x8000)
        {
            Angle -= 1.0f;
            FPSCamera.YawAngle -= 1.0f;
            FPSCamera.UpdateCameraView();

            GameMath::Vector3 PlanetPosition = GameMath::Vector3(PlanetTranslate.Matrix[3][0], PlanetTranslate.Matrix[3][1], PlanetTranslate.Matrix[3][2]);
            GameMath::Matrix4x4 RM;
            GameMath::Vector3 D = FPSCamera.CameraPosition - PlanetPosition;
            GameMath::Vector3 pivot = D.Normalize() * D.Length();
            GameMath::Matrix4x4 n = GameMath::Transform::Translate(-pivot);
            GameMath::Matrix4x4 p = GameMath::Transform::Translate(pivot);
            PlanetRotate = p * GameMath::Transform::RotateQ('Y', Angle) * n;
            Planet.Centroid = FPSCamera.CameraPosition + (FPSCamera.Front * D.Length());
            Planet.Centroid.y -= 1.0f;
            std::cout << "Planet Centroid " << Planet.Centroid;
        }



        Origin.DrawForeground();
        Sun.DrawForeground();
        PlanetBoundingBox.FinalMatrix = GameMath::Transform::Translate(Planet.Centroid);
        PlanetBoundingBox.DrawForeground();
        Planet.FinalMatrix = PlanetTranslate * PlanetRotate;
        Planet.DrawForeground();
        SwapBuffers(GameWindow::DeviceContext);
    }
}


#define DEBUG false
//**********************************************************
//   *          *    **********     **********   *      *
//   * *      * *    *        *         *        * *    *
//   *   *   *  *    *        *         *        *  *   *
//   *     *    *    *  ****  *         *        *   *  *
//   *          *    *        *         *        *    * *
//   *          *    *        *     *********    *     **
//**********************************************************
//******************************************************************
// MAIN FUNCTION
//******************************************************************
int main()
{
#if DEBUG
DebugGraphics();
#else if !DEBUG
//******************************************************************
//GET ALL GAME SETTINGS FOR GAME PLAY 
//******************************************************************
GameSettings::Setup();

//******************************************************************
//INITIALIZE THE GAME WINDOW THAT WILL OUTPUT THE GAME GRAPHICS
//******************************************************************

//OPENGL FUNCTIONS:
//glViewport - glviewport() defines what part of the screen we want to render to by defining the x coordinate,
//             y coordinate, width, and height of the rendering area
//glEnable - enable or disable server-side gl capabilities, here we are enabling "GL_DEPTH_TEST"
//glShadeModel - select flat or smooth shading.flat shading is one color for the polygon whereas smooth shading
//               allows for interpolation of color values between vertices
//glClearColor - specify clear values for the color buffers in rgba format. it basically sets up the
//               background color in the render window

GameWindow::Initialize("3D GAME USING OPENGL - KILL THAT BUGGY", 100, 100, 800, 600);
if (!GameWindow::GetStatus())
    return 0;

glViewport(0, 0, GameWindow::Width, GameWindow::Height);
glEnable(GL_DEPTH_TEST);
glShadeModel(GL_FLAT);
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
GameMath::Matrix4x4 Projection = GameMath::Transform::PerspectiveView((float)GameWindow::Width / (float)GameWindow::Height, 0.1f, 100.0f, 45.0f);


//SETUP THE MAIN SHADER AND THE HELPER SHADER
//MAIN SHADERS - RENDERS OBJECTS ON THE SCREEN USING THE FOREGROUND FRAME BUFFER SO THEY CAN BE DISPLAYED TO THE USER.
//                MOST OF THESE OBJECTS WILL BE GAMEOBJECTS, GAMEWORLD, GAMEFFECTS OBJECTS LIKE BULLET AND INANIMATE OBJECTS
//              LIKE ROCKS, HEALTH AND WEAPON OBJECTS
//
//HELPER SHADER - RENDERS OBJECTS IN THE BACKGROUND USING THE BACKGROUND FRAME BUFFER. THE OBJECTS IN THE BACKGROUND
//                FRAMEOBJECT ARE NEVER RENDERED ON THE SCREEN. THEY ARE USED TO ALLOW USER INTERACTION TO PROCESS.
//                EACH OBJECT IN THE BACKGROUND FRAMEE BUFFER IS GIVEN A UNIQUE COLOR WHICH IS USED TO IDENTIFY THAT OBJECT
//                AND THAT IS HOW THE USER INTERACTS WITH THE WORLD
GameShader MainShader;
GameShader HelperShader;
MainShader.LoadShader(std::string(GameWindow::CWD + "\\Payload\\Shaders\\MainVertex.cpp").c_str(), std::string(GameWindow::CWD + "\\Payload\\Shaders\\MainFragment.cpp").c_str(), std::string(GameWindow::CWD + "\\Payload\\Shaders\\MainGeometry.cpp").c_str());
HelperShader.LoadShader(std::string(GameWindow::CWD + "\\Payload\\Shaders\\HelperVertex.cpp").c_str(), std::string(GameWindow::CWD + "\\Payload\\Shaders\\HelperFragment.cpp").c_str(), nullptr);

//*************************************************************
//  LOAD THE HELP SECTION BOX WHICH IS DISPLAYED AT THE BEGINNING OF THE GAME 
//******************************************************************
HelperShader.Use();
GameEffects::HelpSection* Help = new GameEffects::HelpSection();
Help->TextureLocations[0] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxBeginining.bmp";
Help->TextureLocations[1] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxLevelA.bmp";
Help->TextureLocations[2] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxLevelB.bmp";
Help->TextureLocations[3] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxLevelC.bmp";
Help->TextureLocations[4] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxLevelD.bmp";
Help->TextureLocations[5] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxLevelE.bmp";
Help->TextureLocations[6] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxYouWon.bmp";
Help->TextureLocations[7] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxYouWonAll.bmp";
Help->TextureLocations[8] = GameWindow::CWD + "\\Payload\\Textures\\HelpBoxYouLost.bmp";
Help->Vertices = GameBlenderImport::ImportEffects(std::string(GameWindow::CWD + "\\Payload\\Models\\HelpBox.GameEffects").c_str(), &(Help->TriangleCount));
Help->Load();
MainShader.Use();

//******************************************************************
// SETTING UP SKYBOX
//******************************************************************
SkyBox skybox(GameWindow::CWD + "\\Payload\\SkyBox\\");

//**********************************************
// BACKGROUND FRAMEBUFFER INITIALIZATION
//**********************************************
//OPENGL FUNCTIONS:
//glGenFramebuffers - generate framebuffer object names. the names are basically integer numbers that uniquely identifies that framebuffer
//glBindFramebuffer - bind a framebuffer to a framebuffer target.target must be either GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_FRAMEBUFFER.
//                    you specify the framebuffer using that integer name that you got from glGenFramebuffers() function
//glGenTextures - generate texture names which are integer numbers
//glBindTexture - bind a named texture to a texturing target. target must be one of GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D,
//                GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER,
//               GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY
//glTexImage2D - specify a two-dimensional texture image.the first argument specifies the target texture
//glTexParameteri - set texture parameters
//glFramebufferTexture2D - attach a level of a texture object as a logical buffer of a framebuffer object
//glGenRenderbuffers - generate renderbuffer object names which are integer numbers
//glBindRenderbuffer - bind a renderbuffer to a renderbuffer target
//glRenderbufferStorage - establish data storage, format and dimensions of a renderbuffer object's image
//glFramebufferRenderbuffer - attach a renderbuffer as a logical buffer of a framebuffer object
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
glBindFramebuffer(GL_FRAMEBUFFER, 0);

//******************************************************************
// GAME LOOP VARIABLES INITIALIZATION
//******************************************************************
MSG msg;
const int DESIRED_TIME_TAKEN_PER_FRAME = 1000 / 120;
DWORD FrameStartTime;
DWORD Timer;
DWORD HealthWeaponTimer = 0;
DWORD AITimer = 0;
DWORD LightDimTimer = 0;
POINT MousePointerInfo;
GameWindow::Pixel* PixelPointer;
GameWorld* FakeWorld=nullptr;
float LightsDimEffect = 1.0f;
bool LightHigh = false;
bool LightLow = true;
bool isCameraShaking = false;
bool GameStarted = false;
GameMath::Vector3 OriginalCameraPosition = GameMath::Vector3(FPSCamera.CameraPosition.x, FPSCamera.CameraPosition.y, FPSCamera.CameraPosition.z+2.5f);
std::vector<GameObject*> AlivePlayers;
std::vector<GameObject*> AliveEnemies;
std::vector<int> CompletedLevels;
//**********************************************************
//  *         ********  ********    *********
//  *         *      *  *      *    *       *
//  *         *      *  *      *    *       *   
//  *         *      *  *      *    *********   
//  *         *      *  *      *    *
//  ********  ********  ********    *
//**********************************************************

//**********************************************
//    GAME LOOP STARTS HERE
//**********************************************
while (GameWindow::KeepAlive)
{
    // WINDOWS MESSAGE LOOP IS IMPLEMENTED USING THE PeekMessage() FUNCTION
    // WHICH IS RUN UNDER A WHILE LOOP WITH A TIMER IN PLACE
    // SO IF A LOT OF WINDOWS EVENT MESSAGES GET INTO THE MESSAGE QUEUE
    // THEN PeekMessage() FUNCTION CAN HANDLE AS MANY AS POSSIBLE WITHIN THE
    // DESIRED TIME IT TAKES TO RENDER ONE FRAME AFTER THAT THE GRAPHICS RENDERING TAKES PLACE
    // NO MATTER WHAT. THIS AVOIDS ANY MAJOR LAGS IN THE GRAPHICS RENDERING PROCESS
    FrameStartTime = GetTickCount();
    Timer = FrameStartTime + DESIRED_TIME_TAKEN_PER_FRAME;
    while (GetTickCount() < Timer)
    {
        if (PeekMessage(&msg, GameWindow::WindowHandle, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT || (GetKeyState(27) & 0x8000))
                GameWindow::KeepAlive = false;
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
    if (!FPSCamera.PlayerTakenControl)
    {
        FPSCamera.UpdateCameraPosition();
        FPSCamera.UpdateCameraView();
    }

    //**************************************************************************************
    //   RE-CALCULATING PRPOJECTION MATRIX IF VIEWPORT CHANGES DUE TO RESIZING OF GAMEWINDOW
    //**************************************************************************************
    if (GameWindow::ViewPortChanged == true)
        Projection = GameMath::Transform::PerspectiveView((float)GameWindow::Width / (float)GameWindow::Height, 0.1f, 1000.0f, 60.0f);

    //**************************************************************************************
    //   DISPLAYING THE HELP SECTION SO THE USER CAN READ THE INSTRUCTIONS
    //   AND THEN PRESS "ENTER" ON THE KEYBOARD TO PROCEED FURTHER
    //   UNTIL THAT HAPPENS THE GAME WILL NOT CONTINUE
    //   AFTER THE USER PRESSES THE "ENTER" KEY THEN THE GAME WILL PROCEED FURTHER
    //   AND THE CAMERA POSITION WILL BE RESTORED TO ITS ORIGINAL POSITION
    //**************************************************************************************
    if (!GameStarted)
    {
        FPSCamera.CameraPosition = OriginalCameraPosition;
        FPSCamera.YawAngle = 0.0f;
        FPSCamera.PitchAngle = 0.0f;
        
        HelperShader.Use();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
        glUniformMatrix4fv(3, 1, GL_FALSE, FPSCamera.View.Addressof());
        glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());
        Help->Draw();
        SwapBuffers(GameWindow::DeviceContext);

        if (GameWindow::KeyCode==13)
        {
            //IF GAME HAS COMPLETED EITHER WITH A WIN OR LOSE, USER HAS TO GO BACK TO THE MAIN MENU 
            //TO SELECT A LEVEL 
            if (Help->State > 5)
            {
                Help->State = 0;
                GameWindow::KeyCode = '/0';
                continue;
            }
            else if (Help->State <= 1)
                GameSettings::Load(GameWindow::CWD + "\\Payload\\Settings\\LevelA.txt");
            else if(Help->State ==2)
                GameSettings::Load(GameWindow::CWD + "\\Payload\\Settings\\LevelB.txt");
            else if (Help->State == 3)
                GameSettings::Load(GameWindow::CWD + "\\Payload\\Settings\\LevelC.txt");
            else if (Help->State == 4)
                GameSettings::Load(GameWindow::CWD + "\\Payload\\Settings\\LevelD.txt");
            else if (Help->State == 5)
                GameSettings::Load(GameWindow::CWD + "\\Payload\\Settings\\LevelE.txt"); 
            GameStarted = true;
            
            //LOAD ALL THE GAME ASSETS
            FakeWorld = LoadAssets(FPSCamera);

            //******* DO PIXEL MAPPING OF UI OBJECTS AS WELL
            
            //CAMERA PLACEMENT RIGHT BEFORE THE FIRST PLAYER OBJECT
            GameMath::Vector3 LOC = GameObject::Players[0]->BoundingBox->Centroid + (GameObject::Players[0]->BuggyFront*-5.0f);
            LOC.y += 1.5f;
            float yawangle = atan2(FPSCamera.Front.x, FPSCamera.Front.y) - atan2(GameObject::Players[0]->BuggyFront.x, GameObject::Players[0]->BuggyFront.z);
            FPSCamera.CameraPosition = LOC;
            FPSCamera.YawAngle = yawangle * 180.0f / PI;
            FPSCamera.Rotation = GameMath::Transform::RotateQ(GameObject::Players[0]->YAxis, -FPSCamera.YawAngle);
            
            //PREVENT CAMERA FROM MOVING
            GameCamera::FPS::PlayerTakenControl = true;
            GameObject::Players[0]->PlayerTakenControl = true;

            //UPDATE CAMERA VIEW 
            FPSCamera.UpdateCameraView();
        }
        else if (GameWindow::KeyCode == 65 && Help->State <= 5)//LEVEL A
        {
            Help->State = 1;
            GameWindow::KeyCode = '/0';
        }
        else if (GameWindow::KeyCode == 66 && Help->State <= 5)//LEVEL B
        {
            Help->State = 2;
            GameWindow::KeyCode = '/0';
        }
        else if (GameWindow::KeyCode == 67 && Help->State <= 5)//LEVEL C
        {
            Help->State = 3;
            GameWindow::KeyCode = '/0';
        }
        else if (GameWindow::KeyCode == 68 && Help->State <= 5)//LEVEL D
        {
            Help->State = 4;
            GameWindow::KeyCode = '/0';
        }
        else if (GameWindow::KeyCode == 69 && Help->State <= 5)//LEVEL E
        {
            Help->State = 5;
            GameWindow::KeyCode = '/0';
        }
       
        //RESET KEYCODE SO THE LOOP DOES NOT REPEAT WITH THE SAME KEY CODE AGAIN
        GameWindow::KeyCode = '/0';
        continue;
    }
    

    //****************************************************************************
    //  PROCESS BUGGY MOVEMENTS AND ATTACKS FOR PLAYER BUGGIES
    //****************************************************************************
    for (GameObject* Player : GameObject::Players)
    {
        if (Player->isAlive && !Player->PlayerTakenControl)
        {
            Player->WorldMovement(GameWindow::Time_Taken_Per_Frame);
            Player->Attack();
        }
        else if (Player->isAlive && Player->PlayerTakenControl)
        {
            Player->PlayerControlledWorldMovement(GameWindow::Time_Taken_Per_Frame,FPSCamera);
            Player->PlayerControlledAttack(FPSCamera);
        }
    }
    //****************************************************************************
    //  PROCESS BUGGY MOVEMENTS AND ATTACKS FOR ENEMY BUGGIES
    //****************************************************************************
    for (GameObject* Enemy : GameObject::Enemies)
    {
        if (Enemy->isAlive)
        {
            std::string d;
            std::string location;
            if (Enemy->Health <= GameSettings::EnemyHealthBeforeReload && GameObject::HealthBox->isAlive)
            {
                Enemy->DestinationWorldCube = GameObject::HealthBox->SourceWorldCube;
            }
            else if (Enemy->WeaponQuantity <= GameSettings::EnemyWeaponBeforeReload && GameObject::WeaponBox->isAlive)
            {
                Enemy->DestinationWorldCube = GameObject::WeaponBox->SourceWorldCube;
            }
            else if (Enemy->AttackObject == nullptr)
            {
                GameObject* SearchObject = Enemy->AISearchTarget();
                if (SearchObject != nullptr)
                {
                    if (Enemy->TargetOutOfRange == true)
                    Enemy->DestinationWorldCube = GameObject::FindEmptyNeighbour(SearchObject->SourceWorldCube);
                    else
                    Enemy->AttackObject = SearchObject;
                }
            }
            
            if (Enemy->Speed < GameSettings::EnemySpeedMax && GetTickCount() > AITimer + GameSettings::EnemySpeedIncreaseInterval)
            {
                Enemy->Speed += GameSettings::EnemySpeedIncreaseRate;
                AITimer = GetTickCount();
            }
                       
            Enemy->WorldMovement(GameWindow::Time_Taken_Per_Frame);
            Enemy->Attack();
        }
    }
    
    //****************************************************
    //   GENERATE HEALTH AND WEAPON OBJECTS EVERY SET INTERVAL
    //****************************************************
    if (GetTickCount() > HealthWeaponTimer + GameSettings::HealthAndWeaponSpawnTime && FakeWorld!=nullptr)
    {
        SpawnHealth(FakeWorld);
        SpawnWeapon(FakeWorld);
        HealthWeaponTimer = GetTickCount();
    }

    //******************************************************
    //   HAVE THE POINT LIGHTS VARY THEIR LIGHT INTENSITY BETWEEN 0.08 AND 0.9 EVERY SET INTERVAL TO CREATE
    //   A SUNRISE AND SUNSET EFFECT AND MAKE EACH OF THEM LAST A SET INTERVAL OF TIME
    //******************************************************
    //SUNRISE EFFECT USING POINT LIGHTS
    if (LightLow == true && GetTickCount() > LightDimTimer + GameSettings::LampLightOnTime)
    {
        LightsDimEffect -= GameSettings::LampLightChangeSpeed;
        if (LightsDimEffect < GameSettings::LampLightMin)
        {
            LightHigh = true;
            LightLow = false;
            LightDimTimer = GetTickCount();
        }
        else
        {
            for (PointLight* Light : GameLight::PointLights)
                Light->UpdateColorXYZ(LightsDimEffect, LightsDimEffect, LightsDimEffect);
        }
    }
    //SUNSET EFFECT USING POINT LIGHTS
    else if (LightHigh == true && GetTickCount() > LightDimTimer + GameSettings::LampLightOffTime)
    {
        LightsDimEffect += GameSettings::LampLightChangeSpeed;
        if (LightsDimEffect > GameSettings::LampLightMax)
        {
            LightHigh = false;
            LightLow = true;
            LightDimTimer = GetTickCount();
        }
        else
        {
            for (PointLight* Light : GameLight::PointLights)
                Light->UpdateColorXYZ(LightsDimEffect, LightsDimEffect, LightsDimEffect);
        }
    }



    //******************************************************
    //   *****    ******   **  *   ******   *****  *****   *
    //   *   *    *        * * *   *    *   ***    *   *   *
    //   *****    ***      *  **   *    *   *      *****   *
    //   *     *  ******   *   *   ******   *****  *     * *
    //******************************************************
    //**********************************************************************
    //   ENABLING THE MAIN SHADER AND RESETTING OPENGL COLOR AND DEPTH BITS
    //   UPDATING THE MODEL,VIEW AND PROJECTION MATRIX IN THE MAIN SHADER
    //   ALONG WITH THE CAMERA POSITION AND THE TOTAL NUMBER OF LIGHTS
    //   
    //   USING THE MAIN SHADER
    //   DRAW THE WORLD
    //   DRAW ALL THE ACTIVE GAMEOBJECTS WHICH INCLUDES 8 BUGGIES, 1 HEALTH BOX AND 1 WEAPON BOX
    //   DRAW ALL THE INANIMATE OBJECTS
    //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
    //**********************************************************************
    MainShader.Use();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(5, 1, GL_FALSE, FPSCamera.View.Addressof());
    glUniformMatrix4fv(6, 1, GL_FALSE, Projection.Addressof());
    glUniform4f(9, FPSCamera.CameraPosition.x, FPSCamera.CameraPosition.y, FPSCamera.CameraPosition.z, 1.0f);
    glUniform1i(10, GameLight::GetTotalLightCount());

    GameObject::RealWorld->Draw();

    for (GameObject* Player : GameObject::Players)
    {
        if (Player->isAlive)
        {
            Player->Draw();
        }
    }
    for (GameObject* Enemy : GameObject::Enemies)
    {
        if (Enemy->isAlive)
        {
            Enemy->Draw();
        }
    }
    for (GameObject* Inanimate : GameObject::Inanimates)
        Inanimate->Draw();
    if (GameObject::HealthBox->isAlive)
        GameObject::HealthBox->Draw();
    if (GameObject::WeaponBox->isAlive)
        GameObject::WeaponBox->Draw();

    //*********************************************
    //   USING THE HELPER SHADER
    //   DRAW THE CURRENTLY SELECTED GAMEWORLDCUBE IN YELLOW TINT
    //   DRAW THE CURRENTLY SELECTED PLAYER BUGGY  BOX IN BLUE TINT
    //   DRAW THE CURRENTLY SELECTED ENEMY BUGGY BOX IN RED TINT
    //   DRAW THE HEALTHBOX BOUNDING BOX
    //   DRAW THE WEAPONBOX BOUNDING BOX
    //   DRAW THE ACTIVE BULLET OBJECT
    //   DRAW THE ACTIVE BULLETFIRE OBJECT
    //   DRAW THE ACTIVE EXPLOSION OBJECT
    // 
    //   IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
    //*********************************************

    //OPENGL FUNCTIONS:
    //glBlendFunc - Pixels can be drawn using a function that blends the incoming (source) RGBA values with the RGBA values that
    //              are already in the frame buffer (the destination values). Blending is initially disabled. Use glEnable and glDisable
    //              with argument GL_BLEND to enable and disable blending.
    //              glBlendFunc defines the operation of blending for all draw buffers when it is enabled
    //              sfactor specifies which method is used to scale the source color components.
    //              dfactor specifies which method is used to scale the destination color components
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

    HelperShader.Use();
    Model.Clean();
    glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
    glUniformMatrix4fv(3, 1, GL_FALSE, FPSCamera.View.Addressof());
    glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (std::vector<GameObject*>::iterator it = GameObject::Players.begin(); it != GameObject::Players.end(); it++)
    {
        if ((*it)->isAlive)
        {
            if ((*it)->_Bullet->isAlive == true)
                (*it)->_BulletFire->Draw((*it)->_Bullet->Draw());
            (*it)->Info->Update((*it)->Health, (*it)->WeaponQuantity, &((*it)->BoundingBox->Centroid));
            (*it)->Info->Draw();
            AlivePlayers.push_back(*it);
        }
        else if (!(*it)->isAlive && (*it)->_Explosion->isExploding)
        {
            if (!isCameraShaking)
            {
                (*it)->_Explosion->isCameraShaking = true;
                isCameraShaking = true;
            }
            (*it)->_Explosion->Draw(&FPSCamera, &isCameraShaking);
            AlivePlayers.push_back(*it);
        }
        else if (!(*it)->isAlive && !(*it)->_Explosion->isExploding)
        {
            std::cout << "\n";
            delete (*it);
            std::cout << "\n";
        }
    }
    
    for (std::vector<GameObject*>::iterator it = GameObject::Enemies.begin(); it != GameObject::Enemies.end(); it++)
    {
        if ((*it)->isAlive)
        {
            if ((*it)->_Bullet->isAlive == true)
                (*it)->_BulletFire->Draw((*it)->_Bullet->Draw());
            (*it)->Info->Update((*it)->Health, (*it)->WeaponQuantity, &((*it)->BoundingBox->Centroid));
            (*it)->Info->Draw();
            AliveEnemies.push_back(*it);
        }
        else if (!(*it)->isAlive && (*it)->_Explosion->isExploding)
        {
            if (!isCameraShaking)
            {
                (*it)->_Explosion->isCameraShaking = true;
                isCameraShaking = true;
            }
            (*it)->_Explosion->Draw(&FPSCamera, &isCameraShaking);
            AliveEnemies.push_back(*it);
        }
        else if (!(*it)->isAlive && !(*it)->_Explosion->isExploding)
        {
            std::cout << "\n";
            delete (*it);
            std::cout << "\n";
        }
    }
    
    GameObject::Players.clear();
    for (std::vector<GameObject*>::iterator it = AlivePlayers.begin(); it != AlivePlayers.end(); it++)
        GameObject::Players.push_back(*it);
    AlivePlayers.clear();
    GameObject::Enemies.clear();
    for (std::vector<GameObject*>::iterator it = AliveEnemies.begin(); it != AliveEnemies.end(); it++)
        GameObject::Enemies.push_back(*it);
    AliveEnemies.clear();
        
    if (GameObject::SelectedPlayer != nullptr)
    {
        GameObject::SelectedPlayer->BoundingBox->DrawForeground();
    }
    if (GameObject::SelectedWorldCube != nullptr)
    {
        GameObject::SelectedWorldCube->DrawForeground();
    }
    if (GameObject::SelectedEnemy != nullptr)
    {
        GameObject::SelectedEnemy->BoundingBox->DrawForeground();
    }
    if (GameObject::HealthBox->isAlive)
    {
        GameObject::HealthBox->BoundingBox->DrawForeground();
    }
    if (GameObject::WeaponBox->isAlive)
    {
        GameObject::WeaponBox->BoundingBox->DrawForeground();
    }

    //*******************************************************
    //   USING THE HELPER SHADER DRAW SKYBOX AT THE VERY END IN THE FOREGOUND OR DEFAULT FRAMEBUFFER
    //   SO THE SKYBOX REMAINS AHEAD OF ALL OBJECTS AND VISIBLE ALL THE TIME
    //*******************************************************
    Model.Clean();
    glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
    skybox.Draw();

    //******************************************
    //   USING THE HELPER SHADER
    //   DRAW THE WORLD OBJECT WHICH INVOLVES DRAWING ALL THE GAMEWORLDCUBE OBJECTS WITH UNIQUE COLORS
    //   DRAW ALL THE BOUNDING BOX OBJECTS FOR ALL THE BUGGY OBJECTS WITH UNIQUE COLORS
    //   DRAW ALL THE BOUNDING BOX OBJECTS FOR THE HEALTH AND WEAPON BOX WITH UNIQUE COLORS
    //   IN THE BACKGROUND FRAMEBUFFER
    //******************************************
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    HelperShader.Use();
    glDisable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (GameWindow::ViewPortChanged == true)
    {
        //VIEWPORT IS A GLOBAL STATE FOR OPENGL
        //WHEN IT CHANGES, UPDATE THE BACKGROUND FRAMEBUFFER'S TEXTURE BUFFER AND DEPTH BUFFER WITH NEW WINDOW WIDTH AND HEIGHT
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GameWindow::Width, GameWindow::Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GameWindow::Width, GameWindow::Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        GameWindow::ViewPortChanged = false;
    }

    glUniformMatrix4fv(2, 1, GL_FALSE, Model.Addressof());
    glUniformMatrix4fv(3, 1, GL_FALSE, FPSCamera.View.Addressof());
    glUniformMatrix4fv(4, 1, GL_FALSE, Projection.Addressof());

    FakeWorld->Draw();

    for (GameObject* Player : GameObject::Players)
    {
        if (Player->isAlive)
            Player->BoundingBox->DrawBackground();
    }
    for (GameObject* Enemy : GameObject::Enemies)
    {
        if (Enemy->isAlive)
            Enemy->BoundingBox->DrawBackground();
    }
    if (GameObject::HealthBox->isAlive)
    {
        GameObject::HealthBox->BoundingBox->DrawBackground();
    }
    if (GameObject::WeaponBox->isAlive)
    {
        GameObject::WeaponBox->BoundingBox->DrawBackground();
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
    //
    //OPENGL FUNCTIONS:
    //glReadBuffer - glReadBuffer specifies a color buffer as the source for subsequent glReadPixels, glCopyTexImage1D,
    //               glCopyTexImage2D, glCopyTexSubImage1D, glCopyTexSubImage2D, and glCopyTexSubImage3D commands
    //glReadPixels - glReadPixels and glReadnPixels return pixel data from the frame buffer, starting with the pixel
    //               whose lower left corner is at location (x, y), into client memory starting at location data.
    //               Several parameters control the processing of the pixel data before it is placed into client memory
    //
    //WINDOWS FUNCTIONS:
    //GetCursorPos - Retrieves the position of the mouse cursor, in screen coordinates.
    //ScreenToClient - The function uses the window identified by the hWnd parameter and the screen coordinates given in the POINT structure
    //                 to compute client coordinates. It then replaces the screen coordinates with the client coordinates


    glReadBuffer(GL_COLOR_ATTACHMENT0);
    GetCursorPos(&MousePointerInfo);
    ScreenToClient(GameWindow::WindowHandle, &MousePointerInfo);
    glReadPixels(MousePointerInfo.x, GameWindow::Height - MousePointerInfo.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    std::string Key = GameWindow::ReadKey(pixels[0], pixels[1], pixels[2]);
    PixelPointer = GameWindow::PixelMap[Key];

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

    //HANDLE MOUSE HOVER USER INPUT ONLY WHEN A PLAYER HAS ALREADY BEEN SELECTED
    if (GameObject::SelectedPlayer != nullptr && PixelPointer->type == 1)
    {
        //SELECTED PLAYER IS HOVERING MOUSE OVER A WORLD CUBE SO IT CAN MOVE OVER TO THAT LOCATION
        GameWorldCube* WC = (GameWorldCube*)PixelPointer->payload;
        GameObject::SelectedWorldCube->Centroid = WC->Centroid;
        GameObject::SelectedWorldCube->FinalMatrix = GameMath::Transform::Translate(GameObject::SelectedWorldCube->Centroid);
        GameObject::SelectedEnemy = nullptr;
    }
    else if (GameObject::SelectedPlayer != nullptr && PixelPointer->type == 2)
    {       
        //SELECTED PPLAYER IS HOVERING MOUSE OVER A BOUNDINGBOX WHICH COULD BE PLAYER OR ENEMY BUT NOT ITSELF.
        GameBoundingBox* BB = ((GameBoundingBox*)PixelPointer->payload);
        if (BB->Object->ObjectType <= 2 && BB->Object->Index!=GameObject::SelectedPlayer->Index)
        {
            GameObject::SelectedEnemy = BB->Object;
            GameObject::SelectedEnemy->BoundingBox->ColorTint.x = GameSettings::EnemySelectColorRed;
            GameObject::SelectedEnemy->BoundingBox->ColorTint.y = GameSettings::EnemySelectColorGreen;
            GameObject::SelectedEnemy->BoundingBox->ColorTint.z = GameSettings::EnemySelectColorBlue;
            GameObject::SelectedWorldCube->FinalMatrix = GameMath::Transform::Scale(0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        GameObject::SelectedEnemy = nullptr;
        GameObject::SelectedWorldCube->FinalMatrix = GameMath::Transform::Scale(0.0f, 0.0f, 0.0f);
    }

    //HANDLE MOUSE LEFT BUTTON AND RIGHT BUTTON CLICKS
    if (GameWindow::MouseLButtonPressed)
    {
        //LEFT CLICK ON A BOUNDING BOX WHICH COULD BE PLAYER,ENEMY,HEALTH OR WEAPON BOX
        if (PixelPointer->type == 2)
        {
            GameBoundingBox* BB = ((GameBoundingBox*)PixelPointer->payload);
            //NO PLAYER HAS BEEN SELECTED YET
            if (GameObject::SelectedPlayer == nullptr) 
            {
                //ONLY SELECT THAT BUGGY WHICH IS A PLAYER TYPE
                if (BB->Object->ObjectType == 1)
                {
                    GameObject::SelectedPlayer = BB->Object;
                    GameObject::SelectedPlayer->BoundingBox->ColorTint.x = GameSettings::PlayerSelectColorRed;
                    GameObject::SelectedPlayer->BoundingBox->ColorTint.y = GameSettings::PlayerSelectColorGreen;
                    GameObject::SelectedPlayer->BoundingBox->ColorTint.z = GameSettings::PlayerSelectColorBlue;
                    GameObject::SelectedPlayer->HALT = false;
                }
            }
            //PLAYER WAS SELECTED BEFORE
            else if (GameObject::SelectedPlayer != nullptr)
            {
                //EITHER A PLAYER BUGGY OR A ENEMY BUGGY HAS BEEN CLICKED ON. MARK IT AS ATTACK OBJECT AS LONG AS ITS NOT ITSELF
                if (BB->Object->ObjectType <= 2 && BB->Object->Index != GameObject::SelectedPlayer->Index)
                {
                    GameObject::SelectedPlayer->AttackObject = BB->Object;
                    GameObject::SelectedPlayer = nullptr;
                }
                //HEALTH OR WEAPON BOX SELECT. GO TOWARDS IT !
                else if (BB->Object->ObjectType >= 3)
                {
                    GameObject::SelectedPlayer->DestinationWorldCube = BB->Object->SourceWorldCube;
                    GameObject::SelectedPlayer = nullptr;
                }
            }
        }
        //LEFT CLICK ON A WORLD CUBE
        else if (PixelPointer->type == 1)
        {
            GameWorldCube* WC = (GameWorldCube*)PixelPointer->payload;
            //A PLAYER HAS BEEN SELECTED BEFORE SO GO TOWARDS THE SELECTED WORLDCUBE 
            if (GameObject::SelectedPlayer != nullptr)
            {
                GameObject::SelectedPlayer->DestinationWorldCube = WC;
                GameObject::SelectedPlayer = nullptr;
            }
        }
    }
    else if (GameWindow::MouseRButtonPressed)
    {
        //RIGHT CLICKED ON A BOUNDING BOX. IT ITS A PLAYER BOUNDING BOX THEN HALT ALL ITS MOVEMENTS AND ATTACKS
        if (PixelPointer->type == 2)
        {
            GameBoundingBox* BB = ((GameBoundingBox*)PixelPointer->payload);
            if (BB->Object->ObjectType == 1)
                BB->Object->HALT = true;
        }
       
        //UNSELECT THE PLAYER ON EVERY RIGHT CLICK ON ANY OTHER OBJECT
        if (GameObject::SelectedPlayer != nullptr)
            GameObject::SelectedPlayer = nullptr;
        
    }


    //*********************************************************************************************************
    //  REVERTING TO THE DEFAULT FRAME BUFFER FROM THE BACKGROUND FRAME BUFFER SO IT CAN RENDERED ON THE SCREEN
    //*********************************************************************************************************
    //
    //WINDOWS FUNCTIONS:
    //SwapBuffers - The SwapBuffers function exchanges the front and back buffers if the current pixel format
    //                for the window referenced by the specified device context includes a back buffer.
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SwapBuffers(GameWindow::DeviceContext);


    if (GameObject::Players.size() > 0 && GameObject::Enemies.size() == 0)
    {
        FPSCamera.CameraPosition = OriginalCameraPosition;
        FPSCamera.YawAngle = 0.0f;
        FPSCamera.PitchAngle = 0.0f;
        if (std::find(CompletedLevels.begin(), CompletedLevels.end(), Help->State) == CompletedLevels.end())
            CompletedLevels.push_back(Help->State);
        
        if (CompletedLevels.size() >= 5)
            Help->State = 7;// All Levels Completed - You Won All Page
        else
            Help->State = 6;// Current Level Completed - You Won Page
        UnloadAssets(FakeWorld);
        GameStarted = false;
    }
    else if (GameObject::Players.size() == 0 && GameObject::Enemies.size() > 0)
    {
        FPSCamera.CameraPosition = OriginalCameraPosition;
        FPSCamera.YawAngle = 0.0f;
        FPSCamera.PitchAngle = 0.0f;
        Help->State = 8;//You Lost Page
        UnloadAssets(FakeWorld);
        GameStarted = false;
    }
   
    

    //******************************************************
    //  CALCULATING THE TIME TAKEN FOR ONE FRAME TO PROCESS
    //******************************************************
    GameWindow::Time_Taken_Per_Frame = GetTickCount() - FrameStartTime;
}
//**********************************************
//          GAME LOOP ENDS HERE
//**********************************************
#endif
return 0;
}


