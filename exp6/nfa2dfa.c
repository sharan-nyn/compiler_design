#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NSTATES, NSYM;
const char* alpha = "abcdefghijklmnopqrstuvwxyz";
typedef struct dfa_states dfas_t;
struct dfa_states {
	int state_num;
	int* next_state;
	struct dfa_states* next;
};

dfas_t* get_node(int num) {
	dfas_t* node = (dfas_t*) malloc(sizeof(dfas_t));
	node->state_num=num;
	node->next_state = (int *) malloc(NSYM*sizeof(int));
	memset(node->next_state, -1, NSYM*sizeof(int));
	node->next = NULL;
	return node;
}

void insert_node(dfas_t* head, dfas_t* node) {
	dfas_t* ptr;
	ptr = head;
	while (ptr->next != NULL && ptr->state_num != node->state_num) ptr = ptr->next;
	if (ptr->state_num!= node->state_num) ptr->next = node;
	else return;
}

void bit2state(int num) {
	int index = 0;
	printf("\t[");
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

void print_state(dfas_t* node) {
	int i;
	if (node->state_num == -1) printf("\tφ\t");
	else bit2state(node->state_num);

	for(i=0; i<NSYM; ++i) {
		if (node->next_state[i] == -1) printf("\tφ\t");
		else bit2state(node->next_state[i]);
	}
	printf("\n");
}

void nfa2dfa(dfas_t* head, dfas_t* node, int table[NSTATES][NSTATES][NSYM]) {
	int symbol, j;
	for (symbol=0; symbol<NSYM; ++symbol) {
		int state_num = node->state_num;
		int state = 0;
		int next_state = 0;
		while(state_num>0) {
			if(state_num&1)
				for (j=0; j<NSTATES; ++j)
					if(table[state][j][symbol]) state2bit(j, &next_state);
			state_num = state_num >> 1;
			state++;
		}
		if (!next_state) next_state = -1;
		dfas_t *new_node = get_node(next_state);
		insert_node(head, new_node); //insert new node into list, check if it already exists...
		node->next_state[symbol] = next_state; //set next state for symbol
	}
	if(node->next == NULL) return; //call on next node... stop on next = NULL
	else nfa2dfa(head, node->next, table);
}

void dfa_table(dfas_t* head) {
	dfas_t* ptr = head;
	printf("\n");
	while (ptr != NULL) {
		print_state(ptr);
		ptr = ptr->next;
	}
}

void print_final_states(dfas_t* head, int final) {
	int final_mask = 1;
	dfas_t* ptr = head;
	final_mask = final_mask << final;
	printf("\nFinal-states are:\n");
	while (ptr != NULL) {
		if(ptr->state_num > 0 && ptr->state_num & final_mask)
			bit2state(ptr->state_num);
		ptr = ptr->next;
	}
	printf("\n");
}

void main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Usage: ./a.out <input.txt>\n");
		return;
	}
	FILE *fp = fopen(argv[1],"r");
	char input[20];
	char* token;
	int i , j, final;
	printf("Enter number of states and number of symbols: ");
	scanf("%d %d", &NSTATES, &NSYM);
	printf("Enter the final state: ");
	scanf("%d", &final);
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
	dfas_t* head = get_node(1);
	nfa2dfa(head, head, table);
	dfa_table(head);
	print_final_states(head, final);
}