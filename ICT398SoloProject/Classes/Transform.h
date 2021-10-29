#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace solo
{
    struct Transform
    {
    public:
        glm::vec3 position = {0, 0, 0};
        glm::vec3 scale = {0, 0, 0};
        glm::quat orietnation = {1, 0, 0, 0};
    };   
}
