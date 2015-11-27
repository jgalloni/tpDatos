//
// Created by coco on 06/09/2015.
//

#ifndef TPDATOS_CRIMEN_H
#define TPDATOS_CRIMEN_H

#include <string>
#include <ctime>
#include <map>
#include "Coordinate.h"
#include <algorithm>

#define N_FEATURES 9

class Crime {

	public:
	
		Crime(float x, float y);

		Crime(Crime &crime);

		Coordinate * coordinate;
		struct tm date;
		std::string category;
		int id;
		int prediction_count;
		std::string features[N_FEATURES];
		
		void set_category(char *category);
		void load_date(const tm &date);
		void load_day_of_week(char *day_of_Week);
		void load_district(char *district);
		void load_address(char *address);
		void set_coordinate(float x, float y);
		
	private:
		std::string get_season();
		std::string get_hour();
		std::string ends_meet();
		void set_holiday();
		void set_moment_of_the_day();
		std::string get_month();
};


#endif //TPDATOS_CRIMEN_H
