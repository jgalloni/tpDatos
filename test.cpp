#include "Crime.h"
#include "C45.h"
#include "Coordinate.h"
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
	
}

void c45_basic_tests() {
	
	print_title(" C45 TRIVIAL CHECKS ");
	
	std::vector<Crime>* crimes = new std::vector<Crime>();
	C45* tree = new C45(crimes);

	print_test("Empty tree test", tree);

	std::string class_of_tree = tree->tree_class;
	print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);

	print_test("Empty tree is a leaf", (*tree).is_leaf());

	//Falta test de cuando el set tiene datos de un solo tipo. El codigo esta implementado pero prefiero esperar a que el arbol tome samples.

	delete(tree);
}

int main(int argc, char** argv) {
	coordinate_tests();
	c45_basic_tests();
   	return 0;
}
