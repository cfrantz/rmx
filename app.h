#ifndef PROJECT_APP_H
#define PROJECT_APP_H
#include <memory>
#include <string>
#include <SDL2/SDL.h>

//#include "gfx/raymarch.h"
#include "gfx/swmarch.h"
#include "imwidget/imapp.h"

namespace project {

class App: public ImApp {
  public:
    App(const std::string& name) : ImApp(name, 1280, 720) {}
    ~App() override {}

    void Init() override;
    void ProcessEvent(SDL_Event* event) override;
    void ProcessMessage(const std::string& msg, const void* extra) override;
    bool PreDraw() override;
    void Draw() override;

    void Help(const std::string& topickey);
  private:
    std::string save_filename_;
    std::unique_ptr<GFX::SWMarcher> scene_;
    float theta_, phi_;
    static constexpr float TAU = 3.141592654f * 2.0f;
};

}  // namespace project
#endif // PROJECT_APP_H
