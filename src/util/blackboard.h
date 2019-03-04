//
// Created by alex on 20/01/19.
//

#pragma once

#include "input_manager.h"

#include "../graphics/camera.h"
#include "../graphics/mesh_manager.h"
#include "../graphics/shader_manager.h"
#include "../graphics/texture_manager.h"
#include "../graphics/window.h"
#include "../util/random.h"


// Struct containing all our singletons
// All require individual initialization
struct Blackboard {
    Camera camera;
    float delta_time;
    InputManager input_manager;
    MeshManager mesh_manager;
    ShaderManager shader_manager;
    TextureManager texture_manager;
    Window& window;
    Random randNumGenerator;
};