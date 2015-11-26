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

#define DISCRETE_TESTS 2
#define LOCATION_TESTS 3

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

std::string find_type(C45 tree, Crime* crime, int feature_index){
	std::vector<Crime*>* input = new std::vector<Crime*>();
	input->push_back(crime);
	std::map<const std::string, std::vector<Crime*>*> hashed = (tree.best_test)( *input, feature_index);
	it_type iterator = hashed.begin();
	return (iterator->first);
	
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

	return t_class;
}

////PREDICTION

Crime* make_prediction(C45 tree, Crime* crime){
	
	//posible optimizacion: anidar una funcion recursiva que busque solo el string
	if(!tree.tree_class.empty()){
		crime->category = tree.tree_class;
		//crime->set_prediction(tree.tree_class);
	} else {
		std::map<std::string, C45*> children = tree.children;
		int split_index = tree.split_index;
		std::string my_type = find_type(tree, crime, split_index);
		if(children.count(my_type) == 0){
			make_prediction( *(children["other"]), crime); 
		} else {
			make_prediction( *(children[my_type]), crime);
		}
	}
		
	return crime;
}


////CLASS

C45::C45(std::vector<Crime*>* crimes, int max_hight, int min_divisible, bool location_assigned){
	tree_class = class_of_tree(crimes, min_divisible);
		
	//test function initialization
	discrete_test[0] = split_by_discrete_feature;
	discrete_test[1] = split_biggest_set;
	location_test[0] = split_in_quadrants;
	location_test[1] = split_in_3_clusters;
	location_test[2] = split_in_4_clusters;
	
	children = *(new std::map<std::string, C45*>());
	//lo hago empezar con todas las features
	feature_indeces = new std::vector<int>{ 0, 1, 2 };
	
	//Search of best split
	if (tree_class.empty() && max_hight > 0) {
		
		//Search by descrete feature
		best_test = discrete_test[0];
		int best_index = (*feature_indeces)[0];
		float best_gain = gain_ratio(*crimes, discrete_test[0], best_index);
		float next_gain;
		std::map<const std::string, std::vector<Crime*>*> best_split = (discrete_test[0])(*crimes, best_index);
		
		for (int i = 0; i != DISCRETE_TESTS; i++){
			//usa la funcion dos veces por vuelta. Se podra optimizar?
			for (unsigned int next = 0; next != feature_indeces->size() ; next++){
				if(i == 0 && next == 0) continue;
				next_gain = gain_ratio(*crimes, discrete_test[i], (*feature_indeces)[next]);
				if (next_gain > best_gain){
				
					best_gain = next_gain;
					best_index = (*feature_indeces)[next];
					best_split = (discrete_test[i])(*crimes, best_index);
					best_test = discrete_test[i];
				
				} 
			}
		}
		
		
		
		//Search by location
		
		bool location_branch = location_assigned;
		if(location_assigned == false){
			for(int i = 0; i != LOCATION_TESTS; i++){
				next_gain = gain_ratio(*crimes, location_test[i]);
				if (next_gain > best_gain){
						best_gain = next_gain;
						best_split = (location_test[i])(*crimes, 0);
						best_test = location_test[i];
						best_index = -1;
						location_branch = true;
						
				} 
			}
		}
		
		//If no entropy reducing tests available then,
		if(best_gain == 0){
			tree_class = popular_crime(crimes);
			return;
		}
				
		//Children spawning
		split_index = best_index;
		for(it_type iterator = best_split.begin(); iterator != best_split.end(); iterator++){
				children[iterator->first] = new C45(iterator->second, max_hight - 1, min_divisible, location_branch);
		}
		
		//Criteria: if instance type was not available in the train set, then the prediction is the most common category at this point.
		std::string pop_crime = popular_crime(crimes);
		children["other"] = new C45( new std::vector<Crime*>(), max_hight-1, min_divisible);
		children["other"]->tree_class = pop_crime;
		
	}
	
	//al menos le hace delete a los crimenes
	crimes->clear();
}

bool C45::is_leaf(){
	std::string t_class = this->tree_class;
	return !t_class.empty();
}

void C45::set_feature_indeces(std::vector<int>* indeces){
	this->feature_indeces = indeces;
}











