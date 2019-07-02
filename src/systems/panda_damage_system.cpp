//
// Created by Prayansh Srivastava on 2019-03-04.
//

#include <components/panda.h>
#include <components/health.h>
#include <components/timer.h>
#include <components/velocity.h>
#include <components/interactable.h>
#include "panda_damage_system.h"

PandaDamageSystem::PandaDamageSystem() {

}

void PandaDamageSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    auto view = registry.view<Panda, Health, Timer, Velocity, Interactable, Sprite>();

    for (auto entity: view) {
        //get the sprite
        auto &panda = view.get<Panda>(entity);
        auto &timer = view.get<Timer>(entity);
        auto &health = view.get<Health>(entity);
        auto &velocity = view.get<Velocity>(entity);
        auto &interactable = view.get<Interactable>(entity);
        auto &sprite = view.get<Sprite>(entity);
        if (panda.recovering && interactable.grounded) {
            panda.recovering = false;
            sprite.set_color(1.f, 1.f, 1.f);
        }
        if ((panda.hurt && !panda.invincible && panda.alive) || health.health_points == 0) {
            // Do all damage calcs here
            health.health_points--;
            blackboard.soundManager.playSFX(SFX_PANDA_HURT);
            // Make Panda Bounce
            if (panda.facingRight) {
                velocity.x_velocity = -DMG_REACTION_X;
            } else {
                velocity.x_velocity = DMG_REACTION_X;
            }
            velocity.y_velocity = DMG_REACTION_Y;// tmp reaction
            interactable.grounded = false;

            // Prevent Player control until hit ground
            panda.recovering = true;

            // Make invincibleh
            panda.invincible = true;
            timer.save_watch(DMG_TIMER_LABEL, DMG_INVINCIBLE_TIMER);
            sprite.set_color(0.9, 0.2, 0.0); // red
        }
        // Reset Panda Invincibility timer
        if (timer.exists(DMG_TIMER_LABEL) && timer.is_done(DMG_TIMER_LABEL)) {
            panda.invincible = false;
            timer.remove(DMG_TIMER_LABEL);
        }
        if (health.health_points == 0){
            panda.dying = true;
            panda.alive = false;
        }
        // start of dying animation
        if (panda.dying && interactable.grounded) {
            timer.save_watch(DEATH_TIMER_LABEL, DEATH_TIMER);
            panda.dying = false;
        }

        if (health.health_points < 0 && !panda.hurt && interactable.grounded) {
            velocity.x_velocity = 0.f;
            velocity.y_velocity = 0.f;
        }
        //end of dying animation
        if (timer.exists(DEATH_TIMER_LABEL) && timer.is_done(DEATH_TIMER_LABEL)) {
            panda.dead = true;
            timer.remove(DEATH_TIMER_LABEL);
        }
        panda.hurt = false; // Clear DMG flag
    }
}
