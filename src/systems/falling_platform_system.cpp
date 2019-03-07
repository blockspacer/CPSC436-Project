//
// Created by Kenneth William on 2019-03-06.
//

#include "falling_platform_system.h"


void FallingPlatformSystem::update(Blackboard& blackboard, entt::DefaultRegistry& registry) {

    auto falling_platform_view = registry.view<Platform, Transform, Timer, FallingPlatform>();
    for (auto falling_platform_entity : falling_platform_view) {
        auto& platform_timer  = falling_platform_view.get<Timer>(falling_platform_entity);
        auto& transform  = falling_platform_view.get<Transform>(falling_platform_entity);
        auto& falling_platform  = falling_platform_view.get<FallingPlatform>(falling_platform_entity);

        if(falling_platform.shaking){
            if(falling_platform.shakeLeft){
                transform.x-=1;
            }else{
                transform.x+=1;
            }
            falling_platform.shakeLeft=!falling_platform.shakeLeft;
        }

        if(platform_timer.is_done(std::to_string(falling_platform_entity))) {
            falling_platform.shaking = false;
            registry.assign<ObeysGravity>(falling_platform_entity,1.4f);
            registry.remove<Timer>(falling_platform_entity);
        }
    }
}