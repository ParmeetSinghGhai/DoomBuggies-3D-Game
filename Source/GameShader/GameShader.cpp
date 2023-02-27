#include <GameShader/GameShader.h>
/*************************************************
THIS FUNCTION SIMPLY READS THE VERTEX, FRAGMENT AND GEOMETRY SHADER CODE FROM TEXT FILES AND
CONVERTS INTO CONST CHAR * SO THAT CODE CAN BE USED IN OPENGL FUNCTION : glShaderSource() THAT ASSIGNS CODE TO THE SHADER

THE BASIC PROCEDURE IS AS FOLLOWS:

1) USE THE OPENGL FUNCTION: glCreateProgram() TO CREATE A SHADER PROGRAM WITH A UNIQUE ID

2) USE OPENGL FUNCTION: glCreateShader() TO EMPTY SHADER OBJECT OF TYPE : GL_COMPUTE_SHADER, GL_VERTEX_SHADER,
   GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.

3) USE OPENGL FUNCTION: glShaderSource() TO LOAD THE SHADER CODE TO THE ABOVE CREATED EMPTY SHADER OBJECT

4) USE OPENGL FUNCTION: glCompileShader() TO COMPILE THAT SHADER

4) IF COMPILATION IS SUCCESSFUL THEN USE OPENGL FUNCTION: glAttachShader() TO ATTACH THE SHADER TO THE SHADER PROGRAM

5) FINALLY USE THE OPENGL FUNCTION: glLinkProgram() TO LINK THE PROGRAM OBJECT SPECIFIED BY THE PROGRAM
   If any shader objects of type GL_VERTEX_SHADER are attached to program, they will be used to create an executable
   that will run on the programmable vertex processor

6) OPENGL FUNCTION: glUseProgram() TO USED TO ACTIVATE THE SHADER PROGRAM CREATED ABOVE BY SPECIFYING ITS UNIQUE ID
   THIS ALLOWS MULTIPLE SHADER PROGRAMS TO BE CREATED AND USED

7) OPENGL FUNCTION: glUniform() IS USED TO SPECIFY THE VALUE OF A UNIFORM VARIABLE USED BY A SHADER PROGRAM

8) OPENGL FUNCTION: glGetUniformLocation() RETURNS THE LOCATION OF A UNIFORM VARIABLE USED BY A SHADER PROGRAM
**************************************************/
void GameShader::LoadShader(const char* vertexPath, const char* fragmentPath,const char* geometryPath)
{
std::string vertexCode;
std::string fragmentCode;
std::string geometryCode;
std::ifstream vShaderFile;
std::ifstream fShaderFile;
std::ifstream gShaderFile;
vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
try
{
    std::stringstream vShaderStream;
    if(vertexPath!=nullptr)
    {
        vShaderFile.open(vertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode   = vShaderStream.str();
    }
}
catch (std::ifstream::failure e)
{
    std::cout << "ERROR::SHADER::VERTEX SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
}

try
{
    std::stringstream fShaderStream;
    if(fragmentPath!=nullptr)
    {
        fShaderFile.open(fragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
}
catch (std::ifstream::failure e)
{
    std::cout << "ERROR::SHADER::FRAGMENT SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
}

try
{
    std::stringstream gShaderStream;
    if(geometryPath!=nullptr)
    {
        gShaderFile.open(geometryPath);
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();
    }
}
catch (std::ifstream::failure e)
{
    std::cout << "ERROR::SHADER::GEOMETRY SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
}

const char* vShaderCode = vertexCode.c_str();
const char * fShaderCode = fragmentCode.c_str();
const char * gShaderCode = geometryCode.c_str();

unsigned int vertex, fragment, geometry=-1;
ID = glCreateProgram();

//Vertex shader
if(vertexPath!=nullptr)
{
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    if(checkCompileErrors(vertex, "VERTEX")==false)
    glAttachShader(ID, vertex);
}
//Fragment Shader
if(fragmentPath!=nullptr)
{
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    if(checkCompileErrors(fragment, "FRAGMENT")==false)
    glAttachShader(ID, fragment);
}

//Geometry Shader
if(geometryPath!=nullptr)
{
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    if(checkCompileErrors(geometry, "GEOMETRY")==false)
    glAttachShader(ID, geometry);
}

//Shader Program
glLinkProgram(ID);
if(checkCompileErrors(ID, "PROGRAM")==false)
std::cout<<"\nSUCESS::SHADER_COMPILATION_COMPLETED\n";
else
std::cout<<"\nERROR::SHADER_COMPILATION_FAILED\n";

glDeleteShader(vertex);
glDeleteShader(fragment);
glDeleteShader(geometry);
}

bool GameShader::checkCompileErrors(unsigned int shader, std::string type) const
{
int success;
char infoLog[1024];
if (type != "PROGRAM")
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<"\n ------------------------------------------------------- " << std::endl;
    return true;
    }
}
else
{
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------------------- " << std::endl;
    return true;
    }
}
return false;
}
void GameShader::Use() const
{
    glUseProgram(ID);
}
void GameShader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void GameShader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void GameShader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void GameShader::setMatrix4x4(const std::string &name, GameMath::Matrix4x4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.Addressof());
}
void GameShader::setMatrix4x4(const std::string &name)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, nullmatrix.Addressof());
}

