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
	std::map<const std::string, std::vector<Crime*>*> subsets = std::map<const std::string, std::vector<Crime*>*>();
	Crime* crime;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		crime = set[i];
		std::string category = crime->features[feature_index];
		
		if(subsets.count(category) == 0) {
				subsets[category] = new std::vector<Crime*>();
		}
		
		subsets[category]->push_back(crime);
	}
	
	return subsets;
}

/*
std::map<const std::string, std::vector<Crime*>*> split_biggest_set(std::vector<Crime*> set, int feature_index){
	//En vez de dividir n valores en n subsets, separa el subset mas grande
	std::map<const std::string, std::vector<Crime*>*>* subsets = new std::map<const std::string, std::vector<Crime*>*>();
	Crime* crime;
	
	std::vector<Crime*>* major = new std::vector<Crime*>();
	std::vector<Crime*>* minor = new std::vector<Crime*>();
	
	std::string major_class = popular_crime(&set);
	(*subsets)[major_class] = major;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		crime = set[i];
		std::string category = crime->features[feature_index];
		
		if(subsets->count(category) == 0) {
				(*subsets)[category] = minor;
		}
		
		(*subsets)[category]->push_back(crime);
	}
	
	return *subsets;
}
*/

//LOCATION FUNCTIONS

std::map<const std::string, std::vector<Crime*>*> split_in_quadrants(std::vector<Crime*> set, int feature_index = 0){
	std::map<const std::string, std::vector<Crime*>*> split = std::map<const std::string, std::vector<Crime*>*>();
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
			
		if(split.count(quadrant) == 0) {
				split[quadrant] = new std::vector<Crime*>();
		}
				
		split[quadrant]->push_back(crime);
	}
	
	return split;
}

std::map<const std::string, std::vector<Crime*>*> split_in_3_clusters(std::vector<Crime*> set, int feature_index = 0){
	//funcion menos generica del universo
	std::map<const std::string, std::vector<Crime*>*> split = std::map<const std::string, std::vector<Crime*>*>();
	std::string cluster;
	Coordinate c1 = Coordinate(-122.4099, 37.73587);
	Coordinate c2 = Coordinate(-122.4152, 37.78174);
	Coordinate c3 = Coordinate(-122.4701, 37.7534);
	Crime* crime;
	float norm;
	float next_norm;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		
		crime = set[i];
			
		norm = crime->coordinate->sqnorm(&c1);
		cluster = "c1";
		next_norm = crime->coordinate->sqnorm(&c2);
		if(next_norm < norm){
				norm = next_norm;
				cluster = "c2";
		}
		next_norm = crime->coordinate->sqnorm(&c3);
		if(next_norm < norm){
				norm = next_norm;
				cluster = "c3";
		}	
			
		if(split.count(cluster) == 0) {
				split[cluster] = new std::vector<Crime*>();
		}
				
		split[cluster]->push_back(crime);
	}
	
	return split;
}


std::map<const std::string, std::vector<Crime*>*> split_in_4_clusters(std::vector<Crime*> set, int feature_index = 0){
	//esta ademas esta duplicada
	std::map<const std::string, std::vector<Crime*>*> split = std::map<const std::string, std::vector<Crime*>*>();
	std::string cluster;
	Coordinate c1 = Coordinate(-122.4096, 37.78474);
	Coordinate c2 = Coordinate(-122.4758, 37.74653);
	Coordinate c3 = Coordinate(-122.4335, 37.77281);
	Coordinate c4 = Coordinate(-122.4089, 37.73512);
	Crime* crime;
	float norm;
	float next_norm;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		
		crime = set[i];
			
		norm = crime->coordinate->sqnorm(&c1);
		cluster = "c1";
		next_norm = crime->coordinate->sqnorm(&c2);
		if(next_norm < norm){
				norm = next_norm;
				cluster = "c2";
		}
		next_norm = crime->coordinate->sqnorm(&c3);
		if(next_norm < norm){
				norm = next_norm;
				cluster = "c3";
		}	
		next_norm = crime->coordinate->sqnorm(&c4);
		if(next_norm < norm){
				norm = next_norm;
				cluster = "c4";
		}	
			
		if(split.count(cluster) == 0) {
				split[cluster] = new std::vector<Crime*>();
		}
				
		split[cluster]->push_back(crime);
	}
	
	return split;
}

//OTHER SET OPERATIONS

std::map<const std::string, std::vector<Crime*>*> split_by_category(std::vector<Crime*> set){
	std::map<const std::string, std::vector<Crime*>*> subsets = std::map<const std::string, std::vector<Crime*>*>();
	Crime* crime;
	
	for(std::vector<Crime*>::size_type i = 0; i != set.size(); ++i) {
		crime = set[i];
		std::string category = crime->category;
		
		if(subsets.count(category) == 0) {
				subsets[category] = new std::vector<Crime*>();
		}
		
		subsets[category]->push_back(crime);
	}
	
	return subsets;
}

int subsets_by_feature(std::vector<Crime*> set, int feature_index){
	std::map<const std::string, std::vector<Crime*>*> subsets = split_by_discrete_feature(set, feature_index);
	return subsets.size();
}

