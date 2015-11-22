#include <string>
#include <vector>
#include <stdlib.h>
#include "Crime.h"
#include "C45.h"
#include "Feature.h"

#include  <random>
#include  <iterator>

#define DEFAULT_HEIGHT 10
#define MIN_DIVISIBLE 5

//Las dos primeras funciones son claramente sacadas de internet. Luis habia dicho algo sobre eso no? Me parece que no se podia.
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

std::vector<Crime*> generate_subset(std::vector<Crime*> set, int subset_size){
	std::vector<Crime*>* subset = new std::vector<Crime*>();
	Crime* new_crime;
	for(int i = 0; i < subset_size ; i++){
		new_crime = * select_randomly(set.begin(), set.end());
		(*subset).push_back(new_crime);
	}

	return (*subset);
}


std::vector<C45*> generate_trees(std::vector<Crime*> set, int n_trees, int subset_size){
    std::vector<Crime*> subset = *new std::vector<Crime*>();
    C45* new_tree;
    std::vector<C45*>* trees = new std::vector<C45*>();
    for (int i=0; i<n_trees; i++) {
        subset = generate_subset(set, subset_size);
        new_tree = new C45(&subset, DEFAULT_HEIGHT, MIN_DIVISIBLE);
        (*trees).push_back(new_tree);
    }
    return (*trees);
}


