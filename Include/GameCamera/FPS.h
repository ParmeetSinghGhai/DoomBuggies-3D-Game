#ifndef GAMECAMERA_FPS_H_INCLUDED
#define GAMECAMERA_FPS_H_INCLUDED
#include <GameSettings/GameSettings.h>
class GameCamera::FPS
{
public:
float KeyboardSensitivity=GameSettings::CameraKeyBoardSensitivity;
float MouseSensitivity=GameSettings::CameraMouseSensitivity;
float PitchAngle=0.0f;
float YawAngle=0.0f;

GameMath::Matrix4x4 View;
GameMath::Vector3 CameraPosition=GameMath::Vector3(0.0f,0.0f,5.0f);
GameMath::Vector3 Front=GameMath::Vector3(0.0f,0.0f,-1.0f);
GameMath::Vector3 Right=GameMath::Vector3(1.0f,0.0f,0.0f);
GameMath::Vector3 Up=GameMath::Vector3(0.0f,1.0f,0.0f);
void UpdateCameraView();


private:
void MouseTracking();
int LastX=0;
int LastY=0;
int MouseOffsetX=0;
int MouseOffsetY=0;
POINT p;
HCURSOR SavedCursor;
bool FirstMouse=true;
int CameraHasMouse_ActivationTime=100;
};
#endif // GAMECAMERA_FPS_H_INCLUDED
