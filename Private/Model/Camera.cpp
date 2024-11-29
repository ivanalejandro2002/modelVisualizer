#include "../../Public/Model/Camera.h"
Camera::Camera(): position({0,0,0}), target({0,0,0}) {
    directionMatrix = Mat4_t::identity();
    antiDirectionalMatrix = Mat4_t::identity();
}

Camera::~Camera() = default;

Vec3_t Camera::getPosition() {
    return this->position;
}
Vec3_t Camera::getTarget() {
    return this->target;
}
void Camera::setPosition(const Vec3_t &position) {
    this->position = position;
}
void Camera::setTarget(const Vec3_t &target) {
    this->target = target;
}

Mat4_t Camera::getDirectionMatrix() {
    return this->directionMatrix;
}
Mat4_t Camera::getAntiDirectionalMatrix() {
    return this->antiDirectionalMatrix;
}

void Camera::rotateCamera(double x, double y, double z) {
    directionMatrix = directionMatrix * Mat4_t::make_rotate(x,y,z);
    antiDirectionalMatrix = antiDirectionalMatrix * Mat4_t::make_rotate(-x,-y,-z);
}