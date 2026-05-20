#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    struct PushConstants
    {
        glm::mat4 Model = glm::mat4(1.0f);
    };
}