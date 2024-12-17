#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>


void display() {
    // rotate a triangle around 
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,  1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
    glFlush();
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    // create window class
    WNDCLASS wc = { };
    wc.style = CS_OWNDC; // each window gets unique device context 
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // create window
    HWND hwnd = CreateWindowEx(
        0, // optional window style
        CLASS_NAME, // name of window class
        L"demo", // window text (title)
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // window style

        CW_USEDEFAULT, // x
        CW_USEDEFAULT, // y
        400, // w
        300, // h

        NULL, // parent window    
        NULL, // menu
        hInstance, // instance handle
        NULL // additional data
        );

    // exit if failed to create window
    if (hwnd == NULL) {
        return 0;
    }

    HDC hDC = GetDC(hwnd); // get the device context

    PIXELFORMATDESCRIPTOR pfd = { }; // make pixel format descriptor struct
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1; // should always be 1
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; // draw to window, suppoer opengl
    pfd.iPixelType = PFD_TYPE_RGBA; // rgba colors
    pfd.cColorBits = 32; // docs say should be as many bits as color channel exlcuding alpha (24, but ex say 32?)

    int pf = ChoosePixelFormat(hDC, &pfd); // choose compatible pixel format and store it as pf
    SetPixelFormat(hDC, pf, &pfd); // set the pixel format of the device context to be the chosen pixel format

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd); // sets the members of pfd to match the pixel format and device context

    // create opengl render context
    HGLRC hRC = wglCreateContext(hDC);

    // make the opengl render context the rendering context of the current thread
    wglMakeCurrent(hDC, hRC);

    // make window visible
    ShowWindow(hwnd, nCmdShow);


    // message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // release resources and clean up

    wglMakeCurrent(NULL, NULL); // make hRC no longer the render context of the thread
    ReleaseDC(hwnd, hDC); // release the device context
    wglDeleteContext(hRC); // release the opengl rendering context

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        case WM_PAINT:
            {
                PAINTSTRUCT ps;

                display();

                BeginPaint(hwnd, &ps);
                EndPaint(hwnd, &ps);
            }
            return 0;
        case WM_SIZE:
            {
                glViewport(0, 0, LOWORD(lParam), HIWORD(lParam)); // might break things????
                PostMessage(hwnd, WM_PAINT, 0, 0);
            }
            return 0;
        case WM_LBUTTONDOWN:
            {
                RECT bounds;
                GetWindowRect(hwnd, &bounds);

                // x, y, width, height
                MoveWindow(hwnd, bounds.left-10, bounds.top, 400, 300, TRUE);
            }
            return 0;
        case WM_RBUTTONDOWN:
            {
                RECT bounds;
                GetWindowRect(hwnd, &bounds);

                // x, y, width, height
                MoveWindow(hwnd, bounds.left+10, bounds.top, 400, 300, TRUE);
            }
            return 0;
        case WM_MOUSEWHEEL:
            {
                RECT bounds;
                GetWindowRect(hwnd, &bounds);

                // x, y, width, height
                MoveWindow(hwnd, bounds.left, bounds.top+GET_WHEEL_DELTA_WPARAM(wParam)/-12, 400, 300, TRUE);
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}