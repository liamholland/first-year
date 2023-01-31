/*
Name: Liam Holland
ID: 21386331
Date: 29-03-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//location struct
typedef struct {
	int id, n, s, e, w, in, out;
	char description[101];
} location;

typedef struct {
	char name[15];
	int loc;
	char des[50];
} item;

typedef enum {
	N, S, E, W, IN, OUT,
	LOOK, QUIT, HELP,
	INVENTORY, TAKE, DROP, EXAMINE,
	ERROR
} command;

location locations[10];	//array for locations
item items[50];		//array for items
int numItems;		//number of items scanned in
char itemName[15];	//variable for the item entered by the user
FILE* f;
char commands[][10] = { "n", "s", "e", "w", "in", "out", "look", "quit", "help", "inventory", "take", "drop", "examine" };

void readData();
void input(location* currL, location* oldL);
void lower(char arr[]);
void inv(location* l, int mode, char name[]);
int parseCom(char input[]);

#define INSTRUCTIONS "Enter a direction (N, S, E, W, IN, OUT), LOOK to look around, HELP for instructions, or QUIT to quit\nItem commands are TAKE, DROP, EXAMINE followed by the name of the item\nINVENTORY to see what's in your inventory"

void main() {
	readData();		//read in the data to the structs
	puts(INSTRUCTIONS);
	input(&locations[1], &locations[1]);	//take user input
}

//read in data from the files
void readData() {
	fopen_s(&f, "adventure_locations.txt", "r");
	char line[101];
	int i = 1;	//start at 1 to match up with the ids
	if (f != NULL) {
		while (!feof(f)) {
			fgets(line, 101, f);

			if (isdigit(line[0])) {		//isdigit() check skips the first two header lines
				location* l = &locations[i];	//pointer to make the scanning simpler

				sscanf_s(line, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%[^\n]",
					&l->id, &l->n, &l->s, &l->e, &l->w, &l->in, &l->out, l->description, 101);	//sscanf_s scans the data into the variables

				i++;
			}
		}
		fclose(f);
	}

	fopen_s(&f, "adventure_objects.txt", "r");
	i = -2;		//-2 so the first two lines can be skipped
	if (f != NULL) {
		while (!feof(f)) {
			fgets(line, 101, f);
			if (i >= 0) {	
				item* item = &items[i];
				sscanf_s(line, "%[^\t]\t%d\t%[^\n]", item->name, 15, &item->loc, item->des, 50);
				numItems++;
			}
			i++;
		}
		fclose(f);
	}
}

//take user input
void input(location* currL, location* oldL) {
	if (currL != NULL && currL != &locations[0]) {	//check if the location is valid
		
		char inp[50];	//input variable

		printf("\n%s\n", currL->description);	//print the description of the current location
		inv(currL, 0, NULL);	//check for and print any items in the area

		scanf_s("%s", inp, 50);
		lower(inp);

		//check what command was entered
		switch (parseCom(inp)) {
		case N:
			input(&locations[currL->n], currL);		//go north
			break;
		case S:
			input(&locations[currL->s], currL);		//go south
			break;
		case E:
			input(&locations[currL->e], currL);		//go east
			break;
		case W:
			input(&locations[currL->w], currL);		//go west
			break;
		case IN:
			input(&locations[currL->in], currL);	//go in
			break;
		case OUT:
			input(&locations[currL->out], currL);	//go out
			break;
		case LOOK:
			input(currL, oldL);		//look around - reprint location
			break;
		case QUIT:
			puts("Quitting...");	//quit the program
			break;
		case HELP:
			puts(INSTRUCTIONS);		//help - print out the instructions
			input(currL, oldL);
			break;
		case INVENTORY:
			inv(currL, 4, NULL);	//display the inventory
			input(currL, oldL);
			break;
		case TAKE:
			inv(currL, 1, itemName);	//take an item
			input(currL, oldL);
			break;
		case DROP:
			inv(currL, 2, itemName);	//drop an item
			input(currL, oldL);
			break;
		case EXAMINE:
			inv(currL, 3, itemName);	//examine an item
			input(currL, oldL);
			break;
		default:
			puts("Invalid Command");
			input(oldL, oldL);	//run function again with the last location that worked
			break;
		}
	}
	else {
		puts("Can't go that way...");
		input(oldL, oldL);	//run function again with the last location that worked
	}
}

//parse the command into an int
command parseCom(char input[]) {
	char curr[15] = "";		//name variable

	scanf_s("%[^\n]s", curr, 15);	//get the remaining part of the entered directive
	
	if (strlen(curr) > 1) {		//if it is a relevent entry
		for (int i = 0; i < strlen(curr); i++)	//this is to remove the extra space at the start of the text
			curr[i] = curr[i + 1];		//swap the letters to one before
		curr[strlen(curr)] = '\0';	//replace the end with a \0
	}
	strcpy_s(itemName, 15, curr);	//set the item name as the name of the item they are trying to interact with
	
	for (int i = 0; i < 13; i++) {
		if (!strcmp(input, commands[i])) {
			return (command)i;
		}
	}

	return ERROR;
}

//inventory function - Modes 0 through 4 perform different functions on items
void inv(location* l, int mode, char name[]) {
	for (int i = 0; i < numItems; i++) {
		item* p = &items[i];	//item pointer

		//carry out the relevent operations based on the mode it was called in
		switch (mode){
		case 0:			//Mode 0 - Used to print out descriptions of items if they are in the vicinity
			if(p->loc == l->id)
				printf("There is a %s here\n", p->name);
			break;
		case 1:			//Mode 1 - Used to take items
			if (!strcmp(name, p->name) && p->loc == l->id) {
				p->loc = 0;		//position 0 in the locations used to represent the inventory
				printf("Took %s\n", p->name);
			}
			break;
		case 2:			//Mode 2 - Used to drop items
			if (!strcmp(name, p->name) && p->loc == 0) {
				p->loc = l->id;
				printf("Dropped %s\n", p->name);
			}
			break;
		case 3:			//Mode 3 - Used to examine an item
			if (!strcmp(name, p->name) && (p->loc == l->id || p->loc == 0))
				printf("%s\n", p->des);
			break;
		case 4:			//Mode 4 - Used to print items in the inventory
			if(p->loc == 0)
				printf("%s\n", p->name);
			break;
		default:
			puts("INVALID MODE");
			break;
		}
	}
}

//changes the input to lowercase
void lower(char arr[]) {
	for (int i = 0; i < strlen(arr); i++) {
		arr[i] = tolower(arr[i]);
	}
}
