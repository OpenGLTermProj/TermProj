
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"
#include <stdio.h>
#include <math.h>
#define SLICES  100
#define STACKS  100
GLUquadricObj* qobj;

//Mouse
static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
float g_fDistance = -10.0f;

GLfloat Arm_x = 0;
GLfloat Hand_x = 0;
GLfloat Foot_x = 0;
GLfloat turn = 0;
int temp = 0;
static double time = 0;

void drawLeaf(int rot) {
    glColor3f(0.008, 0.4, 0.19);

    //left
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-1.0f, 3.0f, 0);
    glRotatef(45.0f, 0.0, 0.0, 1.0);
    glScalef(1.0, 2.0, 0.5);
    glutSolidSphere(0.5, SLICES, STACKS);
    glPopMatrix();

    //right
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.5f, 3.0, 0.0);
    glRotatef(45.0f, 0.0, 0.0, -1.0);
    glScalef(1.0, 2.0, 0.5);
    glutSolidSphere(0.5, SLICES, STACKS);
    glPopMatrix();

}
void drawHead(int rot) {

    //head
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.25, 1.5f, 0.0);
    glScalef(1.3, 1.4, 1.3);
    glutSolidSphere(0.8, SLICES, STACKS);
    glPopMatrix();

    //mouth
    glPushMatrix();
    glColor3f(0.8, 0.3, 0.23);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.2f, 0.8f, 0.8);
    glScalef(0.8, 1.0, 0.5);
    glutSolidSphere(0.3, SLICES, STACKS);
    glPopMatrix();

}

void drawEyes(int rot) {
    float earRadius = 0.5;

    //left
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.6f, 1.5f, 0.85f);
    glScalef(1.2, 1.5, 1.0);
    glutSolidSphere(0.2, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.65f, 1.5f, 1.0f);
    glScalef(0.9, 1.4, 0.9);
    glutSolidSphere(0.1, SLICES, STACKS);
    glPopMatrix();

    //right
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.1f, 1.5f, 0.85f);
    glScalef(1.2, 1.5, 1.0);
    glutSolidSphere(0.2, SLICES, STACKS);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.14f, 1.5f, 1.0f);
    glScalef(0.9, 1.4, 0.9);
    glutSolidSphere(0.1, SLICES, STACKS);
    glPopMatrix();

}

void drawBody(int rot) {
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_SMOOTH);

    //1
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.25, 0.5f, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(1.0, 0.6, 1.0);
    gluCylinder(qobj, 0.3, 0.3, 0.4, SLICES, STACKS);
    glPopMatrix();


    //2
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.25, -0.1f, 0.0);
    glRotatef(90, -1.0, 0.0, 0.0);
    glScalef(1.2, 0.6, 0.6);
    glutSolidSphere(0.75, SLICES, STACKS);
    glPopMatrix();


    //3
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.25, -1.0f, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(1.2, 0.9, 1.0);
    glutSolidSphere(1.0, SLICES, STACKS);
    glPopMatrix();


}

void drawArms(int x, int rot) {
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_SMOOTH);

    //left ¾î±ú
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.9f, -0.2f, 0.0);
    glutSolidSphere(0.3, SLICES, STACKS);
    glPopMatrix();

    //left
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-1.0f, -0.2f, 0.0);
    glRotatef(x, 1, 0, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(20, 0.0, -1.0, 0.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(qobj, 0.2, 0.2, 1.35, SLICES, STACKS);
    glPopMatrix();

    //right ¾î±ú
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.4f, -0.2f, 0.0);
    glutSolidSphere(0.3, SLICES, STACKS);
    glPopMatrix();

    //right
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.5f, -0.2f, 0.0);
    glRotatef(x, -1, 0, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(-20, 0.0, -1.0, 0.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(qobj, 0.2, 0.2, 1.35, SLICES, STACKS);
    glPopMatrix();


}

void drawHands(int x, int rot) {
    glColor3f(0.25, 1.0, 0.35);

    //Left
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glRotatef(x, 1, 0, 0);
    glTranslatef(-1.48f, -1.5f, 0.0);
    glScalef(1.0, 1.0, 1.0);
    glutSolidSphere(0.22, SLICES, STACKS);
    glPopMatrix();


    //Right
    glPushMatrix();
    glRotatef(rot, 0.0, 1.0, 0.0);
    glRotatef(x, -1, 0, 0);
    glTranslatef(0.98f, -1.5f, 0.0);
    glScalef(1.0, 1.0, 1.0);
    glutSolidSphere(0.22, SLICES, STACKS);
    glPopMatrix();

}

void drawLegs(int x, int rot) {
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_SMOOTH);

    //Left
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(-0.7f, -1.7f, 0.0);
    glRotatef(x, 1, 0, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(qobj, 0.24, 0.24, 1.45, SLICES, STACKS);
    glPopMatrix();


    //Right
    glPushMatrix();
    glColor3f(0.25, 1.0, 0.35);
    glRotatef(rot, 0.0, 1.0, 0.0);
    glTranslatef(0.2f, -1.7f, 0.0);
    glRotatef(x, -1, 0, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, 1.0);
    gluCylinder(qobj, 0.24, 0.24, 1.45, SLICES, STACKS);
    glPopMatrix();

}


void DrawUser() {
    drawLeaf(turn);
    drawHead(turn);
    drawEyes(turn);
    drawBody(turn);
    drawArms(Arm_x, turn);
    drawHands(Hand_x, turn);
    drawLegs(Arm_x, turn);
}
void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    DrawUser();

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
void move() {
    if (Arm_x == 70) {
        temp = 0;
    }
    else if (Arm_x == -70) {
        temp = 1;
    }
    if (temp == 0) {
        Arm_x -= 10;
        Hand_x -= 9;
        Foot_x -= 5;
    }
    else if (temp == 1) {
        Arm_x += 10;
        Hand_x += 9;
        Foot_x += 5;
    }
}

void MyKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'w':     
        turn = 0;
        move();
        glutPostRedisplay();
        break;
    case 'a':    
        turn = 90;
        move();
        glutPostRedisplay();
        break;
    case 's':     
        turn = 180;
        move();
        glutPostRedisplay();
        break;
    case 'd':      
        turn = -90;
        move();
        glutPostRedisplay();
        break;
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("User");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    glutKeyboardFunc(MyKeyboard);
    glutMainLoop();
}





