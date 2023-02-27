#include <GameObject/GameObject.h>
int GameObject::ObjectIndex=0;
unsigned int GameObject::ALL_MaterialData_SSBO;
unsigned int GameObject::ALL_WeightData_SSBO;
unsigned int GameObject::ALL_ChildBoneData_SSBO;
unsigned int GameObject::ALL_BoneData_SSBO;
int GameObject::WeightData_Binding = 0;
int GameObject::ChildBoneData_Binding = 1;
int GameObject::BoneData_Binding = 2;
int GameObject::MaterialData_Binding = 3;
bool GameObject::ALL_Objects_Loaded = false;
std::vector<GameObject*> GameObject::Players;
std::vector<GameObject*> GameObject::Enemies;
std::vector<GameObject*> GameObject::Inanimates;
GameObject* GameObject::HealthBox=nullptr;
GameObject* GameObject::WeaponBox=nullptr;
GameObject* GameObject::RealWorld=nullptr;
GameObject* GameObject::SelectedPlayer=nullptr;
GameObject* GameObject::LastSelectedPlayer = nullptr;
GameObject* GameObject::SelectedEnemy = nullptr;
GameWorldCube* GameObject::SelectedWorldCube=nullptr;
/*************************************************
MAIN PROPERTIES USED IN THE OBJECT DATA STRUCTURE:

INTEGER ObjectIndex - IT UNIQUELY IDENTIFIES AN OBJECT

INTEGER ObjectType - THIS PROPERTY SPECIFIES WHAT KIND OF THE OBJECT, THIS OBJECT IS. THE OPTION ARE:
             1 - PLAYER OBJECT WHICH IS A BUGGY OBJECT THAT IS MOVABLE
             2 - ENEMY OBJECT WHICH IS A BUGGY OBJECT THAT IS MOVABLE
             3 - HEALTH BOX OBJECT WHICH INCREASES THE HEALTH AMOUNT BY 20 %
             4 - WEAPON BOX OBJECT WHICH INCREASES THE WEAPON AMOUNT BY 20 %

BOOLEAN isAlive - THIS PROPERTY DETERMINES IF THE OBJECT HAS ANY IMPACT ON THE GAME PLAY AND WHETHER
                  IT WILL BE RENDERED ON THE SCREEN. THE OPTIONS ARE:
                  TRUE - THE OBJECT WILL BE RENDERED
                  FALSE - THE OBJECT WILL NOT BE RENDERED

FLOAT Health - THIS PROPERTY DETERMINES THE HEALTH OF THE OBJECT. THE VALUE CAN VARY FROM 0.0 TO 100.0
               IF THE VALUE ENDS UP BEING 0.0 THEN THE OBJECT IS DESTROYED

FLOAT Speed - THIS PROPERTY DETERMINES HOW FAST THE BUGGY OBJECT MOVES AND ITS VALUE IS SET IN METERS PER SECOND
              BUT LATER ITS COVERTED TO FLOATING POINT PER SECOND USING THE SPEEDCONSTANT PROPERTY

FLOAT SpeedConstant - THIS PROPERTY CONVERTS AN OBJECT SPEED FROM METERS PER SECOND TO FLOATING POINT VALUE PER SECOND
                      1 METER = 100 CENTIMETER AND 1 CENTIMETER = 0.16 FLOATING POINT VALUE (MOVEMENT ON SCREEN)

GAMEWORLDCUBE DestinationWorldCube - THIS PROPERTY SPECIFIES WHICH WORLD CUBE THE OBJECT IS NEEDED TO MOVE TO.
                                     WHEN THIS PROPERTY IS SET TO NULLPTR, THE OBJECT DOES NOT MOVE
                                     WHEN ITS SET TO A VALID WORLD CUBE OBJECT THEN THE OBJECT MOVES TO THAT WORLD CUBE

GAMEWORLDCUBE SourceWorldCube - THIS PROPERTY DETERMINES WHAT THE CURRENT LOCATION OF THE OBJECT IS BY REFERENCING THE
                                WORLD CUBE THAT IT HAS OCCUPIED

GAMEOBJECT AttackObject - THIS PROPERTY DETERMINES THE BUGGY OBJECT THAT THIS OBJECT NEEDS TO ATTACK. IF SET TO NULLPTR
                          THE OBJECT DOES NOT ATTACK ANY OTHER OBJECT

BOOLEAN TargetOutOfRange - THIS PROPERTY DETERMINES IF THE ATTACK OBJECT IS OUT OF WEAPON RANGE FOR THE CURRENT OBJECT.
                           THE OPTIONS ARE:
                           TRUE - THE ATTACK OBJECT IS OUT OF WEAPON RANGE IN WHICH CASE THE ATTACK OBJECT IS SET TO NULLPTR
                           FALSE - THE ATTACK OBJECT IS NOT OUT OF WEAPON RANGE

FLOAT WeaponDamageRate - THIS PROPERTY DETERMINES THE AMOUNT BY WHICH THE ATTACK OBJECT'S HEALTH WILL BE LOWERED WHEN THE
                         CURRENT OBJECT ATTACKS THE ATTACK OBJECT

FLOAT WeaponConsumptionRate - THIS PROPERTY DETERMINES THE AMOUNT BY WHICH THE ATTACJ OBJECT'S WEAPON AMOUNT GETS LOWERED
                              WHEN ITS ATTACKING AN ATTACK OBJECT

FLOAT WeaponQuantity - THIS PROPERTY DETERMINES THE AMOUNT OF AMMO THAT THE OBJECT HAS. IT CAN VARY FROM 0.0 TO 100.0

FLOAT WeaponRange - THIS PROPERTY DETERMINES THE MAXIMUM DISTANCE UP TILL THE OBJECT CAN SHOOT / ATTACK ANOTHER OBJECT

GAMEMATH::VECTOR3 BuggyFront - THIS DIRECTION VECTOR DETERMINES THE DIRECTION THE BUGGY IS POINTING TOWARDS

GAMEMATH::VECTOR3 CannonFront - THIS DIRECTION VECTOR DETERMINES THE DIRECTION THE BUGGY'S MACHINE GUN IS POINTING AT
*************************************************/

/*************************************************
EVERY MOVABLE OBJECT ALSO CALLED "BUGGY" IN THIS PROGRAM'S CONTEXT HAS THE FOLLOWING TRANSFORMATIONS LINKED TO IT
1) THE OBJECT ITSELF CAN BE ROTATED WITH IT BEING AT THE ORIGIN, THIS MAKES IT A LOCAL TRANSFORMATION
2) AFTER THE ROTATION IS APPLIED TO IT THEN THE OBJECTS CAN BE TRANSLATED TO ANY WORLD COORDINATE
3) THE OBJECT ALSO CONTAINS "MACHINE GUN TURRET" MESH ON TOP OF IT THAT ROTATES ON THE WORLD Y AXIS
4) ATTACHED TO THE "MACHINE GUN TURRET" IS A "MACHINE GUN ROLLER" MESH THAT ROTATES ON THE WORLD X AXIS

THE TRANSFORMATIONS THAT APPLY TO EACH "BUGGY" OBJECT IS APPLIED TO ITS "MACHINE GUN TURRET" MESH
AND THE "MACHINE GUN" MESH. AFTER THAT EACH OF THOSE OBJECTS GETS THEIR LOCAL ROTATION TRASFORMATIONS APPLIED TO THEM

EVERY BUGGY HAS THE FOLLOWING BONES LINKED TO THE BUGGY MESH:
1) BONE "FRONT WHEEL" WHICH IS HAS ITS HEAD PLACED AT THE CENTER OF THE LEFT FRONT WHEEL
   AND THE TAIL PLACED AT THE CENTER OF THE RIGHT FRONT WHEEL

   ROTATING THIS BONE ABOUT THE WORLD -Z AXIS MAKES THE FRONT WHEEL ROTATE WITH THE CENTER OF THE TWO
   FRONT WHEELS BEING THE AXIS OF THE ROTATION

2) BONE "BACK WHEEL" WHICH HAS ITS HEAD PLACED AT THE CENTER OF THE LEFT BACK WHEEL
   AND THE TAIL PLACED AT THE CENTER OF THE RIGHT BACK WHEEL

   ROTATING THIS BONE ABOUT THE WORLD -Z AXIS MAKES THE FRONT WHEEL ROTATE WITH THE CENTER OF THE TWO
   BACK WHEELS BEING THE AXIS OF THE ROTATION

3) BONE "MACHINEGUN" WHICH HAS ITS HEAD PLACED AT THE CENTER OF THE "MACHINE GUN TURRET" MESH

   ROTATING THIS BONE ABOUT THE WORLD Y AXIS MAKES "MACHINEGUN" ROTATE AND POINT AT OTHER OBJECTS

4) BONE "MACHINEGUNROLLER " WHICH HAS ITS HEAD ALIGNED WITH THE HEAD OF THE "MACHINEGUN" BONE BUT ITS
   TAIL IS PERPENDICULAR AT THE AXIS OF ROTATION OF THE "MACHINEGUN" BONE. "MACHINEGUN" BONE IS THE PARENT
   OF THE "MACHINEGUNROLLER" BONE SO ALL ROTATIONS FROM THE "MACHINEGUN" BONE ARE INHERITED BY THIS BONE


                          "MACHINEGUNROLLER" BONE HEAD
             < * * * * * * * "MACHINEGUN" BONE HEAD
"MACHINEGUNROLLER"         *
 BONE TAIL                 *
                           *
                           *
                           *
                           ^ "MACHINEGUN" BONE TAIL
*************************************************/
GameObject::GameObject() // @suppress("Class members should be properly initialized")
{
this->Index=ObjectIndex++;
}

GameObject::~GameObject()
{
    if (Info != nullptr)
        delete Info;
    if (BoundingBox != nullptr)
        delete BoundingBox;
    if (_Bullet != nullptr)
        delete _Bullet;
    if (_BulletFire != nullptr)
        delete _BulletFire;
    if (_Explosion != nullptr)
        delete _Explosion;
    Unload();
    std::cout << "GAME OBJECT #" << std::to_string(Index) << " GOT DESTROYED\n";
}
/*************************************************
THIS FUNCTION 'DESTROYS' AN OBJECT SO IT DOES NOT RENDER ANYMORE BOTH IN THE FOREGROUND FRAME BUFFER
AND THE BACKGROUND FRAME BUFFER SO IT DOES NOT HAVE ANY IMPACT ON THE GAME WHATSOEVER

1) FIRST IT CHECKS IF THE OBJECT IS ACTIVE BY CHECKING THE "ISALIVE" BOOLEAN VARIABLE OF THE OBJECT
   THIS IS REQUIRED AS MULTIPLE OBJECTS COULD BE ASSIGNED THE SAME TARGET OBJECT SO ONE OBJECT
   COULD HAVE ALREADY DESTROYED A TARGET ASSIGNED IT WHILE THE SECOND A.I OBJECT TRIES TO DESTOY THAT SAME
   TARGET. TO PREVENT THAT FROM HAPPENING THIS CHECK IS REQUIRED

2) IF THE OBJECT IS ALIVE AND IS NEEDED TO BE DESTROYED THEN
   A) WE SET THE "SOURCEWORLDCUBE" OBJECT REFERENCE TO NULLPTR WHICH WAS EARLIER OCCUPIED BY
      THIS OBJECT THAT IS BEING DESTROYED. THIS ALLOWS THAT WORLD CUBE TO OCCUPY OTHER OBJECTS
   B) WE SET THE "DESTINATIONWORLDCUBE" TO NULLPTR AS THIS OBJECT WONT BE MOVING ANYMORE AFTER ITS
      DESTROYED
   C) WE SET THE "ATTACKOBJECT" TO NULLPTR SINCE THIS OBJECT WONT BE ATTACKING ANYOTHER OBJECT AFTER ITS DESTROYED
   D) WE SET THE "SOURCEWORLDCUBE" TO NULLPTR SINCE THIS OBJECT IS NOT OCCUPYING THAT WORLD CUBE ANYMORE
   E) WE SET THE BOUNDINGBOX VALUES TO 0.0
   F) WE SET THE "ISALIVE" VARIABLE OF THE OBJECT TO FALSE INDICATING THAT THE OBJECT IS NOT ALIVE ANYMORE
   G) WE SET THE "ISEXPLODING" VARIABLE OF THE OBJECT TO TRUE TO INITIATE THE EXPLOSING EFFECT FOR THAT OBJECT
*************************************************/
void GameObject::Destroy()
{
    if (isAlive)
    {
        _Explosion->isExploding = true;
        SourceWorldCube->Object = nullptr;
        DestinationWorldCube = nullptr;
        AttackObject = nullptr;
        SourceWorldCube = nullptr;
        isAlive = false;
    }
}

void GameObject::Load()
{
    //*************************************************************
    // LOAD TEXTURE DATA
    //*************************************************************
    unsigned char* ImageData = nullptr;
    int Width = 0;
    int Height = 0;
    if (Mesh.Materials.size() > 0)
    {
        for (GameMesh::MaterialData* Mat : Mesh.Materials)
        {
            std::cout << "LOADING MATERIAL " + std::to_string(Mat->Index) + "\n";
            if (Mat->DiffuseMap != "")
            {
                glGenTextures(1, &DiffuseMap);
                glBindTexture(GL_TEXTURE_2D, DiffuseMap);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Width = 0;
                Height = 0;
                ImageData = nullptr;
                ImageData = GameBMP::LoadBMP((Mat->DiffuseMap).c_str(), &Width, &Height, "OBJECT# " + std::to_string(Index) + " LOADED DIFFUSE MAP");
                if (ImageData != nullptr)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    GameBMP::FreeBMP(ImageData);
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                std::cout << "OBJECT SPECULAR MAP = " + std::to_string(SpecularMap) + "\n";
            }
            if (Mat->SpecularMap != "")
            {
                glGenTextures(1, &SpecularMap);
                glBindTexture(GL_TEXTURE_2D, SpecularMap);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Width = 0;
                Height = 0;
                ImageData = nullptr;
                ImageData = GameBMP::LoadBMP((Mat->SpecularMap).c_str(), &Width, &Height, "OBJECT# " + std::to_string(Index) + " LOADED SPECULAR MAP");
                if (ImageData != nullptr)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, Width, Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, ImageData);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    GameBMP::FreeBMP(ImageData);
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                std::cout << "OBJECT SPECULAR MAP = " + std::to_string(SpecularMap) + "\n";
            }
            if (Mat->AmbientMap != "")
            {
                glGenTextures(1, &AmbientMap);
                glBindTexture(GL_TEXTURE_2D, AmbientMap);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Width = 0;
                Height = 0;
                ImageData = nullptr;
                ImageData = GameBMP::LoadBMP((Mat->AmbientMap).c_str(), &Width, &Height, "OBJECT# " + std::to_string(Index) + " LOADED AMBIENT MAP");
                if (ImageData != nullptr)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, ImageData);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    GameBMP::FreeBMP(ImageData);
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                std::cout << "OBJECT AMBIENT MAP = " + std::to_string(AmbientMap) + "\n";
            }
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
    for (unsigned int i = 0; i < Mesh.Faces.size(); i++)
    {
        //ITERATE THROUGH THE 3 VERTICES OF A FACE
        for (int j = 0; j < 3; j++)
        {
            //*************************************************************
            // LOAD THE X , Y AND Z COORDINATE OF A VERTEX
            //*************************************************************
            v.push_back(Mesh.Vertices[Mesh.Faces[i]->Indices[j]]->x);
            v.push_back(Mesh.Vertices[Mesh.Faces[i]->Indices[j]]->y);
            v.push_back(Mesh.Vertices[Mesh.Faces[i]->Indices[j]]->z);
            //*************************************************************
            // LOAD VERTEX INDEX OF A VERTEX
            //*************************************************************
            vi.push_back(Mesh.Faces[i]->Indices[j]);
            //*************************************************************
            // LOAD UV COORDINATES OF A VERTEX
            //*************************************************************
            if (Mesh.Faces[i]->UVCoords.size() > 0)
            {
                t.push_back(Mesh.Faces[i]->UVCoords[j]->x);
                t.push_back(Mesh.Faces[i]->UVCoords[j]->y);
            }
            //*************************************************************
            // LOAD MATERIAL INDEX FOR A FACE
            //*************************************************************
            if (Mesh.Materials.size() > 0)
                m.push_back(Mesh.Materials.at(Mesh.Faces[i]->Material_index)->Index);
        }
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VVBO);
    glGenBuffers(1, &VIVBO);
    glGenBuffers(1, &TVBO);
    glGenBuffers(1, &MVBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VVBO);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), &v[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (t.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, TVBO);
        glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(float), &t[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    if (m.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, MVBO);
        glBufferData(GL_ARRAY_BUFFER, m.size() * sizeof(int), &m[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(int), (void*)0);
        glEnableVertexAttribArray(2);
        NoMaterialFlag = 0;//SETTING THE OBJECT'S "NOMATERIALFLAG" VALUE FROM DEFAULT -1 TO 0 INDICATING THAT MATERIAL IS PRESENT
    }

    glBindBuffer(GL_ARRAY_BUFFER, VIVBO);
    glBufferData(GL_ARRAY_BUFFER, vi.size() * sizeof(float), &vi[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);

    VAOCount = vi.size();
    BoundingBox = Mesh.BoundingBox;//COPYING OVER THE MESH' BOUNDING BOX OVER TO THE OBJECT'S BOUNDING BOX
    BoundingBox->Object = this;//CREATING A REFERENCE TO THIS OBJECT IN THIS OBJECT'S BOUNDING BOX.
                               //SO THIS GAMEOBJECT CAN BE REFERENCED DIRECTLY FROM WITHIN ITS BOUNDING BOX
    std::cout << "OBJECT# " + std::to_string(Index) + " GOT LOADED INTO GPU\n";
    v.clear();
    t.clear();
    m.clear();
    vi.clear();
}

void GameObject::Unload()
{
    if (VVBO >= 0)
        glDeleteBuffers(0, &VVBO);
    if (TVBO >= 0)
        glDeleteBuffers(0, &TVBO);
    if (MVBO >= 0)
        glDeleteBuffers(0, &MVBO);
    if (VIVBO >= 0)
        glDeleteBuffers(0, &VIVBO);
    if (VAO >= 0)
        glDeleteVertexArrays(0, &VAO);
    if (DiffuseMap >= 0)
        glDeleteTextures(1, &DiffuseMap);
    if (SpecularMap >= 0)
        glDeleteTextures(1, &SpecularMap);
    if (AmbientMap >= 0)
        glDeleteTextures(1, &AmbientMap);
    std::cout << "OBJECT# " + std::to_string(Index) + " GOT UNLOADED FROM GPU";
}

/*************************************************
THIS FUNCTION UPDATES THE MATRIX OF THE BONE IDENTIFIED BY THE ARGUMENT "BONENAME" IN THE VERTEX
SHADER'S STORAGE BUFFER CALLED "BONE". THIS MATRIX IS LATER USED FOR TRANSFORMING THE VERTICES
IN THE MESH WHICH ARE ASSIGNED TO THIS BONE VIA VERTEX GROUP OF SIMILAR NAME
*************************************************/
void GameObject::SetBoneMatrix(std::string BoneName,GameMath::Matrix4x4 Mat)
{
int BoneOffset=((Armature.Bones[BoneName]->Index * 52) + 4) * sizeof(float);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, GameObject::ALL_BoneData_SSBO);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER,GameObject::BoneData_Binding,GameObject::ALL_BoneData_SSBO);
glBufferSubData(GL_SHADER_STORAGE_BUFFER,BoneOffset, 16 *  sizeof(float), Mat.Addressof());
glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

/*************************************************
THIS FUNCTION DETECTS COLLISION BETWEEN A POINT AND A BOUNDING BOX BY CHECKING IF THE POINT
IS INSIDE THE BOUNDING BOX OR NOT.

          * * * * * * * * *
         **    MAXY      **
        * *             * *
       * * * * * * * * *  *
       *  *     p      *  *
MINX   *  *  (0,0,0)   *  * MAXX
       * *             * *
       * * * * * * * * *
               MINY

FOR THE POINT P TO BE INSIDE THE BOUNDING BOX THE FOLLOWING CONDITIONS SHOULD BE TRUE:
1) P'S X COORDINATES SHOULD BE GREATER THAN THE MINX COORDINATE OF THE BOUNDING BOX ON THE WORLD X AXIS
2) P'S X COORDINATES SHOULD BE LESS THAN THE MAXX COORDINATE OF THE BOUNDING BOX ON THE WORLD X AXIS
3) P'S Y COORDINATES SHOULD BE GREATER THAN THE MINY COORDINATE OF THE BOUNDING BOX ON THE WORLD Y AXIS
4) P'S Y COORDINATES SHOULD BE LESS THAN THE MAXY COORDINATE OF THE BOUNDING BOX ON THE WORLD Y AXIS
5) P'S Z COORDINATES SHOULD BE GREATER THAN THE MINZ COORDINATE OF THE BOUNDING BOX ON THE WORLD Z AXIS
6) P'S Z COORDINATES SHOULD BE LESS THAN THE MAXZ COORDINATE OF THE BOUNDING BOX ON THE WORLD Z AXIS

ONCE ALL THESE CONDITIONS ARE TRUE THEN THE FUNCTION RETURNS TRUE INDICATING THAT A COLLISION HAS OCCURED
*************************************************/
bool GameObject::CollisionDetection(GameMath::Vector3 *Point,GameBoundingBox *B2)
{
if((Point->x > B2->minx && Point->x < B2->maxx) && (Point->y > B2->miny && Point->y < B2->maxy) && (Point->z > B2->minz && Point->z < B2->maxz))
return true;
else
return false;
}

/*************************************************
THIS FUNCTIONS IS MEANT TO BE USED BY A.I OBJECTS TO TRACK DOWN THE CLOSEST ENEMY OBJECT TO IT
THAT IS WITHIN ITS WEAPON'S FIRING RANGE. THE PROCEDURE INVOLVED IS AS FOLLOWS:

1) GET A LIST OF ALL MOVABLE / BUGGY OBJECTS
2) ITERATE OVER ALL THOSE OBJECTS IN THAT LIST
   A) IF THE OBJECT IS NOT ALIVE OR
      IF THE OBJECT IS THE SAME OBJECT THAT IS CALLING THIS FUNCTION OR
      IF THE OBJECT IS NOT A ENEMY BUGGY OBJECT
      THEN SKIP THIS OBJECT IN THE LIST
   B) ELSE GET THE DISTANCE THAT OBJECT'S BOUNDING BOX AND THIS OBJECT'S BOUDNING BOX THAT IS CALLING THIS FUNCTION
      SAVE THIS DISTANCE AS THE "LEAST DISTANCE" FOR THE CALLING OBJECT
      SAVE THE FOUND OBJECT AS THE "CLOSEST OBJECT" FOR THE CALLING OBJECT
   C) IF ANOTHER OBJECT IS FOUND IN THE LIST THAT IS EVEN CLOSER THAN THE PREVIOUS CLOSER OBJECT FOUND THEN
      OVERWRITE THE "LEAST DISTANCE" WITH DISTANCE FOR THAT NEWLY FOUND OBJECT
      OVERWRITE THE "CLOSEST OBJECT" WITH THE NEWLY FOUND OBJECT
   D) REPEAT (A), (B) AND (C) TILL ALL OBJECTS HAVE BEEN ITERATED THROUGH IN THAT LIST

3) CHECK IF THE "LEAST DISTANCE" FOUND EARLIER IS LESS THAN THE OBJECT'S WEAPON'S RANGE
   IF YES THEN SET THE CALLNG OBJECT'S VARIABLE "TARGETOUTOFRANGE" TO FALSE
   ELSE SET THE CALLNG OBJECT'S VARIABLE "TARGETOUTOFRANGE" TO TRUE
4) RETURN THE CLOSEST OBJECT FOUND OR NULLPTR IF NO OBJECT WAS FOUND
*************************************************/
GameWorldCube* GameObject::FindEmptyNeighbour(GameWorldCube* Source)
{
    if (Source->Neighbours != nullptr)
    {
        for (int i = 0; i < 8; i++)
        {
            if (Source->Neighbours[i]->Object == nullptr)
            {
                return Source->Neighbours[i];
            }
        }
        for (int i = 0; i < 8; i++)
        {
            FindEmptyNeighbour(Source->Neighbours[i]);
        }
    }
    else
        return nullptr;
}

GameWorldCube* GameObject::FindClosestEmptyNeighbour(GameWorldCube* Source, GameWorldCube* Destination)
{
    std::vector<std::pair<float,GameWorldCube*>> candidates;
    float distance=0.0f;
    for (int i = 0; i < 8; i++)
    {
        distance = (Destination->Centroid - Source->Neighbours[i]->Centroid).Length();
        if (Source->Neighbours[i]->Object == nullptr)
            candidates.push_back(std::pair<float, GameWorldCube*>(distance, Source->Neighbours[i]));
    }
    std::sort(candidates.begin(), candidates.end(), []
    (std::pair<float, GameWorldCube*>& p1, std::pair<float, GameWorldCube*>& p2)
        {
            return p1.first < p2.first;
        }
    );

    if (candidates.size() > 0)
        return (candidates[0]).second;
    else
    {
        for (int i = 0; i < 8; i++)
            FindClosestEmptyNeighbour(Source->Neighbours[i], Destination);
    }
}

GameObject* GameObject::AISearchTarget()
{
float Distance=0.0f;
float LeastDistance=1000000.0f;
GameObject* ClosestObject=nullptr;

for (GameObject* Player : GameObject::Players)
{
    if (!Player->isAlive)
        continue;
    else
    {
        Distance = (Player->BoundingBox->Centroid - BoundingBox->Centroid).Length();
        if (Distance < LeastDistance)
        {
            LeastDistance = Distance;
            ClosestObject = Player;
        }
    }
}
if(ClosestObject!=nullptr)
{
    if (LeastDistance >= GameSettings::EnemyWeaponRange)
        TargetOutOfRange = true;
    else    
        TargetOutOfRange=false;
    return ClosestObject;
}
else
return nullptr;
}




void GameObject::FrontWheelTurn(float Angle)
{
    GameMath::Vector3 Pivot;
    Pivot = Armature.Bones["FrontWheel"]->Head + ((Armature.Bones["FrontWheel"]->Tail - Armature.Bones["FrontWheel"]->Head).Normalize()) * 0.5f;

    float RAngle = (float)((int)10.0f % 360);
    GameMath::Matrix4x4 RM;
    GameMath::Matrix4x4 pTM = GameMath::Transform::Translate(Armature.Bones["FrontWheel"]->Head);
    GameMath::Matrix4x4 nTM = GameMath::Transform::Translate(-Armature.Bones["FrontWheel"]->Head);
    RM = pTM * GameMath::Transform::RotateQ(-ZAxis, RAngle) * nTM;
        
    pTM = GameMath::Transform::Translate(Pivot);
    nTM = GameMath::Transform::Translate(-Pivot);
    RM = pTM * GameMath::Transform::RotateQ(YAxis, Angle) * nTM * RM;
    SetBoneMatrix("FrontWheel", RM);
}

void GameObject::WheelRotation(bool FrontWheel_Clockwise=true, bool BackWheel_Clockwise = true,int TurnDirection=0)
{
    FrontWheelRotationAngle += FrontWheel_Clockwise ? 10.0f : -10.0f;
    BackWheelRotationAngle += BackWheel_Clockwise ? 10.0f : -10.0f;
    if (FrontWheelRotationAngle > 360.0f)
        FrontWheelRotationAngle = 0.0f;
    if (BackWheelRotationAngle > 360.0f)
        BackWheelRotationAngle = 0.0f;
    FrontWheelRotationAngle = (float)((int)FrontWheelRotationAngle % 360);
    BackWheelRotationAngle = (float)((int)FrontWheelRotationAngle % 360);
    
    GameMath::Vector3 Pivot = Armature.Bones["FrontWheel"]->Head + ((Armature.Bones["FrontWheel"]->Tail - Armature.Bones["FrontWheel"]->Head).Normalize()) * 0.5f;
    float RAngle = (float)((int)10.0f % 360);
    

    GameMath::Matrix4x4 RM;
    GameMath::Matrix4x4 pTM = GameMath::Transform::Translate(Armature.Bones["FrontWheel"]->Head);
    GameMath::Matrix4x4 nTM = GameMath::Transform::Translate(-Armature.Bones["FrontWheel"]->Head);
    RM = pTM * GameMath::Transform::RotateQ(-ZAxis, FrontWheelRotationAngle) * nTM;

    if (TurnDirection != 0)
    {
        RAngle = TurnDirection < 0 ? -30.0f : 30.0f;
        pTM = GameMath::Transform::Translate(Pivot);
        nTM = GameMath::Transform::Translate(-Pivot);
        RM = pTM * GameMath::Transform::RotateQ(YAxis, RAngle) * nTM * RM;
    }

    SetBoneMatrix("FrontWheel", RM);

    pTM = GameMath::Transform::Translate(Armature.Bones["BackWheel"]->Head);
    nTM = GameMath::Transform::Translate(-Armature.Bones["BackWheel"]->Head);
    RM = pTM * GameMath::Transform::RotateQ(-ZAxis, BackWheelRotationAngle) * nTM;
    SetBoneMatrix("BackWheel", RM);
}

void GameObject::PlayerControlledWorldMovement(int Time_Per_Frame, GameCamera::FPS& FPSCamera)
{
    FrontWheelTurn(0.0f);
    if (GetKeyState(87) & 0x8000) 
    {
        WheelRotation();
        float FrameDistance = (Speed * SpeedConstant) * Time_Per_Frame;
        GameMath::Matrix4x4 T = GameMath::Transform::Translate((FPSCamera.Front)*FrameDistance);
        BuggyTranslationMatrix = T * BuggyTranslationMatrix;
        BoundingBox->Centroid = T * BoundingBox->Centroid;
        FPSCamera.CameraPosition = T * FPSCamera.CameraPosition;
        BoundingBox->CalculateXYZ();

        BuggyFinalMatrix = BuggyTranslationMatrix * BuggyRotationMatrix;
        BoundingBox->FinalMatrix = BuggyFinalMatrix;
        FPSCamera.UpdateCameraView();
    }
    else if (GetKeyState(83) & 0x8000)
    {
        WheelRotation(false,false);
        float FrameDistance = (Speed * SpeedConstant) * Time_Per_Frame;
        GameMath::Matrix4x4 T = GameMath::Transform::Translate((FPSCamera.Front) * -FrameDistance);
        BuggyTranslationMatrix = T * BuggyTranslationMatrix;
        BoundingBox->Centroid = T * BoundingBox->Centroid;
        FPSCamera.CameraPosition = T * FPSCamera.CameraPosition;
        BoundingBox->CalculateXYZ();

        BuggyFinalMatrix = BuggyTranslationMatrix * BuggyRotationMatrix;
        BoundingBox->FinalMatrix = BuggyFinalMatrix;
        FPSCamera.UpdateCameraView();
    }
    else if (GetKeyState(65) & 0x8000)
    {
        WheelRotation(true,true,1);
        TurnAngle += 0.5f;
        FPSCamera.YawAngle += 0.5f;
        
        GameMath::Vector3 Position = GameMath::Vector3(BuggyTranslationMatrix.Matrix[3][0], BuggyTranslationMatrix.Matrix[3][1], BuggyTranslationMatrix.Matrix[3][2]);
        GameMath::Matrix4x4 RM;
        GameMath::Vector3 D = FPSCamera.CameraPosition - Position;
        GameMath::Vector3 pivot = D.Normalize() * D.Length();
        GameMath::Matrix4x4 r = GameMath::Transform::RotateQ('Y', TurnAngle);
        GameMath::Matrix4x4 n = GameMath::Transform::Translate(-pivot);
        GameMath::Matrix4x4 p = GameMath::Transform::Translate(pivot);
        BuggyRotationMatrix = p * r * n;
        BoundingBox->Centroid = FPSCamera.CameraPosition + (FPSCamera.Front * D.Length());
        BoundingBox->Centroid.y -= 1.5f;

        BuggyFinalMatrix = BuggyTranslationMatrix * BuggyRotationMatrix;
        BoundingBox->FinalMatrix = BuggyFinalMatrix;
        BoundingBox->CalculateXYZ();
        FPSCamera.UpdateCameraView();
        BuggyFront = FPSCamera.Front;
        CannonFront = CannonRotationMatrix * BuggyFront;
        _Bullet->RotationMatrix = r * _Bullet->RotationMatrix;
        _Explosion->BBCentroid = BoundingBox->Centroid;
        _Explosion->BBCentroid.y = 0.0f;
    }
    else if (GetKeyState(68) & 0x8000)
    {
        WheelRotation(true,true,-1);
        TurnAngle -= 0.5f;
        FPSCamera.YawAngle -=0.5f;
                
        GameMath::Vector3 Position = GameMath::Vector3(BuggyTranslationMatrix.Matrix[3][0], BuggyTranslationMatrix.Matrix[3][1], BuggyTranslationMatrix.Matrix[3][2]);
        GameMath::Matrix4x4 RM;
        GameMath::Vector3 D = FPSCamera.CameraPosition - Position;
        GameMath::Vector3 pivot = D.Normalize() * D.Length();
        GameMath::Matrix4x4 r = GameMath::Transform::RotateQ('Y', TurnAngle);
        GameMath::Matrix4x4 n = GameMath::Transform::Translate(-pivot);
        GameMath::Matrix4x4 p = GameMath::Transform::Translate(pivot);
        BuggyRotationMatrix = p * r * n;
        BoundingBox->Centroid = FPSCamera.CameraPosition + (FPSCamera.Front * D.Length());
        BoundingBox->Centroid.y -= 1.5f;

        BuggyFinalMatrix = BuggyTranslationMatrix * BuggyRotationMatrix;
        BoundingBox->FinalMatrix = BuggyFinalMatrix;
        BoundingBox->CalculateXYZ();
        FPSCamera.UpdateCameraView();
        BuggyFront = FPSCamera.Front;
        CannonFront = CannonRotationMatrix * BuggyFront;
        _Bullet->RotationMatrix = r * _Bullet->RotationMatrix;
        _Explosion->BBCentroid = BoundingBox->Centroid;
        _Explosion->BBCentroid.y = 0.0f;
    }
   
}

void GameObject::PlayerControlledAttack(GameCamera::FPS& FPSCamera)
{
    float dummy;
    FPSCamera.MouseTracking(CannonRotationAngle_Buffer, dummy,false);
    GameMath::Matrix4x4 pTM = GameMath::Transform::Translate(Armature.Bones["MachineGun"]->Head);
    GameMath::Matrix4x4 nTM = GameMath::Transform::Translate(-Armature.Bones["MachineGun"]->Head);
    CannonRotationAngle = CannonRotationAngle_Buffer - LastCannonRotationAngle;
    LastCannonRotationAngle = CannonRotationAngle_Buffer;
   
    if (CannonRotationAngle != LastCannonRotationAngle)
    {
        GameMath::Matrix4x4 Rot = GameMath::Transform::RotateQ(YAxis, CannonRotationAngle);
        CannonRotationMatrix = Rot * CannonRotationMatrix;
        CannonFront = Rot * CannonFront;
        SetBoneMatrix("MachineGun", pTM * CannonRotationMatrix * nTM);
        _Bullet->CannonFront = CannonFront;
        _Bullet->TranslationMatrix = GameMath::Matrix4x4();
        _Bullet->RotationMatrix = CannonRotationMatrix;
        _Bullet->CannonPosition = BoundingBox->Centroid;
    }

    //GameMath::Vector3 Roller = (Armature.Bones["MachineGun"]->Tail - Armature.Bones["MachineGun"]->Head).Normalize();
    //std::cout << Roller;

    if (GameWindow::MouseLButtonPressed)
        CannonFiring(10.0f);
}

void GameObject::PlayerControlledCannonFiring()
{
    _Bullet->TranslationMatrix = GameMath::Transform::Translate(BoundingBox->Centroid + (_Bullet->CannonFront * 1.0f));
}

/*************************************************
THIS FUNCTION IS USED BY BOTH THE A.I OBJECTS AND THE FRIENDLY OBJECTS
BEFORE THIS FUNCTION IS CALLED THE OBJECT'S "ATTACKOBJECT" VARIABLE IS SET TO ANOTHER OBJECT
THAT THIS OBJECT NEEDS TO ATTACK. EACH OBJECT HAS AN INDEX THAT UNIQUELY IDENTIFIES IT.
THE PROCEDURE FOLLOWED IS:

1) CHECK IF THE "ATTACKOBJECT" ISALIVE. IF NOT THEN RETURN
2) CHECK IF THIS OBJECT'S "HALT" VARIABLE IS SET TO TRUE. IF YES THEN
   A) SET THE "HALT" VARIABLE TO FALSE
   B) SET THE ATTACKOBJECT" VARIABLE TO NULLPTR
   C) RETURN
3) CHECK IF THIS OBJECT'S "WEAPONQUANITY" IS LESS THAN OR EQUAL TO ZERO
   IF YES THEN SET "ATTACKOBJECT' TO NULLPTR AND RETURN SINCE AN OBJECT WITH NO WEAPONS
   CANNOT ATTACK ANOTHER OBJECT
4) GET THE DISTANCE BETWEEN THIS OBJECT AND THE ATTACK OBJECT
   A) IF THAT DISTANCE IS GREATER THAN THIS OBJECT'S WEAPON RANGE THEN SET THE ATTACK OBJECT TO NULLPTR AND RETURN
   B) ELSE GET THE DIRECTION FROM THIS OBJECT TO THE ATTACK OBJECT AND NORMALIZE IT
5) DO A COLLISION TEST WITH ALL THE OTHER MOVABLE / BUGGY OBJECTS
   A) ITERATE THROUGH A LIST OF ALL MOVABLE OBJECTS
   B) IF THE OBJECT IN THE LIST IS THE ATTACK OBJECT OR THIS OBJECT ITSELF THEN SKIP THE OBJECT IN THE LIST
   C) ELSE GET THE DISTANCE BETWEEN THIS OBJECT AND THE OBJECT IN THE LIST
      IF THAT DISTANCE IS LESS THAN THE DISTANCE BETWEEN THIS OBJECT AND THE ATTACH OBJECT THEN
      CHECK IF THE OBJECT IN THE LIST IS IN FRONT OF THE ATTACK OBJECT BY DOING A COLLISION TEST
      i) TRANSLATE THIS OBJECT'S BOUNDING BOX'S CENTROID IN THE DIRECTION OF THE ATTACK OBJECT
         BY THE DISTANCE BETWEEN THIS OBJECT AND THE OBJECT IN THE LIST. LETS CALL THIS POINT [A]
      ii) DO A COLLISION TEST BETWEEN THAT POINT [A] ABOVE AND THE BOUNDING BOX OF THE OBJECT IN THE LIST
      iii) IF A COLLISION IS FOUND THEN SET THE ATTACKOBJECT TO NULLPTR AND RETURN
6) DO A COLLISION TEST WITH ALL INANIMATE OBJECTS LIKE THE ROCK OBJECTS BY FOLLOWING
   THE SAME PRODECURE MENTIONED ABOVE WITH THE DIFFERENCE OF ITERATING THROUGH A LIST OF ALL THE
   INANIMATE OBJECTS
7) IF THE FUNCTION HAS NOT RETURNED AT THIS POINT THEN
   A) LOWER THE HEALTH OF THE ATTACK OBJECT BY THE VALUE SET IN "WEAPONDAMAGERATE" VARIABLE OF THE CALLING OBJECT
      TO INDICATE THAT THE ATTACK OBJECT HAS BEEN DAMAGED
   B) LOWER THE WEAPON QUANTIY OF THE CALLING OBJECT BY THE VALUE SET IN "WEAPONCONSUMPTIONRATE" OF THE CALLING OBJECT
      TO INDICATE THAT THE WEAPON HAS BEEN USED DURING THIS ATTACH PROCESS
   C) MOVE THE CANNON OF CALLING OBJECT TOWARDS THE ATTACK OBJECT
   D) SIMULATE BULLETS FIRING TOWARDS THE ATTACK OBJECT
**************************************************/
void GameObject::Attack()
{
    if (AttackObject == nullptr)
        return;

    if (HALT == true)
    {
        AttackObject = nullptr;
        HALT = false;
        return;
    }

    if (WeaponQuantity <= 0.0f)
    {
        AttackObject = nullptr;
        return;
    }

    GameMath::Vector3 AttackDirection = (AttackObject->BoundingBox->Centroid - BoundingBox->Centroid);
    float AttackDistance = AttackDirection.Length();
    if (ObjectType == 1 && AttackDistance > GameSettings::PlayerWeaponRange)
    {
        AttackObject = nullptr;
        return;
    }
    else if (ObjectType == 2 && AttackDistance > GameSettings::EnemyWeaponRange)
    {
        AttackObject = nullptr;
        return;
    }
    else
        AttackDirection = AttackDirection.Normalize();

    // COLLISION TEST WITH ITS OWN TEAM SO BUGGY DOES NOT ATTACK ITS TEAM PLAYER.MEANT TO BE USED FOR AIs ONLY
    float SampleDistance;
    GameMath::Vector3 SamplePoint;
    if (ObjectType == 2)
    {
        for (GameObject* Enemy : GameObject::Enemies)
        {
            if (Enemy->Index == Index || Enemy->isAlive == false)
                continue;

            SampleDistance = (Enemy->BoundingBox->Centroid - BoundingBox->Centroid).Length();
            if (SampleDistance < AttackDistance)
            {
                SamplePoint = BoundingBox->Centroid + (AttackDirection * SampleDistance);
                if (CollisionDetection(&SamplePoint, Enemy->BoundingBox) == true)
                {
                    AttackObject = nullptr;
                    return;
                }
            }
        }
    }
    //COLLISION TEST WITH INANIMATE OBJECTS
    for (GameObject* Inanimate : GameObject::Inanimates)
    {
        SampleDistance = (Inanimate->BoundingBox->Centroid - BoundingBox->Centroid).Length();
        if (SampleDistance < AttackDistance)
        {
            SamplePoint = BoundingBox->Centroid + (AttackDirection * SampleDistance);
            if (CollisionDetection(&SamplePoint, Inanimate->BoundingBox) == true)
            {
                AttackObject = nullptr;
                return;
            }
        }
    }

    //GET THE CANNON OF THIS OBJECT TO POINT TOWARDS THE ATTACK OBJECT
    CannonMovement(AttackDirection);

    //INITIATE FIRING OF BULLETS FROM THIS OBJECT TOWARDS THE ATTACK OBJECT
    CannonFiring(AttackDistance);

    //UPDATE HEALTH OF THE OBJECT BEING ATTACKED AND 
    //UPDATE WEAPON QUANTITY OF THIS OBJECT THAT IS ATTACKING THE ATTACK OBJECT
    if (ObjectType == 1)
    {
        AttackObject->Health -= GameSettings::PlayerWeaponDamageRate;
        WeaponQuantity -= GameSettings::PlayerWeaponConsumptionRate;
    }
    else if (ObjectType == 2)
    {
        AttackObject->Health -= GameSettings::EnemyWeaponDamageRate;
        WeaponQuantity -= GameSettings::EnemyWeaponConsumptionRate;
    }

    //CHECK IF THE ENEMY HAS BEEN DESTROYED
    if (AttackObject->Health <= 0.0f)
    {
        if (AttackObject != nullptr)
            AttackObject->Destroy();
        AttackObject = nullptr;
    }
}

/*************************************************
THIS FUNCTIONS APPLIES TO ALL MOVABLE OBJECTS. THE ENTIRE WORLD / FLAT PLANE THAT THE OBJECT
CAN RESIDE IN IS DIVIDED INTO EQUAL SIZE SQUARES OR CUBE. EACH WORLD CUBE HAS AN INDEX ASSIGNED TO IT
THAT UNIQUELY INDETIFIES IT

ALL OBJECTS HAVE THE FOLLOWING WORLD CUBE VARIABLES ASSIGNED TO THEM
1) SOURCE WORLD CUBE - THIS IS THE WORLD CUBE THAT THE OBJECT CURRENTLY RESIDES IN
2) DESTINATION WORLD CUBE - THIS IS THE WORLD CUBE THAT THE OBJECT NEEDS TO MOVE TOWARDS OR OCCUPY
3) NEIGHBOURS WORLD CUBE - THERE ARE 8 OF THESE NEIGHBOURING WORLD CUBES THAT SURROUNDS EVERY OTHER WORLD CUBE
4) VISITED WORLD CUBES - THIS IS A COLLECTION OF WORLD CUBES THAT THE OBJECT HAS VISITED DURING THIS JOURNEY FROM
                         ITS SOURCE WORLD CUBE TO ITS DESTINATION WORLD CUBE. THIS COLLECTION IS MAINTAINED
                         SO THAT THE OBJECT DOES NOT END UP GOING BACK AND FORTH BETWEEN IT NEIGHBOURING WORLD CUBES.
                         WHEN THE OBJECT REACHES IT DESTINATION THEN THIS COLLECTION IS CLEARED

THE PROCEDURE FOLLOWED HERE IS AS FOLLOWS:
1) CHECK IF THE DESTINATION WORLD CUBE FOR THIS OBJECT IS SET TO NULLPTR. IF YES THEN RETURN
2) CHECK IF THE SOURCE WORLD CUBE FOR THIS OBJECT IS THE SAME AS THE DESTINATION WORLD CUBE
   IF YES THEN THAT INDICATES THAT THE OBJECT HAS REACHED ITS DESTINATION AND THE VISITED WORLD CUBES COLLECTION FOR THIS
   OBJECT IS CLEARED
3) ITERATE THROUGH ALL THE 8 NEIGHBOURING WORLD CUBES OF THE OBJECT
   A) CHECK IF THE CURRENT NEIGHBOUR WORLD CUBE EXISTS. IF YES THEN CONTINUE ELSE ITERATE TO NEXT NEIGHBOUR WORLD CUBE
   B) CHECK IF THE CURRENT NEIGHBOUR WORLD CUBE HAS BEEN VISITED BEFORE. IF YES THEN SKIP THIS WORLD CUBE
      AND ITERATE TO NEXT NEIGHBOUR WORLD CUBE.
   C) CHECK IF THE CURRENT NEIGHBOUR WORLD CUBE IS OCCUPIED. IF YES THEN ADD THIS WORLD CUBE TO THE LIST OF VISITED WORLD CUBES
      AND ITERATE TO NEXT NEIGHBOUR WORLD CUBE.
      IF NO THEN SET THE DISTANCE BETWEEN THIS NEIGHBOUR WORLD CUBE TO THE DESTINATION WORLD CUBE AS THE "LEAST DISTANCE"
      AND THIS NEIGHBOUR WORLD CUBE AS THE CLOSEST NEIGHBOUR.
4) IF A CLOSEST NEIGHBOUR IS NOT FOUND THEN SET THE DESTINATION WORLD CUBE TO NULLPTR AND CLEAR THE VISITED WORLD CUBE COLLECTION
   FOR THIS OBJECT AND RETURN
5) IF THE CLOSEST NEIGHBOUR TO MOVE TO IS FOUND THEN DO THE FOLLOWING:
   A) CALCULATE THE DISTANCE BETWEEN THE OBJECT'S BOUNDING BOX AND THE DESTINATION WORLD CUBE
   B) CALCULATE THE DIRECTION FROM THE OBJECT'S BOUNDING BOX TO THE DESTINATION WORLD CUBE, CALL IT ACTUAL DISTANCE
   C) CALCULATE THE DISTANCE IN FLOATING VALUE THAT THE OBJECT WILL TRAVEL IN ONE FRAME, CALL IT FRAME DISTANCE
   D) CHECK IF THE CLOSEST NEIGHBOUR CONTAINS A HEALTH CUBE OR A WEAPON CUBE OBJECT IF YES THEN
      UPDATE THE HEALTH AND THE WEAPON OF THE OBJECT ACCORDINGLY AND SET THE HEALTH AND THE WEAPON OBJECTS TO NULLPTR
      INDICATING THAT THEY HAVE BEEN USED UP
   E) IF THE ACTUAL DISTANCE IS LESS THAN THE FRAME DISTANCE THEN THAT INDICATES THAT THE OBJECT HAS REACHED THE CLOSEST NEIGHBOUR
      i) HERE WE SET THE CURRENT SOURCE WORLD CUBE TO NULLPTR INDICATING THAT ITS EMPTY NOW
      ii) WE SET THE CLOSEST NEIGHBOUR'S OBJECT TO THIS OBJECT INDICATING THAT THIS WORLD CUBE HAS NOW BEEN
          OCCUPIED BY THIS OBJECT
      iii) WE SET THE OBJECT'S SOURCE WORLD CUBE TO THIS CLOSEST NEIGHBOUR WORLD CUBE INDICATING THAT THIS OBJECT
           IS CURRENTLY OCCUPYING THIS WORLD CUBE
      iv) SET THE BUGGY MOVEMENT TOWARDS THIS CLOSEST NEIGHBOUR CUBE
      v) IF THE "HALT" FLAG OF THE OBJECT HAS BEEN RAISED THEN SET THE DESTINATION WORLD CUBE TO NULLPTR AND CLEAR THE COLLECTION
         OF VISITED WORLD CUBES INDICATING THAT THE OBJECT WILL STOP ITS PURSUIT OF THE DESTINATION WORLD CUBE
   F) IF THE FRAME DISTANCE IS LESS THAN THE ACTUAL DISTANCE THEN
      GET THE BUGGY TO MOVE TOWARDS THAT CLOSEST NEIGHBOUR BY THE FLOATING POINT DISTANCE CALCULATED ABOVE
      THIS INDICATES THAT THE CLOSEST NEIGHBOUR IS FAR AWAY AND THE OBJECT IS IN THE MIDDLE OF REACHING THE CLOSEST NEIGHBOUR
*************************************************/
void GameObject::WorldMovement(int Time_Per_Frame)
{
//DESTINATION FOR THE OBJECT HAS NOT BEEN SET YET
if (DestinationWorldCube == nullptr || SourceWorldCube == nullptr)
{
    return;
}
//IN FOLLOWING CASES STOP THE PURSUIT
//  THE OBJECT HAS REACHED THE DESTINATION WHICH IS WHY THE SOURCEWORLDCUBE IS THE SAME AS THE DESTINATIONWORLDCUBE 
//  THE DESTINATION GOT OCCUPIED BY SOMEONE ELSE 
//  A HALT FLAG HAS BEEN RASIED 
else if (SourceWorldCube->Index == DestinationWorldCube->Index || DestinationWorldCube->Object != nullptr || HALT == true)
{
    DestinationWorldCube = nullptr;
    HALT = (HALT ? false : false);//RESET THE HALT FLAG
    return;
}
else
{
    std::vector<int>::iterator WorldCubeFinder;
    GameWorldCube* ClosestNeighbour = GameObject::FindClosestEmptyNeighbour(SourceWorldCube, DestinationWorldCube);
    //IF NO NEIGHTBOUR TO MOVE TO IS FOUND THEN THE OBJECT IS STUCK
    //SET DESTINATION CUBE TO NULLPTR AND RESET PURSUIT
    if(ClosestNeighbour==nullptr)
    {
        DestinationWorldCube=nullptr;
        return;
    }
    else if(ClosestNeighbour!=nullptr)
    {
        //HERE WE NEED TO FIND THE DISTANCE IN FLOATING VALUE THE OBJECT NEEDS TO TRAVEL IN THE NEXT ONE FRAME
	    //
	    //BASED ON THE OBJECT'S SPEED IN METER PER SECOND AND THE CURRENT FRAME RATE
	    //1) COVERT THE SPEED OF THE OBJECT FROM METER PER SECOND TO CENTIMETER PER SECOND BY MULTIPYING IT WITH 100.0
	    //2) ON THE SCREEN USING A RULER WE FOUND OUT THAT A FLOATING VALUE OF 0.16 EQUALS TO 1 CM OF MOVEMENT ON THE SCREEN
	    //3) MULTIPLYING THE CURRENT SPEED IN CENTIMETE PER SECOND WITH 0.16 GIVES US THE SPEED IN FLOATING VALUE PER SECOND
	    //   HERE THE SPEED CONSTANT = 100 * 0.16 WHICH IS WHEN MULTIPLIED WITH SPEED IN METERS PER SECOND
        //   GIVES OUR SPEED IN FLOATING POINT PER SECOND AND THEN DIVIDING BY 1000 GIVES US THE SPEED IN
	    //   FLOATING VALUE PER MILISECOND
	    //4) DISTANCE = SPEED X TIME
	    //            = FLOATING VALUE PER SECOND X TIME TAKEN BY ONE FRAME IN MILISECONDS
        //            = DISTANCE NEEDED TO BE TRAVELLED IN ONE FRAME IN FLOATING VALUES
        //
        //   WARNING !! : MIGHT CAUSE AN ERROR.
        //   WHEN A SOURCE AND DESTINATION ARE SET THEN DURING THE MOVEMENT FROM SOURCE TO DESTINATION, THE OBJECT ONLY 
        //   CHANGES/UPDATES ITS SOURCE WORLDCUBE TO THE NEXT NEIGHTBOURING CUBE WHEN THE DISTANCE BETWEEN THEM IS ZERO(FLOAT)
        //   HOWEVER IF A BUGGY MOVEMENT HAPPENS WITH THAT ZERO DISTANCE THEN THAT WILL CAUSE THE DIRECTION TO BE NAN ALONG 
        //   WITH THE OBJECT BUGGYMATRIX WHICH WILL CAUSE THE OBJECT TO DISAPPEAR. ALWAYS MAKE SURE THAT THE ACTUAL DISTANCE 
        //   IS NOT ZERO BEFORE DOING A BUGGY MOVEMENT ON IT
        float FrameDistance=(Speed * SpeedConstant) * Time_Per_Frame;
        GameMath::Vector3 Direction;
        float ActualDistance = 0.0f;
        Direction.x=ClosestNeighbour->Centroid.x - BoundingBox->Centroid.x;
        Direction.z=ClosestNeighbour->Centroid.z - BoundingBox->Centroid.z;
        Direction.y=0.0f;
        ActualDistance=Direction.Length();
        //THIS INDICATES THAT THE OBJECT HAS REACHED THE CLOSEST NEIGHBOUR AND
        //WILL LEAVE ITS CURRENT SOURCE WORLD CUBE AND OCCUPY THE CLOSEST NEIGHBOUR WORLD CUBE
        if(ActualDistance < FrameDistance)
        {
            if (ActualDistance > 0.000000000000000f)
            {
                Direction = Direction.Normalize();
                BuggyMovement(&Direction, ActualDistance);
            }
            SourceWorldCube->Object=nullptr;
            ClosestNeighbour->Object=this;
            SourceWorldCube=ClosestNeighbour;
            //IF THE CLOSEST NEIGHBOUR CONTAINS A HEALTH OBJECT THEN UPDATE THE HEALTH OF THE
            //OBJECT ACCORDINGLY
            //IF THE CLOSEST NEIGHBOUR CONTAINS A WEAPON OBJECT THEN UPDATE THE WEAPON OF THE
            //OBJECT ACCORDINGLY
            if(ClosestNeighbour->HealthObject!=nullptr)
	        {
                if (ObjectType == 1)
                {
                    Health += GameSettings::PlayerHealthRestorationRate;
                    if (Health > 100.0f)
                        Health = 100.0f;
                }
                else if (ObjectType == 2)
                {
                    Health += GameSettings::EnemyHealthRestorationRate;
                    if (Health > 100.0f)
                        Health = 100.0f;
                }
                ClosestNeighbour->HealthObject->isAlive=false;
                ClosestNeighbour->HealthObject = nullptr;
	        }
            else if(ClosestNeighbour->WeaponObject!=nullptr)
	        {
                if (ObjectType == 1)
                {
                    WeaponQuantity += GameSettings::PlayerWeaponRestorationRate;
                    if (WeaponQuantity > 100.0f)
                        WeaponQuantity = 100.0f;
                }
                else if (ObjectType == 2)
                {
                    WeaponQuantity += GameSettings::EnemyWeaponRestorationRate;
                    if (WeaponQuantity > 100.0f)
                        WeaponQuantity = 100.0f;
                }
                ClosestNeighbour->WeaponObject->isAlive=false;
	            ClosestNeighbour->WeaponObject=nullptr;
	        }
        }
        else
        {
            Direction = Direction.Normalize();
            BuggyMovement(&Direction, FrameDistance);
        }
    }
}

}

/*************************************************
THIS FUNCTION MOVES THE BUGGY BY THE DISTANCE SPECIFIED IN THE DIRECTION SPECIFIED
AND IT ALSO ROTATES THE WHEELS AROUND THEIR CENTER GIVING THE ILLUSION OF WHEELS MOVING

GENETAL PROCEDURE INVOLVED IS AS FOLLOWS:
1) CHECK IF THE OBJECT HAS BONES AS INANIMATE OBJECTS DO NOT HAVE BONE SO THIS PROCEDURE WILL NOT APPLY TO THEM
2) CHECK IF THE DIRECTION PROVIDED IS NOT A NULLPTR OTHERWISE THE BUGGY WONT MOVE

THE PROCEDURE INVOLVED IN ROTATING THE WHEELS IS AS FOLLOWS:
1) IF THE OBJECT IS A BUGGY OR A MOVABLE OBJECT AND THE DIRECTION IS VALID THEN PROCEED FURTHER:
2) THE WHEEL BONES ARE NOT LOCATED AT THE ORGIN AND THE ROTATIONS THAT ARE NEEDED TO BE APPLIED TO THE WHEELS
   ARE LOCAL ROTATION ABOUT THE WORLD -Z AXIS SO IN ORDER TO GET THAT ROTATION TAKE EFFECT WE:
   i) TRANSLATE THE WHEEL BONE TO THE ORIGIN
   ii) APPLY THE ROTATION ABOUT THE -Z AXIS TO THE WHEEL BONE
   iii) TRANSLATE THE WHEEL BONE BACK TO ITS ORGINAL WORLD LOCATION
   iv) APPLY THE MATRIX : TRANSLATION(WHEEL BONE HEAD) * ROTATION(-Z AXIS) * TRANSLATION(-WHEEL BONE HEAD)
       TO THE WHEEL BONE SO THAT TRANSFORMATION CAN BE APPLIED TO THE WHEEL VERTICES THAT ARE LINKED TO THE
       WHEEL BONE
   v) REPEAT THE PROCESS FOR BOTH THE FRONT WHEEL AND BACK WHEEL BONES

THE PROCEDURE INVOLVED IN MOVING THE BUGGY IS AS FOLLOWS:
1) CREATE A TRANSLATION MATRIX THAT TRANSLATE IN THE DIRECTION PROVIDED BY THE DISTANCE PROVIDED
2) MULTIPLY THE ABOVE MATRIX WITH THE EXISTING TRANSLATION MATRIX FOR BUGGY
3) MOVE THE BUGGY'S BOUNDING BOX CENTROID TO THE NEW LOCATION THAT THE BUGGY IS TRANSLATING TO
   BY GETTING THE X, Y AND Z FROM THE LAST COLUMN OF THE NEW TRANSLATION MATRIX WHERE
   MATRIX[3][0] = X COORDINATE
   MATRIX[3][1] = Y COORDINATE
   MATRIX[3][2] = Z COORDINATE

THE PROCEDURE INVOLVED IN ROTATING THE BUGGY IS AS FOLLOWS:
1) USE THE ATAN2 FUNCTION TO GET THE ANGLE THAT THE TARGET DIRECTION IS MAKING WITH THE COORDINATE AXIS
2) USE THE ATAN2 FUNCTION TO GET THE ANGLE THAT THE "BUGGY FRONT" DIRECTION IS MAKING WITH THE COORDINATE AXIS
3) GET THE ABSOLUTE VALUE OF THE DIFFERENCE BETWEEN THE TWO ANGLES. IF THAT DIFFERENCE IS GREATER THAN 0.1
   THEN ROTATE THE BUGGY BY FOLLOWING THIS PROCEDURE:
   i) CONVERT THE DIFFERENCE BETWEEN THE TWO ANGLES FROM RADIAN TO DEGREE
   ii) GET 30 % OF THAT ANGLE IN DEGREES
   iii) USE THE 30 % VALUE TO CREATE A ROTATION MATRIX ABOUT THE WORLD Y AXIS
   iv) MULTPLIY THAT ROTATION MATRIX WITH "BUGGY FRONT" DIRECTION VECTOR FOR THE DIRECTION THE BUGGY IS POINTING AT
       MULTPLIY THAT ROTATION MATRIX WITH "CANNON FRONT" DIRECTION VECTOR FOR THE DIRECTION THE CANNON IS POINTING AT
       MULTPLIY THAT ROTATION MATRIX WITH EXISTING BUGGY'S ROTATION MATRIX TO UPDATED THIS ROTATION MATRIX
       MULTIPLY THAT ROTATION MATRIX WITH EXISTING BULLET ROTATION MATRIX TO UPDATED THIS ROTATION MATRIX
       COPY "CANNON FRONT" DIRECTION VECTOR TO THE BULLET OBJECT CANNON FRONT DIRECTION VECTOR

4) MULTIPLY THE BUGGY TRANSLATION MATRIX AND ROTATION MATRIX TO CREATE THE BUGGY FINAL MATRIX
5) UPDATE THE BULLET OBJECT'S CANNON POSITION VECTOR BY MULTIPLYING THE HEAD VECTOR OF THE MACHINE BUN BONE WITH THE BUGGY FINAL MATRIX
   THE CANNON POSITION VECTOR IS THE WORLD COORDINATE FOR THE MACHINE GUN BONE HEAD VECTOR
   THIS IS THE POSITION VECTOR FROM WHICH BULLETS WILL BE FIRED FROM / TRANSLATING FROM TOWARDS THE ENEMY OBJECT
6) UPDATE THE EXPLOSION OBJECT'S BBCENTROID VECTOR WITH THE BUGGY'S CENTROID COORDINATES
   BBCENTROID VECTOR IS THE WORLD COORDINATE THAT REPRESENTS THE CENTER OF THE BUGGY OBJECT
   THIS IS THE POSITION VECTORS FROM WHICH OBJECT EXPLOSION WILL START


ATAN2 FUNCTION RETURNS THE PRINCIPAL VALUE OF THE ARC TANGENT OF Y/X, EXPRESSED IN RADIANS.
TO COMPUTE THE VALUE, THE FUNCTION TAKES INTO ACCOUNT THE SIGN OF BOTH ARGUMENTS IN ORDER TO DETERMINE THE QUADRANT.

ARGUMENT 1 - Y VALUE REPRESENTING THE PROPORTION OF THE Y-COORDINATE.
ARGUMENT 2 - X VALUE REPRESENTING THE PROPORTION OF THE X-COORDINATE.
RETURN VALUE - PRINCIPAL ARC TANGENT OF Y/X, IN THE INTERVAL [-PI,+PI] RADIANS.

				  (90 degree)
				   +y axis
				      *     * (45 degrees)
				      *   *
				      * *
          (180 degree)-x axis * * * * * * * * * +x axis (0 degree)
	  (-180 degree)-x axis        * *
				      *   *
				      *     * (-45 degrees)
				   -y axis
				  (-90 degree)
*************************************************/
void GameObject::BuggyMovement(GameMath::Vector3* Direction,float Distance)
{
if(Armature.Bones.size() > 0 && Direction!=nullptr)
{
    WheelRotation();
    BuggyTranslationMatrix=GameMath::Transform::Translate((*Direction) * Distance) * BuggyTranslationMatrix;
    BoundingBox->Centroid.x=BuggyTranslationMatrix.Matrix[3][0];
    BoundingBox->Centroid.y=BuggyTranslationMatrix.Matrix[3][1];
    BoundingBox->Centroid.z=BuggyTranslationMatrix.Matrix[3][2];
   
    float DiffInAngles=atan2(Direction->x,Direction->z) - atan2(BuggyFront.x,BuggyFront.z);
    if(abs(DiffInAngles) > 0.1f)
    {
	    BuggyRotationAngle=DiffInAngles * 180.0f/PI;
	    BuggyRotationAngle=(BuggyRotationAngle * 30.0f) /100.0f;
	    GameMath::Matrix4x4 Rot=GameMath::Transform::RotateQ(YAxis,BuggyRotationAngle);
	    BuggyFront=Rot * BuggyFront;
        CannonFront=Rot * CannonFront;
        BuggyRotationMatrix=Rot * BuggyRotationMatrix;
        _Bullet->CannonFront=CannonFront;
        _Bullet->RotationMatrix=Rot * _Bullet->RotationMatrix;
    }

    BuggyFinalMatrix=BuggyTranslationMatrix * BuggyRotationMatrix;
    BoundingBox->FinalMatrix = BuggyFinalMatrix;
    BoundingBox->CalculateXYZ();
    _Bullet->CannonPosition=BuggyFinalMatrix * Armature.Bones["MachineGun"]->Head;
    _Explosion->BBCentroid=BoundingBox->Centroid;
    _Explosion->BBCentroid.y=0.0f;
}
}

/*************************************************
THIS FUNCTION ROTATES THE MACHINE GUN TURRET ABOUT THE WORLD Y AXIS SO IT CAN POINT AT THE ENEMY OR THE TARGET OBJECT
THE CANNON OR MACHINE GUN TURRET ROTATES ALONG WITH THE BUGGY BUT IT ALSO ROTATES ON IT OWN AS WELL
SO WHEN THE BUGGY IS STATIONARY THEN THE CANNON CAN STILL POINT AT ENEMY OBJECTS AND SHOOT AT THEM SO
CANNON ROTATION WILL FOLLOW THE FOLLOWING ROTATION ORDER:

ROTATION = CANNON LOCATION ROTATION  * BUGGY ROTATIONS

WHENEVER THE BUGGY ROTATES, THE CANNON FRONT AND BUGGY FRONT ARE BOTH ROTATED
LATER THIS CANNON FRONT DIRECTION VECTOR IS USED TO COMPUTE FURTHER ROTATION THAT THE CANNON MUST
GO THROUGH TO POINT AT THE ENEMY OBJECT


THE PROCEDURE INVOLVED IN ROTATING THE MACHINE GUN TURRET IS AS FOLLOWS:
1) USE THE ATAN2 FUNCTION TO GET THE ANGLE THAT THE TARGET DIRECTION IS MAKING WITH THE COORDINATE AXIS
2) USE THE ATAN2 FUNCTION TO GET THE ANGLE THAT THE "CANNON FRONT" DIRECTION IS MAKING WITH THE COORDINATE AXIS
3) GET THE ABSOLUTE VALUE OF THE DIFFERENCE BETWEEN THE TWO ANGLES. IF THAT DIFFERENCE IS GREATER THAN 0.1
   THEN ROTATE THE MACHINE GUN TURRET BY FOLLOWING THIS PROCEDURE:
   i) CONVERT THE DIFFERENCE BETWEEN THE TWO ANGLES FROM RADIAN TO DEGREE
   ii) GET 30 % OF THAT ANGLE IN DEGREES
   iii) USE THE 30 % VALUE TO CREATE A ROTATION MATRIX ABOUT THE WORLD Y AXIS
   iv) MULTIPLY THAT ROTATION MATRIX WITH "CANNON FRONT" DIRECTION VECTOR FOR THE DIRECTION THE CANNON IS POINTING AT
	 COPY THE UPDATED CANNON FRONT DIRECTION VECTOR OVER TO THE BULLET OBJECT CANNOT FRONT DIRECTION
         THIS DIRECTION VECTOR WILL BE USED TO FIRE / TRANSLATE THE BULLET FROM
       MULTIPLY THAT CANNON ROTATION MATRIX WITH EXISTING CANNON ROTATION MATRIX TO CREATE THE UPDATED ROTATION MATRIX
         COPY THE UPDATED CANNON ROTATION MATRIX OVER TO THE BULLET OBJECT ROTATION MATRIX
         THIS ROTATION MATRIX WILL BE USED TO POINT THE BULLETS IN THE RIGHT DIRECTION
   v) MULTIPLY THE ROTATION MATRIX WITH THE FOLLWING TRANSLATION MATRICES:
      MATRIX = TRANSLATION MATRIX(MACHINE GUN HEAD) * ROTATION(Y AXIS) *  TRANSLATION MATRIX(-MACHINE GUN HEAD)
      THIS CONVERTS THE ROTATION INTO A LOCAL ROTATION INSTEAD OF A GLOBAL ROTATION
   vi) UPDATE THE FINAL MATRIX FOR THE BONE: "MACHINE GUN" SO THE VERTICES FOR THE MACHINEGUN MESH CAN HAVE THAT
       TRANSFORMATION APPLIED TO THEM
*************************************************/
void GameObject::CannonMovement(GameMath::Vector3 TargetDirection)
{
float DiffInAngles=atan2(TargetDirection.x,TargetDirection.z) - atan2(CannonFront.x,CannonFront.z);
if(abs(DiffInAngles) > 0.1f)
{
    GameMath::Matrix4x4 pTM=GameMath::Transform::Translate(Armature.Bones["MachineGun"]->Head);
    GameMath::Matrix4x4 nTM=GameMath::Transform::Translate(-Armature.Bones["MachineGun"]->Head);

    CannonRotationAngle=DiffInAngles * 180.0f/PI;
    CannonRotationAngle=(CannonRotationAngle * 30.0f) / 100.0f;
    GameMath::Matrix4x4 Rot=GameMath::Transform::RotateQ(YAxis,CannonRotationAngle);
    CannonFront=Rot * CannonFront;
    CannonRotationMatrix=Rot * CannonRotationMatrix;
    _Bullet->CannonFront=CannonFront;
    _Bullet->RotationMatrix=Rot * _Bullet->RotationMatrix;
    SetBoneMatrix("MachineGun",pTM  * CannonRotationMatrix * nTM);
}
}

/*************************************************
THIS FUNCTION ROTATES THE MACHINE GUN ABOUT THE WORLD X AXIS SO IT LOOKS LIKE AS IF IT ROTATING ABOUT ITS CENTER

THE PROCEDURE INVOLVED IN ROTATING THE MACHINE GUN IS AS FOLLOWS:
1) INCREMENT THE CANNON'S ROTATION ANGLE BY 30 DEGREES.IF ITS 360 DEGREE THEN RESET IT TO ZERO DEGREE
2) CREATE A ROTATION MATRIX USING THAT ANGLE FOR ROTATION ABOUT THE WORLD X AXIS
3) MULTIPLY THE ROTATION MATRIX BY THE FOLLOWING TRANSLATION MATRICES:
   MATRIX = TRANSLATION MATRIX(MACHINE GUN HEAD) * ROTATION(X AXIS) *  TRANSLATION MATRIX(-MACHINE GUN HEAD)
   THIS CONVERTS THE ROTATION INTO A LOCAL ROTATION INSTEAD OF A GLOBAL ROTATION
4) UPDATE THE FINAL MATRIX FOR THE BONE: "MACHINE GUN ROLLER" SO THE VERTICES FOR THE MACHINEGUNROLLER MESH
   CAN HAVE THAT TRANSFORMATION APPLIED TO THEM

THE PROCEDURE INVOLVED IN MOVING / TRANSLATING THE BULLER OBJECT FORWARD TOWARDS THE ENEMY OBJECT OR THE TARGET OBJECT
IS AS FOLLOWS:
1) INCREMENT THE "DISTANCERATE" VARIABLE BY 25.0, IF ITS 75.0 THEN RESET IT TO 0.0, ITS UPPER LIMIT IS SET TO 75 TO
   COMPENSATE FOR THE LENGTH OF THE BULLET OBJECT ITSELF
2) USE THE "DISTANCERATE" VARIABLE TO GET A PERCENTAGE(0 %, 25 % , 50 %, 75 %, 100 %) OF THE ATTACK DISTANCE
3) USE THIS DISTANCE VALUE TO TRANSLATE THE "BULLET" OBJECT IN THE DIRECTION THAT THE MACHINE GUN IS POINTING AT
   WHICH IS REFERRED TO BY THE "CANNON FRONT" DIRECTION VECTOR
4) SET THE "ISALIVE" VARIABLE OF THE BULLET OBJECT TO TRUE SO IT CAN BE RENDERED / DISPLAYED ON THE SCREEN
*************************************************/
void GameObject::CannonFiring(float AttackDistance)
{
if(CannonFireRotationAngle>360.0f)
  CannonFireRotationAngle=0.0f;
CannonFireRotationAngle += 30.0f;
CannonFireRotationAngle = (float)((int)CannonFireRotationAngle % 360);
GameMath::Matrix4x4 pTM=GameMath::Transform::Translate(Armature.Bones["MachineGun"]->Head);
GameMath::Matrix4x4 nTM=GameMath::Transform::Translate(-Armature.Bones["MachineGun"]->Head);
SetBoneMatrix("MachineGunRoller",pTM * GameMath::Transform::RotateQ(XAxis,CannonFireRotationAngle) * nTM);

if(_Bullet->DistanceRate >=75.0f)
_Bullet->DistanceRate=0.0f;
else
_Bullet->DistanceRate=_Bullet->DistanceRate + 25.0f;

_Bullet->TranslationMatrix=GameMath::Transform::Translate(_Bullet->CannonFront  * ((AttackDistance * _Bullet->DistanceRate)/100.0f));
_Bullet->isAlive=true;
}

/*************************************************
THIS FUNCTION UPDATES THE BUGGY'S FINAL MATRIX INTO THE MAIN VERTEX SHADER
ALONG WITH:
1) BUGGY OBJECT'S DIFFUSEMAP
2) BUGGY OBJECT'S SPECULARMAP
3) BUGGY OBJECT'S AMBIENTMAP
4) BUGGY OBJECT'S ARMATURE BONE START INDEX
5) BUGGY OBJECT'S ARMATURE BONE END INDEX
6) BUGGY OBJECT'S NO MATERIAL FLAG
*************************************************/
void GameObject::Draw()
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, DiffuseMap);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, SpecularMap);
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, AmbientMap);
glUniformMatrix4fv(4, 1, GL_FALSE,BuggyFinalMatrix.Addressof());
glUniform1i(7,BoneStartIndex);
glUniform1i(8,BoneEndIndex);
glUniform1i(11,NoMaterialFlag);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES,0,VAOCount);
}



