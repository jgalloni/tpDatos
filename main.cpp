#include <iostream>
#include "Reader.h"
#include "random_forest.h"
#include <iostream>
#include <fstream>
#define NUMBER_OF_CATEGORIES 39
#include <iomanip>

using namespace std;
void output_predictions(std::vector<float> results){

    std::ofstream myfile;
    myfile.open ("predictions.csv");
    myfile << "Id,ARSON,ASSAULT,BAD CHECKS,BRIBERY,BURGLARY,DISORDERLY CONDUCT,DRIVING UNDER THE INFLUENCE,DRUG/NARCOTIC,DRUNKENNESS,EMBEZZLEMENT,EXTORTION,FAMILY OFFENSES,FORGERY/COUNTERFEITING,FRAUD,GAMBLING,KIDNAPPING,LARCENY/THEFT,LIQUOR LAWS,LOITERING,MISSING PERSON,NON-CRIMINAL,OTHER OFFENSES,PORNOGRAPHY/OBSCENE MAT,PROSTITUTION,RECOVERED VEHICLE,ROBBERY,RUNAWAY,SECONDARY CODES,SEX OFFENSES FORCIBLE,SEX OFFENSES NON FORCIBLE,STOLEN PROPERTY,SUICIDE,SUSPICIOUS OCC,TREA,TRESPASS,VANDALISM,VEHICLE THEFT,WARRANTS,WEAPON LAWS\n";

    std::vector<float> probabilities = *(new std::vector<float>(NUMBER_OF_CATEGORIES));

    int current_id = 0;

    int current_category = 0;

    bool previous_was_category = false;

    for (unsigned int i=0; i<results.size(); ++i) {

        if (results[i]==-1) {
            myfile << std::to_string(current_id);
            for (int j=0; j<NUMBER_OF_CATEGORIES; ++j) {
                myfile << ",";
                myfile << std::setprecision(3) << std::fixed << probabilities[j];

            }
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
    }


    myfile.close();
}
int main(int argc, char* argv[]) {

  int n_trees;
  int subset_size;


    if (argc < 3) {
      std::cerr << "Usage example: " << argv[0] << "N_TREES " << "subset_size" << std::endl;
      return 1;
    }

    n_trees = atoi(argv[1]);
    subset_size = atoi(argv[2]);

  std::vector<Crime*> train = readCsv("train.csv");

  std::vector<C45*> trees = generate_trees(train, n_trees, subset_size);

  std::cout << n_trees <<" trees were succesfully generated." << std::endl;

  std::vector<Crime*>().swap(train);

  std::vector<Crime*> predict = readCsv2("test.csv");

  std::vector<float> results = make_predictions(trees,predict);
  std::cout << "Results were succesfully generated." << std::endl;
  std::vector<Crime*>().swap(predict);

  output_predictions(results);

  cout<< "Output file \"predictions.csv\" succesfully generated." <<endl;

    return 0;
}
