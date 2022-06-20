/*
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"
#include <stdio.h>

//Texture
unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];  

//Mouse
static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
float g_fDistance = -10.0f;

void DrawEmptyCard() {
    glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -2.0f, 1.0f);  //¾Õ¸é
    glTexCoord2f(0.34f, 0.0f); glVertex3f(1.0f, -2.0f, 1.0f);
    glTexCoord2f(0.34f, 0.5f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.35f, 0.0f); glVertex3f(1.0f, -2.0f, 0.95f);  //µÞ¸é
    glTexCoord2f(0.68f, 0.0f); glVertex3f(-1.0f, -2.0f, 0.95f);
    glTexCoord2f(0.68f, 0.5f); glVertex3f(-1.0f, 1.0f, 0.95f);
    glTexCoord2f(0.35f, 0.5f); glVertex3f(1.0f, 1.0f, 0.95f);

    glTexCoord2f(0.68f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.95f);  //À­¸é
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.68f, 0.51f); glVertex3f(1.0f, 1.0f, 0.95f);

    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f, -2.0f, 0.95f);  //¾Æ·§¸é
    glTexCoord2f(0.34f, 0.5f); glVertex3f(1.0f, -2.0f, 0.95f);
    glTexCoord2f(0.34f, 1.0f); glVertex3f(1.0f, -2.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -2.0f, 1.0f);

    glTexCoord2f(0.34f, 0.5f); glVertex3f(1.0f, -2.0f, 0.95f);  //¿ìÃø¸é
    glTexCoord2f(0.68f, 0.5f); glVertex3f(1.0f, 1.0f, 0.95f);
    glTexCoord2f(0.68f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.34f, 1.0f); glVertex3f(1.0f, -2.0f, 1.0f);

    glTexCoord2f(0.68f, 0.5f); glVertex3f(-1.0f, -2.0f, 0.95f);  //ÁÂÃø¸é
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-1.0f, -2.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.68f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.95f);

}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();									

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    DrawEmptyCard();
    glEnd();
    glutSwapBuffers();
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

AUX_RGBImageRec* LoadBMP(const char* szFilename) {
    FILE* pFile = NULL;
    if (!szFilename) {
        return NULL;
    }

    pFile = fopen(szFilename, "r");
    if (pFile) {
        fclose(pFile);
        return auxDIBImageLoad(szFilename);     
    }
    return NULL;
}

int LoadGLTextures(const char* szFilePath) {     
    int Status = FALSE;
    glClearColor(0.0, 0.0, 0.0, 0.5);
    memset(pTextureImage, 0, sizeof(void*) * 1);   

    if (pTextureImage[0] = LoadBMP(szFilePath)) {  
        Status = TRUE;                              
        glGenTextures(1, &MyTextureObject[0]);     
        glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
            pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEnable(GL_TEXTURE_2D);
    }
    if (pTextureImage[0]) {                
        if (pTextureImage[0]->data) {      
            free(pTextureImage[0]->data);   
        }
        free(pTextureImage[0]);             
    }
    return Status;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Empty Card");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);

    if (LoadGLTextures("Texture/emptycard.bmp")) {
        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glutMainLoop();
    }


}


*/
