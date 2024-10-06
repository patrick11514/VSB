#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

class Object
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;

public:
    Object(float *points /* TODO: args for glVertexAttribPointer */);

    void setArray();

    // operators
    bool operator==(const Object &other) const;
};