#include "Scene.h"
#include <GL/gl.h>
#include <iostream>
#include "math/Vec3.h"
#include "math/Quat.h"
#include <cmath>

Scene::Scene() {
    numMeshes = 0;    
}

Scene::~Scene() {
    this->clear();
}

void Scene::clear() {
    std::list<Mesh*>::iterator iter;
    for(iter = meshes.begin(); iter != meshes.end(); iter++) {
        delete *iter;
    }
    meshes.clear();
    numMeshes = 0;
}

void Scene::render(RenderMode mode) {
    if (mode == FACES || mode == MESHES) {
        std::list<Mesh*>::iterator mIter;
        for(mIter = meshes.begin(); mIter != meshes.end(); mIter++){
            glPushMatrix();
            float m[16];
            (*mIter)->orientation.toMatrix(m);
            glMultMatrixf(m);
            glScalef((*mIter)->sx, (*mIter)->sy, (*mIter)->sz);
            glTranslatef((*mIter)->tx, (*mIter)->ty, (*mIter)->tz);
            std::list<Face*>::iterator fIter;
            glPushName((*mIter)->id);
            if(mode == MESHES)
                glColor3f((*mIter)->r,(*mIter)->g, (*mIter)->b);
            fIter = (*mIter)->faces.begin();
            if(!(*mIter)->manifold)
                fIter++;
            for(; fIter != (*mIter)->faces.end(); fIter++) {
                std::list<Loop*>::iterator lIter;
                for (lIter = (*fIter)->loops.begin(); lIter != (*fIter)->loops.end(); lIter++) {
                    HalfEdge *he = (*lIter)->hed;
                    glPushName((*lIter)->id);
                    if (mode == FACES)
                        glColor3f((*lIter)->r, (*lIter)->g, (*lIter)->b);
                    glNormal3f((*lIter)->normal.x, (*lIter)->normal.y, (*lIter)->normal.z);
                    glBegin(GL_POLYGON);
                    do {
                        Vertex *vert = he->origin;
                        glVertex3f(vert->x, vert->y, vert->z);
                    }while((he = he->next) != (*lIter)->hed);
                    glEnd();
                    glPopName();
                }
            }
            glPopName();
            glPopMatrix();
        }
    } else if (mode == POINTS) {
        std::list<Mesh*>::iterator mIter;
        for(mIter = meshes.begin(); mIter != meshes.end(); mIter++){
            glPushMatrix();
            float m[16];
            (*mIter)->orientation.toMatrix(m);
            glMultMatrixf(m);
            glScalef((*mIter)->sx, (*mIter)->sy, (*mIter)->sz);
            glTranslatef((*mIter)->tx, (*mIter)->ty, (*mIter)->tz);
            std::list<Vertex*>::iterator vIter;
            glPushName((*mIter)->id);
            for(vIter = (*mIter)->vertices.begin(); vIter != (*mIter)->vertices.end(); vIter++) {
                glColor3f((*vIter)->r, (*vIter)->g, (*vIter)->b);
                glPushName((*vIter)->id);
                glBegin(GL_POINTS);
                glVertex3f((*vIter)->x, (*vIter)->y, (*vIter)->z);
                glEnd();
                glPopName();
            }
            glPopName();
            glPopMatrix();
        }
    } else if (mode == LINES) {
        std::list<Mesh*>::iterator mIter;
        for(mIter = meshes.begin(); mIter != meshes.end(); mIter++){
            glPushMatrix();
            float m[16];
            (*mIter)->orientation.toMatrix(m);
            glMultMatrixf(m);
            glScalef((*mIter)->sx, (*mIter)->sy, (*mIter)->sz);
            glTranslatef((*mIter)->tx, (*mIter)->ty, (*mIter)->tz);
            std::list<Edge*>::iterator eIter;
            glPushName((*mIter)->id);
            for(eIter = (*mIter)->edges.begin(); eIter != (*mIter)->edges.end(); eIter++) {
                glColor3f((*eIter)->r,(*eIter)->g, (*eIter)->b);
                glPushName((*eIter)->id);
                glBegin(GL_LINES);
                    glVertex3f((*eIter)->hed1->origin->x, (*eIter)->hed1->origin->y, (*eIter)->hed1->origin->z);
                    glVertex3f((*eIter)->hed2->origin->x, (*eIter)->hed2->origin->y, (*eIter)->hed2->origin->z);
                glEnd();
                glPopName();
            }
            glPopName();
            glPopMatrix();
        }
    }
}

bool Scene::isEmpty() {
    return meshes.empty();
}

Mesh* Scene::getSolid(int idSolid) {
    Mesh *m;
    std::list<Mesh*>::iterator iter;
    for(iter = meshes.begin(); iter != meshes.end(); iter++)
        if ((*iter)->getId() == idSolid) {
            m = *iter;
            return m;
        }
    return NULL;
}

Face* Scene::getFace(Mesh *m, int idFace) {
    Face *f;
    std::list<Face*>::iterator iter;
    for(iter = m->faces.begin(); iter != m->faces.end(); iter++)
        if((*iter)->getId() == idFace) {
            f = *iter;
            return f;
        }
    return NULL;
}

HalfEdge* Scene::getHEd(Face *f, int idVertex1, int idVertex2) {
    HalfEdge *h;
    std::list<Loop*>::iterator iter;
    for(iter = f->loops.begin(); iter != f->loops.end(); iter++)
    {
        h = (*iter)->hed;
        do {
            if(h->origin->getId() == idVertex1 &&
               h->next->origin->getId() == idVertex2)
                return h;
        }while((h = h->next) != (*iter)->hed);
    }
    return NULL;
}

Loop* Scene::getLoop(int solidId, int id) {
    Mesh *m = getSolid(solidId);
    std::list<Face*>::iterator fIter;
    for(fIter = m->faces.begin(); fIter != m->faces.end(); fIter++) {
        std::list<Loop*>::iterator lIter;
        for (lIter = (*fIter)->loops.begin(); lIter != (*fIter)->loops.end(); lIter++)
            if((*lIter)->id == id)
                return *lIter;
    }
    return NULL;
}

Vertex* Scene::getVertex(int solidId, int id) {
    Mesh *m = getSolid(solidId);
    std::list<Vertex*>::iterator vIter;
    for(vIter = m->vertices.begin(); vIter != m->vertices.end(); vIter++)
        if((*vIter)->id == id)
            return *vIter;
    return NULL;
}

Edge* Scene::getEdge(int solidId, int id) {
    Mesh *m = getSolid(solidId);
    std::list<Edge*>::iterator eIter;
    for(eIter = m->edges.begin(); eIter != m->edges.end(); eIter++)
        if((*eIter)->id == id)
            return *eIter;
    return NULL;
}

HalfEdge* Scene::getHEd(Face *f, int idVertex1) {
    HalfEdge *h;
    std::list<Loop*>::iterator iter;
    for(iter = f->loops.begin(); iter != f->loops.end(); iter++)
    {
        h = (*iter)->hed;
        do {
            if(h->origin->getId() == idVertex1)
                return h;
        }while((h = h->next) != (*iter)->hed);
    }
    return NULL;
}

void Scene::mvfs(float x, float y, float z) {
    Mesh *m;
    Face *f;
    Loop *l;
    Vertex *v;
    HalfEdge *he;


    m = new Mesh(numMeshes++);
    f = new Face(m);
    l = new Loop(f);
    v = new Vertex(x, y, z, m);
    he = new HalfEdge;

    f->outter = l;
    l->hed = he;
    he->loop = l;
    he->next = he->prev = he;
    he->origin = v;
    he->edge = NULL;
    //v->hed = he;
    meshes.push_back(m);
    //updateBoundingBox();
}

void Scene::lmev(HalfEdge *he1, HalfEdge *he2, float x,
                float y, float z)
{
    HalfEdge *he;
    Face *f = he1->loop->face;

    Vertex *v = new Vertex(x, y, z, he1->loop->face->solid);
    Edge *e = new Edge(he1->loop->face->solid);

    he = he1;
    while (he1 != he2) {
        he->origin = v;
        he = he->mate()->next;
    }
    addhe(e, he2->origin, he1, PLUS);
    addhe(e, v, he2, MINUS);

    v->hed = he2->prev;
    he2->origin->hed = he2;

    //updateBoundingBox();
}

void Scene::lmef(HalfEdge *h1, HalfEdge *h2) {
    Face *f;
    Loop *l;
    Edge *e;
    HalfEdge *he, *nhe1, *nhe2, *temp;

    f = new Face(h1->loop->face->solid);
    l = new Loop(f);
    e = new Edge(h1->loop->face->solid);
    f->outter = l;

    he = h1;
    while(he != h2) {
        he->loop = l;
        he = he->next;
    }

    nhe1 = addhe(e, h2->origin, h1, PLUS);
    nhe2 = addhe(e, h1->origin, h2, MINUS);

    nhe1->prev->next = nhe2;
    nhe2->prev->next = nhe1;
    temp = nhe1->prev;
    nhe1->prev = nhe2->prev;
    nhe2->prev = temp;

    l->hed = nhe1;
    h2->loop->hed = nhe2;

    //h1->loop->setNormal();
    h2->loop->setNormal();

    l->setNormal();
}

bool Scene::mev(int idSolid, int idFace1, int idFace2, int idVertex1,
                int idVertex2, int idVertex3, float x,
                float y, float z)
{
    Mesh *s;
    Face *f1, *f2;
    HalfEdge *he1, *he2;

    if((s = getSolid(idSolid)) == NULL)
        return false;
    if((f1 = getFace(s, idFace1)) == NULL)
        return false;
    if((f2 = getFace(s, idFace2)) == NULL)
        return false;
    if((he1 = getHEd(f1, idVertex1, idVertex2)) == NULL)
        return false;
    if((he2 = getHEd(f2, idVertex1, idVertex3)) == NULL)
        return false;
    lmev(he1, he2, x, y, z);
    return true;
}

bool Scene::smev(int idSolid, int idFace, int idVertex1, float x, float y, float z) {
    Mesh *s;
    Face *f;
    HalfEdge *he;

    if((s = getSolid(idSolid)) == NULL)
        return false;
    if((f = getFace(s, idFace)) == NULL)
        return false;
    if((he = getHEd(f, idVertex1)) == NULL)
        return false;

    lmev(he, he, x, y, z);
    return true;
}

bool Scene::mef(int idSolid, int idFace, int idVertex1, int idVertex2, int idVertex3, int idVertex4) {
    Mesh *s;
    Face *f;
    HalfEdge *he1, *he2;

    if((s = getSolid(idSolid)) == NULL)
        return false;
    if((f = getFace(s, idFace)) == NULL)
        return false;
    if((he1 = getHEd(f, idVertex1, idVertex2)) == NULL)
        return false;
    if((he2 = getHEd(f, idVertex3, idVertex4)) == NULL)
        return false;

    lmef(he1, he2);
    return true;
}

bool Scene::smef(int idSolid, int idFace, int idVertex1, int idVertex2) {
    Mesh *s;
    Face *f;
    HalfEdge *he1, *he2;

    if((s = getSolid(idSolid)) == NULL)
        return false;
    if((f = getFace(s, idFace)) == NULL)
        return false;
    if((he1 = getHEd(f, idVertex1)) == NULL)
        return false;

    //if((he2 = getHEd(f, idVertex3, idVertex2)) == NULL)
    //    return false;
    he2 = he1;
    if(he2->origin->getId() != idVertex2)
        do {
            he2 = he2->next;
            if (he2 == he1)
                return false;
        } while(he2->origin->getId() != idVertex2);

    lmef(he1, he2);
    return true;
}

void Scene::removeSolid(int idSolid) {
    std::list<Mesh*>::iterator iter;
    for(iter = meshes.begin(); iter != meshes.end(); iter++)
        if ((*iter)->id == idSolid) {
            delete *iter;
            meshes.erase(iter);
            numMeshes--;
            return;
        }
}

void Scene::sweep(int idSolid, int idFace, float dx, float dy, float dz) {
    Mesh *m = getSolid(idSolid);
    Face *f = getFace(m, idFace);
    HalfEdge *first, *scan;
    Vertex *v;
    
    std::list<Loop*>::iterator lIter;
    for(lIter = f->loops.begin(); lIter != f->loops.end(); lIter++) {
        first = (*lIter)->hed;
        scan = first->next;
        v = scan->origin;
        lmev(scan, scan, v->x + dx, v->y + dy, v->z + dz);
        while(scan != first) {
            v = scan->next->origin;
            lmev(scan->next, scan->next, v->x + dx, v->y + dy, v->z + dz);
            lmef(scan->prev, scan->next->next);
            scan = scan->next->mate()->next;
        }
        lmef(scan->prev, scan->next->next);

    }
}


void Scene::rsweep(int idSolid, int idFace, int disc, float angle) {
    HalfEdge *first, *cfirst, *last, *scan;
    Mesh *m = getSolid(idSolid);
    Face *f = getFace(m, idFace);
    
    Quat q;
    q.fromAxisAngle(Vec3(1.0, 0.0, 0.0), angle/(float)disc);

    std::list<Loop*>::iterator lIter = f->loops.begin();
    first = (*lIter)->hed;

    while(first->edge != first->next->edge) 
        first = first->next;
    last = first->next;
    while(last->edge != last->next->edge)
        last = last->next;
    cfirst = first;
    while(disc--) {
        Vec3 vcoord(cfirst->next->next->origin->x, cfirst->next->next->origin->y, cfirst->next->next->origin->z);
        Vec3 v = rotateVec(vcoord, q);
        lmev(cfirst->next, cfirst->next, v.x, v.y, v.z);
        scan = cfirst->next;
        while(scan != last->next) {
            vcoord = Vec3(scan->prev->origin->x, scan->prev->origin->y, scan->prev->origin->z);
            v = rotateVec(vcoord, q);
            lmev(scan->prev, scan->prev, v.x, v.y, v.z);
            lmef(scan->prev->prev, scan->next);
            scan = scan->next->next->mate();
        }
        last = scan;
        cfirst = cfirst->next->next->mate();
    }
    while(cfirst != scan) {
        lmef(cfirst, cfirst->next->next->next);
        cfirst = cfirst->prev->mate()->prev;
    }
}

void Scene::updateBoundingBox() {
    std::list<Mesh*>::iterator mIter = meshes.begin();
    this->bbox = (*mIter)->bb;
    mIter++;
    for(; mIter != meshes.end(); mIter++) {
        if((*mIter)->bb.pMin.x < this->bbox.pMin.x)
            this->bbox.pMin.x = (*mIter)->bb.pMin.x;
        if((*mIter)->bb.pMax.x > this->bbox.pMax.x)
            this->bbox.pMax.x = (*mIter)->bb.pMax.x;

        if((*mIter)->bb.pMin.y < this->bbox.pMin.y)
            this->bbox.pMin.y = (*mIter)->bb.pMin.y;
        if((*mIter)->bb.pMax.y > this->bbox.pMax.y)
            this->bbox.pMax.y = (*mIter)->bb.pMax.y;

        if((*mIter)->bb.pMin.y < this->bbox.pMin.y)
            this->bbox.pMin.y = (*mIter)->bb.pMin.y;
        if((*mIter)->bb.pMax.y > this->bbox.pMax.y)
            this->bbox.pMax.y = (*mIter)->bb.pMax.y;
    }
}

void Scene::addCube(float minX, float minY, float minZ, float size)
{
    //v0, f0, s0
    mvfs(minX, minY, minZ+size);
    //v1
    smev(numMeshes - 1, 0, 0, minX+size, minY, minZ + size);
    //v2
    smev(numMeshes - 1, 0, 1, minX+size, minY+size, minZ + size);
    //v3
    smev(numMeshes - 1, 0, 2, minX, minY+size, minZ + size);
    //f1 -> front
    smef(numMeshes - 1, 0, 3, 0);

    sweep(numMeshes - 1, 0, 0, 0, -size);

    return;
}

void Scene::addCorner(float minX, float minY, float minZ,
                float midX, float midY, float midZ,
                float maxX, float maxY, float maxZ)
{
    //v0
    mvfs(minX, minY, minZ);
    //v1
    smev(numMeshes - 1, 0, 0, maxX, minY, minZ);
    //v2
    smev(numMeshes - 1, 0, 1, maxX, maxY, minZ);
    //v3
    smev(numMeshes - 1, 0, 2, midX, maxY, minZ);
    //v4
    smev(numMeshes - 1, 0, 3, midX, midY, minZ);
    //v5
    smev(numMeshes - 1, 0, 4, minX, midY, minZ);
    //f1 -> front
    smef(numMeshes - 1, 0, 5, 0);
    sweep(numMeshes - 1, 0, 0.0, 0.0, maxZ - minZ);
}

void Scene::addCylinder(float pX, float pY, float pZ, float radius, float height, int disc)
{
    int num = 0;

    mvfs(radius*cos(0) + pX, pY, radius*sin(0) + pZ);

    for(float i = 2.0*M_PI/(float)disc; i <= 2.0*M_PI; i+= 2.0*M_PI/(float)disc)
    {
        if(i+ 2.0*M_PI/(float)disc >= 2.0*M_PI)
        {
            smev(numMeshes - 1, 0, num, radius*cos(i) + pX, pY, radius*sin(i) + pZ);
            num++;
            smef(numMeshes - 1, 0, num, 0);
        }
        else
        {
            smev(numMeshes - 1, 0, num, radius*cos(i) + pX, pY, radius*sin(i) + pZ);
            num++;
        }
    }

    sweep(numMeshes - 1, 0, 0.0, height, 0.0);
}

void Scene::addSphere(float pX, float pY, float pZ, float radius, int disc) {
    float step = M_PI/disc;

    //          cos       sin
    mvfs(-1.0*radius, 0, 0);
    for(int i = 0, idNum = 0; i < disc; i++, idNum++) {
        float angle = M_PI - step*(i+1);
        smev(numMeshes - 1, 0, idNum, radius*cos(angle), radius*sin(angle), 0.0);
    }
    rsweep(numMeshes -1 , 0, 2*disc, 2*M_PI);
}

void Scene::addR2D2()
{
    float radius = 4.0;
    int disc = 20;
    float step = M_PI/disc;

    //Semi-esfera
    mvfs(-1.0*radius, 0, 0);
    for(int i = 0, idNum = 0; i < disc; i++, idNum++) {
        float angle = M_PI - step*(i+1);
        smev(numMeshes - 1, 0, idNum, radius*cos(angle), radius*sin(angle), 0.0);
    }
    rsweep(numMeshes -1 , 0, 2*disc, M_PI);

    //Cilindro central
    mvfs(radius, 0, 0);
    int idNum;
    for(int i = 0, idNum = 0; i < 2*disc; i++, idNum++) {
        float angle = step*(i+1);
        smev(numMeshes - 1, 0, idNum, radius*cos(angle), radius*sin(angle), 0.0);
    }
    smef(numMeshes - 1, 0, idNum, 0);
    sweep(numMeshes - 1, 0, 0, 0, -8);

    //Cubo central
    addCube(-0.5, -3.0, -9, 1);

    //Cilindro direita
    radius = 1.5;
    mvfs(radius + 6 , 0, 0);
    for(int i = 0, idNum = 0; i < 2*disc; i++, idNum++) {
        float angle = step*(i+1);
        smev(numMeshes - 1, 0, idNum, radius*cos(angle) + 6, radius*sin(angle), 0.0);
    }
    smef(numMeshes - 1, 0, idNum, 0);
    sweep(numMeshes - 1, 0, 0, 0, -8);

    //Cubo direita baixo
    addCube(5.5, -1.0, -9, 1);

    //Cilindro esquerda
    mvfs(radius - 6 , 0, 0);
    for(int i = 0, idNum = 0; i < 2*disc; i++, idNum++) {
        float angle = step*(i+1);
        smev(numMeshes - 1, 0, idNum, radius*cos(angle) - 6, radius*sin(angle), 0.0);
    }
    smef(numMeshes - 1, 0, idNum, 0);
    sweep(numMeshes - 1, 0, 0, 0, -8);

    //Cubo esquerda baixo
    addCube(-6.5, -1.0, -9, 1);

    //Cubo direita cima
    addCube(4.0, -0.75, -3.0, 1.5);

    //Cubo esquerda cima
    addCube(-4.75, -0.75, -3, 1);
}
