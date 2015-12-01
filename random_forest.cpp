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


#define NUMBER_OF_CATEGORIES 39

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
    //por ahi podemos hacer que en vez de subset size le pasemos un porcentaje 0-1
    std::vector<Crime*> subset = std::vector<Crime*>();
    C45* new_tree;
    std::vector<C45*>* trees = new std::vector<C45*>();
    for (int i=0; i<n_trees; i++) {
        subset = generate_subset(set, subset_size);
        new_tree = new C45(subset, DEFAULT_HEIGHT, MIN_DIVISIBLE);
        (*trees).push_back(new_tree);
      //  if (i%10==0 || i== n_trees-1){
      //      std::cout << "van " << i << " arboles\n" << std::endl;
      //  }
    }
    return (*trees);
}

std::vector<float> make_predictions(std::vector<C45*> trees, std::vector<Crime*> predict_these){
    // por ahora que devuelva un map con strings en cada id, despues vemos si lo hacemos void y
    // que output directamente o que.


    const std::string categories[] = {"ARSON","ASSAULT","BAD CHECKS","BRIBERY","BURGLARY","DISORDERLY CONDUCT","DRIVING UNDER THE INFLUENCE","DRUG/NARCOTIC","DRUNKENNESS","EMBEZZLEMENT","EXTORTION","FAMILY OFFENSES","FORGERY/COUNTERFEITING","FRAUD","GAMBLING","KIDNAPPING","LARCENY/THEFT","LIQUOR LAWS","LOITERING","MISSING PERSON","NON-CRIMINAL","OTHER OFFENSES","PORNOGRAPHY/OBSCENE MAT","PROSTITUTION","RECOVERED VEHICLE","ROBBERY","RUNAWAY","SECONDARY CODES","SEX OFFENSES FORCIBLE","SEX OFFENSES NON FORCIBLE","STOLEN PROPERTY","SUICIDE","SUSPICIOUS OCC","TREA","TRESPASS","VANDALISM","VEHICLE THEFT","WARRANTS","WEAPON LAWS"};

    std::map<std::string, int> categories_indeces = std::map<std::string,int>();
    for (int i=0; i<NUMBER_OF_CATEGORIES; i++) {
        categories_indeces[categories[i]] = i;
    }


//    std::vector<std::vector<float>> results = *(new std::vector<std::vector<float>>());

//    std::map<std::string, int> tree_votes = *(new std::map<std::string, int>());
    std::string prediction;
    Crime* to_predict;
    int prediction_index;

    //for crime for tree
//    for (int i=0; i<predict_these.size(); i++) {
//        to_predict = predict_these[i];
//
//        for (int j=0; j<trees.size(); j++) {
//            //llena un map con los votos para cada categoria de los arboles
//            prediction = make_prediction(*trees[j], to_predict);
//            if (tree_votes.count(prediction) != 0){
//                tree_votes[prediction]++;
//            }else{
//                tree_votes[prediction] = 1;
//            }
//
//
//        }


    //for tree for crime

//
//    float divisor = trees.size();
////    float probability;
//    std::vector<float> probabilities;
//
//
//    for (int j=0; j<trees.size(); ++j) {
//        for (int i=0; i<predict_these.size(); ++i) {
//            to_predict = predict_these[i];
//            prediction = make_prediction(*trees[j], to_predict);
//            prediction_index = categories_indeces[prediction];
//            if (j==0) {
//                probabilities = *(new std::vector<float>(39));
//            }
//
//            probabilities[prediction_index] = probabilities[prediction_index] + 1/divisor;
////            results[to_predict->id] = probabilities;
//            results.push_back(probabilities);
//        }
//        delete trees[j];
//
//    }



    //for crime for tree pero con vectores

    std::vector<float> results = std::vector<float>();
    std::vector<float> probabilities = std::vector<float>(NUMBER_OF_CATEGORIES);
    float divisor = trees.size();

    for (unsigned int i=0; i<predict_these.size(); ++i) {
        to_predict = predict_these[i];
        for (unsigned int j=0; j<trees.size(); ++j) {
            prediction = make_prediction(*trees[j], to_predict);
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

        // if (i%20000==0) {
        //   std::cout << "van " << i << "crimenes\n" << std::endl;
        // }


//        probabilities[prediction_index] = probabilities[prediction_index] + 1/divisor;
//        results.push_back(probabilities);
    }

//        if (i%20000==0){
//            std::cout << "van " << i << " crimenes\n" << std::endl;
//        }

//        std::vector<float> probabilities;
//
//        float probability;
//
//        probabilities = *(new std::vector<float>(40));
//        probabilities[0]= to_predict->id;
//        float divisor = trees.size();
//        int votes;
//        for(auto it = tree_votes.begin(); it != tree_votes.end(); ++it ){
//            votes = it->second;
//            probability = votes/divisor;
//            if (probability>0.001) {
//                int index = categories_indeces[it->first] + 1;
//                probabilities[index] = probability;
//            }
//
//        }
//
//        results.push_back(probabilities) ;
//
//        probabilities.clear();
//        //cuarda en la pos id del crimen la categoria mas votada
//        //results[to_predict->id] = most_voted_category;
//        tree_votes.clear();
//
//    }


    return (results);

}


void output_predictions(std::vector<float> results){

    std::ofstream myfile;
    myfile.open ("predictions.csv");
    myfile << "Id,ARSON,ASSAULT,BAD CHECKS,BRIBERY,BURGLARY,DISORDERLY CONDUCT,DRIVING UNDER THE INFLUENCE,DRUG/NARCOTIC,DRUNKENNESS,EMBEZZLEMENT,EXTORTION,FAMILY OFFENSES,FORGERY/COUNTERFEITING,FRAUD,GAMBLING,KIDNAPPING,LARCENY/THEFT,LIQUOR LAWS,LOITERING,MISSING PERSON,NON-CRIMINAL,OTHER OFFENSES,PORNOGRAPHY/OBSCENE MAT,PROSTITUTION,RECOVERED VEHICLE,ROBBERY,RUNAWAY,SECONDARY CODES,SEX OFFENSES FORCIBLE,SEX OFFENSES NON FORCIBLE,STOLEN PROPERTY,SUICIDE,SUSPICIOUS OCC,TREA,TRESPASS,VANDALISM,VEHICLE THEFT,WARRANTS,WEAPON LAWS\n";

    ///refactor aca!!!!!!!!


    //para for crime for tree con los resultadis de una dimension

    std::vector<float> probabilities = *(new std::vector<float>(NUMBER_OF_CATEGORIES));

    int current_id = 0;

    int current_category = 0;

    bool previous_was_category = false;

    for (unsigned int i=0; i<results.size(); ++i) {

        if (results[i]==-1) {
            //cambiar por const
            myfile << std::to_string(current_id);
            for (int j=0; j<NUMBER_OF_CATEGORIES; ++j) {
                myfile << ",";
                myfile << std::setprecision(3) << std::fixed << probabilities[j];

            }
            // va a imprimir un ultimo renglon vacio
            myfile << "\n";
            current_id++;
            std::fill(probabilities.begin(), probabilities.end(), 0);
            continue;
        }

        if (!previous_was_category) {
            previous_was_category = true;
            current_category = int(results[i]);
        } else {
            probabilities[current_category] = results[i];
            previous_was_category = false;
        }

      //  if (current_id%20000==0){
      //      std::cout << "van " << current_id << " registros pasados\n" << std::endl;
      //  }
    }


    //para for tree for crime

//    for (i=0; i<results.size(); ++i) {
//        probabilities = results[i];
//
//        myfile <<  std::to_string(i);
//
//
//        for (j=0; j<number_of_categories; ++j) {
//            myfile << ",";
//
//
//        }
//
//        myfile << "\n";
//
//
//        if (i%20000==0){
//            std::cout << "van " << i << " registros pasados\n" << std::endl;
//        }
//    }

    //viejo

//    for (i=0; i<results.size(); ++i) {
//        probabilities = results[i];
//
//        myfile <<  std::to_string(int(probabilities[0]));
//
//
//        for (j=1; j<number_of_categories; ++j) {
//            myfile << ",";
//            myfile << std::setprecision(3) << std::fixed << probabilities[j];
//
//        }
//
//        myfile << "\n";
//
//
//        if (i%20000==0){
//            std::cout << "van " << i << " registros pasados\n" << std::endl;
//        }
//    }

//    int i=0;
//    for(auto it = results.begin(); it != results.end(); ++it ){
//        myfile << std::to_string(it->first);
//        int index_of_category = categories_indeces[it->second];
//       // unsigned int index_of_category = categories_indeces["WEAPON LAWS"]; esto queda para los tests del writer
//        for (int i=0; i<index_of_category; i++) {
//            myfile << ",0";
//        }
//        myfile << ",1";
//        for (int i =index_of_category+1; i<number_of_categories; i++) {
//            myfile << ",0";
//        }
//        myfile << "\n";
//
//        if (i%20000==0){
//            std::cout << "van " << i << " registros pasados\n" << std::endl;
//        }
//        i++;
//

    myfile.close();
}
