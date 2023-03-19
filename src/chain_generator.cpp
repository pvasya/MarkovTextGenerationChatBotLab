#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const bool CONTINUE_IF_NO_MATCH = true;

string clean_text(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) { return tolower(c); });
    text.erase(
        remove_if(text.begin(), text.end(),
            [](unsigned char c) { return !(isalpha(c) || isspace(c)); }),
        text.end());
    return text;
}

void tokenize(string text, string tokens[], int& count) {
    stringstream ss(text);
    string token;
    count = 0;
    while (ss >> token) {
        tokens[count++] = token;
    }
}

void buildMarkovModel(string tokens[], int size, int order, string prefixes[],
    string suffixes[], int& modelSize) {
    modelSize = 0;
    for (int i = 0; i < size - order; i++) {
        string prefix = "";
        for (int j = 0; j < order; j++) {
            prefix += tokens[i + j] + " ";
        }
        string suffix = tokens[i + order];
        bool found = false;
        for (int k = 0; k < modelSize; k++) {
            if (prefix == prefixes[k]) {
                suffixes[k] += " " + suffix;
                found = true;
                break;
            }
        }
        if (!found) {
            prefixes[modelSize] = prefix;
            suffixes[modelSize] = suffix;
            modelSize++;
        }
    }
}

string generateText(string prefixes[], string suffixes[], int modelSize,
    int order, int length) {
    srand(time(NULL)); // seed random
    int index = rand() % modelSize;
    string currentPrefix = prefixes[index];
    string currentSuffixes = "";
    string text = currentPrefix;
    while (length) {
        int index = rand() % modelSize;
        currentSuffixes = suffixes[index];
        int whiteSpaceCounter = 1;
        for (char c : currentSuffixes) {
            if (c == ' ') {
                whiteSpaceCounter++;
            }
        }

        string currentSuffix = currentSuffixes;
        if (whiteSpaceCounter >= 2) {
            int random_value = rand() % whiteSpaceCounter;
            stringstream suffixStream(currentSuffixes);
            int flag = 0;
            while (suffixStream.eof()) {
                getline(suffixStream, currentSuffix, ' ');
                if (flag == random_value) {
                    break;
                }
                flag++;
            }
        }
        text += currentSuffix;
        text += ' ';
        currentPrefix = currentPrefix.substr(currentPrefix.find(' ') + 1);
        currentPrefix += currentSuffix;
        currentPrefix += " ";
        length--;
    }

    return text;
}

int main() {
    // Load text corpus
    ifstream file("dialogues.txt");
    stringstream buffer;
    buffer << file.rdbuf();
    string corpus = clean_text(buffer.str());
    // cout << corpus;
    file.close();

    // Tokenize corpus
    const int MAX_TOKENS = 10000000;
    string* tokens = new string[MAX_TOKENS];
    int count;
    tokenize(corpus, tokens, count);
    // Build Markov chain model
    const int MAX_PREFIXES = 10000000;
    string* prefixes = new string[MAX_PREFIXES];
    string* suffixes = new string[MAX_PREFIXES];
    int modelSize = 0;
    int order = 2;
    buildMarkovModel(tokens, count, order, prefixes, suffixes, modelSize);
    ofstream out("dialogues_chain.txt");
    for(int i = 0;i<modelSize;i++){
        out <<prefixes[i]<<suffixes[i] <<endl;
    }
    out.close();
    return 0;
}
