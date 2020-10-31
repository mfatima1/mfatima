
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: malihameer
 *
 * Created on October 23, 2020, 3:39 PM
 */

#include <ctime>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include<iomanip>
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include"error.h"
using namespace std;

// Function prototypes
void DisplayError(error);
void origGame();
void gameStatus(vector<char>& guessed, int numGuessus, bool total, set<string>& curGuessSet);
void currGuess(set<string>& curGuessSet, char curGuess, vector<char>& guessed);
void evilGame(int initGuess, map<int, set<string> >& dict, int numGuessus, bool total);
void intoMap(map<int, set<string> >& dict, ifstream& dictionary);
int guessSize(int minSize, int maxSize, map<int, set<string> >& dict);
char getGuess(vector<char>& guessed);
int guesses();
string GetLine();
int getNum();

int main() {

    priority_queue<string> user;
    string name;

    cout << "Enter your name: " << endl;
    cin >> name;
    cout << endl;
    user.push(name);
    cout << "Welcome to hangman " << name << " good luck!!\n";

    // Reading the dictionary.txt file into a map of integer and strings.
    ifstream dictionary("dictionary.txt");
    map<int, set<string> > dict;
    intoMap(dict, dictionary);

    // Getting the maximum and minimum size of words in dictionary according to initialized map 
    map<int, set<string> >::iterator itr = --dict.end();
    int maxSize = itr->first;
    itr = dict.begin();
    int minSize = itr->first;

    cout << "Select the Game Mode: \n";
    cout << "1. Classic Hangman \n";
    cout << "2. Evil Hangman \n";

    int userInput;
    cin >> userInput;
    if (userInput == 1) {
        //original hangman game
        cout << "You will get 6 lives to guess the secret word\n";
        origGame();
        char choice;
        cout << "Would you like to play again? (Y/N)\n";
        cin >> choice;
        //converts the user input to lowercase letter
        choice = toupper(choice);
        if (choice == 'Y') {
            cout << "Great choice " << name << "!!\n";
        origGame();
        } else {
            exit(0);
        }
    } else if (userInput == 2) {
        cout <<"You will have as many attempts as you wish but can you outsmart the computer?\n";
        // Getting the input about the initial guess size from the user
        int initGuess = guessSize(minSize, maxSize, dict);
        // Prompting the user about the number of tries he wants 
        int numGuessus = guesses();
        // Prompting the user about whether she wants a running total of the words remaining 
        char otherChar;
        cout << "Do you want the running total of the words remaining? [y/n] " << endl;
        cin>>otherChar;
        bool total = otherChar == 'y' ? true : false;
        //Running the actual game 
        evilGame(initGuess, dict, numGuessus, total);

    } else {
        exit(0);
    }

    return 0;
}
//original hangman game
void origGame() {

    /*object of count error class
     *used to keep track of errors and displays the hangman respectively
     */
    error count;
    //vector of strings that will be used to guess in the game
    vector<string> wordlist;
    wordlist.push_back("watermelon");
    wordlist.push_back("love");
    wordlist.push_back("cocktail");
    wordlist.push_back("grapes");
    wordlist.push_back("library");
    wordlist.push_back("cat");
    wordlist.push_back("sculpture");
    wordlist.push_back("galaxy");
    wordlist.push_back("brownies");
    wordlist.push_back("hamster");

    srand(static_cast<unsigned int> (time(0)));
    std::random_shuffle(wordlist.begin(), wordlist.end());
    //random word generator
    int random;
    if (wordlist.size() <= 10) {
        random = rand() % 10 + 1; //Get random numbers from 1-10
    }
    //selects a random word to guess 
    const string word = wordlist[random - 1];
    //words guessed so far
    string soFar(word.size(), '-');
    //letters which the player already used
    string used;
    //to display the hangman
    DisplayError(count);

    /* This loop prompts user to enter their guess letter by letter.
     * It then finds the letter that was guessed in the word.
     * Displays the hangman according to the errors.
     * Determines whether the player wins or loses
     */
    while ((count.getNum() < 6) && (soFar != word)) {

        cout << "\nSo far, the word is:\n" << soFar << endl << endl;
        //outputs the letters which are already used
        cout << "\nYou've used the following letters:\n" << used << "\n " << endl;
        char guess;
        //prompts the user to enter their guess letter by letter.
        cout << "\nEnter your guess: ";
        cin>>guess;
        //make user can enter lower or uppercase
        guess = tolower(guess); 
        while (used.find(guess) != string::npos) {
            cout << "\nYou've already guessed " << guess << endl;
            cout << "Enter your guess: ";
            cin>>guess;
            guess = toupper(guess);
        }
        used += guess;

        if (word.find(guess) != string::npos) {
         //update soFar to include newly guessed letter
            for (unsigned int i = 0; i < word.length(); i++) {
                if (word[i] == guess) {
                    soFar[i] = guess;
                }
            }
        } else {
         //gets the number of errors made by the player
            count.stackdown();
            DisplayError(count);
        }
    }
    if (count.getNum() == 6) {
        cout << "YOU LOSE!" << endl;
        cout << "\nThe word was " << word << endl;
    } else {
        cout << "YOU WIN!" << endl;
        cout << "\nThe word was " << word << endl;
    }
}
//This function outputs the hangman respective to the number of errors 

void DisplayError(error count) {
    if (count.getNum() == 0) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 1) {
      
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 2) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << "  |   |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 3) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << "  |\\  |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 4) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << " /|\\  |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 5) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << " /|\\  |" << endl;
        cout << " /    |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
    if (count.getNum() == 6) {
        
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "  0   |" << endl;
        cout << " /|\\  |" << endl;
        cout << " / \\  |" << endl;
        cout << "      |" << endl;
        cout << "=========" << endl;
    }
}

 //Evil hangman game.
void evilGame(int initGuess, map<int, set<string> >& dict, int numGuessus, bool total) {
    // generates a set of words with word length matching initial length
    set<string> curGuessSet = dict[initGuess];
    // contains the letters the user has used, and matches if the word contains those letters 
    vector<char> guessed(initGuess, '_');
    //initGuess is the length of the word
   
    while (numGuessus > 0) {
    // outputs the game status 
        gameStatus(guessed, numGuessus, total, curGuessSet);
    // prompts the player to enter a letter 
        char curGuess = getGuess(guessed);
    // updates the curGuessSet according to the user guess 
        currGuess(curGuessSet, curGuess, guessed);
         
    //determines if the user wins
        if (count(guessed.begin(), guessed.end(), '_') == 0) {
            cout << "YOU WIN!" << endl;
            cout << "The letter was : " << endl;
            for (int i = 0; i < guessed.size(); ++i) {
                cout << guessed[i];
            }
            cout << endl;
            return;
        }
        numGuessus--;
    }
    //outputs the word that the computer initially selected
    cout << "The Word was: " << *curGuessSet.begin() << "\n" << endl;
    cout << "YOU LOSE!" << endl;

    return;
}
//current guessed set is updated by dividing the set and choosing the one with most number of words.
void currGuess(set<string>& curGuessSet, char curGuess, vector<char>& guessed) {
    
    int wordLength = curGuessSet.begin()->length();
    //map containing the word families
    map<int, set<string> > partitions;
    //to check the used letters
    //assigns repeated Letter position to the position of last repeated letter.
    int repCharPos = -1;
    for (int i = 0; i < guessed.size(); i++) {
        if (guessed[i] == curGuess)
            repCharPos = i;
    }
    //loops through the current word set to populate set partitions
    for (set<string>::iterator itr = curGuessSet.begin(); itr != curGuessSet.end(); ++itr) {
        string curWord = *itr;
        int pos;
        // finds the last repeated letter
        // if the letter is not found, it starts from zero since the repCharPos is set to 1
       
        pos = curWord.find(curGuess, repCharPos + 1);
        if (pos == string::npos)
            partitions[0].insert(curWord);
        else {
            for (int i = 0; i < wordLength; ++i) {
                if (pos == i)
                    partitions[i + 1].insert(curWord);
            }
        } 
    }
    
    //updating the current guess set according to max size 
    int maxIndx = -1;
    int maxSize = -1;
    for (map<int, set<string> >::iterator itr = partitions.begin(); itr != partitions.end(); ++itr) {
        int size = itr->second.size();
        if (size > maxSize) {
            maxSize = size;
            maxIndx = itr->first;
        }
    }
    curGuessSet = partitions[maxIndx];
   
     // updates the guessed letters vector according to the partitions selected . maxIndx -1 corresponds 
     // to the index of the letter in the word family.
     
    if (maxIndx != 0)
        guessed[maxIndx - 1] = curGuess;

    return;
}
//Prompts the user to enter a character
char getGuess(vector<char>& guessed) {
    cout << "Enter you guess : ";
    char curChar;
    while (true) {
        cin>>curChar;
        if (curChar >= 'a' && curChar <= 'z')
            break;
        else
            cout << "Please enter an alphabetical lowercase character" << endl;
    }
    return curChar;
}

/*
 * This function outputs the game status including 
 * the letters used, the word length 
 * total of remaining words 
 * # of guesses left
 */
void gameStatus(vector<char>& guessed, int numGuessus, bool total, set<string>& curGuessSet) {
    // Printing out the guessed chars vector 
    for (int i = 0; i < guessed.size(); i++) {
        cout << guessed[i] << " ";
    }
    cout << endl;
    // Printing out the no of guesses left 
    cout << "You have " << numGuessus << " attempts left" << endl;
    // Printing out the total of words left, if user desires
    if (total)
        cout << "Words left : " << curGuessSet.size() << endl;
}
 // reads the dictionary contents into a map which contains sizes mapped to a sets of strings of those sizes.
void intoMap(map<int, set<string> >& dict, ifstream& dictionary) {
    string line;
    while (getline(dictionary, line)) {
        dict[line.length()].insert(line);
    }
    return;
}

 // Takes in input from the user and checks if the initial guessed size is in min and max range of words
 // in the dictionary.
int guessSize(int minSize, int maxSize, map<int, set<string> >& dict) {
    cout << "Enter the length of the word you want to guess " << endl;
    int guess;
    while (true) {
        guess = getNum();
        if (dict.count(guess))
            break;
        cout << "Cannot find a word with that length in the dictionary!" << endl;
    }
    return guess;
}
//Prompts the user for the number of attempts
int guesses() {
    cout << "Enter the number of attempts you would like to have" << endl;
    int numGuessus = getNum();
    return numGuessus;
}
// This function gets a line from the user and returns it in the form of a string.
string GetLine() {
    string line;
    getline(cin, line);
    return line;
}
//This function accepts an integer from the user and returns it,
int getNum() {
    int num;
    while (true) {
        if (cin >> num) {

            return num;
        } else
            cout << "Please enter an integer" << endl;
    }
}
