#include "Coordinate.h"
#include <cmath>
#include <stdlib.h>
#include <stddef.h>

Coordinate::Coordinate(float x, float y){
	this->x = x;
	this->y = y;
}

//Squared norm for distance comparison (taking the squared root is not needed)
float Coordinate::sqnorm(Coordinate* other){
	return (pow(((this->x) - (other->x)), 2) + pow(((this->y) - (other->y)), 2));
}
