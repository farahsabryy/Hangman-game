#ifndef HANGMANGAME_H
#define HANGMANGAME_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include <set>
#include <map>
#include <vector>


class MainMenu; // Forward declaration

class HangmanGame : public QWidget {
    Q_OBJECT

public:
    HangmanGame(QWidget *parent = nullptr, const QString &difficulty = "Easy", MainMenu *mainMenu = nullptr); // Add MainMenu pointer

private slots:
    void guessLetter();
    void resetGame();
    void backToMainMenu(); // Add this line

private:
    void updateDisplay();
    void initializeWordBank();
    QString getRandomWord(const QString& difficulty);

    QString secretWord;
    std::set<QChar> guessedLetters;
    int remainingAttempts;
    bool isGameWon;
    QString currentDifficulty;
    MainMenu *mainMenu; // Add this line

    QLabel *wordLabel;
    QLabel *attemptsLabel;
    QLabel *hangmanLabel;
    QLineEdit *guessInput;
    QPushButton *guessButton;
    QPushButton *resetButton;
    QPushButton *backButton; // Add this line

    QStringList hangmanArt;
    std::map<QString, std::vector<QString>> wordsByDifficulty;
};

#endif // HANGMANGAME_H
