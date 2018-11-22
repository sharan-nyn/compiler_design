#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NSTATES, NSYM;
const char* alpha = "abcdefghijklmnopqrstuvwxyz";

int minimize(int marked[][NSTATES], int table[NSTATES][NSYM]) {
	int new_flag=0, i, j;
	int out1, out2, k, merge=0;
	for(i=0; i<NSTATES; ++i) {
		for (j=0; j<i; ++j) {
			if(marked[i][j] == 0) {
				// printf("\nfor i,j: %d,%d\n", i,j);
				for(k=0; k<NSYM; ++k) {
					out1 = table[i][k];
					out2 = table[j][k];
					// printf("\npair to check (%d, %d)\n", out1, out2);
					if(marked[out1][out2]==1 || marked[out2][out1]==1) {
						//we found that the pair is marked
						if(marked[i][j] == 0) marked[i][j]=1;
						if(marked[j][i] == 0) marked[j][i]=1;
						// printf("marked: (%d,%d)", i,j);
						new_flag = 1;
					}
				}
				merge++;
			}
		}
	}
	if(new_flag) minimize(marked, table);
	else return merge;
}

void markfinal(int final, int marked[][NSTATES]) {
	int i=0,j;
	int final_c=final;
	while(final) {
		if(final&1)
			for(j=0; j<NSTATES; j++) {
				if(i==j || (final_c & (1<<j))) continue;
				else {
					if(marked[i][j]!=-1) marked[i][j]=1;
					if(marked[j][i]!=-1) marked[j][i]=1;
				}
			}
		final = final >> 1;
		i++;
	}
}

void bit2state(int num, int final) {
	int index = 0;
	printf("\t");
	if (num & final) printf("*");
	printf("[");
	while (num) {
		if(num&1) printf("%c,", alpha[index]);
		num = num >> 1;
		index++;
	}
	printf("\b]\t");
}

void state2bit(int state, int *num) {
	int mask = 1;
	mask = mask << state;
	*num = (*num) | mask;
}

void main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Usage: ./a.out <input.txt>\n");
		return;
	}
	FILE *fp = fopen(argv[1],"r");
	char final_s[20];
	char *token, *rest;
	int i , j, k, l, input, final=0, merged=0;
	printf("Enter number of states and number of symbols: ");
	scanf("%d %d", &NSTATES, &NSYM);
	printf("Enter final states (comma separated): ");
	scanf("%s", final_s);
	rest = final_s;
	// getting the final states as string
	while ((token = strtok_r(rest, ",", &rest))) {	// use strtok_r to separate comma seperated values
		int mask=1;
		input = atoi(token);
		if(input == -1) continue;
		mask = mask << input;
		final |= mask;
	}
	// init the input table
	int table[NSTATES][NSYM], bit_tab[NSTATES][NSYM];
	memset(bit_tab, 0, sizeof(int)*NSTATES*NSYM); //reset table to 0
	for(i=0; i<NSTATES; ++i) {
		for (j=0; j<NSYM; ++j) {
			fscanf(fp, "%d", &input);	// read input from file
			table[i][j] = input;
			state2bit(input, &bit_tab[i][j]);
		}
	}
	// init the marked table
	int marked[NSTATES][NSTATES];
	memset(marked, 0, sizeof(int)*NSTATES*NSTATES); //reset table to 0
	for(i=0; i<NSTATES; ++i) {
		for (j=i; j<NSTATES; ++j)
			marked[i][j] = -1;
	}
	markfinal(final, marked);
	merged = minimize(marked, table);
	int m_states[merged];
	memset(m_states, 0, sizeof(int)*merged);
	for(i=0,k=0; i<NSTATES; ++i)
		for(j=0; j<i; ++j)
			if(!marked[i][j]) {
				state2bit(i, &m_states[k]);
				state2bit(j, &m_states[k++]);
			}
	for (k=0; k<merged; ++k) {
		bit2state(m_states[k], final);
		for (i=0; i<NSTATES; ++i)
			if(1<<i & m_states[k]) break;
		for (j=0; j<NSYM; ++j) {
			for(l=0; l<merged; ++l)
				if(bit_tab[i][j]&m_states[l]) break;
			if(l!=merged) bit2state(m_states[l], final);
			else bit2state(bit_tab[i][j], final);
		}
		printf("\n");
	}
	for(i=0; i<NSTATES; ++i) {
		for (k=0; k<merged; ++k)
			if(1<<i & m_states[k]) break;
		if(k!=merged) continue;
		else bit2state(1<<i, final);
		for(j=0; j<NSYM; ++j) {
			for(k=0; k<merged; ++k)
				if(bit_tab[i][j]&m_states[k]) break;
			if(k!=merged) bit2state(m_states[k], final);
			else bit2state(bit_tab[i][j], final);
		}
		printf("\n");
	}
}