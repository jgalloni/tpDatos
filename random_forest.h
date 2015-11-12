#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H

#include <string>
#include <vector>
#include <stdlib.h>
#include "Crime.h"
#include "C45.h"
#include "Feature.h"

std::vector<Crime*> generate_subset(std::vector<Crime*> set, int subset_size);

#endif //RANDOM_FOREST_H