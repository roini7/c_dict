#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define DELIM ","

/***********************
* Roi Nir
* 206235228
* ex_5
***********************/

typedef struct Word {
	char** translations;
	struct Word* next;
} Word;

typedef struct {
	char** languages;
	int numOfLanguages;
	Word* wordList;
} Dictionary;

void menu(Dictionary* dictionaries, int numOfDictionaries);
char* getString();
void printDict(Dictionary* dictionaries, int numOfDict);
Dictionary* createDict(Dictionary* dictionaries, int numOfDict);
void createWord(Dictionary* dictionaries, int numOfDict, int *ptrIsInvalid);
void deleteWord(Dictionary* dictionaries, int numOfDict, int* ptrIsInvalid);
void searchWord(Dictionary* dictionaries, int numOfDict,int* ptrIsInvalid);
Dictionary* deleteElement(Dictionary* dictionaries, int position, int* ptrNumDict);
Dictionary* deleteDict(Dictionary* dictionaries, int numOfDict, int* ptrNumDict, int* ptrIsInvalid);
int checkWord(Dictionary* dictionaries, int dictPosition);
int checkOption(int option, int numOfDict);

/******************
* Function Name: printDict
* Input: Dictionary*, int
* Output: none
* Function Operation: prints the available dictionaries
******************/
void printDict(Dictionary* dictionaries, int numOfDict) {
	printf("Choose a dictionary:\n");
	// iterates over every dictionary and its matching langauges to print stored data
	for (int i = 0; i < numOfDict;i++) {
		printf("%d. ", i + 1);
		for (int j = 0; j < dictionaries[i].numOfLanguages; j++) {
			printf("%s", dictionaries[i].languages[j]);
			if (j < dictionaries[i].numOfLanguages - 1) {
				printf(",");
			}
		}
		printf("\n");
	}
}

/******************
* Function Name: getString
* Input: none
* Output: char*
* Function Operation: dynamically allocates a string from the user
******************/
char* getString() {
	char* dynamicArr = NULL;
	int arrSize = 1;
	// clear buffer
	while (getchar() != '\n');
	dynamicArr = malloc(sizeof(char));
	if (dynamicArr == NULL) {
		return NULL;
	}
	// feed first char from buffer to allocated array
	scanf("%c", &dynamicArr[0]);
	// loop until reaching '\n'
	while (dynamicArr[arrSize - 1] != '\n') {
		arrSize++;
		// set backup for dynamicArr in case of memory allocation failure
		char* temp = dynamicArr;
		// increment size of dynamicArr by one byte(char)
		dynamicArr = realloc(dynamicArr, arrSize * sizeof(char));
		// if allocation fails free the array built so far and return NULL
		if (dynamicArr == NULL) {
			free(temp);
			return NULL;
		}
		// feed next char from buffer until '\n'
		scanf("%c", &dynamicArr[arrSize - 1]);
	}
	// insert '\0' to end string
	dynamicArr[arrSize - 1] = '\0';
	return dynamicArr;
}

/******************
* Function Name: createDict
* Input: Dictionary*, int
* Output: Dictionary*
* Function Operation: create a new dictionary or add to an existing array of dictionaries
******************/
Dictionary* createDict(Dictionary* dictionaries, int numOfDict) { 
	int wordCounter = 0;
	// backup dictionaries address in case of memory allocation failure and return it
	Dictionary* temp = dictionaries;
	// reallocate memory for another dictionary or allocate memory for first dictionary
	dictionaries = realloc(dictionaries, sizeof(Dictionary) * (numOfDict + 1));
	if (dictionaries == NULL) {
		printf("The creation of the dictionary has failed!\n");
		return temp;
	}
	printf("Define a new dictionary:\n");
	// get a string from user and split it with strtok
	char* arr = getString();
	char* piece = strtok(arr, DELIM);
	dictionaries[numOfDict].languages = NULL;
	// keep looping if piece points to a string and point each pointer to matching string
	while (piece != NULL) {
		//  backup languages pointer array in case of memory allocation failure
		char** temp1 = dictionaries[numOfDict].languages;
		// add a language for each word in languages or allocate memory for first langauge
		dictionaries[numOfDict].languages = realloc(dictionaries[numOfDict].languages, sizeof(char*) * (wordCounter + 1));
		// check for memory allocation failure and free necessary memory blocks
		if (dictionaries[numOfDict].languages == NULL) {
			printf("The creation of the dictionary has failed!\n");
			free(temp1);
			temp = dictionaries;
			dictionaries = realloc(dictionaries, sizeof(Dictionary) * (numOfDict));
			if (dictionaries == NULL) {
				return temp;
			}
			return dictionaries;
		}
		// point language pointer to word in splited string in matching order
		dictionaries[numOfDict].languages[wordCounter] = piece;
		wordCounter++;
		// increment loop by pointing to next language
		piece = strtok(NULL, DELIM);
	}
	// set dictionary fields to valid values
	dictionaries[numOfDict].wordList = NULL;
	dictionaries[numOfDict].numOfLanguages = wordCounter;
	printf("The dictionary has been created successfully!\n");
	return dictionaries;
}

/******************
* Function Name: createDict
* Input: Dictionary*, int, int*
* Output: none
* Function Operation: add a word to a dictionary with it's translations
******************/
void createWord(Dictionary* dictionaries, int numOfDict, int* ptrIsInvalid) {
	int option, wordCounter = 0;
	// check for dictionaries to add words to
	if (numOfDict == 0) {
		printf("This option is not available right now, try again:\n");
		*ptrIsInvalid = 1;
		return;
	}
	// print all available dictionaries
	printDict(dictionaries,numOfDict);
	scanf("%d", &option);
	// check if user option is valid
	option = checkOption(option, numOfDict);
	// print languages from selected dictionary
	printf("Enter a word in ");
	for (int j = 0; j < dictionaries[option - 1].numOfLanguages; j++) {
		printf("%s", dictionaries[option - 1].languages[j]);
		if (j < dictionaries[option - 1].numOfLanguages - 1) {
			printf(",");
		}
	}
	printf(":\n");
	//create first node if word list is empty
	if (dictionaries[option - 1].wordList == NULL) { 
		// allocate memory for first node in list
		Word* newWord = (Word*)malloc(sizeof(Word));
		// check for memory allocation failure
		if (newWord == NULL) {
			printf("The addition of the word has failed!\n");
			return;
		}
		// set node to head
		newWord->next = NULL;
		dictionaries[option - 1].wordList = newWord;
		// get data(translations) for node
		char* arr = getString();
		// allocate memory for array of pointers(data) in node to point to translations
		newWord->translations = (char**)malloc(sizeof(char*));
		// check for memory allocation failure
		if (newWord->translations == NULL) {
			printf("The addition of the word has failed!\n");
			free(newWord);
			free(arr);
			dictionaries[option - 1].wordList = NULL;
			return;
		}
		// keep looping if piece points to a string and point each pointer to matching string
		char* piece = strtok(arr, DELIM);
		while (piece != NULL) {
			char** temp = newWord->translations;
			// add a translation for each word in translations or allocate memory for first translation
			newWord->translations = realloc(newWord->translations, sizeof(char*) * (wordCounter + 1));
			// check for memory allocation failure
			if (newWord->translations == NULL) {
				printf("The addition of the word has failed!\n");
				free(temp);
				free(arr);
				dictionaries[option - 1].wordList = NULL;
				return;
			}
			// point translation pointer to translation in splited string in matching order
			newWord->translations[wordCounter] = piece;
			// increment loop by pointing to next translation
			piece = strtok(NULL, DELIM);
			wordCounter++;
		}
		printf("The word has been added successfully!\n");
		return;
	//add a node if word list isn't empty
	} else {
		// allocate memory for node
		Word* newWord = (Word*)malloc(sizeof(Word));
		// check for memory allocation failure
		if (newWord == NULL) {
			printf("The addition of the word has failed!\n");
			return;
		}
		// point temp to head
		Word* temp = dictionaries[option - 1].wordList;
		// traverse the linked list nodes using temp
		while (temp->next != NULL) {
			temp = temp->next;
		}
		// link: point last node(temp) to new node
		temp->next = newWord;
		// set new node to be last node
		newWord->next = NULL;
		// get data(translations) for node
		char* arr = getString();
		// allocate memory for array of pointers(data) in node to point to translations
		newWord->translations = (char**)malloc(sizeof(char*));
		// check for memory allocation failure
		if (newWord->translations == NULL) {
			printf("The addition of the word has failed!\n");
			free(newWord);
			free(arr);
			temp->next = NULL;
			return;
		}
		// keep looping if piece points to a string and point each pointer to matching string
		char* piece = strtok(arr, DELIM);
		while (piece != NULL) {
			char** temp1 = newWord->translations;
			// add a translation for each word in translations or allocate memory for first translation
			newWord->translations = realloc(newWord->translations, sizeof(char*) * (wordCounter + 1));
			// check for memory allocation failure and free necessary memory blocks
			if (newWord->translations == NULL) {
				printf("The addition of the word has failed!\n");
				free(arr);
				free(temp1);
				free(newWord);
				temp->next = NULL;
				return;
			}
			// point translation pointer to translation in splited string in matching order
			newWord->translations[wordCounter] = piece;
			// increment loop by pointing to next translation
			piece = strtok(NULL, DELIM);
			wordCounter++;
		}
		printf("The word has been added successfully!\n");
		return;
	}
}

/******************
* Function Name: deleteWord
* Input: Dictionary*, int, int*
* Output: none
* Function Operation: delete a word from a dictionary
******************/
void deleteWord(Dictionary* dictionaries, int numOfDict, int* ptrIsInvalid) {
	int option, position =  1;
	int isFound = 0;
	char validation;
	// check for dictionaries to delete words from
	if (numOfDict == 0) {
		printf("This option is not available right now, try again:\n");
		*ptrIsInvalid = 1;
		return;
	}
	// print available dictionaries
	printDict(dictionaries, numOfDict);
	scanf("%d", &option);
	// check if user option is valid
	option = checkOption(option, numOfDict);
	printf("Enter a word in %s:\n", dictionaries[option - 1].languages[0]);
	// get word to delete from dictionary
	char* arr = getString();
	// check for memory allocation failure
	if (arr == NULL) {
		printf("The deletion of the word has failed!\n");
		free(arr);
		return;
	}
	//check for words in dictionary
	if (checkWord(dictionaries, option - 1)) {
		printf("The deletion of the word has failed!\n");
		free(arr);
		return;
	}
	// validate user choice
	printf("Are you sure? (y/n)\n");
	scanf(" %c",&validation);
	// if the user doesn't confirm choice cancel process
	if (validation != 'y') {
		printf("The deletion of the word has been canceled.\n");
		free(arr);
		return;
	}
	//traverse the linked list nodes and find word to delete
	Word* temp = dictionaries[option - 1].wordList;
	while (temp->next != NULL) {
		// compare each translation to given string
		if (strcmp(temp->translations[0],arr) == 0) {
			// set flag to true if found
			isFound = 1;
			break;
		}
		// update position of word and increment loop
		position++;
		temp = temp->next;
	}
	// if word wasn't found print failure and free arr
	if ((strcmp(temp->translations[0], arr) != 0) && isFound != 1) {
		printf("The deletion of the word has failed!\n");
		free(arr);
		return;
	}
	// if word is in first position move head to next node and free first node
	Word* temp1 = dictionaries[option - 1].wordList;
	if (position == 1) {
		dictionaries[option - 1].wordList = temp1->next;
		free(temp1->translations[0]);
		free(temp1->translations);
		free(temp1);
		free(arr);
		printf("The word has been deleted successfully!\n");
		return;
	}
	/*
	otherwise set temp1 to point to the node before the desirable node to delete
	and set temp2 to point to the desirable node to delete
	link temp1 to the node after temp2 and free temp1 data and node
	*/
	for (int i = 0;i < position - 2; i++) {
		temp1 = temp1->next;
	}
	Word* temp2 = temp1->next;
	temp1->next = temp2->next;
	free(temp2->translations[0]);
	free(temp2->translations);
	free(temp2);
	free(arr);
	printf("The word has been deleted successfully!\n");
	return;
}

/******************
* Function Name: searchWord
* Input: Dictionary*, int, int*
* Output: none
* Function Operation: search for word in a dictionary
******************/
void searchWord(Dictionary* dictionaries, int numOfDict, int *ptrIsInvalid) {
	int option, isFound = 0;
	// check for dictionaries to search words in
	if (numOfDict == 0) {
		printf("This option is not available right now, try again:\n");
		*ptrIsInvalid = 1;
		return;
	}
	// print available dictionaries
	printDict(dictionaries,numOfDict);
	scanf("%d", &option);
	// check if user option is valid
	option = checkOption(option, numOfDict);
	printf("Enter a word in %s:\n", dictionaries[option - 1].languages[0]);
	// get string to search in dictionary
	char* arr = getString();
	// check for memory allocation failure
	if (arr == NULL) {
		printf("The search has failed successfully!\n");
		free(arr);
		return;
	}
	// check if there are any words to search for in dictionary
	if (checkWord(dictionaries, option -1)) {
		printf("There are no translations for \"%s\" in this dictionary.\n", arr);
		free(arr);
		return;
	}
	Word* temp = dictionaries[option - 1].wordList;
	// traverse nodes and break out of loop if translation is found
	while (temp->next != NULL) {
		if (strcmp(temp->translations[0], arr) == 0) {
			// set flag to 1 if word is found
			isFound = 1;
			break;
		}
		temp = temp->next;
	}
	// search in last node as well
	if (strcmp(temp->translations[0], arr) == 0) {
		isFound = 1;
	}
	// if no translations are found free necassary memory block and return 
	if (!isFound) {
		printf("There are no translations for \"%s\" in this dictionary.\n",arr);
		free(arr);
		return;
	}
	// print translations for searched word in dictionary
	printf("The translations are:\n");
	for (int i = 1;i < dictionaries[option - 1].numOfLanguages;i++) {
		printf("%s: %s",dictionaries[option - 1].languages[i], temp->translations[i]);
		if (i < dictionaries[option - 1].numOfLanguages - 1) printf(", ");
	}
	printf("\n");
	free(arr);
	return;
}

/******************
* Function Name: deleteElement
* Input: Dictionary*, int, int*
* Output: Dictionary*
* Function Operation: delete an element from an array
******************/
Dictionary* deleteElement(Dictionary* dictionaries, int position, int* ptrNumDict) {
	// point all elements of array from 'position' to their successor
	for (int i = position; i < *ptrNumDict - 1; i++) {
		dictionaries[i] = dictionaries[i + 1];
	}
	// decrement number of dictionaries by one
	*ptrNumDict -= 1;
	// if there is one dictionary to delete free it
	if (*ptrNumDict == 0) {
		free(dictionaries);
		dictionaries = NULL;
		printf("The dictionary has been deleted successfully!\n");
		return dictionaries;
	}
	// if there is more than one dictionary reduce size of dictionary array by one
	Dictionary* temp = dictionaries;
	dictionaries = realloc(dictionaries, sizeof(Dictionary) * (*ptrNumDict));
	// in case of allocation failure the final two dictionaries of the array will point to the same dictionary
	if (dictionaries == NULL) {
		dictionaries = temp;
		printf("The deletion of the dictionary has failed!\n");
	} else {
		printf("The dictionary has been deleted successfully!\n");
	}
	return dictionaries;
}

/******************
* Function Name: deleteDict
* Input: Dictionary*, int, int*, int*
* Output: Dictionary*
* Function Operation: delete a dictionary
******************/
Dictionary* deleteDict(Dictionary* dictionaries, int numOfDict, int* ptrNumDict, int* ptrIsInvalid) { //delete a dictionary
	int option;
	char validation;
	// check for dictionaries to delete
	if (*ptrNumDict == 0) {
		printf("This option is not available right now, try again:\n");
		*ptrIsInvalid = 1;
		return dictionaries;
	}
	// print available dictionaries to choose from
	printDict(dictionaries, numOfDict);
	scanf("%d",&option);
	// check for valid option from user
	option = checkOption(option, numOfDict);
	printf("Are you sure? (y/n)\n");
	scanf(" %c",&validation);
	// if user doesn't confirm deletion of dictionary then cancel proccess
	if (validation != 'y') {
		printf("The deletion of the dictionary has been canceled.\n");
		return dictionaries;
	}
	// if there are no translations in selected dictionary
	if (dictionaries[option - 1].wordList == NULL) {
		// free languages strings then pointer array to said strings
		free(dictionaries[option - 1].languages[0]);
		free(dictionaries[option - 1].languages);
		// call deleteElement from array and delete dictionary
		dictionaries = deleteElement(dictionaries, option - 1, ptrNumDict);
	//otherwise if there are translations in selected dictionary
	} else {
		Word* headTemp = dictionaries[option - 1].wordList;
		/*
		set head node to be next node
		traverse word list and free translations->pointers_to_translations->nodes in said order
		repeat proccess untill all nodes are freed
		*/
		while (headTemp->next != NULL) {
			dictionaries[option - 1].wordList = headTemp->next;
			free(headTemp->translations[0]);
			free(headTemp->translations);
			free(headTemp);
			headTemp = dictionaries[option - 1].wordList;
		}
		// free last node and languages in dictionary
		free(headTemp->translations[0]);
		free(headTemp->translations);
		free(dictionaries[option - 1].wordList);
		free(dictionaries[option - 1].languages[0]);
		free(dictionaries[option - 1].languages);
		// call deleteElement from array and delete dictionary
		dictionaries = deleteElement(dictionaries, option - 1, ptrNumDict);
	}
	return dictionaries;
}

/******************
* Function Name: freeAll
* Input: Dictionary*, int*
* Output: Dictionary*
* Function Operation: free all data stored in heap and quit program
******************/
Dictionary* freeAll(Dictionary* dictionaries, int* ptrNumOfDict) {
	Word* headTemp = NULL;
	char** languages = NULL;
	// if there are no dictionaries then job well done
	if (*ptrNumOfDict == 0) {
		return dictionaries;
	}
	// otherwise there is work to be done
	else {
		int numOfDict = *ptrNumOfDict;
		// itterate over all allocated dictionaries
		for (int i = 0; i < numOfDict; i++) {
			headTemp = dictionaries[i].wordList;
			languages = dictionaries[i].languages;
			// if there are translations(nodes) in i'th dictionary
			if (headTemp != NULL) {
				/*
				set head node to be next node
				traverse word list and free translations->pointers_to_translations->nodes in said order
				repeat proccess untill all nodes are freed in i'th dictionary
				*/
				while (headTemp->next != NULL) {
					dictionaries[i].wordList = headTemp->next;
					free(headTemp->translations[0]);
					free(headTemp->translations);
					free(headTemp);
					headTemp = dictionaries[i].wordList;
				}
				// free last node and it's translations in i'th dictionary
				free(headTemp->translations[0]);
				free(headTemp->translations);
				free(headTemp);
			}
			// free i'th dictionary string of languages and pointer array to languages in said order
			free(languages[0]);
			free(languages);
		}
		// free all allocated dictionaries
		free(dictionaries);
	}
	// set dictionaries to NULL
	dictionaries = NULL;
	return dictionaries;
}

/******************
* Function Name: checkWord
* Input: Dictionary*, int
* Output: int
* Function Operation: check if there are any words(nodes) in given dictionary
******************/
int checkWord(Dictionary* dictionaries, int dictPosition) {
	Word* temp = dictionaries[dictPosition].wordList;
	// if there are words(nodes) in dictionary then return true
	if (temp == NULL) {
		return 1;
	}
	// otherwise false
	return 0;
}

/******************
* Function Name: checkOption
* Input: int, int
* Output: int
* Function Operation: check for a valid input by user of type int
******************/
int checkOption(int option, int numOfDict) {
	// if chosen option is bigger than or smaller than available dictionaries then repeat until valid input
	while (option > numOfDict || option <= 0) {
		printf("This option is not available right now, try again:\n");
		scanf("%d", &option);
	}
	// return chosen option
	return option;
}

/******************
* Function Name: menu
* Input: Dictionary*, int
* Output: none
* Function Operation: provide a list of options for the user to choose from
******************/
void menu(Dictionary* dictionaries, int numOfDictionaries) {
	// initialise variables and pointers to addresses
	int* ptrNumOfDict = &numOfDictionaries;
	int option = 0;
	// set flag to menu print
	int isInvalid = 0;
	int* ptrIsInvalid = &isInvalid;
	while (1) {
		// if flag is triggered then don't print menu
		if(!isInvalid){
			printf("Welcome to the dictionaries manager!\n"
				"Choose an option:\n"
				"1. Create a new dictionary.\n"
				"2. Add a word to a dictionary.\n"
				"3. Delete a word from a dictionary.\n"
				"4. Find a word in a dictionary.\n"
				"5. Delete a dictionary.\n"
				"6. Exit.\n");
		}
		// set flag to zero after each action preformed by the user
		isInvalid = 0;
		scanf("%d", &option);
		// call chosen functions in corilation with menu options
		if (option == 1) {
			dictionaries = createDict(dictionaries, numOfDictionaries);
			numOfDictionaries++;
		} else if (option == 2) {
			createWord(dictionaries, numOfDictionaries, ptrIsInvalid);
		} else if (option == 3) {
			deleteWord(dictionaries,numOfDictionaries, ptrIsInvalid);
		} else if (option == 4) {
			searchWord(dictionaries, numOfDictionaries, ptrIsInvalid);
		} else if (option == 5) {
			dictionaries = deleteDict(dictionaries, numOfDictionaries, ptrNumOfDict, ptrIsInvalid);
		} else if (option == 6) {
			// free all allocated memory in heap and quit program
			dictionaries = freeAll(dictionaries, ptrNumOfDict);
			printf("Bye!\n");
			break;
		} else {
			// ask for valid input until given
			printf("Wrong option, try again:\n");
			// don't print menu in next iteration
			isInvalid = 1;
		}
	}
}

int main() {
	Dictionary* dictionaries = NULL;
	int numOfDictionaries = 0;
	menu(dictionaries, numOfDictionaries);
	return 0;
}