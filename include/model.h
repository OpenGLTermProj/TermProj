#pragma once
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <camera.h>
#include "stb_image.h"

bool LoadModel(const char *path, MODEL &model)
{
    // init variables
    char headerState = ' ';
    int index = 0;

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Impossible to open the file !\n");
        return false;
    }

    while (1)
    {
        int res = 0;
        char lineHeader[128];
        // read the first word of the line

        res = fscanf(file, "%s", lineHeader);
        if (res == -1)
            break;
        if (strcmp(lineHeader, "v") == 0)
        {

            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            model.vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            model.uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            model.normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {

            headerState = 'f';
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            model.faces.push_back(ivec3(vertexIndex[0] - 1, vertexIndex[1] - 1, vertexIndex[2] - 1));
            model.uvIndices.push_back(uvIndex[0] - 1);
            model.uvIndices.push_back(uvIndex[1] - 1);
            model.uvIndices.push_back(uvIndex[2] - 1);
            model.normalIndices.push_back(normalIndex[0] - 1);
            model.normalIndices.push_back(normalIndex[1] - 1);
            model.normalIndices.push_back(normalIndex[2] - 1);
        }
    }
}

void DrawModel(MODEL model)
{
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < model.faces.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vec3 p = model.vertices[model.faces[i][j]];

            if (model.normals.size() == model.vertices.size())
            {
                vec3 n = model.normals[model.faces[i][j]];
                glNormal3f(n[0], n[1], n[2]);
            }
            glVertex3f(p[0], p[1], p[2]);
        }
    }
    glEnd();
}

unsigned int loadTexture(const char *path, GLuint &texture_ID)
{

    glGenTextures(1, &texture_ID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << std::endl;
        stbi_image_free(data);
    }
    return texture_ID;
}

