/*
Name: Liam Holland
ID: 21386331
Date: 08-03-22
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

char words[1000][50];
FILE* f;

void main() {
	int numWords = 0;		//number of words
	int numSyllables = 0;	//number of syllables
	int numSentences = 0;	//number of sentences
	int wordPos = 0;		//position in the word being constructed
	int i = 0;

	char word[50];			//word currently being constructed

	//read words/count words/count sentences
	fopen_s(&f, "article-irish-times.txt", "r");
	while (!feof(f)) {
		char c = fgetc(f);	//get the next character
		if (isalnum(c) || (c == '\'' && wordPos != 0)) {	//if the character is alphanumeric or its an ' in a word
			word[wordPos] = c;								//add it to the current word
			wordPos++;
		}
		else if (wordPos != 0) {	//if its not alphanumeric and we're currently in a word
			word[wordPos] = '\0';	//end the word
			wordPos = 0;
			strcpy_s(words[numWords], 50, word);	//put the word in the words array
			numWords++;		//count the word

			if (strchr(".:;?!", c) != NULL)	//if the character that ended the word is a sentence character
				numSentences++;				//count the sentence
		}

		i++;
	}
	fclose(f);

	int prevVowel = 0;	//boolean variable
	int localSyl = 0;	//numer of syllables in a given word

	//count the syllables
	for (int j = 0; j < numWords + 1; j++) {		//for each word j
		for (int k = 0; k < strlen(words[j]); k++) {	//for each letter k in word j
			
			//if the letter is a vowel, not following a vowel, and isnt an e at the end of a word
			if (strchr("aeiouy", tolower(words[j][k])) != NULL && !prevVowel && !(tolower(words[j][k]) == 'e' && k == strlen(words[j]) - 1)) {
				numSyllables++;		//count the syllable
				localSyl++;			//count the local syllables
				prevVowel = 1;		//do not count the next letter if it is a vowel
			}
			else
				prevVowel = 0;		//count the next letter if it is a vowel
		}

		if (localSyl == 0) {	//if there were no recorded syllables
			numSyllables++;		//count one syllable
		}
		localSyl = 0;	//reset local syllables
	}

	printf("%f", 206.835 - (84.6 * ((double)numSyllables / (double)numWords)) - (1.015 * ((double)numWords / (double)numSentences)));
	printf("\nnumWords: %d\nnumSentences: %d\nnumSyllables: %d", numWords, numSentences, numSyllables);
}