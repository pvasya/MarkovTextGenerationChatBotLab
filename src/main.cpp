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

string generateText(string prefixes[], string suffixes[], int modelSize,int order, int length,int id) {
    int index = rand() % modelSize;
    string currentPrefix = prefixes[id];
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
    string temp, s,tempstr;
    int indextemp;
    int modelSize = 0;
    int order = 1;
    int length = 10;
    const int MAX_PREFIXES = 100000;
    string* prefixes = new string[MAX_PREFIXES];
    string* suffixes = new string[MAX_PREFIXES];
    ifstream file("dialogues_chain.txt");
    while (getline(file, s)) {
        indextemp = 0;
        temp = "";
        tempstr = "";
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == ' ') {
                break;
                indextemp = i + 1;
            }
            temp += s[i];
        }
        prefixes[modelSize] = temp;
        temp = "";
        for (int i = indextemp; i < s.length(); i++) {
            if (s[i] == ' ') {
                temp += " " + tempstr;
                tempstr = "";
                continue;
            }
            tempstr += s[i];
        }
        suffixes[modelSize++] = temp;
    }
    file.close();
    cout << "hi" << endl;
    string str;
    int id = 0;
    while (str != "close") {
        srand(time(NULL));
        length = (rand() % 10) + 5;
        cout << "> ";
        getline(cin, str);
        str = clean_text(str);
        int ind = str.rfind(' ');
        str = str.substr(ind + 1);
        for (int i = 0; i < modelSize; i++) {
            if (str == prefixes[i]) {
                id = i;
                break;
            }
       }
        cout << endl<<"Chatbot: " << generateText(prefixes, suffixes, modelSize, order, length, id) << endl;
    }
    return 0;
}
