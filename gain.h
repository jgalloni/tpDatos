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
float info_x(std::vector<Crime*> set, int feature_index, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int));
std::vector<float> split_info(std::vector<Crime*> set, int feature_index, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int));
float gain(std::vector<Crime*> set, int feature_index, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int));
float gain_ratio(std::vector<Crime*> set, int feature_index, std::map<const std::string, std::vector<Crime*>*> (*split)(std::vector<Crime*>, int));

#endif //GAIN_H
