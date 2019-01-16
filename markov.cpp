#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <ctime>

using namespace std;

int invalidSyntax() {
    cerr << "Invalid syntax." << endl;
    cerr << "Usage: markov [-s <sentences>] <files>" << endl;
    return 22; //EINVAL
}

int fileOpenError(char* filename) {
    cerr << "Error opening file " << filename << endl;
    return 2; //ENOENT
}

void badSentences() {
    cerr << "Sentence count must be greater than zero." << endl;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int sentences = 3;
    bool fileGiven = false;
    vector<string> startingWords;
    unordered_map<string, vector<string>> markovDictionary;
    //Interpret the arguments
    if (argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "-s") == 0) {
                if(i + 1 < argc) {
                    sentences = atoi(argv[i + 1]);
                    if(sentences < 1) {
                        badSentences();
                        sentences = 3;
                    }
                    i++;
                } else {
                    return invalidSyntax();
                }
            } else {
                fileGiven = true;
                ifstream currentFile(argv[i]);
                if(currentFile.is_open()) {
                    string prevWord;
                    string currentWord;
                    currentFile >> prevWord;
                    startingWords.push_back(prevWord);
                    while(currentFile >> currentWord) {
                        if(currentWord[0] >= 'A' && currentWord[0] <= 'Z') {
                            startingWords.push_back(currentWord);
                        }
                        markovDictionary[prevWord].push_back(currentWord);
                        prevWord = currentWord;
                    }
                    currentFile.close();
                } else {
                    return fileOpenError(argv[i]);
                }
            }
        }
    }
    //If no file is given, interpret the user's input 
    if(!fileGiven) {
        cout << "The program will read all of the sentences you enter until you input ENDFILE" << endl;
        cout << "> Markov: ";
        string prevWord;
        cin >> prevWord;
        startingWords.push_back(prevWord);
        while(true) {
            string currentWord;
            cin >> currentWord;
            if(currentWord == "ENDFILE") {
                break;
            }
            if(currentWord[0] >= 'A' && currentWord[0] <= 'Z') {
                startingWords.push_back(currentWord);
            }
            markovDictionary[prevWord].push_back(currentWord);
            prevWord = currentWord;
        }
        cout << endl;
    }
    //Build the markov chained sentences
    string currentWord = startingWords[rand() % startingWords.size()];
    int words = 0;
    while(true) {
        if(words > 0) {
            cout << " ";
        }
        cout << currentWord;
        words++;
        if(currentWord[currentWord.length() - 1] == '.' || currentWord[currentWord.length() - 1] == '?' || currentWord[currentWord.length() - 1] == '!') {
            sentences--;
            if(sentences == 0) {
                break;
            }
        }
        if(markovDictionary.find(currentWord) == markovDictionary.end()) {
            currentWord = startingWords[rand() % startingWords.size()];
        } else {
            int size = markovDictionary[currentWord].size();
            currentWord = markovDictionary[currentWord].at(rand() % size);
        }
    }
    cout << endl;
}