/*
 This file is part of JustGarble.

    JustGarble is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JustGarble is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JustGarble.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../include/justGarble.h"
#include "../include/garble.h"
#ifdef __cplusplus
extern "C" {
#endif


int check_test_seq(int *input, int *outputs, int nc)
{
	int n = 3;
	int m = 2;
	int w[3];
	w[1] = 0; // DFF reset
	for(int cc=0;cc<3;cc++)
	{
		int x = input[cc*n+0];
		int y = input[cc*n+1];
		w[0] = x & y;
		w[1] = w[1] ^ w[0];
		outputs[cc*m + 0] = w[1];
		outputs[cc*m + 1] = w[1] & x;
	}


	return 0;
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Enter scd file name\n");
		return -1;
	}



	GarbledCircuit gc;

	int i, j;
	readCircuitFromFile(&gc, argv[1]);

	int n = gc.n;
	int m = gc.m;
	int c = gc.c;
	block *inputLabels = (block *)malloc(sizeof(block)*2*n*c);
	block *outputMap = (block *)malloc(sizeof(block)*2*m*c);


	//garbleCircuit(&gc, inputLabels, outputMap);
	//checkCircuit(&gc, inputLabels, outputMap, &(check_test_seq));


	double timeGarble[TIMES];
	double timeEval[TIMES];
	double timeGarbleMedians[TIMES];
	double timeEvalMedians[TIMES];

	for (j = 0; j < TIMES; j++) {
		for (i = 0; i < TIMES; i++) {
			timeGarble[i] = (double)garbleCircuit(&gc, inputLabels, outputMap);
			timeEval[i] = (double)timedEval(&gc, inputLabels);
		}
		timeGarbleMedians[j] = doubleMean(timeGarble, TIMES);// / (gc.q * gc.c);
		timeEvalMedians[j] = doubleMean(timeEval, TIMES);// / (gc.q * gc.c);
	}
	double garblingTime = doubleMean(timeGarbleMedians, TIMES);
	double evalTime = doubleMean(timeEvalMedians, TIMES);
	printf("%lf %lf\n", garblingTime, evalTime);



	return 0;
}

#ifdef __cplusplus
}
#endif
