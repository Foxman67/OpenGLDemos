#include "Renderable.h"

struct cubeVertex{
    GLfloat x,y,z;
    GLubyte r,g,b,a;
};
static const cubeVertex vertices[] = {
    //assuming cube central origin is 0,0,0
    {0.5f, 0.5f, 0.5f, 100, 100, 100, 255},   // x, y, z 0
    {0.5f, 0.5f, -0.5f, 50, 50, 50, 255},  // x, y,-z 1
    {0.5f, -0.5f, 0.5f, 75, 75, 75, 255},  // x,-y, z 2
    {-0.5f, 0.5f, 0.5f, 25, 25, 25, 255},  //-x, y, z 3
    {0.5f, -0.5f, -0.5f, 100, 100, 100, 255}, // x,-y,-z 4
    {-0.5f, 0.5f, -0.5f, 75, 75, 75, 255}, //-x, y,-z 5
    {-0.5f, -0.5f, 0.5f, 50, 50,50, 255}, //-x,-y, z 6
    {-0.5f, -0.5f, -0.5f, 25, 25, 25, 255},//-x,-y,-z 7

};
static const GLushort cubeindices[]=
{
    //front +z
    6,3,0,
    0,2,6,
    // back -z
    1,5,7,
    7,4,1,
    //bot -y
    6,2,4,
    4,7,6,
    // top y
    5,1,0,
    0,3,5,
    //right x
    0,1,4,
    4,2,0,
    //left -x
    5,3,6,
    6,7,5,

};
GLuint Renderable::m_uiProgram = 0;
GLuint Renderable::m_uiVBO = 0;
GLuint Renderable::m_uiIBO = 0;
GLuint Renderable::m_uiVAO = 0;
int Renderable::count = 0;
GLint Renderable::m_iProjViewUni = 0;
GLint Renderable::m_iWorldUni = 0;
GLint Renderable::m_iColorUni = 0;
Renderable::~Renderable()
{
    count--;
    if(count == 0)
    {
        //cleanup when nobody is left
        glDeleteProgram(m_uiProgram);
        glDeleteBuffers(1,&m_uiVBO);
        glDeleteBuffers(1,&m_uiIBO);
        glDeleteVertexArrays(1,&m_uiVAO);
    }
}
void Renderable::init()
{
    if(!m_uiProgram)
    {
        glEnable(GL_DEPTH_TEST);
        m_uiProgram = engine::LoadShaders("data/cube.vsh","data/cube.fsh");

        glGenVertexArrays(1,&m_uiVAO);
        glBindVertexArray(m_uiVAO);
        
        glGenBuffers(1, &m_uiVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertex)*8, vertices, GL_STATIC_DRAW);
        //vertex data bound

        glGenBuffers(1, &m_uiIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6*2*3, cubeindices, GL_STATIC_DRAW);

        //indicie data bound, 6 faces of 2 triangle of 3 vertexs
        int posAttr = glGetAttribLocation(m_uiProgram, "a_position");
        int colorAttr = glGetAttribLocation(m_uiProgram, "a_color");

        glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

        glVertexAttribPointer(posAttr,3,GL_FLOAT,GL_FALSE,sizeof(cubeVertex),0);
        glEnableVertexAttribArray(posAttr);

        glVertexAttribPointer(colorAttr,4,GL_UNSIGNED_BYTE,GL_TRUE,sizeof(cubeVertex),(void *)(sizeof(GLfloat)*3));
        glEnableVertexAttribArray(colorAttr);
        //get the uniform locations
        m_iProjViewUni = glGetUniformLocation(m_uiProgram,"projview");
        m_iWorldUni = glGetUniformLocation(m_uiProgram,"world");
        m_iColorUni = glGetUniformLocation(m_uiProgram,"color");

        printf("init was successful");
    }
    count++;
}

void Renderable::update(float dt)
{
    m_iRot += dt;
}

void Renderable::render(int width, int height)
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //last few matrix calculations
    glm::mat4 mView = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(10.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 mProj = glm::perspective(glm::radians(60.0f),(float)width/(float)height,0.1f,1000.0f);
    glm::mat4 mProjView = mProj*mView;

    glm::mat4 mWorld = glm::translate(glm::vec3(10.0f,0.0f,0.0f)) *
             glm::rotate(glm::mat4(1.0f),m_iRot,glm::vec3(1.0f,0.0f,0.0f))*
             glm::rotate(glm::mat4(1.0f),m_iRot*2,glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4  mColor = glm::translate(glm::vec3(m_fR,m_fG,m_fB));
    //program bindings
    glUseProgram(m_uiProgram);
    glUniformMatrix4fv(m_iProjViewUni,1,GL_FALSE, glm::value_ptr(mProjView));
    glUniformMatrix4fv(m_iWorldUni,1,GL_FALSE,glm::value_ptr(mWorld));
    glUniformMatrix4fv(m_iColorUni,1,GL_FALSE,glm::value_ptr(mColor));
    glBindVertexArray(m_uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER,m_uiVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_uiIBO);

    glDrawElements(GL_TRIANGLES,6*3*2,GL_UNSIGNED_SHORT,0);

}