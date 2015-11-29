#include "Crime.h"
#include "test_functions.h"
#include "C45.h"
#include <tgmath.h>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

float info(std::vector<Crime*> set){
	std::map<const std::string, std::vector<Crime*>> subsets = split_by_category(set);
	float set_size = (float) set.size();
	float freq; //absolute frequency of instance
	float r_freq; //relative frequency of instance
	float info = 0;
	
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		freq = (float) (iterator->second).size();
		r_freq = freq / set_size ;
		info = info - r_freq * ( log2f(r_freq) );
	}
	
	return info;
}

float info_x(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>> (*split)(std::vector<Crime*>, int), int feature_index = 0){
	std::map<const std::string, std::vector<Crime*>> subsets = split(set, feature_index);
	float abs_t = set.size();
	float abs_ti;
	float info_x = 0;
	std::vector<Crime*> current_subset;
	
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		current_subset = iterator->second;
		abs_ti = current_subset.size();
		info_x = info_x + ( (abs_ti / abs_t) * info( current_subset ) );
	}
	
	return info_x;
}

std::vector<float> split_info(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>> (*split)(std::vector<Crime*>, int), int feature_index = 0){
	std::map<const std::string, std::vector<Crime*>> subsets = split(set, feature_index);
	std::vector<float> output = std::vector<float>();
	float abs_t = set.size();
	float abs_ti;
	float info_x = 0;
	float split_info = 0;
	std::vector<Crime*> current_subset;
		
	for(it_type iterator = subsets.begin(); iterator != subsets.end(); iterator++) {
		current_subset = iterator->second;
		abs_ti = current_subset.size();
		info_x = info_x + ( (abs_ti / abs_t) * info( current_subset ) );
		split_info = split_info + info( current_subset );
	}
		
	output.push_back(info_x);
	output.push_back(split_info);
	return output;
}

float gain(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>> (*split)(std::vector<Crime*>, int), int feature_index = 0){
	return info(set) - info_x(set, split, feature_index);
}

float gain_ratio(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>> (*split)(std::vector<Crime*>, int), int feature_index = 0){
	std::vector<float> partition_data = split_info(set, split, feature_index);
	float info_x = partition_data[0];
	float split_info = partition_data[1];
	if(split_info == 0) return 0;
	
	return (info(set) - info_x) / split_info;
}
