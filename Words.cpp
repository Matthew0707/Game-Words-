#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <unordered_set>


std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}


bool isWordValid(const std::string& word, const std::string& source) {
    std::string sourceCopy = source;
    for (char c : word) {
        size_t pos = sourceCopy.find(tolower(c));
        if (pos != std::string::npos) {
            sourceCopy.erase(pos, 1);
        }
        else {
            return false;
        }
    }
    return true;
}


std::vector<std::string> generateSourceWords() {
    return {
        "программирование",
        "компьютерный",
        "алгоритмизация",
        "интерфейсный",
        "разработка",
        "дизайнерский",
        "информатика",
        "операционная",
        "аппаратный",
        "технология"
    };
}


std::string chooseRandomSourceWord(const std::vector<std::string>& wordList) {
    srand(static_cast<unsigned int>(time(0)));
    int index = rand() % wordList.size();
    return wordList[index];
}


void displayCurrentPlayer(int playerNum, int totalPlayers) {
    std::cout << "\nХод игрока " << playerNum << " из " << totalPlayers << ":\n";
}


int handlePlayerTurn(const std::string& sourceWord, std::unordered_set<std::string>& usedWords, std::vector<int>& scores, int currentPlayer) {
    std::cout << "Введите слово: ";
    std::string input;
    std::getline(std::cin, input);

    std::string trimmedInput = trimString(input);
    if (trimmedInput.empty()) {
        std::cout << "Ход пропущен.\n";
        return 1;
    }

    std::string wordLower = trimmedInput;
    std::transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);

    if (usedWords.find(wordLower) != usedWords.end()) {
        std::cout << "Слово уже было введено ранее. Вы теряете " << wordLower.length() << " баллов.\n";
        scores[currentPlayer] -= wordLower.length();
        return 0;
    }

    if (isWordValid(wordLower, sourceWord)) {
        std::cout << "Слово принято. Вы получаете " << wordLower.length() << " баллов.\n";
        scores[currentPlayer] += wordLower.length();
        usedWords.insert(wordLower);
        return 0;
    }
    else {
        int wrongLetters = 0;
        std::string sourceCopy = sourceWord;
        for (char c : wordLower) {
            size_t pos = sourceCopy.find(c);
            if (pos != std::string::npos) {
                sourceCopy.erase(pos, 1);
            }
            else {
                wrongLetters++;
            }
        }
        std::cout << "Неверное слово. Вы теряете " << wrongLetters << " баллов.\n";
        scores[currentPlayer] -= wrongLetters;
        return 0;
    }
}


void determineWinner(const std::vector<int>& scores) {
    std::cout << "\nРезультаты игры:\n";
    int maxScore = scores[0];
    std::vector<int> winners = {0};
    for (int i = 1; i < static_cast<int>(scores.size()); ++i) {
        std::cout << "Игрок " << (i + 1) << ": " << scores[i] << " баллов\n";
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winners.clear();
            winners.push_back(i);
        }
        else if (scores[i] == maxScore) {
            winners.push_back(i);
        }
    }
    std::cout << "Игрок 1: " << scores[0] << " баллов\n";
    if (scores[0] > maxScore) {
        maxScore = scores[0];
        winners.clear();
        winners.push_back(0);
    }
    else if (scores[0] == maxScore) {
        winners.push_back(0);
    }

    if (winners.size() == 1) {
        std::cout << "\nПобедил игрок " << (winners[0] + 1) << " с " << maxScore << " баллами!\n";
    }
    else {
        std::cout << "\nНичья между игроками: ";
        for (size_t i = 0; i < winners.size(); ++i) {
            std::cout << winners[i] + 1;
            if (i < winners.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ". Победила дружба!\n";
    }
}


void playGame() {
    std::vector<std::string> wordList = generateSourceWords();
    std::string sourceWord = chooseRandomSourceWord(wordList);
    std::string sourceWordLower = sourceWord;
    std::transform(sourceWordLower.begin(), sourceWordLower.end(), sourceWordLower.begin(), ::tolower);

    std::cout << "Добро пожаловать в игру «СЛОВА»!\n";
    std::cout << "Исходное слово: " << sourceWord << "\n";

    int numPlayers;
    while (true) {
        std::cout << "Введите количество игроков (от 2 до 4): ";
        std::cin >> numPlayers;
        if (std::cin.fail() || numPlayers < 2 || numPlayers > 4) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Неверный ввод. Пожалуйста, введите число от 2 до 4.\n";
        }
        else {
            std::cin.ignore(10000, '\n');
            break;
        }
    }

    std::vector<int> scores(numPlayers, 0);
    std::unordered_set<std::string> usedWords;

    bool gameOver = false;
    int consecutiveSkips = 0;
    int currentPlayer = 0;

    while (!gameOver) {
        displayCurrentPlayer(currentPlayer + 1, numPlayers);
        int skipTurn = handlePlayerTurn(sourceWordLower, usedWords, scores, currentPlayer);
        if (skipTurn == 1) {
            consecutiveSkips++;
        }
        else {
            consecutiveSkips = 0;
        }

        if (consecutiveSkips >= numPlayers) {
            gameOver = true;
            std::cout << "\nВсе игроки пропустили ход подряд. Игра окончена.\n";
        }
        else {
            currentPlayer = (currentPlayer + 1) % numPlayers;
        }
    }

    determineWinner(scores);
}

int main() {
    playGame();
    return 0;
}
