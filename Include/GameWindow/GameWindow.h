#ifndef GameWindow_H
#define GameWindow_H
#include <stdio.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include <GameWindow/KHR/KHRPlatform.h>
#include <GameWindow/glad/glad.h>
#include <map>
//Macro to check the GameWindow Initialize Method Status
#define CHECKSTATUS if(GameWindow::Status!=ERR::OK)return;
class GameWorldCube;
class GameBoundingBox;
class GameDisplayCube;
class GameWindow
{
public:
static void Initialize(std::string WindowName,int X,int Y,int Width,int Height);
static int X;
static int Y;
static int Width;
static int Height;
static int Time_Taken_Per_Frame;
static int Pixel_R_Index;
static int Pixel_G_Index;
static int Pixel_B_Index;
static bool WindowHasMoved;
static bool WindowHasResized;
static bool ViewPortChanged;
static bool KeepAlive;
static bool HasFocus;
static bool CameraHasMouse;
static char KeyCode;
static bool KeyPressed;
static bool MouseLButtonPressed;
static bool MouseRButtonPressed;
static bool MouseMButtonPressed;
static DWORD MouseTimer;
static HWND WindowHandle;
static HDC DeviceContext;
static std::string WindowName;
static std::string CWD;
static std::wstring WWindowName;
struct Pixel
{
int type=0;//default type =0
void *payload;
};
static std::map<std::string,GameWindow::Pixel*> PixelMap;
static void PixelMapping(GameWorldCube* payload);
static void PixelMapping(GameBoundingBox* payload);
static void GenerateKey(Pixel *pixel,float* Rf,float* Gf,float* Bf);
static std::string ReadKey(int R,int G,int B);
static bool GetStatus();


private:
enum class ERR
{
OK,
NO_HMODULE_FOUND,
WNDCLASS_NOT_REGISTERED,
WNDHANDLE_NOT_CREATED,
COULD_NOT_CHOOSE_PIXEL_FORMAT,
COULD_NOT_SET_PIXEL_FORMAT,
COULD_NOT_CREATE_OPENGL_CONTEXT,
COULD_NOT_SET_OPENGL_CONTEXT,
COULD_NOT_LOAD_GLAD,
EVENT_NOT_FOUND
};
static GameWindow::ERR Status;
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void GetHModule(HMODULE &hmodule);
static void RegisterClassCustom(HMODULE &hmodule);
static void CreateWnd(HMODULE &hmodule);
static void ChoosePixFormat(int &pixelformat,PIXELFORMATDESCRIPTOR &pfd,HDC &dc);
static void SetPixFormat(int &pixelformat,PIXELFORMATDESCRIPTOR &pfd,HDC &dc);
static void CreateOpenGLContext(HGLRC &rc,HDC &dc);
static void SetOpenGLContext(HGLRC &rc,HDC &dc);
static void LoadGlad();
};
#endif
