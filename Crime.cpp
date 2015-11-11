//
// Created by coco on 06/09/2015.
//

#include "Crime.h"

Crime::Crime(float x, float y){
	Coordinate* coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
}

Crime::Crime(Crime &crime){
	this->coordinate = new Coordinate(crime.coordinate->x,crime.coordinate->y);
	this->features[0]=crime.features[0];
	this->category=crime.category;
	this->date=crime.date;
	this->features[1]=crime.features[1];
	this->features[2]=crime.features[2];
}

void Crime::set_category(char *category){
	this->category = std::string(category);
}

void Crime::load_date(const tm &date) {
	this->date = date;
}

void Crime::load_day_of_week(char *dayOfWeek) {
	//this->day_of_week = std::string(day_of_week);
	this->features[0]=std::string(dayOfWeek);
}

void Crime::load_district(char *district) {
	//this->district = std::string(district);
	this->features[1]=std::string(district);
}

void Crime::load_adress(char *adress) {
	std::string slash ("/");
	std::string st ("ST");
	std::string av("AV");
	std::string dirs[3] = { slash, st, av };
	std::size_t found;

	for (int i = 0; i < 3; i++)
	{
		found = std::string(adress).find(dirs[i]);
		if (found != std::string::npos)
		{
			//this->adress = dirs[i];
			this->features[2]=dirs[i];
			return;
		}
	}
	this->features[2] = std::string(adress);
}

void Crime::set_coordinate(float x, float y) {
	Coordinate * coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
}
