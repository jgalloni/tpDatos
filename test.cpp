#include "Crime.h"
#include "C45.h"
#include "Coordinate.h"
#include "Reader.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>

void print_title(const char* test_title){
	using namespace std;
	cout << "===================================" << endl;
	cout << "~" << test_title << "~" << endl;
	cout << "===================================" << endl;	
}

void print_test(const char* name, bool result) {
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void reader_test() {
	
	print_title(" READER TEST ");
	
	std::vector<Crime*> crimes = readCsv("train.csv");
	print_test("There are 878049 registers in train set", crimes.size() == 878049);
	print_test("There are not 87 registers in train set", crimes.size() != 87);

	//delete(*crimes);
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



void c45_basic_tests() {
	
	print_title(" C45 TRIVIAL CHECKS ");
	
	std::vector<Crime*>* crimes = new std::vector<Crime*>();
	C45* t1 = new C45(crimes);

	print_test("Empty tree test", t1);

	std::string class_of_tree = t1->tree_class;
	print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);

	print_test("Empty tree is a leaf", (*t1).is_leaf());

	std::vector<Crime*> homogeneous = readCsv("homogeneous.csv");
	C45* t2 = new C45(&homogeneous);
	class_of_tree = t2->tree_class;
	print_test("Category of homogeneous.csv set is 'OTHER OFFENSES'", class_of_tree.compare("OTHER OFFENSES") == 0);
	print_test("Category of homogeneous.csv set is not 'CANDY'", class_of_tree.compare("CANDY") != 0);
	
	delete(t1);
	delete(t2);
}

int main(int argc, char** argv) {
	coordinate_tests();
	c45_basic_tests();
	reader_test();
   	return 0;
}
