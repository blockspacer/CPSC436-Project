//
// Created by Kenneth William on 2019-02-23.
//

#include "chase_system.h"
#include <iostream>
#include "components/collidable.h"
#include "components/platform.h"
#include "components/velocity.h"
#include "components/interactable.h"
#include "components/panda.h"
#include "components/boss.h"
#include "components/jacko.h"
#include "components/dracula.h"
#include "components/chases.h"
#include "components/transform.h"
#include "components/bread.h"
#include "components/llama.h"
#include "components/spit.h"
#include "components/obstacle.h"
using namespace std;

ChaseSystem::ChaseSystem() {}


void ChaseSystem::update(Blackboard &blackboard, entt::DefaultRegistry& registry) {
    auto chaser_view = registry.view<Boss, Chases, Transform, Velocity, Interactable>();
    auto panda_view = registry.view<Panda, Transform, Velocity>();

    for (auto entity: chaser_view) {
        auto &jacko = chaser_view.get<Boss>(entity);
        auto &transform = chaser_view.get<Transform>(entity);
        auto &velocity = chaser_view.get<Velocity>(entity);
        auto &chases = chaser_view.get<Chases>(entity);
        auto &interactable = chaser_view.get<Interactable>(entity);


        auto chasedPosition = registry.get<Transform>(chases.target);

        if(chases.evading){
            if(abs(transform.x-chasedPosition.x) > 500 || abs(transform.y-chasedPosition.y) > 500){

                    chases.evading=false;
            }
        }

        if(chases.evading&&jacko.alive){
            if (chasedPosition.x < transform.x) {
                velocity.x_velocity = chases.chase_speed*6;
            } else if (chasedPosition.x > transform.x) {
                velocity.x_velocity = -chases.chase_speed*6;
            }
            if(abs(transform.x-chasedPosition.x) < 200){
                if (chasedPosition.y < transform.y) {
                    velocity.y_velocity = chases.chase_speed*5;
                } else if (chasedPosition.y > transform.y) {
                    velocity.y_velocity = -chases.chase_speed*5;
                }
            }else{
                velocity.y_velocity = -chases.chase_speed*7;
            }

        }
        else if(!chases.stomping){
            if (chasedPosition.x < transform.x) {
                velocity.x_velocity = -chases.chase_speed;
            } else if (chasedPosition.x > transform.x) {
                velocity.x_velocity = chases.chase_speed;
            }

            if (chasedPosition.y < transform.y) {
                velocity.y_velocity = -chases.chase_speed;
            } else if (chasedPosition.y > transform.y) {
                velocity.y_velocity = chases.chase_speed;
            }

        }
        else{
            if(!interactable.grounded){
                velocity.y_velocity+=1500*blackboard.delta_time;
                velocity.x_velocity=0;
            }else{
                chases.stomping=false;
                if(registry.has<Jacko>(entity)){
                    blackboard.soundManager.playSFX(SFX_JACKO_LAUGH);
                }else{
                    blackboard.soundManager.playSFX(SFX_DRACULA_LAUGH);
                }

            }


        }


    }
}