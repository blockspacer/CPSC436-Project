//
// Created by Kenneth William on 2019-03-27.
//

#ifndef PANDAEXPRESS_A_STAR_SYSTEM_H
#define PANDAEXPRESS_A_STAR_SYSTEM_H

#include "system.h"
#include "components/panda.h"
#include "components/dracula.h"
#include "components/transform.h"
#include "components/collidable.h"
#include "util/csv_reader.h"
#include "util/constants.h"
#include <iostream>
#include "util/Location.h"
#include "util/coordinates.h"

class AStarSystem: public System{
public:
    AStarSystem(Blackboard &blackboard, entt::DefaultRegistry &registry);
    void destroyGrid();
    virtual void update(Blackboard &blackboard, entt::DefaultRegistry &registry);
    void createGrid(Blackboard &blackboard, entt::DefaultRegistry &registry);
    std::vector<Location*> findPath(Location* start, Location* end);
    std::vector<Coordinates> getProjectilePath(Blackboard &blackboard, entt::DefaultRegistry &registry);
    Location* getGridLocation(float x, float y);
    Coordinates getScreenLocation(int i, int j);
    void cleanup();

private:
    float Y_OFFSET = 30.f;
    float X_OFFSET = -50.f;
    int cols=0;
    int rows=0;
    std::vector<std::vector<Location*>> grid;
    std::vector<Location> data;
    bool startedInPlatform=false;
};


#endif //PANDAEXPRESS_A_STAR_SYSTEM_Hs
