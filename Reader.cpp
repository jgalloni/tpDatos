//
// Created by coco on 23/08/2015.
//hola

#include "Reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

void check_quotemarks(char* line){
	int quotemarks = 0;
	for (int i = 0; i < 300; i++)
	{
		if (*(line+i) == '\"')
		{
			quotemarks++;
		}
		if (*(line+i) == ',' && quotemarks % 2 != 0)
		{
			*(line+i) = ' ';
		}
	}
}


std::vector<Crime*> readCsv(std::string fileName) {
	
    std::ifstream file_c;
    file_c.open(fileName);
    
//     char line[300];
//     char district[30],date[30],category[30],desc[30],day[30];
//     char resolution[30];
//     char adress[30];
	char line[400];
	char district[60], date[60], category[60], desc[100], day[60];
	char resolution[60];
	//char resolution2[60];
	char adress[60];
	//float x, y;
	char x[60];
	char y[60];
	
    std::vector<Crime*> crimes;
    
    if(!file_c.is_open()){
        std::cout<<"Error reading file:"<<fileName<<"\n";
        return crimes ;
    }
    
    std::cout<<"Reading file:"<<fileName<<"\n";
    
    Crime *crime;
    file_c.getline(line,400);
	file_c.getline(line, 400);
	
	while (!file_c.eof()) {
		check_quotemarks(line);
		//sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", date, category, desc, day, district, resolution, adress, &x, &y);
		//sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", date, category, desc, day, district, resolution,resolution2, adress,x,y);
		sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", date, category, desc, day, district, resolution, adress, x, y);
		crime = new Crime(atof(x),atof(y));
        crime->load_district(district);
        crime->load_adress(adress);
        crime->load_day_of_week(day);
        crime->set_category(category);
        tm tm1;
        sscanf(date,"%4d%2d%2d %2d%2d%2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,
              &tm1.tm_hour,&tm1.tm_min,&tm1.tm_sec);
        crime->load_date(tm1);
        crimes.push_back(crime);
		file_c.getline(line, 400);
    }
    
    std::cout<< crimes.size()<<" registers have been read."<<"\n";
    return crimes;
}
