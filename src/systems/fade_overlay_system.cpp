//
// Created by Avery Brown on 2019-03-30.
//

#include <graphics/fade_overlay.h>
#include <components/panda.h>
#include <components/timer.h>
#include <scene/story_intro_beach.h>
#include "fade_overlay_system.h"

FadeOverlaySystem::FadeOverlaySystem() {}

void FadeOverlaySystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    float change_in_alpha = FADE_OUT_SPEED*blackboard.delta_time;

    float alpha;

    if(blackboard.camera.transition_ready) {
        alpha = 0.f;
    }

    auto viewFade = registry.view<FadeOverlay, Timer>();
    for (auto entity: viewFade) {
        auto &fadeOverlay = viewFade.get<FadeOverlay>(entity);
        auto &timer = viewFade.get<Timer>(entity);
        float curr_time = timer.get_curr_time();
        if ((int) curr_time == 0){
            fadeOverlay.set_fadeIn(true);
        }
        if (fadeOverlay.fadeIn()) {
            alpha = fadeOverlay.alpha() - change_in_alpha;
            if (alpha < 0.f) {
                fadeOverlay.set_fadeIn(false);
            }
        } else {
            alpha = fadeOverlay.alpha() + change_in_alpha;
        }

        float position_x = blackboard.camera.position().x;
        float position_y = blackboard.camera.position().y;
        float scale_x = blackboard.camera.size().x;
        float scale_y = blackboard.camera.size().y;
        fadeOverlay.set_pos(position_x, position_y);
        fadeOverlay.set_scale({scale_x*2, scale_y*2});
        fadeOverlay.set_alpha(alpha);
    }
}
