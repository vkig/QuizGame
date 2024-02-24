#ifndef QUIZ_H
#define QUIZ_H
#include <QString>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QUrl>
#include <QDir>

class kviz
{
public:
    kviz();
    bool read(QString filename);
    QString convert(QString toBeConverted);
    QString generateName(QString &filename, int version);
    bool exportQuiz(QString path = "./quizzes");
    void newQuestion(QString question, QVector<QString> answear);
    void removeQuestion(int index);
    void editQuestion(int index, QString question, QVector<QString> answear);
    void clear();
    void addQuizName(QString newName);
    bool saveResults(QString name, int score);
    QVector<QString> loadPreviousResults();
    QVector<QString> getAnswears(int index);
    QString getCorrectAnswear(int index);
    QString getQuitName();
    QString getQuestion(int index);
    int size();
    int getTimelimit();
    void setTimelimit(int limit);
private:
    QString quizName;
    QVector<QString> questions;
    QVector<QVector<QString> > answears;
    int timelimit=60;
};

#endif // QUIZ_H
