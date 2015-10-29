//
// Created by coco on 06/09/2015.
//

#ifndef TPDATOS_CRIMEN_H
#define TPDATOS_CRIMEN_H


#include <string>
#include <ctime>

class Crimen {
private:
    struct tm fecha;
    std::string categoria;
    std::string descripcion;
    std::string diaDeLaSemana;
    std::string distrito;
    std::string resolucion;
    std::string direccion;
    float x;
    float y;


public:
    tm getFecha() const {
        return fecha;
    }

    void setFecha(const tm &fecha) {
        Crimen::fecha = fecha;
    }


    const std::string &getCategoria() const {
        return categoria;
    }

    void setCategoria(char *categoria) {
        Crimen::categoria = std::string(categoria);
    }

    const std::string &getDescripcion() const {
        return descripcion;
    }

    void setDescripcion(char *descripcion) {
        Crimen::descripcion = std::string(descripcion);
    }

    const std::string &getDiaDeLaSemana() const {
        return diaDeLaSemana;
    }

    void setDiaDeLaSemana(char *diaDeLaSemana) {
        Crimen::diaDeLaSemana = std::string(diaDeLaSemana);
    }

    const std::string &getDistrito() const {
        return distrito;
    }

    void setDistrito(char *distrito) {
        Crimen::distrito = std::string(distrito);
    }

    const std::string &getResolucion() const {
        return resolucion;
    }

    void setResolucion(char *resolucion) {
        Crimen::resolucion = std::string(resolucion);
    }

    const std::string &getDireccion() const {
        return direccion;
    }

    void setDireccion(char *direccion) {
		std::string slash ("/");
		std::string st ("ST");
		std::string av("AV");
		std::string dirs[3] = { slash, st, av };
		std::size_t found;

		for (int i = 0; i < 3; i++)
		{
			found = std::string(direccion).find(dirs[i]);
			if (found != std::string::npos)
			{
				Crimen::direccion = dirs[i];
				return;
			}
		}
        Crimen::direccion = std::string(direccion);
    }

    float getX() const {
        return x;
    }

    void setX(float x) {
        Crimen::x = x;
    }

    float getY() const {
        return y;
    }

    void setY(float y) {
        Crimen::y = y;
    }
};


#endif //TPDATOS_CRIMEN_H
