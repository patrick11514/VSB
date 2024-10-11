#include "ObjectData.hpp"

ObjectData::ObjectData(Model &model)
{
    glGenBuffers(1, &this->VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    model.putDataToBuffer();

    // Vertex Array Object (VAO)
    glGenVertexArrays(1, &this->VAO); // generate the VAO
    glBindVertexArray(this->VAO);     // bind the VAO
    glEnableVertexAttribArray(0);     // enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void ObjectData::setArray()
{
    glBindVertexArray(this->VAO);
}

bool ObjectData::operator==(const ObjectData &other) const
{
    return this->VAO == other.VAO;
}
