#pragma once
#include "Scene.h"

class Renderable : public Scene
{
    public:
        Renderable(engine::App* pApp) : Scene(pApp,"basic renderable"){}

        virtual ~Renderable();

        void init();

        void update(float dt);
        void render(int width, int height);
        // just basic color setting
        void setColor(float r, float g, float b) {m_fR = r; m_fG = g; m_fB = b;}
    private:

    float m_fR = 1.0f;
    float m_fG = 0.0f;
    float m_fB = 0.0f;
    float m_iRot = 0;
    static GLuint m_uiProgram;
    static GLuint m_uiVBO;
    static GLuint m_uiIBO;
    static GLuint m_uiVAO;
    static GLint m_iProjViewUni;
    static GLint m_iWorldUni;
    static GLint m_iColorUni;
    static int count;
};