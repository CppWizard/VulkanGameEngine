#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    struct UniformBufferObject
    {
        glm::mat4 View;
        glm::mat4 Projection;
    };
}