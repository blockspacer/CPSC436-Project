//
// Created by Prayansh Srivastava on 2019-02-12.
//

#include <graphics/background.h>
#include <graphics/health_bar.h>
#include <graphics/cave.h>
#include <graphics/text.h>
#include <graphics/fade_overlay.h>
#include "horizontal_scene.h"
#include "util/constants.h"
#include <algorithm>

HorizontalScene::HorizontalScene(Blackboard &blackboard, SceneManager &scene_manager) :
        GameScene(scene_manager),
        level_system(),
        sprite_transform_system(),
        background_transform_system(JUNGLE_TYPE),
        physics_system(),
        player_movement_system(JUNGLE_TYPE),
        enemy_system(),
        player_animation_system(JUNGLE_TYPE),
        panda_dmg_system(),
        falling_platform_system(),
        enemy_animation_system(),
        health_bar_transform_system(),
        text_transform_system(),
        score_system(JUNGLE_TYPE),
        pause_menu_transform_system(),
        transition_system(JUNGLE_TYPE),
        hud_transform_system(),
        label_system(),
        render_system(),
        powerup_system()
{
    high_score_ = 0;
    init_scene(blackboard);
    gl_has_errors("horizontal_scene");
}

void HorizontalScene::update(Blackboard &blackboard) {
    auto &panda = registry_.get<Panda>(panda_entity);
    auto &interactable = registry_.get<Interactable>(panda_entity);
    auto &fadeOverlay = registry_.get<FadeOverlay>(fade_overlay_entity);

    if (blackboard.camera.transition_ready) {
        if (fadeOverlay.alpha() < 0.9f) {
            fade_overlay_system.update(blackboard, registry_);
        } else {
            go_to_next_scene(blackboard);
            return;
        }
    }

    if (blackboard.input_manager.key_just_pressed(SDL_SCANCODE_ESCAPE)) {
        if (pause) {
            pause = false;
            registry_.destroy(pause_menu_entity);
        } else {
            pause = true;
            create_pause_menu(blackboard);
        }
    } else if (blackboard.input_manager.key_just_pressed(SDL_SCANCODE_RETURN) && pause) {
        blackboard.camera.set_position(0, 0);
        reset_scene(blackboard);
        registry_.destroy(pause_menu_entity);
        change_scene(MAIN_MENU_SCENE_ID);
        pause = false;
        return;
    }

    if (!pause) {
        if (panda.alive && !panda.dead) {
            if (!blackboard.camera.in_transition) {
                update_camera(blackboard);
                background_transform_system.update(blackboard, registry_);
                score_system.update(blackboard, registry_);
            }
            player_movement_system.update(blackboard, registry_);
        } else if (!panda.alive && interactable.grounded) {
            fade_overlay_system.update(blackboard, registry_);
        }

        if (fadeOverlay.alpha() > 0.f) {
            fade_overlay_system.update(blackboard, registry_);
        }

        if (mode_ == STORY_EASY || mode_ == STORY_HARD)
            check_end_timer();

        update_panda(blackboard);

        level_system.update(blackboard, registry_);
        physics_system.update(blackboard, registry_);
        enemy_system.update(blackboard, registry_, JUNGLE_TYPE);
        sprite_transform_system.update(blackboard, registry_);
        health_bar_transform_system.update(blackboard, registry_);
        player_animation_system.update(blackboard, registry_);
        text_transform_system.update(blackboard, registry_);
        label_system.update(blackboard, registry_);
        timer_system.update(blackboard, registry_);
        falling_platform_system.update(blackboard, registry_);
        enemy_animation_system.update(blackboard, registry_);
        transition_system.update(blackboard, registry_);
        powerup_system.update(blackboard, registry_);
        hud_transform_system.update(blackboard, registry_);// Must run last
        high_score_ = std::max<int>(high_score_, (int) blackboard.score);

        if (!blackboard.camera.transition_ready) {
            panda_dmg_system.update(blackboard, registry_);
        }
    } else {
        pause_menu_transform_system.update(blackboard, registry_);
    }
}

void HorizontalScene::update_panda(Blackboard &blackboard) {
    vec2 cam_position = blackboard.camera.position();
    vec2 cam_size = blackboard.camera.size();

    auto &transform = registry_.get<Transform>(panda_entity);
    auto &panda = registry_.get<Panda>(panda_entity);
    auto &panda_collidable = registry_.get<Collidable>(panda_entity);

    if (transform.x + panda_collidable.width < cam_position.x - cam_size.x / 2 ||
        transform.y - panda_collidable.height > cam_position.y + cam_size.y / 2 || panda.dead) {
        if (mode_ == ENDLESS) {
            reset_scene(blackboard);
        } else if (blackboard.story_lives > 1) {
            blackboard.story_lives -= 1;
            blackboard.story_health = MAX_HEALTH;
            reset_scene(blackboard);
        } else {
            blackboard.story_lives -= 1;
            blackboard.camera.set_position(0, 0);
            reset_scene(blackboard);
            change_scene(MAIN_MENU_SCENE_ID);
            return;
        }
    } else if (transform.x + panda_collidable.width / 2 > cam_position.x + cam_size.x / 2) {
        transform.x = cam_position.x + cam_size.x / 2 - panda_collidable.width / 2;
    }
}

void HorizontalScene::update_camera(Blackboard &blackboard) {
    vec2 cam_position = blackboard.camera.position();

    auto &panda_transform = registry_.get<Transform>(panda_entity);
    float y_offset = std::min(0.f, panda_transform.y + MAX_CAMERA_Y_DIFF);
    blackboard.camera.set_position(
            cam_position.x + CAMERA_SPEED * blackboard.delta_time,
                                   y_offset);
    blackboard.camera.compose();
}

void HorizontalScene::render(Blackboard &blackboard) {
    if (mode_ == STORY_HARD) {
        blackboard.window.colorScreen(vec3{136.f, 19.f, 126.f});
    } else {
        blackboard.window.colorScreen(vec3{19.f, 136.f, 126.f});
    }
    render_system.update(blackboard, registry_);
}

void HorizontalScene::reset_scene(Blackboard &blackboard) {
    cleanup();
    blackboard.camera.in_transition = false;
    blackboard.camera.transition_ready = false;
    blackboard.score = 0;
    blackboard.soundManager.changeBackgroundMusic(blackboard.soundManager.currentStage);
    init_scene(blackboard);
}

void HorizontalScene::cleanup() {
    level_system.destroy_entities(registry_);
    for (uint32_t e: bg_entities) {
        registry_.destroy(e);
    }
    bg_entities.clear();
    if (registry_.valid(timer_entity))
        registry_.destroy(timer_entity);
    GameScene::cleanup();
}

void HorizontalScene::go_to_next_scene(Blackboard &blackboard) {
    auto &health = registry_.get<Health>(panda_entity);
    blackboard.story_health = health.health_points;
    if (mode_ == STORY_EASY) {
        cleanup();
        blackboard.camera.in_transition = false;
        blackboard.camera.transition_ready = false;
        change_scene(STORY_EASY_SKY_SCENE_ID);
    }
    else if (mode_ == STORY_HARD) {
        cleanup();
        blackboard.camera.in_transition = false;
        blackboard.camera.transition_ready = false;
        change_scene(STORY_HARD_SKY_SCENE_ID);
    }
}

void HorizontalScene::init_scene(Blackboard &blackboard) {
    blackboard.randNumGenerator.init(0);
    blackboard.camera.set_position(CAMERA_START_X, CAMERA_START_Y);
    blackboard.camera.compose();
    blackboard.time_multiplier = DEFAULT_SPEED_MULTIPLIER;
    create_background(blackboard);
    create_panda(blackboard);
    if (mode_ == ENDLESS) {
        create_score_text(blackboard);
        create_high_score_text(blackboard, high_score_);
        physics_system.set_story(false);
    } else if (mode_ == STORY_EASY || mode_ == STORY_HARD) {
        timer_entity = registry_.create();
        auto& timer = registry_.assign<Timer>(timer_entity);
        timer.save_watch(END_TIMER_LABEL, END_TIMER_LENGTH);
        create_lives_text(blackboard);
        physics_system.set_story(true);
    }
    create_fade_overlay(blackboard);
    auto &fadeOverlay = registry_.get<FadeOverlay>(fade_overlay_entity);
    fadeOverlay.set_alpha(1.0);
    blackboard.post_process_shader = std::make_unique<Shader>(blackboard.shader_manager.get_shader("sprite"));
    level_system.init(mode_, registry_);
}

void HorizontalScene::create_background(Blackboard &blackboard) {
    std::vector<Texture> textures;
    textures.reserve(4);
    // This order matters for rendering
    textures.push_back(blackboard.texture_manager.get_texture("bg_top"));
    textures.push_back(blackboard.texture_manager.get_texture("bg_front"));
    textures.push_back(blackboard.texture_manager.get_texture("bg_middle"));
    textures.push_back(blackboard.texture_manager.get_texture("bg_back"));
    // end order
    auto shader = blackboard.shader_manager.get_shader("sprite");
    if (mode_ == STORY_HARD) {
        shader = blackboard.shader_manager.get_shader("recolor");
    }
    auto mesh = blackboard.mesh_manager.get_mesh("sprite");
    int i = 0;
    for (Texture t: textures) {
        auto bg_entity = registry_.create();
        auto &bg = registry_.assign<Background>(bg_entity, t, shader, mesh, i);
        registry_.assign<Layer>(bg_entity, BACKGROUND_LAYER - i);
        bg.set_pos1(0.0f, 0.0f);
        bg.set_pos2(blackboard.camera.size().x, 0.0f);
        bg.set_rotation_rad(0.0f);
        bg.set_size(blackboard.camera.size().x, blackboard.camera.size().y);
        bg_entities.push_back(bg_entity);
        i++;
    }
}

void HorizontalScene::set_mode(SceneMode mode, Blackboard &blackboard) {
    Scene::set_mode(mode, blackboard);
    reset_scene(blackboard);
}

void HorizontalScene::set_high_score(int value) {
    high_score_ = value;
}

int HorizontalScene::get_high_score() {
    return high_score_;
}

void HorizontalScene::check_end_timer() {
    auto& timer = registry_.get<Timer>(timer_entity);
    if (timer.exists(END_TIMER_LABEL) && timer.is_done(END_TIMER_LABEL)) {
        level_system.generate_end_level();
        timer.remove(END_TIMER_LABEL);
    }
}
