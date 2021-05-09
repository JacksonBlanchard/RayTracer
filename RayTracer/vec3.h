#pragma once
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp> // SFML
using namespace std;

class vec3
{
private:
	sf::Vector3f vec;

public:
	vec3() : vec(0, 0, 0) {};
	vec3(double v0, double v1, double v2) : vec(v0, v1, v2) {};
	vec3(sf::Vector3f _vec) : vec(_vec) {};

	double x() const { return vec.x; }
	double y() const { return vec.y; }
	double z() const { return vec.z; }

	vec3 operator-() const { return vec3(-vec); }

	double operator[](int i) const { 
		if (i == 0) return x();
		if (i == 1) return y();
		if (i == 2) return z();
	}
	
	vec3& operator+=(const vec3 &_v) {
		vec += _v.vec;
		return *this;
	}

	vec3& operator-=(const vec3 &_v) {
		vec -= _v.vec;
		return *this;
	}

	vec3& operator*=(float t) {
		vec *= t;
		return *this;
	}

	vec3& operator/=(float t) {
		vec /= t;
		return *this;
	}

	double length() const {
		return sqrt(length_squared());
	}

	double length_squared() const {
		return (x() * x()) + (y() * y()) + (z() * z());
	}
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v[0] << " " << v[1] << " " << v[2];
}

inline bool operator==(vec3 u, vec3 v) {
	return (u[0] == v[0]) && (u[1] == v[1]) && (u[2] == v[2]);
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v[0], t * v[1], t * v[2]);
}
// commutative multiplication
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return v * (1/t);
}

inline double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0]
		 + u[1] * v[1]
		 + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u[1] * v[2] - u[2] * v[1],
				u[2] * v[0] - u[0] * v[2],
				u[0] * v[1] - u[1] * v[0]);
}

// normalized (obviously)
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

// normalized
inline vec3 reflectView(vec3 view, vec3 normal) {
	return unit_vector((2 * dot(view, normal) * normal) - view);
}

// normalized
inline vec3 refractView(vec3 view, vec3 normal, double eta) {
	// must previously check for total internal reflection
	double cos_i = dot(view, normal);
	double sin_i2 = 1 - (cos_i*cos_i);

	if (eta * sqrt(sin_i2) > 1.0) {
		return reflectView(view, normal);
	}

	vec3 r_perp = eta * (view - (cos_i * normal));
	vec3 r_para = normal * sqrt(1 - (eta*eta * sin_i2));

	return -unit_vector(r_perp + r_para);
}

// Type aliases for vec3
using pos3 = vec3;		// 3D point
using color3 = vec3;	// RGB color

