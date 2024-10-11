#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>

#include "Model.hpp"

class ObjectData
{
private:
    GLuint VBO = 0;
    GLuint VAO = 0;
    Model model;
    std::function<void()> sliceAttrs;
    size_t numberOfAttrs;

public:
    ObjectData(Model model, size_t numberOfAttrs = 1, std::function<void()> sliceAttrs = ObjectData::defaultSlice);
    ObjectData(const ObjectData &other);
    ObjectData(ObjectData &&other);

    static void defaultSlice();

    void setArray() const;

    // operators
    bool operator==(const ObjectData &other) const;
};