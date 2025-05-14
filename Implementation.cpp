#include "WordGuessingGame.h"

bool EasyGameMode::validateMode() { return true; }

bool EasyGameMode::checkWordLength(int length)  {
    return length >= 2 && length <= 4;
}

string EasyGameMode::getFilename()  { return "easy.txt"; }

bool HardGameMode::validateMode()  { return true; }

bool HardGameMode::checkWordLength(int length)  {
    return length >= 5 && length <= 7;
}

string HardGameMode::getFilename() { return "hard.txt"; }

GameGrid::GameGrid(int gridSize) : rows(gridSize), cols(gridSize) {
    grid = new char* [rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new char[cols];
        memset(grid[i], 0, cols);
    }
}

GameGrid::~GameGrid() {
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

char** GameGrid::getGrid() { return grid; }

int GameGrid::getRows() const { return rows; }

int GameGrid::getCols() const { return cols; }

void GameGrid::initializeGrid() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = 'A' + rand() % 26;
        }
    }
}

void GameGrid::display() {
    cout << "   ";
    for (int j = 0; j < cols; ++j) {
        cout << j % 10 << " ";
    }
    cout << endl;

    for (int i = 0; i < rows; ++i) {
        cout << (i % 10) << "  ";
        for (int j = 0; j < cols; ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

void GameGrid::randomizeGridPlacement(char** words, int wordCount) {
    initializeGrid();

    for (int i = 0; i < wordCount; ++i) {
        int wordLen = static_cast<int>(strlen(words[i]));
        bool placed = false;
        int attempts = 0;

        while (!placed && attempts < 100) {
            bool isHorizontal = rand() % 2 == 0;
            int startRow = rand() % (rows - wordLen + 1);
            int startCol = rand() % (cols - wordLen + 1);

            if (isHorizontal) {
                for (int j = 0; j < wordLen; ++j) {
                    grid[startRow][startCol + j] = words[i][j];
                }
            }
            else {
                for (int j = 0; j < wordLen; ++j) {
                    grid[startRow + j][startCol] = words[i][j];
                }
            }

            placed = true;
            attempts++;
        }
    }
}

void GameGrid::updateWordPosition(int positions[][2], int wordLength) {
    for (int i = 0; i < wordLength; ++i) {
        int row = positions[i][0];
        int col = positions[i][1];
        grid[row][col] = ' ';
    }
}

bool WordChecker::validateWord(char** words, int wordCount, const char* word) {
    for (int i = 0; i < wordCount; ++i) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

void ScoreManager::updateScore(const char* name, int score) {
    static const int MAX_SCORES = 5;
    char names[MAX_SCORES][50] = { 0 };
    int scores[MAX_SCORES] = { 0 };
    int currentCount = 0;

    // Check if file exists, if not create and initialize it
    ifstream checkFile("topScore.txt");
    if (!checkFile.good()) {
        ofstream initFile("topScore.txt");
        if (initFile.is_open()) {
            // Initialize with 5 "Unknown" players with score 0
            for (int i = 0; i < MAX_SCORES; ++i) {
                initFile << "Unknown " << 0 << endl;
            }
            initFile.close();
        }
    }
    checkFile.close();

    // Read existing scores
    ifstream scoreFile("topScore.txt");
    if (scoreFile.is_open()) {
        while (currentCount < MAX_SCORES&&
            scoreFile >> names[currentCount] >> scores[currentCount]) {
            currentCount++;
        }
        scoreFile.close();
    }

    // Check if new score qualifies
    int insertIndex = -1;
    for (int i = 0; i < currentCount; ++i) {
        if (score > scores[i]) {
            insertIndex = i;
            break;
        }
    }

    // If score qualifies, insert it
    if (insertIndex != -1) {
        // Shift scores down
        for (int i = currentCount; i > insertIndex; --i) {
            // Manual character array copy
            for (int j = 0; j < 50; ++j) {
                names[i][j] = names[i - 1][j];
            }
            scores[i] = scores[i - 1];
        }

        // Manual character array copy for name
        for (int j = 0; name[j] != '\0' && j < 49; ++j) {
            names[insertIndex][j] = name[j];
            names[insertIndex][j + 1] = '\0';
        }
        scores[insertIndex] = score;

        // Ensure we don't exceed MAX_SCORES
        if (currentCount < MAX_SCORES) {
            currentCount++;
        }

        // Write updated scores
        ofstream outputFile("topScore.txt");
        if (outputFile.is_open()) {
            for (int i = 0; i < currentCount; ++i) {
                outputFile << names[i] << " " << scores[i] << endl;
            }
            outputFile.close();
        }
    }
}

int GameRun::getValidInput(const string& prompt) {
    int input;
    while (true) {
        cout << prompt;
        if (cin >> input) {
            return input;
        }

        // Clear error flags and ignore invalid input
        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid input. Please enter a valid number." << endl;
    }
}

char** GameRun::allocateWordArray(int rows, int cols) {
    char** words = new char* [rows];
    for (int i = 0; i < rows; ++i) {
        words[i] = new char[cols];
        memset(words[i], 0, cols);
    }
    return words;
}

void GameRun::deallocateWordArray(char** words, int rows) {
    if (words) {
        for (int i = 0; i < rows; ++i) {
            delete[] words[i];
        }
        delete[] words;
    }
}

void GameRun::displayMainMenu() {
    cout << "Menu:\n";
    cout << "------------------------\n";
    cout << "1. Play game\n";
    cout << "2. How to play game\n";
    cout << "3. View Top Scores\n";
    cout << "4. Exit\n";
    cout << "------------------------\n";
}

void GameRun::displayModeMenu() {
    cout << "------------------------\n";
    cout << "Game Modes:\n";
    cout << "1. Easy\n";
    cout << "2. Hard\n";
    cout << "------------------------\n";
}

void GameRun::displayTopScores() {
    ifstream scoreFile("topScore.txt");
    if (!scoreFile) {
        cout << "Error: Unable to open top score file." << endl;
        return;
    }

    cout << "\n---------------- Top 5 Scores ----------------\n";
    cout << "Rank\tName\t\tScore\n";
    cout << "-------------------------------------------\n";

    string name;
    int score;
    int rank = 1;

    while (scoreFile >> name >> score && rank <= 5) {
        cout << rank << ".\t" << name << "\t\t" << score << endl;
        rank++;
    }
    scoreFile.close();

    Sleep(5000);  // 5-second delay
    system("cls");
}

void GameRun::howToPlay() {
    cout << "\n---------------- How to Play ----------------\n";
    cout << "1. Choose a game mode: Easy or Hard.\n";
    cout << "2. A grid of random letters will be displayed.\n";
    cout << "3. Find the words hidden in the grid.\n";
    cout << "4. Enter the word length and its letter positions.\n";
    cout << "5. You have 5 lives to find all words.\n";
    cout << "------------------------------------------\n";
    Sleep(5000);
    system("cls");
}

void GameRun::displayWordHints(char** words, int wordCount) {
    cout << "\n--- Words to Find ---" << endl;
    for (int i = 0; i < wordCount; ++i) {
        cout << words[i] << endl;
    }
}

bool GameRun::playGame() {
    srand(static_cast<unsigned>(time(0)));

    displayMainMenu();
    int mainChoice = getValidInput("Enter your choice: ");

    if (mainChoice == 1) {
        cout << "Enter your name: ";
        cin.ignore();
        cin.getline(playerName, 50);

        displayModeMenu();
        int modeChoice = getValidInput("Enter your choice: ");

        GameMode* mode = nullptr;
        if (modeChoice == 1) {
            system("cls");
            mode = new EasyGameMode();
        }
        else if (modeChoice == 2) {
            system("cls");
            mode = new HardGameMode();
        }
        else {
            system("cls");
            cout << "Invalid mode selected. Returning to main menu.\n";
            return true;
        }

        int totalScore = 0;
        for (int level = 1; level <= 3; level++) {
            int maxWordsForLevel = (level == 1) ? 3 : (level == 2) ? 5 : 7;
            int gridSize = (level == 1) ? 10 : (level == 2) ? 15 : 20;

            try {
                // Dynamically allocate word array
                char** words = allocateWordArray(50, 20);
                int totalWords = 0;

                ifstream file(mode->getFilename());
                if (!file) {
                    throw runtime_error("Error: Unable to open file " + mode->getFilename());
                }

                char buffer[20];
                while (file >> buffer && totalWords < 50) {
                    // Manual string copy
                    for (int j = 0; buffer[j] != '\0' && j < 19; ++j) {
                        words[totalWords][j] = buffer[j];
                        words[totalWords][j + 1] = '\0';
                    }
                    totalWords++;
                }
                file.close();

                if (totalWords == 0) {
                    throw runtime_error("No words found in the file!");
                }

                // Randomize words manually
                for (int i = 0; i < maxWordsForLevel; ++i) {
                    int index = rand() % totalWords;

                    // Swap words[i] and words[index] manually
                    char temp[20];
                    for (int j = 0; words[i][j] != '\0' && j < 19; ++j) {
                        temp[j] = words[i][j];
                        temp[j + 1] = '\0';
                    }

                    for (int j = 0; words[index][j] != '\0' && j < 19; ++j) {
                        words[i][j] = words[index][j];
                        words[i][j + 1] = '\0';
                    }

                    for (int j = 0; temp[j] != '\0' && j < 19; ++j) {
                        words[index][j] = temp[j];
                        words[index][j + 1] = '\0';
                    }
                }
                totalWords = maxWordsForLevel;

                GameGrid grid(gridSize);
                grid.randomizeGridPlacement(words, maxWordsForLevel);

                displayWordHints(words, maxWordsForLevel);

                int guessedWords = 0;
                int lives = 5;
                int score = 0;

                while (guessedWords < maxWordsForLevel && lives > 0) {
                    cout << "\n--- Level " << level << ": Guess " << maxWordsForLevel << " words! ---" << endl;
                    cout << "Current Score: " << score << " | Lives Remaining: " << lives
                        << " | Guessed words: " << guessedWords << endl;

                    grid.display();

                    char selectedWord[20] = "";
                    int wordLength;

                    while (true) {
                        wordLength = getValidInput(
                            string("\nPlease enter the word length ") +
                            (mode->getFilename() == "easy.txt" ? "(2 to 4)" : "(5 to 7)") + ": "
                        );

                        if (mode->checkWordLength(wordLength)) {
                            break;
                        }
                        else {
                            cout << "Invalid word length. ";
                        }
                    }

                    int selectedPositions[20][2];
                    for (int i = 0; i < wordLength; ++i) {
                        int row, col;
                        row = getValidInput(string("Input row (0-") + to_string(gridSize - 1) + "): ");
                        col = getValidInput(string("Input column (0-") + to_string(gridSize - 1) + "): ");

                        if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
                            cout << "Invalid grid position! Try again." << endl;
                            i--;
                            continue;
                        }

                        selectedWord[i] = grid.getGrid()[row][col];
                        selectedPositions[i][0] = row;
                        selectedPositions[i][1] = col;
                    }
                    selectedWord[wordLength] = '\0';

                    if (WordChecker::validateWord(words, maxWordsForLevel, selectedWord)) {
                        cout << "Correct! You've guessed a word: " << selectedWord << endl;
                        guessedWords++;
                        grid.updateWordPosition(selectedPositions, wordLength);
                        score += 10;
                        system("cls");
                    }
                    else {
                        cout << "Incorrect! Try again." << endl;
                        lives--;
                        system("cls");
                    }
                }

                // Free the dynamically allocated word array
                deallocateWordArray(words, 50);

                // Game over logic
                if (guessedWords < maxWordsForLevel) {
                    totalScore += score;
                    cout << "Game Over! Final Score: " << totalScore << endl;
                    ScoreManager::updateScore(playerName, totalScore);
                    delete mode;
                    for (int i = 0; i < 50; ++i) {
                        delete[] words[i];  // Delete each row (array of chars)
                    }
                    delete[] words;
                    return false;  // Exit the game
                }
                else {
                    totalScore += score;
                    cout << "Level " << level << " completed!" << endl;
                }
            }
            catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
                delete mode;
                return false;  // Exit the game on error
            }
        }

        delete mode;
    }
    else if (mainChoice == 2) {
        
        howToPlay();
    }
    else if (mainChoice == 3) {
        
        displayTopScores();
    }
    else if (mainChoice == 4) {
        return false;
    }
    else {
        cout << "Invalid choice. Please try again." << endl;
    }
    return true;
}
