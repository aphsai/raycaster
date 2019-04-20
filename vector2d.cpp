#include "vector2d.hpp"

Vector2D::Vector2D() {
	x = 0, y = 0;
}
Vector2D::Vector2D(double x_, double y_) {
	x = x_, y = y_;
}

Vector2D Vector2D::operator+(Vector2D const &a) {
	return Vector2D { x + a.x, y + a.y };
}

Vector2D Vector2D::operator-(Vector2D const &a) {
	return Vector2D { x - a.x, y - a.y };
}

Vector2D Vector2D::operator*(Vector2D const &a) {
	return Vector2D { x * a.x, y * a.y };
}

Vector2D Vector2D::operator*(const double &a) {
	return Vector2D { x * a, y * a };
}
