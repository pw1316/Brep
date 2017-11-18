#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "brep.hpp"

/* Windows parameters */
CHAR appName[] = "BRep OpenGL";
HWND  g_hWnd;
HDC   g_hDC;
HGLRC g_hGLRC;

#define SWAPBUFFERS SwapBuffers(g_hDC)
#define APP_WIDTH 800 
#define APP_HEIGHT 600

#define FACEMODE 0
#define LINEMODE 1
INT renderMode = FACEMODE;

GLfloat eyeX = -5;
GLfloat eyeY = 1.5;
GLfloat eyeZ = 0.5;
GLfloat yaw = 0;
GLfloat pitch = 0;
bool RDown = false;
INT mouseX;
INT mouseY;

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

/* OpenGL Code */
GLvoid gl_resize_cb(GLsizei, GLsizei);
GLvoid gl_init_cb(GLsizei, GLsizei);
GLvoid gl_draw_cb(GLvoid);

void normalizeVec3(GLfloat &x, GLfloat &y, GLfloat &z);
Brep *g_brep = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;

    /* Register the frame class */
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, appName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = appName;
    wndclass.lpszClassName = appName;

    if (!RegisterClass(&wndclass))
        return FALSE;

    /* Create the frame */
    g_hWnd = CreateWindow(appName,
        "BRep OpenGL",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        APP_WIDTH,
        APP_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* make sure window was created */
    if (!g_hWnd)
        return FALSE;

    /* show and update main window */
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    while (1) {
        /* Message */
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                return TRUE;
            }
        }
        /* Render */
        gl_draw_cb();
    }
}

/* Main window proc */
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG lRet = 1;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg) {
    case WM_CREATE:
        g_hDC = GetDC(hWnd);
        if (!bSetupPixelFormat(g_hDC))
            PostQuitMessage(0);

        g_hGLRC = wglCreateContext(g_hDC);
        wglMakeCurrent(g_hDC, g_hGLRC);
        GetClientRect(hWnd, &rect);
        gl_init_cb(rect.right, rect.bottom);
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        gl_resize_cb(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        if (g_hGLRC)
            wglDeleteContext(g_hGLRC);
        if (g_hDC)
            ReleaseDC(hWnd, g_hDC);
        g_hGLRC = 0;
        g_hDC = 0;

        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        if (g_hGLRC)
            wglDeleteContext(g_hGLRC);
        if (g_hDC)
            ReleaseDC(hWnd, g_hDC);

        PostQuitMessage(0);
        break;

    case WM_RBUTTONDOWN:
        RDown = true;
        mouseX = lParam & 0xFFFF;
        mouseY = lParam >> 16;
        break;

    case WM_RBUTTONUP:
        RDown = false;
        break;

    case WM_MOUSEMOVE:
        if (RDown)
        {
            INT x = (int)(short)LOWORD(lParam);
            INT y = (int)(short)HIWORD(lParam);
            yaw += 3.14159265359f * (x - mouseX) / 360;
            pitch -= 3.14159265359f * (y - mouseY) / 360;
            mouseX = x;
            mouseY = y;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            break;
        case VK_RIGHT:
            break;
        case VK_UP:
            break;
        case VK_DOWN:
            break;
        case 'W':
            eyeX += 0.2f * cosf(pitch) * cosf(yaw);
            eyeY += 0.2f * sinf(pitch);
            eyeZ += 0.2f * cosf(pitch) * sinf(yaw);
            break;
        case 'S':
            eyeX -= 0.2f * cosf(pitch) * cosf(yaw);
            eyeY -= 0.2f * sinf(pitch);
            eyeZ -= 0.2f * cosf(pitch) * sinf(yaw);
            break;
        case 'A':
        {
            GLfloat tmpX = -cosf(pitch) * sinf(yaw), tmpY = 0, tmpZ = cosf(pitch) * cosf(yaw);
            normalizeVec3(tmpX, tmpY, tmpZ);
            eyeX -= 0.2f * tmpX;
            eyeZ -= 0.2f * tmpZ;
        }
        break;
        case 'D':
        {
            GLfloat tmpX = -cosf(pitch) * sinf(yaw), tmpY = 0, tmpZ = cosf(pitch) * cosf(yaw);
            normalizeVec3(tmpX, tmpY, tmpZ);
            eyeX += 0.2f * tmpX;
            eyeZ += 0.2f * tmpZ;
        }
        break;
        }
        break;

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

BOOL bSetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 8;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

/* OpenGL code */
GLvoid gl_resize_cb(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLdouble)width / height, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

GLvoid gl_init_cb(GLsizei width, GLsizei height)
{
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLdouble)width / height, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    GLfloat lightPos[] = { 5, 4, 0, 1 };
    GLfloat lightPos2[] = { 0, 4, 5, 1 };
    GLfloat lightAmb[] = { 0.2f, 0.f, 0.f, 0.5f };
    GLfloat lightDiff[] = { 0.45f, 0.f, 0.f, 1.f };
    GLfloat lightSpec[] = { 0.45f, 0.f, 0.f, 1.f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);

    g_brep = new Brep();
    //// Cube1
    //brep->mvfs(1, -1, 1);
    //BSolid *solid1 = brep->solids.front();
    //brep->mev(solid1->GetFace(0)->outLoop, solid1->GetVertex(0), 1, 1, 1);
    //brep->mev(solid1->GetFace(0)->outLoop, solid1->GetVertex(1), -1, 1, 1);
    //brep->mev(solid1->GetFace(0)->outLoop, solid1->GetVertex(2), -1, -1, 1);
    //brep->mef(solid1->GetFace(0)->outLoop, solid1->GetVertex(3), solid1->GetVertex(0));
    //brep->mev(solid1->GetFace(1)->outLoop, solid1->GetVertex(0), 1, -1, -1);
    //brep->mev(solid1->GetFace(1)->outLoop, solid1->GetVertex(4), 1, 1, -1);
    //brep->mef(solid1->GetFace(1)->outLoop, solid1->GetVertex(5), solid1->GetVertex(1));
    //brep->mev(solid1->GetFace(2)->outLoop, solid1->GetVertex(5), -1, 1, -1);
    //brep->mef(solid1->GetFace(2)->outLoop, solid1->GetVertex(6), solid1->GetVertex(2));
    //brep->mev(solid1->GetFace(3)->outLoop, solid1->GetVertex(6), -1, -1, -1);
    //brep->mef(solid1->GetFace(3)->outLoop, solid1->GetVertex(7), solid1->GetVertex(3));
    //brep->mef(solid1->GetFace(4)->outLoop, solid1->GetVertex(7), solid1->GetVertex(4));

    //// Cube2
    //brep->mvfs(0.5, 1, 0.5);
    //BSolid *solid2 = brep->solids.back();
    //brep->mev(solid2->GetFace(0)->outLoop, solid2->GetVertex(0), 0.5, 2, 0.5);
    //brep->mev(solid2->GetFace(0)->outLoop, solid2->GetVertex(1), -0.5, 2, 0.5);
    //brep->mev(solid2->GetFace(0)->outLoop, solid2->GetVertex(2), -0.5, 1, 0.5);
    //brep->mef(solid2->GetFace(0)->outLoop, solid2->GetVertex(3), solid2->GetVertex(0));
    //brep->mev(solid2->GetFace(1)->outLoop, solid2->GetVertex(0), 0.5, 1, -0.5);
    //brep->mev(solid2->GetFace(1)->outLoop, solid2->GetVertex(4), 0.5, 2, -0.5);
    //brep->mef(solid2->GetFace(1)->outLoop, solid2->GetVertex(5), solid2->GetVertex(1));
    //brep->mev(solid2->GetFace(2)->outLoop, solid2->GetVertex(5), -0.5, 2, -0.5);
    //brep->mef(solid2->GetFace(2)->outLoop, solid2->GetVertex(6), solid2->GetVertex(2));
    //brep->mev(solid2->GetFace(3)->outLoop, solid2->GetVertex(6), -0.5, 1, -0.5);
    //brep->mef(solid2->GetFace(3)->outLoop, solid2->GetVertex(7), solid2->GetVertex(3));
    //brep->mef(solid2->GetFace(4)->outLoop, solid2->GetVertex(7), solid2->GetVertex(4));

    //brep->kfmhr(solid1->GetFace(2), solid2->GetFace(5));

    // sweep3
    g_brep->mvfs(0.5, 1, 0.5);
    BSolid *solid3 = g_brep->solids.back();
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(0), 0.5, 2, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(1), 0, 2, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(2), -0.5, 2, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(3), -0.5, 1, 0.5);
    g_brep->mef(solid3->GetFace(0)->outLoop, solid3->GetVertex(4), solid3->GetVertex(0));

    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(2), 0, 1.75, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(5), -0.25, 1.75, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(6), -0.25, 1.25, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(7), 0.25, 1.25, 0.5);
    g_brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(8), 0.25, 1.75, 0.5);
    g_brep->mef(solid3->GetFace(0)->outLoop, solid3->GetVertex(5), solid3->GetVertex(6), solid3->GetVertex(9), solid3->GetVertex(8));
    g_brep->kemr(solid3->GetFace(0)->outLoop, solid3->GetFace(0)->outLoop->findHalfEdgeWithVertex(solid3->GetVertex(2), solid3->GetVertex(5))->edge, solid3->GetVertex(2));
    g_brep->dump();
    g_brep->sweep(solid3, solid3->GetFace(0), 0, 0, -2);
    g_brep->dump();
}

GLvoid gl_draw_cb(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, \
        eyeX + cosf(pitch) * cosf(yaw), \
        eyeY + sinf(pitch), \
        eyeZ + cosf(pitch) * sinf(yaw), \
        0, 1, 0);

    glPushMatrix();
    if (renderMode == FACEMODE)
    {
        for (std::list<BSolid *>::iterator solidIt = g_brep->solids.begin(); solidIt != g_brep->solids.end(); ++solidIt)
        {
            BSolid *solid = *solidIt;
            GLuint id = glGenLists(solid->faces.size());
            if (id)
            {
                GLuint faceId = id;
                for (std::list<BFace *>::iterator faceIt = solid->faces.begin(); faceIt != solid->faces.end(); ++faceIt)
                {
                    BFace *face = *faceIt;
                    GLUtesselator *faceTess = gluNewTess();
                    if (faceTess != nullptr)
                    {
                        gluTessCallback(faceTess, GLU_TESS_BEGIN, (void(__stdcall*)(void))glBegin);
                        gluTessCallback(faceTess, GLU_TESS_END, (void(__stdcall*)(void))glEnd);
                        gluTessCallback(faceTess, GLU_TESS_VERTEX, (void(__stdcall*)())glVertex3dv);
                        glNewList(faceId, GL_COMPILE);
                        std::vector<GLdouble*> data;
                        gluTessBeginPolygon(faceTess, nullptr);
                        for (BLoop* loop : face->loops)
                        {
                            gluTessBeginContour(faceTess);
                            BHalfEdge *he = loop->firstHalfEdge;
                            do
                            {
                                GLdouble *d = new GLdouble[3];
                                d[0] = he->vertex->x;
                                d[1] = he->vertex->y;
                                d[2] = he->vertex->z;
                                gluTessVertex(faceTess, d, d);
                                data.push_back(d);
                                he = he->next;
                            } while (he != loop->firstHalfEdge);
                            gluTessEndContour(faceTess);
                        }
                        gluTessEndPolygon(faceTess);
                        for (GLdouble* d : data)
                        {
                            delete[] d;
                        }
                        glEndList();
                        gluDeleteTess(faceTess);
                    }
                    glCallList(faceId);
                    faceId++;
                }
                glDeleteLists(id, solid->faces.size());
            }
        }
    }
    else if (renderMode == LINEMODE)
    {
        for (std::list<BSolid *>::iterator solidIt = g_brep->solids.begin(); solidIt != g_brep->solids.end(); ++solidIt)
        {
            BSolid *solid = *solidIt;
            glPushMatrix();
            for (std::list<BFace *>::iterator faceIt = solid->faces.begin(); faceIt != solid->faces.end(); ++faceIt)
            {
                BFace *face = *faceIt;
                for (std::list<BLoop *>::iterator loopIt = face->loops.begin(); loopIt != face->loops.end(); ++loopIt)
                {
                    BLoop *loop = *loopIt;
                    glBegin(GL_LINE_LOOP);
                    BHalfEdge *he = loop->firstHalfEdge;
                    do
                    {
                        glVertex3f(he->vertex->x, he->vertex->y, he->vertex->z);
                        he = he->next;
                    } while (he != loop->firstHalfEdge);
                    glVertex3f(he->vertex->x, he->vertex->y, he->vertex->z);
                    glEnd();
                }
                //glTranslatef(0.1, 0.1, 0);
            }
            glPopMatrix();
        }
    }
    else
    {
        //Nothing
    }

    glPopMatrix();

    SWAPBUFFERS;
}

void normalizeVec3(GLfloat &x, GLfloat &y, GLfloat &z)
{
    GLfloat len = sqrtf(x * x + y * y + z * z);
    if (abs(len) <= 0.000001)
    {
        x = 0;
        y = 0;
        z = 0;
    }
    else
    {
        x /= len;
        y /= len;
        z /= len;
    }
}
