//
// Created by coco on 23/08/2015.
//hola

#include "Reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

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
    
	char line[400] = {0};
	char district[60], date[60], category[60], desc[100], day[60];
	char resolution[60];
	//char resolution2[60];
	char address[60];
	//float x, y;
	char x[60];
	char y[60];

	double maximoX=0;
	double maximoY=0;
	double minimoX=0;
	double minimoY=0;
	
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
		sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", date, category, desc, day, district, resolution, address, x, y);
		if(atof(y)>50){ // evita los datos fuera de san francisco
			file_c.getline(line, 400);
			continue;
		}
		crime = new Crime(atof(x),atof(y));
        crime->load_district(district);
        crime->load_address(address);
        crime->load_day_of_week(day);
        crime->set_category(category);
        tm tm1;
        sscanf(date,"%4d - %2d - %2d %2d : %2d : %2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,
              &tm1.tm_hour,&tm1.tm_min,&tm1.tm_sec);
        //cout << "year: " << tm1.tm_year << " month: " << tm1.tm_mon << " hour: " << tm1.tm_hour << " min: " << tm1.tm_min << " sec: " << tm1.tm_sec <<endl;
        crime->load_date(tm1);
        crimes.push_back(crime);
		file_c.getline(line, 400);


		if(maximoX==0||maximoX<atof(x))
			maximoX=atof(x);

		if(maximoY==0||maximoY<atof(y))
			maximoY=atof(y);

		if(minimoX==0||minimoX>atof(x))
			minimoX=atof(x);

		if(minimoY==0||minimoY>atof(y))
			minimoY=atof(y);
    }
    file_c.close();
    cout<< crimes.size()<<" registers have been read."<< endl;

	/*std::cout<< "borde inferrio izquierdo: " << minimoX<<","<<minimoY<<"\n";
	std::cout<< "borde inferrio derecho: " << maximoX<<","<<minimoY<<"\n";
	std::cout<< "borde superior izquierdo: " << minimoX<<","<<maximoY<<"\n";
	std::cout<< "borde superior derecho: " << maximoX<<","<<maximoY<<"\n";
	std::cout<<"x medio: "<<(maximoX+minimoX)/2<<"\n";
	std::cout<<"y medio: "<<(maximoY+minimoY)/2<<"\n";
	std::cout<<"========================="<<"\n";*/

    return crimes;
}

std::vector<Crime*> readCsv2(std::string fileName) {//lee el archivo a predecir
	std::ifstream file_c;
	file_c.open(fileName);
	std::cout<<"Reading file:"<<fileName<<"\n";
	char line[400];
	char district[60], date[60], day[60],id[60];
	char address[60];
	char x[60];
	char y[60];

	Crime *crime;
	file_c.getline(line,400);
	file_c.getline(line, 400);

	std::vector<Crime*> crimes;
	if(!file_c.is_open()){
		std::cout<<"Error reading file:"<<fileName<<"\n";
		return crimes ;
	}

	while (!file_c.eof()) {
		sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", id, date, day, district, address, x, y);

		crime = new Crime(atof(x),atof(y));
		crime->load_district(district);
		crime->load_address(address);
		crime->load_day_of_week(day);
		crime->id=atoi(id);
		tm tm1;
		sscanf(date,"%4d%2d%2d %2d%2d%2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,
			   &tm1.tm_hour,&tm1.tm_min,&tm1.tm_sec);
		crime->load_date(tm1);
		crimes.push_back(crime);
		file_c.getline(line, 400);

	}
	file_c.close();
	std::cout<< crimes.size()<<" registers have been read."<<"\n";

	return crimes;
}
