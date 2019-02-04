//
// Created by alex on 29/01/19.
//

#pragma once

#include <queue>
#include "scene.h"
#include "scene_manager.h"
#include "../systems/sprite_render_system.h"
#include "../systems/sprite_transform_system.h"
#include "../util/blackboard.h"
#include "../systems/physics_system.h"
#include "../systems/player_movement_system.h"
#include "../systems/collision_system.h"

class TestScene : public Scene {
private:
    uint32_t panda_entity, enemy_entity;
    std::queue<uint32_t> platforms; // platforms are in order of x
    std::queue<uint32_t> floating_platforms; // platforms are in order of x

    SpriteTransformSystem sprite_transform_system;
    SpriteRenderSystem sprite_render_system;
    PhysicsSystem physics_system;
    PlayerMovementSystem player_movement_system;
    CollisionSystem collision_system;

    float last_placed_x;
    float last_placed_x_floating;

    const float CAMERA_START_X = 0.f;
    const float CAMERA_START_Y = 0.f;
    const float CAMERA_SPEED = 150.f;
    const float PANDA_START_X = 0.f;
    const float PANDA_START_Y = 0.f;
    const float PLATFORM_START_X = -200.f;
    const float PLATFORM_START_Y = 200.f;
    const float BREAD_SPEED = 50.f;
    const int MAX_PLATFORMS = 30;

    void create_panda(Blackboard& blackboard);
    void create_bread(Blackboard& blackboard);
    void generate_platforms(Blackboard &blackboard);
    void generate_floating_platforms(Blackboard &blackboard);
    void reset_scene(Blackboard& blackboard);
    void init_scene(Blackboard &blackboard);

public:
    TestScene(Blackboard& blackboard, SceneManager& scene_manager);

    virtual void update(Blackboard& blackboard) override;

    virtual void render(Blackboard& blackboard) override;
};