
#define _CRT_SECURE_NO_WARNINGS

#include "include/model.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>


#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>



float g_fDistance = -4.5f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;


static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

std::vector < glm::vec3 > vertices;
std::vector < glm::ivec3 > faces;
std::vector < glm::vec2 > uvs;
std::vector < glm::vec3 > normals;


// 조명
GLfloat light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1, 1, 1, 1.0 };




void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

void InitLight()
{
    GLfloat LightPosition[] = { 0.0, 2.0, 0.0, 1.0 };
    glEnable(GL_LIGHTING);      //조명 활성화
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb); //주변광 설정
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //확산광 설정
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //반사광 설정
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

}

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);    //구로 셰이딩
    glEnable(GL_DEPTH_TEST); // 깊이버퍼
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_FRONT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    InitLight();
}



void DrawSurface(std::vector < glm::vec3 >& vectices, 
    std::vector < glm::vec3 >& normals, 
    std::vector < glm::ivec3 >& faces)
{
    glBegin(GL_TRIANGLES);
    for (int i = 0 ;i < faces.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            glm::vec3 p = vertices[faces[i][j]];

            if (normals.size() == vertices.size())
            {
                glm::vec3 n = normals[faces[i][j]];
                glNormal3f(n[0], n[1], n[2]);
            }
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    
    glEnd();
}

void DrawWireSurface(std::vector < glm::vec3 >& vectices,
    std::vector < glm::ivec3 >& faces)
{
    glBegin(GL_LINES);
    for (int i = 0; i < faces.size(); i++)
    {
        glm::vec3 p1 = vertices[faces[i][0]];
        glm::vec3 p2 = vertices[faces[i][1]];
        glm::vec3 p3 = vertices[faces[i][2]];
        
        glVertex3f(p1[0], p1[1], p1[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        glVertex3f(p3[0], p3[1], p3[2]);
        glVertex3f(p3[0], p3[1], p3[2]);
        glVertex3f(p1[0], p1[1], p1[2]);
    }
    glEnd();
}

void DrawHUD()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(0.05, 0.05);
    glVertex2f(0.3, 0.05);
    glVertex2f(0.3, 0.15);
    glVertex2f(0.05, 0.15);
    glEnd();
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);
    glScalef(18, 18, 18);

    //Draw here
    //DrawSurface(vertices,normals,faces);
    

    DrawHUD();
    glutSwapBuffers();
}



void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("171413 황희재");
    init();

    //LoadObj("../Data/bunny/bunny.obj", vertices, faces, uvs, normals);
  

    glutDisplayFunc(render);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);

    glutMainLoop();
}