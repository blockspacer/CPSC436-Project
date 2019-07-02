//
// Created by Prayansh Srivastava on 2019-02-12.
//

#ifndef PANDAEXPRESS_DRACULA_BOSS_SCENE_H
#define PANDAEXPRESS_DRACULA_BOSS_SCENE_H


#include <queue>
#include <level/boss_level_system.h>
#include <systems/background_transform_system.h>
#include <systems/panda_damage_system.h>
#include <systems/timer_system.h>
#include <systems/health_bar_transform_system.h>
#include <systems/fade_overlay_system.h>
#include <systems/pause_menu_transform_system.h>
#include <systems/hud_transform_system.h>
#include <systems/render_system.h>
#include "../systems/sprite_transform_system.h"
#include "../util/blackboard.h"
#include "../systems/physics_system.h"
#include "../systems/player_movement_system.h"
#include "../systems/timer_system.h"
#include "../systems/dracula_ai_system.h"
#include "../systems/falling_platform_system.h"
#include "../systems/chase_system.h"
#include "../systems/player_animation_system.h"
#include "../systems/enemy_animation_system.h"
#include "game_scene.h"
#include "../systems/seek_system.h"
#include <systems/a_star_system.h>
#include "../systems/transition_system.h"

class DraculaBossScene: public GameScene {
private:
    const float MAX_CAMERA_Y_DIFF = 200.f;

    bool pause = false;
    std::vector<uint32_t> bg_entities;
    uint32_t dracula_entity;
    uint32_t bat_entity;

    BossLevelSystem level_system;
    SpriteTransformSystem sprite_transform_system;
    BackgroundTransformSystem background_transform_system;
    PhysicsSystem physics_system;
    PlayerMovementSystem player_movement_system;
    ChaseSystem chase_system;
    DraculaAISystem dracula_ai_system;
    PlayerAnimationSystem player_animation_system;
    TimerSystem timer_system;
    FallingPlatformSystem falling_platform_system;
    PandaDamageSystem panda_dmg_system;
    EnemyAnimationSystem enemy_animation_system;
    HealthBarTransformSystem health_bar_transform_system;
    FadeOverlaySystem fade_overlay_system;
    PauseMenuTransformSystem pause_menu_transform_system;
    HudTransformSystem hud_transform_system;
    RenderSystem render_system;
    //AStarSystem a_star_system;
    SeekSystem seek_system;
    TransitionSystem transition_system;
    Timer scene_timer;


    void create_background(Blackboard &blackboard);
    void create_dracula(Blackboard& blackboard, uint32_t panda);
    void update_panda(Blackboard& blackboard);
    void update_camera(Blackboard& blackboard);
public:

    DraculaBossScene(Blackboard &blackboard,
                    SceneManager &scene_manager);
    virtual void update(Blackboard& blackboard) override;

    virtual void render(Blackboard& blackboard) override;
    void init_scene(Blackboard &blackboard);
    void reset_scene(Blackboard& blackboard) override;

    void cleanup();
};


#endif //PANDAEXPRESS_BOSS_SCENE_H

