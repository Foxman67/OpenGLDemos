#include "App.h"
#include <stdlib.h>
#include <stdio.h>
#if _WIN32
#include <windows.h>
#endif

namespace engine
{
    void _fatalError(const char* errorDesc)
    {
        fprintf(stderr, "Error: %s\n", errorDesc);
        exit(1);
    }

    void _errorCallback(int error, const char* errorDesc)
    {
        _fatalError(errorDesc);
    }

    void _mouseScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
    {
        App* pApp = (App*)glfwGetWindowUserPointer(pWindow);
        pApp->setMouseScroll(glm::vec2((float)xoffset,(float)yoffset));
    }

    App::App(const std::string &name)
    : m_name(name)
    {
        _init();
        for(int i = 0; i<NUM_KEYS; ++i)
        {
            m_lastKeysDown[i]= false;
        }
    }

    App::~App()
    {

    }

    void App::_init()
    {
    #if _WIN32
        DWORD len = GetCurrentDirectory(0,NULL);
        char* dir = new char[len];
        GetCurrentDirectory(len,dir);
        printf("dir");
        if(strstr(dir,"\\Debug"))
        {
            int offset = strstr(dir, "\\Debug") - dir;
            if(offset == len - 7)
            {
                char* newDir = new char[len + strlen("\\..\\..")];
                sprintf(newDir, "%s%s", dir, "\\..\\..");
                SetCurrentDirectory(newDir);
                delete[] newDir;
            }
        }
        delete[] dir;
    #endif
        if(!glfwInit())
            _fatalError("Failed to init GLFW\n");
        
        printf("GLFW initalized");
        glfwSetErrorCallback(_errorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        int resX = 1280;
        int resY = 720;
        GLFWmonitor* pMonitorToUse = nullptr;

        m_pWindow = glfwCreateWindow(resX,resY,m_name.c_str(),pMonitorToUse,NULL);
        if(!m_pWindow)
            _fatalError("Winodw creation failed\n");
        
        glfwSetWindowUserPointer(m_pWindow,this);
        glfwSetScrollCallback(m_pWindow, _mouseScrollCallback);
        glfwMakeContextCurrent(m_pWindow);
        glfwSwapInterval(1);

        GLenum err = glewInit();
        if(GLEW_OK != err)
            _fatalError((const char*)glewGetErrorString(err));
        else
            printf("GLEW initialized\n");
    }

    void App::_internalUpdate(float dt)
    {
            update(dt);
            for(int i = 0; i<NUM_KEYS; ++i)
            {
                m_lastKeysDown[i] = isKeyDown(GLFW_KEY_SPACE + i);
            }
    }


    void App::run()
    {
        double lastTime = glfwGetTime();
        while(!glfwWindowShouldClose(m_pWindow) && !isKeyDown(GLFW_KEY_ESCAPE))
        {
            double currTime = glfwGetTime();
            float elapsedTime = (float)(currTime-lastTime);
            lastTime = currTime;

            glfwGetFramebufferSize(m_pWindow, &m_width, &m_height);
            if(m_width !=0 && m_height != 0)
            {
                glViewport(0,0,m_width,m_height);

                _internalUpdate(elapsedTime);
                render();
            }

            m_mouseScroll = glm::vec2(0.0f,0.0f);
            glfwSwapBuffers(m_pWindow);
            glfwPollEvents();
        }
    }

    void App::setMouseScroll(const glm::vec2& scroll)
    {
        m_mouseScroll = scroll;
    }

    bool App::isKeyDown(int key) const
    {
        key = toupper(key);
        return glfwGetKey(m_pWindow,key) == GLFW_PRESS;
    }

    bool App::isKeyJustDown(int key) const
    {
        key = toupper(key);
        return glfwGetKey(m_pWindow,key) == GLFW_PRESS && !m_lastKeysDown[key-GLFW_KEY_SPACE];
    }

    bool App::isLMBDown() const
    {
        int state = glfwGetMouseButton(m_pWindow,GLFW_MOUSE_BUTTON_LEFT);
        return state == GLFW_PRESS;
    }

    bool App::isRMBDown() const
    {
        int state = glfwGetMouseButton(m_pWindow,GLFW_MOUSE_BUTTON_RIGHT);
        return state == GLFW_PRESS;
    }

    bool App::isMMBDown() const
    {
        int state = glfwGetMouseButton(m_pWindow,GLFW_MOUSE_BUTTON_MIDDLE);
        return state == GLFW_PRESS;
    }

    glm::vec2 App::getMousePos() const
    {
        double xpos, ypos;
        glfwGetCursorPos(m_pWindow, &xpos, &ypos);
        return glm::vec2((float)xpos,(float)ypos);
    }

    glm::vec2 App::getScreenSize() const
    {
        int w,h;
        glfwGetFramebufferSize(m_pWindow, &w, &h);
        return glm::vec2((float)w,(float)h);
    }
}

