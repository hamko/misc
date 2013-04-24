/*****************
 ƒ‹ƒ“ƒQƒNƒbƒ^–@
 Š`’J‚³‚ñ‚©‚ç‚Ì’¸‚«•¨
*****************/


#include <stdlib.h>
#include "runge_kutta.h"

void runge_kutta(double (*F)(double[], double, int, double[]), double h, double vector[], double t, int n, double input[]){
	int i;
	double *vector0, *vector1, *vector2, *vector3, *temp;
	
	vector0 = (double *)calloc(n, sizeof(double));
	vector1 = (double *)calloc(n, sizeof(double));
	vector2 = (double *)calloc(n, sizeof(double));
	vector3 = (double *)calloc(n, sizeof(double));
	temp = (double *)calloc(n, sizeof(double));

	for(i=0;i<n;i++){
		vector0[i] = h * F(vector, t, i, input) / 2;
	}

	add_vector(vector, vector0, temp, n);
	for(i=0;i<n;i++){
		vector1[i] = h * F(temp, t + h/2, i, input) / 2;
	}

	add_vector(vector, vector1, temp, n);
	for(i=0;i<n;i++){
		vector2[i] = h * F(temp, t + h/2, i, input);
	}
	
	add_vector(vector, vector2, temp, n);
	for(i=0;i<n;i++){
		vector3[i] = h * F(temp, t + h, i, input);
	}

	for(i=0;i<n;i++){
		vector[i] = vector[i] + (vector0[i]*2 + vector1[i]*4 + vector2[i]*2 + vector3[i]) / 6;
	}

	free(vector0);
	free(vector1);
	free(vector2);
	free(vector3);
	free(temp);
}

void add_vector(double add1[], double add2[], double result[], int n){
	int i;

	for(i=0;i<n;i++){
		result[i] = add1[i] + add2[i];
	}
}


