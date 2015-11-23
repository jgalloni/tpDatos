#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include "Crime.h"
#include <tgmath.h>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

std::map<const std::string, std::vector<Crime*>*> split_by_discrete_feature(std::vector<Crime*> set, int feature_index);
std::map<const std::string, std::vector<Crime*>*> split_biggest_set(std::vector<Crime*> set, int feature_index);
std::map<const std::string, std::vector<Crime*>*> split_by_category(std::vector<Crime*> set);
int subsets_by_feature(std::vector<Crime*> set, int feature_index);
std::map<const std::string, std::vector<Crime*>*> split_in_quadrants(std::vector<Crime*> set, int feature_index);
std::map<const std::string, std::vector<Crime*>*> split_in_3_clusters(std::vector<Crime*> set, int feature_index = 0);
std::map<const std::string, std::vector<Crime*>*> split_in_4_clusters(std::vector<Crime*> set, int feature_index = 0);

#endif //TEST_FUNCTIONS_H
