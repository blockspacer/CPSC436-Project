//
// Created by Prayansh Srivastava on 2019-03-05.
//

#include <components/score.h>
#include <graphics/text.h>
#include <components/transform.h>
#include <sstream>
#include <iomanip>
#include "score_system.h"
#include "util/constants.h"

ScoreSystem::ScoreSystem(SceneID scene_id) : scene_id(scene_id) {

}

void ScoreSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    auto view = registry.view<Score, Text>();
    for (auto entity: view) {
        auto &text = view.get<Text>(entity);
        auto cam = blackboard.camera;
        std::string score_text;
        std::stringstream ss;
        switch (scene_id) {
            case HORIZONTAL_SCENE_ID: {
                ss << std::setfill('0') << std::setw(6) << (int) abs(cam.position().x) / 100;
            }
                break;
            case VERTICAL_SCENE_ID: {
                ss << std::setfill('0') << std::setw(6) << (int) abs(cam.position().y) / 100;
            }
                break;
            default:
                fprintf(stderr, "Invalid scene ID: %d\n", scene_id);
        }
        score_text = ss.str();
        text.set_text(score_text);
    }

}
