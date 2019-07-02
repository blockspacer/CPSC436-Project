//
// Created by Kenneth William on 2019-02-24.
//

#include "jacko_ai_system.h"
#include <iostream>
using namespace std;



JackoAISystem::JackoAISystem(Blackboard& blackboard, entt::DefaultRegistry& registry) {
    // Declare nodes
    root = SelectorNode();
    SelectorNode* firstSelector = new SelectorNode;
    checkPandaDeath = new AINodes::CheckPandaDeath(getPandaHealth(blackboard,registry));
    checkHealth = new AINodes::CheckHealth(getJackoHealth(blackboard,registry));
    AINodes::ChasePanda* chasePanda = new AINodes::ChasePanda(blackboard, registry);
    AINodes::ChaseFood* chaseFood = new AINodes::ChaseFood(blackboard, registry);
    AINodes::StompPanda* stompPanda = new AINodes::StompPanda(blackboard, registry);



    // Building the behaviour tree
    root.addChild(firstSelector);

    firstSelector->addChild(checkPandaDeath);
    firstSelector->addChild(checkHealth);
    firstSelector->addChild(chasePanda);
    firstSelector->addChild(stompPanda);

    checkHealth->addChild(chaseFood);

}

void JackoAISystem::update(Blackboard& blackboard, entt::DefaultRegistry& registry){
    checkHealth->updateHealth(getJackoHealth(blackboard,registry));
    checkPandaDeath->updateHealth(getPandaHealth(blackboard,registry));
    root.run();
}

int JackoAISystem::getPandaHealth(Blackboard& blackboard, entt::DefaultRegistry& registry){
    auto panda_view = registry.view<Panda, Health>();
    for(auto panda_entity : panda_view){
        auto &health = panda_view.get<Health>(panda_entity);
        return health.health_points;
    }
    return 0;
}

int JackoAISystem::getJackoHealth(Blackboard& blackboard, entt::DefaultRegistry& registry){
    auto jacko_view = registry.view<Jacko, Health>();
    for(auto jacko_entity : jacko_view){
        auto &health = jacko_view.get<Health>(jacko_entity);
        return health.health_points;
    }
    return 0;
}

