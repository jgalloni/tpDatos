#include "Crime.h"
#include "C45.h"
#include "Coordinate.h"
#include "Reader.h"
#include "random_forest.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>

#define DAY_OF_WEEK 0
#define DISTRICT 1
#define ADDRESS 2
#define DEFAULT_HIGHT 50

typedef std::map<const std::string, std::vector<Crime*>*>::iterator it_type;

void print_title(const char* test_title){
	using namespace std;
	cout << "===================================" << endl;
	cout << "~" << test_title << "~" << endl;
	cout << "===================================" << endl;	
}

void print_test(const char* name, bool result) {
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void reader_test(std::vector<Crime*> crimes) {
	
	print_title(" READER TEST ");
	
	print_test("There are 878049 registers in train set", crimes.size() == 878049);
	print_test("There are not 87 registers in train set", crimes.size() != 87);

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
	C45* t1 = new C45(crimes, DEFAULT_HIGHT);

	print_test("Empty tree test", t1);

	std::string class_of_tree = t1->tree_class;
	print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);

	print_test("Empty tree is a leaf", (*t1).is_leaf());

	std::vector<int>* v1 = new std::vector<int>{ 0, 1, 2 };
	print_test("New tree will check all features", (* (*t1).feature_indeces) == (*v1) );
	std::vector<int>* v2 = new std::vector<int>{ 0};
	(*t1).set_feature_indeces(v2);
	print_test("Set tree will check set features", (*(*t1).feature_indeces) == (*v2));

	C45* t2 = new C45(&homogeneous, DEFAULT_HIGHT);
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
	float infox0 = info_x(reduced, DAY_OF_WEEK);
	float infox1 = info_x(reduced, DISTRICT);
	float infox2 = info_x(reduced, ADDRESS);
	float gain0 = gain(reduced, DAY_OF_WEEK);
	float gain1 = gain(reduced, DISTRICT);
	float gain2 = gain(reduced, ADDRESS);
	float gainr0 = gain_ratio(reduced, DAY_OF_WEEK);
	float gainr1 = gain_ratio(reduced, DISTRICT);
	float gainr2 = gain_ratio(reduced, ADDRESS);
	
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
	
	std::vector<Crime*> sub1 = generate_subset(crimes, 50);
	
	print_test("Succesfully generates subset of size 50", sub1.size() == 50);
	print_test("Previous subset not size 51", sub1.size() != 51);
	
	C45* t0 = new C45(&sub1, DEFAULT_HIGHT);
	
	print_test("Creates tree with it", t0);
	

}

int main(int argc, char** argv) {
	std::vector<Crime*> train = readCsv("train.csv");
	std::vector<Crime*> homogeneous = readCsv("homogeneous.csv");
	std::vector<Crime*> reduced = readCsv("reduced.csv");
	
	reader_test(train);
	coordinate_tests();
	c45_basic_tests(homogeneous);
	c45_set_operation_test(homogeneous);
	c45_gain_calculation_test(homogeneous, reduced);
	random_forest_test(train);
   	return 0;
}
