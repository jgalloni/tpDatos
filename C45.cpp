#include "Crime.h"
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
		return "LARCENY/THEFTY"; //este typo es intencional, para diferenciar de las predicciones de robo que no vienen de aca
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

//SET OPERATIONS

std::map<const std::string, std::vector<Crime*>*> split_by_discrete_feature(std::vector<Crime*> set, int feature_index){
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

std::map<const std::string, std::vector<Crime*>*> split_by_category(std::vector<Crime*> set){
	std::map<const std::string, std::vector<Crime*>*>* subsets = new std::map<const std::string, std::vector<Crime*>*>();
	Crime* crime;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		crime = set[i];
		std::string category = crime->category;
		
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

//GAIN CALCULATION

float info(std::vector<Crime*> set){
	std::map<const std::string, std::vector<Crime*>*> subsets = split_by_category(set);
	float set_size = (float) set.size();
	float freq; //absolute frequency of instance
	float r_freq; //relative frequency of instance
	float info = 0;
	
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		freq = (float) (iterator->second)->size();
		r_freq = freq / set_size ;
		info = info - r_freq * ( log2f(r_freq) );
	}
	
	return info;
}

float info_x(std::vector<Crime*> set, int feature_index){
	std::map<const std::string, std::vector<Crime*>*> subsets = split_by_discrete_feature(set, feature_index);
	float abs_t = set.size();
	float abs_ti;
	float info_x = 0;
	std::vector<Crime*>* current_subset;
	
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		current_subset = iterator->second;
		abs_ti = current_subset->size();
		info_x = info_x + ( (abs_ti / abs_t) * info( *current_subset ) );
	}
	
	return info_x;
}

std::vector<float> split_info(std::vector<Crime*> set, int feature_index){
	std::map<const std::string, std::vector<Crime*>*> subsets = split_by_discrete_feature(set, feature_index);
	std::vector<float>* output = new std::vector<float>();
	float abs_t = set.size();
	float abs_ti;
	float info_x = 0;
	float split_info = 0;
	std::vector<Crime*>* current_subset;
	
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		current_subset = iterator->second;
		abs_ti = current_subset->size();
		info_x = info_x + ( (abs_ti / abs_t) * info( *current_subset ) );
		split_info = split_info + info( *current_subset );
	}
	
	output->push_back(info_x);
	output->push_back(split_info);
	return *output;
}

float gain(std::vector<Crime*> set, int feature_index){
	return info(set) - info_x(set, feature_index);
}

float gain_ratio(std::vector<Crime*> set, int feature_index){
	std::vector<float> partition_data = split_info(set, feature_index);
	float info_x = partition_data[0];
	float split_info = partition_data[1];
	if(split_info == 0) return 0;
	
	return (info(set) - info_x) / split_info;
}


////PREDICTION

Crime* make_prediction(C45 tree, Crime* crime){
	
	//posible optimizacion: anidar una funcion recursiva que busque solo el string
	if(!tree.tree_class.empty()){
		crime->category = tree.tree_class;
	} else {
		std::map<std::string, C45*> children = tree.children;
		int split_index = tree.split_index;
		std::string my_type = crime->features[split_index];
		//cout << my_type <<endl;
		if(children.count(my_type) == 0){
			//printf("fui por other\n");
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
	
	//if(!tree_class.empty()){
	//	cout << "CLASS " << tree_class << " DEPTH " << max_hight << endl;
	//}
	
	children = *(new std::map<std::string, C45*>());
	//lo hago empezar con todas las features
	feature_indeces = new std::vector<int>{ 0, 1, 2 };
	
	//printf("Creando: altura %i, set de tamanio %i\n", max_hight, crimes->size());
		
	if (tree_class.empty() && max_hight > 0) {
		
		int best_index = (*feature_indeces)[0];
		float best_gain = gain(*crimes, best_index); //despues probar con gain simple y ver cual da mejor cross validation
		std::map<const std::string, std::vector<Crime*>*> best_split = split_by_discrete_feature(*crimes, best_index);
		
		for (unsigned int next = 1; next != feature_indeces->size() ; next++){
			float next_gain = gain_ratio(*crimes, (*feature_indeces)[next]);
			if (next_gain > best_gain){
			
				best_gain = next_gain;
				best_index = (*feature_indeces)[next];
				best_split = split_by_discrete_feature(*crimes, best_index);
			
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











