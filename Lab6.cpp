/////////////////////////////////////////////////////////////////////
// Name: Gilberto Almanza
// Date: 11/11/2026
// Class: CSCI 1470.04
// Semester: Fall 2026
// CSCI 1470 Instructor: Dr. Jonatan Reyes
// Program Description: Program will check spelling.
/////////////////////////////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<chrono>
#include<algorithm>

using namespace std;
using namespace std::chrono;

const int MAX_WORDS = 466551;
const bool debug = true;
const string LINEAR_SEARCH_UNSORTED = "LINEAR_UNSORTED";
const string LINEAR_SEARCH_SORTED = "LINEAR_SORTED";
const string BINARY_SEARCH = "BINARY";

void arrayDictionary(string words[]);
void dictionaryLoad(string words[], bool& load);
bool searchWord(const string words[], string foundWord, string searchType);
bool linearUnsorted(const string words[], string foundWord);
bool linearSorted(const string words[], string foundWord);
bool binarySearch(const string words[], string foundWord);

int main() {

    string* words = new string[MAX_WORDS];
    int maxMistakes = 0;
    string searchType = "";

    ifstream inFile("settings.txt");

    if (!inFile)
    {
        cout << "Error opening file!";
        return 1;
    }

    inFile >> maxMistakes;
    inFile >> searchType;
    inFile.close();

    cout << "*****************************" << endl;
    cout << "Welcome to the spell checker!" << endl;
    cout << "*****************************" << endl;

    arrayDictionary(words);

    bool load = true;
    dictionaryLoad(words, load);

    if (load == false) {
        cout << "Error opening dictionary file!" << endl;
        return 1;
    }

    if (searchType == LINEAR_SEARCH_SORTED ||
        searchType == BINARY_SEARCH) {

        sort(words, words + MAX_WORDS);
    }

    string foundWord;
    int mistakes = 0;
    char answer = 'Y';

    while ((answer == 'Y' || answer == 'y') &&
        mistakes < maxMistakes) {

        cout << "Please enter a word: ";
        cin >> foundWord;

        auto start = high_resolution_clock::now();

        bool found = searchWord(words, foundWord, searchType);

        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        if (found == true) {

            cout << "The word " << foundWord << " is spelled correctly!" << endl;

            mistakes = 0;
        }
        else {

            cout << "Oops! It looks like '" << foundWord << "' isn't quite right." << endl;

            cout << "No worries, you're doing great!" << " Let's try spelling it again." << endl;

            mistakes++;
        }

        if (debug) {

            cout << fixed << setprecision(0);

            if (searchType == BINARY_SEARCH) {

                cout << "Binary search took " << elapsed.count() * 1000000 << " microseconds." << endl;
            }
            else {

                cout << "Linear search took " << elapsed.count() * 1000000 << " microseconds." << endl;
            }
        }

        if (found == true) {

            cout << endl;
            cout << "Do you want to continue (Y/N): ";
            cin >> answer;
        }

        cout << endl;
    }

    if (mistakes >= maxMistakes) {

        cout << "You have reached the maximum number (" << maxMistakes << ") of misspelled words allowed." << endl;
    }

    cout << "Thank you for using the spell checker!" << endl;

    return 0;
}


void arrayDictionary(string words[]) {

    for (int i = 0; i < MAX_WORDS; i++) {
        words[i] = "";
    }
}


void dictionaryLoad(string words[], bool& load) {

    ifstream inFile("dictionary.txt");

    if (!inFile) {
        load = false;
        return;
    }

    int i = 0;
    bool read = true;

    while (i < MAX_WORDS && read) {

        if (inFile >> words[i]) {
            i++;
        }
        else {
            read = false;
        }
    }

    inFile.close();
}


bool searchWord(const string words[], string foundWord,
    string searchType) {

    bool found = false;

    if (searchType == LINEAR_SEARCH_UNSORTED) {

        found = linearUnsorted(words, foundWord);
    }
    else if (searchType == LINEAR_SEARCH_SORTED) {

        found = linearSorted(words, foundWord);
    }
    else if (searchType == BINARY_SEARCH) {

        found = binarySearch(words, foundWord);
    }

    return found;
}


bool linearUnsorted(const string words[], string foundWord) {

    int position = 0;
    bool found = false;

    while (position < MAX_WORDS && !found) {

        if (words[position] == foundWord) {

            found = true;
        }
        else {

            position++;
        }
    }

    return found;
}


bool linearSorted(const string words[], string foundWord) {

    int position = 0;
    bool found = false;

    while (position < MAX_WORDS && !found) {

        if (words[position] == foundWord) {

            found = true;
        }
        else if (words[position] > foundWord) {

            position = MAX_WORDS;
        }
        else {

            position++;
        }
    }

    return found;
}


bool binarySearch(const string words[], string foundWord) {

    int first = 0;
    int last = MAX_WORDS - 1;
    int middle;
    bool found = false;

    while (first <= last && !found) {

        middle = (first + last) / 2;

        if (foundWord == words[middle]) {

            found = true;
        }
        else if (foundWord < words[middle]) {

            last = middle - 1;
        }
        else {

            first = middle + 1;
        }
    }

    return found;
}