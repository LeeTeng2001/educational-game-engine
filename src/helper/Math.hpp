#pragma once

#include <cmath>
#include <cstring>

namespace Math {
    constexpr float Pi = M_PI;
    constexpr float TwoPi = Pi * 2.0f;
    constexpr float PiOver2 = Pi / 2.0f;
    constexpr float Infinity = std::numeric_limits<float>::infinity();
    constexpr float NegInfinity = -std::numeric_limits<float>::infinity();

    inline float ToRadians(float degrees) {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(float radians) {
        return radians * 180.0f / M_PI;
    }

    inline float Lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

    inline bool NearZero(float val, float epsilon = 0.001f) {
        return fabs(val) <= epsilon;
    }

    inline float Abs(float value) {
        return fabs(value);
    }

    inline float Cos(float angle) {
        return cosf(angle);
    }

    inline float Sin(float angle) {
        return sinf(angle);
    }

    inline float Tan(float angle) {
        return tanf(angle);
    }

    inline float Acos(float value) {
        return acosf(value);
    }

    inline float Atan2(float y, float x) {
        return atan2f(y, x);
    }

    inline float Cot(float angle) {
        return 1.0f / Tan(angle);
    }
}

// Vector2 ----------------------------------------------------------------------
struct Vector2 {
public:
    float x;
    float y;

    Vector2() = default;
    Vector2(float _x, float _y) : x(_x), y(_y) {};

    // Common vector operation
    friend Vector2 operator+(const Vector2 &a, const Vector2 &b);
    friend Vector2 operator-(const Vector2 &a, const Vector2 &b);
    friend Vector2 operator*(const Vector2 &a, const Vector2 &b);  // component wise
    friend Vector2 operator*(const Vector2 &vec, float scalar);  // scalar
    friend Vector2 operator*(float scalar, const Vector2 &vec);

    Vector2 &operator*=(float scalar);
    Vector2 &operator+=(const Vector2 &right);
    Vector2 &operator-=(const Vector2 &right);

    // Common helper methods
    [[nodiscard]] float LengthSq() const;
    [[nodiscard]] float Length() const;
    void Set(float inX, float inY);
    void Normalize();

    // Helper static
    static float Dot(const Vector2& a, const Vector2& b);
    static float AngleBetween(const Vector2& a, const Vector2& b);
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float f);
    static Vector2 Reflect(const Vector2& v, const Vector2& n);
    static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

    static const Vector2 Zero;
};


// 3D Vector ----------------------------------------------------------------------
class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3() = default;
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

    // Cast to a const float pointer
    [[nodiscard]] const float* GetAsFloatPtr() const;

    // Common Vector operator
    friend Vector3 operator+(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a, const Vector3& b);
    friend Vector3 operator*(const Vector3& left, const Vector3& right);
    friend Vector3 operator*(const Vector3& vec, float scalar);
    friend Vector3 operator*(float scalar, const Vector3& vec);

    Vector3& operator*=(float scalar);
    Vector3& operator+=(const Vector3& right);
    Vector3& operator-=(const Vector3& right);

    // Common helper methods
    [[nodiscard]] float LengthSq() const;
    [[nodiscard]] float Length() const;
    void Normalize();
    void Set(float inX, float inY, float inZ);

    // Helper static
    static Vector3 Normalize(const Vector3& vec);
    static float Dot(const Vector3& a, const Vector3& b);
    static Vector3 Cross(const Vector3& a, const Vector3& b);
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float f);
    static Vector3 Reflect(const Vector3& v, const Vector3& n); // Reflect V about (normalized) N

    // 3D
    static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
    // This will transform the vector and renormalize the w component
    static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
    // Transform a Vector3 by a quaternion
    static Vector3 Transform(const Vector3& v, const class Quaternion& q);

    static const Vector3 Zero;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 NegUnitX;
    static const Vector3 NegUnitY;
    static const Vector3 NegUnitZ;
    static const Vector3 Infinity;
    static const Vector3 NegInfinity;
};


// 3x3 Matrix ----------------------------------------------------------------------
class Matrix3 {
public:
    float mat[3][3]{};

    Matrix3();
    explicit Matrix3(float inMat[3][3]);

    // Cast to a const float pointer
    [[nodiscard]] const float *GetAsFloatPtr() const;

    // Matrix multiplication
    friend Matrix3 operator*(const Matrix3 &left, const Matrix3 &right);
    Matrix3 &operator*=(const Matrix3 &right);

    // Create a scale matrix with x and y scales
    static Matrix3 CreateScale(float xScale, float yScale);
    static Matrix3 CreateScale(const Vector2 &scaleVector);
    static Matrix3 CreateScale(float scale);  // uniform factor

    // Create a rotation matrix about the Z axis, theta is in radians
    static Matrix3 CreateRotation(float theta);

    // Create a translation matrix (on the xy-plane)
    static Matrix3 CreateTranslation(const Vector2 &trans);

    static const Matrix3 Identity;
};

// 4x4 Matrix ----------------------------------------------------------------------
class Matrix4 {
public:
    float mat[4][4];

    Matrix4();

    explicit Matrix4(float inMat[4][4]);

    // Cast to a const float pointer
    [[nodiscard]] const float *GetAsFloatPtr() const;

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4 &a, const Matrix4 &b);
    Matrix4 &operator*=(const Matrix4 &right);

    // Invert the matrix - super slow
    void Invert();

    // Get the translation component of the matrix
    [[nodiscard]] Vector3 GetTranslation() const;
    // Get the X axis of the matrix (forward)
    [[nodiscard]] Vector3 GetXAxis() const;
    // Get the Y axis of the matrix (left)
    [[nodiscard]] Vector3 GetYAxis() const;
    // Get the Z axis of the matrix (up)
    [[nodiscard]] Vector3 GetZAxis() const;
    // Extract the scale component from the matrix
    [[nodiscard]] Vector3 GetScale() const;

    // Create a scale matrix with x, y, and z scales
    static Matrix4 CreateScale(float xScale, float yScale, float zScale);
    static Matrix4 CreateScale(const Vector3 &scaleVector);
    static Matrix4 CreateScale(float scale);  // from uniform factor

    // Create rotation about ?-axis
    static Matrix4 CreateRotationX(float theta);
    static Matrix4 CreateRotationY(float theta);
    static Matrix4 CreateRotationZ(float theta);

    // Create a rotation matrix from a quaternion
    static Matrix4 CreateFromQuaternion(const class Quaternion &q);

    static Matrix4 CreateTranslation(const Vector3 &trans);

    static Matrix4 CreateLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

    static Matrix4 CreateOrtho(float width, float height, float near, float far);

    static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far);

    // Create "Simple" View-Projection Matrix from Chapter 6
    static Matrix4 CreateSimpleViewProj(float width, float height);

    static const Matrix4 Identity;
};


// (Unit) Quaternion
class Quaternion {
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Quaternion() {
        *this = Quaternion::Identity;
    }

    // This directly sets the quaternion components --
    // don't use for axis/angle
    explicit Quaternion(float inX, float inY, float inZ, float inW) {
        Set(inX, inY, inZ, inW);
    }

    // Construct the quaternion from an axis and angle
    // It is assumed that axis is already normalized,
    // and the angle is in radians
    explicit Quaternion(const Vector3 &axis, float angle) {
        float scalar = Math::Sin(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = Math::Cos(angle / 2.0f);
    }

    // Directly set the internal components
    void Set(float inX, float inY, float inZ, float inW) {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }

    void Conjugate() {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }

    [[nodiscard]] float LengthSq() const {
        return (x * x + y * y + z * z + w * w);
    }

    [[nodiscard]] float Length() const {
        return sqrtf(LengthSq());
    }

    void Normalize() {
        float length = Length();
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    // Normalize the provided quaternion
    static Quaternion Normalize(const Quaternion &q) {
        Quaternion retVal = q;
        retVal.Normalize();
        return retVal;
    }

    // Linear interpolation
    static Quaternion Lerp(const Quaternion &a, const Quaternion &b, float f) {
        Quaternion retVal;
        retVal.x = Math::Lerp(a.x, b.x, f);
        retVal.y = Math::Lerp(a.y, b.y, f);
        retVal.z = Math::Lerp(a.z, b.z, f);
        retVal.w = Math::Lerp(a.w, b.w, f);
        retVal.Normalize();
        return retVal;
    }

    static float Dot(const Quaternion &a, const Quaternion &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    // Spherical Linear Interpolation
    static Quaternion Slerp(const Quaternion &a, const Quaternion &b, float f) {
        float rawCosm = Quaternion::Dot(a, b);

        float cosom = -rawCosm;
        if (rawCosm >= 0.0f) {
            cosom = rawCosm;
        }

        float scale0, scale1;

        if (cosom < 0.9999f) {
            const float omega = Math::Acos(cosom);
            const float invSin = 1.f / Math::Sin(omega);
            scale0 = Math::Sin((1.f - f) * omega) * invSin;
            scale1 = Math::Sin(f * omega) * invSin;
        } else {
            // Use linear interpolation if the quaternions
            // are collinear
            scale0 = 1.0f - f;
            scale1 = f;
        }

        if (rawCosm < 0.0f) {
            scale1 = -scale1;
        }

        Quaternion retVal;
        retVal.x = scale0 * a.x + scale1 * b.x;
        retVal.y = scale0 * a.y + scale1 * b.y;
        retVal.z = scale0 * a.z + scale1 * b.z;
        retVal.w = scale0 * a.w + scale1 * b.w;
        retVal.Normalize();
        return retVal;
    }

    // Concatenate
    // Rotate by q FOLLOWED BY p
    static Quaternion Concatenate(const Quaternion &q, const Quaternion &p) {
        Quaternion retVal;

        // Vector component is:
        // ps * qv + qs * pv + pv x qv
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);
        Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
        retVal.x = newVec.x;
        retVal.y = newVec.y;
        retVal.z = newVec.z;

        // Scalar component is:
        // ps * qs - pv . qv
        retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

        return retVal;
    }

    static const Quaternion Identity;
};

namespace Color {
    static const Vector3 Black(0.0f, 0.0f, 0.0f);
    static const Vector3 White(1.0f, 1.0f, 1.0f);
    static const Vector3 Red(1.0f, 0.0f, 0.0f);
    static const Vector3 Green(0.0f, 1.0f, 0.0f);
    static const Vector3 Blue(0.0f, 0.0f, 1.0f);
    static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
    static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
    static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
    static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
    static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}
