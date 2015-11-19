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

class Crime {

	public:
	
		Crime(float x, float y);

		Crime(Crime &crime);

		Coordinate * coordinate;
		struct tm date;
		std::string category;
		int id;
		int prediction_count;
		std::map<std::string, int> *prediction;
		//std::string day_of_week;
		//std::string district;
		//std::string adress;
		std::string features[6];//day,district,address
		void set_category(char *category);
		void load_date(const tm &date);
		void load_day_of_week(char *day_of_Week);
		void load_district(char *district);
		void load_adress(char *adress);
		void set_coordinate(float x, float y);
		std::string to_csv();
		void set_prediction(std::string category);
		std::string get_prediction();
	private:
		void set_holiday();
		void set_moment_of_the_day();
		std::string get_month();
};


#endif //TPDATOS_CRIMEN_H
