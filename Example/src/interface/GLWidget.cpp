#include "GLWidget.h"
#include <QtGui>
#include <iostream>
#include "trackball.h"
#include "dialogedit.h"
#include "dialogrotate.h"
#include "dialognorma.h"

#include <GL/glu.h>


/* TODO:
 *  1 - Evento pra mudar currentMode entre DRAW e EDIT
 *      1.1 - ajustar perspective
 *      1.2 - variáveis com valores corretos (numPts, startLineLoop, ...)
 *  2 - Picking pro modo EDIT
 *  3 - Popup menu?
 *  3 - Novas funcionalidades
 *
 **/

void drawBlack(std::set<Vertex*> list);
void drawBlack(std::set<Mesh*> list);
void drawBlack(std::set<Loop*> list);
void drawBlack(std::set<Edge*> list);

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer  | QGL::SampleBuffers | QGL::StencilBuffer), parent)
{
    setFocusPolicy(Qt::StrongFocus);

    spinning = moving = false;
    zoom_fat = 1.0;
    zoomDepth = 50.0;
    pivot[0] = pivot[1] = pivot[2] = 0.0;
    trackball(curquat, 0.0, 0.0, 0.0, 0.0);
	clipClose = 0.001;
    fovy = 60.0;
    panFactor = tan(radians(fovy) * 0.5);
    show_grid = false;
    perspective = true;
    selectionMode = false;

    currentMode = EDIT;
    selectFace = false;
    selectMesh = false;
    selectEdge = false;
    selectVertex = true;

    startLineLoop = false;
    startBezier = false;
    numPts = 0;

    setVertexSelect();

    QMenu * menu = new QMenu(this);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* action1 = new QAction("Delete Mesh", this);
    QAction* action2 = new QAction("Vector", this);
    QAction* action3 = new QAction("Neighborhood", this);
    QAction* action4 = new QAction("Rotate", this);
    QAction* action5 = new QAction("Scale", this);
    QAction* action6 = new QAction("Translate", this);
    QAction* action7 = new QAction("Inface", this);
    QAction* action8 = new QAction("Normal", this);
    QAction* action9 = new QAction(this);
    action9->setSeparator(true);
    QAction* action10 = new QAction("Move", this);
    action10->setMenu(menu);

    menu->addAction(action2);
    menu->addAction(action8);




    addAction(action10);
    addAction(action6);
    addAction(action4);
    addAction(action5);
    addAction(action9); //SEPARADOR
    addAction(action7);
    addAction(action3);
    addAction(action1);
    addAction(action9);

    connect(action1, SIGNAL(triggered()), this, SLOT(deleteMesh()) );
    connect(action2, SIGNAL(triggered()), this, SLOT(move()) );
    connect(action8, SIGNAL(triggered()), this, SLOT(moveNormal()) );
    connect(action3, SIGNAL(triggered()), this, SLOT(neighborhood()) );
    connect(action4, SIGNAL(triggered()), this, SLOT(rotate()) );
    connect(action5, SIGNAL(triggered()), this, SLOT(scale()) );
    connect(action6, SIGNAL(triggered()), this, SLOT(translate()) );
    connect(action7, SIGNAL(triggered()), this, SLOT(inface()) );
}

GLWidget::~GLWidget()
{
}

void GLWidget::showGrid(bool s)
{
    show_grid = s;
    updateGL();
}

void GLWidget::setPerspective(bool s)
{
    perspective = s;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadProjection();
    updateGL();
}


void GLWidget::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable( GL_DEPTH_TEST );
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth( 2.0 );
    glEnable( GL_POLYGON_SMOOTH );
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glFrontFace(GL_CW);
    //Inicialização da luz
    GLfloat lambient_model[4] = { 0.4, 0.4, 0.4, 1.0};
    GLfloat a[4] = { 0.5, 0.5, 0.5, 1.0};
    GLfloat d[4] = { 0.8, 0.8, 0.8, 0.80};
    GLfloat s[4] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat p[4] = { 100.0, 100.0, 120.0, 0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    glLightfv(GL_LIGHT0, GL_SPECULAR, s);
    glLightfv(GL_LIGHT0, GL_POSITION, p);
    glEnable(GL_LIGHT0);


    glEnable(GL_LIGHT_MODEL_AMBIENT);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lambient_model);

    //Inicialização do viewport
    int w = width(), h = height();
    glViewport(0, 0, (GLint) w , (GLint) h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadProjection();
}


void GLWidget::paintGL()
{
	if(!isVisible()) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    drawBackground();
    //brep
    if(currentMode == EDIT) {
        if(perspective)
            glTranslatef(0.0, 0.0, -zoomDepth);
        GLfloat m[4][4];
        build_rotmatrix(m, curquat);
        glMultMatrixf(&m[0][0]);

        glTranslatef(-pivot[0], -pivot[1], -pivot[2]);

        if(show_grid)
            drawGrid();

        drawAxis(&m[0][0]);

        if(!scene.isEmpty()) {
            if (selectMesh)
                scene.render(MESHES);
            else
                scene.render(FACES);
            scene.render(POINTS);
            scene.render(LINES);
        }
    } else { //DRAW
        if(!drawScene.isEmpty()) {
            if (selectMesh)
                drawScene.render(MESHES);
            else
                drawScene.render(FACES);
            drawScene.render(POINTS);
            drawScene.render(LINES);
        }

        if(startBezier && numPts == 1)
        {
            glColor3f(1.0, 0, 0);
            glBegin(GL_POINTS);
                glVertex3f(curveP1[0], curveP1[1], 0.0);
            glEnd();
        }
        curve.draw();
    }
}


void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	loadProjection();

    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(currentMode == EDIT) {
        if(event->button() == SPIN_BUTTON)
        {
            beginx = event->x();
            beginy = event->y();

            if(event->modifiers() & Qt::ShiftModifier)
                moving = true;
            else if(event->modifiers() & Qt::CTRL)
            {
                shotPicking(event->x(), this->height() - event->y());
                updateGL();
            } else
                spinning = true;
        }
    } else { //currentMode == DRAW
        if(event->button() == Qt::LeftButton) {
            draw(event->x(), this->height() - event->y());
            updateGL();
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float width = size().width(), height = size().height();

    if(spinning)
    {
        trackball(lastquat,
          (2.0 * beginx - width) / width,
          (height - 2.0 * beginy) / height,
          (2.0 * event->x() - width) / width,
          (height - 2.0 * event->y()) / height
        );
        beginx = event->x();
        beginy = event->y();
        add_quats(lastquat, curquat, curquat);
        updateGL();
    }
    else if(moving)
    {
        float modelview[16];
        glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

        float dx, dy;
        if(perspective)
        {
            dx = (((2.0 * beginx - width) / width) - ((2.0 * event->x() - width) / width)) * zoomDepth * width/height * panFactor;
            dy = (((height - 2.0 * beginy) / height) - ((height - 2.0 * event->y()) / height)) * zoomDepth * panFactor;
        }
        else
        {
            dx = (((2.0 * beginx - width) / width) - ((2.0 * event->x() - width) / width)) * zoom_fat * width/height;
            dy = (((height - 2.0 * beginy) / height) - ((height - 2.0 * event->y()) / height)) * zoom_fat;
        }

        beginx = event->x();
        beginy = event->y();

        modelview[0] *= dx;
        modelview[4] *= dx;
        modelview[8] *= dx;

        modelview[1] *= dy;
        modelview[5] *= dy;
        modelview[9] *= dy;

        pivot[0] += modelview[0];
        pivot[1] += modelview[4];
        pivot[2] += modelview[8];

        pivot[0] += modelview[1];
        pivot[1] += modelview[5];
        pivot[2] += modelview[9];

        updateGL();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        moving = spinning = false;
    }
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	if(event->modifiers() & Qt::ControlModifier)
	{
		float ds = event->delta()*0.001;
		clipClose += ds;
		clipClose =  clipClose < 0.0001 ? 0.0001 : (clipClose > 1000.0 ? 1000.0 : clipClose);
	}
	else
	{
		float ds = event->delta()*0.01;

        zoomDepth -= ds;
        zoomDepth = zoomDepth < 0.0 ? 0.0 : zoomDepth;

		zoom_fat -= ds;
		zoom_fat = zoom_fat < 0.0 ? 0.0 : zoom_fat;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	loadProjection();
	updateGL();
}

void GLWidget::drawGrid()
{
    static struct Grid
    {
        float l;
        int res;
        float d;
        float* vertices;

        Grid()
        :   l(100.0),
            res(71),
            d(l/float(res-1)),
            vertices(new float[res*res*2])
        {
            bool forward = true;
            for(int j=0; j<res; j++)
            {
                for(int i=0; i<res; i++)
                {
                    if(forward)
                        vertices[i*2 + j*res*2] = i*d - l*0.5;
                    else
                        vertices[i*2 + j*res*2] = (res-i-1)*d - l*0.5;

                    vertices[i*2 + j*res*2 + 1] = j*d - l*0.5;
                }

                forward = !forward;
            }
        }
    } grid;

	glPushAttrib(GL_LIGHTING_BIT);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.4, 0.4, 0.4);
    glVertexPointer(2, GL_FLOAT, 0, reinterpret_cast<void*>(grid.vertices));
    glDrawArrays(GL_LINE_STRIP, 0, grid.res*grid.res);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glDrawArrays(GL_LINE_STRIP, 0, grid.res*grid.res);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopAttrib();
	glPopClientAttrib();
    glPopMatrix();
}



void GLWidget::drawAxis(GLfloat *mat)
{
    static struct Axis
    {
        Axis()
        {
            id = glGenLists(1);

            glNewList(id, GL_COMPILE);

                glLineWidth(2);
                glBegin(GL_LINES);
                    glColor3f(1.0, 0.9, 0.9);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.8, 0.0, 0.0);
                    glVertex3f(1.0, 0.0, 0.0);

                    glColor3f(0.9, 1.0, 0.9);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.0, 0.8, 0.0);
                    glVertex3f(0.0, 1.0, 0.0);

                    glColor3f(0.9, 0.9, 1.0);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.0, 0.0, 0.8);
                    glVertex3f(0.0, 0.0, 1.0);
                glEnd();

            glEndList();
        }

        int id;

    } axis;

    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT | GL_LINE_BIT);
    glDisable( GL_LIGHTING );

    glViewport(0, 0, 80.0, 80.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho (-1.5, 1.5, -1.5, 1.5, -1.199, 1.199);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(mat);

    glCallList(axis.id);

    glColor3f(1.0, 0.0, 0.0);
    renderText(1.2, 0.0, 0.0, "x");
    glColor3f(0.0, 1.0, 0.0);
    renderText(0.0, 1.2, 0.0, "y");
    glColor3f(0.0, 0.0, 1.0);
    renderText(0.0, 0.0, 1.2, "z");

    glEnable(GL_MULTISAMPLE); //rendertText desabilita o multisample.

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void GLWidget::drawBackground()
{    
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if(currentMode == EDIT) {
        glBegin(GL_QUADS);
            glColor3f(0.6431,0.5843,0.8705);
            glVertex2f(-1.0,-1.0);
            glVertex2f(1.0,-1.0);
            glColor3f(0.0862,0.0,0.4039);
            glVertex2f(1.0,1.0);
            glVertex2f(-1.0,1.0);
        glEnd();
    } else {
        glBegin(GL_QUADS);
            glColor3f(0.7031,0.8143,0.5505);
            glVertex2f(-1.0,-1.0);
            glVertex2f(1.0,-1.0);
            glColor3f(0.3562, 0.55,0.35039);
            glVertex2f(1.0,1.0);
            glVertex2f(-1.0,1.0);
        glEnd();

        int disc = 32;
        float step = 2.0/(float)disc;
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(0.7);
        for(int i = 0; i < disc; i++){
            for(int j = 0; j < disc; j++) {
                glBegin(GL_LINE_LOOP);
                    glVertex2f(-1 + step*i, -1 + step*j);
                    glVertex2f(-1 + step*i, -1 + step*(j+1));
                    glVertex2f(-1 + step*(i+1), -1 + step*(j+1));
                    glVertex2f(-1 +step*(i+1), -1 +step*j);
                glEnd();
            }
        }
        glLineWidth(2.0);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}


void GLWidget::shotPicking(int x, int y)
{
    GLuint buff[512] = {0};
    int hits, view[4];

    glSelectBuffer(512, buff);
    glGetIntegerv(GL_VIEWPORT, view);

    glRenderMode(GL_SELECT);
    glInitNames();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x, y, 5.0, 5.0, view);
    loadProjection();
    glMatrixMode(GL_MODELVIEW);

    if(selectMesh)
        scene.render(MESHES);
    else if(selectFace)
        scene.render(FACES);
    else if(selectEdge)
        scene.render(LINES);
    else if(selectVertex)
        scene.render(POINTS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);

//    std::cout << "hits: "  << hits << std::endl;
    int nearestMesh = buff[3];
    int nearestItem = buff[4];
    int nearestZ = buff[1];
    for (int i = 1; i < hits; i++) {
        if (nearestZ > buff[5*i+1]) {
            nearestZ = buff[5*i+1];
            nearestMesh = buff[5*i + 3];
            nearestItem = buff[5*i + 4];
        }
    }
//    for (int i = 0; i < hits; i++) {
//        std::cout << "hit: " << i << std::endl;
//        std::cout << "\tNumber: " << buff[5*i] << std::endl;
//        std::cout << "\tMin z: " << buff[5*i+1] << std::endl;
//        std::cout << "\tMax z: " << buff[5*i+2] << std::endl;
//        std::cout << "\tName mesh: " << buff[5*i+3] << std::endl;
//        std::cout << "\tName item: " << buff[5*i+4] << std::endl;
//    }

//    std::cout << "mesh escolhido: " << nearestMesh << std::endl;
//    std::cout << "obj escolhido: " << nearestItem << std::endl;

    if (hits == 0)
        return;
    Mesh *m = scene.getSolid(nearestMesh);

    glMatrixMode(GL_MODELVIEW);
    if(selectMesh) {
        std::set<Mesh*>::iterator it;
        for (it = meshList.begin(); it != meshList.end(); ++it)
        {
            if(*it == m)
            {
                m->r = 0.6;
                m->g = 0.6;
                m->b = 0.6;
                meshList.erase(it);
                updateGL();
                return;
            }
        }
        m->r = 1.0;
        m->g = 0.0;
        m->b = 0.0;
        meshList.insert(m);
    } else if(selectFace) {
        Loop *l = scene.getLoop(m->id, nearestItem);

        std::set<Loop*>::iterator it;
        for (it = faceList.begin(); it != faceList.end(); ++it)
        {
            if(*it == l)
            {
                l->r = 0.6;
                l->g = 0.6;
                l->b = 0.6;
                faceList.erase(it);
                updateGL();
                return;
            }
        }
        l->r = 1.0;
        l->g = 0.0;
        l->b = 0.0;
        faceList.insert(l);
    } else if(selectEdge) {
        Edge *e = scene.getEdge(m->id, nearestItem);

        std::set<Edge*>::iterator it;
        for (it = edgeList.begin(); it != edgeList.end(); ++it)
        {
            if(*it == e)
            {
                e->r = 0.0;
                e->g = 0.0;
                e->b = 0.0;
                edgeList.erase(it);
                updateGL();
                return;
            }
        }
        e->r = 1.0;
        e->g = 0.0;
        e->b = 0.0;
        edgeList.insert(e);
    } else if(selectVertex) {
        Vertex *v = scene.getVertex(m->id, nearestItem);

        std::set<Vertex*>::iterator it;
        for (it = vertexList.begin(); it != vertexList.end(); ++it)
        {
            if(*it == v)
            {
                v->r = 0.0;
                v->g = 0.0;
                v->b = 0.0;
                vertexList.erase(it);
                updateGL();
                return;
            }
        }
        v->r = 1.0;
        v->g = 0.0;
        v->b = 0.0;
        vertexList.insert(v);
    }
}

void GLWidget::loadProjection()
{
    float w = width();
    float h = height();
    float ratio = w/h;

    if(perspective)
    {
        gluPerspective(fovy, ratio, clipClose, 20000);
    }
    else
    {
        //FIXME: Projeção ortográfica não está funcionando shader de iluminação por fragmento.
        //if(w >= h)
        //    glOrtho(-zoom_fat*ratio, zoom_fat*ratio, -zoom_fat, zoom_fat, -20000.0, 20000.0);
        //else
        //    glOrtho(-zoom_fat, zoom_fat, -zoom_fat/ratio, zoom_fat/ratio, -20000.0, 20000.0);
        glOrtho(0, w, 0, h, -1, 1);
    }
}

void GLWidget::deleteMesh()
{
    std::set<Mesh*>::iterator iter;
    for(iter = meshList.begin(); iter != meshList.end(); iter++) {
        scene.removeSolid((*iter)->id);
    }
    meshList.clear();
}

void GLWidget::move()
{
    DialogEdit edit(this);
    if(edit.exec())
    {
        //Verificar o que está selecionado
        if(selectVertex) {
            std::set<Vertex*>::iterator vIter;
            for(vIter = vertexList.begin(); vIter != vertexList.end(); vIter++)
                (*vIter)->move(edit.getX(), edit.getY(), edit.getZ());
        } else if(selectEdge) {
            //Para toda aresta selecionada, mover os 2 vértices dessa aresta
            std::set<Edge*>::iterator eIter;
            std::set<Vertex*> tempVertexList;
            for(eIter = edgeList.begin(); eIter != edgeList.end(); eIter++) {
                tempVertexList.insert((*eIter)->hed1->origin);
                tempVertexList.insert((*eIter)->hed2->origin);
            }
            std::set<Vertex*>::iterator vIter;
            for(vIter = tempVertexList.begin(); vIter != tempVertexList.end(); vIter++)
                (*vIter)->move(edit.getX(), edit.getY(), edit.getZ());
        } else if(selectFace) {
            //Para toda face selecionada, mover os vértices da face
            std::set<Loop*>::iterator lIter;
            std::set<Vertex*> tempVertexList;
            for(lIter = faceList.begin(); lIter != faceList.end(); lIter++) {
                HalfEdge *h = (*lIter)->hed;
                do {
                    tempVertexList.insert(h->origin);
                    h = h->next;
                } while(h != (*lIter)->hed);
            }
            std::set<Vertex*>::iterator vIter;
            for(vIter = tempVertexList.begin(); vIter != tempVertexList.end(); vIter++)
                (*vIter)->move(edit.getX(), edit.getY(), edit.getZ());
        }
    }
}

void GLWidget::moveNormal()
{
    DialogNorma edit(this);
    if(edit.exec())
    {
        if(selectFace)
        {
            //Para toda face selecionada, mover os vértices da face
            std::set<Loop*>::iterator lIter;
            std::set<Vertex*> tempVertexList;
            for(lIter = faceList.begin(); lIter != faceList.end(); lIter++)
            {
                HalfEdge *h = (*lIter)->hed;
                do {
                    tempVertexList.insert(h->origin);
                    //
                    h->origin->move(-edit.getSize()*h->loop->normal.x , -edit.getSize()*h->loop->normal.y, -edit.getSize()*h->loop->normal.z);

                    h = h->next;
                } while(h != (*lIter)->hed);
            }
        }
    }
}

void GLWidget::neighborhood()
{
    if(selectFace)
    {
        std::set<Loop*>  aux;
        HalfEdge* he;

        std::set<Loop*>::iterator iter;
        for(iter = faceList.begin(); iter != faceList.end(); iter++)
        {
            he = (*iter)->hed;
            do
            {
                he->mate()->loop->r = 1.0;
                he->mate()->loop->g = 0.0;
                he->mate()->loop->b = 0.0;

                aux.insert(he->mate()->loop);
                he = he->next;
            }while(he != (*iter)->hed);
        }

        for(iter = aux.begin(); iter != aux.end(); iter++)
            faceList.insert( (*iter) );

    }
    else if(selectEdge)
    {
        std::set<Edge*>  aux;
        Edge* e;
        HalfEdge* h;

        std::set<Edge*>::iterator iter;
        for(iter = edgeList.begin(); iter != edgeList.end(); iter++)
        {
            e = (*iter);
            h = (*iter)->hed1;
            while(h->mate()->next->edge != (*iter))
            {
                e = h->mate()->next->edge;
                h = h->mate()->next;

                e->r = 1.0;
                e->g = 0.0;
                e->b = 0.0;

                aux.insert(e);
            }

            e = (*iter);
            h = (*iter)->hed2;
            while(h->mate()->next->edge != (*iter))
            {
                e = h->mate()->next->edge;
                h = h->mate()->next;

                e->r = 1.0;
                e->g = 0.0;
                e->b = 0.0;

                aux.insert(e);
            }

        }

        for(iter = aux.begin(); iter != aux.end(); iter++)
            edgeList.insert( (*iter) );
    }
    else //Vertex
    {
        std::set<Vertex*> aux;
        std::set<Vertex*>::iterator vIter;
        for(vIter = vertexList.begin(); vIter != vertexList.end(); vIter++) {
            HalfEdge *h = (*vIter)->hed;
            do {
                h = h->mate();
                h->origin->r = 1.0;
                h->origin->g = 0.0;
                h->origin->b = 0.0;
                aux.insert(h->origin);
                h = h->next;
            } while(h != (*vIter)->hed);
        }
        for(vIter = aux.begin(); vIter != aux.end(); vIter++)
            vertexList.insert(*vIter);
    }
}

void GLWidget::rotate()
{
    DialogRotate edit(this);
    if(edit.exec())
    {
        std::set<Mesh*>::iterator mIter;
        for(mIter = meshList.begin(); mIter != meshList.end(); mIter++)
            (*mIter)->rotate(edit.getAngle()*M_PI/180.0, edit.getX(), edit.getY(), edit.getZ());
    }
}

void GLWidget::scale()
{
    DialogEdit edit(this);
    if(edit.exec())
    {
        //Percorrer a lista de meshes selecionados e aplica a escala
        std::set<Mesh*>::iterator mIter;
        for(mIter = meshList.begin(); mIter != meshList.end(); mIter++)
            (*mIter)->scale(edit.getX(), edit.getY(), edit.getZ());
    }
}

void GLWidget::translate()
{
    DialogEdit edit(this);
    if(edit.exec())
    {
        //Percorrer a lista de meshes selecionados e aplica a translação
        std::set<Mesh*>::iterator mIter;
        for(mIter = meshList.begin(); mIter != meshList.end(); mIter++)
            (*mIter)->translate(edit.getX(), edit.getY(), edit.getZ());
    }
}

void GLWidget::inface()
{
    if(selectFace && currentMode==EDIT)
    {
        std::set<Loop*>::iterator fIter;
        for(fIter = faceList.begin(); fIter != faceList.end(); fIter++)
        {
            scene.sweep( (*fIter)->face->solid->id, (*fIter)->face->id,
                        -0.01*(*fIter)->normal.x, -0.01*(*fIter)->normal.y, -0.01*(*fIter)->normal.z);
                         //-(*fIter)->normal.x, -(*fIter)->normal.y, -(*fIter)->normal.z);
        }

        //Para toda face selecionada, mover os vértices da face
        std::set<Loop*>::iterator lIter;
        for(lIter = faceList.begin(); lIter != faceList.end(); lIter++)
        {
            HalfEdge *h = (*lIter)->hed;
            double centerx = 0.0;
            double centery = 0.0;
            double centerz = 0.0;
            double i=0;

            do
            {
                centerx += h->origin->x;
                centery += h->origin->y;
                centerz += h->origin->z;
                i++;
                h = h->next;
            } while(h != (*lIter)->hed);

            centerx /= i;
            centery /= i;
            centerz /= i;

            h = (*lIter)->hed;

            do
            {
                h->origin->setPosition((centerx+h->origin->x)/2.0, (centery+h->origin->y)/2.0, (centerz+h->origin->z)/2.0);
                h = h->next;
            } while(h != (*lIter)->hed);
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    float modif = 1.0;
    if(keyEvent->modifiers() & Qt::ControlModifier)
        modif *= -1.0;

    switch(keyEvent->key())
    {
        case Qt::Key_7:
        {
            float a[] = {1.f, 0.f, 0.f};
            axis_to_quat(a, 0.f, curquat);
            break;
        }
        case Qt::Key_1:
        {
            float b[] = {1.f*modif, 0.f, 0.f};
            axis_to_quat(b, M_PI_2, curquat);
            break;
        }
    }

    updateGL();
}


void GLWidget::switchMode() {
    if (currentMode == EDIT) {
        currentMode = DRAW;
        numPts = 0;
        //startLineLoop = true;
        setPerspective(false);
    } else {
        currentMode = EDIT;
        setPerspective(true);
    }
}

void GLWidget::draw(int x, int y)
{
    if(startLineLoop) {
        if(numPts == 0) {
            drawScene.mvfs(x, y, 0.0);
            drawScene.getSolid(drawScene.numMeshes - 1)->manifold = false;
            numPts++;
            std::cout << "primeiro ponto" << std::endl;
        } else {
            Vertex *first = drawScene.getVertex(drawScene.numMeshes - 1, 0);
            if(fabs(x - first->x) < 3 && fabs(y - first->y) < 3) {
                drawScene.smef(drawScene.numMeshes -1, 0, numPts - 1, 0);
                numPts = 0;
                //Força a criação de um novo mesh.
                drawScene.numMeshes++;
            } else {
                Mesh *m = drawScene.getSolid(drawScene.numMeshes - 1);
                std::list<Vertex*>::iterator vIter = m->vertices.begin();
                vIter++;
                for(; vIter != m->vertices.end(); vIter++)
                    if(fabs(x - (*vIter)->x) < 3 && fabs(y - (*vIter)->y)< 3)
                        return;

                drawScene.smev(drawScene.numMeshes - 1, 0, numPts - 1, x, y, 0.0);
                numPts++;
            }
        }
    }

    if(startBezier)
    {
        if(numPts == 1)
        {
            curve = Bezier(curveP1[0], curveP1[1], 0.0, x, y, 0.0);
            numPts=0;
            //startBezier=false;
            return;
        }
        curveP1[0] = x;
        curveP1[1] = y;
        numPts++;

    }
}

void GLWidget::setMeshSelect()
{
    if(selectFace == true)
        drawBlack(faceList);
    else if(selectEdge == true)
        drawBlack(edgeList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=true;
    selectFace=false;
    selectEdge=false;
    selectVertex=false;

    faceList.clear();
    edgeList.clear();
    vertexList.clear();

    updateGL();
}

void GLWidget::setFaceSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectEdge == true)
        drawBlack(edgeList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=false;
    selectFace=true;
    selectEdge=false;
    selectVertex=false;

    meshList.clear();
    edgeList.clear();
    vertexList.clear();

    updateGL();
}

void GLWidget::setEdgeSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectFace == true)
        drawBlack(faceList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=false;
    selectFace=false;
    selectEdge=true;
    selectVertex=false;

    meshList.clear();
    faceList.clear();
    vertexList.clear();

    updateGL();
}

void GLWidget::setVertexSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectFace == true)
        drawBlack(faceList);
    else if(selectEdge == true)
        drawBlack(edgeList);

    selectMesh=false;
    selectFace=false;
    selectEdge=false;
    selectVertex=true;

    meshList.clear();
    faceList.clear();
    edgeList.clear();

    updateGL();
}

void GLWidget::doneDrawing()
{
    if(numPts > 0) {
        //Erro: uma face não foi fechada
        return;
    }

    //Copia os meshes de drawScene para scene
    std::list<Mesh*>::iterator iter;
    for(iter = drawScene.meshes.begin(); iter != drawScene.meshes.end(); ) {
        //mapeia os vértices para a nova tela
        //As coordenadas de draw vão de (0 -> w) (0 - h)
        //As coordenadas da área de edit estão fixas (-10 -> 10) (-7.5 -> 7.5)

        float w = width();
        float h = height();

        std::list<Vertex*>::iterator vIter;
        for(vIter = (*iter)->vertices.begin(); vIter != (*iter)->vertices.end(); vIter++) {
            (*vIter)->x = 20*(*vIter)->x/(float)w - 10;
            (*vIter)->y = 15.0*(*vIter)->y/(float)h - 7.5;
            (*vIter)->z = 0.0;
        }
        (*iter)->id = scene.numMeshes;
        scene.meshes.push_back(*iter);
        scene.numMeshes++;
        iter = drawScene.meshes.erase(iter);
        drawScene.numMeshes--;
    }
}

void GLWidget::sweep(float dX, float dY, float dZ)
{
    std::set<Loop*>::iterator fIter;
    for(fIter = faceList.begin(); fIter != faceList.end(); fIter++) {
        if (!(*fIter)->face->solid->manifold)
            (*fIter)->face->solid->manifold = true;
        scene.sweep((*fIter)->face->solid->id, (*fIter)->face->id, dX, dY, dZ);
    }
}

void drawBlack(std::set<Mesh*> list)
{
    std::set<Mesh*>::iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
            (*it)->r = 0.6;
            (*it)->g = 0.6;
            (*it)->b = 0.6;
            list.erase(it);
    }
}

void drawBlack(std::set<Loop*> list)
{
    std::set<Loop*>::iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
            (*it)->r = 0.6;
            (*it)->g = 0.6;
            (*it)->b = 0.6;
            list.erase(it);
    }
}

void drawBlack(std::set<Edge*> list)
{
    std::set<Edge*>::iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
            (*it)->r = 0.0;
            (*it)->g = 0.0;
            (*it)->b = 0.0;
            list.erase(it);
    }
}

void drawBlack(std::set<Vertex*> list)
{
    std::set<Vertex*>::iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
            (*it)->r = 0.0;
            (*it)->g = 0.0;
            (*it)->b = 0.0;
            list.erase(it);
    }
}
