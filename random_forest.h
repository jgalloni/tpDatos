#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include <string>
#include <vector>
#include <stdlib.h>
#include "Crime.h"
#include "C45.h"

std::vector<Crime*> generate_subset(std::vector<Crime*> set, int subset_size);
std::vector<C45*> generate_trees(std::vector<Crime*> set, int n_trees, int subset_size);
std::map<unsigned int, std::string> make_predictions(std::vector<C45*> trees, std::vector<Crime*> predict_these);

#endif //RANDOM_FOREST_H
