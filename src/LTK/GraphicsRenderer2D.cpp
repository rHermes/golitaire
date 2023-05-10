//
// Created by rhermes on 1/25/23.
//

#include "GraphicsRenderer2D.h"

using namespace LTK;

GraphicsRenderer2D::GraphicsRenderer2D(std::shared_ptr<ResourceManager> resManager)
: resManager_{std::move(resManager)}
{
    ellipseProgram_ = resManager_->loadShaderProgram("data/shaders/Ellipse2DLine.vert", "data/shaders/Ellipse2DLine.frag");
}

GraphicsRenderer2D::~GraphicsRenderer2D() {
    resManager_->removeProgram(ellipseProgram_);
}

void GraphicsRenderer2D::render() {
    
}
