// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h" // getLine()
#include "filelib.h" // fileExists()
#include "lexicon.h" // Lexcon()
#include "stack.h"
#include "queue.h"
#include "vector.h"

using namespace std;

// FUNC DECLARATION
string prompt_for_dictionary();
int prompt_for_words(string & w1, string & w2, Lexicon & dict);
int find_word_ladder(string w1, string w2, Lexicon & dict, Stack<string> & result);
Stack<string> copyStrStack(Stack<string> stk);


int main() {

    string inputfilename = prompt_for_dictionary();

    Lexicon dict = Lexicon(inputfilename);

    while (true) { // keep playing till prompt_for_words return -1;
        string w1;
        string w2;
        if (prompt_for_words(w1, w2, dict) == -1) { // empty string input found because use pressing ENTER only.
            cout << "Have a nice day." << endl;
            return 0;
        }

        Stack<string> result;
        if (find_word_ladder(w1, w2, dict, result) == -1) {
            cout << "No word ladder found from " << w2 << " back to " << w1 << endl;
        }
        else {
            cout << "A ladder from " << w2 << " back to "  << w1 << " :"  << endl;
            cout << result.toString() << endl;
        }
    }
}

// print Header
// check existence of input dictionary
// return inputfilename
string prompt_for_dictionary() {
    cout << "Welcome to CS 106B Word Ladder.                        " << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;

    string inputfilename;
    inputfilename = getLine("Dictionary file name?");
    while (!fileExists(inputfilename)) {
        cout << "The file doesn't exist. Try again." << endl;
        inputfilename = getLine("Dictionary file name?");
    }
    return inputfilename;
}

// convert all user input into lowercase
// check for circumstances under which a word ladder between the two inputs will not possibly exist.
int prompt_for_words(string & w1, string & w2, Lexicon & dict) {
    while (true) {
        w1 = toLowerCase(getLine("Word #1 (or Enter to quit)"));
        if (w1 == "") {
            return -1;
        }
        w2 = toLowerCase(getLine("Word #2 (or Enter to quit)"));
        if (w2 == "") {
            return -1;
        }

        if (!dict.contains(w1) || !dict.contains(w2)) {
            cout << "The two words must be found in the dictionary\n" << endl;
        }
        else if (w1.length() != w2.length()) {
            cout << "The two words must have the same length.\n" << endl;
        }
        else if (w1 == w2) {
            cout << "The two words must be different.\n" << endl;
        }
        else {
            return 0;
        }
    }
}

// main algorith of wordLadder problem, using BFS.
// involving Queue, Stack data structure.
int find_word_ladder(string w1, string w2, Lexicon & dict, Stack<string> & result) {
    Stack<string> stk;
    stk.push(w1);
    Queue<Stack<string>> que;
    que.enqueue(stk);
    while (!que.isEmpty()) {
        stk = que.dequeue();
        string curt = stk.top();
        for (int i = 0; i < (int) curt.length(); i++) {
            for (int j = 97; j < 123; j++) { // ascii : a - z
                char character = (char) j;
                string next = string(curt); // create a copy
                next[i] = character; // c++ string is mutable
                if (!dict.contains(next)) {
                    continue;
                } else {
                    Stack<string> newStk = copyStrStack(stk);
                    newStk.push(next);
                    if (next == w2) {
                        result = newStk;
                        return 0; // found
                    } else {
                        que.enqueue(newStk);
                    }
                }
            }
        }
    }
    return -1; // not found
}

Stack<string> copyStrStack(Stack<string> stk) { // pass by value
    return stk;
}
