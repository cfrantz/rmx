#include <cstdio>

#include <gflags/gflags.h>
#include <GL/glew.h>

#include "app.h"
#include "imgui.h"
#include "absl/memory/memory.h"
#include "absl/strings/match.h"
#include "imwidget/error_dialog.h"
#include "util/browser.h"
#include "util/os.h"
#include "util/logging.h"
#include "util/imgui_impl_sdl.h"
#include "glm/gtc/type_ptr.hpp"

#include "version.h"

#ifdef HAVE_NFD
#include "nfd.h"
#endif


namespace project {

void App::Init() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOGF(FATAL, "GLEW Error: %s\n", glewGetErrorString(err));
        abort();
    }
    clear_color_ = ImVec4(0, 0, 0, 0);
    theta_ = 0;
    phi_ = 0;

#if 1
    scene_ = absl::make_unique<GFX::RayMarchScene>(1920, 1080);
    if (scene_->LoadProgram("content/raymarch.vs", "content/raymarch.fs")) {
        LOGF(INFO, "Shader program loaded.");
    }
    scene_->Init();
#else
    scene_ = absl::make_unique<GFX::SWMarcher>(256, 256);
#endif
}

void App::ProcessEvent(SDL_Event* event) {
}

void App::ProcessMessage(const std::string& msg, const void* extra) {
}

bool App::PreDraw() {
    ImGuiIO& io = ImGui::GetIO();
    auto* c = scene_->camera();
    float speed = 0.1f;
    float rspeed = 0.05f;

    if (io.KeysDown[SDL_SCANCODE_Q]) {
        c->eye -= c->right * speed;
    } else if (io.KeysDown[SDL_SCANCODE_E]) {
        c->eye += c->right * speed;
    }
    if (io.KeysDown[SDL_SCANCODE_W]) {
        c->eye += c->forward * speed;
    } else if (io.KeysDown[SDL_SCANCODE_S]) {
        c->eye -= c->forward * speed;
    }
    if (io.KeysDown[SDL_SCANCODE_A]) {
        theta_ += rspeed;
    } else if (io.KeysDown[SDL_SCANCODE_D]) {
        theta_ -= rspeed;
    }
    if (io.KeysDown[SDL_SCANCODE_2]) {
        phi_ -= rspeed;
    } else if (io.KeysDown[SDL_SCANCODE_X]) {
        phi_ += rspeed;
    }



    float cost = cosf(theta_), sint = sinf(theta_);
    float cosp = cosf(phi_), sinp = sinf(phi_);

    c->forward = glm::vec3(sint*cosp, -sinp, cost*cosp);
    c->right = glm::vec3(cost, 0.0f, -sint);
    c->up = glm::normalize(glm::cross(c->forward, c->right));

    /*
    LOGF(INFO, "eye = (%.2f, %.2f, %.2f)", c->eye.x, c->eye.y, c->eye.z);
    LOGF(INFO, "theta=%.2f phi=%.2f", theta_, phi_);
    LOGF(INFO, "up = (%.2f, %.2f, %.2f)", c->up.x, c->up.y, c->up.z);
    LOGF(INFO, "right = (%.2f, %.2f, %.2f)", c->right.x, c->right.y, c->right.z);
    LOGF(INFO, "forward = (%.2f, %.2f, %.2f)", c->forward.x, c->forward.y, c->forward.z);
    */


    glViewport(0, 0,
               (int)ImGui::GetIO().DisplaySize.x,
               (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(clear_color_.x, clear_color_.y, clear_color_.z, clear_color_.w);
    glClear(GL_COLOR_BUFFER_BIT);
    scene_->Draw();
    return true;
}

void App::Draw() {
    ImGui::SetNextWindowSize(ImVec2(500,300), ImGuiSetCond_FirstUseEver);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                char *filename = nullptr;
                auto result = NFD_OpenDialog("z2prj;nes", nullptr, &filename);
                if (result == NFD_OKAY) {
                    // DOSTUFF
                    save_filename_.assign(filename);
                }
                free(filename);
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                if (save_filename_.empty())
                    goto save_as;
                // DOSTUFF
            }
            if (ImGui::MenuItem("Save As")) {
save_as:
                char *filename = nullptr;
                auto result = NFD_SaveDialog("z2prj", nullptr, &filename);
                if (result == NFD_OKAY) {
                    std::string savefile = filename;
                    if (absl::EndsWith(savefile, ".z2prj")) {
                        save_filename_.assign(savefile);
                        // DOSTUFF
                    } else {
                        ErrorDialog::Spawn("Bad File Extension",
                            ErrorDialog::OK | ErrorDialog::CANCEL,
                            "Project files should have the extension .z2prj\n"
                            "If you want to save a .nes file, use File | Export\n\n"
                            "Press 'OK' to save anyway.\n")->set_result_cb(
                                [=](int result) {
                                    if (result == ErrorDialog::OK) {
                                        save_filename_.assign(savefile);
                                        // DOSTUFF
                                    }
                                });
                    }
                }
                free(filename);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Debug Console", nullptr,
                            &console_.visible());
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Online Help")) {
                Help("root");
            }
            if (ImGui::MenuItem("About")) {
                ErrorDialog::Spawn("About App",
                    "Empty Project\n\n",
#ifdef BUILD_GIT_VERSION
                    "Version: ", BUILD_GIT_VERSION, "-", BUILD_SCM_STATUS
#else
                    "Version: Unknown"
#warning "Built without version stamp"
#endif
                    );

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

#if 0
    if (!loaded_) {
        char *filename = nullptr;
        auto result = NFD_OpenDialog("z2prj,nes", nullptr, &filename);
        if (result == NFD_OKAY) {
            project_.Load(filename, false);
            if (ends_with(filename, ".z2prj")) {
                save_filename_.assign(filename);
            }
        }
        free(filename);
    }
#endif
    if (ImGui::Begin("stuff")) {
        ImGui::ColorEdit4("Sky Color", glm::value_ptr(scene_->sky_color_));
        ImGui::ColorEdit4("Ambient", glm::value_ptr(scene_->ambient_));
        ImGui::ColorEdit4("Light0", glm::value_ptr(scene_->light0col_));
        ImGui::InputInt("Operation", &scene_->op_);
        ImGui::End();
    }
#if 0
    if (ImGui::Begin("Scene")) {
        scene_->Draw();
        ImGui::End();
    }
#endif

}

void App::Help(const std::string& topickey) {
}

}  // namespace project
