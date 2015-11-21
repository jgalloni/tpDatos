#ifndef COORDINATE_H
#define COORDINATE_H

#include <string>

class Coordinate {

	public:
		Coordinate(float x, float y);
	
		float x;
		float y;
		
		float sqnorm(Coordinate* other); //Squared norm for distance comparison (taking the squared root is not needed)
		std::string as_string();
};


#endif //COORDINATE_H
