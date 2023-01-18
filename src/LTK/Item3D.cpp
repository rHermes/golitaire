//
// Created by rhermes on 11/12/22.
//

#include "Item3D.h"

using namespace LTK;

void Item3D::updateTransform() const {
    // The order of transforms are:
    // pre -> scale -> rotate -> translate -> post

    transform_ = postTransform_;
    transform_ = glm::translate(transform_, position_);
    transform_ *= static_cast<glm::mat4>(rotation_);
    transform_ = glm::scale(transform_, glm::vec3(scale_));
    transform_ *= preTransform_;

    transform_dirty_ = false;
}

const glm::vec3 &Item3D::getPosition() const {
    return position_;
}

void Item3D::setPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTrans;
    transform_dirty_ = true;
}

void Item3D::appendPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTrans * postTransform_;
    transform_dirty_ = true;
}

void Item3D::prependPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTransform_ * postTrans;
    transform_dirty_ = true;
}

void Item3D::setPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTrans;
    transform_dirty_ = true;
}

void Item3D::appendPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTrans * preTransform_;
    transform_dirty_ = true;
}

void Item3D::prependPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTransform_ * preTrans;
    transform_dirty_ = true;
}

void Item3D::setScale(float scale) {
    scale_ = scale;
    transform_dirty_ = true;
}

void Item3D::setPosition(glm::vec3 pos) {
    position_ = std::move(pos);
    transform_dirty_ = true;
}

const glm::mat4 &Item3D::getTransform() const {
    // We are a bit naughty here, and call the update transform here.
    if (transform_dirty_)
        updateTransform();

    return transform_;
}

void Item3D::rotateX(const float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(1.0f, 0.0f, 0.0f));
    transform_dirty_ = true;
}

void Item3D::rotateY(const float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(0.0f, 1.0f, 0.0f));
    transform_dirty_ = true;
}

void Item3D::rotateZ(float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(0.0f, 0.0f, 1.0f));
    transform_dirty_ = true;
}

void Item3D::setRotation(float pitch, float yaw, float roll) {
    setRotation(glm::vec3(pitch, yaw, roll));
}

void Item3D::setRotation(const glm::vec3 &eulerAngles) {
    rotation_ = glm::quat(eulerAngles);
    transform_dirty_ = true;
}

void Item3D::setRotation(const glm::quat &quaterion) {
    rotation_ = quaterion;
    transform_dirty_ = true;
}
