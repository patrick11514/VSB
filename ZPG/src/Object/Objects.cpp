#include "Object.hpp"
#include "Objects.hpp"
#include "Model.hpp"
#include "ObjectData.hpp"

#include "Models/sphere.h"

Object createBall(const ShaderProgram &shaderProgram, const Transformations &transformations)
{
    Model model{sphere, sizeof(sphere) / sizeof(float)};
    ObjectData data(model,
                    2,
                    []()
                    {
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
                    });

    return Object(data, shaderProgram, transformations, []()
                  { glDrawArrays(GL_TRIANGLES, 0, 2880); });
}
