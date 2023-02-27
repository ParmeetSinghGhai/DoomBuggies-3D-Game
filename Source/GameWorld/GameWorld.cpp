#include<GameWorld/GameWorld.h>
int GameWorld::Index=0;
/*************************************************
THE WORLD IS BASICALLY A FLAT SURFACE THAT THE GAME OBJECTS RESIDE ON. THE WORLD REPRESENTATION IS SPLIT INTO TWO WAYS:
1) THE FOREGROUND REPRESENTATION THAT GETS RENDERED ON THE SCREEN WITH PROPER TEXTURES
2) THE BACKGROUND VERSION THAT IS RENDERED IN THE BACKGROUND SCREEN BUFFER AND ALLOWS USER TO INTERACT WITH THE GAME OBJECTS
AND THE GAMEWORLD

GAMEWORLD OBJECT IS DIVIDED EQUALLY INTO INDIVIDUAL GAMEWORLD CUBE OBJECTS. EACH GAMEWORLDCUBE HAS THE FOLLOWING IMPORTANT PROPERTIES:
1) 3D VECTOR CALLED CENTROID THAT CONTAINS THE WORLD COORDINATES FOR THE CENTER OF A GAMEWORLDCUBE
2) GAMEOBJECT CALLED "OBJECT" THAT REPRESENTS THE GAMEOBJECT THAT IS CURRENTLY OCCUPYING THIS GAMEWORLDCUBE
3) GAMEOBJECT CALLED "HEALTH OBJECT" THAT REPRESENTS THE HEALTH OBJECT THAT IS CURRENTLY OCCUPYING THIS GAMEWORLDCUBE
4) GAMEOBJECT CALLED "WEAPON OBJECT" THAT REPRESENTS THE WEAPON OBJECT THAT IS CURRENTLY OCCUPYING THIS GAMEWORLDCUBE
5) ARRAY OF 8 GAMEWORLDCUBES WHICH REPRESENT THE NEIGHBOURING WORLDCUBES OF THIS GAMEWORLDCUBE
6) FLOATING VALUES R, G AND B WHICH REPRESENT THE UNIQUE COLOR VALUE ASSIGNED TO THIS GAMEWORLDCUBE OBJECT
   BY THE PIXELMAPPING FUNCTION

   *******************
   * NWC * NWC * NWC *
   *******************
   * NWC * WC  * NWC *
   *******************
   * NWC * NWC * NWC *
   *******************
   WC = GAMEWORDLCUBE
   NWC= 8 NEIGHBOURING WORLD CUBE



THE GAMEWORLD OBJECT CONSTRUCTOR DOES THE FOLLOWING:
1) IT ACCEPTS 6 FLOATING VALUES AS ARGUMENTS NAMELY: MINX,MAXX,MINY,MAXY,MINZ,MAXZ
2) A XBOUND VARIABLE IS SET AS MAXX - MINX, WHICH GIVES US THE SIZE OF THE WORLD OBJECT IN TERMS OF THE
   X COORDINATES
3) A ZBOUND VARIABLE IS SET AS MAXZ - MINZ, WHICH GIVES US THE SIZE OF THE WORLD OBJECT IN TERMS OF THE
   Z COORDINATES

   E.G - IN THIS EXAMPLE WE ARE LOOKING AT THE WORLD OBJECT FROM THE Y AXIS

                           (0,0,-50)
                ******************************
                *   *   *   *  *             *
                *WC *WC * WC*WC*             *
     (-100,0,0) ******************************(100,0,0)
                *WC *  *(0,0,0)*             *
                * * * **       *             *
                *WC *WC*       *             *
                ******************************
                           (0,0,50)

            XBOUND = 100 -(-100) = 200
            ZBOUND = 50 -(-50) = 100
            WC= GAMEWORLDCUBE

4) SET UP A PIXEL OBJECT WITH THE COLOR (RGB) =(0,0,0) (STRING KEY="000000000") ASSIGNED TO IT
   THIS KEY IS RESERVED FOR THE WORLD BACKGROUND WHICH IS ANY OTHER SPACE BESIDES THE WORLD AND THE OBJECTS
   CONTAINED IN THAT WORLD WHICH IS WHY THIS PIXEL OBJECT IS ASSIGNED THE PAYLOAD OF NULLPTR
5) DIVIDE THE GAMEWORLD INTO EQUAL SIZE GAME WORLDCUBES DETERMINED BY THE VARIABLE "SmallestDivision"
   BY FOLLOWING THIS PROCEDURE:
   1) ITERATE FROM 0 TILL ZBOUND. WITH EACH ITERATION OF ZBOUND, ITERATE FROM 0 TILL XBOUND
   2) WITH EACH ITERATION OF XBOUND
      A) INCREMENT FRON MINX TO XBOUND BY THE VALUE OF "SmallestDivision"
      B) INCREMENT FRON MINZ TO ZBOUND BY THE VALUE OF "SmallestDivision"
         E.G - MINX=-100
               MINZ=-50
               SmallestDivision=2

               ITERATION 1 OVER XBOUND : MINX + 2 = (-100) + 2 = -98
               ITERATION 2 OVER XBOUND : MINX + 2 = (-88) + 2 = -96
	       ITERATION 3 OVER XBOUND : MINX + 2 = (-86) + 2 = -94

               ITERATION 1 OVER ZBOUND : MINZ + 2 = (-50) + 2 = -48
               ITERATION 2 OVER ZBOUND : MINZ + 2 = (-48) + 2 = -46
	       ITERATION 3 OVER ZBOUND : MINZ + 2 = (-46) + 2 = -44
   3) USE THE VALUES OF THE X AND Z OBTAINED FROM THE TWO LOOPS ABOVE TO CREATE A NEW GAMEWORLDCUBE
      i) THE GAMEWORLDCUBE'S CENTROID X COMPONENT = (MINX + X) + (SMALLESTDIVISION / 2.0)
      ii) THE GAMEWORLDCUBE'S CENTROID Z COMPONENT = (MINZ + Z) + (SMALLESTDIVISION / 2.0)
      iii) THE GAMEWORLDCUBE'S CENTROID Y COMPONENT REMAINS THE SAME AS THE WORLD OBJECT'S MAXY
          E.G - MINX=-100
                MINZ=-50
                SmallestDivision=2

                1ST GAMEWORLDCUBE CENTROID'S COORDINATES:
                X =0 , Z =0
                (MINX + X) + (SMALLESTDIVISION / 2.0) = (-100 + 0) + (2 / 2)= -99
                (MINZ + Z) + (SMALLESTDIVISION / 2.0) = (-50 + 0) + (2 / 2)= -49

                2ND GAMEWORLDCUBE CENTROID'S COORDINATES:
                X =2 , Z =0
                (MINX + X) + (SMALLESTDIVISION / 2.0) = (-100 + 2) + (2 / 2)= -97
                (MINZ + Z) + (SMALLESTDIVISION / 2.0) = (-50 + 0) + (2 / 2)= -49
      iv) ADD THE NEWLY CREATED GAMEWORLDCUBE TO THE "WorldCoordMap" MAP USING THE Z AND X VALUES AS KEYS
      v) USE THE PIXELMAPPING FUNCTION TO ASSIGN TO A NEW PIXEL OBJECT WITH A UNIQUE COLOR,THIS GAMEWORLDCUBE OBJECT

6) AFTER ALL THE GAMEWORLDCUBES ARE CREATED, THEN ITERATE THROUGH ALL THE GAMEWORLDCUBES
   TO ADD REFERENCES TO NEIGHBOURING GAMEWORLDCUBES FOR EACH OF THE GAMEWORLDCUBES

		   MINZ
		    |
		    |
	   *******************
	   * BL  *  L  *  FL *
	   *******************
   MINX----* B   *  C  *  F  *----MAXX
	   *******************
	   * BR  *  R  *  FR *
	   *******************
                    |
                    |
                   MAXZ

   C = CENTER GAMEWORLDCUBE
   B = BACK GAMEWORLDCUBE
   F = FRONT GAMEWORLDCUBE
   L = LEFT GAMEWORLDCUBE
   R = RIGHT GAMEWORLDCUBE
   BL = BACK LEFT GAMEWORLDCUBE
   BR = BACK RIGHT GAMEWORLDCUBE
   FL = FRONT LEFT GAMEWORLDCUBE
   FR = FRONT RIGHT GAMEWORLDCUBE

   IF CENTER GAMEWORLDCUBE IS KEYED IN THE "WorldCoordMap" MAP AT KEY X AND Z THEN
   BACK GAMEWORLDCUBE WILL BE LOCATED AT X-2,Z, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE LEFT EDGE OF THE WORLD OBJECT
   FRONT GAMEWORLDCUBE WILL BE LOCATED AT X+2,Z, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE RIGHT EDGE OF THE WORLD OBJECT
   LEFT GAMEWORLDCUBE WILL BE LOCATED AT X,Z-2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE TOP EDGE OF THE WORLD OBJECT
   RIGHT GAMEWORLDCUBE WILL BE LOCATED AT X,Z+2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE BOTTOM EDGE OF THE WORLD OBJECT
   BACKLEFT GAMEWORLDCUBE WILL BE LOCATED AT X-2,Z-2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE LEFT OR TOP EDGE OF THE WORLD OBJECT
   BACKRIGHT GAMEWORLDCUBE WILL BE LOCATED AT X-2,Z+2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE LEFT OR BOTTOM EDGE OF THE WORLD OBJECT
   FRONTLEFT GAMEWORLDCUBE WILL BE LOCATED AT X+2,Z-2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE RIGHT OR TOP EDGE OF THE WORLD OBJECT
   FRONTRIGHT GAMEWORLDCUBE WILL BE LOCATED AT X+2,Z+2, UNLESS THE CENTER GAMEWORLD CUBE IS AT THE RIGHT OR BOTTOM EDGE OF THE WORLD OBJECT

7) COLLECT ALL THE GAMEWORLD CUBE DATA IN ONE ARRAY AND LOAD IT IN GPU MEMORY
8) THE DRAW FUNCTION DRAWS ALL THE GAMEWORLDCUBES IN ONE GO USING THE UNIQUE COLOR ASSIGNED TO EACH CUBE DURING
   THE PIXEL MAPPING FUNCTION IN THE BACKGROUND FRAME BUFFER USING THE HELPER SHADER
*************************************************/
GameWorld::GameWorld(float minx,float maxx,float miny,float maxy,float minz,float maxz)
{
this->minx=minx;
this->maxx=maxx;
this->miny=miny;
this->maxy=maxy;
this->minz=minz;
this->maxz=maxz;
xbound=(int)abs(maxx - minx);
zbound=(int)abs(maxz - minz);

//KEY "000000000" IS RESERVED FOR THE WORLD BACKGROUND
//HENCE ASSIGNING THIS KEY TO AN EMPTY PIXEL OBJECT THAT POINTS TO NULLPTR
GameWindow::Pixel* pixel=new GameWindow::Pixel();
pixel->type=0;
pixel->payload=nullptr;
GameWindow::PixelMap["000000000"]=pixel;


//DIVIDE WORLD OBJECT INTO INDIVIDUAL GAMEWORLD CUBES
int count=0;
int tx=0;
int tz=0;
GameWorldCube* TempCube;
for(int z=0;z<zbound;z=z+SmallestDivision)
{
  for(int x=0;x<xbound;x=x+SmallestDivision)
  {
   TempCube=MakeCube(z,x,minx,minz,maxy);
   (WorldCoordMap[z])[x]=TempCube;
   count++;
  }
}

//ADD REFERENCES TO NEIGHBOURING WORLD CUBES FOR EACH GAMEWORLDCUBE
for(int z=0;z<zbound;z=z+SmallestDivision)
{
  for(int x=0;x<xbound;x=x+SmallestDivision)
  {
    TempCube=(WorldCoordMap[z])[x];
    for(int i=0;i<8;i++)
    {
       if(i==0 && x!=0)//BACK CUBE
       {tz=z;tx=x-SmallestDivision;}
       else if(i==1 && x!=xbound - SmallestDivision)//FRONT CUBE
       {tz=z;tx=x+SmallestDivision;}
       else if(i==2 && z!=0)//LEFT CUBE
       {tz=z-SmallestDivision;tx=x;}
       else if(i==3 && z!=zbound - SmallestDivision)//RIGHT CUBE
       {tz=z+SmallestDivision;tx=x;}
       else if(i==4 && x!=0 && z!=0)//BACK LEFT CUBE
       {tz=z-SmallestDivision;tx=x-SmallestDivision;}
       else if(i==5 && x!=0 && z!=zbound - SmallestDivision)//BACK RIGHT CUBE
       {tz=z+SmallestDivision;tx=x-SmallestDivision;}
       else if(i==6 && x!=xbound - SmallestDivision && z!=0)//FRONT LEFT CUBE
       {tz=z-SmallestDivision;tx=x+SmallestDivision;}
       else if(i==7 && x!=xbound - SmallestDivision && z!=zbound - SmallestDivision)//FRONT RIGHT CUBE
       {tz=z+SmallestDivision;tx=x+SmallestDivision;}

       TempCube->Neighbours[i]=(WorldCoordMap[tz])[tx];//THE NEIGHBOUR CUBE EXIST IN THE WORLDCOORDMAP
                                                       //SAVE A REFERENCE TO IT IN THE CORRESPONDING NEIGHBOUR
    }
  }
}

//LOAD GAMEWORLD CUBE DATA IN GPU
unsigned int WorldVBO;
glGenVertexArrays(1, &WorldVAO);
glGenBuffers(1, &WorldVBO);
glBindVertexArray(WorldVAO);
glBindBuffer(GL_ARRAY_BUFFER, WorldVBO);
glBufferData(GL_ARRAY_BUFFER, WorldVBOBuffer.size() * sizeof(float), &WorldVBOBuffer.at(0), GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
glBindVertexArray(0);
WorldVBOBuffer.clear();
WorldVAOCount=count * 36;
std::cout<<"Total WorldCubes created ="<<count<<"\n";
}
/*************************************************
THIS FUNCTION USES THE X AMD Z VALUES ALONG WITH THE MINX, MINZ AND MAXY VALUES FROM THE WORLD OBJECT
TO CREATE A NEW GAMEWORLD CUBE USING THE FOLLOWING PROCEDURE:

i) THE GAMEWORLDCUBE'S CENTROID X COMPONENT = (MINX + X) + (SMALLESTDIVISION / 2.0)
ii) THE GAMEWORLDCUBE'S CENTROID Z COMPONENT = (MINZ + Z) + (SMALLESTDIVISION / 2.0)
iii) THE GAMEWORLDCUBE'S CENTROID Y COMPONENT REMAINS THE SAME AS THE WORLD OBJECT'S MAXY
   E.G - MINX=-100
	 MINZ=-50
	 SmallestDivision=2

	 GAMEWORLDCUBE CENTROID'S COORDINATES:
	 X =0 , Z =0
	 (MINX + X) + (SMALLESTDIVISION / 2.0) = (-100 + 0) + (2 / 2)= -99
	 (MINZ + Z) + (SMALLESTDIVISION / 2.0) = (-50 + 0) + (2 / 2)= -49

	 GAMEWORLDCUBE CENTROID'S COORDINATES:
	 X =2 , Z =0
	 (MINX + X) + (SMALLESTDIVISION / 2.0) = (-100 + 2) + (2 / 2)= -97
	 (MINZ + Z) + (SMALLESTDIVISION / 2.0) = (-50 + 0) + (2 / 2)= -49

iv) ADD THE NEWLY CREATED GAMEWORLDCUBE TO THE "WorldCoordMap" MAP USING THE Z AND X VALUES AS KEYS
v) USE THE PIXELMAPPING FUNCTION TO ASSIGN TO A NEW PIXEL OBJECT WITH A UNIQUE COLOR,THIS GAMEWORLDCUBE OBJECT
vi) LOAD THE VERTEX DATA FOR THIS GAMEWORLDCUBE IN THE COMMON VBO BUFFER
Vii) RETURN THE NEWLY CREATE GAMEWORLDCUBE
*************************************************/
GameWorldCube* GameWorld::MakeCube(int z,int x,float minx,float minz,float maxy)
{
GameWorldCube* Cube;
Cube=new GameWorldCube();
Cube->Index=Index++;
Cube->Centroid.x=(minx + x) + (SmallestDivision /2.0f);
Cube->Centroid.z=(minz + z) + (SmallestDivision /2.0f);
Cube->Centroid.y=maxy;
Cube->Object=nullptr;
Cube->ColorTint.x=0.8f;
Cube->ColorTint.y=0.8f;
Cube->ColorTint.z=0.0f;
GameWindow::PixelMapping(Cube);
Cube->Load(&WorldVBOBuffer,SmallestDivision);
return Cube;
}
/*************************************************
THIS FUNCTION CREATES VERTEX DATA FOR A GAMEWORLDCUBE USING ITS CENTROID
AND THE SmallestDivision VALUE AND THEN LOADS THAT VERTEX DATA IN THE COMMON VERTEX BUFFER OBJECT
WHICH IS USED TO RENDER ALL THE GAMEWORLDCUBES IN THE BACKGROUND FRAME BUFFER USING THE HELPER SHADER

THE COLORS VALUES USED FOR RENDERING THE GAMEWORLDCUBE IN THE BACKGROUND FRAME BUFFER
ARE THE RGB VALUES THAT WERE ASSIGNED TO THIS GAMEWORLDCUBE BY THE PIXELMAPPING FUNCTION.
THESE RGB VALUES CORRESPOND TO A UNIQUE COLOR VALUE THAT UNIQUELY IDENTIFY THIS GAMEWORLDCUBE
*************************************************/
void GameWorldCube::Load(std::vector<float>* VBOBuffer,int SmallestDivision)
{
float size=SmallestDivision/2.0f;
float vertices[] =
{
    //FRONT TRIANGLE
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,

    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,

    //BACK TRIANGLE
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,

    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,

    //LEFT TRIANGLE
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,

    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,

    //RIGHT TRIANGLE
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,

    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,

    //TOP TRIANGLE
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,

    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,

    //BOTTOM TRIANGLE
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,

    Centroid.x+size, Centroid.y, Centroid.z - size,R,G,B,
    Centroid.x+size, Centroid.y, Centroid.z + size,R,G,B,
    Centroid.x-size, Centroid.y, Centroid.z + size,R,G,B,
};

for(float f:vertices)
VBOBuffer->push_back(f);
}
/*************************************************
THIS FUNCTION CREATES VERTEX DATA FOR A GAMEWORLDCUBE USING ITS CENTROID
AND THE SmallestDivision VALUE AND THEN LOADS THAT VERTEX DATA IN THAT CUBE'S OWN VERTEX ARRAY OBJECT
WHICH IS USED TO RENDER THAT CUBE ON THE SCREEN USING THAT FOREGROUND FRAME BUFFER AND THE MAIN SHADER
*************************************************/
void GameWorldCube::Load(int SmallestDivision)
{
float size=SmallestDivision/2.0f;
float sizey=0.1f;
float vertices[] =
{
    //Front
    Centroid.x-size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x+size, Centroid.y, Centroid.z + size,

    Centroid.x+size, Centroid.y, Centroid.z + size,
    Centroid.x-size, Centroid.y, Centroid.z + size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z + size,

    //Back
    Centroid.x-size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x+size, Centroid.y, Centroid.z - size,

    Centroid.x+size, Centroid.y, Centroid.z - size,
    Centroid.x-size, Centroid.y, Centroid.z - size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z - size,

    //Left
    Centroid.x-size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x-size, Centroid.y, Centroid.z + size,

    Centroid.x-size, Centroid.y, Centroid.z + size,
    Centroid.x-size, Centroid.y, Centroid.z - size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z - size,

    //Right
    Centroid.x+size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x+size, Centroid.y, Centroid.z + size,

    Centroid.x+size, Centroid.y, Centroid.z + size,
    Centroid.x+size, Centroid.y, Centroid.z - size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z - size,

    //Top
    Centroid.x-size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z - size,

    Centroid.x+size, Centroid.y+sizey, Centroid.z - size,
    Centroid.x+size, Centroid.y+sizey, Centroid.z + size,
    Centroid.x-size, Centroid.y+sizey, Centroid.z + size,

    //Bottom
    Centroid.x-size, Centroid.y, Centroid.z + size,
    Centroid.x-size, Centroid.y, Centroid.z - size,
    Centroid.x+size, Centroid.y, Centroid.z - size,

    Centroid.x+size, Centroid.y, Centroid.z - size,
    Centroid.x+size, Centroid.y, Centroid.z + size,
    Centroid.x-size, Centroid.y, Centroid.z + size
};
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glBindVertexArray(0);
}
/*************************************************
THIS FUNCTION DRAWS A GAMEWORLDCUBE ON THE SCREEN USING THE COLORTINT COLOR VALUE
AND THE COLOR EFFECT FLOATING POINT VALUE THAT GOES BACK AND FORTH BETWEEN VALUES 0.1 AND 1.0
CREATING A BLINKING EFFECT BY INCREMENTING AND DECREMENTING THE COLOR VALUES
*************************************************/
void GameWorldCube::DrawForeground()
{
glUniform1i(5,1);
if(ColorSwitch==true)
{
    ColorEffect=ColorEffect - 0.01f;
    if(ColorEffect < 0.1f)
    ColorSwitch=false;
}
else if(ColorSwitch==false)
{
    ColorEffect=ColorEffect + 0.01f;
    if(ColorEffect > 1.0f)
    ColorSwitch=true;
}
glUniform4f(6,ColorEffect + ColorTint.x,ColorEffect+ColorTint.y,ColorEffect+ColorTint.z,1.0f);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GameWorldCube::Free()
{
glDeleteVertexArrays(1,&VAO);
glDeleteBuffers(1,&VBO);
}
/*************************************************
THIS FUNCTION DRAWS ALL THE GAMEWORLDCUBES WHICH ARE PART OF THE GAMEWORLD OBJECT
IN THE BACKGROUND FRAME BUFFER USING THE HELPER SHADER
*************************************************/
void GameWorld::Draw()
{
glUniform1i(5,0);
glBindVertexArray(WorldVAO);
glDrawArrays(GL_TRIANGLES,0,WorldVAOCount);
}




/**********************************************************************************************************
A CUBEMAP IS BASICALLY A TEXTURE THAT CONTAINS 6 INDIVIDUAL 2D TEXTURES THAT EACH FORM ONE SIDE OF A CUBE:
A TEXTURED CUBE. CUBE MAPS HAVE THE USEFUL PROPERTY THAT THEY CAN BE INDEXED/SAMPLED USING A DIRECTION VECTOR.

IF WE IMAGINE WE HAVE A CUBE SHAPE THAT WE ATTACH SUCH A CUBEMAP TO, THE DIRECTION VECTOR TO SAMPLE
THE CUBEMAP WOULD BE SIMILAR TO THE (INTERPOLATED) VERTEX POSITION OF THE CUBE. THIS WAY WE CAN SAMPLE
THE CUBEMAP USING THE CUBE’S ACTUAL POSITION VECTORS AS LONG AS THE CUBE IS CENTERED ON THE ORIGIN. WE CAN
THEN RETRIEVE THE TEXTURE COORDINATES OF ALL VERTICES AS THE VERTEX POSITIONS OF THE CUBE. THE RESULT IS A TEXTURE
COORDINATE THAT ACCESSES THE PROPER INDIVIDUAL FACE TEXTURE OF THE CUBEMAP

A CUBEMAP USED TO TEXTURE A 3D CUBE CAN BE SAMPLED USING THE POSITIONS OF THE CUBE AS THE TEXTURE
COORDINATES. WHEN A CUBE IS CENTERED ON THE ORIGIN (0,0,0) EACH OF ITS POSITION VECTORS IS ALSO A DIRECTION
VECTOR FROM THE ORIGIN. THIS DIRECTION VECTOR IS EXACTLY WHAT WE NEED TO GET THE CORRESPONDING TEXTURE VALUE AT
THAT SPECIFIC CUBE’S POSITION. FOR THIS REASON WE ONLY NEED TO SUPPLY POSITION VECTORS AND DON’T NEED TEXTURE
COORDINATES.
************************************************************************************************************/
void SkyBox::Draw()
{
glUniform1i(5,4);
glDepthFunc(GL_LEQUAL);
glBindVertexArray(VAO);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
glDrawArrays(GL_TRIANGLES, 0, 36);
glBindVertexArray(0);
glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
glDepthFunc(GL_LESS);
}

SkyBox::SkyBox(std::string FilePath) // @suppress("Class members should be properly initialized")
{
unsigned int skyboxVBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &skyboxVBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &vertices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
int Width=0;
int Height=0;
unsigned char* ImageData=nullptr;

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"right.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"left.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"top.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"bottom.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"front.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);

ImageData=nullptr;
ImageData=GameBMP::LoadBMP(std::string(FilePath+"back.bmp").c_str(),&Width,&Height,"SkyBox");
ImageData=GameBMP::FlipVertically(ImageData,Width,Height);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB,Width,Height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
GameBMP::FreeBMP(ImageData);


glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glBindVertexArray(0);
}

float SkyBox::vertices[]=
{
-1.0f,  1.0f, -1.0f,
-1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f, -1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f, -1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

-1.0f,  1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,
 1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
 1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
 1.0f, -1.0f,  1.0f
};

