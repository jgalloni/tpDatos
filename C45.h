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

class C45 {
	public:
		C45(std::vector<Crime*>* crimes);

		std::string tree_class;
		std::vector<C45*>* children;

		bool is_leaf();
};


#endif //C45_H
