#ifndef C45_H
#define C45_H

#include "Crimen.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <stddef.h>

std::string class_of_tree(std::vector<Crimen>* crimes);

class C45 {
	public:
		C45(std::vector<Crimen>* crimes);

		std::string tree_class;
		std::vector<C45*>* children;

		bool is_leaf();
};


#endif //C45_H
