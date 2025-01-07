#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <stack>
#include <QFile>
#include <QTextStream>

class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu(); // Add destructor to save records on exit
    void addRecord(const QString &record);

private slots:
    void startGame();
    void showRecords();

private:
    void loadRecords(); // Add this function
    void saveRecords(); // Add this function

    QLabel *backgroundLabel;
    QPushButton *startButton;
    QPushButton *recordsButton;
    std::stack<QString> gameRecords;
};

#endif // MAINMENU_H
