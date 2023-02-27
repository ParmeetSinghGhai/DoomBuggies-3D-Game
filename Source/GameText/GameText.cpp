#include <GameText/GameText.h>
int GameText::GameTextIndex = 0;
GameText::~GameText()
{
    if (HealthVBO >= 0)
        glDeleteBuffers(1, &HealthVBO);
    if (HealthVAO >= 0)
        glDeleteVertexArrays(1, &HealthVAO);
    if (WeaponVBO >= 0)
        glDeleteBuffers(1, &WeaponVBO);
    if (WeaponVAO >= 0)
        glDeleteVertexArrays(1, &WeaponVAO);
    if (VBO >= 0)
        glDeleteBuffers(1, &VBO);
    if (VAO >= 0)
        glDeleteVertexArrays(1, &VAO);
    if (Texture >= 0)
        glDeleteTextures(1, &Texture);
    std::cout << "GAMETEXT #"<<std::to_string(Index)<<" FROM OBJECT #"<<std::to_string(ObjectIndex)<<" GOT DESTROYED\n";
}
/*************************************************
EVERY MOVABLE GAME OBJECT GETS A INFO PANEL THAT DISPLAYS THAT
OBJECT'S HEALTH STATUS AND WEAPON STATUS AND THE INFO PANEL IS ALWAYS FACING TOWARDS THE CAMERA
SO NO MATTER WHAT THE ORIENTATION OF THE OBJECT IS, THE INFO PANEL ALWAYS FACES THE CAMERA
AND THE USER CAN SEE THAT INFO PANEL HEAD ON

THE PROCEDURE FOLLOWED HERE IS:
1) USE THE HELPER SHADER TO DISPLAY THE IMAGE FOR THE INFO PANEL FOR ALL MOVABLE GAMEOBJECTS
2) EACH INFO PANEL CONTAINS TWO BOXES FOR HEALTH AND WEAPON STATUS RESPECTIVELY
3) THE BOXES SCALE IN THE X AXIS TO GROW BIGGER OR SMALLER IN SIZE TO INDICATE THE HEALTH AND WEAPON STATUS
4) THE COLOR OF THE TWO BOXES IS ALSO UPDATED WHERE
   A) IF THE HEALTH OR WEAPON IS BELOW 30 % THEN THE CORRESPONDING BOX WILL BE RED IN COLOR
   B) IF THE HEALTH OR WEAPON IS BETWEEN 30 % - 70 % THEN THE CORRESPONDING BOX WILL BE YELLOW IN COLOR
   C) IF THE HEALTH OR WEAPON IS 70 % - 100 % THEN THE CORRESPONDING BOX WILL BE GREEN IN COLOR
5) EACH BOX ALSO HAS A MATRIX WHICH IS CALCULATED AS FOLLOWS:
   THE CAMERA MATRIX / VIEW MATRIX IS COMPOSED OF THE FOLLOWING:

   VIEW MATRIX=INVERSE OF CAMERA ROTATION ON Y AXIS * INVERSE OF CAMERA ROTATION ON X AXIS * INVERSE OF CAMERA TRANSLATION

   EACH OBJECT IS MULTIPLIED BY THIS MATRIX SO THEY WOULD SHOW UP BASED ON THE CAMERA'S PERSPECTIVE SO TO GET THE INFO PANEL
   NOT TO BE EFFECTED BY THE WORLD MATRIX AND TO ALWAYS SHOW IN FRONT OF THE CAMERA, EACH INFO PANEL IS MULTIPLIED BY

   INFOPANEL MATRIX=CAMERA ROTATION ON Y AXIS * CAMERA ROTATION ON X AXIS * TRANSLATION MATRIX BASED ON OBJECT'S LOCATION

   THE ROTATION MATRIX "CAMERA ROTATION ON Y AXIS * CAMERA ROTATION ON X AXIS" MAKES SURE THAT THE INFO PANEL ROTATES ALONG WITH THE
   CAMERA OR THE INFOPANEL DOES NOT ROTATE AT ALL BASED ON THE CAMERA'S PERSPECTIVE AND THE TRANSLATION MATRIX BASED ON OBJECT'S
   BOUNDING BOX CENTROD VECTOR MAKES SURE THAT THE INFO PANEL ALWAYS MOVES ALONG WITH THE OBJECT AND STAYS ON TOP ON THE OBJECT AT A FIXED HEIGHT
*************************************************/
GameText::GameText(std::string BMPLocation,GameCamera::FPS *Camera,int ObjectIndex)
{
Index = GameText::GameTextIndex++;
this->ObjectIndex = ObjectIndex;

this->Camera=Camera;
float vertices[]=
{
0.0f,1.0f,1.0f,0.0f,1.0f,
0.5f,1.0f,1.0f,1.0f,1.0f,
0.5f,0.5f,1.0f,1.0f,0.0f,

0.5f,0.5f,1.0f,1.0f,0.0f,
0.0f,0.5f,1.0f,0.0f,0.0f,
0.0f,1.0f,1.0f,0.0f,1.0f
};
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(7);

glGenTextures(1, &Texture);
glBindTexture(GL_TEXTURE_2D, Texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int Width=0;
int Height=0;
unsigned char *ImageData=GameBMP::LoadBMP((BMPLocation).c_str(),&Width,&Height,"Text");
if(ImageData!=nullptr)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ImageData);
  glGenerateMipmap(GL_TEXTURE_2D);
  GameBMP::FreeBMP(ImageData);
}
glBindTexture(GL_TEXTURE_2D,0);


float healthvertices[]=
{
//HEALTHBOX IS MADE UP OF TWO TRIANGLES
//HERE WE SPECIFY THE THREE VERTICES OF ONE TRIANGLE ALONG WITH THE COLOR ASSIGNED TO EACH VERTEX
//HOWEVER THESE COLOR VALUES WONT BE USED TO COLOR THE HEALTH STATUS BOX INSTEAD THEY ARE USED AS MARKERS
//FOR THE VERTICES OF THE HEALTH BOX WHICH CAN BE TRANSLATED IN THE X DIRECTION BY THE VALUE OF
//HEALTH METER
//THE COLOR VALUES X COORDINATE BEING MORE THAN 1.0 IS THE MARKER WHICH IS WHY THEY ARE SET TO 2.0
0.5f,1.0f,1.0f,0.0f,0.0f,0.0f,
0.5f,1.0f,1.0f,2.0f,0.0f,0.0f,
0.5f,0.85f,1.0f,2.0f,0.0f,0.0f,

0.5f,0.85f,1.0f,2.0f,0.0f,0.0f,
0.5f,0.85f,1.0f,0.0f,0.0f,0.0f,
0.5f,1.0f,1.0f,0.0f,0.0f,0.0f
};

glGenVertexArrays(1, &HealthVAO);
glGenBuffers(1, &HealthVBO);
glBindVertexArray(HealthVAO);
glBindBuffer(GL_ARRAY_BUFFER, HealthVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(healthvertices), healthvertices, GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);


float weaponvertices[]=
{
//WAEPONBOX IS MADE UP OF TWO TRIANGLES
//HERE WE SPECIFY THE THREE VERTICES OF ONE TRIANGLE ALONG WITH THE COLOR ASSIGNED TO EACH VERTEX
//HOWEVER THESE COLOR VALUES WONT BE USED TO COLOR THE WEAPON STATUS BOX INSTEAD THEY ARE USED AS MARKERS
//FOR THE VERTICES OF THE WEAPON BOX WHICH CAN BE TRANSLATED IN THE X DIRECTION BY THE VALUE OF
//HEALTH METER
//THE COLOR VALUES X COORDINATE BEING MORE THAN 1.0 IS THE MARKER WHICH IS WHY THEY ARE SET TO 2.0
0.5f,0.83f,1.0f,0.0f,0.0f,0.0f,
0.5f,0.83f,1.0f,2.0f,0.0f,0.0f,
0.5f,0.67f,1.0f,2.0f,0.0f,0.0f,

0.5f,0.67f,1.0f,2.0f,0.0f,0.0f,
0.5f,0.67f,1.0f,0.0f,0.0f,0.0f,
0.5f,0.83f,1.0f,0.0f,0.0f,0.0f
};

glGenVertexArrays(1, &WeaponVAO);
glGenBuffers(1, &WeaponVBO);
glBindVertexArray(WeaponVAO);
glBindBuffer(GL_ARRAY_BUFFER, WeaponVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(weaponvertices), weaponvertices, GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
}

/*************************************************
THIS FUNCTION UPDATES THE COLOR OF THE HEALTH BOX AND THE WEAPON BASED ON THEIR RESPECTIVE
VARIABLES "HEALTH METER" AND "WEAPON METER". THE VALUES OF THESE VARIABLES IS GENERATED BASED ON THE
ARGUMENTS "HEALTH" AND "WEAPON" PASSED ONTO THE FUNCTION AND THE FORMULAE:

HEALTH METER = HEALTH / 100 , VALUES GENERATED ARE BETWEEN 0.0 AND 1.0
WEAPON METER = WEAPON / 100 , VALUES GENERATED ARE BETWEEN 0.0 AND 1.0

THE COLOR OF THE TWO BOXES IS DECIDED BASED ON THE FOLLOWING CRITERIA:
 A) IF THE HEALTH METER OR WEAPON IS BELOW 30 % OR 0.3 THEN THE CORRESPONDING BOX WILL BE RED IN COLOR
 B) IF THE HEALTH METER OR WEAPON IS BETWEEN 30 % - 70 % OR 0.3 - 0.7 THEN THE CORRESPONDING BOX WILL BE YELLOW IN COLOR
 C) IF THE HEALTH METER OR WEAPON IS 70 % - 100 % OR 0.7 - 1.0 THEN THE CORRESPONDING BOX WILL BE GREEN IN COLOR

THE TRANSFORMATION MATRIX FOR THE INFOPANEL IS GENERATED LIKE SO
 INFOPANEL MATRIX=CAMERA ROTATION ON Y AXIS * CAMERA ROTATION ON X AXIS * TRANSLATION MATRIX BASED ON OBJECT'S LOCATION

 THE ROTATION MATRIX "CAMERA ROTATION ON Y AXIS * CAMERA ROTATION ON X AXIS" MAKES SURE THAT THE INFO PANEL ROTATES ALONG WITH THE
 CAMERA OR THE INFOPANEL DOES NOT ROTATE AT ALL BASED ON THE CAMERA'S PERSPECTIVE AND THE TRANSLATION MATRIX BASED ON OBJECT'S
 BOUNDING BOX CENTROD VECTOR MAKES SURE THAT THE INFO PANEL ALWAYS MOVES ALONG WITH THE OBJECT AND STAYS ON TOP ON THE OBJECT AT A FIXED HEIGHT
*************************************************/
void GameText::Update(float Health,float Weapon,GameMath::Vector3* Location)
{
if(Health<=100.0f && Health > 0.0f)
{
    HealthMeter=(Health/100.0f);
    if(HealthMeter>0.7f)
    {
        HealthColor.x=0.0f;
        HealthColor.y=1.0f;
    }
    else if(HealthMeter>0.3f && HealthMeter<0.7f)
    {
        HealthColor.x=1.0f;
        HealthColor.y=1.0f;
    }
    else if(HealthMeter<0.3f)
    {
        HealthColor.x=1.0f;
        HealthColor.y=0.0f;
    }
}
if(Weapon<=100.0f && Weapon > 0.0f)
{
    WeaponMeter=(Weapon/100.0f);
    if(WeaponMeter>0.7f)
    {
        WeaponColor.x=0.0f;
        WeaponColor.y=1.0f;
    }
    else if(WeaponMeter>0.3f && WeaponMeter<0.7f)
    {
        WeaponColor.x=1.0f;
        WeaponColor.y=1.0f;
    }
    else if(WeaponMeter<0.3f)
    {
        WeaponColor.x=1.0f;
        WeaponColor.y=0.0f;
    }
}
TMatrix.Clean();
TMatrix=GameMath::Transform::RotateQ(Camera->Up,Camera->YawAngle) * GameMath::Transform::RotateQ(Camera->Right,Camera->PitchAngle);
TMatrix.Matrix[3][0]=Location->x;
TMatrix.Matrix[3][1]=Location->y;
TMatrix.Matrix[3][2]=Location->z;
}

/*************************************************
THIS FUNCTION SENDS OUT THE INFO PANEL'S TO THE HELPER SHADER
1) HEALTH COLOR AND WEAPON COLOR 3D VECTORS
2) HEALTH METER AND WEAPON METER VALUES WHICH IS USED TO CALCULATE THE SCALING MATRIX FOR THE HEALTH AND WEAPON BOX
3) INFO PANEL MATRIX
*************************************************/
void GameText::Draw()
{
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, Texture);

glUniformMatrix4fv(2, 1, GL_FALSE,TMatrix.Addressof());
glUniform1i(5,2);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 6);

glUniform1i(5,3);
glUniform4f(6,HealthColor.x,HealthColor.y,HealthColor.z,0.5f);
glUniform1f(8,HealthMeter);
glBindVertexArray(HealthVAO);
glDrawArrays(GL_TRIANGLES, 0, 6);

glUniform1i(5,3);
glUniform4f(6,WeaponColor.x,WeaponColor.y,WeaponColor.z,0.5f);
glUniform1f(8,WeaponMeter);
glBindVertexArray(WeaponVAO);
glDrawArrays(GL_TRIANGLES, 0, 6);
}
