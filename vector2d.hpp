#include <math.h>

class Vector2D {
	public:
		double x, y;

		Vector2D();
		Vector2D(double, double);

		Vector2D normalize ();
		Vector2D operator+(Vector2D const &a);
		Vector2D operator-(Vector2D const &a);
		Vector2D operator*(Vector2D const &a);
		Vector2D operator*(const double &a);

		double magnitude ();

};
