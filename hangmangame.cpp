#include "hangmangame.h"
#include "mainmenu.h"
#include <QHBoxLayout>
#include <QRegularExpression>

HangmanGame::HangmanGame(QWidget *parent, const QString &difficulty, MainMenu *mainMenu) : QWidget(parent), remainingAttempts(6), isGameWon(false), currentDifficulty(difficulty), mainMenu(mainMenu) {
    // Initialize hangman ASCII art
    hangmanArt = {
        "  +---+\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|   |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        "      |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " /    |\n"
        "      |\n"
        "=========",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " / \\  |\n"
        "      |\n"
        "========="
    };

    // Initialize word bank
    initializeWordBank();

    // Set up the GUI
    wordLabel = new QLabel(this);
    attemptsLabel = new QLabel(this);
    hangmanLabel = new QLabel(this);
    guessInput = new QLineEdit(this);
    guessButton = new QPushButton("Guess", this);
    resetButton = new QPushButton("Reset", this);
    backButton = new QPushButton("Back to Main Menu", this); // Add this line

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(hangmanLabel);
    layout->addWidget(wordLabel);
    layout->addWidget(attemptsLabel);
    layout->addWidget(guessInput);
    layout->addWidget(guessButton);
    layout->addWidget(resetButton);
    layout->addWidget(backButton); // Add this line

    setLayout(layout);

    // Connect signals and slots
    connect(guessButton, &QPushButton::clicked, this, &HangmanGame::guessLetter);
    connect(resetButton, &QPushButton::clicked, this, &HangmanGame::resetGame);
    connect(backButton, &QPushButton::clicked, this, &HangmanGame::backToMainMenu); // Add this line

    // Initialize the game with the selected difficulty
    resetGame();
}

void HangmanGame::initializeWordBank() {
    wordsByDifficulty["Easy"] = {"apple", "banana", "cat", "dog", "elephant"};
    wordsByDifficulty["Medium"] = {"computer", "keyboard", "python", "program", "development"};
    wordsByDifficulty["Hard"] = {"encyclopedia", "hippopotamus", "javascript", "programming", "information"};
}

QString HangmanGame::getRandomWord(const QString& difficulty) {
    const std::vector<QString>& words = wordsByDifficulty[difficulty];
    if (!words.empty()) {
        int index = rand() % words.size();
        return words[index];
    }
    return "";
}

void HangmanGame::guessLetter() {
    QString input = guessInput->text();
    if (input.isEmpty()) {
        return;
    }

    QChar letter = input.at(0).toLower();
    if (!letter.isLetter()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid letter.");
        return;
    }

    if (guessedLetters.find(letter) != guessedLetters.end()) {
        QMessageBox::information(this, "Already Guessed", "You've already guessed this letter.");
        return;
    }

    guessedLetters.insert(letter);
    if (!secretWord.contains(letter)) {
        remainingAttempts--;
    }

    updateDisplay();

    if (secretWord.toLower().toStdString().find(letter.toLatin1()) == std::string::npos) {
        remainingAttempts--;
    } else {
        bool allGuessed = true;
        for (QChar c : secretWord) {
            if (guessedLetters.find(c.toLower()) == guessedLetters.end()) {
                allGuessed = false;
                break;
            }
        }
        if (allGuessed) {
            isGameWon = true;
        }
    }

    if (isGameWon || remainingAttempts <= 0) {
        if (isGameWon) {
            QMessageBox::information(this, "Congratulations!", "You've guessed the word!");
        } else {
            QMessageBox::information(this, "Game Over", "You've run out of attempts. The word was: " + secretWord);
        }

        // Add the game result to the records
        QString result = isGameWon ? "Won" : "Lost";
        QString record = "Word: " + secretWord + ", Result: " + result + ",Wrong Attempts: " + QString::number(6 - remainingAttempts);
        mainMenu->addRecord(record);

        // Go back to the main menu
        backToMainMenu();
    }
}

void HangmanGame::resetGame() {
    secretWord = getRandomWord(currentDifficulty);
    guessedLetters.clear();
    remainingAttempts = 6;
    isGameWon = false;
    updateDisplay();
}

void HangmanGame::updateDisplay() {
    QString visibleWord;
    for (QChar c : secretWord) {
        if (guessedLetters.find(c.toLower()) != guessedLetters.end()) {
            visibleWord += c;
        } else {
            visibleWord += "_";
        }
        visibleWord += " ";
    }
    wordLabel->setText(visibleWord);

    attemptsLabel->setText("Remaining attempts: " + QString::number(remainingAttempts));

    int stage = 6 - remainingAttempts;
    if (stage >= 0 && stage < hangmanArt.size()) {
        hangmanLabel->setText(hangmanArt[stage]);
    }
}

void HangmanGame::backToMainMenu() {
    // Show the main menu and close the game window
    mainMenu->show();
    this->close();
}
