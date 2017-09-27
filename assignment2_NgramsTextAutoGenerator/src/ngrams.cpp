// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "hashmap.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"

using namespace std;

int prompt_for_input(string & inputfilename, int & N);
int prompt_for_output_length(int lowerbound);
HashMap<Vector<string>, Vector<string>> build_Ngram_map(string inputfilename, int N);
int mapWindowToNgrams(Vector<string> window, HashMap<Vector<string>, Vector<string>> & ngrams, int N);
string mimic_text_using(HashMap<Vector<string>, Vector<string>> ngrams, int output_length);
string vector2string(Vector<string> & v);


int main() {
    string inputfilename;
    int N;
    prompt_for_input(inputfilename, N);

    HashMap<Vector<string>, Vector<string>> ngrams = build_Ngram_map(inputfilename, N);

    while (true) {
        int output_length = prompt_for_output_length(N);

        if (output_length == 0) {
            cout << "Exiting." << endl;
            return 0;
        }
        cout << "..." << mimic_text_using(ngrams, output_length) << "..." << endl;
    }
}

string mimic_text_using(HashMap<Vector<string>, Vector<string>> ngrams, int output_length) {
    int random_counter = randomInteger(0, ngrams.size() - 1);
    Vector<string> starter;
    for (Vector<string> v : ngrams) { // loop over ngrams HashMap to find a Vector<string> to start our return text
        if (random_counter <= -1) {
            starter = v;
            break;
        }
        random_counter--;
    }

    string result = vector2string(starter); // init result string
    int resultSize = starter.size(); // with size variable
    Vector<string> window = starter; // size = N - 1
    while (resultSize < output_length) {
        Vector<string> candidates = ngrams[window];
        // given that the ngrams is organized as Vector<prefix> : Vector<suffix candidates>
        // where the occurrence of each candidate suffix is proportional to the frequencies they occur in the actual input txt
        // so a random pick will generate an output where each word occurs with "appropriate probability".
        int randomIdx = randomInteger(0, candidates.size() - 1);
        string candidate = candidates[randomIdx];
        result += candidate + " ";
        resultSize++;
        window.add(candidate);
        window.remove(0);
    }
    return result;
}

// a helper function implements " ".join(List<string>)
string vector2string(Vector<string> & v) {
    string rst;
    for (string str : v) {
        rst += str + " ";
    }
    return rst;
}

HashMap<Vector<string>, Vector<string>> build_Ngram_map(string inputfilename, int N) {
    ifstream input;
    input.open(inputfilename);
    HashMap<Vector<string>, Vector<string>> ngrams;
    Vector<string> words;
    string word;
    while (input >> word) { // read word one by one separated by \s
        words.add(word);
    }
    int wordSize = words.size(); // a b c d e f g h i j k, size = 11
    Vector<string> window;
    Vector<string> firstN = words.subList(0, N); // a b c (N = 3)
    words += firstN; // a b c d e f g h i j k a b c, size = 14
    int start = 0;
    while (start < wordSize) { // loop over till the start = k and the last window = {k, a, b}.
        window = words.subList(start, N);
        mapWindowToNgrams(window, ngrams, N);
        start++;
    }
    return ngrams;
}

// a helper function that splits window and put prefix/suffix into ngrams
int mapWindowToNgrams(Vector<string> window, HashMap<Vector<string>, Vector<string>> & ngrams, int N) {
    if (!ngrams.containsKey(window.subList(0, N - 1))) {
        Vector<string> suffix; // {}
        suffix.add(window[N - 1]); // {last_word}
        ngrams.put(window.subList(0, N - 1), suffix);
    } else {
        ngrams[window.subList(0, N - 1)].add(window[N - 1]);
    }
    return 0;
}

int prompt_for_input(string & inputfilename, int & N) {

    cout << "Welcome to CS 106B Random Writer ('N-Grams').      " << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups  " << endl;
    cout << "of words, and I'll create random text for you.     " << endl;

    inputfilename = getLine("Input file name?");
    while (!fileExists(inputfilename)) {
        cout << "Unable to open that file.  Try again." << endl;
        inputfilename = getLine("Input file name?");
    }
    string n = getLine("Value of N?");
    while (!stringIsInteger(n) || stringToInteger(n) < 2) {
        cout << "Invalid input or it is smaller than 2. Try again." << endl;
        n = getLine("Value of N?");
    }
    N = stringToInteger(n);

    return 0;
}

int prompt_for_output_length(int lowerbound) {
    string numStr = getLine("# of random words to generate (0 to quit)?");
    while (true) {
        if (!stringIsInteger(numStr)) {
            cout << "Illegal integer format. Try again." << endl;
            numStr = getLine("# of random words to generate (0 to quit)?");
        } else if (stringToInteger(numStr) == 0) {
            return 0;
        } else if (stringToInteger(numStr) < lowerbound) {
            cout << "Must be at least " << lowerbound << " words." << endl;
            numStr = getLine("# of random words to generate (0 to quit)?");
        } else {
            break;
        }
    }
    return stringToInteger(numStr);
}
