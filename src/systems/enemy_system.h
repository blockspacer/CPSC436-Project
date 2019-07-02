//
// Created by Rebecca Roth on 2019-03-03.
//

#ifndef PANDAEXPRESS_ENEMY_SYSTEM_H
#define PANDAEXPRESS_ENEMY_SYSTEM_H
#include "system.h"
#include "util/constants.h"
#include "components/ghost.h"
#include "components/panda.h"
#include "components/bread.h"
#include "components/llama.h"
#include "components/spit.h"
#include "systems/ghost_movement_system.h"
#include "level/level_system.h"
#include "components/velocity.h"
#include "components/transform.h"
#include "components/collidable.h"
#include "components/timer.h"

class EnemySystem {
private:
    GhostMovementSystem ghost_movement_system;
    const float BREAD_SPEED = 50.f;
    const float PROJECTILE_SPEED_X = -300.f;
    const float PROJECTILE_SPEED_Y = 10.f;
    const std::string SPIT_TIMER_LABEL = "spit";
    const float VERTICAL_BUFFER = 300.f;

    void generate_projectile(float x, float y, bool spit_left, Blackboard &blackboard, entt::DefaultRegistry &registry);
    void handle_bread(vec2 cam_position, vec2 cam_size, SceneType scene_type, Blackboard &blackboard,
                      entt::DefaultRegistry &registry);
    void handle_ghosts(vec2 cam_position, vec2 cam_size, SceneType scene_type, Blackboard &blackboard,
                       entt::DefaultRegistry &registry);
    void handle_llamas(vec2 cam_position, vec2 cam_size, SceneType scene_type, Blackboard &blackboard,
                       entt::DefaultRegistry &registry);
    void handle_spit(vec2 cam_position, vec2 cam_size, SceneType scene_type, Blackboard &blackboard,
                     entt::DefaultRegistry &registry);


public:
    EnemySystem();
    virtual void update(Blackboard &blackboard, entt::DefaultRegistry &registry, SceneType scene_type);
};


#endif //PANDAEXPRESS_ENEMY_SYSTEM_H