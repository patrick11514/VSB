#include "Object.hpp"

Object::Object(float *points)
{
    this->points = points;
    glGenBuffers(1, &this->VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->points), this->points, GL_STATIC_DRAW);

    // Vertex Array Object (VAO)
    glGenVertexArrays(1, &this->VAO); // generate the VAO
    glBindVertexArray(this->VAO);     // bind the VAO
    glEnableVertexAttribArray(0);     // enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

Object::~Object()
{
    delete[] this->points;
}

void Object::setArray()
{
    printf("Setting array %d\n", this->VAO);
    glBindVertexArray(this->VAO);
}

bool Object::operator==(const Object &other) const
{
    return this->VAO == other.VAO;
}