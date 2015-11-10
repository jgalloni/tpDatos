//
// Created by coco on 06/09/2015.
//

#include "Crime.h"

Crime::Crime(float x, float y){
	Coordenada coordenada = new Coordenada(x,y);
	this->coordenada = coordenada;
}

void Crime::set_category(char *category) {
	this->category = std::string(category);
}

void Crime::load_date(const tm &date) {
	this->date = date;
}

void Crime::load_day_of_week(char *dayOfWeek) {
	this->day_of_week = std::string(day_of_week);
}

void Crime::load_district(char *district) {
	this->district = std::string(district);
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
			this->adress = dirs[i];
			return;
		}
	}
	this->adress = std::string(adress);
}

void Crime::set_coordinate(float x, float y) {
	Coordinate coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
}
