/*
Name: Liam Holland
ID: 21386331
Date: 18-01-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int shipX, shipY;
int gridSize = 9;	//grid size set to 9 as the last digit of my ID is 1

void randomSearch(int gs);					//random search function
void gridSearch(int gs, int posChecked);	//sequential search function
void printGrid(int gs);						//print out the grid along with the location of the ship

void main() {
	//initialise the coordinates of the ship
	srand(time(NULL) * (gridSize - 1));
	shipX = rand() % gridSize;
	shipY = rand() % gridSize;

	puts("----------Random Search------------\n");
	randomSearch(gridSize);

	puts("----------Grid Search----------\n");
	gridSearch(gridSize, 0);

	printGrid(gridSize);
}

void randomSearch(int gs) {
	int x = 0, y = 0, i = 0;
	srand(time(NULL) * (gs));

	//finds random coordinates until they both match the ship coordinates
	while (x != shipX || y != shipY) {
		if (x != shipX)
			x = rand() % gs;
		if (y != shipY)
			y = rand() % gs;
		i++;
	}

	printf("Ship found after %d locations!\n", i);
	printf("The ship coordinates were (%d, %d)\n", x, y);
}

void gridSearch(int gs, int posChecked) {
	int x = posChecked % gs;		//gets the x coordinate between 0 and gridSize - 1
	int y = (posChecked - x) / gs;	//gets the y coordinate.In an nxn grid, the y will be the number of positions checked, minus the x coordinate (excess) divided by the grid size

	//exit condition
	if (x == shipX && y == shipY) {
		printf("Ship found after %d locations!\n", posChecked);
		printf("The Ships coordinates were (%d, %d)\n\n", x, y);
	}
	else {
		gridSearch(gs, posChecked + 1);	//recursion
	}
}

void printGrid(int gs) {
	for (int i = gs; i > 0; i--) {
		for (int j = gs; j > 0; j--) {
			//checks if the currently printing position matches the coordinates of the ship
			if ((i - 1) == shipY && (gs - j) == shipX) {
				printf("|");
				printf("\033[0;31m");	//sets text colour to red
				printf("*");
				printf("\033[0m");		//sets text colour to default
			}
			else
				printf("|_");
		}
		printf("|\n");	//adds the missing line at the end of each row
	}
}