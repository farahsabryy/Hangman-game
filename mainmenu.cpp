#include "mainmenu.h"
#include "hangmangame.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    // Load the background image
    QPixmap background(":/images/152241700-6beb6cd4-62e4-4830-84c3-7b67997bae49.qrc.jpeg");

    // Debug: Check if the image is loaded correctly
    if (background.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load the background image.");
    }

    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(background);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create buttons
    startButton = new QPushButton("Start Game", this);
    recordsButton = new QPushButton("Show Records", this);

    // Set button styles
    startButton->setStyleSheet("QPushButton { font-size: 20px; padding: 10px; }");
    recordsButton->setStyleSheet("QPushButton { font-size: 20px; padding: 10px; }");

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(backgroundLabel);
    layout->addWidget(startButton);
    layout->addWidget(recordsButton);
    setLayout(layout);

    // Connect buttons to slots
    connect(startButton, &QPushButton::clicked, this, &MainMenu::startGame);
    connect(recordsButton, &QPushButton::clicked, this, &MainMenu::showRecords);

    // Load records from file when the application starts
    loadRecords();
}

MainMenu::~MainMenu() {
    // Save records to file when the application exits
    saveRecords();
}

void MainMenu::startGame() {
    // Show difficulty selection dialog
    QStringList difficulties = {"Easy", "Medium", "Hard"};
    bool ok;
    QString difficulty = QInputDialog::getItem(this, "Select Difficulty", "Choose a difficulty level:", difficulties, 0, false, &ok);

    if (ok && !difficulty.isEmpty()) {
        // Hide the main menu
        this->hide();

        // Start the Hangman game with the selected difficulty
        HangmanGame *game = new HangmanGame(nullptr, difficulty, this); // Pass 'this' to HangmanGame
        game->show();
    }
}

void MainMenu::showRecords() {
    QString records;
    std::stack<QString> tempStack = gameRecords; // Copy the stack to avoid modifying the original

    while (!tempStack.empty()) {
        records += tempStack.top() + "\n"; // Get the top record
        tempStack.pop(); // Remove the top record
    }

    if (records.isEmpty()) {
        records = "No records available.";
    }

    QMessageBox::information(this, "Game Records", records);
}

void MainMenu::addRecord(const QString &record) {
    gameRecords.push(record); // Add the record to the stack
}

void MainMenu::loadRecords() {
    QFile file("game_records.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString record = in.readLine();
            gameRecords.push(record); // Load records into the stack
        }
        file.close();
    }
}

void MainMenu::saveRecords() {
    QFile file("game_records.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        std::stack<QString> tempStack = gameRecords; // Copy the stack to avoid modifying the original

        while (!tempStack.empty()) {
            out << tempStack.top() << "\n"; // Save the top record
            tempStack.pop(); // Remove the top record
        }

        file.close();
    }
}
