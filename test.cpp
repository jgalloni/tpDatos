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
#define DEFAULT_HEIGHT 10
#define MIN_DIVISIBLE 5

using namespace std;
typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

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
    
    Coordinate* c0 = new Coordinate(0,0);
    Coordinate* c1 = new Coordinate(1,1);
    Coordinate* c2 = new Coordinate(2,2);
    Coordinate* c3 = new Coordinate(0,1);
    Coordinate* c4 = new Coordinate(1,0);
    
    print_test("The squared norm from (1,1) to (2,2) is 2", (*c1).sqnorm(c2) == 2 );
    print_test("The squared norm from (0,1) to (1,0) is 2", (*c3).sqnorm(c4) == 2 );
    print_test("The squared norm from (1,1) to (2,2) is not 3", (*c1).sqnorm(c2) != 3 );
    print_test("The operation is commutative", (*c2).sqnorm(c1) == 2);
    print_test("The squared norm from (0,0) to (1,0) is 1", (*c0).sqnorm(c4) == 1 );
    
    delete(c0);
    delete(c1);
    delete(c2);
    delete(c3);
    delete(c4);
}

void c45_basic_tests(std::vector<Crime*> homogeneous) {
    
    print_title(" C45 TRIVIAL CHECKS ");
    
    std::vector<Crime*>* crimes = new std::vector<Crime*>();
    C45* t1 = new C45(crimes, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    
    print_test("Empty tree test", t1);
    
    std::string class_of_tree = t1->tree_class;
    print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);
    
    print_test("Empty tree is a leaf", (*t1).is_leaf());
    
    std::vector<int>* v1 = new std::vector<int>{ 0, 1, 2 };
    print_test("New tree will check all features", (* (*t1).feature_indeces) == (*v1) );
    std::vector<int>* v2 = new std::vector<int>{ 0};
    (*t1).set_feature_indeces(v2);
    print_test("Set tree will check set features", (*(*t1).feature_indeces) == (*v2));
    
    C45* t2 = new C45(&homogeneous, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    class_of_tree = t2->tree_class;
    print_test("Category of homogeneous.csv set is 'OTHER OFFENSES'", class_of_tree.compare("OTHER OFFENSES") == 0);
    print_test("Category of homogeneous.csv set is not 'CANDY'", class_of_tree.compare("CANDY") != 0);
    
    delete(t1);
    delete(t2);
}

void c45_set_operation_test(std::vector<Crime*> homogeneous) {
    print_title(" OPERATIONS WITH SETS ");
    
    print_test("Homogeneous set has 1 subset by day of week (Wednesday)", subsets_by_feature(homogeneous, DAY_OF_WEEK) == 1);
    print_test("Homogeneous set has 5 subsets by district", subsets_by_feature(homogeneous, DISTRICT) == 5);
    print_test("Homogeneous set has 3 subsets by adress (av,st,/)", subsets_by_feature(homogeneous, ADDRESS) == 3);
    
    std::map<const std::string, std::vector<Crime*>*> subs0 = split_by_discrete_feature(homogeneous, DAY_OF_WEEK);
    std::map<const std::string, std::vector<Crime*>*> subs1 = split_by_discrete_feature(homogeneous, DISTRICT);
    
    print_test("Subset in single member maps are not empty", subs0["Wednesday"]->size() != 0);
    print_test("Subset in single member map is as big as the original set", subs0["Wednesday"]->size() == homogeneous.size());
    print_test("Subsets in mutiple member maps are not empty", subs1["NORTHERN"]->size() != 0);
    unsigned int sum = 0;
    
    for(it_type iterator = subs1.begin(); iterator != subs1.end(); iterator++) {
        sum = sum + (iterator->second)->size();
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

void random_forest_test(std::vector<Crime*> crimes) {
    
    print_title(" RANDOM FOREST TEST ");
    
    std::vector<Crime*> sub0 = generate_subset(crimes, 50);
    std::vector<Crime*> sub1 = generate_subset(crimes, 50);
    
    print_test("Succesfully generates subset of size 50", sub1.size() == 50);
    print_test("Previous subset not size 51", sub1.size() != 51);
    
    C45* t0 = new C45(&sub1, DEFAULT_HEIGHT, 1);
    
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
    
    
    
}

void speed_test(std::vector<Crime*> crimes){
    
    print_title(" SPEED TEST ");
    
    std::vector<Crime*> sub1 = generate_subset(crimes, 100);
    std::vector<Crime*> sub2 = generate_subset(crimes, 1000);
    std::vector<Crime*> sub3 = generate_subset(crimes, 10000);
    std::vector<Crime*> sub4 = generate_subset(crimes, 100000);
    
    
    C45* t1;
    C45* t2;
    C45* t3;
    C45* t4;
    
    
    int start;
    int end;
    
    start = clock();
    t1 = new C45(&sub1, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 100 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    start = clock();
    t2 = new C45(&sub2, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 1000 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    start = clock();
    t3 = new C45(&sub3, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 10000 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    start = clock();
    t4 = new C45(&sub4, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    end = clock();
    std::cout << "Size 100000 tree took " << end - start << " ticks, or " << ((float)end - start)/CLOCKS_PER_SEC << " seconds." << std::endl;
    
    
    
    
    
    
    //para suprimir warning de que las variables no se usan.
    t1->is_leaf();
    t2->is_leaf();
    t3->is_leaf();
    t4->is_leaf();
}

void clustering_dataset(std::vector<Crime*> crimes){
    
    print_title(" CLUSTERING DATA ");
    
    std::vector<Crime*> cluster_crimes[4];
    for(Crime* crime:crimes){
        if(crime->coordinate->x <= -122.43&&crime->coordinate->y < 37.7616)
            cluster_crimes[0].push_back(crime);
        else if(crime->coordinate->x > -122.43&&crime->coordinate->y <= 37.7616)
            cluster_crimes[1].push_back(crime);
        else if(crime->coordinate->x <= -122.43&&crime->coordinate->y > 37.7616)
            cluster_crimes[2].push_back(crime);
        else
            cluster_crimes[3].push_back(crime);
    }
    std::cout<<cluster_crimes[0].size()<<" crimes in cluster 1"<<"\n";
    std::cout<<cluster_crimes[1].size()<<" crimes in cluster 2"<<"\n";
    std::cout<<cluster_crimes[2].size()<<" crimes in cluster 3"<<"\n";
    std::cout<<cluster_crimes[3].size()<<" crimes in cluster 4"<<"\n";
    //return clusterCrimes; //por ahora lo dejo asi, total esta en el modulo de test (tiraba warning)
}

void c45_classification_test(std::vector<Crime*> data, std::vector<Crime*> predict){
    
    print_title(" C45 CLASSIFICATION TEST ");
    
    std::vector<Crime*> sample0 = generate_subset(data, 10000);
    std::vector<Crime*> sample1 = generate_subset(data, 10000);
    
    C45* t0 = new C45(&sample0, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    C45* t1 = new C45(&sample1, DEFAULT_HEIGHT, MIN_DIVISIBLE);
    Crime* predict_this = predict[0];
    
    Crime* prediction0 = make_prediction(*t0, predict_this);
    Crime* prediction1 = make_prediction(*t1, predict_this);
    
    print_test("Prediction succesfully made", !(prediction0->category).empty());
    cout << "Prediction is: " << prediction0->category << endl;
    //print_test("Prediction succesfully made", !(prediction0->get_prediction()).empty());
    //cout << "Prediction is: " << prediction0->get_prediction() << endl;
    
    print_test("Same instance, another tree", !(prediction1->category).empty());
    cout << "Prediction is: " << prediction1->category << endl;
    
    //print_test("Same instance, another tree", !(prediction1->get_prediction()).empty());
    //cout << "Prediction is: " << prediction1->get_prediction() << endl;
    
    predict_this = predict[1000];
    prediction0 = make_prediction(*t0, predict_this);
    prediction1 = make_prediction(*t1, predict_this);
    
    
    print_test("Another instance, first tree", !(prediction0->category).empty());
    cout << "Prediction is: " << prediction0->category << endl;
    //cout << prediction0->to_csv()<<endl;
    print_test("Same instance, another tree", !(prediction1->category).empty());
    cout << "Prediction is: " << prediction1->category << endl;
    //cout << prediction1->to_csv()<<endl;
    
    
    
    
}

int main(int argc, char** argv) {
    std::vector<Crime*> train = readCsv("train.csv");
    std::vector<Crime*> homogeneous = readCsv("homogeneous.csv");
    std::vector<Crime*> reduced = readCsv("reduced.csv");
    std::vector<Crime*> predict = readCsv2("test.csv");
    
    reader_test(train, predict);
    coordinate_tests();
    c45_basic_tests(homogeneous);
    c45_set_operation_test(homogeneous);
    c45_gain_calculation_test(homogeneous, reduced);
    c45_classification_test(train, predict);
    random_forest_test(train);
    speed_test(train);
    clustering_dataset(train);
   	return 0;
}

