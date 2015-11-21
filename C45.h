#ifndef C45_H
#define C45_H

#include "Crime.h"
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>

std::string class_of_tree(std::vector<Crime*>* crimes);

class C45 {
	public:
		C45(std::vector<Crime*>* crimes, int max_hight, int min_divisible);
		
		std::map<const std::string, std::vector<Crime*>*> (*discrete_test[1])(std::vector<Crime*> set, int feature_index);
		std::map<const std::string, std::vector<Crime*>*> (*location_test[1])(std::vector<Crime*> set, int feature_index);
		
		std::string tree_class;
		std::map<std::string, C45*> children;
		std::vector<int>* feature_indeces;
		int split_index;
		std::map<const std::string, std::vector<Crime*>*> (*best_test)(std::vector<Crime*> set, int feature_index);
		
		bool is_leaf();
		void set_feature_indeces(std::vector<int>* indeces);
};

Crime* make_prediction(C45 tree, Crime* crime);

#endif //C45_H
