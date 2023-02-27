#include <GameCamera/GameCamera.h>
/**************************************************
YAW ANGLE IS THE ROTATION ANGLE WHEN ROTATION TAKES PLACE ALONG THE Y AXIS
PITCH ANGLE IS THE ROTATION ANGLE WHEN ROTATION TAKES PLACE ALONG THE X AXIS

WHEN THE USER MOVES THE MOUSE ALONG ITS X AXIS THEN THAT MOVEMENT GETS COVERTED INTO YAW ANGLE
WHEN THE USER MOVES THE MOUSE ALONG ITS Y AXIS THEN THAT MOVEMENT GETS CONVERTED INTO PITCH ANGLE

                         Y AXIS (PITCH)
                           *
                           *
                           *
                           *
                           *
      * * * * * * MOUSE ON MOUSE PAD * * * * * * X AXIS (YAW)
                           *
                           *
                           *
                           *
                           *

TO BEGIN WITH THE CAMERA HAS ITS OWN FRAME OF REFERENCE WHERE ITS PLACED AT THE ORIGIN AND ITS LOOKING TOWARDS
THE -Z AXIS WITH ITS Y AXIS POINTING UPWARDS AND X AXIS POINTING TO THE RIGHT

                         Y AXIS
                          *       -Z AXIS
                          *       *
                          *     *
                          *   *
                          * *
            * * * * * * CAMERA * * * * * * X AXIS
                        * * (0,0,0)
                      *   *
                    *     *
                  *       *
              +Z AXIS     *

THE USER'S MOUSE MOVEMENTS ARE USED TO ROTATE THE CAMERA AND THE USER'S KEYBOARD MOVEMENTS ARE USED TO MOVE THE CAMERA ALONG WITH ITS 3 AXIS
IT IS THESE CAMERA MOVEMENTS AND ROTATIONS THAT GET APPLIED TO ALL 3D OBJECTS IN THE FORM OF A VIEW MATRIX. THE STEPS INVOLVED IN CREATING A
VIEW MATRIX IS AS FOLLOWS:

1) SET THE CAMERA POSITION AS (0,0,0) WHERE X=0, Y=0 AND Z=0
2) SET THE CAMERA X AXIS AS (1,0,0) , Y AXIS AS (0,1,0) AND THE Z AXIS AS (0,0,-1) SINCE CAMERA IS LOOKING AT THE -Z AXIS
3) COMPUTE THE YAW ANGLE AND THE PITCH ANGLE FROM MOUSE INPUTS AND USE THEM IN THE ROTATION MATRIX ALONG Y AXIS AND X AXIS RESPECTIVELY
   YOU DO THE X (PITCH) ROTATIONS FIRST AND THE Y (YAW) ROTATIONS WHERE THE ORDER OF MATRIX MULTIPLICATION IS Y MATRIX * X MATRIX

          Y AXIS                                 X AXIS
   [COS(YAW)  0  SIN(YAW)]              [1       0           0    ]      [ COS(YAW)    SIN(YAW)SIN(PITCH)     SIN(YAW)COS(PITCH)]
   [   0      1      0   ]       X      [0  COS(PITCH) -SIN(PITCH)]  =   [  0             COS(PITCH)            -SIN(PITCH)     ]  ------- (1)
   [-SIN(YAW) 0  COS(YAW)]              [0  SIN(PITCH)  COS(PITCH)]      [-SIN(YAW)    SIN(PITCH)COS(YAW)     COS(PITCH)COS(YAW)]

4) NOW WE USE THE MATRIX (1) AND MULPLITY IT WITH THE CAMERA'S X, Y AND Z AXIS TO GET THE NEW ROTATED CAMERA AXIS

   [ COS(YAW)    SIN(YAW)SIN(PITCH)     SIN(YAW)COS(PITCH)]      [ 1 ]      [COS(YAW) ]
   [  0             COS(PITCH)            -SIN(PITCH)     ]   X  [ 0 ]   =  [    0    ]
   [-SIN(YAW)    SIN(PITCH)COS(YAW)     COS(PITCH)COS(YAW)]      [ 0 ]      [-SIN(YAW)]


   [ COS(YAW)    SIN(YAW)SIN(PITCH)     SIN(YAW)COS(PITCH)]      [ 0 ]      [SIN(YAW)SIN(PITCH)]
   [  0             COS(PITCH)            -SIN(PITCH)     ]   X  [ 1 ]   =  [    COS(PITCH)    ]
   [-SIN(YAW)    SIN(PITCH)COS(YAW)     COS(PITCH)COS(YAW)]      [ 0 ]      [SIN(PITCH)COS(YAW)]


   [ COS(YAW)    SIN(YAW)SIN(PITCH)     SIN(YAW)COS(PITCH)]      [  0 ]     [-SIN(YAW)COS(PITCH)]
   [  0             COS(PITCH)            -SIN(PITCH)     ]   X  [  0 ]  =  [    SIN(PITCH)     ]
   [-SIN(YAW)    SIN(PITCH)COS(YAW)     COS(PITCH)COS(YAW)]      [ -1 ]     [-COS(PITCH)COS(YAW)]


   ROTATED X AXIS =  [COS(YAW) , 0  ,-SIN(YAW)]
   ROTATED Y AXIS =  [SIN(YAW)SIN(PITCH) ,  COS(PITCH) ,   SIN(PITCH)COS(YAW)]
   ROTATED Z AXIS =  [-SIN(YAW)COS(PITCH) ,  SIN(PITCH) ,  -COS(PITCH)COS(YAW)]

5) USING THE UPDATED/ROTATED CAMERA AXIS WE GET THE UPDATED CAMERA POSITION BY TRANSLATING THE CURRENT CAMERA POSITION
   ALONG ONE OF THESE ROTATED AXIS BASED ON THE INPUT RECEIVED FROM THE KEYBOARD

   PRESSING 'W' ON THE KEYABORD WILL TRANSLATE THE CAMERA IN A POSITIVE WAY ALONG THE NEW/ROTATED CAMERA Z AXIS
   PRESSING 'S' ON THE KEYABORD WILL TRANSLATE THE CAMERA IN A NEGATIVE WAY ALONG THE NEW/ROTATED CAMERA Z AXIS
   PRESSING 'A' ON THE KEYABORD WILL TRANSLATE THE CAMERA IN A POSITIVE WAY ALONG THE NEW/ROTATED CAMERA X AXIS
   PRESSING 'D' ON THE KEYABORD WILL TRANSLATE THE CAMERA IN A NEGATIVE WAY ALONG THE NEW/ROTATED CAMERA X AXIS

   THE FORMULAE USED IS
   UPDATED CAMERA POSITION = CURRENT CAMERA POSITION + (NEW DIRECTION * SCALAR DISTANCE NEEDED TO BE MOVED IN THIS DIRECTION)

6) AFTER WE OBTAIN THE CAMERA'S UPDATED POSITION THEN WE USE IT TO CONTRUCT THE VIEW MATRIX
   THE PRINCIPLE USED FOR THIS CONVERSION IS AS FOLLOWS:
   1) CAMERA MOVING FORWARD IS THE SAME AS THE WORLD MOVING BACKWARDS
      CAMERA MOVING BACKWARDS IS THE SAME AS THE WORLD MOVING FORWARDS
      IN GENERAL THE WORLD MOVES IN THE REVERSE OF THE DIRECTION THAT THE CAMERA MOVES IN

   2) CAMERA ROTATING COUNTERCLOCKWISE ON THE Y AXIS IS THE SAME AS THE WORLD ROTATING CLOCKWISE ALONG THE Y AXIS
      CAMERA ROTATING CLOCKWISE ON THE Y AXIS IS THE SAME AS THE WORLD ROTATING COUNTERCLOCKWISE ALONG THE Y AXIS
      IN GENERAL THE WORLD ROTATES IN THE REVERSE OF THE DIRECTION THAT THE CAMERA ROTATES IN

   USING THE ABOVE TWO PRINCIPLES WE CAN MOVE/ROTATE THE WORLD BY
   1) MOVING THE WORLD IN THE REVERSE DIRECTION THAT THE CAMERA MOVES BY TRANSLATING THE WORLD OBJECTS BY THE NEGATIVE OF THE CAMERA POSITION
      WHICH GIVES US THE FOLLOWING TRANSLATION MATRIX:
      [1   0   0  -CAMERAPOSITION.X]
      [1   0   0  -CAMERAPOSITION.Y]    ---------- (2)
      [1   0   0  -CAMERAPOSITION.Z]
      [0   0   0          1        ]

   2) ROTATING THE WORLD IN THE REVERSE DIRECTION THAT THE CAMERA MOVES BY USING THE INVERSE OF THE CAMERA ROTATION MATRIX IN (1) ABOVE
      INVERSE OF A ROTATION MATRIX IS ITS TRANSPOSE

      [COS(YAW)                 0            -SIN(YAW)      ]
      [SIN(YAW)SIN(PITCH)    COS(PITCH)   SIN(PITCH)COS(YAW)] ------------ (3)
      [SIN(YAW)COS(PITCH)   -SIN(PITCH)   COS(PITCH)COS(YAW)]


7) WE FINALLY GET THE VIEW MATRIX BY MULTIPLYING THE TRANSLATION MATRIX ABOVE IN (2) WITH THE ROTATION MATRIX IN (3) IN THAT ORDER
   WHERE THE TRANSLATION HAPPENS FIRST AND THEN THE ROTATION TAKES PLACE

      [COS(YAW)                 0            -SIN(YAW)          0 ]       [1   0   0  -CAMERAPOSITION.X]
      [SIN(YAW)SIN(PITCH)    COS(PITCH)   SIN(PITCH)COS(YAW)    0 ]   X   [1   0   0  -CAMERAPOSITION.Y]
      [SIN(YAW)COS(PITCH)   -SIN(PITCH)   COS(PITCH)COS(YAW)    0 ]       [1   0   0  -CAMERAPOSITION.Z]
      [ 0                       0                0              1 ]       [0   0   0          1        ]
**************************************************/
void GameCamera::FPS::UpdateCameraView()
{
  if(GameWindow::HasFocus==true)
  MouseTracking();

  //******* Mouse Updates *************************
  //******* Multiplication order = X Axis (first) , Y Axis (Second) = Y Matrix * X Matrix
  const float YCos {cos(GameMath::Transform::GetRadian(YawAngle))};
  const float YSin {sin(GameMath::Transform::GetRadian(YawAngle))};
  const float XCos {cos(GameMath::Transform::GetRadian(PitchAngle))};
  const float XSin {sin(GameMath::Transform::GetRadian(PitchAngle))};

  Front=GameMath::Vector3((-YSin * XCos),XSin,(-YCos * XCos)).Normalize();
  Right=GameMath::Vector3(YCos,0.0f,-YSin).Normalize();
  Up=GameMath::Vector3((YSin * XSin),XCos,(YCos * XSin)).Normalize();
  //***********************************************

  //******* Keyboard Updates **********************
  if(GetKeyState(87) & 0x8000)
  CameraPosition = CameraPosition + (Front * KeyboardSensitivity);
  if(GetKeyState(83) & 0x8000)
  CameraPosition = CameraPosition - (Front * KeyboardSensitivity);
  if(GetKeyState(65) & 0x8000)
  CameraPosition = CameraPosition - (Right * KeyboardSensitivity);
  if(GetKeyState(68) & 0x8000)
  CameraPosition = CameraPosition + (Right * KeyboardSensitivity);
  //***********************************************

  //******** View Matrix Updates ******************
  View.Matrix[0][0]=YCos;
  View.Matrix[0][1]=XSin * YSin;
  View.Matrix[0][2]=XCos * YSin;
  View.Matrix[0][3]=0.0f;

  View.Matrix[1][0]=0.0f;
  View.Matrix[1][1]=XCos;
  View.Matrix[1][2]=-XSin;
  View.Matrix[1][3]=0.0f;

  View.Matrix[2][0]=-YSin;
  View.Matrix[2][1]=XSin * YCos;
  View.Matrix[2][2]=XCos * YCos;
  View.Matrix[2][3]=0.0f;

  View.Matrix[3][0]=(-CameraPosition.x * YCos) + (-CameraPosition.z * -YSin);
  View.Matrix[3][1]=(-CameraPosition.x * XSin * YSin) + (-CameraPosition.y * XCos) + (-CameraPosition.z * XSin * YCos);
  View.Matrix[3][2]=(-CameraPosition.x * XCos * YSin) + (-CameraPosition.y * -XSin) + (-CameraPosition.z * XCos * YCos);
  View.Matrix[3][3]=1.0f;
  //***********************************************
}
/***********************************************
THIS FUCNTION IS DESIGNED TO GET YAW AND PITCH ANGLES FROM MOUSE INPUT, IT DOES IT THE FOLLOWING WAY:

1) IF THE GAME WINDOW JUST GOT FOCUS THE SET THE VARIABLE "FIRSTMOUSE" TRUE INDICATING THAT THE WINDOW HAD LOST THE MOUSE BEFORE
   AND NOW IT GOT IT BACK AND IS IN FOCUS. AT THIS POINT THE FUNCTION RETURNS

2) THE NEXT TIME THIS FUNCTION IS CALLED AND IT GETS THE CURRENT CURSOR POSITION AND SAVES IT IN WINDOW POINT STRUCTURE
   AFTER THAT:
   A) IF THIS WAS THE FIRST TIME THE GAME WINDOW GOT THE MOUSE THEN MOVE THE MOUSE TO THE CENTER OF THE GAME WINDOW
      THE CENTER OF THE GAME WINDOW IS = WIDTH OF THE GAME WINDOW / 2 , HEIGHT OF THE GAME WINDOW / 2
      THEN GET THE CURRENT CURSOR POSITION AND SET IT AS THE LAST CURSOR POSITION AND RETURN
      ALSO SET THE VARIABLE  "FIRSTMOUSE" TO FALSE INDICATING THAT THIS SCENARIO HAS BEEN PROCESSED

   B) IF THE GAME WINDOW WAS RESIZED THEN MOVE THE MOUSE TO THE CENTER OF THE GAME WINDOW
      THE CENTER OF THE GAME WINDOW IS = WIDTH OF THE GAME WINDOW / 2 , HEIGHT OF THE GAME WINDOW / 2
      THEN GET THE CURRENT CURSOR POSITION AND SET IT AS THE LAST CURSOR POSITION AND RETURN
      ALSO SET THE VARIABLE  "WINDOWHASRESIZED" TO FALSE INDICATING THAT THIS SCENARIO HAS BEEN PROCESSED

   C) CHECK IF THE MOUSE IS IN THE ALLOWED MOUSE SPACE AS DESCRIBED BELOW

          ***********************************************
          *                Y+5                          * GAMEWINDOW WITH (X,Y,WIDTH,HEIGHT)
          *     ****************************            *
          *     *                          *            *
          * X+5 *  ALLOWED MOUSE SPACE     * (X+WIDTH)-5*
          *     *                          *            *
          *     *                          *            *
          *     ****************************            *
          *              (Y+HEIGHT)-5                   *
          ***********************************************

      IF NOT THEN MOVE THE MOUSE TO THE CENTER OF THE GAME WINDOW
      THE CENTER OF THE GAME WINDOW IS = WIDTH OF THE GAME WINDOW / 2 , HEIGHT OF THE GAME WINDOW / 2
      THEN GET THE CURRENT CURSOR POSITION AND SET IT AS THE LAST CURSOR POSITION AND RETURN
      THIS ALLOWS THE USER TO GET 360 DEGREE ROTATIONS WHILE KEEPING THE MOUSE CAPTURED BY THE GAME WINDOW

3) IF A, B AND C ARE NOT HIT IN THE ABOVE SCENARIOS THEN YOU GET THE CALCULATE THE MOUSE OFFSET
   BY SUBTRACTING THE LAST MOUSE POSITIONS FROM THE CURRENT MOUSE POSITIONS
   AFTER THAT YOU SET THE CURRENT MOUSE POSITION AS THE LAST MOUSE POSITION

4) IF THE GAMEWINDOW HAS CAPTURED THE MOUSE AND THE MOUSE LEFT BUTTON HAS BEEN PRESSED FOR MORE THAN 100 MILISECONDS
   THEN USE THE MOUSEOFFSET TO UPDATE THE VALUES OF THE YAW AND THE PITCH ANGLES
   AND ALSO HIDE THE MOUSE CURSOR BY SETTING THE MOUSE CURSOR TO NULL BEFORE SAVING THE CURSOR STATE
   THIS ALLOWS THE USER TO ROTATE THE CAMERA ONLY WHEN THE MOUSE LEFT BUTTON IS PRESSED FOR MORE THAN 100 MILISECONDS
   WHICH ALSO ALLOWS THE USER TO USE THE REGULAR MOUSE CLICKS TO INTERACT WITH GAME OBJECTS

   IF THE MOUSE LEFT BUTTON IS NOT PRESSED FOR MORE THAN 100 MILISECONDS THEN
   SET THE VARIABLE "CAMERAHASMOUSE" TO FALSE INDICATING THAT THE GAMEWINDOW HAS LOST MOUSE FOCUS
   THIS PREVENTS THE MOUSE CURSOR FROM GETTING PUT BACK INTO THE MIDDLE OF THE GAME WINDOW
   AND ALSO ALLOWS THE MOUSE CURSOR TO SHOW UP AGAIN SO USER CAN USE THAT CURSOR TO INTERACT WITH GAME OBJECTS
/***********************************************/
void GameCamera::FPS::MouseTracking()
{
if(GameWindow::HasFocus==false)
{
FirstMouse=true;
return;
}

GetCursorPos(&p);
if(FirstMouse==true)
{
    SetCursorPos(GameWindow::X +(GameWindow::Width/2),GameWindow::Y + (GameWindow::Height/2));
    GetCursorPos(&p);
    LastX=p.x;
    LastY=p.y;
    FirstMouse=false;
    return;
}
if(GameWindow::WindowHasResized==true)
{
    SetCursorPos(GameWindow::X +(GameWindow::Width/2),GameWindow::Y + (GameWindow::Height/2));
    GetCursorPos(&p);
    LastX=p.x;
    LastY=p.y;
    GameWindow::WindowHasResized=false;
    return;
}
if((p.x < GameWindow::X + 5 || p.y < GameWindow::Y + 5 || p.x > GameWindow::X + (GameWindow::Width -5) || p.y > GameWindow::Y + (GameWindow::Height-5)) && GameWindow::CameraHasMouse==true)
{
    SetCursorPos(GameWindow::X +(GameWindow::Width/2),GameWindow::Y + (GameWindow::Height/2));
    GetCursorPos(&p);
    LastX=p.x;
    LastY=p.y;
    return;
}
MouseOffsetX=p.x - LastX;
MouseOffsetY=p.y - LastY;
LastX=p.x;
LastY=p.y;


if(GetTickCount() > GameWindow::MouseTimer + CameraHasMouse_ActivationTime && GameWindow::MouseLButtonPressed==true)
{
   YawAngle=YawAngle - (MouseOffsetX * MouseSensitivity);
   PitchAngle=PitchAngle - (MouseOffsetY * MouseSensitivity);

   GameWindow::CameraHasMouse=true;
   if((SavedCursor=GetCursor())!=NULL)
   SetCursor(NULL);
}
else
{
   if(GameWindow::CameraHasMouse==true)
   GameWindow::CameraHasMouse=false;

   if(GetCursor()==NULL)
   SetCursor(SavedCursor);
}
}


