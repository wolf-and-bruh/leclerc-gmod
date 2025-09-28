#pragma once

#include <cmath>
#include <algorithm>

constexpr double M_PI = 3.14159265358979323846;
constexpr float M_PI_F = 3.14159265358979323846f;

constexpr float deg2rad(float deg) {
	return deg * M_PI_F / 180.f;
}

constexpr float rad2deg(float rad) {
	return rad * 180.f / M_PI_F;
}

constexpr float get_delta(float hspeed, float maxspeed, float airaccelerate)
{
	auto term = (30.0 - (airaccelerate * maxspeed / 66.0)) / hspeed;

	if (term < 1.0f && term > -1.0f) {
		return acos(term);
	}

	return 0.f;
}

constexpr float normalize_yaw(float yaw) noexcept {
	// bring into [-180, 180]
	while (yaw > 180.f) yaw -= 360.f;
	while (yaw < -180.f) yaw += 360.f;
	return yaw;
}

class Vector {
public:
	float x{}, y{}, z{};

	Vector() : x(0.f), y(0.f), z(0.f) {}
	Vector(const Vector& other) : x(other.x), y(other.y), z(other.z) {}
	Vector(Vector&& other) : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)) {}
	Vector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

	Vector& operator=(const Vector& other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	Vector&& operator+(const Vector& other) const {
		return Vector(x + other.x, y + other.y, z + other.z);
	}

	Vector&& operator-(const Vector& other) const {
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector&& operator*(const Vector& other) const {
		return Vector(x * other.x, y * other.y, z * other.z);
	}

	Vector&& operator/(const Vector& other) const {
		return Vector(x / other.x, y / other.y, z / other.z);
	}

	Vector&& operator*(float num) const {
		return Vector(x * num, y * num, z * num);
	}

	Vector&& operator/(float num) const {
		return Vector(x / num, y / num, z / num);
	}

	Vector& operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector& operator*=(const Vector& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector& operator/=(const Vector& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	Vector& operator*=(float num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	Vector& operator/=(float num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}

	float Length2D() const {
		return std::sqrtf(x * x + y * y);
	}

	float LengthSqr() const {
		return x * x + y * y + z * z;
	}

	float Length() const {
		return std::sqrtf(LengthSqr());
	}

	Vector Cross(const Vector& other) const {
		return Vector(y * other.z - z * other.y, x * other.z - z * other.x, x * other.y - y * other.x);
	}

	float Dot(const Vector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float Normalize() {
		float length = Length();
		x /= length + 1e-6f;
		y /= length + 1e-6f;
		z /= length + 1e-6f;
		return length;
	}

	Vector Scale(float factor) const noexcept
	{
		return { x * factor, y * factor, z * factor };
	}
};

class __declspec(align(16)) VectorAligned : public Vector {
public:
	using Vector::Vector;

	VectorAligned(const Vector& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
};

class Angle {
public:
	float p, y, r;

	Angle() : p(0.f), y(0.f), r(0.f) {}
	Angle(const Angle& other) : p(other.p), y(other.y), r(other.r) {}
	Angle(Angle&& other) : p(std::move(other.p)), y(std::move(other.y)), r(std::move(other.r)) {}
	Angle(float p_, float y_, float r_) : p(p_), y(y_), r(r_) {}

	Angle& operator=(const Angle& other) {
		p = other.p;
		y = other.y;
		r = other.r;

		return *this;
	}

	Angle&& operator+(const Angle& other) const {
		return Angle(p + other.p, y + other.y, r + other.r);
	}

	Angle&& operator-(const Angle& other) const {
		return Angle(p - other.p, y - other.y, r - other.r);
	}

	Angle&& operator*(const Angle& other) const {
		return Angle(p * other.p, y * other.y, r * other.r);
	}

	Angle&& operator/(const Angle& other) const {
		return Angle(p / other.p, y / other.p, r / other.r);
	}

	Angle&& operator*(float num) const {
		return Angle(p * num, y * num, r * num);
	}

	Angle&& operator/(float num) const {
		return Angle(p / num, y / num, r / num);
	}

	Angle& operator+=(const Angle& other) {
		p += other.p;
		y += other.y;
		r += other.r;

		return *this;
	}

	Angle& operator-=(const Angle& other) {
		p -= other.p;
		y -= other.y;
		r -= other.r;

		return *this;
	}

	Angle& operator*=(const Angle& other) {
		p *= other.p;
		y *= other.y;
		r *= other.r;

		return *this;
	}

	Angle& operator/=(const Angle& other) {
		p /= other.p;
		y /= other.y;
		r /= other.r;

		return *this;
	}

	Angle& operator/=(float num) {
		p /= num;
		y /= num;
		r /= num;

		return *this;
	}

	Angle& operator*=(float num) {
		p *= num;
		y *= num;
		r *= num;

		return *this;
	}

	void Normalize() noexcept {
		p = std::clamp(p, -89.f, 89.f);
		y = Normalize180(y);
		r = 0.f;
	}

	static float Normalize180(float ang) noexcept {
		ang = fmodf(ang + 180, 360);

		if (ang < 0)
			ang += 360;

		return ang - 180;
	}

	static Angle FromVector(const Vector& vec) {
		Angle ang;

		if (vec.y == 0.f && vec.x == 0.f) {
			ang.y = 0;

			if (vec.z > 0)
				ang.p = 270;
			else
				ang.p = 90;
		}
		else {
			ang.y = (std::atan2f(vec.y, vec.x) * 180.f / M_PI_F);
			if (ang.y < 0)
				ang.y += 360;

			ang.p = (std::atan2f(-vec.z, vec.Length2D()) * 180.f / M_PI_F);
			if (ang.p < 0)
				ang.p += 360;
		}

		return ang;
	}

	Vector Forward() const {
		float radp = deg2rad(p);
		float rady = deg2rad(y);

		float sp = std::sinf(radp), cp = std::cosf(radp);
		float sy = std::sinf(rady), cy = std::cosf(rady);

		return Vector(cp * cy, cp * sy, -sp);
	}

	Vector Right() const {
		float radp = deg2rad(p);
		float rady = deg2rad(y);
		float radr = deg2rad(r);

		float sp = std::sinf(radp), cp = std::cosf(radp);
		float sy = std::sinf(rady), cy = std::cosf(rady);
		float sr = std::sinf(radr), cr = std::cosf(radr);

		return Vector(-1.f * sr * sp * cy + -1.f * cr * -sy,
			-1.f * sr * sp * sy + -1.f * cr * cy,
			-1.f * sr * cp);
	}

	Vector Up() const {
		float radp = deg2rad(p);
		float rady = deg2rad(y);
		float radr = deg2rad(r);

		float sp = std::sinf(radp), cp = std::cosf(radp);
		float sy = std::sinf(rady), cy = std::cosf(rady);
		float sr = std::sinf(radr), cr = std::cosf(radr);

		return Vector(cr * sp * cy + -sr * -sy,
			cr * sp * sy + -sr * cy,
			cr * cp);
	}

	void Vectors(Vector& outForward, Vector& outRight, Vector& outUp) const {
		outForward = Forward();
		outRight = Right();
		outUp = Up();
	}

	void FixAngles() {
		// normalize yaw
		while (y > 180.f)  y -= 360.f;
		while (y < -180.f) y += 360.f;

		// clamp pitch
		if (p > 89.f)  p = 89.f;
		if (p < -89.f) p = -89.f;

		// reset roll
		r = 0.f;
	}
};

class Matrix3x4
{
public:
	Matrix3x4() {}
	Matrix3x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		flMatVal[0][0] = m00;	flMatVal[0][1] = m01; flMatVal[0][2] = m02; flMatVal[0][3] = m03;
		flMatVal[1][0] = m10;	flMatVal[1][1] = m11; flMatVal[1][2] = m12; flMatVal[1][3] = m13;
		flMatVal[2][0] = m20;	flMatVal[2][1] = m21; flMatVal[2][2] = m22; flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		flMatVal[0][0] = xAxis.x; flMatVal[0][1] = yAxis.x; flMatVal[0][2] = zAxis.x; flMatVal[0][3] = vecOrigin.x;
		flMatVal[1][0] = xAxis.y; flMatVal[1][1] = yAxis.y; flMatVal[1][2] = zAxis.y; flMatVal[1][3] = vecOrigin.y;
		flMatVal[2][0] = xAxis.z; flMatVal[2][1] = yAxis.z; flMatVal[2][2] = zAxis.z; flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	Matrix3x4(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector const& p)
	{
		flMatVal[0][3] = p.x;
		flMatVal[1][3] = p.y;
		flMatVal[2][3] = p.z;
	}

	inline Vector GetOrigin()
	{
		return Vector{ flMatVal[0][3],flMatVal[1][3],flMatVal[2][3] };
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}

	float* operator[](int i) { return flMatVal[i]; }
	const float* operator[](int i) const { return flMatVal[i]; }
	float* Base() { return &flMatVal[0][0]; }
	const float* Base() const { return &flMatVal[0][0]; }

	float flMatVal[3][4];
};