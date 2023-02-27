#include <GameBoundingBox/GameBoundingBox.h>
int GameBoundingBox::BoundingBoxIndex=0;
GameBoundingBox::~GameBoundingBox()
{
    if (VBO >= 0)
        glDeleteBuffers(1, &VBO);
    if (VAO >= 0)
        glDeleteVertexArrays(1, &VAO);
    std::cout << "GAMEBOUNDINGBOX #" << std::to_string(Index) << " FROM OBJECT #"<<std::to_string(ObjectIndex)<<" GOT DESTROYED\n";
}
/*************************************************
A BOUNDING BOX IS A CUBE THAT SURROUNDS AN OBJECT WHICH MOVES ALONG WITH THE OBJECT
AND HELPS WITH COLLISION DETECTION BETWEEN THIS OBJECT AND OTHER OBJECTS THAT ALSO HAVE THEIR OWN
BOUNDING BOXES

A BOUNDING BOX CONTAINS A 3D VECTOR CALLED CENTROID THAT REPRESENTS THE CENTER OF THE BOUDNING BOX
AND 6 FLOATING POINT VALUES NAMELY MINX,MAXX,MINY,MAXY,MINZ AND MAXZ WHICH REPRESENT THE
MINIMUM AND MAXIMUM VALUES ALONG THE X , Y AND Z COORDINATE AXIS WITH THE VALUE OF THE CENTROID BEING AT
THE CENTER OF THOSE MINIMUM AND MAXIMUM VALUES

AN EXAMPLE WOULD BE THE CENTROID BEING AT THE POSITION (4,5,7) HERE THE MINX WOULD BE 3 AND MAXX WOULD BE 5 ON THE X AXIS

THE DEFAULT CONSTRUCTORS:
1) SETS THE MINX,MAXX,MINY,MAXY,MINZ AND MAXZ BASED ON THE ARGUMENTS PASSED TO IT
2) CALCULATES THE CENTROID BASED ON THOSE VALUES USING THE FORMULAE:
   CENTROID VALUE = (MAXIMUM VALUE + MINIMUM VALUE) / 2
3) BASED ON THE CENTROID, THE OFFSET FOR THE MINIMUM AND MAXIMUM VALUES ARE CALCULATED AND SAVED
   SO THEY CAN BE LATER USED TO CALCULATE THE NEW MINIMUM AND MAXIMUM VALUES WHEN THE CENTROID MOVES
   THE FORMULAE USED IS AS FOLLOWS:
   MAXIMUM VALUE OFFSET = CURRENT MAXIMUM VALUE - CENTROID VALUE
   MINIMUM VALUE OFFSET = CENTROID VALUE - CURRENT MINIMUM VALUE
4) THE COLOR TINT IS SET TO (0.0,0.0,0.5) IN THE FORMAT (RED,GREEN,BLUE)

BOUDING BOXED ARE ONLY TRANSLATED AND NOT ROTATED OR SCALED
*************************************************/
GameBoundingBox::GameBoundingBox(float maxx,float minx,float maxy,float miny,float maxz,float minz,int ObjectIndex)
{
Index=BoundingBoxIndex++;
this->ObjectIndex = ObjectIndex;

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

this->maxx=maxx;
this->minx=minx;
this->maxy=maxy;
this->miny=miny;
this->maxz=maxz;
this->minz=minz;

Centroid.x=(maxx + minx) / 2.0f;
Centroid.y=(maxy + miny) / 2.0f;
Centroid.z=(maxz + minz) / 2.0f;

offset_maxx=maxx - Centroid.x;
offset_minx=Centroid.x - minx;
offset_maxy=maxy - Centroid.y;
offset_miny=Centroid.y - miny;
offset_maxz=maxz - Centroid.z;
offset_minz=Centroid.z - minz;

ColorTint.x=0.0f;
ColorTint.y=0.0f;
ColorTint.z=0.5f;
}
/*************************************************
WHENEVER THE BOUNDING BOX MOVES, IT CENTROID IS THE MAIN COMPONENT THAT IS TRANSLATED
BASED ON THE CENTROID AND THE OFFSET VALUES FOR THE MINIMUM AND MAXIMUM VARIABLED ALONG EACH COORDINATE AXIS
THE NEW MINIMUM AND MAXIMUM VALUES ARE COMPUTED

THE FORMULAE USED IS AS FOLLOWS:
MINIMUM VALUE = CENTROID VALUE - OFFSET FOR MINIMUM VALUE
MAXIMUM VALUE = CENTROID VALUE + OFFSET FOR THE MAXIMUM VALUE

FOR EXAMPLE LET THE CENTROID BE AT (X,Y,Z) = (6,7,8) AND THE OFFSETS FOR WERE MINX=2 AND MAXX=1
THE NEW MINX WILL BE MINX = 6 - 2 = 4
THE NEW MAXX WILL BE MAXX = 6 + 1 = 7
************************************************/
void GameBoundingBox::CalculateXYZ()
{
minx=Centroid.x - offset_minx;
maxx=Centroid.x + offset_maxx;
miny=Centroid.y - offset_miny;
maxy=Centroid.y + offset_maxy;
minz=Centroid.z - offset_minz;
maxz=Centroid.z + offset_maxz;
}

void GameBoundingBox::Load()
{
//** The Bounding Box uses its own VAO *****
float vertices[] =
{
    //Front
    minx,maxy,maxz,R,G,B,
    maxx,maxy,maxz,R,G,B,
    maxx,miny,maxz,R,G,B,

    maxx,miny,maxz,R,G,B,
    minx,miny,maxz,R,G,B,
    minx,maxy,maxz,R,G,B,

    //Back

    minx,maxy,minz,R,G,B,
    maxx,maxy,minz,R,G,B,
    maxx,miny,minz,R,G,B,

    maxx,miny,minz,R,G,B,
    minx,miny,minz,R,G,B,
    minx,maxy,minz,R,G,B,

    //Left
    minx,maxy,maxz,R,G,B,
    minx,maxy,minz,R,G,B,
    minx,miny,minz,R,G,B,

    minx,miny,minz,R,G,B,
    minx,miny,maxz,R,G,B,
    minx,maxy,maxz,R,G,B,

    //Right
    maxx,maxy,maxz,R,G,B,
    maxx,maxy,minz,R,G,B,
    maxx,miny,minz,R,G,B,

    maxx,miny,minz,R,G,B,
    maxx,miny,maxz,R,G,B,
    maxx,maxy,maxz,R,G,B,

    //Top
    minx,maxy,maxz,R,G,B,
    minx,maxy,minz,R,G,B,
    maxx,maxy,minz,R,G,B,

    maxx,maxy,minz,R,G,B,
    maxx,maxy,maxz,R,G,B,
    minx,maxy,maxz,R,G,B,

    //Bottom
    minx,miny,maxz,R,G,B,
    minx,miny,minz,R,G,B,
    maxx,miny,minz,R,G,B,

    maxx,miny,minz,R,G,B,
    maxx,miny,maxz,R,G,B,
    minx,miny,maxz,R,G,B
};

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
glBindVertexArray(0);
}

/*************************************************
THIS FUNCTION RENDERS/DRAWS THE BOUDNING BOX USING THE HELPER SHADER
IN THE BACKGROUND FRAMEBUFFER
*************************************************/
void GameBoundingBox::DrawBackground()
{
glUniformMatrix4fv(2, 1, GL_FALSE, FinalMatrix.Addressof());
glUniform1i(5,0);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
}
/*************************************************
THIS FUNCTION RENDERS/DRAWS THE BOUDNING BOX USING THE MAIN SHADER
IN THE FOREGROUND FRAMEBUFFER

WHILE RENDERING THE BOUNDING BOX IT ALSO CREATES A FLASHING OR A BLINKING EFFECT
ON THE BOUNDING BOX. THIS IS HOW ITS DONE
1) "COLOREFFECTS" VARIABLE ACTS AS THE BRIGHTNESS LEVEL FOR THE COLOR SET FOR THE BOUNDING BOX
   USING THE "COLORTINT" VARIABLE
2) WHEN THE "COLOREFFECTS" VARIABLE IS SET TO 0.1 THEN ITS INCREMENTED GRADUALLY BY 0.01 VALUE TO
   MAKE THE "COLORTINT" COLOR DARKER TILL IT REACHES THE VALUE OF 1.0
3) WHEN THE "COLOREFFECTS" VARIABLE IS SET TO 1.0 THEN ITS DECREMENTED GRADUALLY BY 0.01 VALUE TO
   MAKE THE "COLORTINT" COLOR LIGHTER TILL IT REACHES THE VALUE OF 0.0
4) THE LOOP BETWEEN (2) AND (3) IS REPEATED AS LONG AS THE DRAW FUNCTIONS IS CALLED
   THEREBY CREATING A BLINK OR A FLASH EFFECT ON THE BOUNDING BOX
*************************************************/
void GameBoundingBox::DrawForeground()
{
glUniform1i(5,1);
#if !DEBUG
if(ColorSwitch==true)
{
    ColorEffect=ColorEffect - 0.08f;
    if(ColorEffect < 0.1f)
    ColorSwitch=false;
}
else if(ColorSwitch==false)
{
    ColorEffect=ColorEffect + 0.08f;
    if(ColorEffect > 1.0f)
    ColorSwitch=true;
}
#endif
glUniformMatrix4fv(2, 1, GL_FALSE, FinalMatrix.Addressof());
#if !DEBUG
glUniform4f(6,ColorEffect + ColorTint.x,ColorEffect+ColorTint.y,ColorEffect+ColorTint.z,0.5f);
#else
glUniform4f(6, ColorTint.x, ColorTint.y, ColorTint.z, 1.0f);
#endif
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
}

