#include "Math.hpp"

const Vector2 Vector2::Zero{0.0f, 0.0f};

Vector2 operator+(const Vector2 &a, const Vector2 &b) {
    return {a.x + b.x, a.y + b.y};
}

Vector2 operator-(const Vector2 &a, const Vector2 &b) {
    return {a.x - b.x, a.y - b.y};
}

Vector2 operator*(const Vector2 &a, const Vector2 &b) {
    return {a.x * b.x, a.y * b.y};
}

Vector2 operator*(const Vector2 &vec, float scalar) {
    return {vec.x * scalar, vec.y * scalar};
}

Vector2 operator*(float scalar, const Vector2 &vec) {
    return {vec.x * scalar, vec.y * scalar};
}

Vector2 &Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 &Vector2::operator+=(const Vector2 &right) {
    x += right.x;
    y += right.y;
    return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &right) {
    x -= right.x;
    y -= right.y;
    return *this;
}

float Vector2::LengthSq() const {
    return (x*x + y*y);
}

float Vector2::Length() const {
    return (sqrtf(LengthSq()));
}

void Vector2::Normalize() {
    float length = Length();
    x /= length;
    y /= length;
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b) {
    return (a.x * b.x + a.y * b.y);
}

float Vector2::AngleBetween(const Vector2 &a, const Vector2 &b) {
    return acosf(Dot(a, b));
}

Vector2 Vector2::Lerp(const Vector2 &a, const Vector2 &b, float f) {
    return Vector2(a + f * (b - a));
}

Vector2 Vector2::Reflect(const Vector2 &v, const Vector2 &n) {
    return v - 2.0f * Vector2::Dot(v, n) * n;
}

Vector2 Vector2::Transform(const Vector2& vec, const Matrix3& mat, float w /*= 1.0f*/) {
    Vector2 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] + w * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] + w * mat.mat[2][1];
    //ignore w since we aren't returning a new value for it...
    return retVal;
}

void Vector2::Set(float inX, float inY) {
    x = inX;
    y = inY;
}

// Vector3 ----------------------------------------------------------------------------d

const float *Vector3::GetAsFloatPtr() const {
    return reinterpret_cast<const float*>(&x);
}

Vector3 operator+(const Vector3 &a, const Vector3 &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 operator-(const Vector3 &a, const Vector3 &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 operator*(const Vector3 &left, const Vector3 &right) {
    return {left.x * right.x, left.y * right.y, left.z * right.z};
}

Vector3 operator*(const Vector3 &vec, float scalar) {
    return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}

Vector3 operator*(float scalar, const Vector3 &vec) {
    return {vec.x * scalar, vec.y * scalar, vec.z * scalar};
}

Vector3 &Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 &Vector3::operator+=(const Vector3 &right) {
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &right) {
    x -= right.x;
    y -= right.y;
    z -= right.z;
    return *this;
}

float Vector3::LengthSq() const {
    return (x*x + y*y + z*z);
}

float Vector3::Length() const {
    return (sqrtf(LengthSq()));
}

void Vector3::Normalize() {
    float length = Length();
    x /= length;
    y /= length;
    z /= length;
}

void Vector3::Set(float inX, float inY, float inZ) {
    x = inX;
    y = inY;
    z = inZ;
}

Vector3 Vector3::Normalize(const Vector3 &vec) {
    Vector3 temp = vec;
    temp.Normalize();
    return temp;
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

Vector3 Vector3::Lerp(const Vector3 &a, const Vector3 &b, float f) {
    return Vector3(a + f * (b - a));
}

Vector3 Vector3::Reflect(const Vector3 &v, const Vector3 &n) {
    return v - 2.0f * Vector3::Dot(v, n) * n;
}

Vector3 Vector3::Transform(const Vector3 &vec, const Matrix4 &mat, float w /*= 1.0f*/) {
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
               vec.z * mat.mat[2][0] + w * mat.mat[3][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
               vec.z * mat.mat[2][1] + w * mat.mat[3][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
               vec.z * mat.mat[2][2] + w * mat.mat[3][2];
    //ignore w since we aren't returning a new value for it...
    return retVal;
}

// This will transform the vector and renormalize the w component
Vector3 Vector3::TransformWithPerspDiv(const Vector3 &vec, const Matrix4 &mat, float w /*= 1.0f*/) {
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
               vec.z * mat.mat[2][0] + w * mat.mat[3][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
               vec.z * mat.mat[2][1] + w * mat.mat[3][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
               vec.z * mat.mat[2][2] + w * mat.mat[3][2];
    float transformedW = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
                         vec.z * mat.mat[2][3] + w * mat.mat[3][3];
    if (!Math::NearZero(abs(transformedW))) {
        transformedW = 1.0f / transformedW;
        retVal *= transformedW;
    }
    return retVal;
}

// Transform a Vector3 by a quaternion
Vector3 Vector3::Transform(const Vector3 &v, const Quaternion &q) {
    // v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
    Vector3 qv(q.x, q.y, q.z);
    Vector3 retVal = v;
    retVal += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, v) + q.w * v);
    return retVal;
}

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.f);
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NegUnitX(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NegUnitY(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NegUnitZ(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Infinity(Math::Infinity, Math::Infinity, Math::Infinity);
const Vector3 Vector3::NegInfinity(Math::NegInfinity, Math::NegInfinity, Math::NegInfinity);

// Matrix3 ----------------------------------------------------------------
static float m3Ident[3][3] =
        {
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f }
        };
const Matrix3 Matrix3::Identity(m3Ident);

Matrix3::Matrix3() {
    *this = Matrix3::Identity;
}

Matrix3::Matrix3(float (*inMat)[3]) {
    memcpy(mat, inMat, 9 * sizeof(float));
}

const float *Matrix3::GetAsFloatPtr() const {
    return reinterpret_cast<const float *>(&mat[0][0]);
}

Matrix3 operator*(const Matrix3 &left, const Matrix3 &right) {
    Matrix3 retVal;
    // row 0
    retVal.mat[0][0] =
            left.mat[0][0] * right.mat[0][0] +
            left.mat[0][1] * right.mat[1][0] +
            left.mat[0][2] * right.mat[2][0];

    retVal.mat[0][1] =
            left.mat[0][0] * right.mat[0][1] +
            left.mat[0][1] * right.mat[1][1] +
            left.mat[0][2] * right.mat[2][1];

    retVal.mat[0][2] =
            left.mat[0][0] * right.mat[0][2] +
            left.mat[0][1] * right.mat[1][2] +
            left.mat[0][2] * right.mat[2][2];

    // row 1
    retVal.mat[1][0] =
            left.mat[1][0] * right.mat[0][0] +
            left.mat[1][1] * right.mat[1][0] +
            left.mat[1][2] * right.mat[2][0];

    retVal.mat[1][1] =
            left.mat[1][0] * right.mat[0][1] +
            left.mat[1][1] * right.mat[1][1] +
            left.mat[1][2] * right.mat[2][1];

    retVal.mat[1][2] =
            left.mat[1][0] * right.mat[0][2] +
            left.mat[1][1] * right.mat[1][2] +
            left.mat[1][2] * right.mat[2][2];

    // row 2
    retVal.mat[2][0] =
            left.mat[2][0] * right.mat[0][0] +
            left.mat[2][1] * right.mat[1][0] +
            left.mat[2][2] * right.mat[2][0];

    retVal.mat[2][1] =
            left.mat[2][0] * right.mat[0][1] +
            left.mat[2][1] * right.mat[1][1] +
            left.mat[2][2] * right.mat[2][1];

    retVal.mat[2][2] =
            left.mat[2][0] * right.mat[0][2] +
            left.mat[2][1] * right.mat[1][2] +
            left.mat[2][2] * right.mat[2][2];

    return retVal;
}

Matrix3 &Matrix3::operator*=(const Matrix3 &right) {
    *this = *this * right;
    return *this;
}

Matrix3 Matrix3::CreateScale(float xScale, float yScale) {
    float temp[3][3] = {
                    {xScale, 0.0f,   0.0f},
                    {0.0f,   yScale, 0.0f},
                    {0.0f,   0.0f,   1.0f},
            };
    return Matrix3(temp);
}

Matrix3 Matrix3::CreateScale(const Vector2 &scaleVector) {
    return CreateScale(scaleVector.x, scaleVector.y);
}

Matrix3 Matrix3::CreateScale(float scale) {
    return CreateScale(scale, scale);
}

Matrix3 Matrix3::CreateRotation(float theta) {
    float temp[3][3] =
            {
                    {cosf(theta),  sinf(theta), 0.0f},
                    {-sinf(theta), cosf(theta), 0.0f},
                    {0.0f,              0.0f,             1.0f},
            };
    return Matrix3(temp);
}

Matrix3 Matrix3::CreateTranslation(const Vector2 &trans) {
    float temp[3][3] =
            {
                    {1.0f,    0.0f,    0.0f},
                    {0.0f,    1.0f,    0.0f},
                    {trans.x, trans.y, 1.0f},
            };
    return Matrix3(temp);
}

// Matrix4 ------------------------------------------------------------
static float m4Ident[4][4] =
        {
                { 1.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f, 1.0f }
        };

const Matrix4 Matrix4::Identity(m4Ident);

Matrix4::Matrix4() {
    *this = Matrix4::Identity;
}

Matrix4::Matrix4(float (*inMat)[4]) {
    memcpy(mat, inMat, 16 * sizeof(float));
}

const float *Matrix4::GetAsFloatPtr() const {
    return reinterpret_cast<const float *>(&mat[0][0]);
}

Matrix4 operator*(const Matrix4 &a, const Matrix4 &b) {
    Matrix4 retVal;
    // row 0
    retVal.mat[0][0] =
            a.mat[0][0] * b.mat[0][0] +
            a.mat[0][1] * b.mat[1][0] +
            a.mat[0][2] * b.mat[2][0] +
            a.mat[0][3] * b.mat[3][0];

    retVal.mat[0][1] =
            a.mat[0][0] * b.mat[0][1] +
            a.mat[0][1] * b.mat[1][1] +
            a.mat[0][2] * b.mat[2][1] +
            a.mat[0][3] * b.mat[3][1];

    retVal.mat[0][2] =
            a.mat[0][0] * b.mat[0][2] +
            a.mat[0][1] * b.mat[1][2] +
            a.mat[0][2] * b.mat[2][2] +
            a.mat[0][3] * b.mat[3][2];

    retVal.mat[0][3] =
            a.mat[0][0] * b.mat[0][3] +
            a.mat[0][1] * b.mat[1][3] +
            a.mat[0][2] * b.mat[2][3] +
            a.mat[0][3] * b.mat[3][3];

    // row 1
    retVal.mat[1][0] =
            a.mat[1][0] * b.mat[0][0] +
            a.mat[1][1] * b.mat[1][0] +
            a.mat[1][2] * b.mat[2][0] +
            a.mat[1][3] * b.mat[3][0];

    retVal.mat[1][1] =
            a.mat[1][0] * b.mat[0][1] +
            a.mat[1][1] * b.mat[1][1] +
            a.mat[1][2] * b.mat[2][1] +
            a.mat[1][3] * b.mat[3][1];

    retVal.mat[1][2] =
            a.mat[1][0] * b.mat[0][2] +
            a.mat[1][1] * b.mat[1][2] +
            a.mat[1][2] * b.mat[2][2] +
            a.mat[1][3] * b.mat[3][2];

    retVal.mat[1][3] =
            a.mat[1][0] * b.mat[0][3] +
            a.mat[1][1] * b.mat[1][3] +
            a.mat[1][2] * b.mat[2][3] +
            a.mat[1][3] * b.mat[3][3];

    // row 2
    retVal.mat[2][0] =
            a.mat[2][0] * b.mat[0][0] +
            a.mat[2][1] * b.mat[1][0] +
            a.mat[2][2] * b.mat[2][0] +
            a.mat[2][3] * b.mat[3][0];

    retVal.mat[2][1] =
            a.mat[2][0] * b.mat[0][1] +
            a.mat[2][1] * b.mat[1][1] +
            a.mat[2][2] * b.mat[2][1] +
            a.mat[2][3] * b.mat[3][1];

    retVal.mat[2][2] =
            a.mat[2][0] * b.mat[0][2] +
            a.mat[2][1] * b.mat[1][2] +
            a.mat[2][2] * b.mat[2][2] +
            a.mat[2][3] * b.mat[3][2];

    retVal.mat[2][3] =
            a.mat[2][0] * b.mat[0][3] +
            a.mat[2][1] * b.mat[1][3] +
            a.mat[2][2] * b.mat[2][3] +
            a.mat[2][3] * b.mat[3][3];

    // row 3
    retVal.mat[3][0] =
            a.mat[3][0] * b.mat[0][0] +
            a.mat[3][1] * b.mat[1][0] +
            a.mat[3][2] * b.mat[2][0] +
            a.mat[3][3] * b.mat[3][0];

    retVal.mat[3][1] =
            a.mat[3][0] * b.mat[0][1] +
            a.mat[3][1] * b.mat[1][1] +
            a.mat[3][2] * b.mat[2][1] +
            a.mat[3][3] * b.mat[3][1];

    retVal.mat[3][2] =
            a.mat[3][0] * b.mat[0][2] +
            a.mat[3][1] * b.mat[1][2] +
            a.mat[3][2] * b.mat[2][2] +
            a.mat[3][3] * b.mat[3][2];

    retVal.mat[3][3] =
            a.mat[3][0] * b.mat[0][3] +
            a.mat[3][1] * b.mat[1][3] +
            a.mat[3][2] * b.mat[2][3] +
            a.mat[3][3] * b.mat[3][3];

    return retVal;
}

Matrix4 &Matrix4::operator*=(const Matrix4 &right) {
    *this = *this * right;
    return *this;
}

void Matrix4::Invert() {
    // Thanks slow math
    // This is a really janky way to unroll everything...
    float tmp[12];
    float src[16];
    float dst[16];
    float det;

    // Transpose matrix
    // row 1 to col 1
    src[0] = mat[0][0];
    src[4] = mat[0][1];
    src[8] = mat[0][2];
    src[12] = mat[0][3];

    // row 2 to col 2
    src[1] = mat[1][0];
    src[5] = mat[1][1];
    src[9] = mat[1][2];
    src[13] = mat[1][3];

    // row 3 to col 3
    src[2] = mat[2][0];
    src[6] = mat[2][1];
    src[10] = mat[2][2];
    src[14] = mat[2][3];

    // row 4 to col 4
    src[3] = mat[3][0];
    src[7] = mat[3][1];
    src[11] = mat[3][2];
    src[15] = mat[3][3];

    // Calculate cofactors
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];

    dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
    dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];

    dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

    // Calculate determinant
    det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

    // Inverse of matrix is divided by determinant
    det = 1 / det;
    for (int j = 0; j < 16; j++) {
        dst[j] *= det;
    }

    // Set it back
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = dst[i * 4 + j];
        }
    }
}

Vector3 Matrix4::GetTranslation() const {
    return {mat[3][0], mat[3][1], mat[3][2]};
}

Vector3 Matrix4::GetXAxis() const {
    return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
}

Vector3 Matrix4::GetYAxis() const {
    return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
}

Vector3 Matrix4::GetZAxis() const {
    return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
}

Vector3 Matrix4::GetScale() const {
    Vector3 retVal;
    retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
    retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
    retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
    return retVal;
}

Matrix4 Matrix4::CreateScale(float xScale, float yScale, float zScale) {
    float temp[4][4] =
            {
                    {xScale, 0.0f,   0.0f,   0.0f},
                    {0.0f,   yScale, 0.0f,   0.0f},
                    {0.0f,   0.0f,   zScale, 0.0f},
                    {0.0f,   0.0f,   0.0f,   1.0f}
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateScale(const Vector3 &scaleVector) {
    return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Matrix4 Matrix4::CreateScale(float scale) {
    return CreateScale(scale, scale, scale);
}

Matrix4 Matrix4::CreateRotationX(float theta) {
    float temp[4][4] =
            {
                    {1.0f, 0.0f,              0.0f,             0.0f},
                    {0.0f, cosf(theta),  sinf(theta), 0.0f},
                    {0.0f, -sinf(theta), cosf(theta), 0.0f},
                    {0.0f, 0.0f,              0.0f,             1.0f},
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateRotationY(float theta) {
    float temp[4][4] =
            {
                    {cosf(theta), 0.0f, -sinf(theta), 0.0f},
                    {0.0f,             1.0f, 0.0f,              0.0f},
                    {sinf(theta), 0.0f, cosf(theta),  0.0f},
                    {0.0f,             0.0f, 0.0f,              1.0f},
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateRotationZ(float theta) {
    float temp[4][4] =
            {
                    {cosf(theta),  sinf(theta), 0.0f, 0.0f},
                    {-sinf(theta), cosf(theta), 0.0f, 0.0f},
                    {0.0f,              0.0f,             1.0f, 0.0f},
                    {0.0f,              0.0f,             0.0f, 1.0f},
            };
    return Matrix4(temp);
}


Matrix4 Matrix4::CreateFromQuaternion(const class Quaternion &q) {
    float mat[4][4];

    mat[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
    mat[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
    mat[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
    mat[0][3] = 0.0f;

    mat[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
    mat[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
    mat[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
    mat[1][3] = 0.0f;

    mat[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
    mat[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
    mat[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
    mat[2][3] = 0.0f;

    mat[3][0] = 0.0f;
    mat[3][1] = 0.0f;
    mat[3][2] = 0.0f;
    mat[3][3] = 1.0f;

    return Matrix4(mat);
}

Matrix4 Matrix4::CreateTranslation(const Vector3 &trans) {
    float temp[4][4] =
            {
                    {1.0f,    0.0f,    0.0f,    0.0f},
                    {0.0f,    1.0f,    0.0f,    0.0f},
                    {0.0f,    0.0f,    1.0f,    0.0f},
                    {trans.x, trans.y, trans.z, 1.0f}
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateLookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
    Vector3 zaxis = Vector3::Normalize(target - eye);
    Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
    Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
    Vector3 trans{};
    trans.x = -Vector3::Dot(xaxis, eye);
    trans.y = -Vector3::Dot(yaxis, eye);
    trans.z = -Vector3::Dot(zaxis, eye);

    float temp[4][4] =
            {
                    {xaxis.x, yaxis.x, zaxis.x, 0.0f},
                    {xaxis.y, yaxis.y, zaxis.y, 0.0f},
                    {xaxis.z, yaxis.z, zaxis.z, 0.0f},
                    {trans.x, trans.y, trans.z, 1.0f}
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateOrtho(float width, float height, float near, float far) {
    float temp[4][4] =
            {
                    {2.0f / width, 0.0f,          0.0f,                0.0f},
                    {0.0f,         2.0f / height, 0.0f,                0.0f},
                    {0.0f,         0.0f,          1.0f / (far - near), 0.0f},
                    {0.0f,         0.0f,          near / (near - far), 1.0f}
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreatePerspectiveFOV(float fovY, float width, float height, float near, float far) {
    float yScale = Math::Cot(fovY / 2.0f);
    float xScale = yScale * height / width;

    float temp[4][4] =
            {
                    {xScale, 0.0f,   0.0f,                       0.0f},
                    {0.0f,   yScale, 0.0f,                       0.0f},
                    {0.0f,   0.0f,   far / (far - near),         1.0f},
                    {0.0f,   0.0f,   -near * far / (far - near), 0.0f}
            };
    return Matrix4(temp);
}

Matrix4 Matrix4::CreateSimpleViewProj(float width, float height) {
    float temp[4][4] =
            {
                    {2.0f / width, 0.0f,          0.0f, 0.0f},
                    {0.0f,         2.0f / height, 0.0f, 0.0f},
                    {0.0f,         0.0f,          1.0f, 0.0f},
                    {0.0f,         0.0f,          1.0f, 1.0f}
            };
    return Matrix4(temp);
}

// Quaternion --------------------------------------------------------d

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);
