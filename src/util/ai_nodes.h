//
// Created by Kenneth William on 2019-04-14.
//

#ifndef PANDAEXPRESS_AI_NODES_H
#define PANDAEXPRESS_AI_NODES_H

#include "systems/system.h"
#include "systems/a_star_system.h"
#include "util/selector_node.h"
#include "util/coordinates.h"
#include "util/Location.h"
#include "components/dracula.h"
#include "components/boss.h"
#include "components/chases.h"
#include "components/causes_damage.h"
#include "components/seeks.h"
#include "components/evades.h"
#include "components/timer.h"
#include "components/panda.h"
#include "components/food.h"
#include "components/transform.h"
#include "components/velocity.h"
#include "components/interactable.h"
#include "components/health.h"

class AINodes {
public:

/***
 * Checks whether Jacko has more than one health point left
 * run() returns false if healthpoints are more than one or if childnodes all return false
 */
    class CheckHealth: public SelectorNode{
    private:
        int health;

    public:
        CheckHealth(int health) :
                health(health) {}

        void updateHealth(int newHealth){
            health=newHealth;
        }
        virtual bool run() override {
            if(health>=2){
                return false;
            }
            for (Node* child : getChildren()) {
                if (child->run())
                    return true;
            }
            return false;
        }
    };

/***
 * Check if the panda is dead, right now it just resets
 * but in the future we may want the boss to laugh at the
 * player when they lose
 */
    class CheckPandaDeath: public Node{
    private:
        int health;

    public:
        CheckPandaDeath(int health) :
                health(health) {}
        void updateHealth(int newHealth){
            health=newHealth;
        }
        virtual bool run() override {
            return (health<1);
        }
    };

/***
 * This node makes sure that Jacko starts chasing the Panda again if he's not
 * looking for food and always returns false
 */

    class ChasePanda: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
    public:
        ChasePanda(Blackboard& blackboard, entt::DefaultRegistry& registry) :
                blackboard(blackboard),
                registry(registry){}
        virtual bool run() override {
            auto drac_view = registry.view<Boss, Chases>();
            for (auto drac_entity: drac_view) {
                auto &chase = drac_view.get<Chases>(drac_entity);

                auto panda_view = registry.view<Panda>();
                for(auto panda_entity:panda_view){
                    chase.target=panda_entity;
                }

            }

            return false;
        }
    };

    class StompPanda: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
    public:
        StompPanda(Blackboard& blackboard, entt::DefaultRegistry& registry) :
                blackboard(blackboard),
                registry(registry){}
        virtual bool run() override {
            auto drac_view = registry.view<Boss, Transform, Velocity, Interactable, Chases>();
            for (auto drac_entity: drac_view) {
                auto &drac_transform = drac_view.get<Transform>(drac_entity);
                auto &drac_velocity = drac_view.get<Velocity>(drac_entity);
                auto &drac_chases = drac_view.get<Chases>(drac_entity);
                auto &drac_interactable = drac_view.get<Interactable>(drac_entity);

                auto panda_view = registry.view<Panda, Transform>();
                for(auto panda_entity:panda_view){
                    auto &pa_transform = panda_view.get<Transform>(panda_entity);
                    if(pa_transform.x > drac_transform.x && pa_transform.x < drac_transform.x+100 && pa_transform.y > drac_transform.y){
                        drac_chases.stomping=true;
                    }
                }

            }

            return false;
        }
    };

    class ShootBat: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
        AStarSystem& a_star_system;
        int batCount = 0;
        bool pathSet = false;
        std::vector<Coordinates> path;
    public:
        ShootBat(Blackboard& blackboard, entt::DefaultRegistry& registry, AStarSystem& a_star_system) :
                blackboard(blackboard),
                registry(registry),
                a_star_system(a_star_system){}
        virtual bool run() override {

            auto drac_view = registry.view<Dracula, Transform, Chases, Velocity, Timer>();
            for (auto drac_entity: drac_view) {
                auto &dracula = drac_view.get<Dracula>(drac_entity);
                auto &drac_transform = drac_view.get<Transform>(drac_entity);
                auto &drac_velocity = drac_view.get<Velocity>(drac_entity);
                auto &drac_chases = drac_view.get<Chases>(drac_entity);
                auto &timer = drac_view.get<Timer>(drac_entity);

                if(dracula.shooter_count>3){
                    return false;
                }

                if(timer.watch_exists("batAttack")) {
                    if (timer.is_done("batAttack")) {
                        if(!pathSet){
                            path = a_star_system.getProjectilePath(blackboard,registry);
                            pathSet=true;
                        }
                        drac_chases.chase_speed=0.f;
                        drac_velocity.x_velocity = 0;
                        drac_velocity.y_velocity = 0;

                        if (timer.watch_exists("batShooter")) {

                            if (timer.is_done("batShooter")) {
                                if(batCount <8){
                                    auto panda_view = registry.view<Panda, Transform>();
                                    for (auto panda_entity:panda_view) {
                                        auto &pa_transform = panda_view.get<Transform>(panda_entity);


                                        auto bat_entity = registry.create();

                                        auto texture = blackboard.texture_manager.get_texture("bat");
                                        auto shader = blackboard.shader_manager.get_shader("sprite");
                                        auto mesh = blackboard.mesh_manager.get_mesh("sprite");

                                        float scaleY = 50.0 / texture.height();
                                        float scaleX = 50.0 / texture.width();
                                        registry.assign<Transform>(bat_entity, path[0].x, path[0].y, 0., scaleX, scaleY);
                                        registry.assign<Sprite>(bat_entity, texture, shader, mesh);
                                        registry.assign<Velocity>(bat_entity);
                                        registry.assign<Timer>(bat_entity);
                                        registry.assign<CausesDamage>(bat_entity, TOP_VULNERABLE_MASK, 1);
                                        registry.assign<Collidable>(bat_entity, texture.width() * scaleX,
                                                                    texture.height() * scaleY);
                                        registry.assign<Seeks>(bat_entity, path);
                                        batCount++;
                                        timer.save_watch("batShooter", 0.1f);
                                        blackboard.soundManager.playSFX(SFX_BAT_SHOT);
                                }

                                }else{
                                    timer.remove("batAttack");
                                    timer.remove("batShooter");
                                    drac_chases.chase_speed=120.f;
                                    dracula.shooter_count++;
                                    batCount=0;
                                    pathSet=false;
                                    return false;
                                }

                            }
                        } else {
                            timer.save_watch("batShooter", 1.f);
                            blackboard.soundManager.playSFX(SFX_DRACULA_LAUGH);

                        }
                    }
                }else {
                    timer.save_watch("batAttack", 4.f);
                    return false;
                }

            }

            return false;
        }
    };


    class TeleportAttack: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
        AStarSystem& a_star_system;
        float dracula_chase_speed = 200;
        float teleport_chase_speed= 300;
    public:
        TeleportAttack(Blackboard& blackboard, entt::DefaultRegistry& registry, AStarSystem& a_star_system):
                blackboard(blackboard),
                registry(registry),
                a_star_system(a_star_system){}
            virtual bool run() override {
                    auto drac_view = registry.view<Dracula, Transform, Chases, Velocity, Timer, Sprite>();
                    for (auto drac_entity: drac_view) {
                        auto &dracula = drac_view.get<Dracula>(drac_entity);
                        auto &drac_transform = drac_view.get<Transform>(drac_entity);
                        auto &drac_velocity = drac_view.get<Velocity>(drac_entity);
                        auto &drac_chases = drac_view.get<Chases>(drac_entity);
                        auto &timer = drac_view.get<Timer>(drac_entity);
                        auto &sprite = drac_view.get<Sprite>(drac_entity);

                        if(dracula.shooter_count<3){
                            return false;
                        }

                        if(timer.watch_exists("teleport")) {
                            if (timer.is_done("teleport")) {
                                sprite.set_color(0.1, 0.9, 0.f); // green
                                if(timer.watch_exists("teleportDelay")) {
                                    drac_chases.chase_speed=0.f;
                                    drac_velocity.x_velocity=0.f;
                                    drac_velocity.y_velocity=0.f;
                                    if (timer.is_done("teleportDelay")) {
                                        auto panda_view = registry.view<Panda, Transform>();
                                        for(auto panda_entity: panda_view) {
                                            auto panda_transform = panda_view.get<Transform>(panda_entity);
                                            Coordinates teleport_coords(0,0);
                                            Location *panda_location = a_star_system.getGridLocation(panda_transform.x, panda_transform.y);
                                            Location *teleport_location_right = a_star_system.getGridLocation(panda_transform.x+300, panda_transform.y);
                                            Location *teleport_location_left = a_star_system.getGridLocation(panda_transform.x-300, panda_transform.y);
                                            Location *teleport_location_above = a_star_system.getGridLocation(panda_transform.x, panda_transform.y-400);

                                            if((!teleport_location_left->platform && !teleport_location_right->platform)){
                                                //blackboard.randNumGenerator.init(4);
                                                int teleportChooser = blackboard.randNumGenerator.nextInt(0,2);
                                                switch(teleportChooser){
                                                case 0: teleport_coords = a_star_system.getScreenLocation(teleport_location_right->i,
                                                                                                          teleport_location_right->j);
                                                break;

                                                case 1: teleport_coords = a_star_system.getScreenLocation(teleport_location_left->i,
                                                                                                          teleport_location_left->j);
                                                break;

                                                case 2: teleport_coords = a_star_system.getScreenLocation(teleport_location_above->i,
                                                        teleport_location_above->j);
                                                break;
                                                    }

                                            }else{
                                                teleport_coords = a_star_system.getScreenLocation(teleport_location_above->i,
                                                                                                  teleport_location_above->j);
                                            }


                                            drac_transform.x = teleport_coords.x;
                                            drac_transform.y = teleport_coords.y;
                                            drac_chases.chase_speed=teleport_chase_speed;
                                            timer.remove("teleportDelay");
                                            timer.remove("teleport");
                                            dracula.shooter_count++;
                                            sprite.set_color(1.f, 1.f, 1.f);
                                            if(dracula.shooter_count>6){
                                                dracula.shooter_count=0;
                                                drac_chases.chase_speed=dracula_chase_speed;
                                            }
                                            return false;
                                        }

                                    }else{
                                        return false;
                                    }
                                }else{
                                    timer.save_watch("teleportDelay", 0.5f);
                                    blackboard.soundManager.playSFX(SFX_TELEPORT);
                                    return false;
                                }

                            }

                        }else {
                            timer.save_watch("teleport", 1.f);
                            return false;
                        }

                    }
                return false;
        }

    };


    class TeleportIntoFrame: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
        AStarSystem& a_star_system;
    public:
        TeleportIntoFrame(Blackboard& blackboard, entt::DefaultRegistry& registry, AStarSystem& a_star_system):
                blackboard(blackboard),
                registry(registry),
                a_star_system(a_star_system){}
        virtual bool run() override {
            auto drac_view = registry.view<Dracula, Transform, Chases, Velocity, Timer>();
            for (auto drac_entity: drac_view) {
                auto &dracula = drac_view.get<Dracula>(drac_entity);
                auto &drac_transform = drac_view.get<Transform>(drac_entity);
                auto &drac_velocity = drac_view.get<Velocity>(drac_entity);
                auto &drac_chases = drac_view.get<Chases>(drac_entity);
                auto &timer = drac_view.get<Timer>(drac_entity);

                if( !(drac_transform.x<blackboard.camera.position().x || drac_transform.x>(blackboard.camera.position().x+1600)) ){
                    return false;
                }

                if(timer.watch_exists("teleportIntoFrame")) {
                    if (timer.is_done("teleportIntoFrame")) {
                        if(timer.watch_exists("teleportDelay")) {
                            drac_chases.chase_speed=0.f;
                            drac_velocity.x_velocity=0.f;
                            drac_velocity.y_velocity=0.f;

                            if (timer.is_done("teleportDelay")) {
                                auto panda_view = registry.view<Panda, Transform>();
                                for(auto panda_entity: panda_view) {
                                    auto panda_transform = panda_view.get<Transform>(panda_entity);
                                    Coordinates teleport_coords=Coordinates(0,0);
                                    int teleportOffset;
                                    Location *teleport_location;

                                    if(blackboard.randNumGenerator.nextInt()%2){
                                        teleport_location = a_star_system.getGridLocation(panda_transform.x+teleportOffset, panda_transform.y-600);
                                    }else{
                                        teleport_location = a_star_system.getGridLocation(panda_transform.x+teleportOffset, panda_transform.y-600);
                                    }

                                    teleport_coords = a_star_system.getScreenLocation(teleport_location->i,
                                                                                      teleport_location->j);


                                    drac_transform.x = teleport_coords.x;
                                    drac_transform.y = teleport_coords.y;
                                    drac_chases.chase_speed=120.f;
                                    timer.remove("teleportDelay");
                                    timer.remove("teleportIntoFrame");

                                    return false;
                                }

                            }else{
                                return false;
                            }
                        }else{
                            timer.save_watch("teleportDelay", 0.5f);
                            return false;
                        }

                    }

                }else {
                    timer.save_watch("teleportIntoFrame", 1.f);
                    return false;
                }

            }
            return false;
        }

    };

/***
 * This node makes Jacko try to get food when his health is low,
 * if there is no food left he will speed up to try to kill the panda
 */
    class ChaseFood: public Node{
    private:
        Blackboard& blackboard;
        entt::DefaultRegistry& registry;
    public:
        ChaseFood(Blackboard& blackboard, entt::DefaultRegistry& registry) :
                blackboard(blackboard),
                registry(registry){}
        virtual bool run() override {
            auto drac_view = registry.view<Boss, Chases>();
            for (auto drac_entity: drac_view) {
                auto &chase = drac_view.get<Chases>(drac_entity);

                auto food_view = registry.view<Food>();
                if (food_view.empty()){
                    auto panda_view = registry.view<Panda>();
                    for(auto panda_entity: panda_view){
                        chase.target=panda_entity;
                    }
                    chase.chase_speed=180;
                }
                for(auto food_entity:food_view){
                    if(!registry.get<Food>(food_entity).eaten){
                        chase.target=food_entity;
                    }
                }
            }
            return true;
        }
    };

};


#endif //PANDAEXPRESS_AI_NODES_H
