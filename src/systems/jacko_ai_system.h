//
// Created by Kenneth William on 2019-02-24.
//

#ifndef PANDAEXPRESS_JACKO_AI_SYSTEM_H
#define PANDAEXPRESS_JACKO_AI_SYSTEM_H

#include "system.h"
#include "util/ai_nodes.h"
#include "util/selector_node.h"
#include "components/jacko.h"
#include "components/chases.h"
#include "components/evades.h"
#include "components/panda.h"
#include "components/food.h"
#include "components/transform.h"
#include "components/velocity.h"
#include "components/interactable.h"
#include "components/health.h"

class JackoAISystem: public System {
    private:
        SelectorNode root;
    AINodes::CheckHealth* checkHealth;
    AINodes::CheckPandaDeath* checkPandaDeath;
    public:
    JackoAISystem(Blackboard& blackboard, entt::DefaultRegistry& registry);
    virtual void update(Blackboard& blackboard, entt::DefaultRegistry& registry) override;
    int getPandaHealth(Blackboard& blackboard, entt::DefaultRegistry& registry);
    int getJackoHealth(Blackboard& blackboard, entt::DefaultRegistry& registry);
    void evasion(Blackboard& blackboard, entt::DefaultRegistry& registry);
};


#endif //PANDAEXPRESS_JACKO_AI_SYSTEM_H
