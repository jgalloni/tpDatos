#include <string>
#include <vector>
#include <stdlib.h>
#include "Crime.h"
#include "C45.h"
#include  <random>
#include  <iterator>
#include <iomanip>

//esto despues se saca
#include <iostream>
#include <fstream>
#include <future>


#define NUMBER_OF_CATEGORIES 39

#define DEFAULT_HEIGHT 10
#define MIN_DIVISIBLE 5

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


void getTree(std::vector<Crime*> set,std::vector<Crime*> subset, int subset_size,C45* new_tree,std::vector<C45*>* trees){
    subset = generate_subset(set, subset_size);
    new_tree = new C45(subset, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    (*trees).push_back(new_tree);
}

std::vector<C45*> generate_trees(std::vector<Crime*> set, int n_trees, int subset_size){
    std::vector<Crime*> subset = std::vector<Crime*>();
    C45* new_tree;
    std::vector<C45*>* trees = new std::vector<C45*>();
    std::vector<std::future<void>> futures;
    for (int i=0; i<n_trees; i++) {
        futures.push_back(std::async(std::launch::async,getTree, set,subset,subset_size,new_tree,trees));
    }
    for(auto &e : futures) {
        e.get();
    }
    return (*trees);
}


std::vector<float> make_predictions(std::vector<C45*> trees, std::vector<Crime*> predict_these){
    const std::string categories[] = {"ARSON","ASSAULT","BAD CHECKS","BRIBERY","BURGLARY","DISORDERLY CONDUCT","DRIVING UNDER THE INFLUENCE","DRUG/NARCOTIC","DRUNKENNESS","EMBEZZLEMENT","EXTORTION","FAMILY OFFENSES","FORGERY/COUNTERFEITING","FRAUD","GAMBLING","KIDNAPPING","LARCENY/THEFT","LIQUOR LAWS","LOITERING","MISSING PERSON","NON-CRIMINAL","OTHER OFFENSES","PORNOGRAPHY/OBSCENE MAT","PROSTITUTION","RECOVERED VEHICLE","ROBBERY","RUNAWAY","SECONDARY CODES","SEX OFFENSES FORCIBLE","SEX OFFENSES NON FORCIBLE","STOLEN PROPERTY","SUICIDE","SUSPICIOUS OCC","TREA","TRESPASS","VANDALISM","VEHICLE THEFT","WARRANTS","WEAPON LAWS"};

    std::map<std::string, int> categories_indeces = std::map<std::string,int>();
    for (int i=0; i<NUMBER_OF_CATEGORIES; i++) {
        categories_indeces[categories[i]] = i;
    }

    std::string prediction;
    Crime* to_predict;
    int prediction_index;


    std::vector<float> results = std::vector<float>();
    std::vector<float> probabilities = std::vector<float>(NUMBER_OF_CATEGORIES);
    float divisor = trees.size();

    for (unsigned int i=0; i<predict_these.size(); ++i) {
        to_predict = predict_these[i];
        for (unsigned int j=0; j<trees.size(); ++j) {
            prediction = make_prediction_concurrent(*trees[j], to_predict);
            prediction_index = categories_indeces[prediction];
            probabilities[prediction_index] = probabilities[prediction_index] + 1/divisor;
        }
        for (int k=0; k<NUMBER_OF_CATEGORIES; ++k) {
            if (probabilities[k]!=0) {
                results.push_back(k);
                results.push_back(probabilities[k]);
            }
        }
        results.push_back(-1);
        std::fill(probabilities.begin(), probabilities.end(), 0);

    }

    return (results);

}
