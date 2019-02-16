//
// Created by Prayansh Srivastava on 2019-02-15.
//
#include "level_system.h"

#ifndef PANDAEXPRESS_HORIZONTAL_LEVEL_SYSTEM_H
#define PANDAEXPRESS_HORIZONTAL_LEVEL_SYSTEM_H

#endif //PANDAEXPRESS_HORIZONTAL_LEVEL_SYSTEM_H

class HorizontalLevelSystem : public LevelSystem {
    void load_next_chunk();

    void generate_next_chunk(Blackboard &blackboard, entt::DefaultRegistry &registry);

public:

    HorizontalLevelSystem();

    void update(Blackboard &blackboard, entt::DefaultRegistry &registry);

    void destroy_entities(entt::DefaultRegistry &registry);

    void destroy_off_screen(entt::DefaultRegistry &registry, float x);
};