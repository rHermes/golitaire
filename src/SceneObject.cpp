//
// Created by rhermes on 11/12/22.
//

#include "SceneObject.h"

using namespace gol;

void SceneObject::updateTransform() const {
    // The order of transforms are:
    // pre -> scale -> rotate -> translate -> post

    transform_ = postTransform_;
    transform_ = glm::translate(transform_, position_);
    transform_ *= static_cast<glm::mat4>(rotation_);
    transform_ = glm::scale(transform_, glm::vec3(scale_));
    transform_ *= preTransform_;

    transform_dirty_ = false;
}

const glm::vec3 &SceneObject::getPosition() const {
    return position_;
}

void SceneObject::setPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTrans;
    transform_dirty_ = true;
}

void SceneObject::appendPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTrans * postTransform_;
    transform_dirty_ = true;
}

void SceneObject::prependPostTransform(const glm::mat4 &postTrans) {
    postTransform_ = postTransform_ * postTrans;
    transform_dirty_ = true;
}

void SceneObject::setPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTrans;
    transform_dirty_ = true;
}

void SceneObject::appendPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTrans * preTransform_;
    transform_dirty_ = true;
}

void SceneObject::prependPreTransform(const glm::mat4 &preTrans) {
    preTransform_ = preTransform_ * preTrans;
    transform_dirty_ = true;
}

void SceneObject::setScale(float scale) {
    scale_ = scale;
    transform_dirty_ = true;
}

void SceneObject::setPosition(glm::vec3 pos) {
    position_ = std::move(pos);
    transform_dirty_ = true;
}

const glm::mat4 &SceneObject::getTransform() const {
    // We are a bit naughty here, and call the update transform here.
    if (transform_dirty_)
        updateTransform();

    return transform_;
}

void SceneObject::rotateX(const float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(1.0f, 0.0f, 0.0f));
    transform_dirty_ = true;
}

void SceneObject::rotateY(const float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(0.0f, 1.0f, 0.0f));
    transform_dirty_ = true;
}

void SceneObject::rotateZ(float radians) {
    rotation_ = glm::rotate(rotation_, radians, glm::vec3(0.0f, 0.0f, 1.0f));
    transform_dirty_ = true;
}

void SceneObject::setRotation(float pitch, float yaw, float roll) {
    setRotation(glm::vec3(pitch, yaw, roll));
}

void SceneObject::setRotation(const glm::vec3 &eulerAngles) {
    rotation_ = glm::quat(eulerAngles);
    transform_dirty_ = true;
}

void SceneObject::setRotation(const glm::quat &quaterion) {
    rotation_ = quaterion;
    transform_dirty_ = true;
}
