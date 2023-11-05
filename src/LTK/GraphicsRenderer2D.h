//
// Created by rhermes on 1/25/23.
//

#pragma once

#include <vector>
#include "ResourceManager.h"
#include "RenderItem2D.h"

namespace LTK {

    class Viewport;
    class EllipseRenderer2D;

/*** A renderer for 2d graphics elements.
 *
 * The architecture here is kind of wonky, but there is a reason for it.
 * As the only collection we have is of "RenderItem2DS", we cannot unify
 * all the rendering into just one function, it would involve a huge "if else"
 * test on the type of element we have. Instead we use a mechanism where the
 * individual elements will call back to the graphics renderer with a specific function.
 *
 * This is sort of cheesy, but it works and it's efficient. The rendering of a single frame
 * goes something like this:
 *
 * 1. The "render" function is called and variables that are common for the entire frame is setup
 *    This includes things like the viewport resolution and other variables which will have the
 *    same value across all the invocations of a shader program for this frame
 *
 * 2. The renderer steps through each of the render items it has and calls the "render" function.
 *
 * 3. Most, if not all, RenderItems call back to one of the functions in the graphics renderer,
 *    such as "drawEllipse". This then uses the shaders that was setup in step 1, to do the drawing.
 *
 * This allows for efficient drawing.
 */
class GraphicsRenderer2D {
private:
    std::shared_ptr<ResourceManager> resManager_;

    ResourceManager::Key ellipseProgram_{0};

    std::vector<std::shared_ptr<RenderItem2D>> renderItems_;

    ResourceManager::Key currentProgram_{0};
public:

    explicit GraphicsRenderer2D(std::shared_ptr<ResourceManager> resManager);
    ~GraphicsRenderer2D();

    void render(const glm::mat4& proj, const glm::mat4& view, const Viewport& vp);

    void addRenderItem(std::shared_ptr<RenderItem2D> renderItem);

    // These are specific to the underlying render items.
    void drawEllipse(const EllipseRenderer2D& el, const glm::mat4& proj, const glm::mat4& view, const Viewport& vp);
};

} // LTK
