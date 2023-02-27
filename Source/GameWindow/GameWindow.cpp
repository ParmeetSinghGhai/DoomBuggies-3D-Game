#include <GameWindow/GameWindow.h>
int GameWindow::X;
int GameWindow::Y;
int GameWindow::Width;
int GameWindow::Height;
int GameWindow::Time_Taken_Per_Frame=16;
int GameWindow::Pixel_R_Index=0;
int GameWindow::Pixel_G_Index=0;
int GameWindow::Pixel_B_Index=1;
bool GameWindow::KeepAlive;
bool GameWindow::WindowHasResized;
bool GameWindow::WindowHasMoved;
bool GameWindow::ViewPortChanged=true;
bool GameWindow::HasFocus;
bool GameWindow::CameraHasMouse;
bool GameWindow::KeyPressed;
char GameWindow::KeyCode;
bool GameWindow::MouseLButtonPressed;
bool GameWindow::MouseRButtonPressed;
bool GameWindow::MouseMButtonPressed;
DWORD GameWindow::MouseTimer;
HWND GameWindow::WindowHandle;
HDC GameWindow::DeviceContext;
GameWindow::ERR GameWindow::Status;
std::string GameWindow::WindowName;
std::string GameWindow::CWD;
std::wstring GameWindow::WWindowName;
std::map<std::string,GameWindow::Pixel*> GameWindow::PixelMap;

/*************************************************
A GUI APPLICATION RECEIVES EVENTS FROM THE USER AND THE OPERATING AND IT MUST RESPOND TO ALL OF THEM AND
IT DOES WITH THE HELP OF "MESSAGES" WHICH ARE BASICALLY NUMERIC VALUES ASSOCIATE TO THESE EVENTS.

AN APPLICATION WILL RECEIVE THOUSANDS OF EVENTS WHILE IT RUNS WHERE EACH KEYSTORE AND MOUSE BUTTON GENERATES A MESSAGE
FOR EACH THREAD THAT CREATES A WINDOW, THE OPERATING SYSTEM CREATES A QUEUE FOR WINDOW MESSAGE
YOU CAN PULL A MESSAGE FROM THE QUEUE BY CALLING THE getmessage() FUNCTION OR PeekMessage() FUNCTION

IMPORTANT FUNCTIONS USED IN THE MESSAGE LOOP ARE AS FOLLOWS:

1) getmessage() - THIS FUNCTION REMOVES THE FIRST MESSAGE FROM THE HEAD OF THE QUEUE.
               IF THE QUEUE IS EMPTY, THE FUNCTION BLOCKS UNTIL ANOTHER MESSAGE IS QUEUED

2) peekmessage()- PEEKMESSAGE RETURNS IMMEDIATELY REGARDLESS OF WHETHER A MESSAGE IS IN THE QUEUE SO IT DOES NOT BLOCK

3) TranslateMessage()- TRANSLATES KEYSTROKES (KEY DOWN, KEY UP) INTO CHARACTERS.

4) dispatchmessage()- TELLS THE OPERATING SYSTEM TO CALL THE WINDOW PROCEDURE OF THE WINDOW THAT IS THE TARGET OF THE MESSAGE.

5) postquitmessage()- PUTS A WM_QUIT MESSAGE ON THE MESSAGE QUEUE. WM_QUIT IS A SPECIAL MESSAGE
                   IT CAUSES GETMESSAGE TO RETURN ZERO, SIGNALING THE END OF THE MESSAGE LOOP

6) PostMessageW() - PLACES (POSTS) A MESSAGE IN THE MESSAGE QUEUE ASSOCIATED WITH THE THREAD THAT CREATED THE SPECIFIED WINDOW
                 AND RETURNS WITHOUT WAITING FOR THE THREAD TO PROCESS THE MESSAGE.

7) DefWindowProc()- THIS FUNCTION PERFORMS THE DEFAULT ACTION FOR THE MESSAGE, WHICH VARIES BY MESSAGE TYPE.

8) clienttoscreen()- FUNCTION CONVERTS THE CLIENT-AREA COORDINATES OF A SPECIFIED POINT TO SCREEN COORDINATES

IF YOU DON'T HANDLE A PARTICULAR MESSAGE IN YOUR WINDOW PROCEDURE, PASS THE MESSAGE PARAMETERS DIRECTLY TO THE DEFWINDOWPROC FUNCTION.
FOR EXAMPLE, SUPPOSE THAT THE USER PRESSES THE LEFT MOUSE BUTTON. THIS CAUSES A CHAIN OF EVENTS:

1) THE OPERATING SYSTEM PUTS A WM_LBUTTONDOWN MESSAGE ON THE MESSAGE QUEUE.
2) YOUR PROGRAM CALLS THE GETMESSAGE FUNCTION.
3) GETMESSAGE PULLS THE WM_LBUTTONDOWN MESSAGE FROM THE QUEUE AND FILLS IN THE MSG STRUCTURE.
4) YOUR PROGRAM CALLS THE TRANSLATEMESSAGE AND DISPATCHMESSAGE FUNCTIONS.
5) INSIDE DISPATCHMESSAGE, THE OPERATING SYSTEM CALLS YOUR WINDOW PROCEDURE.
6) YOUR WINDOW PROCEDURE CAN EITHER RESPOND TO THE MESSAGE OR IGNORE IT.
*************************************************/
LRESULT CALLBACK GameWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    case WM_PAINT:
    return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    case WM_SETFOCUS:
        HasFocus=true;
        break;
    case WM_KILLFOCUS:
        HasFocus=false;
        break;
    case WM_SIZE:
	    {
            WindowHasResized=true;
	        PostMessageW(hWnd, WM_USER + 1, wParam, lParam);
	    }
	    break;
    case WM_KEYDOWN:
        {
            KeyPressed = true;
            KeyCode = (char)wParam;
        }
        break;
    case WM_KEYUP:
        {
            KeyPressed = false;
            KeyCode = '/0';
        }
        break;
  	case WM_MOVE:
        {
            WindowHasMoved=true;
            PostMessageW(hWnd, WM_USER + 1, wParam, lParam);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            MouseTimer=GetTickCount();
            MouseLButtonPressed=true;
        }
        break;
    case WM_LBUTTONUP:
        {
            MouseTimer=0;
            MouseLButtonPressed=false;
        }
        break;
    case WM_RBUTTONDOWN:
        MouseRButtonPressed=true;
        break;
    case WM_RBUTTONUP:
        MouseRButtonPressed=false;
        break;
    case WM_MBUTTONDOWN:
        MouseMButtonPressed = true;
        break;
    case WM_MBUTTONUP:
        MouseMButtonPressed = false;
        break;
    case WM_USER + 1:
        {
        RECT rc;
        GetClientRect(WindowHandle,&rc);
        POINT pt = { rc.left, rc.top };
        POINT pt2 = { rc.right, rc.bottom };
        ClientToScreen(WindowHandle, &pt);
        ClientToScreen(WindowHandle, &pt2);
        SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);
        Width = rc.right - rc.left;
	    Height = rc.bottom - rc.top;
        X = rc.left;
	    Y = rc.top;
        glViewport(0,0,Width,Height);
        ViewPortChanged=true;
	    break;
        }
    case WM_CLOSE:
    case WM_DESTROY:
	PostQuitMessage(0);
    break;
    default:
    return DefWindowProc(hWnd, message, wParam, lParam);
    break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
bool GameWindow::GetStatus()
{
    switch(GameWindow::Status)
    {
        case ERR::OK:
        std::cout<<"STATUS::OK";
        return true;
        break;

        case ERR::NO_HMODULE_FOUND:
        std::cout<<"STATUS::NO HMODULE FOUND";
        return false;
        break;

        case ERR::WNDCLASS_NOT_REGISTERED:
        std::cout<<"STATUS::COULD NOT REGISTER WINDOWS CLASS";
        return false;
        break;

        case ERR::WNDHANDLE_NOT_CREATED:
        std::cout<<"STATUS::COULD NOT CREATE WINDOW";
        return false;
        break;

        case ERR::COULD_NOT_CHOOSE_PIXEL_FORMAT:
        std::cout<<"STATUS::COULD_NOT_CHOOSE_PIXEL_FORMAT";
        return false;
        break;

        case ERR::COULD_NOT_SET_PIXEL_FORMAT:
        std::cout<<"STATUS::COULD_NOT_SET_PIXEL_FORMAT";
        return false;
        break;

        case ERR::COULD_NOT_CREATE_OPENGL_CONTEXT:
        std::cout<<"STATUS::COULD_NOT_CREATE_OPENGL_CONTEXT";
        return false;
        break;

        case ERR::COULD_NOT_SET_OPENGL_CONTEXT:
        std::cout<<"STATUS::COULD_NOT_SET_OPENGL_CONTEXT";
        return false;
        break;

        case ERR::COULD_NOT_LOAD_GLAD:
        std::cout<<"STATUS::COULD_NOT_LOAD_GLAD";
        return false;
        break;

        case ERR::EVENT_NOT_FOUND:
        std::cout<<"STATUS::EVENT_NOT_FOUND";
        return false;
        break;

        default:
        std::cout<<"STATUS NOT FOUND";
        return true;
    }
}
void GameWindow::GetHModule(HMODULE &hmodule)
{
    hmodule=GetModuleHandle(NULL);
    if(hmodule==NULL)
      GameWindow::Status=ERR::NO_HMODULE_FOUND;
    else
      GameWindow::Status=ERR::OK;
}
void GameWindow::RegisterClassCustom(HMODULE &hmodule)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = GameWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hmodule;
    wcex.hIcon = LoadIcon(hmodule, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = GameWindow::WWindowName.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
	GameWindow::Status=ERR::WNDCLASS_NOT_REGISTERED;
    else
	GameWindow::Status=ERR::OK;
}
void GameWindow::CreateWnd(HMODULE &hmodule)
{
    HWND whandle = CreateWindow(GameWindow::WWindowName.c_str(), GameWindow::WWindowName.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, GameWindow::X, GameWindow::Y, GameWindow::Width, GameWindow::Height, NULL, NULL, hmodule, NULL);
    if(whandle==NULL)
    {
        GameWindow::Status=ERR::WNDHANDLE_NOT_CREATED;
        GameWindow::WindowHandle=NULL;
    }
    else
    {
        GameWindow::Status=ERR::OK;
        GameWindow::WindowHandle=whandle;
    }
}
void GameWindow::ChoosePixFormat(int &pixelformat,PIXELFORMATDESCRIPTOR &pfd,HDC &dc)
{
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pixelformat = ChoosePixelFormat(dc, &pfd);
    if (pixelformat == 0)
	GameWindow::Status=ERR::COULD_NOT_CHOOSE_PIXEL_FORMAT;
    else
	GameWindow::Status=ERR::OK;
}
void GameWindow::SetPixFormat(int &pixelformat,PIXELFORMATDESCRIPTOR &pfd,HDC &dc)
{
    if (!SetPixelFormat(dc, pixelformat, &pfd))
	GameWindow::Status=ERR::COULD_NOT_SET_PIXEL_FORMAT;
    else
	GameWindow::Status=ERR::OK;
}
void GameWindow::CreateOpenGLContext(HGLRC &rc,HDC &dc)
{
    rc=wglCreateContext(dc);
    if (rc == NULL)
	GameWindow::Status=ERR::COULD_NOT_CREATE_OPENGL_CONTEXT;
    else
	GameWindow::Status=ERR::OK;
}
void GameWindow::SetOpenGLContext(HGLRC &rc,HDC &dc)
{
    if (!wglMakeCurrent(dc, rc))
        GameWindow::Status=ERR::COULD_NOT_SET_OPENGL_CONTEXT;
    else
	GameWindow::Status=ERR::OK;
}
void GameWindow::LoadGlad()
{
    if(!gladLoadGL())
	GameWindow::Status=ERR::COULD_NOT_LOAD_GLAD;
    else
	GameWindow::Status=ERR::OK;
}
/*************************************************
THIS FUNCTIONS CREATE A WINDOW THAT IS USED TO RENDER OPENGL GRAPHICS. THE PROCESS USED IS AS FOLLOWS:

1) USE THE WINDOWS FUNCTION GetModuleHandle(NULL) TO GET THE HARDWARE MODULE OF THE CONSOLE WINDOW
   THAT IS CREATED FIRST SINCE THIS IS A CONSOLE APPLICATION

2) DECLARE VARIABLE OF TYPE WINDOWS CLASS STRUCTURE "WNDCLASSEX". A WINDOW CLASS DEFINES A SET OF BEHAVIOURS THAT
   SEVERAL WINDOWS MIGHT HAVE IN COMMON. EVERY WINDOW MUST BE ASSOCIATED WITH A WINDOW CLASS.
   SETUP VARIOUS PROPERTIES IN THE "WNDCLASSEX" WINDOW CLASS STRUCTURE.
   THE IMPORTANT PROPERTIES TO DEFINE ARE:

     A) "lpszClassName" IS THE STRING THAT UNIQUELY INDENTIFIES THE CLASS
     B) "lpfnWndProc" IS A POINTER TO THE APPLICATION-DEFINED FUNCTION CALLED THE WINDOWS PROCEDURE OR "WINDOW PROC"

   AFTER THAT USE THE RegisterClassEx() TO REGISTER THIS NEWLY CREATED CLASS.

3) USE THE FUNCTION : CreateWindow() BY PASSING ALONG TO IT THE NEWLY CREATED CLASS NAME ALONG WITH THE HMODULE OF THE
   CONSOLE WINDOW AMONG OTHER PARAMETERS TO CREATE A NEW WINDOW.
   THIS FUNCTION RETURNS THE WINDOW HANDLE "HWND" OF THE NEWLY CREATED WINDOW

4) USE THE GetDC() FUNCTION TO RETREIVE THE HANDLE TO THE DEVICE CONTEXT TYPE: "HDC" FOR THE CLIENT AREA OF THE NEWLY CREATED WINDOW

5) SETUP A "PIXELFORMATDESCRIPTOR" STRUCTURE VARIABLE AND USE THE FUNCTION: ChoosePixelFormat() BY PASSING TO IT THE
   "HDC" DEVICE CONTEXT OBTAINED FOR THE NEWLY CREATED WINDOW. IT RETURNS A INTEGER VALUE REPRESENTING A PIXEL FORMAT INDEX
   THAT IS THE CLOSEST MATCH TO THE "PIXELFORMATDESCRIPTOR" VARIABLE PASSED ONTO THIS FUNCTION

6) PASS ON THE PIXEL FORMAT INDEX GIVEN OUT BY ChoosePixelFormat() TO THE FUNCTION: SetPixFormat() TO SETUP THE PIXEL
   FORMAT FOR THE DEVICE CONTEXT OF THE NEW WINDOW

7) USE THE FUNCTION: wglCreateContext() TO CREATE A NEW OPENGL RENDERING CONTEXT WHICH IS SUITABLE FOR DRAWING ON THE DEVICE
   REFERENCED BY DEVICE CONTEXT OF THE NEWLY CREATED WINDOW. IT RETURNS A HANDLE TO OPENGL RENDERING CONTEXT "HGLRC"

8) USE THE FUNCTION: wglMakeCurrent() TO SET THE OPENGL RENDERING CONTEXT "HGLRC" OBTAINED ABOVE THE CURRENT RENDERING CONTEXT
   DOING SO WILL MAKE SURE THAT ALL FUTURE OPENGL DRAW COMMANDS WILL RENDER TO THIS OPENGL RENDERING CONTEXT THAT IS LINKED
   TO THE DEVICE CONTEXT OF THE NEW CREATED WINDOW

9) USE GLAD OPENGL LOADING LIBRARY FUNCTION: gladLoadGL() TO LOAD POINTERS TO THE OPENGL FUNCTIONS AT RUNTIME

10) USE THE FUNCTION: ShowWindow() TO SHOW THE NEWLY CREATED WINDOW

11) USE THE FUNCTION: UpdateWindow() TO SEND A WM_PAINT MESSAGE TO THE WINDOWS PROC FUNCTION


SINCE OPENGL IS A STANDARD/SPECIFICATION IT IS UP TO THE DRIVER MANUFACTURER TO IMPLEMENT THE SPECIFICATION TO A DRIVER THAT THE
SPECIFIC GRAPHICS CARD SUPPORTS. SINCE THERE ARE MANY DIFFERENT VERSIONS OF OPENGL DRIVERS, THE LOCATION OF
MOST OF ITS FUNCTIONS IS NOT KNOWN AT COMPILE-TIME AND NEEDS TO BE QUERIED AT RUN-TIME. IT IS THEN THE TASK OF
THE DEVELOPER TO RETRIEVE THE LOCATION OF THE FUNCTIONS HE/SHE NEEDS AND STORE THEM IN FUNCTION POINTERS FOR
LATER USE. RETRIEVING THOSE LOCATIONS IS OS-SPECIFIC

GLAD IS AN OPEN SOURCE LIBRARY THAT RETRIEVES THE LOCATION OF THE OPENGL FUNCTIONS AND STORE THEM IN FUNCTION POINTERS FOR LATER USE
AND IS GENERATED VIA WEB SERVICE : https://glad.dav1d.de/
*************************************************/
void GameWindow::Initialize(std::string WindowName,int X,int Y,int Width,int Height)
{
    //step 0: declare local variables
    HMODULE hmodule;
    int pixelformat;
    PIXELFORMATDESCRIPTOR pfd;
    HDC dc;
    HGLRC rc;

    //Step 1: Get Hardware Module for the console Window
    GameWindow::GetHModule(hmodule);
    CHECKSTATUS;
    GameWindow::WindowName=WindowName;
    GameWindow::WWindowName = std::wstring(GameWindow::WindowName.begin(), GameWindow::WindowName.end());
    GameWindow::X=X;
    GameWindow::Y=Y;
    GameWindow::Width=Width;
    GameWindow::Height=Height;

    //Step 2: Register Window class
    GameWindow::RegisterClassCustom(hmodule);
    CHECKSTATUS;

    //Step 3: Create the Graphics Window
    GameWindow::CreateWnd(hmodule);
    CHECKSTATUS;
    dc = GetDC(GameWindow::WindowHandle);

    //Step 4: Choose Pixel Format
    GameWindow::ChoosePixFormat(pixelformat,pfd,dc);
    CHECKSTATUS;

    //Step 5: Set Pixel Format
    GameWindow::SetPixFormat(pixelformat,pfd,dc);
    CHECKSTATUS;

    //Step 6: Create OpenGL context
    GameWindow::CreateOpenGLContext(rc,dc);
    CHECKSTATUS;

    //Step 7: Set Current OpenGL context
    GameWindow::SetOpenGLContext(rc,dc);
    CHECKSTATUS;
    GameWindow::DeviceContext=dc;

    //Step 8: Load GLAD
    GameWindow::LoadGlad();
    CHECKSTATUS;

    //Step 9: Show and Update Window
    ShowWindow(GameWindow::WindowHandle, SW_SHOWNORMAL);
    UpdateWindow(GameWindow::WindowHandle);

    //Step 10: Set Initial Parameters
    KeepAlive=true;
    WindowHasResized=false;
    WindowHasMoved=false;
    HasFocus=true;
}

