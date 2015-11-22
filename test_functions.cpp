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



//DISCRETE FUNCTIONS

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

//LOCATION FUNCTIONS

std::map<const std::string, std::vector<Crime*>*> split_in_quadrants(std::vector<Crime*> set, int feature_index = 0){
	std::map<const std::string, std::vector<Crime*>*>* split = new std::map<const std::string, std::vector<Crime*>*>();
	std::string quadrant;
	Crime* crime;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		
		crime = set[i];
		
		if(crime->coordinate->x <= -122.43&&crime->coordinate->y < 37.7616)
			quadrant = "Q1";
		else if(crime->coordinate->x > -122.43&&crime->coordinate->y <= 37.7616)
			quadrant = "Q2";
		else if(crime->coordinate->x <= -122.43&&crime->coordinate->y > 37.7616)
			quadrant = "Q3";
		else
			quadrant = "Q4";
			
		if(split->count(quadrant) == 0) {
				(*split)[quadrant] = new std::vector<Crime*>();
		}
				
		(*split)[quadrant]->push_back(crime);
	}
	
	return *split;
}

//OTHER SET OPERATIONS

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

