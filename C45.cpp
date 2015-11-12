#include "Crime.h"
#include "C45.h"
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

////AUX FUNCTIONS

//De existir, devuelve la clase que identifica el arbol. De lo contrario devuelve NULL
std::string class_of_tree(std::vector<Crime*>* crimes){
	if((*crimes).size() == 0){
		return "LARCENY/THEFT";	
	}
	
	std::string t_class = (*crimes)[0]->category;
	for (unsigned i=1; i < (*crimes).size(); i++) {
	    if ((*crimes)[i]->category != t_class) return "";
	}

	return t_class;
}

std::map<const std::string, std::vector<Crime*>*> split_by_discrete_feature(std::vector<Crime*> set, int feature_index){
	//ay caramba
	std::map<const std::string, std::vector<Crime*>*>* subsets = new std::map<const std::string, std::vector<Crime*>*>();
	Crime* crime;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		crime = set[i];
		std::string category = crime->features[feature_index];
		
		if(subsets->count(category) == 0) {
				(*subsets)[category] = new std::vector<Crime*>();
		}
		
		(*subsets)[category]->push_back(crime);
	}
	
	return *subsets;
}

int subsets_by_feature(std::vector<Crime*> set, int feature_index){
	std::map<const std::string, std::vector<Crime*>*> subsets = split_by_discrete_feature(set, feature_index);
	return subsets.size();
}

////TEST FUNCTIONS
// DISCRETE TEST FUNCTIONS


// CONTINUOUS TEST FUNCTIONS


////CLASS

C45::C45(std::vector<Crime*>* crimes){
	tree_class = class_of_tree(crimes);
	children = new std::vector<C45*>();
}

bool C45::is_leaf(){
	std::string t_class = this->tree_class;
	return !t_class.empty();
}










