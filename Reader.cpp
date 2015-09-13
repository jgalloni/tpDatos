//
// Created by coco on 23/08/2015.
//

#include "Reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Crimen> readCsv(std::string fileName) {
    std::ifstream archivo;
    archivo.open(fileName);
    char linea[300];
    char distrito[30],fecha[30],categoria[30],desc[30],dia[30];
    char resolucion[30];
    char direccion[30];
    float x,y;
    std::vector<Crimen> crimenes;
    if(!archivo.is_open()){
        std::cout<<"error al abrir el archivo:"<<fileName<<"\n";
        return crimenes ;
    }
    std::cout<<"leyedo el archivo:"<<fileName<<"\n";
    Crimen crimen=Crimen();
    archivo.getline(linea,300);
    while(!archivo.eof()) {
        archivo.getline(linea,300);
        sscanf(linea,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",fecha,categoria,desc,dia,distrito,resolucion,direccion,&x,&y);
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
    }
    std::cout<<"se leyeron "<<crimenes.size()<<" registros"<<"\n";
    return crimenes;
}
