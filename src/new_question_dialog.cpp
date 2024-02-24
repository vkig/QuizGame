#include "include/new_question_dialog.h"
#include "ui_new_question_dialog.h"

//Új ablak létrehozása megadott kvízkérdéssel
NewQuestionDialog::NewQuestionDialog(QWidget *parent, QString question, QVector<QString> valasz):
    QDialog(parent),
    ui(new Ui::NewQuestionDialog)
{
    ui->setupUi(this);
    ui->kerdes->setText(question);
    if(valasz.size()!=0)
    {
            ui->A_lehetoseg->setText(valasz[0]);
            ui->B_lehetoseg->setText(valasz[1]);
            ui->C_lehetoseg->setText(valasz[2]);
            ui->D_lehetoseg->setText(valasz[3]);
            ui->helyes_valasz->setCurrentText(valasz[4]);
    }
}

NewQuestionDialog::~NewQuestionDialog()
{
    delete ui;
}


//A megszerkesztett kérdés lekérése
QPair<QString, QVector<QString> > NewQuestionDialog::getQuestionAndAnswears()
{
    return question;
}


//Ha elfogadjuk a szerkesztést akkor a lekérhető vektorba beillesztjük az adatokat
void NewQuestionDialog::on_buttonBox_accepted()
{
    question.first=ui->kerdes->text();
    question.second.push_back(ui->A_lehetoseg->text());
    question.second.push_back(ui->B_lehetoseg->text());
    question.second.push_back(ui->C_lehetoseg->text());
    question.second.push_back(ui->D_lehetoseg->text());
    question.second.push_back(ui->helyes_valasz->currentText());
    accepted=true;
}


//Ha nem fogadjuk el, akkor nem történik semmi
void NewQuestionDialog::on_buttonBox_rejected()
{
    accepted=false;
}


//Lekérhetjük, hogy elfogadtuk-e a szerkesztést
bool NewQuestionDialog::isaccepted()
{
    return accepted;
}


