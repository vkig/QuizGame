#ifndef NEW_QUESTION_DIALOG_H
#define NEW_QUESTION_DIALOG_H

#include <QDialog>
#include <QVector>
#include <QString>
#include <QDebug>

namespace Ui {
class NewQuestionDialog;
}

class NewQuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewQuestionDialog(QWidget *parent = nullptr, QString question="", QVector<QString> answear = QVector<QString>(0));
    ~NewQuestionDialog();
    QPair<QString, QVector<QString> > getQuestionAndAnswears();
    bool isaccepted();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::NewQuestionDialog *ui;
    QPair<QString, QVector<QString> > question;
    bool accepted=false;
};

#endif // NEW_QUESTION_DIALOG_H
