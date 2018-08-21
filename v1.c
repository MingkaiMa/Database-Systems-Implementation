#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>


typedef struct IntSet
{
	int* setArray;
	int size;

} IntSet;



int compareFunc(const void* a, const void* b){
	return (*(int*)a - *(int*)b);
}


int inArray(int* array, int size, int toPut){
	for(int i = 0; i < size; ++i){
		if(array[i] == toPut)
			return 1;
	}
	return 0;
}

IntSet* IntSet_Input(char* s){

	char *str = s;
	//int* setList;

	//IntSet *result;



	int status, i;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	const char* pattern = "^ *{ *(-?([0-9]+ *,)+ *)*((-?[0-9]+ *})|( *})) *$";
	char* buf = str;
	//char& buf = argv[1];

	regcomp(&reg, pattern, cflags);
	status = regexec(&reg, buf, nmatch, pmatch, 0);
	if(status == REG_NOMATCH)
		printf("No match\n");
	else if(status == 0)
	{
		printf("Match:\n");
		for(i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i)
			putchar(buf[i]);
		printf("\n");
	}
	regfree(&reg);


	int length = strlen(buf);


	printf("length is: %d\n", length);


	int dyArraySize = 10;

	int* dyArray = malloc(sizeof(int) * dyArraySize);
	int dyIndex = 0;

	int k = 0;

	while(k < length){

		//printf("beg: k is: %d\n", k);
		while(!isdigit(buf[k]) && buf[k] != '-')
			++k;

		if(k >= length)
			break;

		int j = k;

		while(isdigit(buf[j]) || buf[j] == '-')
			++j;


		//printf("k is: %d, j is: %d\n", k, j);
		char* nb = malloc(sizeof(char) * (j - k));

		for(int ii = 0; k < j; ++k){
			nb[ii] = buf[k];
			++ii;
		}

		printf("--%s\n", nb);

		int toPut = atoi(nb);

		if(!inArray(dyArray, dyIndex, toPut)){

			dyArray[dyIndex] = toPut;
			++dyIndex;

			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize *= 2;
			}
		}
	}

	qsort(dyArray, dyIndex, sizeof(int), compareFunc);
	for(int i = 0; i < dyIndex; ++i){
		printf("%d, ", dyArray[i]);
	}
	printf("\n");

	printf("dyIndex is: %d\n", dyIndex);
	int* _setArray = malloc(sizeof(int) * dyIndex);

	for(int i = 0; i < dyIndex; ++i)
		_setArray[i] = dyArray[i];





	for(int i = 0; i < dyIndex; ++i)
		printf("%d, ", _setArray[i]);

	printf("\n");
	IntSet* result = malloc(sizeof(IntSet));
	result->setArray = _setArray;
	result->size = dyIndex;


	return result;

}

char* IntSet_Output(IntSet* intset){
	int* array = intset->setArray;
	int size = intset->size;


	int csSize = 100;

	char* cstring = malloc(sizeof(char) * (csSize));

	cstring[0] = '{';

	int csIndex = 1;
	for(int i = 0; i < size; ++i){
		
		int curNb = array[i];

		if(array[i] < 0){
			cstring[csIndex] = '-';
			++csIndex;
			if(csIndex == csSize){
				cstring = realloc(cstring, csSize * 2);
				csSize = csSize * 2;
			}
			curNb = curNb * (-1);
		}

		int digitSize = 0;

		while(curNb){
			curNb = curNb / 10;
			++digitSize;
		}


		curNb = abs(array[i]);

		while(digitSize > 0){

			int thisdigit = curNb / (int)pow(10, digitSize - 1);
		
			char ch = '0' + thisdigit;

			cstring[csIndex] = ch;
			++csIndex;
			if(csIndex == csSize){
				cstring = realloc(cstring, csSize * 2);
				csSize = csSize * 2;
			}

			curNb = curNb - thisdigit * (int)pow(10, digitSize - 1);
			--digitSize;
		}


		if(i != size - 1){
			cstring[csIndex] = ',';
			++csIndex;
			if(csIndex == csSize){
				cstring = realloc(cstring, csSize * 2);
				csSize = csSize * 2;
			}
		}
	}

	cstring[csIndex] = '}';
	++csIndex;


	printf("csIndex: %d\n", csIndex);
	char* resStr = calloc(csIndex, sizeof(char));

	//strcpy(resStr, cstring);

	for(int ii = 0; ii < csIndex; ++ii){
		printf("ii: %d, ", ii);
		resStr[ii] = cstring[ii];
	}
	printf("\n");

	free(cstring);

	printf("at %lu\n", strlen(resStr));

	return resStr;

}



// operations

// i <@ S
//
int ifMember(int target, IntSet* S){
	for(int i = 0; i < S->size; ++i){
		if(S->setArray[i] == target)
			return 1;
	}

	return 0;
}



// @ S
//
int getCardinality(IntSet* S){
	return S->size;
}

// A @> B
//
int isSubset(IntSet* A, IntSet* B){
	for(int i = 0; i < A->size; ++i){
		if(!ifMember(A->setArray[i], B))
			return 0;
	}
	return 1;
}


// A = B
//

int isEqual(IntSet* A, IntSet* B){
	if(A->size != B->size)
		return 0;

	for(int i = 0; i < A->size; ++i){
		if(A->setArray[i] != B->setArray[i])
			return 0;
	}
	return 1;
}

// A && B
//

IntSet* intersection(IntSet* A, IntSet* B){

	int dyArraySize = 100;
	int* dyArray = malloc(sizeof(int) * dyArraySize);
	int dyIndex = 0;

	for(int i = 0; i < A->size; ++i){
		if(ifMember(A->setArray[i], B)){
			dyArray[dyIndex] = A->setArray[i];
			++dyIndex;
			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize *= 2;
			}
		}
	}


	int* array = calloc(dyIndex, sizeof(int));

	for(int i = 0; i < dyIndex; ++i){
		array[i] = dyArray[i];
	}

	IntSet* result = malloc(sizeof(IntSet));
	result->setArray = array;
	result->size = dyIndex;

	return result;
}

// A || B
//
IntSet* Uunion(IntSet* A, IntSet* B){

	int dyArraySize = 100;
	int* dyArray = malloc(sizeof(int) * dyArraySize);
	int dyIndex = 0;

	for(int i = 0; i < A->size; ++i){
		dyArray[dyIndex] = A->setArray[i];
		++dyIndex;
		if(dyIndex == dyArraySize){
			dyArray = realloc(dyArray, dyArraySize * 2);
			dyArraySize = dyArraySize * 2;
		}
	}

	for(int i = 0; i < B->size; ++i){
		if(!ifMember(B->setArray[i], A)){
			dyArray[dyIndex] = B->setArray[i];
			++dyIndex;
			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize = dyArraySize * 2;
			}
		}
	}


	qsort(dyArray, dyIndex, sizeof(int), compareFunc);

	int* array = calloc(dyIndex, sizeof(int));

	for(int i = 0; i < dyIndex; ++i){
		array[i] = dyArray[i];
	}

	IntSet* result = malloc(sizeof(IntSet));
	result->setArray = array;
	result->size = dyIndex;

	return result;
}


// A !! B
//

IntSet* Ddisjunction(IntSet* A, IntSet* B){
	int dyArraySize = 100;
	int* dyArray = malloc(sizeof(int) * dyArraySize);
	int dyIndex = 0;


	for(int i = 0; i < A->size; ++i){
		if(!ifMember(A->setArray[i], B)){
			dyArray[dyIndex] = A->setArray[i];
			++dyIndex;
			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize = dyArraySize * 2;
			}
		}
	}

	for(int i = 0; i < B->size; ++i){
		if(!ifMember(B->setArray[i], A)){
			dyArray[dyIndex] = B->setArray[i];
			++dyIndex;
			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize = dyArraySize * 2;
			}
		}
	}

	qsort(dyArray, dyIndex, sizeof(int), compareFunc);

	int* array = calloc(dyIndex, sizeof(int));

	for(int i = 0; i < dyIndex; ++i){
		array[i] = dyArray[i];
	}

	IntSet* result = malloc(sizeof(IntSet));
	result->setArray = array;
	result->size = dyIndex;

	return result;
}


// A - B
//

IntSet* diff(IntSet* A, IntSet* B){
	int dyArraySize = 100;
	int* dyArray = malloc(sizeof(int) * dyArraySize);
	int dyIndex = 0;


	for(int i = 0; i < A->size; ++i){
		if(!ifMember(A->setArray[i], B)){
			dyArray[dyIndex] = A->setArray[i];
			++dyIndex;
			if(dyIndex == dyArraySize){
				dyArray = realloc(dyArray, dyArraySize * 2);
				dyArraySize = dyArraySize * 2;
			}
		}
	}


	int* array = calloc(dyIndex, sizeof(int));

	for(int i = 0; i < dyIndex; ++i){
		array[i] = dyArray[i];
	}

	IntSet* result = malloc(sizeof(IntSet));
	result->setArray = array;
	result->size = dyIndex;

	return result;

}




int main(int argc, char *argv[]){
	char* s = argv[1];
	char* s1 = argv[2];

 
	IntSet* r1 =  IntSet_Input(s);
	IntSet* r2 = IntSet_Input(s1);
 	// printf("%d\n", r->setArray[0]);
	// printf("size is: %d\n", r->size);

	// printf("Output start from here\n");
	//char* ss = IntSet_Output(r);
	// printf("%lu\n", strlen(ss));

	// printf("%s\n", ss);


	// test i <@ S
	// if(ifMember(3, r))
	// 	printf("3 is a member of r\n");
	// else
	// 	printf("3 is not a member of r\n");

	// test @ S
	// printf("Size of set is: %d\n", getCardinality(r));


	// test A @> B
	// if(isSubset(r1, r2))
	// 	printf("r1 is subset of r2\n");
	// else
	// 	printf("r1 is not subset of r2\n");

	// test A = B
	// if(isEqual(r1, r2))
	// 	printf("r1 is equal to r2\n");
	// else
	// 	printf("ri is not equial to r2\n");

	// test A && B
	// IntSet* r3 = intersection(r1, r2);

	// char* ss = IntSet_Output(r3);

	// printf("%s\n", ss);

	// test A || B

	// IntSet* r3 = Uunion(r1, r2);

	// char* ss = IntSet_Output(r3);

	// printf("%s\n", ss);


	// test A !! B

	// IntSet* r3 = Ddisjunction(r1, r2);

	// char* ss = IntSet_Output(r3);

	// printf("%s\n", ss);


	// test A - B
	IntSet* r3 = diff(r1, r2);

	char* ss = IntSet_Output(r3);

	printf("%s\n", ss);


}