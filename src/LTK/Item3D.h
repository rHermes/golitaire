#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace LTK {

    /// An object that will be rendered by the SceneRenderer
class Item3D {
protected:
    // We store the rotation as a quaterion, but we may manipulate it anyway we want.
    glm::quat rotation_{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 position_{0, 0, 0};
    glm::vec3 scale_{1.0};

    glm::mat4 preTransform_{1.0f};
    glm::mat4 postTransform_{1.0f};

    mutable bool transform_dirty_{true};
    mutable glm::mat4 transform_{1.0f};

    // This is not really a const operation, but the semantics are const
    virtual void updateTransform() const;

public:
    void setScale(float scale);
    void setScale(glm::vec3 scale);
    void setScaleX(const float scale);
    void setScaleY(const float scale);
    void setScaleZ(const float scale);

    void setPosition(glm::vec3 pos);


    // Set the rotation, first applying pitch, then yaw, then roll
    void setRotation(float pitch, float yaw, float roll);
    void setRotation(const glm::vec3& eulerAngles);
    void setRotation(const glm::quat& quaterion);

    // Rotate the object
    void rotateX(glm::vec3::value_type radians);
    void rotateY(glm::vec3::value_type radians);
    void rotateZ(glm::vec3::value_type radians);

    /// Set the transformation applied after the other transformations
    void setPostTransform(const glm::mat4& postTrans);

    /// Add a transformation after the existing post transforms
    void appendPostTransform(const glm::mat4& postTrans);

    /// Add a transformation before the existing post transforms
    void prependPostTransform(const glm::mat4& postTrans);

    /// Set the transformation applied before the other transformations
    void setPreTransform(const glm::mat4& preTrans);

    /// Add a transformation after the existing pre transforms
    void appendPreTransform(const glm::mat4& preTrans);

    // Add a transformation before the existing pre transforms
    void prependPreTransform(const glm::mat4& preTrans);


    [[nodiscard]] const glm::mat4& getTransform() const;
    [[nodiscard]] const glm::vec3& getPosition() const;
};

} // gol
