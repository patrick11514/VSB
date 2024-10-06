#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

class Object
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    float *points;

public:
    Object(float *points, size_t count /* TODO: args for glVertexAttribPointer */);
    ~Object();

    void setArray();

    // operators
    bool operator==(const Object &other) const;
};