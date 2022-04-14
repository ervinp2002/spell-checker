# Spell Checker Program

Designed by: Ervin Pangilinan

Semester: Spring 2022 (v1.0) April 8, 2022

Course: CSC 245 - Data Structures & Algorithms Analysis

Language Used: C++ 

## Objective
The goal of this project is to create a spell checker program. This program 
will be run from the commmand line where it will take input by specifying
a text file for the program to check. Afterwards, all misspelled words will be listed in alphabetical order in 
a separate text file. The reading from a dictionary and writing in a separate
text file is accomplished through the implementation of a BinarySearchTree class
and HashTable class.

## Command Line Syntax
    ./a.out [textfile] 
 
NOTE: The dictionary file and textfile must be in the same directory as the executable.

## Features
The program will parse each word in the text file as lowercase. When the program 
parses a word that is not found in the dictionary file, the user can pick from the
following options:

- Adding the word to the dictionary
- Ignoring the currently-parsed word, and all future instances of it
- Moving onto the next word
- Search for a suggested spelling
- Quit checking the file

## Sample Input
The specified text file that the program will parse from is specified as the
first command line argument, followed by the dictionary file. If a dictionary
file isn't specified, then the program will start with a blank dictionary. 

An example is listed below:

    03-25-21

    Thsi is a fiel that has soem spelling errors.

    Every tiem I hit the keys I egt into troulbe!

    don't worry abuot me thuogh,

    hte end!

## Sample Output
For each misspelled word, the following menu would be outputted:

    !@#$%^&*(){}    THE SPELL CHECKER PROGRAM       !@#$%^&*(){}

    thsi On Line 4 Was Not Found In Dictionary

    A) Add the Word To Dictionary
    I) Ignore Word, and Skip Future References
    G) Go On To Next Word
    S) Search For A Suggested Spelling
    Q) Quit Spell Checking File

    Selection: 

## OOP Design
Along with the implementation of a BinarySearchTree class and HashTable
class, the following are also included in the client program:

- iostream
- fstream
- cstdlib
- cctype
- string

## Program Outline
The program can be broken down into the following steps:

- Step 1: Taking input files
- Step 2: Parsing through each word
- Step 3: Options for misspelled words
- Step 4: Writing misspelled words to an external file

### Taking Input File
By specifying input file on the command line, the program can determine
which file is used as the text file. With a dictionary file present in the same directory, the HashTable for the dictionary will be filled with every word from the dictionary file. Without a dictionary file, the
program will start with an empty dictionary HashTable.

### Parsing Through Each Word
The input from the text file is read line-by-line. The program will start with an
empty string, and will concatenate each alphabetic character on the line or apostrophe 
until a space is reached. Upon reaching the space, the program will check if the
currently-parsed string is in the dictionary. If the string isn't found in the dictionary
and is not flagged as a word to ignore, then Step 3 will be executed.

### Options For Misspelled Words
Each misspelled word is inserted into a BinarySearchTree, then the program presents
the following options:

For adding a word to the dictionary, the flagged word is inserted into the dictionary HashTable and file.

For a word to be skipped, along with all future instances of it, the word is insertedinto a HashTable of words to not be flagged. However, the program does nothing for skipping one instance of a word.

To find a suggested spelling for a misspelled word, the program iterates through each  character in the string. The current character is swapped with the character in the next index of the word and the new string is checked to see if it exists in the HashTable. If it exists, the program outputs that specific string. If not, the swapping is undone and the 
program then increments to the next character and repeats the swap again. 

Quitiing the spelling check for the file results in terminating the program.

### Writing Misspelled Words To An External File
Writing to the external file is done through an inorder traversal of the BinarySearchTree
and having the printing of each node result in writing to the external file.

