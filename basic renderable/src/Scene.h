#pragma once
#include "App.h"
#include <string>
#include <ctime>

namespace engine {class App;}

class Scene 
{
    public:
        Scene(engine::App* pApp, const std::string& name) : m_name(name), m_pApp(pApp){}
        virtual ~Scene(){}

        const std::string& getName() const {return m_name;}

        virtual void init() = 0;
        virtual void update(float dt) = 0;
        virtual void render(int width, int height) = 0;

    protected:
        std::string m_name;
        engine::App* m_pApp; 
};