#include "GLWindow.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>
#include "Input.h"
#include <WinUser.h>


void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  char msgbuf[250];
  sprintf( msgbuf, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
           type, severity, message );
  std::cout << msgbuf << std::endl;

}

GLWindow::GLWindow(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	isRunning = false;
	QueryPerformanceCounter(&lastCount);
}


GLWindow::~GLWindow(void)
{
}


bool GLWindow::create(int width, int height, int bpp, bool fullscreen, bool vsync)
{
	this->vsync = vsync;

	this->windowClass.cbSize = sizeof(WNDCLASSEX);
	this->windowClass.style = CS_HREDRAW | CS_VREDRAW;
	this->windowClass.lpfnWndProc = GLWindow::StaticWndProc;
	this->windowClass.cbClsExtra = 0;
	this->windowClass.cbWndExtra = 0;
	this->windowClass.hInstance = this->hInstance;
	this->windowClass.hIcon = (HICON) LoadIcon(0, IDI_APPLICATION );
	this->windowClass.hIconSm = 0;
	this->windowClass.hCursor = (HCURSOR) LoadCursor(0, IDI_APPLICATION );
	this->windowClass.hbrBackground = 0;
	this->windowClass.lpszMenuName = 0;
	this->windowClass.lpszClassName = 0;
	this->windowClass.lpszClassName = "GLWindowClass";

	if(!RegisterClassEx(&windowClass))
	{
		MessageBox(NULL, "Failed to register window class", NULL, MB_OK);
		return false;
	}

	this->hWnd = CreateWindowEx(NULL, //ExtendedStyle
		"GLWindowClass", //class name
		"Tetris", //window name
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0, 0, //x y
		width, height,
		NULL, //parent
		NULL, //menu
		hInstance, // Application instance
		this); // pointer to this class

	if(!hWnd)
		return false;
	
	this->hdc = GetDC(hWnd);
	ShowWindow(this->hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

LRESULT CALLBACK GLWindow::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GLWindow* window = NULL;

	if(uMsg == WM_CREATE)
	{
		window = (GLWindow*) ((LPCREATESTRUCT)lParam)->lpCreateParams;

		//Lagra pekare till glwindowklassen i fonstrets userdata
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
	}

	window = (GLWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(!window)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	return window->WndProc(hWnd, uMsg, wParam, lParam);
}

void GLWindow::setupPixelFormat(void)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER,           // support double-buffering
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

    pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);
}

LRESULT GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
		hdc = GetDC(hWnd);
		setupPixelFormat();

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, 0 };

		//Skapa opengl context
		HGLRC tmpContext = wglCreateContext(hdc);
		wglMakeCurrent(hdc, tmpContext);

		//Initiera glew, m�ste ha ett opengl context f�r att kunna g�ra det!
		glewExperimental = GL_TRUE;
		glewInit();
        // During init, enable debug output
        glEnable              ( GL_DEBUG_OUTPUT );
        glDebugMessageCallback( MessageCallback, 0 );

		//wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress

		if(!wglCreateContextAttribsARB)
		{
			std::cerr << "OpenGL 3.0 is not supported falling back to GL 2.1" << std::endl;
			hglrc = tmpContext;
		}
		else
		{
			hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
			wglDeleteContext(tmpContext);
		}

		wglMakeCurrent(hdc, hglrc);

		if(vsync)
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);

		isRunning = true;
		}
	break;
	case WM_DESTROY:
	case WM_CLOSE:
		//wglMakeCurrent(hdc, NULL);
		//wglDeleteContext(hglrc);
		isRunning = false;
		return 0;
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case VK_LEFT:
			input->keyDown(KEY_LEFT);
			break;
		case VK_RIGHT:
			input->keyDown(KEY_RIGHT);
			break;
		case VK_UP:
			input->keyDown(KEY_UP);
			break;
		case VK_DOWN:
			input->keyDown(KEY_DOWN);
			break;
		case 0x41: //A-key
			input->keyDown(KEY_LEFT);
			break;
		case 0x44: //D-key
			input->keyDown(KEY_RIGHT);
			break;
		case 0x53: //S-key
			input->keyDown(KEY_DOWN);
			break;
		case 0x51: //Q-key
			input->keyDown(KEY_ROTATECC);
			break;
		case 0x45: //E-key
			input->keyDown(KEY_ROTATECLOCK);
			break;
	
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool GLWindow::running()
{
	return isRunning;
}

void GLWindow::processEvents()
{
    MSG msg;

    //While there are messages in the queue, store them in msg
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //Process the messages one-by-one
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

float GLWindow::getElapsedTime()
{
	LARGE_INTEGER freq;
	LARGE_INTEGER count;
	LARGE_INTEGER diff;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&count);
	
	diff.QuadPart = count.QuadPart - lastCount.QuadPart;
	
	float elapsedTime = (double)diff.QuadPart / (freq.QuadPart);

	lastCount.QuadPart = count.QuadPart;

	return elapsedTime;
}