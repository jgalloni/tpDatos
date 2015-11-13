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
	C45* t1 = new C45(crimes);

	print_test("Empty tree test", t1);

	std::string class_of_tree = t1->tree_class;
	print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);

	print_test("Empty tree is a leaf", (*t1).is_leaf());

	std::vector<int>* v1 = new std::vector<int>{ 0, 1, 2 };
	print_test("New tree will check all features", (* (*t1).feature_indeces) == (*v1) );
	std::vector<int>* v2 = new std::vector<int>{ 0};
	(*t1).set_feature_indeces(v2);
	print_test("Set tree will check set features", (*(*t1).feature_indeces) == (*v2));

	C45* t2 = new C45(&homogeneous);
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
}

void c45_gain_calculation_test(std::vector<Crime*> homogeneous) {
		
	print_title(" GAIN CALCULATION TEST ");
	
	float info0 = info(homogeneous, DAY_OF_WEEK);
	float info1 = info(homogeneous, DISTRICT);
	float info2 = info(homogeneous, ADDRESS);
	
	//Checks back of envelope calculations for these values. Values are not checked exactly in the case of floats.
	print_test("Info of homogeneous in day of week field is 0", info0 == 0);
	print_test("Info of homogeneous in district field is greater than 0", info1 > 0);
	print_test("Info of homogeneous in district field is greater than 2.2", info1 > 2.2);
	print_test("Info of homogeneous in district field is less than 2.3", info1 < 2.3);
	print_test("Info of homogeneous in address field is greater than 0", info2 > 0);
	print_test("Info of homogeneous in address field is greater than 1.5", info2 > 1.5);
	print_test("Info of homogeneous in address field is less than 1.6", info2 < 1.6);

}

void random_forest_test(std::vector<Crime*> crimes) {
	
	print_title(" RANDOM FOREST TEST ");
	
	std::vector<Crime*> sub1 = generate_subset(crimes, 50);
	
	print_test("Succesfully generates subset of size 50", sub1.size() == 50);
	print_test("Previous subset not size 51", sub1.size() != 51);
	

}

int main(int argc, char** argv) {
	std::vector<Crime*> train = readCsv("train.csv");
	std::vector<Crime*> homogeneous = readCsv("homogeneous.csv");
	
	reader_test(train);
	coordinate_tests();
	c45_basic_tests(homogeneous);
	c45_set_operation_test(homogeneous);
	c45_gain_calculation_test(homogeneous);
	random_forest_test(train);
   	return 0;
}
