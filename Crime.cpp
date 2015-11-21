//
// Created by coco on 06/09/2015.
//

#include "Crime.h"

Crime::Crime(float x, float y){
	Coordinate* coordinate = new Coordinate(x,y);
	this->coordinate = coordinate;
// 	this->prediction=new std::map<std::string, int>();
// 	(*this->prediction)["ARSON"]=0;
//  	(*this->prediction)["ASSAULT"]=0;
// 	(*this->prediction)["BAD CHECKS"]=0;
// 	(*this->prediction)["BRIBERY"]=0;
// 	(*this->prediction)["BURGLARY"]=0;
// 	(*this->prediction)["DISORDERLY "]=0;
// 	(*this->prediction)["CONDUCT"]=0;
// 	(*this->prediction)["DRIVING UNDER THE INFLUENCE"]=0;
// 	(*this->prediction)["DRUG/NARCOTIC"]=0;
// 	(*this->prediction)["DRUNKENNESS"]=0;
// 	(*this->prediction)["EMBEZZLEMENT"]=0;
// 	(*this->prediction)["EXTORTION"]=0;
// 	(*this->prediction)["FAMILY OFFENSES"]=0;
// 	(*this->prediction)["FORGERY/COUNTERFEITING"]=0;
// 	(*this->prediction)["FRAUD"]=0;
// 	(*this->prediction)["GAMBLING"]=0;
// 	(*this->prediction)["KIDNAPPING"]=0;
// 	(*this->prediction)["LARCENY/THEFT"]=0;
// 	(*this->prediction)["LIQUOR LAWS"]=0;
// 	(*this->prediction)["LOITERING"]=0;
// 	(*this->prediction)["MISSING PERSON"]=0;
// 	(*this->prediction)["NON-CRIMINAL"]=0;
// 	(*this->prediction)["OTHER OFFENSES"]=0;
// 	(*this->prediction)["PORNOGRAPHY/OBSCENE MAT"]=0;
// 	(*this->prediction)["PROSTITUTION"]=0;
// 	(*this->prediction)["RECOVERED VEHICLE"]=0;
// 	(*this->prediction)["ROBBERY"]=0;
// 	(*this->prediction)["RUNAWAY"]=0;
// 	(*this->prediction)["SECONDARY CODES"]=0;
// 	(*this->prediction)["SEX OFFENSES FORCIBLE"]=0;
// 	(*this->prediction)["SEX OFFENSES NON FORCIBLE"]=0;
// 	(*this->prediction)["STOLEN PROPERTY"]=0;
// 	(*this->prediction)["SUICIDE"]=0;
// 	(*this->prediction)["SUSPICIOUS OCC"]=0;
// 	(*this->prediction)["TREA"]=0;
// 	(*this->prediction)["TRESPASS"]=0;
// 	(*this->prediction)["VANDALISM"]=0;
// 	(*this->prediction)["VEHICLE THEFT"]=0;
// 	(*this->prediction)["WARRANTS"]=0;
// 	(*this->prediction)["WEAPON LAWS"]=0;
}

Crime::Crime(Crime &crime){
	this->coordinate = new Coordinate(crime.coordinate->x,crime.coordinate->y);
	this->features[0]=crime.features[0];
	this->category=crime.category;
	this->date=crime.date;
	this->features[1]=crime.features[1];
	this->features[2]=crime.features[2];
	this->features[3]=crime.features[3];
	//this->features[4]=crime.features[4];
	//this->features[5]=crime.features[5];
	//this->prediction_count=crime.prediction_count;
	//this->prediction=new std::map<std::string, int>(*(crime.prediction));
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
	//this->district = std::string(district);
	this->features[1]=std::string(district);
}

void Crime::load_adress(char *adress) {
	std::string slash ("/");
	std::string av("AV");
	std::string dirs[2] = { slash, av };
	std::size_t found;

	for (int i = 0; i < 2; i++)
	{
		found = std::string(adress).find(dirs[i]);
		if (found != std::string::npos)
		{
			//this->adress = dirs[i];
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

std::string Crime::to_csv() {
	return std::to_string(this->id).append(",").append(
	std::to_string( (*this->prediction)["ARSON"]/this->prediction_count)).append(",").append(
			std::to_string( (*this->prediction)["ASSAULT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["BAD CHECKS"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["BRIBERY"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["BURGLARY"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["DISORDERLY "]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["CONDUCT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["DRIVING UNDER THE INFLUENCE"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["DRUG/NARCOTIC"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["DRUNKENNESS"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["EMBEZZLEMENT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["EXTORTION"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["FAMILY OFFENSES"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["FORGERY/COUNTERFEITING"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["FRAUD"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["GAMBLING"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["KIDNAPPING"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["LARCENY/THEFT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["LIQUOR LAWS"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["LOITERING"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["MISSING PERSON"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["NON-CRIMINAL"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["OTHER OFFENSES"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["PORNOGRAPHY/OBSCENE MAT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["PROSTITUTION"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["RECOVERED VEHICLE"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["ROBBERY"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["RUNAWAY"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["SECONDARY CODES"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["SEX OFFENSES FORCIBLE"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["SEX OFFENSES NON FORCIBLE"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["STOLEN PROPERTY"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["SUICIDE"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["SUSPICIOUS OCC"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["TREA"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["TRESPASS"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["VANDALISM"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["VEHICLE THEFT"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["WARRANTS"]/this->prediction_count)).append(",").append(
	std::to_string( (*this->prediction)["WEAPON LAWS"]/this->prediction_count)).append("\n");
	}



bool pred(const std::pair<std::string, int>& lhs,const std::pair<std::string, int>& rhs)
{
	return lhs.second < rhs.second;
}

std::string Crime::get_prediction() {
	return std::max_element((*this->prediction).begin(), (*this->prediction).end(), pred)->first;
}
