#include "Crime.h"
#include "C45.h"
#include <tgmath.h>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

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

//SET OPERATIONS

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


////CLASS

C45::C45(std::vector<Crime*>* crimes, int max_hight){
	tree_class = class_of_tree(crimes);
	children = *(new std::map<std::string, C45*>());
	//lo hago empezar con todas las features
	feature_indeces = new std::vector<int>{ 0, 1, 2 };
		
	//Por la unica razon que se detiene es por max_hight, aun los arboles
	//no se convierten en hojas cuando son de clase homogenea
	//esto hacerlo en class_of_tree
	if (tree_class.empty() && max_hight > 0) {
		
		int best_index = (*feature_indeces)[0];
		float best_gain = gain_ratio(*crimes, best_index); //despues probar con gain simple y ver cual da mejor cross validation
		std::map<const std::string, std::vector<Crime*>*> best_split = split_by_discrete_feature(*crimes, best_index);
		
		for (unsigned int next = 1; next != feature_indeces->size() ; next++){
			float next_gain = gain_ratio(*crimes, (*feature_indeces)[next]);
			if (next_gain > best_gain){
			
				best_gain = next_gain;
				best_index = (*feature_indeces)[next];
				best_split = split_by_discrete_feature(*crimes, best_index);
			
			} 
		}
		
		split_index = best_index;
		for(it_type iterator = best_split.begin(); iterator != best_split.end(); iterator++){
				children[iterator->first] = new C45(iterator->second, max_hight - 1);
		}
		//Aca crear un arbol mas que sea una hoja con la categoria mas comun del padre
	}
}

bool C45::is_leaf(){
	std::string t_class = this->tree_class;
	return !t_class.empty();
}

void C45::set_feature_indeces(std::vector<int>* indeces){
	this->feature_indeces = indeces;
}











