#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points( t_point *points, int num_point);

/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments( t_line *lines, int num_line);

// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull
t_line *convex_hull( t_point *points, int num_point, int *num_line);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line; // number of lines
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
		
	t_line *lines;

	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	lines = convex_hull( points, num_point, &num_line);
	
	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}

void print_points( t_point *points, int num_point){
	printf("\n\n#points\n");
    for(int i=0; i<num_point; i++){
		printf("points(%d,%d)\n", points[i].x, points[i].y);
	}
};

//segments(7107,2909,7107,2909)
void print_line_segments( t_line *lines, int num_line){
	printf("\n\n#line segments\n");
    for(int i=0; i<num_line; i++){
		printf("segments(%d,%d,%d,%d)\n", lines[i].from.x, lines[i].from.y, lines[i].to.x, lines[i].to.y);
	}
};

t_line *convex_hull( t_point *points, int num_point, int *num_line){
	
	t_line *saves = (t_line *)malloc(sizeof( t_line));
	long long int min_l = RANGE * RANGE;
	int l = 0;

	for(int i=0; i<num_point; i++){
        l = points[i].x * points[i].x + points[i].y * points[i].y;
        if (l < min_l){
            min_l = l;
            saves[0].from = points[i];
        }
	}
	
	int a = 0;
    int b = 0;
    int c = 0;
	int checking[3] = {0,0,-1};
	int cdex = 0;
	int index = 0;

	
	while(index<num_point){
		
		if(cdex != 0){
			if ( points[index].x == saves[cdex-1].from.x && points[index].y == saves[cdex-1].from.y) {
				index = index+1;
			}
			saves = (t_line *) realloc(saves, (cdex+1)*sizeof( t_line));
			saves[cdex].from.x = saves[cdex-1].to.x;
			saves[cdex].from.y = saves[cdex-1].to.y;
		}
		
		a = points[index].y - saves[cdex].from.y;
		b = (-1) * (points[index].x - saves[cdex].from.x);
		c = (saves[cdex].from.x) * a + saves[cdex].from.y * b;
		checking[0] = 0; 
		checking[1] = 0; 
		checking[2] = -1;

		for(int put=0; put<num_point; put++){
			if  (a * points[put].x + b * points[put].y > c){
				checking[0]=1;
			}
			else if(a * points[put].x + b * points[put].y < c){
				checking[1]=1;
			}
			else{
				if (a * points[put].x + b * points[put].y == c && put != index){
					if ( points[put].x != saves[cdex].from.x 
					&& points[put].y != saves[cdex].from.y) {checking[2]=put;}
				}
			}
		}

		if (checking[0]+checking[1] == 1){
			saves[cdex].to = points[index];
			if(checking[2]!=-1){
				int l1 = (saves[cdex].from.x - saves[cdex].to.x) * (saves[cdex].from.x - saves[cdex].to.x) + (saves[cdex].from.y - saves[cdex].to.y) * (saves[cdex].from.y - saves[cdex].to.y);
				int l2 = (saves[cdex].from.x - points[checking[2]].x) * (saves[cdex].from.x - points[checking[2]].x) + (saves[cdex].from.y - points[checking[2]].y) * (saves[cdex].from.y - points[checking[2]].y);
				
				if (l1 < l2) {cdex--; index++;}
				else index = 0;
			}
			else index = 0;

			cdex++;
			
		}
		else index++;

		if (saves[cdex-1].to.x == saves[0].from.x && saves[cdex-1].to.y == saves[0].from.y) break;
	}

	*num_line = cdex;
	
	return saves;

};