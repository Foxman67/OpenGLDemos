#include "App.h"
#include "Scene.h"
#include "Renderable.h"
class Application: public engine::App
{
public:
    Application() : App("Sample")
    {
        Renderable* pRender = new Renderable(this);
        pRender->init();
        m_pScene = pRender;

    }

    ~Application()
    {

    }

    void update (float dt) override
    {
        if(m_pScene != nullptr)
        {
            m_pScene->update(dt);
        }
    }

    void render() override
    {
        m_pScene->render(m_width, m_height);

    }
private:
    Scene* m_pScene;
};

int main(int, char**)
{
    Application app;
    app.run();
}