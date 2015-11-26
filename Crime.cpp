//
// Created by coco on 06/09/2015.
//

#include "Crime.h"
#include <iostream>
using namespace std;

Crime::Crime(float x, float y){
	Coordinate* coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
}

void Crime::set_category(char *category){
	this->category = std::string(category);
}

void Crime::load_date(const tm &date) {
	this->date = date;
	set_holiday();
	set_moment_of_the_day();
	this->features[5]=get_month();
}

std::string Crime::get_month(){
	//cout << "mes:" << date.tm_mon << endl;
	switch (date.tm_mon){
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";
	}
	return "";
}

void Crime::set_holiday(){
	switch (date.tm_mon) {
		case 0: {//january
			if (date.tm_mday == 0) {//1º (new year)
				this->features[3] = "holiday";
			}
			else if ((date.tm_wday == 1) && (date.tm_mday > 14) && (date.tm_mday < 22)) {//monday week 3 (M.L.K day)
				this->features[3] = "holiday";
			}
			else {
				this->features[3] = "work";
			}
			break;
			case 1: //febrero
				if ((date.tm_wday == 1) && (date.tm_mday > 14) && (date.tm_mday < 22)) {//monday week 3 (President day)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 5://May
				if ((date.tm_wday == 1) && (date.tm_mday > 24) && (date.tm_mday < 32)) {//monday week 3 (Memorial day)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 7: //july
				if ((date.tm_mday == 4)) {//4º (indep. day)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 9: //September
				if ((date.tm_wday == 1) && (date.tm_mday < 7)) {//monday week 1 (Laboral day)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 10: //October
				if ((date.tm_wday == 1) && (date.tm_mday > 7) && (date.tm_mday < 15)) {//monday week 2 (Columbus  day)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 11: //November
				if ((date.tm_mday == 11)) {//11 (Vet.  day)
					this->features[3] = "holiday";
				}
				else if ((date.tm_wday == 4) && (date.tm_mday > 20) &&
						 (date.tm_mday < 28)) {//thursday week 4 (Thanksgiving)
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			case 12://December
				if (date.tm_mday == 25) {//christmas
					this->features[3] = "holiday";
				}
				else {
					this->features[3] = "work";
				}
			break;
			default:
				this->features[3] = "work";
		}
	}
}

void Crime::set_moment_of_the_day() {
	if(date.tm_hour>2&&date.tm_hour<=6){
		this->features[4]="early morning";
	}
	else if(date.tm_hour>6&&date.tm_hour<=11){
		this->features[4]="morning";
	}
	else if(date.tm_hour>11&&date.tm_hour<=13){
		this->features[4]="noon";
	}
	else if(date.tm_hour>13&&date.tm_hour<=17){
		this->features[4]="afternoon";
	}
	else if(date.tm_hour>17&&date.tm_hour<=20){
		this->features[4]="evening";
	}
	else if(date.tm_hour>20&&date.tm_hour<=23){
		this->features[4]="night";
	}
	else if(date.tm_hour>23||date.tm_hour<=2){
		this->features[4]="midnight";
	}
}

void Crime::load_day_of_week(char *dayOfWeek) {
	//this->day_of_week = std::string(day_of_week);
	this->features[0]=std::string(dayOfWeek);
}

void Crime::load_district(char *district) {
	this->features[1]=std::string(district);
}

void Crime::load_address(char *address) {
	std::string slash ("/");
	std::string av("AV");
	std::string dirs[2] = { slash, av };
	std::size_t found;

	for (int i = 0; i < 2; i++)
	{
		found = std::string(address).find(dirs[i]);
		if (found != std::string::npos)
		{
			this->features[2]=dirs[i];
			return;
		}
	}

	this->features[2] = std::string("ST");
}

void Crime::set_coordinate(float x, float y) {
	Coordinate * coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
}

void Crime::set_prediction(std::string category){
	this->prediction->at(category)+=1;
	prediction_count++;
}


bool pred(const std::pair<std::string, int>& lhs,const std::pair<std::string, int>& rhs)
{
	return lhs.second < rhs.second;
}

std::string Crime::get_prediction() {
	return std::max_element((*this->prediction).begin(), (*this->prediction).end(), pred)->first;
}
