#include <string>
#include <vector>
#include <stdlib.h>
#include "Crime.h"
#include "C45.h"
#include  <random>
#include  <iterator>


//esto despues se saca
#include <iostream>
#include <fstream>




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
        new_tree = new C45(&subset, DEFAULT_HEIGHT, MIN_DIVISIBLE);
        (*trees).push_back(new_tree);
//        if (i%10==0 || i== n_trees-1){
//            std::cout << "van " << i << " arboles\n" << std::endl;
//        }
    }
    return (*trees);
}

std::vector<std::vector<float>> make_predictions(std::vector<C45*> trees, std::vector<Crime*> predict_these){
    // por ahora que devuelva un map con strings en cada id, despues vemos si lo hacemos void y
    // que output directamente o que.
    
    int number_of_categories = 39; //esto no es mejor ponerlo como un define?
    const std::string categories[] = {"ARSON","ASSAULT","BAD CHECKS","BRIBERY","BURGLARY","DISORDERLY CONDUCT","DRIVING UNDER THE INFLUENCE","DRUG/NARCOTIC","DRUNKENNESS","EMBEZZLEMENT","EXTORTION","FAMILY OFFENSES","FORGERY/COUNTERFEITING","FRAUD","GAMBLING","KIDNAPPING","LARCENY/THEFT","LIQUOR LAWS","LOITERING","MISSING PERSON","NON-CRIMINAL","OTHER OFFENSES","PORNOGRAPHY/OBSCENE MAT","PROSTITUTION","RECOVERED VEHICLE","ROBBERY","RUNAWAY","SECONDARY CODES","SEX OFFENSES FORCIBLE","SEX OFFENSES NON FORCIBLE","STOLEN PROPERTY","SUICIDE","SUSPICIOUS OCC","TREA","TRESPASS","VANDALISM","VEHICLE THEFT","WARRANTS","WEAPON LAWS"};
    
    std::map<std::string, int> categories_indeces = std::map<std::string,int>();
    for (int i=0; i<number_of_categories; i++) {
        categories_indeces[categories[i]] = i;
    }
    
    
//    std::map<int, std::vector<float>> results = *(new std::map<int, std::vector<float> >());
    std::vector<std::vector<float>> results = std::vector<std::vector<float>>();

    std::map<std::string, int> tree_votes = std::map<std::string, int>();
    std::string prediction;
    Crime* to_predict;
    for (unsigned int i=0; i<predict_these.size(); i++) {
        to_predict = predict_these[i];
        
        for (unsigned int j=0; j<trees.size(); j++) {
            //llena un map con los votos para cada categoria de los arboles
            prediction = make_prediction(*trees[j], to_predict);
            if (tree_votes.count(prediction) != 0){
                tree_votes[prediction]++;
            }else{
                tree_votes[prediction] = 1;
            }
            
            
        }
//        if (i%20000==0){
//            std::cout << "van " << i << " crimenes\n" << std::endl;
//        }
        

        
        std::vector<float> probabilities;
        
        float probability;
        
        probabilities = std::vector<float>(40);
        probabilities[0]= to_predict->id;
        float divisor = trees.size();
        int votes;
        for(auto it = tree_votes.begin(); it != tree_votes.end(); ++it ){
            votes = it->second;
            probability = votes/divisor;
            if (probability>0.001) {
                int index = categories_indeces[it->first] + 1;
                probabilities[index] = probability;
            }
        
        }
        
        results.push_back(probabilities) ;
        
        probabilities.clear();
        //cuarda en la pos id del crimen la categoria mas votada
        //results[to_predict->id] = most_voted_category;
        tree_votes.clear();
    
    }
    
    
    return (results);
    
}



void output_predictions(std::vector<std::vector<float>> results){
    unsigned int number_of_categories = 39;

    
    std::ofstream myfile;
    myfile.open ("predictions.csv");
    myfile << "Id,ARSON,ASSAULT,BAD CHECKS,BRIBERY,BURGLARY,DISORDERLY CONDUCT,DRIVING UNDER THE INFLUENCE,DRUG/NARCOTIC,DRUNKENNESS,EMBEZZLEMENT,EXTORTION,FAMILY OFFENSES,FORGERY/COUNTERFEITING,FRAUD,GAMBLING,KIDNAPPING,LARCENY/THEFT,LIQUOR LAWS,LOITERING,MISSING PERSON,NON-CRIMINAL,OTHER OFFENSES,PORNOGRAPHY/OBSCENE MAT,PROSTITUTION,RECOVERED VEHICLE,ROBBERY,RUNAWAY,SECONDARY CODES,SEX OFFENSES FORCIBLE,SEX OFFENSES NON FORCIBLE,STOLEN PROPERTY,SUICIDE,SUSPICIOUS OCC,TREA,TRESPASS,VANDALISM,VEHICLE THEFT,WARRANTS,WEAPON LAWS\n";
    std::vector<float> probabilities;
    unsigned int i;
    unsigned int j;
    
    ///refactor aca!!!!!!!!
    
    for (i=0; i<results.size(); ++i) {
        probabilities = results[i];
        
        myfile <<  std::to_string(int(probabilities[0]));
 
        
        for (j=1; j<number_of_categories; ++j) {
            myfile << ",";
            myfile << std::to_string(probabilities[j]);
//            myfile << ",";
        }
//        myfile << std::to_string(probabilities[j+1]);
        myfile << "\n";
//        if (i%20000==0){
//            std::cout << "van " << i << " registros pasados\n" << std::endl;
//        }
    }
    

    
    myfile.close();
}




