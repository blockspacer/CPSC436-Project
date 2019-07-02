//
// Created by Kenneth William on 2019-02-24.
//

#include "dracula_ai_system.h"
#include <iostream>
using namespace std;

/***
 * Making these nodes global variables so they can be accessed from the update function
 */


DraculaAISystem::DraculaAISystem(Blackboard& blackboard, entt::DefaultRegistry& registry):
        a_star_system(blackboard, registry)
{

    // Declare nodes
    root = SelectorNode();
    SelectorNode* firstSelector = new SelectorNode;
    checkPandaDeath = new AINodes::CheckPandaDeath(getPandaHealth(blackboard,registry));
    checkHealth = new AINodes::CheckHealth(getDraculaHealth(blackboard,registry));
    AINodes::ChasePanda* chasePanda = new AINodes::ChasePanda(blackboard, registry);
    AINodes::ChaseFood* chaseFood = new AINodes::ChaseFood(blackboard, registry);
    AINodes::StompPanda* stompPanda = new AINodes::StompPanda(blackboard, registry);
    AINodes::ShootBat* shootBat = new AINodes::ShootBat(blackboard,registry, a_star_system);
    AINodes::TeleportAttack* teleportAttack = new AINodes::TeleportAttack(blackboard, registry, a_star_system);
    AINodes::TeleportIntoFrame* teleportIntoFrame = new AINodes::TeleportIntoFrame(blackboard, registry, a_star_system);


    // Building the behaviour tree
    root.addChild(firstSelector);

    //firstSelector->addChild(teleportIntoFrame);
    firstSelector->addChild(checkPandaDeath);
    firstSelector->addChild(shootBat);
    firstSelector->addChild(teleportAttack);
    firstSelector->addChild(checkHealth);
    firstSelector->addChild(chasePanda);
    firstSelector->addChild(stompPanda);

    //checkHealth->addChild(chaseFood);

}

void DraculaAISystem::update(Blackboard& blackboard, entt::DefaultRegistry& registry){
    checkHealth->updateHealth(getDraculaHealth(blackboard,registry));
    checkPandaDeath->updateHealth(getPandaHealth(blackboard,registry));
    root.run();
}

int DraculaAISystem::getPandaHealth(Blackboard& blackboard, entt::DefaultRegistry& registry){
    auto panda_view = registry.view<Panda, Health>();
    for(auto panda_entity : panda_view){
        auto &health = panda_view.get<Health>(panda_entity);
        return health.health_points;
    }
    return 0;
}

int DraculaAISystem::getDraculaHealth(Blackboard& blackboard, entt::DefaultRegistry& registry){
    auto drac_view = registry.view<Dracula, Health>();
    for(auto drac_entity : drac_view){
        auto &health = drac_view.get<Health>(drac_entity);
        return health.health_points;
    }
    return 0;
}

void DraculaAISystem::cleanup(){
    a_star_system.cleanup();
}
