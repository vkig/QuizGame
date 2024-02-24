#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "quiz.h"
#include <QTimer>
#include <QButtonGroup>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QPair>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update();
    void nextQuestion();
    void checkAnswear();
    void generateOrder(QVector<int> &indexVector, int size);
    int getRandomIndex(QVector<int> &indexVector);
    void removeSelection();

private slots:
    void resizeEvent(QResizeEvent* event);
    void timer_thick();
    void createNewQuiz();
    void chooseQuiz();
    void chooseWithExplorer();
    void exit();
    void startGame();
    void exitToHome();
    void newGame();
    void editQuiz();
    void createQuestion();
    void editQuestion();
    void removeQuestion();
    void moveToNextQuestion();
    void saveQuiz();
    void quizNameEditing();
    void goToInfo();
    void goToHighscores();
    void timelimitEditing();
    void chooseFromFileList();
    void fileListItemDoubleClicked(QListWidgetItem *item);
    void fileChoosingCancelled();
    void fileChoosingOk();

private:
    Ui::MainWindow *ui;
    void connectUiElementsToActions();
    QString fileName="./quizzes/állati_kviz.qvz";
    kviz quiz;
    kviz newQuiz;
    QString playerName;
    int actualIndex;
    int actualScore;
    int questionIndex;
    QTimer* timer = new QTimer(this);
    QButtonGroup* group;
    int remainingTime;
    QVector<int> indexVector;
};
#endif // MAINWINDOW_H
