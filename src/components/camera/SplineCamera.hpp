#pragma once

#include "CameraComponent.hpp"
#include <vector>

struct Spline {
    // Control points for spline (Requires n+2 points where n is numberof points in segment)
    std::vector<Vector3> mControlPoints;
    // Given spline segment where startIdx = P1, compute position based on t value
    [[nodiscard]] Vector3 Compute(size_t startIdx, float t) const;
    // Returns number of control points
    [[nodiscard]] size_t GetNumPoints() const { return mControlPoints.size(); }
};


class SplineCamera : public CameraComponent {
public:
    explicit SplineCamera(class Actor *owner);

    void Update(float deltaTime) override;

    // Restart the spline
    void Restart();

    // Setter
    void SetSpeed(float speed) { mSpeed = speed; }
    void SetSpline(const Spline &spline) { mPath = spline; }
    void SetPaused(bool pause) { mPaused = pause; }

private:
    // Spline path camera follows
    Spline mPath;
    // Current control point index and t
    size_t mIndex = 1;
    float mT = 0;
    // Amount t changes/sec
    float mSpeed = 0.5f;
    // Whether to move the camera long the path
    bool mPaused = true;
};
