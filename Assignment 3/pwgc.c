#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF	0x04
#define GOAT	0x02
#define CABBAGE	0x01

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename( FILE *fp, int state);

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc( int state, int *p, int *w, int *g, int *c);

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end( int state);

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
static int is_possible_transition( int state1,	int state2);

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP( int state);

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW( int state);

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG( int state);

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1 
static int changePC( int state);

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited( int visited[], int depth, int state);

// 방문한 경로(상태들)을 차례로 화면에 출력
static void print_path( int visited[], int depth);

// recursive function
static void dfs_main( int initial_state, int goal_state, int depth, int visited[]);

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix( int graph[][16]);

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num);

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph( char *filename, int graph[][16], int num);

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int initial_state, int goal_state)
{
	int depth = 0;
	int visited[16] = {0,}; // 방문한 정점을 저장
	
	dfs_main( initial_state, goal_state, depth, visited); 
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int graph[16][16] = {0,};
	
	// 인접 행렬 만들기
	make_adjacency_matrix( graph);

	// 인접 행렬 출력 (only for debugging) <- 이 값은 넣지 말기
	//print_graph( graph, 16);
	
	// .net 파일 만들기
	save_graph( "pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search( 0, 15); // initial state, goal state
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void get_pwgc( int state, int *p, int *w, int *g, int *c){

	if(state > 7){ (*p)=1; }
	else {(*p)=0;}

	if(state % 2 == 0) { (*c)=0; }
	else (*c)=1;

	if(state > 3 && state < 8) { (*w)=1; }
	else if(state > 11) { (*w)=1; }
	else{ (*w)=0;}

	if(state == 2 || state == 3 || state == 6 || state == 7 || state == 10 || state == 11 || state == 14 || state == 15 ){
		(*g)=1;
	}
	else (*g)=0;

};

static int is_dead_end( int state){
	
	if (state == 3 || state == 12) return 1;

	if (state == 6 || state == 7 || state == 8 || state == 9) return 1;

	return 0;
};

static int is_possible_transition( int state1,	int state2){
	
	if(state1 == state2) return 0;
	if( is_dead_end(state1) == 1 ) return 0;
	if( is_dead_end(state2) == 1 ) return 0;
	/*
	switch(state1){
		case 0 : { if ( state2 == 10) return 1; else return 0; } 
		case 1 : { if ( state2 == 11 || state2 == 13) return 1; else return 0; }
		case 2 : { if (state2 == 10 || state2 == 11 || state2 == 14) return 1; else return 0; }
		case 3 : { return 0; }
		case 4 : { if (state2 == 13 || state2 == 14) return 1; else return 0; }
		case 5 : { if (state2 == 13 || state2 == 15) return 1; else return 0; }
		case 6 : { return 0; }
		case 7 : { return 0; }
		case 8 : { return 0; } 
		case 9 : { return 0; } 
		case 10 : { if (state2 == 0 || state2 == 2) return 1; else return 0;  }
		case 11 : { if (state2 == 1 || state2 == 2) return 1; else return 0; }
		case 12 : { return 0; }
		case 13 : { if (state2 == 1 || state2 == 4 || state2 == 5) return 1; else return 0; }
		case 14 : { if (state2 == 2 || state2 == 4) return 1; else return 0; }
		case 15 : { if (state2 == 5) return 1; else return 0; }

		default : return 0;
	}
	*/
	int p = 0;
	int w = 0;
	int g = 0;
	int c = 0;
	get_pwgc(state1, &p, &w, &g, &c);

	int p1 = 0; 
	int w1 = 0;
	int g1 = 0; 
	int c1 = 0;
	get_pwgc(state2, &p1, &w1, &g1, &c1);

	if(p == p1) return 0; 

	int check = 0;
	int m1=0; int m2=0; int m3 = 0;

	if ( w != w1) {check++; m1 = 3;}

	if ( g != g1) {check++; m2 = 3;}

	if ( c != c1) {check++; m3 = 3;}

	if (check==0) return 1;
	else if (check==1){
		if(m1 != 0) {
			if(p == w) return 1;
			//else return 0;
		}
		if(m2 != 0) {
			if(p == g) return 1;
			//else return 0;
		}
		if(m3 != 0) {
			if(p == c) return 1;
			//else return 0;
		}
	}
	else return 0;
	
};

// 상태 변경: 농부 이동
static int changeP( int state){

	if(state > 7) return state-8;
	else return state+8;

};

// 상태 변경: 농부, 늑대 이동
static int changePW( int state){

	int p, w, g, c = 0;
	p =0; w=0; g=0; c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p==0) p=1;
	else p=0;

	if(w==0) w=1;
	else w=0;

	int news = 0;
	news = p*8 + w*4 + g*2 + c;

	if (is_possible_transition(state, news) == 1) return news;
	else return -1;

};

// 상태 변경: 농부, 염소 이동
static int changePG( int state){
	
	int p, w, g, c = 0;
	p =0; w=0; g=0; c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p==0) p=1;
	else p=0;

	if(g==0) g=1;
	else g=0;

	int news = 0;
	news = p*8 + w*4 + g*2 + c;

	if (is_possible_transition(state, news) == 1) return news;
	else return -1;

};

// 상태 변경: 농부, 양배추 이동
static int changePC( int state){

	int p, w, g, c = 0;
	p =0; w=0; g=0; c=0;
	get_pwgc(state, &p, &w, &g, &c);

	if(p==0) p=1;
	else p=0;

	if(c==0) c=1;
	else c=0;

	int news = 0;
	news = p*8 + w*4 + g*2 + c;

	if (is_possible_transition(state, news) == 1) return news;
	else return -1;

};

// 주어진 state가 이미 방문한 상태인지 검사
static int is_visited( int visited[], int depth, int state){
	int p, w, g, c = 0;
	p=0; w=0; g=0; c=0;
	for(int i=0; i<depth; i++){
		if(visited[i] == state){
			get_pwgc(state, &p, &w, &g, &c);
			printf("\tnext state <%d%d%d%d> has been visited\n", p, w, g, c);
			return 1;
		}
	}
	
	return 0;
};

void print_graph( int graph[][16], int num){
	for(int i=0; i<num; i++){
		for(int j=0; j<16; j++){
			printf("%d\t", graph[i][j]);
		}
		printf("\n");
	}
	printf("\n");
};

static void print_path( int visited[], int depth){
	int p, w, g, c;
	p=0; w=0; g=0; c=0;
	printf("Goal-state found!\n");
	for(int i=0; i<depth; i++){
		p=0; w=0; g=0; c=0;
		get_pwgc(visited[i], &p, &w, &g, &c);
		printf("<%d%d%d%d>\n", p, w, g, c);
	}
	printf("\n");
};

static void dfs_main( int initial_state, int goal_state, int depth, int visited[]){

	int p, w, g, c;
	p=0; w=0; g=0; c=0;
	get_pwgc(initial_state, &p, &w, &g, &c);
	printf("current state is <%d%d%d%d> (depth %d)\n", p, w, g, c, depth);
	
	if(initial_state == goal_state) {
		visited[++depth] = goal_state;
		print_path(visited, depth);
		return;
	}

	int a, s, d, f;
	a=0; s=0; d=0; f=0;
	get_pwgc(initial_state, &a, &s, &d, &f);

	int a1, s1, d1, f1;
	a1=0; s1=0; d1=0; f1=0;

	if(is_dead_end(changeP(initial_state))==1){
		if(a==0) a1=1;
		else a1=0;
		printf("	next state <%d%d%d%d> is dead-end\n", a1, s, d, f);
	}
	else{
		int next = changeP(initial_state);
		if(is_visited( visited, depth, next)==0){
			visited[++depth] = next;
			dfs_main(next, goal_state, depth, visited);
			depth--;
			printf("back to <%d%d%d%d> (depth %d)\n", a, s, d, f, depth);
		};
	}

	if(changePW(initial_state) == -1){
		if(a==0) a1=1;
		else a1=0;

		if(s==0) s1=1;
		else s1=0;
		
		if(a==s) printf("	next state <%d%d%d%d> is dead-end\n", a1, s1, d, f);
	}
	else{
		int next = changePW(initial_state);
		if(is_visited( visited, depth, next)==0){
			visited[++depth] = next;
			dfs_main(next, goal_state, depth, visited);
			depth--;
			printf("back to <%d%d%d%d> (depth %d)\n", a, s, d, f, depth);
		}
	}

	if(changePG(initial_state)==-1){
		if(a==0) a1=1;
		else if(a==1) a1=0;

		if(d==0) d1=1;
		else if(d==1) d1=0;

		if(a==d)printf("	next state <%d%d%d%d> is dead-end\n", a1, s, d1, f);
	}
	else{
		int next = changePG(initial_state);
		if(is_visited( visited, depth, next)==0){
			visited[++depth] = next;
			dfs_main(next, goal_state, depth, visited);
			depth--;
			printf("back to <%d%d%d%d> (depth %d)\n", a, s, d, f, depth);
		}
	}

	if(changePC(initial_state)==-1){
		if(a==0) a1=1;
		else if(a==1) a1=0;

		if(f==0) f1=1;
		else if(f==1) f1=0;
		if(a==f) printf("	next state <%d%d%d%d> is dead-end\n", a1, s, d, f1);
	}
	else{
		int next = changePC(initial_state);

		if(is_visited( visited, depth, next)==0){
			visited[++depth] = next;
			dfs_main(next, goal_state, depth, visited);
			depth--;
			printf("back to <%d%d%d%d> (depth %d)\n", a, s, d, f, depth);
		}
	}

};

void make_adjacency_matrix( int graph[][16]){
	for(int i=0; i<16; i++){
		for(int j=0; j<16; j++){
			if(is_possible_transition(i, j) == 1) { graph[i][j] = 1; }
		}
	}
};

/////////////////////////////////////////////////////////

static void print_statename( FILE *fp, int state){

	switch(state){
		case 0 : fprintf(fp, "1 \"<0000>\""); break;
		case 1 : fprintf(fp, "2 \"<0001>\""); break;
		case 2 : fprintf(fp, "3 \"<0010>\""); break;
		case 3 : fprintf(fp, "4 \"<0011>\""); break;
		case 4 : fprintf(fp, "5 \"<0100>\""); break;
		case 5 : fprintf(fp, "6 \"<0101>\""); break;
		case 6 : fprintf(fp, "7 \"<0110>\""); break;
		case 7 : fprintf(fp, "8 \"<0111>\""); break;
		case 8 : fprintf(fp, "9 \"<1000>\""); break;
		case 9 : fprintf(fp, "10 \"<1001>\""); break;
		case 10 : fprintf(fp, "11 \"<1010>\""); break;
		case 11 : fprintf(fp, "12 \"<1011>\""); break;
		case 12 : fprintf(fp, "13 \"<1100>\""); break;
		case 13 : fprintf(fp, "14 \"<1101>\""); break;
		case 14 : fprintf(fp, "15 \"<1110>\""); break;
		case 15 : fprintf(fp, "16 \"<1111>\""); break;
	}
	
};

void save_graph( char *filename, int graph[][16], int num){
	
	FILE *fp;
	fp = fopen( (filename) , "w+");
	
	fprintf(fp, "*Vertices %d\n", num);

	for(int i = 0; i<num; i++){
		print_statename( fp, i);
		fprintf(fp, "\n");
	}
	
	fprintf(fp, "*Edges\n");
	for(int i = 0; i<num/2; i++){
		for(int j=0; j<num; j++){
			if(graph[i][j] == 1) fprintf(fp, "  %d  %d\n", i+1 , j+1);
		}
	}
	
	fclose(fp);
};
