#include "Feature.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct feature {
	
	char* name;
	void* value; //puede ser string (discretas) o int (continuas)

} feature_t;

feature_t* load_feature(char* name, void* value){
	feature_t* feat = new feature_t;

	feat->name = name;
	feat->value = value;
	
	return feat;
}
