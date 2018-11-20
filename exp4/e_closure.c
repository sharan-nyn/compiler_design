#include <stdio.h>
#include <stdlib.h>

int states;
int *visit_p;

void eclosure(int tab[][states], int state) {
	int i;
	if(!visit_p[state]) {
		visit_p[state] = 1;
		printf("S%d ", state+1);
	} else return;
	for(i=0; i<states; ++i){
		if(i==state) continue;
		// Checks if entry is 1, i.e if it has an eplison edge from state to i.
		if(tab[state][i]) {
			eclosure(tab, i);
		}
	}
	
}

void main(){
	int i, j;
	printf("Enter the number of states: ");
	scanf("%d", &states);
	if(!states) return;
	int eptrans[states][states];
	for (i=0; i<states; ++i){
		for (j=0; j<states; ++j){
			printf("Epsilon transition from S%d to S%d ?\n(1/0): ", i+1,j+1);
			scanf("%d", &eptrans[i][j]);
		}
	}
	for (i=0; i<states; ++i){
		visit_p = (int *)calloc(states, sizeof(int));
		printf("E-Closure of S%d: ", i+1);
		eclosure(eptrans, i);
		free((void *)visit_p);
		printf("\n");
	}
}