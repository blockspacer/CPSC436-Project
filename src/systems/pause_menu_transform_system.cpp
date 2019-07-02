//
// Created by Avery Brown on 2019-03-31.
//

//
// Created by Avery Brown on 2019-03-30.
//

#include <graphics/sprite.h>
#include <components/pause_menu.h>
#include <scene/horizontal_scene.h>
#include "pause_menu_transform_system.h"

PauseMenuTransformSystem::PauseMenuTransformSystem() {}

void PauseMenuTransformSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {

    auto view = registry.view<PauseMenu, Sprite>();

    for (auto entity: view) {
        auto &pause = view.get<PauseMenu>(entity);
        auto &sprite = view.get<Sprite>(entity);
        float position_x = blackboard.camera.position().x;
        float position_y = blackboard.camera.position().y;
        sprite.set_scale(vec2{0.8, 0.8});
        sprite.set_pos(position_x, position_y);
    }

}
