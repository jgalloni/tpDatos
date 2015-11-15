#ifndef C45_H
#define C45_H

#include "Crime.h"
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>

std::string class_of_tree(std::vector<Crime*>* crimes);

std::map<const std::string, std::vector<Crime*>*> split_by_discrete_feature(std::vector<Crime*> set, int feature_index);
int subsets_by_feature(std::vector<Crime*> set, int feature_index);

float info(std::vector<Crime*> set);
float info_x(std::vector<Crime*> set, int feature_index);
float gain(std::vector<Crime*> set, int feature_index);
float gain_ratio(std::vector<Crime*> set, int feature_index);

class C45 {
	public:
		C45(std::vector<Crime*>* crimes, int max_hight, int min_divisible);

		std::string tree_class;
		std::map<std::string, C45*> children;
		std::vector<int>* feature_indeces;
		int split_index;
		
		bool is_leaf();
		void set_feature_indeces(std::vector<int>* indeces);
};

Crime* make_prediction(C45 tree, Crime* crime);

#endif //C45_H
