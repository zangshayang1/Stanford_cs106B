/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"
#include "vector.h"
#include "hashmap.h"
#include "filelib.h"
#include "strlib.h"
#include "assert.h"
#include "simpio.h"
#include "random.h"

using namespace std;

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
HashMap<string, Vector<string>> parse_stream(ifstream & input);
string generate(HashMap<string, Vector<string>> & grammar, string symbol);
string vec2sentence(Vector<string> v);

Vector<string> grammarGenerate(ifstream& input, string symbol, int times) {
    HashMap<string, Vector<string>> grammar = parse_stream(input);

    cout << grammar.toString() << endl;

    Vector<string> rst;
    while (times > 0) {
        rst.add(generate(grammar, symbol));
        times--;
    }
    return rst;
}

HashMap<string, Vector<string>> parse_stream(ifstream & input) {
    HashMap<string, Vector<string>> grammar;
    string line;
    while (getline(input, line)) {
        Vector<string> split_line = stringSplit(line, "::=");
        assert (split_line.size() == 2);
        string symbol = split_line[0];
        Vector<string> rules = stringSplit(split_line[1], "|");
        grammar.put(symbol, rules);
    }
    return grammar;
}

string generate(HashMap<string, Vector<string>> & grammar, string symbol) {
    // given symbol, there are multiple patterns that could be generated and returned
    // pick a random one
    assert (grammar.containsKey(symbol));
    Vector<string> patterns = grammar[symbol];
//    cout << "patterns: " << patterns.toString() << endl;
    int idx = randomInteger(0, patterns.size() - 1);
//    cout << "idx: " << idx << endl;
    string pattern = patterns[idx]; // e.g. "<adj> <noun>"
//    cout << "pattern: " << pattern << endl;
    Vector<string> components = stringSplit(pattern, " "); // list all the components that make the pattern

    Vector<string> rst;
    for (string component : components) {
        if (grammar.containsKey(component)) {
            rst.add(generate(grammar, component));
        }
        else {
            rst.add(component);
        }
    }

    cout << rst.toString() << endl;

    return vec2sentence(rst);
}

string vec2sentence(Vector<string> v) {
    string sentence;
    for (string word : v) {
        sentence += trim(word) + " "; // trim() to avoid accumulative space
    }
    return sentence;
}
