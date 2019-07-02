//
// Created by Rebecca Roth on 2019-02-22.
//

#include "ghost_movement_system.h"
#include "physics_system.h"
#include "components/ghost.h"
#include "components/panda.h"
#include "components/velocity.h"
#include "components/interactable.h"
#include "components/transform.h"
#include "components/obeys_gravity.h"
#include "components/collidable.h"
#include "scene/horizontal_scene.h"
#include "scene/vertical_scene.h"
#include "scene/scene_manager.h"

GhostMovementSystem::GhostMovementSystem() {}

void GhostMovementSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry, SceneType scene_type) {
    vec2 cam_position = blackboard.camera.position();
    vec2 cam_size = blackboard.camera.size();

    auto ghost_view = registry.view<Ghost, Transform, Velocity, Collidable>();
    for (auto enemy_entity : ghost_view) {
        auto& ghost = ghost_view.get<Ghost>(enemy_entity);
        auto& gh_transform = ghost_view.get<Transform>(enemy_entity);
        auto& gh_velocity = ghost_view.get<Velocity>(enemy_entity);
        auto& gh_collidable = ghost_view.get<Collidable>(enemy_entity);

        if (ghost.done)
            break;
        else if (!ghost.onScreen) {
            if ((scene_type == JUNGLE_TYPE) && (gh_transform.x + gh_collidable.width / 2 < cam_position.x + cam_size.x / 2)) {
                ghost.onScreen = true;
                ghost.waiting = true;
            }
            else if ((scene_type == SKY_TYPE) && (gh_transform.y - gh_collidable.height > cam_position.y - cam_size.x / 4)) {
                ghost.onScreen = true;
                ghost.waiting = true;
            }
        }
            // Waiting state
        else if (ghost.onScreen && ghost.waiting){
            if (ghost.waittime < 0){
                ghost.waiting = false;

                // Set swoop curve values based on current ghost and panda positions
                auto pandas_view = registry.view<Panda, Transform>();
                for (auto panda_entity : pandas_view) {
                    auto &pa_transform = pandas_view.get<Transform>(panda_entity);
                    ghost.aim_pt.x = pa_transform.x;
                    ghost.aim_pt.y = pa_transform.y;
                    ghost.aim_tangent.x = abs(pa_transform.x - cam_position.x + cam_size.x) * -2.5;
                    ghost.aim_tangent.y = pa_transform.y * 4;
                }
                ghost.start_pt.x = gh_transform.x;
                ghost.start_pt.y = gh_transform.y;
            }
            else {
                if (scene_type == JUNGLE_TYPE)
                    gh_velocity.x_velocity = HorizontalScene::CAMERA_SPEED;
                else {
                    gh_velocity.y_velocity = -VerticalScene::CAMERA_SPEED;
                }
                if (int(llround(floor(ghost.waittime))) % 2 == 0) {
                    if (ghost.waiting_high && ghost.waiting_left) {
                        gh_transform.x = gh_transform.x + 3;
                        ghost.waiting_left = false;
                        ghost.waiting_right = true;
                    } else if (ghost.waiting_high && ghost.waiting_right) {
                        gh_transform.y = gh_transform.y + 3;
                        ghost.waiting_high = false;
                        ghost.waiting_low = true;
                    } else if (ghost.waiting_low && ghost.waiting_right) {
                        gh_transform.x = gh_transform.x - 3;
                        ghost.waiting_left = true;
                        ghost.waiting_right = false;
                    } else {
                        gh_transform.y = gh_transform.y - 3;
                        ghost.waiting_high = true;
                        ghost.waiting_low = false;
                    }
                }
                ghost.waittime = ghost.waittime - blackboard.delta_time*4;
            }
        }
            // Curve to swoop at player
        else if (ghost.onScreen && !ghost.waiting){
            if (ghost.swoopTime <= 1){
                float t = ghost.swoopTime;
                float x=(2*t*t*t-3*t*t+1)*ghost.start_pt.x+(-2*t*t*t+3*t*t)*ghost.aim_pt.x+
                        (t*t*t-2*t*t+t)*ghost.start_tangent.x+(t*t*t-t*t)*ghost.aim_tangent.x;
                float y=(2*t*t*t-3*t*t+1)*ghost.start_pt.y+(-2*t*t*t+3*t*t)*ghost.aim_pt.y+
                        (t*t*t-2*t*t+1)*ghost.start_tangent.y+(t*t*t-t*t)*ghost.aim_tangent.y;
                gh_transform.x = x;
                gh_transform.y = y;
                ghost.swoopTime = ghost.swoopTime + blackboard.delta_time/2;
            }
            else {
                gh_velocity.x_velocity = -500;
                gh_velocity.y_velocity = 100;
                ghost.done = true;
            }
        }
    }
}