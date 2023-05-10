//
// Created by rhermes on 2/1/23.
//

#pragma once

namespace LTK {
    class GraphicsRenderer2D;

class RenderItem2D {

public:
    RenderItem2D() = default;
    virtual ~RenderItem2D() = default;

    // Remove copy and assign operators, to avoid shearing
    RenderItem2D(const RenderItem2D&) = delete;
    RenderItem2D& operator=(const RenderItem2D&) = delete;
    RenderItem2D(RenderItem2D&&) = delete;
    RenderItem2D& operator=(RenderItem2D&&) = delete;

    virtual void render(const GraphicsRenderer2D&) = 0;
};

} // LTK
