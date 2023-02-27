#ifndef GameShader_H
#define GameShader_H
#include <fstream>
#include <sstream>
#include <GameMath/GameMath.h>
#include <GameWindow/GameWindow.h>
#include <GameWindow/glad/glad.h>
#include <GameWindow/KHR/khrplatform.h>
class GameShader
{
public:
unsigned int ID;
GameMath::Matrix4x4 nullmatrix;

void LoadShader(const char* vertexPath, const char* fragmentPath,const char* geometryPath);
void Use() const;
void setBool(const std::string &name, bool value) const;
void setInt(const std::string &name, int value) const;
void setFloat(const std::string &name, float value) const;
void setMatrix4x4(const std::string &name, GameMath::Matrix4x4 value) const;
void setMatrix4x4(const std::string &name);
private:
bool checkCompileErrors(unsigned int shader, std::string type) const;
};
#endif
