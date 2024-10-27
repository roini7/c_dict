# Dictionary Manager

This project is a C-based Dictionary Manager, allowing users to create and manage multiple dictionaries with dynamic memory allocation and linked lists for storing translations.

## Features

- **Create and Delete Dictionaries**: Add or remove dictionaries and manage memory allocation dynamically.
- **Add, Search, and Delete Words**: Add translations, search for words, and delete entries within a selected dictionary.
- **Memory Management**: Handles dynamic memory allocation and deallocation to prevent memory leaks.

## Project Structure

- **Dictionary**: Each dictionary contains a linked list of words and translations.
- **Word List**: Each word node contains translations across languages, stored in dynamically allocated arrays.

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/dictionary-manager.git
   cd dictionary-manager
2. Compile the program:  
   ```bash
    gcc dictionary_manager.c -o dictionary_manager
3. Run the program:  
    ```bash
    ./dictionary_manager
    
## Usage  

Upon running, select from menu options to create, manage, and delete dictionaries or translations.  
Add a New Dictionary: Choose option 1 to create a new dictionary.  
Add a Word: Choose option 2 to add a new translation to an existing dictionary.  
Delete a Word: Choose option 3 to remove a word from a selected dictionary.  
Find a Word: Choose option 4 to search for translations of a specific word.  
Delete a Dictionary: Choose option 5 to remove an entire dictionary.  
Exit Program: Choose option 6 to terminate the program, freeing all allocated memory.  

## Memory Management  

All data is dynamically allocated using pointers. The program handles memory allocation for dictionaries, word lists, and translations. When a  dictionary or word is deleted, the corresponding memory is freed to prevent leaks. Upon exiting the program, all allocated memory is released, ensuring  efficient memory management throughout the application.  
    
