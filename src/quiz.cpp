#include "include/quiz.h"

kviz::kviz()
{

}

//Kviz beolvasása fileból
bool kviz::read(QString filename)
{
    QFile befile(filename);
    if(!befile.open(QFile::ReadOnly))
    {
        return false;
    }
    else
    {
        QTextStream stream(&befile);
        stream.setEncoding(QStringConverter::Encoding::Utf8);
        QString line;
        line=stream.readLine();
        line=line.mid(line.indexOf(':')+1, line.size()-line.indexOf(':')+1);
        quizName=line;
        line=stream.readLine();
        timelimit=line.toInt();
        int i=0;
        QVector<QString> v;
        while(!stream.atEnd())
        {
            line=stream.readLine();
            line=line.mid(line.indexOf(':')+1, line.size()-line.indexOf(':')+1);
            switch(i%6)
            {
            case 0: v.clear(); questions.push_back(line); break;
                case 1:
                case 2:
                case 3:
                case 4: v.push_back(line); break;
                case 5: line=line.left(1); v.push_back(line); answears.push_back(v); break;
            }
            i++;
        }
        return true;
    }
}


//A filenév megfelelő formátumba konvertálása
QString kviz::convert(QString konvertalando)
{
    for(int i=0;i<konvertalando.size();i++)
    {
        if(konvertalando[i] == ' ')
        {
            konvertalando.remove(i, 1);
            konvertalando.insert(i, "_");
        }
        while(konvertalando[i]== '_' && i+1<konvertalando.size() && konvertalando[i+1]==' ')
        {
            konvertalando.remove(i+1, 1);
        }
        if(konvertalando[i].isUpper())
        {
            QChar c=konvertalando[i].toLower();
            konvertalando.remove(i, 1);
            konvertalando.insert(i, c);
        }  
    }
    if(konvertalando[konvertalando.size()-1]=='_')
    {
        konvertalando.remove(konvertalando.size()-1, 1);
    }
    return konvertalando;
}


//Ha korábban létezett már a file akkor indexel
QString kviz::generateName(QString &dir, int verzio)
{
    QString filename;
    int i=verzio+1;
    if(verzio==0)
    {
        filename=convert(quizName)+"_"+QString::number(i);
    }
    else if(verzio>0)
    {
        filename=convert(quizName)+"_"+QString::number(i);
    }
    if(QFile::exists(dir+"/"+filename+".qvz"))
    {
        filename=generateName(dir, i);
    }
    return filename;
}


//A kvíz kiírása fileba
bool kviz::exportQuiz(QString hova)
{
    if(QFile::exists(hova+"/"+convert(quizName)+".qvz"))
    {
        hova=hova+"/"+generateName(hova, 0)+".qvz";
    }
    else
    {
        hova=hova+"/"+convert(quizName)+".qvz";
    }
    QFile kifile(hova);
    if(!kifile.open(QFile::WriteOnly))
    {
        return false;
    }
    QTextStream stream(&kifile);
    stream.setEncoding(QStringConverter::Encoding::Utf8);
    stream<<"Name:"<<quizName<<"\n";
    stream<<timelimit<<"\n";
    for(int i=0;i<questions.size();i++)
    {
        stream<<"Q:"<<questions[i]<<"\n";
        for(int j=0;j<answears[i].size(); j++)
        {
            switch(j)
            {
            case 0: stream<<"A:"<<answears[i][j]<<"\n"; break;
            case 1: stream<<"B:"<<answears[i][j]<<"\n"; break;
            case 2: stream<<"C:"<<answears[i][j]<<"\n"; break;
            case 3: stream<<"D:"<<answears[i][j]<<"\n"; break;
            case 4: stream<<"Ans:"<<answears[i][j]<<";"<<"\n";
            }
        }
    }
    return true;
}


//Új kérdés hozzáadása
void kviz::newQuestion(QString kerdes, QVector<QString> valasz)
{
    questions.push_back(kerdes);
    answears.push_back(valasz);
}


//Adott indexű kérdés törlése
void kviz::removeQuestion(int index)
{
    QVector<QString>::iterator it;
    it = std::find(questions.begin(), questions.end(), questions[index]);
    questions.erase(it);
}


//Adott indexű kérdés módosítása megadott paraméterekkel
void kviz::editQuestion(int index, QString kerdes, QVector<QString> valasz)
{
    questions[index]=kerdes;
    answears[index]=valasz;
}


//A kviz nevének lekérése
QString kviz::getQuitName()
{
    return quizName;
}


//A kviz méretének lekérése
int kviz::size()
{
    return questions.size();
}


//Adott indexű kérdés lekérése
QString kviz::getQuestion(int index)
{
    return questions[index];
}


//Adott indexű kérédés válaszlehetőségeinek lekérése
QVector<QString> kviz::getAnswears(int index)
{
    return answears[index];
}


//Adott indexű kérdés helyes válaszának lekérése
QString kviz::getCorrectAnswear(int index)
{
    if(index<answears.size())
    {
        if(answears[index][4]=="A")
        {
            return answears[index][0];
        }
        if(answears[index][4]=="B")
        {
            return answears[index][1];
        }
        if(answears[index][4]=="C")
        {
            return answears[index][2];
        }
        if(answears[index][4]=="D")
        {
            return answears[index][3];
        }
    }
    return "";
}


//A kviz törlése
void kviz::clear()
{
    quizName="";
    timelimit=60;
    if(questions.size()>0)
    {
        questions.clear();
        answears.clear();
    }
}


//Kviznév hozzáadása
void kviz::addQuizName(QString new_name)
{
    quizName=new_name;
}



//Eredmény kiírása file-ba
bool kviz::saveResults(QString name, int elert_pontszam)
{
    QString converted;
    converted=convert(quizName);
    QDir dir("./");
    dir.mkdir("saved_data");
    dir.cd("saved_data");
    QString korabbi(" című kérdéssor korábbi eredményei:");
    QString hova=dir.absolutePath();
    hova=hova+"/"+converted+"_score.score";
    QFile kifile(hova);
    QTextStream stream(&kifile);
    if(kifile.exists())
    {
        if(kifile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            stream<<name<<";"<<elert_pontszam<<"\n";
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(!kifile.open(QFile::WriteOnly))
        {
            return false;
        }
        stream.setEncoding(QStringConverter::Encoding::Utf8);
        stream<<name<<";"<<elert_pontszam<<"\n";
        kifile.flush();
        kifile.close();
    }
    return true;
}


//Korábbi eredmények beolvasása fileból
QVector<QString> kviz::loadPreviousResults()
{
    QVector<QString> eredmeny=QVector<QString> (0);
    QDir dir("./");
    QDir dir2("./saved_data");
    if(!dir2.exists())
        dir.mkdir("saved_data");
    dir.cd("saved_data");
    QString hova=dir.absolutePath();
    hova=hova+"/"+convert(quizName)+"_score.score";
    QFile f(hova);
    QTextStream stream(&f);
    if(f.exists() && f.open(QFile::ReadOnly))
    {
        QString line;
        QString line2;
        while(!stream.atEnd())
        {
            line=stream.readLine();
            line2=line.mid(line.indexOf(";")+1,  line.size()-line.indexOf(";"));
            line=line.mid(0, line.indexOf(";"));
            eredmeny.push_back(line);
            eredmeny.push_back(line2);
        }
        return eredmeny;
    }
    else
    {
        return eredmeny;
    }
}


//Időkorlát lekérése
int kviz::getTimelimit()
{
    return timelimit;
}


//Időkorlát beállítása
void kviz::setTimelimit(int korlat)
{
    timelimit=korlat;
}
