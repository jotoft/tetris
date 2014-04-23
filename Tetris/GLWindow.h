#pragma once
#include <Windows.h>
#include "Input.h"

class GLWindow
{
public:
	GLWindow(void);
	GLWindow(HINSTANCE hInstance);
	~GLWindow(void);
	bool create(int width, int height, int bpp, bool fullscreen, bool vsync);
	bool running();
	void processEvents();
	float getElapsedTime();
    void swapBuffers() { SwapBuffers(hdc); }
	void setInput(Input *input) {this->input = input;}
private:
	WNDCLASSEX windowClass;
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void setupPixelFormat(void);
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HINSTANCE hInstance;
	HWND hWnd;
	HDC hdc;
	HGLRC hglrc;
	bool isRunning;
	bool vsync;
	LARGE_INTEGER lastCount;
	Input *input;
};

