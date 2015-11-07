#include "Crimen.h"
#include "C45.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

//AUX

//De existir, devuelve la clase que identifica el arbol. De lo contrario devuelve NULL
std::string class_of_tree(std::vector<Crimen>* crimes){
	if((*crimes).size() == 0){
		return "LARCENY/THEFT";	
	}
	
	std::string t_class = (*crimes)[0].getCategoria();
	for (unsigned i=1; i < (*crimes).size(); i++) {
	    if ((*crimes)[i].getCategoria() != t_class) return "";
	}

	return t_class;
}

//CLASS

C45::C45(std::vector<Crimen>* crimes){
	tree_class = class_of_tree(crimes);
	children = new std::vector<C45*>();
}

bool C45::is_leaf(){
	std::string t_class = this->tree_class;
	return !t_class.empty();
}










