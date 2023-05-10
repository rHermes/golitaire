//
// Created by rhermes on 1/25/23.
//

#pragma once

#include <vector>
#include "ResourceManager.h"
#include "RenderItem2D.h"

namespace LTK {

    // A renderer for 2d graphics elements.
class GraphicsRenderer2D {
private:
    std::shared_ptr<ResourceManager> resManager_;

    ResourceManager::Key ellipseProgram_{0};

    std::vector<std::shared_ptr<RenderItem2D>> renderItems_;
public:

    explicit GraphicsRenderer2D(std::shared_ptr<ResourceManager> resManager);
    ~GraphicsRenderer2D();

    void render();
};

} // LTK
