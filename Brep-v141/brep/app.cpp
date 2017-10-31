/*
* Example of a Windows OpenGL program.
* The OpenGL code is the same as that used in
* the X Window System sample
*/
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "brep.hpp"

/* Windows globals, defines, and prototypes */
CHAR szAppName[] = "Win OpenGL";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

#define SWAPBUFFERS SwapBuffers(ghDC) 
#define BLACK_INDEX     0 
#define RED_INDEX       13 
#define GREEN_INDEX     14 
#define BLUE_INDEX      16 
#define WIDTH           800 
#define HEIGHT          600

#define FACEMODE 0
#define LINEMODE 1
INT renderMode = LINEMODE;

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

/* OpenGL globals, defines, and prototypes */
GLfloat latitude, longitude, latinc, longinc;
GLdouble radius;

#define GLOBE    1 
#define CYLINDER 2 
#define CONE     3 

GLvoid resize(GLsizei, GLsizei);
GLvoid initializeGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);

Brep *brep = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG        msg;
    WNDCLASS   wndclass;

    /* Register the frame class */
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = szAppName;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
        return FALSE;

    /* Create the frame */
    ghWnd = CreateWindow(szAppName,
        "Generic OpenGL Sample",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* make sure window was created */
    if (!ghWnd)
        return FALSE;

    /* show and update main window */
    ShowWindow(ghWnd, nCmdShow);

    UpdateWindow(ghWnd);

    /* animation loop */
    while (1) {
        /*
        *  Process all pending messages
        */

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
        drawScene();
    }
}

/* main window procedure */
LONG WINAPI MainWndProc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    LONG    lRet = 1;
    PAINTSTRUCT    ps;
    RECT rect;

    switch (uMsg) {

    case WM_CREATE:
        ghDC = GetDC(hWnd);
        if (!bSetupPixelFormat(ghDC))
            PostQuitMessage(0);

        ghRC = wglCreateContext(ghDC);
        wglMakeCurrent(ghDC, ghRC);
        GetClientRect(hWnd, &rect);
        initializeGL(rect.right, rect.bottom);
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        if (ghRC)
            wglDeleteContext(ghRC);
        if (ghDC)
            ReleaseDC(hWnd, ghDC);
        ghRC = 0;
        ghDC = 0;

        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        if (ghRC)
            wglDeleteContext(ghRC);
        if (ghDC)
            ReleaseDC(hWnd, ghDC);

        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            longinc += 0.5F;
            break;
        case VK_RIGHT:
            longinc -= 0.5F;
            break;
        case VK_UP:
            latinc += 0.5F;
            break;
        case VK_DOWN:
            latinc -= 0.5F;
            break;
        }

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

GLvoid resize(GLsizei width, GLsizei height)
{
    GLfloat aspect;

    glViewport(0, 0, width, height);

    aspect = (GLfloat)width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 2.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

GLvoid createObjects()
{
    GLUquadricObj *quadObj;

    glNewList(GLOBE, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_LINE);
    gluSphere(quadObj, 1.5, 16, 16);
    glEndList();

    glNewList(CONE, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.3, 0.0, 0.6, 15, 10);
    glEndList();

    glNewList(CYLINDER, GL_COMPILE);
    glPushMatrix();
    glRotatef((GLfloat)90.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
    glTranslatef((GLfloat)0.0, (GLfloat)0.0, (GLfloat)-1.0);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.3, 0.3, 0.6, 12, 2);
    glPopMatrix();
    glEndList();
}

GLvoid initializeGL(GLsizei width, GLsizei height)
{
    GLfloat aspect;

    glClearIndex((GLfloat)BLACK_INDEX);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    aspect = (GLfloat)width / height;
    gluPerspective(45.0, aspect, 2.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(4, 4, 5, 0, 0, 0, 0, 1, 0);

    brep = new Brep();
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
    brep->mvfs(0.5, 1, 0.5);
    BSolid *solid3 = brep->solids.back();
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(0), 0.5, 2, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(1), 0, 2, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(2), -0.5, 2, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(3), -0.5, 1, 0.5);
    brep->mef(solid3->GetFace(0)->outLoop, solid3->GetVertex(4), solid3->GetVertex(0));

    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(2), 0, 1.75, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(5), -0.25, 1.75, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(6), -0.25, 1.25, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(7), 0.25, 1.25, 0.5);
    brep->mev(solid3->GetFace(0)->outLoop, solid3->GetVertex(8), 0.25, 1.75, 0.5);
    brep->mef(solid3->GetFace(0)->outLoop, solid3->GetVertex(5), solid3->GetVertex(6), solid3->GetVertex(9), solid3->GetVertex(8));
    brep->kemr(solid3->GetFace(0)->outLoop, solid3->GetFace(0)->outLoop->findHalfEdgeWithVertex(solid3->GetVertex(2), solid3->GetVertex(5))->edge, solid3->GetVertex(2));
    brep->dump();
    brep->sweep(solid3, solid3->GetFace(0), 0, 0, -2);
    brep->dump();
}

GLvoid drawScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    if (renderMode == FACEMODE)
    {
        for (std::list<BSolid *>::iterator solidIt = brep->solids.begin(); solidIt != brep->solids.end(); ++solidIt)
        {
            BSolid *solid = *solidIt;
            for (std::list<BFace *>::iterator faceIt = solid->faces.begin(); faceIt != solid->faces.end(); ++faceIt)
            {
                BFace *face = *faceIt;
                glBegin(GL_TRIANGLE_FAN);
                BHalfEdge *he = face->outLoop->firstHalfEdge;
                do
                {
                    glVertex3f(he->vertex->x, he->vertex->y, he->vertex->z);
                    he = he->next;
                } while (he != face->outLoop->firstHalfEdge);
                glEnd();
            }
        }
    }
    else if (renderMode == LINEMODE)
    {
        for (std::list<BSolid *>::iterator solidIt = brep->solids.begin(); solidIt != brep->solids.end(); ++solidIt)
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