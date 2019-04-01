//
// Created by cowan on 17/02/19.
//

#include <util/constants.h>
#include "vertical_level_system.h"
#include <components/timer.h>
#include <algorithm>

VerticalLevelSystem::VerticalLevelSystem() : LevelSystem() {
    for (int i = 0; i <= MAX_DIFFICULTY; i++) {
        levels[i] = Level::load_level(i, VERTICAL_LEVEL_TYPE);
    }
}

void VerticalLevelSystem::init() {
    LevelSystem::init();

    if (mode_ == ENDLESS) {
        rng_.init((unsigned int) rand());
    }

    last_row_generated_ = last_row_loaded_ = FIRST_ROW_Y;
    difficulty = MIN_DIFFICULTY;
    difficulty_timer.save_watch(LEVEL_UP_LABEL, LEVEL_UP_INTERVAL);
    load_next_chunk(0);
}

void VerticalLevelSystem::load_next_chunk() {
    int level = rng_.nextInt(std::max(0, difficulty - 5), difficulty);
    load_next_chunk(level);
}

void VerticalLevelSystem::load_next_chunk(int id) {
    Level lvl = levels[id];
    for (int y = (int) lvl.height() - 1; y >= 0; y--) {
        std::vector<char> row;
        row.reserve(lvl.width());
        for (int x = 0; x < lvl.width(); x++) {
            row.push_back(lvl.get_tile_at(x, y));
        }
        last_row_loaded_ -= CELL_HEIGHT;
        chunks_.push(row);
    }
}

void VerticalLevelSystem::generate_next_chunk(Blackboard &blackboard,
                                              entt::DefaultRegistry &registry) {
    float off_screen = blackboard.camera.position().y - blackboard.camera.size().x;
    while (last_row_generated_ > off_screen && !chunks_.empty()) {
        std::vector<char> col = chunks_.front();
        float x = COL_X_OFFSET;
        for (int c:col) {
            generateEntity(c, x, last_row_generated_, blackboard, registry);
            x += CELL_WIDTH;
        }
        last_row_generated_ -= CELL_HEIGHT;
        chunks_.pop();
    }
}

void VerticalLevelSystem::destroy_entities(entt::DefaultRegistry &registry) {
    registry.destroy<Spit>(); // Destroy Spit here since it was not generated by LevelSystem
    LevelSystem::destroy_entities(registry);
}

void VerticalLevelSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    float max_y =
            blackboard.camera.position().y + blackboard.camera.size().y; // some distance off camera
    float min_y =
            blackboard.camera.position().y - blackboard.camera.size().y; // some distance off camera
    while (last_row_loaded_ > min_y) {
        load_next_chunk();
    }

    difficulty_timer.update(blackboard.delta_time);

    if (difficulty < MAX_DIFFICULTY && difficulty_timer.is_done(LEVEL_UP_LABEL)) {
        difficulty++;
        difficulty_timer.reset_watch(LEVEL_UP_LABEL);
    }

    destroy_off_screen(registry, max_y);
    generate_next_chunk(blackboard, registry);
}

void VerticalLevelSystem::destroy_off_screen(entt::DefaultRegistry &registry, float max_y) {
    auto platforms = registry.view<Platform, Transform>();
    for (uint32_t entity: platforms) {
        auto &transform = platforms.get<Transform>(entity);
        if (transform.y > max_y) {
            registry.destroy(entity);
        }
    }

    auto llamas = registry.view<Llama, Transform>();
    for (uint32_t entity: llamas) {
        auto &transform = llamas.get<Transform>(entity);
        if (transform.y > max_y) {
            registry.destroy(entity);
        }
    }

    auto spits = registry.view<Spit, Transform>();
    for (uint32_t entity: spits) {
        auto &transform = spits.get<Transform>(entity);
        if (transform.y > max_y) {
            registry.destroy(entity);
        }
    }
}

void VerticalLevelSystem::set_mode(SceneMode mode) {
    mode_ = mode;
    if (mode_ == ENDLESS) {
        rng_.init((unsigned int) rand());
    } else if (mode_ == STORY) {
        rng_.init(STORY_SEED);
    }
}
