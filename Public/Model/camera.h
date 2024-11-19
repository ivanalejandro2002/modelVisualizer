#pragma once
#include "../Geometry/vec3_t.h"
#include "../Mat4_t/Mat4_t.h"
class Camera{
  private:
    Vec3_t position;
    Vec3_t target;
    Mat4_t directionMatrix;
    Mat4_t antiDirectionalMatrix;
  public:
    Camera();
    ~Camera();
    Vec3_t getPosition();
    Vec3_t getTarget();
    void setPosition(const Vec3_t &position);
    void setTarget(const Vec3_t &target);
    Mat4_t getDirectionMatrix();
    Mat4_t getAntiDirectionalMatrix();
};