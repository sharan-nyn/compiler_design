#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* alpha = "abcdefghijklmnopqrstuvwxyz";
int NSTATES, NSYM;
int *visit_p;

void print_state(int num) {
	int index = 0;
	if (num == 0) {
		printf("\tφ\t");
		return;
	}
	printf("\t{");
	while (num) {
		if(num&1) printf("%c,", alpha[index]);
		num = num >> 1;
		index++;
	}
	printf("\b}\t");
}

void state2bit(int state, int *num) {
	int mask = 1;
	mask = mask << state;
	*num = (*num) | mask;
}

void eclosure_r(int tab[NSTATES][NSTATES][NSYM], int state, int *ecl) {
	int i;
	if(!visit_p[state]) {
		visit_p[state] = 1;
		state2bit(state, ecl);
	} else return;
	for(i=0; i<NSTATES; ++i){
		if(i==state) continue;
		if(tab[state][i][NSYM-1]) {
			eclosure_r(tab, i, ecl);
		}
	}
}

int get_eclosure(int tab[NSTATES][NSTATES][NSYM], int state) {
	int ecl=0, index=0;
	visit_p = (int *)calloc(NSTATES, sizeof(int));
	while(state) {
		if(state&1) eclosure_r(tab, index, &ecl);
		state = state >> 1;
		index++;
	}
	free(visit_p);
	return ecl;
}

int get_edges(int tab[NSTATES][NSTATES][NSYM], int state, int symbol){
	int result=0, index=0, i;
	while(state){
		if(state&1)
			for(i=0; i<NSTATES; ++i)
				if(tab[index][i][symbol]) state2bit(i, &result);
		state = state>>1;
		index++;
	}
	return result;
}

void main(int argc, char* argv[]) {
	char input[20];
	char *token, *rest;
	if(argc != 2){
		printf("Usage: ./a.out <input.txt>\n");
		return;
	}
	FILE *fp = fopen(argv[1],"r");
	int i , j, k;
	printf("Enter number of states and number of symbols: ");
	scanf("%d %d", &NSTATES, &NSYM);

	int table[NSTATES][NSTATES][NSYM];
	memset(table, 0, sizeof(int)*NSTATES*NSTATES*NSYM); //reset table to 0

	for(i=0; i<NSTATES; ++i) {
		for (j=0; j<NSYM; ++j) {
			char* rest = input;
			fscanf(fp, "%s", input);						// read input from file
			if (atoi(input) == -1) continue;				// if state is -1 i.e null then skip
			while ((token = strtok_r(rest, ",", &rest)))	// use strtok_r to separate comma seperated values
				table[i][atoi(token)][j] = 1;				// fill the 3d matrix
		}
	}

	for (i=0; i<NSTATES; ++i) {
		int ec_result, result;
		print_state(1<<i);
		ec_result = get_eclosure(table, 1<<i);
		for(j=0; j<NSYM-1; ++j) {
			result = get_edges(table, ec_result, j);
			result = get_eclosure(table, result);
			print_state(result);
		}
		printf("\n");
	}
}