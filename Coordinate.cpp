#include "Coordinate.h"
#include <string>
#include <iostream>
#include <sstream>
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

std::string Coordinate::as_string(){
	std::ostringstream string_stream;
	string_stream << "(" << x << "," << y << ")";
	std::string copy = string_stream.str();
	return copy;
}
