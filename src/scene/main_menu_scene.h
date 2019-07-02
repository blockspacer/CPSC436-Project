//
// Created by alex on 21/02/19.
//

#ifndef PANDAEXPRESS_MAIN_MENU_SCENE_H
#define PANDAEXPRESS_MAIN_MENU_SCENE_H

#include <cinttypes>
#include <vector>
#include "../systems/sprite_transform_system.h"
#include "../util/blackboard.h"
#include <systems/pause_menu_transform_system.h>
#include <systems/render_system.h>
#include "scene.h"



class MainMenuScene : public Scene {
private:
    Sprite splash_sprite_;
    uint32_t button_width_, button_height_;
    std::vector<Sprite> button_sprites_;
    std::vector<Sprite> button_bg_sprites_;
    std::vector<uint32_t> button_y_positions_;
    std::vector<SceneID> button_targets_;
    int selected_button_ = -1;
    bool pause = false;

    const int BUTTON_WIDTH = 625;
    const int BUTTON_HEIGHT = 140;
    const int BUTTON_PADDING = 40;
    const int BUTTON_X =  450;
    const int BUTTON_Y = -250;
    const float TUTORIAL_X =  680.f;
    const float TUTORIAL_Y = 350.f;
    const float TUTORIAL_WIDTH = 110.f;
    const float TUTORIAL_HEIGHT = 100.f;

    PauseMenuTransformSystem pause_menu_transform_system;
    RenderSystem render_system;
    SpriteTransformSystem sprite_transform_system;

public:
    MainMenuScene(Blackboard& blackboard, SceneManager& scene_manager);

    virtual void update(Blackboard& blackboard) override;

    virtual void render(Blackboard& blackboard) override;

    void add_item(Blackboard& blackboard, const char* texture_name, SceneID sceneID);

    void reset_scene(Blackboard& blackboard) override;

    void create_pause_menu(Blackboard& blackboard);

    void create_letter_button(Blackboard& blackboard);
};

#endif //PANDAEXPRESS_MAIN_MENU_SCENE_H
