#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

#include "BinarySearchTree.h"
#include "HashTable.h"

struct BNode {
    string word;
    int lineNum;
    bool operator < (BNode otherStruct) const {
        return (word < otherStruct.word);
    }
    bool operator == (BNode otherStruct) const {
        return (word == otherStruct.word);
    }
};

typedef BinarySearchTree<BNode> BST;
ostream &operator << (ostream &out, BNode &temp);

void checkFiles(int argc, ifstream &input, fstream &dict, char *argv[]);
void initializeDict(fstream &dict, HashTable &table);
void parseLine(ifstream &input, HashTable &dict, ofstream &outFile, HashTable &notFlagged, BST &tree, fstream &dictionary);
void scanWord(ifstream &input, HashTable &dict, string temp, int lineNum, BST &tree, HashTable &notFlagged, fstream &dictionary);
string provideSuggestion(string &word, HashTable &dict);
void options(BNode misspelled, HashTable &dict, HashTable &notFlagged, fstream &dictionary);

int main(int argc, char *argv[]) {
    // PRE: Dictionary file, then text file are passed as command-line arguments in that order. 
    // POST: Program uses dictionary file to detect any misspelled words.
    ifstream text;
    fstream dictionary;
    ofstream notFound("notFound.txt");
    HashTable words(1000);
    HashTable doNotFlag(1000);
    BNode root = {"ZZZZ", -1};
    BST tree(root);

    // Step 1: Set file to read from
    checkFiles(argc, text, dictionary, argv);
    initializeDict(dictionary, words);

    // Step 2: Parse each line
    parseLine(text, words, notFound, doNotFlag, tree, dictionary);

    // Step 3: Write inorder traversal of BST to the "notFound" file. 
    tree.printTree(notFound);

    text.close();
    dictionary.close();
    notFound.close();

    return 0;
}

ostream &operator << (ostream &out, BNode &temp) {
    out << temp.word << " " << temp.lineNum;
    return out;
}

void checkFiles(int argc, ifstream &input, fstream &dict, char *argv[]) {
    // PRE: Command-line arguments are present and dictionary and text files are in the same directory. 
    // POST: Validates and opens the dictionary file to use and text file to check. 
    if (argc == 1) {
        cerr << "File not specified..." << endl;
        exit(1);
    } else {
        string fileName = argv[argc - 2];
        input.open(fileName.c_str());
        dict.open("dict.txt");

        if (!input) {
            cerr << "File not found..." << endl;
            exit(1);
        }
    }
}

void initializeDict(fstream &dict, HashTable &table) {
    // PRE: There is an existing dictionary file and declared HashTable. 
    // POST: Fills the HashTable with words from the dictionary file. 
    string temp;
    while (!dict.eof()) {
        getline(dict, temp);
        table.insert(temp);
    }
}

void parseLine(ifstream &input, HashTable &dict, ofstream &outFile, HashTable &notFlagged, BST &tree, fstream &dictionary) {
    // PRE: Dictionary file, input file, and HashTable have been initialized. 
    // POST: Parses through each line in the text file.
    int lineNum = 1;
    string temp;
    while (!input.eof()) {
        getline(input, temp);
        scanWord(input, dict, temp, lineNum, tree, notFlagged, dictionary);
        lineNum++;
    }
    cout << "*** ALL SPELLCHECKING HAS BEEN COMPLETE ***" << endl;
}

void scanWord(ifstream &input, HashTable &dict, string temp, int lineNum, BST &tree, HashTable &notFlagged, fstream &dictionary) {
    // PRE: There is already a line from the file that can be scanned for words. 
    // POST: Checks each word from input to see if it is in the dictionary. 
    string word = "";
    for (int i = 0; i < temp.length(); i++) {
        if (isalpha(temp[i]) || temp[i] == '\'') {
            word += tolower(temp[i]);
        } else {
            bool correctlySpelled = dict.find(word);
            if (!correctlySpelled && !notFlagged.find(word)) {
                BNode misspelled = {word, lineNum};
                if (tree.find(misspelled) == misspelled) {
                    tree.remove(misspelled);
                    tree.insert(misspelled);
                } else {
                    tree.insert(misspelled);
                }
                
                options(misspelled, dict, notFlagged, dictionary);
            }
            word = "";
        }
    }
}

string provideSuggestion(string &word, HashTable &dict) {
    // PRE: The word has been parsed from the input file.
    // POST: Each character of the word is iterated through to find a match in the dictionary.
    bool match = false;
    int i = 0;
    char temp;

    while (!match && i < word.length()) {
        if (i != word.length() - 1) {
            // Swap char at current index and next index if not at last letter yet.
            temp = word[i];
            word[i] = word[i + 1];
            word[i + 1] = temp;

            if (dict.find(word)) {
                match = true;
            } else {
                // Undo swap if there is no match to a word in the dictionary. 
                word[i + 1] = word[i];
                word[i] = temp;
            }
        }
        i++;
    }

    return match ? word : "No suggestions available.";
}

void options(BNode misspelled, HashTable &dict, HashTable &doNotFlag, fstream &dictionary) {
    // PRE: Misspelled word has been flagged. 
    // POST: Gives user a list of options to execute on the flagged word. 
    char option;
    string useless = " ";
    cout << "\n\n!@#$%^&*(){}\tTHE SPELL CHECKER PROGRAM\t!@#$%^&*(){}\n" << endl;
    cout << misspelled.word << " On Line " << misspelled.lineNum << " Was Not Found In Dictionary\n" << endl;
    cout << "A) Add the Word To Dictionary" << endl;
    cout << "I) Ignore Word, and Skip Future References" << endl;
    cout << "G) Go On To Next Word" << endl;
    cout << "S) Search For A Suggested Spelling" << endl;
    cout << "Q) Quit Spell Checking File\n" << endl;

    do {
        cout << "Selection: ";
        cin >> ws;
        cin >> option;
        option = toupper(option);

        switch (option) {
            case 'A':
                dict.insert(misspelled.word);
                dictionary << misspelled << endl;
                break;
            
            case 'I':
                doNotFlag.insert(misspelled.word);
                break;
            
            case 'G':
                break;
            
            case 'S':
                cout << "Suggested Spelling: " << provideSuggestion(misspelled.word, dict) << endl;
                cout << "Press ENTER to continue... ";
                cin.ignore();
                getline(cin, useless);
                break;
            
            case 'Q':
                exit(1);
                break;
        }
        cout << endl;

    } while (option != 'A' && option != 'I' && option != 'G' && option != 'S' && option != 'Q');  
}

