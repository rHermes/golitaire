//
// Created by rhermes on 1/25/23.
//

#include "GraphicsRenderer2D.h"
#include "Viewport.h"
#include "EllipseRenderer2D.h"

using namespace LTK;

GraphicsRenderer2D::GraphicsRenderer2D(std::shared_ptr<ResourceManager> resManager)
: resManager_{std::move(resManager)}
{
    ellipseProgram_ = resManager_->loadShaderProgram("data/shaders/Ellipse2DLine.vert", "data/shaders/Ellipse2DLine.frag");
}

GraphicsRenderer2D::~GraphicsRenderer2D() {
    resManager_->removeProgram(ellipseProgram_);
}

void GraphicsRenderer2D::render(const glm::mat4& proj, const glm::mat4& view, const Viewport& viewport) {
    currentProgram_ = 0;

    for (const auto& ri : renderItems_) {
        ri->render(*this, proj, view);
    }

}

void GraphicsRenderer2D::drawEllipse(const EllipseRenderer2D &el, const glm::mat4 &proj, const glm::mat4 &view,
                                     const Viewport &vp) {
    auto& prog = resManager_->getProgram(ellipseProgram_);
    if (currentProgram_ != ellipseProgram_) {
        prog.use();
        currentProgram_ = ellipseProgram_;
    }

    el.bindVAO();
    prog.setMat4("mvp", proj * view);

    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(el.count()));
}

void GraphicsRenderer2D::addRenderItem(std::shared_ptr<RenderItem2D> renderItem) {
    // TODO(rHermes): We need to resort the render item, once we are done with it.
    renderItems_.push_back(std::move(renderItem));
}
