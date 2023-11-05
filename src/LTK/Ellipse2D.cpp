//
// Created by rhermes on 1/18/23.
//

#include "Ellipse2D.h"

using namespace LTK;

Ellipse2D::Ellipse2D(EllipseRenderer2D &renderer, size_t renderIndex)
: renderer_{renderer}, renderIndex_{renderIndex} {
}

void Ellipse2D::updateTransform() const {
    Item3D::updateTransform();

}
