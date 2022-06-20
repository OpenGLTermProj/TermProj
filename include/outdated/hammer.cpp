/*
#define _CRT_SECURE_NO_WARNINGS
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"
#include <stdio.h>

//Mouse
static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
float g_fDistance = -10.0f;

void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void DrawHammer() {
    glColor3f(1.0, 0.0, 0.0);
    glScalef(0.8, 1, 1);
    glTranslatef(-4.0f, 2.0f, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(1.0f, 0, 0);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidCube(1.0f);
    glTranslatef(4.8f, 0, 0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(-0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(-0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(-0.7f, 0, 0);
    glutSolidSphere(1, 40, 16);
    glTranslatef(-1.0f, 0, 0);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidCube(1.0f);
    glTranslatef(-0.85f, -3.0f, 0);
    glScalef(1, 7, 1);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidCube(1.0f);
    glFlush();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();									

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    DrawHammer();
   
    glEnd();
    glutSwapBuffers();
}

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Hammer");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    glutMainLoop();
}

*/





