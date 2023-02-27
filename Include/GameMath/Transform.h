#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED
#define PI 3.14159265
class GameMath::Transform
{
public:
static float GetDegree(const float &AngleinRadian);
static float GetRadian(const float &AngleinDegree);
static void CopyMatrixtoArray(GameMath::Matrix4x4 *mat,float *output);

static GameMath::Matrix4x4 Scale(const GameMath::Vector3 &ScaleVector);
static GameMath::Matrix4x4 Scale(const float &x,const float &y,const float &z);

static GameMath::Matrix4x4 Translate(const GameMath::Vector3 &TranslateVector);
static GameMath::Matrix4x4 Translate(const float &x,const float &y,const float &z);

static GameMath::Matrix4x4 Rotate(const char &Axis, const float &AngleOfRotationinDegrees);
static GameMath::Matrix4x4 RotateQ(char axis, const float &AngleOfRotationinDegrees);
static GameMath::Matrix4x4 RotateQ(GameMath::Vector3 Axis, const float &AngleOfRotationinDegrees);
static GameMath::Matrix4x4 RotateQ(float w,float x,float y,float z);

static GameMath::Matrix4x4 PerspectiveView(const float AspectRatio,const float &Near,const float &Far, const float &AngleofViewinDegrees);
static GameMath::Matrix4x4 PerspectiveView(float Top,float Bottom,float Right,float Left,float Near,float Far);
static GameMath::Matrix4x4 LookAt(const float &YawAngleinDegrees,const float &PitchAngleinDegrees,const GameMath::Vector3 &CameraPosition);
};
#endif // TRANSFORM_H_INCLUDED
