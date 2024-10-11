#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "Model.hpp"

class ObjectData
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;

public:
    ObjectData(Model &model /* TODO: args for glVertexAttribPointer */);

    void setArray();

    // operators
    bool operator==(const ObjectData &other) const;
};