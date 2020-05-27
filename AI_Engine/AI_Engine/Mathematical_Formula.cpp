#include "Mathematical_Formula.h"

void RotateY(float* X, float angle)
{
	float x1 = X[0], y1 = X[1], z1 = X[2];
	//fprintf(stderr, "YY%f %f %f ", x1, y1, z1);
	X[0] = cos(angle * PI / 180) * x1 + sin(angle * PI / 180) * z1;
	X[1] = y1;
	X[2] = -1 * sin(angle * PI / 180) * x1 + cos(angle * PI / 180) * z1;
	//fprintf(stderr, "YY%f %f %f ", X[0], X[1], X[2]);
}
void normalize(float* xyz)
{
	float w = sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
	xyz[0] /= w;
	xyz[1] /= w;
	xyz[2] /= w;
}
