//
// Created by coco on 23/08/2015.
//hola

#include "Reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

void check_comillas(char* linea){
	int i = 0;
	int comillas = 0;
	for (int i = 0; i < 300; i++)
	{
		if (*(linea+i) == '\"')
		{
			if (*(linea+i + 1) == '\"')
			{
				comillas++;
			}
		}
		if (*(linea+i) == ',' && comillas % 2 != 0)
		{
			*(linea+i) = ' ';
		}
	}
}


std::vector<Crimen> readCsv(std::string fileName) {
    std::ifstream archivo;
    archivo.open(fileName);
//     char linea[300];
//     char distrito[30],fecha[30],categoria[30],desc[30],dia[30];
//     char resolucion[30];
//     char direccion[30];
	char linea[400];
	char distrito[60], fecha[60], categoria[60], desc[100], dia[60];
	char resolucion[60];
	char direccion[60];
	//float x, y;
	char x[60];
	char y[60];
    std::vector<Crimen> crimenes;
    if(!archivo.is_open()){
        std::cout<<"error al abrir el archivo:"<<fileName<<"\n";
        return crimenes ;
    }
    std::cout<<"leyendo el archivo:"<<fileName<<"\n";
    Crimen crimen=Crimen();
    archivo.getline(linea,400);
	archivo.getline(linea, 400);
	while (!archivo.eof()) {
		check_comillas(linea);
		//sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", fecha, categoria, desc, dia, distrito, resolucion, direccion, &x, &y);
		sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", fecha, categoria, desc, dia, distrito, resolucion, direccion,x,y);
        crimen.setDistrito(distrito);
        crimen.setDireccion(direccion);
        crimen.setDiaDeLaSemana(dia);
        crimen.setDescripcion(desc);
        crimen.setCategoria(categoria);
        tm tm1;
        sscanf(fecha,"%4d%2d%2d %2d%2d%2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,
              &tm1.tm_hour,&tm1.tm_min,&tm1.tm_sec);
        crimen.setFecha(tm1);
        crimen.setResolucion(resolucion);
        crimenes.push_back(crimen);
		archivo.getline(linea, 400);
    }
    std::cout<<"se leyeron "<<crimenes.size()<<" registros"<<"\n";
    return crimenes;
}
