#ifndef GAIN_H
#define GAIN_H

#include "Crime.h"
#include "test_functions.h"
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>

typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

float info(std::vector<Crime*> set);
float info_x(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int), int feature_index);
std::vector<float> split_info(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int), int feature_index);
float gain(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int), int feature_index);
float gain_ratio(std::vector<Crime*> set, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int), int feature_index);

#endif //GAIN_H
