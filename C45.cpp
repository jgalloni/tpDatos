#include "Crime.h"
#include "test_functions.h"
#include "gain.h"
#include "C45.h"
#include <tgmath.h>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

using namespace std;

typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

////AUX FUNCTIONS

std::string popular_crime(std::vector<Crime*>* crimes){
	std::map<const std::string, int> pop_crimes = *(new std::map<const std::string, int>());
	std::string current_crime;

	for(std::vector<Crime*>::size_type i = 0; i != (*crimes).size(); ++i){
		current_crime = (*crimes)[i]->category;
		if(pop_crimes.count(current_crime) == 0){
			pop_crimes[current_crime] = 1;
		} else {
			pop_crimes[current_crime] += 1;
		}
	}
	
	typedef std::map<const std::string, int>::iterator it_type;
	it_type iterator = pop_crimes.begin();
	std::string popular_crime = iterator->first;
	int popularity = iterator->second;
	for(; iterator != pop_crimes.end(); iterator++) {
		if((iterator->second) > popularity){
				popular_crime = iterator->first;
				popularity = iterator->second;
		}
	}
	
	return popular_crime;
}

//De existir, devuelve la clase que identifica el arbol. De lo contrario devuelve NULL
std::string class_of_tree(std::vector<Crime*>* crimes, int min_divisible){
	if((*crimes).size() == 0){
		return "LARCENY/THEFT";
	} else if (crimes->size() < (unsigned int) min_divisible){
		return popular_crime(crimes);
	}
	
	std::string t_class = (*crimes)[0]->category;
	for (unsigned i=1; i < (*crimes).size(); i++) {
	    if ((*crimes)[i]->category != t_class) return "";
	}

	//printf("Hoja alcanzada\n");

	return t_class;
}

////PREDICTION

Crime* make_prediction(C45 tree, Crime* crime){
	
	//posible optimizacion: anidar una funcion recursiva que busque solo el string
	if(!tree.tree_class.empty()){
		//crime->category = tree.tree_class;
		crime->set_prediction(tree.tree_class);
	} else {
		std::map<std::string, C45*> children = tree.children;
		int split_index = tree.split_index;
		std::string my_type = crime->features[split_index];
		//cout << my_type <<endl;
		if(children.count(my_type) == 0){
			printf("fui por other\n");
			make_prediction( *(children["other"]), crime); 
		} else {
			//printf("fui por clase\n");
			make_prediction( *(children[my_type]), crime);
		}
	}
	
	return crime;
}


////CLASS

C45::C45(std::vector<Crime*>* crimes, int max_hight, int min_divisible){
	tree_class = class_of_tree(crimes, min_divisible);
	
	//test function initialization
	test[0] = split_by_discrete_feature;
	
	//if(!tree_class.empty()){
	//	cout << "CLASS " << tree_class << " DEPTH " << max_hight << endl;
	//}
	
	children = *(new std::map<std::string, C45*>());
	//lo hago empezar con todas las features
	feature_indeces = new std::vector<int>{ 0, 1, 2 };
	
	//printf("Creando: altura %i, set de tamanio %i\n", max_hight, crimes->size());
		
	if (tree_class.empty() && max_hight > 0) {
		
		int best_index = (*feature_indeces)[0];
		float best_gain = gain_ratio(*crimes, best_index, test[0]); //despues probar con gain simple y ver cual da mejor cross validation
		std::map<const std::string, std::vector<Crime*>*> best_split = (test[0])(*crimes, best_index);
		
		for (unsigned int next = 1; next != feature_indeces->size() ; next++){
			float next_gain = gain_ratio(*crimes, (*feature_indeces)[next], test[0]);
			if (next_gain > best_gain){
			
				best_gain = next_gain;
				best_index = (*feature_indeces)[next];
				best_split = (test[0])(*crimes, best_index);
			
			} 
		}
		
		//If no entropy reducing tests available then,
		if(best_gain == 0){
			tree_class = popular_crime(crimes);
			return;
		}
		
		//cout << "RAMIFICATION DEPTH " << max_hight << " WITH " << std::to_string(best_split.size()) << " CHILDREN" << endl;
		
		split_index = best_index;
		for(it_type iterator = best_split.begin(); iterator != best_split.end(); iterator++){
				children[iterator->first] = new C45(iterator->second, max_hight - 1, min_divisible);
		}
		
		std::string pop_crime = popular_crime(crimes);
		children["other"] = new C45( new std::vector<Crime*>(), max_hight-1, min_divisible);
		children["other"]->tree_class = pop_crime;
	}
}

bool C45::is_leaf(){
	std::string t_class = this->tree_class;
	return !t_class.empty();
}

void C45::set_feature_indeces(std::vector<int>* indeces){
	this->feature_indeces = indeces;
}











