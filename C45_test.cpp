#include "Crimen.h"
#include "C45.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>

void print_test(const char* name, bool result) {
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void basic_tests() {
	std::vector<Crimen>* crimes = new std::vector<Crimen>();
	C45* tree = new C45(crimes);

	print_test("Empty tree test", tree);

	std::string class_of_tree = tree->tree_class;
	print_test("LARCENY/THEFT is default class", class_of_tree.compare("LARCENY/THEFT") == 0);

	print_test("Empty tree is a leaf", (*tree).is_leaf());

	//Falta test de cuando el set tiene datos de un solo tipo. El codigo esta implementado pero prefiero esperar a que el arbol tome samples.

	delete(tree);
}

int main(int argc, char** argv) {
	basic_tests();
   	return 0;
}
