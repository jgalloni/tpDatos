#include "Crime.h"
#include "C45.h"
#include "Coordinate.h"
#include "Reader.h"
#include "random_forest.h"
#include "gain.h"
#include "test_functions.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <time.h>
#include <map>


#define DAY_OF_WEEK 0
#define DISTRICT 1
#define ADDRESS 2
#define SEASON 3
#define MOMENT_OF_DAY 4
#define MONTH 5
#define HOUR 6
#define WEATHER 7
#define DEFAULT_HEIGHT 10
#define MIN_DIVISIBLE 5

using namespace std;
typedef std::map<const std::string, std::vector<Crime*>>::iterator it_type;

void print_title(const char* test_title){
    cout << "===================================" << endl;
    cout << "~" << test_title << "~" << endl;
    cout << "===================================" << endl;
}

void print_test(const char* name, bool result) {
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void reader_test(std::vector<Crime*> crimes, std::vector<Crime*> predict) {
    
    print_title(" READER TEST ");
    
    print_test("There are more than 800000 registers in train set", crimes.size() > 800000);
    print_test("There are not 87 registers in train set", crimes.size() != 87);
    print_test("Predict set is not empty", predict.size() > 0);
    
}

void coordinate_tests() {
    
    print_title(" COORDINATE TEST ");
    
    Coordinate c0 = Coordinate(0,0);
    Coordinate c1 = Coordinate(1,1);
    Coordinate c2 = Coordinate(2,2);
    Coordinate c3 = Coordinate(0,1);
    Coordinate c4 = Coordinate(1,0);
    
    print_test("The squared norm from (1,1) to (2,2) is 2", c1.sqnorm(&c2) == 2 );
    print_test("The squared norm from (0,1) to (1,0) is 2", c3.sqnorm(&c4) == 2 );
    print_test("The squared norm from (1,1) to (2,2) is not 3", c1.sqnorm(&c2) != 3 );
    print_test("The operation is commutative", c2.sqnorm(&c1) == 2);
    print_test("The squared norm from (0,0) to (1,0) is 1", c0.sqnorm(&c4) == 1 );

}

void c45_basic_tests(std::vector<Crime*> homogeneous) {
    
    print_title(" C45 TRIVIAL CHECKS ");
    
    std::vector<Crime*> crimes = std::vector<Crime*>();
    C45* t1 = new C45(crimes, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    
    print_test("Empty tree test", t1);
    
    std::string class_of_tree = t1->tree_class;
    print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);
    
    print_test("Empty tree is a leaf", (*t1).is_leaf());
    
   // std::vector<int>* v1 = new std::vector<int>{ 0, 1, 2 };
   // print_test("New tree will check all features", (* (*t1).feature_indeces) == (*v1) );
    std::vector<int> v2 = std::vector<int>{ 0};
    (*t1).set_feature_indeces(&v2);
    print_test("Set tree will check set features", (*t1).feature_indeces == v2);
    
    C45* t2 = new C45(homogeneous, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    class_of_tree = t2->tree_class;
    print_test("Category of homogeneous.csv set is 'OTHER OFFENSES'", class_of_tree.compare("OTHER OFFENSES") == 0);
    print_test("Category of homogeneous.csv set is not 'CANDY'", class_of_tree.compare("CANDY") != 0);
    
    delete(t1);
    delete(t2);
}

void c45_set_operation_test(std::vector<Crime*> homogeneous, std::vector<Crime*> train) {
    print_title(" OPERATIONS WITH SETS ");
    
    print_test("Homogeneous set has 1 subset by day of week (Wednesday)", subsets_by_feature(homogeneous, DAY_OF_WEEK) == 1);
    print_test("Homogeneous set has 5 subsets by district", subsets_by_feature(homogeneous, DISTRICT) == 5);
    print_test("Homogeneous set has 3 subsets by address (av,st,/)", subsets_by_feature(train, ADDRESS) == 3);
    cout << subsets_by_feature(homogeneous, ADDRESS) << endl;
    
    std::map<const std::string, std::vector<Crime*>> subs0 = split_by_discrete_feature(homogeneous, DAY_OF_WEEK);
    std::map<const std::string, std::vector<Crime*>> subs1 = split_by_discrete_feature(homogeneous, DISTRICT);
    
    print_test("Subset in single member maps are not empty", subs0["Wednesday"].size() != 0);
    print_test("Subset in single member map is as big as the original set", subs0["Wednesday"].size() == homogeneous.size());
    print_test("Subsets in mutiple member maps are not empty", subs1["NORTHERN"].size() != 0);
    unsigned int sum = 0;
    
    for(it_type iterator = subs1.begin(); iterator != subs1.end(); iterator++) {
        sum = sum + (iterator->second).size();
    }
    print_test("Sum of members of subsets is equal to members of original set", sum == homogeneous.size());
}

void c45_gain_calculation_test(std::vector<Crime*> homogeneous, std::vector<Crime*> reduced) {
    
    print_title(" GAIN CALCULATION TEST ");
    
    float info0 = info(homogeneous);
    float info1 = info(reduced);
    float infox0 = info_x(reduced, split_by_discrete_feature, DAY_OF_WEEK);
    float infox1 = info_x(reduced, split_by_discrete_feature, DISTRICT);
    float infox2 = info_x(reduced, split_by_discrete_feature, ADDRESS);
    float gain0 = gain(reduced, split_by_discrete_feature, DAY_OF_WEEK);
    float gain1 = gain(reduced, split_by_discrete_feature, DISTRICT);
    float gain2 = gain(reduced, split_by_discrete_feature, ADDRESS);
    float gainr0 = gain_ratio(reduced, split_by_discrete_feature, DAY_OF_WEEK);
    float gainr1 = gain_ratio(reduced, split_by_discrete_feature, DISTRICT);
    float gainr2 = gain_ratio(reduced, split_by_discrete_feature, ADDRESS);
    
    //Checks back of envelope calculations for these values. Values are not checked exactly in the case of floats.
    print_test("Info of homogeneous is 0", info0 == 0);
    print_test("Info of reduced is greater than 0", info1 > 0);
    print_test("Info of reduced is greater than 2", info1 > 2);
    print_test("Info of reduced is less than 2.1", info1 < 2.1);
    print_test("Info of day of week partition is equal to original set", infox0 == info1);
    cout << infox0 << " " << info1 << endl;
    print_test("Info of district partition is greater than 0", infox1 > 0);
    print_test("Info of district partition is greater than 0.9", infox1 > 0.9);
    print_test("Info of district partition is less than 1", infox1 < 1 );
    print_test("Info of address partition is greater than 0", infox2 > 0);
    print_test("Info of address partition is not less than 0", !(infox2 < 0));
    print_test("Info of address partition is greater than ", infox2 > 1.15);
    print_test("Info of address partition is less than", infox2 < 1.25);
    print_test("Gain of partitioning by day of week is 0", gain0 == 0);
    print_test("Gain of partitioning by district is greater than 1", gain1 > 1);
    print_test("Gain of partitioning by district is less than 1.1", gain1 < 1.1);
    print_test("Gain of partitioning by address is greater than 0.8", gain2 > 0.8);
    print_test("Gain of partitioning by address is less than 0.9", gain2 < 0.9);
    print_test("Gain ratio of partitioning by day of week is 0", gainr0 == 0);
    print_test("Gain ratio of partitioning by district is greater than 1", gainr1 > 0.3);
    print_test("Gain ratio of partitioning by district is less than 1.1", gainr1 < 0.4);
    print_test("Gain ratio of partitioning by address is greater than 0.8", gainr2 > 0.2);
    print_test("Gain ratio of partitioning by address is less than 0.9", gainr2 < 0.3);
}

void random_forest_test(std::vector<Crime*> crimes, std::vector<Crime*> predict) {
    
    print_title(" RANDOM FOREST TEST ");
    
    std::vector<Crime*> sub0 = generate_subset(crimes, 50);
    std::vector<Crime*> sub1 = generate_subset(crimes, 50);
    
    print_test("Succesfully generates subset of size 50", sub1.size() == 50);
    print_test("Previous subset not size 51", sub1.size() != 51);
    
    C45* t0 = new C45(sub1, DEFAULT_HEIGHT, 1);
    
    print_test("Creates tree with it", t0);
    
    bool different = false;
    for(std::vector<Crime*>::size_type i = 0; i != sub0.size(); i++){
        if(sub0[i] != sub1[i]){
            different = true;
            continue;
        }
    }
    print_test("Two separately generated subsets of same size are different", different == true);
    
    std::vector<C45*> one_tree = generate_trees(crimes, 1, 50);
    C45* t1 = one_tree[0];
    print_test("Sucesfully creates one tree", t1);
    
    std::vector<C45*> fifty_trees = generate_trees(crimes, 50, 100);
    
    print_test("Sucesfully creates 50 trees", fifty_trees.size() == 50);
    
    std::vector<Crime*> one_crime = generate_subset(predict, 1);
    
    std::vector<float> results = make_predictions(fifty_trees,one_crime);
    
    int amount_of_crimes = 0;
    for (unsigned int i=0; i<results.size(); ++i) {
        if (results[i]==-1) {
            amount_of_crimes++;
        }
    }
    
    print_test("Sucesfully creates vector with one set of probabilities from 50 trees", amount_of_crimes == 1);
    
    int j =0;
    float sum_of_probs = 0;
    while (results[j]!=-1) {
        sum_of_probs = results[j+1];
        j= j+2;
        
    }
    
    print_test("Sum of predicted probabilities is not greater than 1", sum_of_probs <= 1);
    
}

void speed_test(std::vector<Crime*> crimes){
    
    print_title(" SPEED TEST ");
    
    std::vector<Crime*> sub1 = generate_subset(crimes, 100);
    std::vector<Crime*> sub2 = generate_subset(crimes, 1000);
    std::vector<Crime*> sub3 = generate_subset(crimes, 10000);
    
    
    C45* t1;
    C45* t2;
    C45* t3;
    
    
    int start;
    int end;
    
    start = clock();
    t1 = new C45(sub1, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 100 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    start = clock();
    t2 = new C45(sub2, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 1000 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    start = clock();
    t3 = new C45(sub3, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 10000 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;

    t1->is_leaf();
    t2->is_leaf();
    t3->is_leaf();
    
    delete t1;
    delete t2;
    delete t3;
}

void c45_classification_test(std::vector<Crime*> data, std::vector<Crime*> predict){
    
    print_title(" C45 CLASSIFICATION TEST ");
    
    std::vector<Crime*> sample0 = generate_subset(data, 1000);
    std::vector<Crime*> sample1 = generate_subset(data, 1000);
    
    C45* t0 = new C45(sample0, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    C45* t1 = new C45(sample1, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    Crime* predict_this = predict[0];
    
    std::string prediction0 = make_prediction(*t0, predict_this);
    std::string prediction1 = make_prediction(*t1, predict_this);
    
    print_test("Prediction succesfully made", !prediction0.empty());
    cout << "Prediction is: " << prediction0 << endl;
    
    print_test("Same instance, another tree", !prediction1.empty());
    cout << "Prediction is: " << prediction1 << endl;
    
    predict_this = predict[1000];
    prediction0 = make_prediction(*t0, predict_this);
    prediction1 = make_prediction(*t1, predict_this);
    
    
    print_test("Another instance, first tree", !prediction0.empty());
    cout << "Prediction is: " << prediction0 << endl;
    print_test("Same instance, another tree", !prediction1.empty());
    cout << "Prediction is: " << prediction1 << endl;
    
    delete t0;
    delete t1;
    
    
}

void single_gain_test(std::vector<Crime*> data, std::map<const std::string, std::vector<Crime*>> (*split)(std::vector<Crime*>, int), int index, std::string test_name){
	
	float gain;
	float sum_gain = 0;
	float i = 0;
	std::vector<Crime*> set;
	
	for(; i != 100; i++){
		set = generate_subset(data, 1000);
		gain = gain_ratio(set, split, index);
		sum_gain += gain;
	}
	
	float mean = sum_gain/i;
	
	cout << test_name << ", set size 1000: " << mean << endl;
}

void feature_analysis(std::vector<Crime*> data){
	print_title(" GAIN RATIO RESULTS ");
	
	single_gain_test(data, split_by_discrete_feature, DAY_OF_WEEK, "DayOfWeek");
	single_gain_test(data, split_by_discrete_feature, DISTRICT, "District");
	single_gain_test(data, split_by_discrete_feature, ADDRESS, "Address");
	single_gain_test(data, split_by_discrete_feature, SEASON, "Season");
	single_gain_test(data, split_by_discrete_feature, MOMENT_OF_DAY, "MomentOfDay");
	single_gain_test(data, split_by_discrete_feature, MONTH, "Month");
	single_gain_test(data, split_by_discrete_feature, HOUR, "Hour");
	single_gain_test(data, split_by_discrete_feature, WEATHER, "Weather");
	single_gain_test(data, split_in_3_clusters, 0, "Location 3 clusters");
	single_gain_test(data, split_in_4_clusters, 0, "Location 4 clusters");
	single_gain_test(data, split_in_quadrants, 0, "Location 4 quadrants");
	

}

void tree_cross_validation(std::vector<Crime*> train){
	
	print_title(" C45 CROSS VALIDATION ");
	
	int start;
    int end;
	
	int tree_size = 9000;
	
	int total_predictions = 0;
	int correct_predictions = 0;
	
	std::vector<Crime*> tree_food;
	C45* tree;
	Crime* predict_this;
	
	std::string correct_answer;
	std::string prediction;
	cout << "Probability of prediction being part of train set: ~2%" << endl;

	std::string result;
	
	for(int i = 0; i != 500; i++){
		start = clock();
		
		result = "incorrect";
		
		tree_food = generate_subset(train, tree_size + 1);
		predict_this = tree_food.back();
		tree_food.pop_back();
		tree = new C45(tree_food, DEFAULT_HEIGHT, MIN_DIVISIBLE);
				
		correct_answer = predict_this -> category;
		prediction = make_prediction(*tree, predict_this);
		
		total_predictions ++;
		if(correct_answer.compare(prediction)) {
			correct_predictions++;
			result = "correct";
		}
		
		delete tree;
		end = clock();
		
		cout << (i+1) << ". Prediction " << prediction << " is " << result << ". Time: " << ((float)end - start)/CLOCKS_PER_SEC << " seconds. (" << (correct_predictions)*100/total_predictions << "%)" << endl;
		
	}
	
	cout << "Out of " << total_predictions << " predictions made, " << correct_predictions << " were correct." << endl;
	
}

int main(int argc, char** argv) {
    std::vector<Crime*> train = readCsv("train.csv");
    //std::vector<Crime*> homogeneous = readCsv("homogeneous.csv");
    //std::vector<Crime*> reduced = readCsv("reduced.csv");
    //std::vector<Crime*> predict = readCsv2("test.csv");
    
    //reader_test(train, predict);
    //coordinate_tests();
    //c45_basic_tests(homogeneous);
    //c45_set_operation_test(homogeneous, train);
    //c45_gain_calculation_test(homogeneous, reduced);
    //c45_classification_test(train, predict);
    //random_forest_test(train,predict);
    //speed_test(train);
    //feature_analysis(train);
    tree_cross_validation(train);
        
   	return 0;
}

