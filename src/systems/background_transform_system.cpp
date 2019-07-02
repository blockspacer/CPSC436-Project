//
// Created by Prayansh Srivastava on 2019-02-07.
//

#include "background_transform_system.h"
#include <util/constants.h>

BackgroundTransformSystem::BackgroundTransformSystem(SceneType scene_type) : scene_type(scene_type) {

}

void BackgroundTransformSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    auto viewBg = registry.view<Background>();
    for (auto entity: viewBg) {
        //get the background
        auto &background = viewBg.get(entity);
        // move the background
        switch (scene_type) {
            case JUNGLE_TYPE: {
                horizontal_background_transform(blackboard, background);
            }
                break;
            case SKY_TYPE: {
                vertical_background_transform(blackboard, background);
            }
                break;
            case BOSS_TYPE: {
                boss_background_transform(blackboard, background);
            }
                break;
            case STORY_TYPE: {
                story_beach_background_transform(blackboard, background);
            }
                break;
            case BOSS_DRACULA_TYPE: {
                boss_dracula_background_transform(blackboard, background);
            }
                break;
        }
    }
}

void BackgroundTransformSystem::horizontal_background_transform(Blackboard &blackboard,
                                                                Background &background) {
    float displacement = background.z_pos() * HORIZONTAL_LAYER_SPEED * blackboard.delta_time;
    Camera camera = blackboard.camera;
    background.set_pos1(background.pos1().x + displacement, background.pos1().y);
    background.set_pos2(background.pos2().x + displacement, background.pos2().y);
    if (background.pos1().x < (camera.position().x - camera.size().x)) {
        background.set_pos1(background.pos1().x + camera.size().x * 2, background.pos1().y);
    }
    if (background.pos2().x < (camera.position().x - camera.size().x)) {
        background.set_pos2(background.pos2().x + camera.size().x * 2, background.pos2().y);
    }
}

void BackgroundTransformSystem::vertical_background_transform(Blackboard &blackboard,
                                                              Background &background) {
    if (background.z_pos() != 0) {
        Camera camera = blackboard.camera;
        float displacement = background.z_pos() * VERTICAL_LAYER_SPEED * blackboard.delta_time;
        background.set_pos1(background.pos1().x + displacement, background.pos1().y);
        background.set_pos2(background.pos2().x + displacement, background.pos2().y);
        if (background.pos1().y > (camera.position().y + camera.size().y)) {
            background.set_pos1(background.pos1().x, background.pos1().y - camera.size().y * 2);
            background.set_z_pos(background.z_pos() * -1);
        }
        if (background.pos2().y > (camera.position().y + camera.size().y)) {
            background.set_pos2(background.pos2().x, background.pos2().y - camera.size().y * 2);
            background.set_z_pos(background.z_pos() * -1);
        }
    }
}

float BackgroundTransformSystem::clamp(float val, float from, float to) {
    if (val > from) {
        return std::min(val, to);
    } else if (val < to) {
        return std::max(val, from);
    } else {
        return val;
    }
}

void BackgroundTransformSystem::boss_background_transform(Blackboard &blackboard,
                                                          Background &background) {
    if (background.z_pos() == 0) { // follow the camera
        // this is the moon
        Camera camera = blackboard.camera;
        background.set_pos1(camera.position().x, camera.position().y);
    } else if (background.z_pos() == 3) { // follow the camera only in x-direction
        // This is the grass
        Camera camera = blackboard.camera;
        float halfWidth = camera.size().x / 2.0f;
        if (camera.position().x <= -halfWidth) {
            background.set_pos1(-halfWidth, background.pos1().y);
            background.set_pos2(-camera.size().x - halfWidth, background.pos1().y);
        } else if(camera.position().x >= halfWidth) {
            background.set_pos1(halfWidth + camera.size().x, background.pos1().y);
            background.set_pos2(halfWidth, background.pos1().y);
        } else {
            background.set_pos1(-halfWidth, background.pos1().y);
            background.set_pos2(halfWidth, background.pos1().y);
        }
    } else {
        // everything else
        float og_pos1 = 0.0;
        float clampValue = blackboard.camera.size().x / 0.5f;
        float displacement = clamp(
                (og_pos1 - blackboard.camera.position().x) * (background.z_pos() / 10.0f),
                -clampValue,
                clampValue
        );
        float maxDist = blackboard.camera.size().y / 0.8f;
        float displacementY = clamp(
                (og_pos1 - blackboard.camera.position().y) * (background.z_pos() / 2.0f),
                0, maxDist);
        vec2 pos1 = background.pos1();
        background.set_pos1(blackboard.camera.position().x + displacement,
                            blackboard.camera.position().y + displacementY);
    }
}

void BackgroundTransformSystem::story_beach_background_transform(Blackboard &blackboard,
                                                              Background &background) {
    if (background.z_pos() != 0 && background.z_pos() != 5 && background.z_pos() != 4) {
        float displacement =  STORY_LAYER_SPEED * blackboard.delta_time / background.z_pos();
        background.set_pos1(background.pos1().x, background.pos1().y + displacement);
        if (background.pos1().y > WAVE_MAX || background.pos1().y < WAVE_MIN ) {
            background.set_z_pos(background.z_pos() * -1);
        }
    }
}

void BackgroundTransformSystem::boss_dracula_background_transform(Blackboard &blackboard,
                                                                  Background &background) {
    Camera camera = blackboard.camera;
    float og_pos1 = 0.0;
    float clampValue = blackboard.camera.size().x / 0.5f;
    float displacement = clamp(
            (og_pos1 - blackboard.camera.position().x) * (background.z_pos() / 10.0f),
            -clampValue,
            clampValue
    );
    float maxDist = blackboard.camera.size().y / 0.8f;
    float displacementY = clamp(
            (og_pos1 - background.pos1().y) * (background.z_pos() / 2.0f),
            0, maxDist);
    background.set_pos1(blackboard.camera.position().x + displacement,
                        background.pos1().y + displacementY);
    float halfWidth = camera.size().x / 2.0f;
    float camLeft = camera.position().x - halfWidth;
    float camRight = camera.position().x + halfWidth;
    float bgLeft = background.pos1().x - halfWidth;
    float bgRight = background.pos1().x + halfWidth;
    if (camLeft < bgLeft) {
        // camLeft is farther than background's left (space)
        background.set_pos2(background.pos1().x - camera.size().x, background.pos1().y);
    } else if (camRight >= bgRight) {
        // camRight is farther than background's right (space)
        background.set_pos2(background.pos1().x + camera.size().x, background.pos1().y);
    }
}
