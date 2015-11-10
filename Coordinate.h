#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {

	public:
		Coordinate(float x, float y);
	
		float x;
		float y;
		
		float sqnorm(Coordinate* other); //Squared norm for distance comparison (taking the squared root is not needed)

};


#endif //COORDINATE_H
