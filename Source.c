/*
Name: Liam Holland
ID: 21386331
Date: 01-03-22
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void readDict();
void refresh();
void makeGuess();
int contains(char l, char arr[]);

char words[100000][8];	//dictionary
char word[8];			//random word for a given round
char used[26];			//the letters guessed by the user

int usedIndex, numWords, numGuesses = 0;	//index of the used array, number of words taken from dictionary, number of guesses made in a round

FILE* fptr;

void main() {
	readDict();

	srand(time(NULL));
	strcpy_s(word, 8, words[rand() % (numWords + 1)]);	//get the random word

	refresh();
}

void readDict() {
	fopen_s(&fptr, "dictionary.txt", "r");
	char line[100];

	if (fptr != NULL) {
		while (!feof(fptr)) {
			fgets(line, 100, fptr);
			int l = strlen(line);
			if (l >= 4 && l <= 7) {
				line[l - 1] = '\0';	//change the last character from a newline to an escape character
				strcpy_s(words[numWords], 8, line);
				numWords++;
			}
		}
	}
}

void refresh() {
	printf("Guesses: %d", numGuesses);	//displays number of guesses made
	for (int i = 0; i < 26; i++) {
		if (used[i] != NULL) {
			printf("\033[0;31m");	//set the colour to red
			if(contains(used[i], word))	//if the word contains a letter in the used array
				printf("\033[0;32m");	//change the colour to green

			if (i % 7 == 0) printf("\n");

			printf("\t%c", used[i]);	//print the character
		}
		else if (used[i] == NULL) break;	//stop looping if there are no more values
	}
	printf("\033[0;37m");	//set the colour back to white
	
	//displays the positions of the correctly guessed letters
	printf("\n");
	int dashes = 0;
	for (int i = 0; i < strlen(word); i++) {
		if (contains(word[i], used)) {	//if the used array contains a letter in the word
			printf("%c", word[i]);
		}
		else {
			printf("-");
			dashes++;
		}
	}
	printf("\n");

	if (dashes > 0) {
		makeGuess();
	}
	else
		printf("\nWord Guessed After %d Guesses!\n", numGuesses);
}

//takes a guess from the user
void makeGuess() {
	char guess[2];	//an array used instead of a character to avoid issues with scanf
	printf("Make a guess: ");

	scanf_s("%s", guess, 2);
	if (isalpha(guess[0])) {	//if the guess was a letter
		if (contains(guess[0], used)) {	//if the used array contains the guessed letter
			printf("Already Guessed\n");
			makeGuess();
		}
		else {
			used[usedIndex] = guess[0];	//add the guess to the used letters array
			usedIndex++;
			numGuesses++;
			system("cls");	//clear the screen - system dependant command
			refresh();
		}
	}
	else {
		printf("Invalid Guess\n");
		makeGuess();
	}
}

//checks if a given letter is in a given char array
int contains(char l, char arr[]) {
	for (int i = 0; i < strlen(arr); i++) {
		if (arr[i] != NULL && l == arr[i])
			return 1;
	}
	return 0;
}