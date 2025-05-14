#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>

using namespace std;

class GameMode {
public:
    virtual bool validateMode() = 0;
    virtual bool checkWordLength(int length) = 0;
    virtual string getFilename() = 0;
    virtual ~GameMode() {}
};

class EasyGameMode : public GameMode {
public:
    bool validateMode() override;
    bool checkWordLength(int length) override;
    string getFilename() override;
};

class HardGameMode : public GameMode {
public:
    bool validateMode() override;
    bool checkWordLength(int length) override;
    string getFilename() override;
};

class GameGrid {
private:
    char** grid;
    int rows, cols;

public:
    GameGrid(int gridSize);
    ~GameGrid();
    char** getGrid();
    int getRows() const;
    int getCols() const;
    void initializeGrid();
    void display();
    void randomizeGridPlacement(char** words, int wordCount);
    void updateWordPosition(int positions[][2], int wordLength);
};

class WordChecker {
public:
    static bool validateWord(char** words, int wordCount, const char* word);
};

class ScoreManager {
public:
    static void updateScore(const char* name, int score);
};

class GameRun {
private:
    char playerName[50];
    int getValidInput(const string& prompt);
    char** allocateWordArray(int rows, int cols);
    void deallocateWordArray(char** words, int rows);

public:
    void displayMainMenu();
    void displayModeMenu();
    void displayTopScores();
    void howToPlay();
    void displayWordHints(char** words, int wordCount);
    bool playGame();
};
