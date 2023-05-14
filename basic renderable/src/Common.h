#pragma once
#include <stdio.h>
#include <string>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine
{
    //loads a whole file in. user is responsable for handling the return.
    void* LoadFile(const std::string& filename, int* pOutFileSize = nullptr);
    
    //loads vertex and pixel shader
    GLuint LoadShaders(const std::string& vsh, const std::string& fsh);
}