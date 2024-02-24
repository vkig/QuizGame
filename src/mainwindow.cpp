#include "include/mainwindow.h"
#include "include/new_question_dialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // automatikus beolvasás
  ui->stackedWidget->setCurrentWidget(ui->home);
  quiz.read(fileName);
  generateOrder(indexVector, quiz.size());
  newQuiz = quiz;
  ui->filename->setText(quiz.getQuitName());
  // eredménytábla elkészítése
  ui->scoreTable->setColumnCount(2);
  ui->scoreTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
  ui->scoreTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Score"));
  // Új kérdésre lépéskor törlődjön az előző választás ahhoz kell a group
  group = new QButtonGroup(this);
  group->addButton(ui->A_valasz);
  group->addButton(ui->B_valasz);
  group->addButton(ui->C_valasz);
  group->addButton(ui->D_valasz);
  connectUiElementsToActions();
}

MainWindow::~MainWindow() { delete ui; }

// File olvasás Qt explorerrel
void MainWindow::chooseQuiz() {
  ui->stackedWidget->setCurrentWidget(ui->home);
  QStringList fileNames;
  QFileDialog fdial(this, tr("Choose quiz"), "./quizzes",
                    tr("Quiz Files (*.qvz)"));
  fdial.setAcceptMode(QFileDialog::AcceptOpen);
  fdial.show();
  hide();
  if (fdial.exec() && fdial.selectedFiles().size() != 0) {
    fileNames = fdial.selectedFiles();
    QString fileName = fileNames[0];
    quiz.clear();
    if (quiz.read(fileName)) {
      update();
      ui->filename->setText(quiz.getQuitName());
    } else {
      QMessageBox::critical(this, "Error happend while reading the file",
                            "Check the file and try again! Filename: " +
                                fileName);
    }
  }
  show();
}

// File olvasás Windows Explorerrel
void MainWindow::chooseWithExplorer() {
  ui->stackedWidget->setCurrentWidget(ui->home);
  fileName = QFileDialog::getOpenFileName(this, tr("Choose quiz"), "./quizzes",
                                          tr("Quiz Files (*.qvz)"));
  if (fileName.size() > 0) {
    quiz.clear();
    if (quiz.read(fileName)) {
      update();
      ui->filename->setText(quiz.getQuitName());
    } else {
      QMessageBox::critical(this, "Error happend while reading the file",
                            "Check the file and try again! Filename: " +
                                fileName);
    }
  }
}

// File olvasás listwidgettel
void MainWindow::chooseFromFileList() {
    ui->stackedWidget->setCurrentWidget(ui->read_file);
  QDir mappa("./quizzes");
  QStringList l = mappa.entryList(QStringList() << "*.qvz", QDir::Files);
  ui->fileList->clear();
  for (int i = 0; i < l.size(); i++) {
      ui->fileList->addItem(l[i]);
  }
}

// File kiválasztása dupla klikkel
void MainWindow::fileListItemDoubleClicked(QListWidgetItem *item) {
  fileName = "./quizzes/";
  fileName += item->text();
  quiz.clear();
  if (quiz.read(fileName)) {
    update();
    ui->filename->setText(quiz.getQuitName());
  } else {
    QMessageBox::critical(this, "Error happend while reading the file",
                            "Check the file and try again! Filename: " +
                                fileName);
  }
  ui->stackedWidget->setCurrentWidget(ui->home);
}

// Listás választás mégse
void MainWindow::fileChoosingCancelled() {
  ui->stackedWidget->setCurrentWidget(ui->home);
}

// Listás választás OK
void MainWindow::fileChoosingOk() {
  fileName = "./quizzes/";
    fileName += ui->fileList->selectedItems()[0]->text();
  quiz.clear();
  if (quiz.read(fileName)) {
    update();
    ui->filename->setText(quiz.getQuitName());
  } else {
    QMessageBox::critical(this, "Error happend while reading the file",
                          "Check the file and try again! Filename: " +
                                fileName);
  }
  ui->stackedWidget->setCurrentWidget(ui->home);
}

void MainWindow::connectUiElementsToActions()
{
    connect(ui->actionChoose, &QAction::triggered, this, &MainWindow::chooseQuiz);
    connect(ui->actionChooseFromFileList, &QAction::triggered, this, &MainWindow::chooseFromFileList);
    connect(ui->actionChooseWithExplorer, &QAction::triggered, this, &MainWindow::chooseWithExplorer);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
    connect(ui->actionExitToHome, &QAction::triggered, this, &MainWindow::exitToHome);
    connect(ui->actionNewGame, &QAction::triggered, this, &MainWindow::newGame);
    connect(ui->actionEditQuiz, &QAction::triggered, this, &MainWindow::editQuiz);
    connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::goToInfo);
    connect(ui->actionHighscores, &QAction::triggered, this, &MainWindow::goToHighscores);
    connect(ui->actionCreateNewQuiz, &QAction::triggered, this, &MainWindow::createNewQuiz);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->createQuestion, &QPushButton::clicked, this, &MainWindow::createQuestion);
    connect(ui->editQuestion, &QPushButton::clicked, this, &MainWindow::editQuestion);
    connect(ui->removeQuestion, &QPushButton::clicked, this, &MainWindow::removeQuestion);
    connect(ui->nextQuestion, &QPushButton::clicked, this, &MainWindow::moveToNextQuestion);
    connect(ui->saveQuiz, &QPushButton::clicked, this, &MainWindow::saveQuiz);
    connect(ui->fileChoosingCancel, &QPushButton::clicked, this, &MainWindow::fileChoosingCancelled);
    connect(ui->fileChoosingOk, &QPushButton::clicked, this, &MainWindow::fileChoosingOk);
    connect(ui->quizName, &QLineEdit::editingFinished, this, &MainWindow::quizNameEditing);
    connect(ui->timelimitEditor, &QLineEdit::editingFinished, this, &MainWindow::timelimitEditing);
    connect(ui->fileList, &QListWidget::itemDoubleClicked, this, &MainWindow::fileListItemDoubleClicked);
}

// Ablak bezárása
void MainWindow::exit() { close(); }

// Játék indítása gomb megnyomása esetén fut
void MainWindow::startGame() {
  if (ui->filename->text().size() != 0 && ui->playerName->text().size() != 0) {
    if (indexVector.size() != quiz.size()) {
      generateOrder(indexVector, quiz.size());
    }
    ui->stackedWidget->setCurrentWidget(ui->game);
    actualIndex = getRandomIndex(indexVector);
    actualScore = 0;
    questionIndex = 1;
    playerName = ui->playerName->text();
    timer->stop();
    timer->start(1000);
    disconnect(timer, SIGNAL(timeout()), this, SLOT(timer_thick()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_thick()));
    nextQuestion();
    removeSelection();
  } else if (ui->filename->text().size() == 0) {
    QMessageBox::warning(this, "Error", "You haven't chosen a quiz!");
      chooseQuiz();
  } else if (ui->playerName->text().size() == 0) {
    QMessageBox::warning(this, "Error", "You haven't set the player name!");
  }
}

//"Következő kérdés" gomb megnyomása esetén
void MainWindow::moveToNextQuestion() {
  if (actualIndex != -1) {
    checkAnswear();
    nextQuestion();
    removeSelection();
  }
}

// Kiválasztás eltávolítása (köv kérdésnél ne legyen egyik se előre kiválasztva)
void MainWindow::removeSelection() {
  QAbstractButton *checked = group->checkedButton();
  if (checked) {
    group->setExclusive(false);
    checked->setChecked(false);
    group->setExclusive(true);
  }
}

// Visszaszámláló a kérdések kitöltésére
void MainWindow::timer_thick() {
  if (ui->stackedWidget->currentWidget() == ui->game) {
    int timeleftMinutes = remainingTime / 60;
    int timeleftSeconds = remainingTime % 60;
    ui->timeleftMinutes->display(timeleftMinutes);
    ui->timeleftSeconds->display(timeleftSeconds);
    remainingTime--;
    if (remainingTime == -1 && questionIndex <= quiz.size()) {
      remainingTime = quiz.getTimelimit();
      checkAnswear();
      nextQuestion();
    }
  } else {
      timer->stop();
  }
}

// Az aktuális válasz kiértékelése
void MainWindow::checkAnswear() {
  if (questionIndex <= quiz.size() + 1) {
    if (actualIndex != -1) {
      if (ui->A_valasz->isChecked() &&
          ui->A_valasz->text() == quiz.getCorrectAnswear(actualIndex)) {
        actualScore++;
      }
      if (ui->B_valasz->isChecked() &&
          ui->B_valasz->text() == quiz.getCorrectAnswear(actualIndex)) {
        actualScore++;
      }
      if (ui->C_valasz->isChecked() &&
          ui->C_valasz->text() == quiz.getCorrectAnswear(actualIndex)) {
        actualScore++;
      }
      if (ui->D_valasz->isChecked() &&
          ui->D_valasz->text() == quiz.getCorrectAnswear(actualIndex)) {
        actualScore++;
      }
      actualIndex = getRandomIndex(indexVector);
    }
    questionIndex++;
  }
}

// Az új kérdéshez kiiratás, utolsó kérdés esetén az eredmény
void MainWindow::nextQuestion() {
  if (actualIndex != -1) {
    remainingTime = quiz.getTimelimit();
      timer_thick();
    ui->actualQuestionIndex->setText(QString::number(questionIndex) + "/" +
                                         QString::number(quiz.size()));
    ui->actualQuiz->setText(quiz.getQuitName());
    ui->aktualis_kerdes->setTitle(quiz.getQuestion(actualIndex));
    QVector<QString> v = quiz.getAnswears(actualIndex);
    QVector<int> questions_index_vector;
    generateOrder(questions_index_vector, 4);
    ui->A_valasz->setText(v[getRandomIndex(questions_index_vector)]);
    ui->B_valasz->setText(v[getRandomIndex(questions_index_vector)]);
    ui->C_valasz->setText(v[getRandomIndex(questions_index_vector)]);
    ui->D_valasz->setText(v[getRandomIndex(questions_index_vector)]);
    ui->actualScore->setNum(actualScore);
  }
  if (actualIndex == -1 && questionIndex == quiz.size() + 1) {
      timer->stop();
      disconnect(timer, SIGNAL(timeout()), this, SLOT(timer_thick()));
    remainingTime = 0;
      timer_thick();
    ui->actualScore->setNum(actualScore);
    QMessageBox::information(this, "Result:",
                             playerName + " got " + QString::number(actualScore) +
                                 " points.");
    quiz.saveResults(playerName, actualScore);
  }
}

// Random elem kivétele egy paraméterként kapott index vektorból
int MainWindow::getRandomIndex(QVector<int> &v) {
  if (v.size() > 0) {
    int index = v[rand() % v.size()];
    QVector<int>::iterator it = std::find(v.begin(), v.end(), index);
    v.erase(it);
    return index;
  }
  return -1;
}

// Kilépés a kezdőlapra
void MainWindow::exitToHome() {
  ui->stackedWidget->setCurrentWidget(ui->home);
}

// Új játék kezdése
void MainWindow::newGame() {
  if (ui->filename->text().size() != 0 && ui->playerName->text().size() != 0) {
    if (indexVector.size() != quiz.size()) {
      generateOrder(indexVector, quiz.size());
    }
    ui->stackedWidget->setCurrentWidget(ui->game);
    actualIndex = getRandomIndex(indexVector);
    actualScore = 0;
    questionIndex = 1;
    playerName = ui->playerName->text();
    timer->stop();
    timer->start(1000);
    disconnect(timer, SIGNAL(timeout()), this, SLOT(timer_thick()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_thick()));
    nextQuestion();
    removeSelection();
  } else if (ui->filename->text().size() == 0) {
      QMessageBox::warning(this, "Error", "You haven't chosen a quiz!");
      chooseQuiz();
  } else if (ui->playerName->text().size() == 0) {
      QMessageBox::warning(this, "Error", "You haven't set the player name!");
      ui->stackedWidget->setCurrentWidget(ui->home);
  }
}

// Index vektor létrehozása
void MainWindow::generateOrder(QVector<int> &indexVector, int size) {
  indexVector.clear();
  for (int i = 0; i < size; i++) {
    indexVector.push_back(i);
  }
}

// Meglévő kérdéssor szerkesztése
void MainWindow::editQuiz() {
    ui->stackedWidget->setCurrentWidget(ui->editor);
  newQuiz = quiz;
  update();
}

// Új kérdéssor létrehozása
void MainWindow::createNewQuiz() {
    ui->stackedWidget->setCurrentWidget(ui->editor);
  newQuiz.clear();
  update();
}

// Új kérdés gomb megnyomásakor előugró szerkesztő ablak
void MainWindow::createQuestion() {
  QPair<QString, QVector<QString>> uj_kerdes;
  NewQuestionDialog *d = new NewQuestionDialog(this);
  d->setWindowTitle("Új kérdés létrehozása");
  d->setModal(true);
  d->exec();
  if (d->isHidden()) {
    uj_kerdes = d->getQuestionAndAnswears();
    if (uj_kerdes.first.size() > 0) {
      newQuiz.newQuestion(uj_kerdes.first, uj_kerdes.second);
    }
    update();
  }
  d->close();
  delete d;
}

// Kérdés szerkesztése megnyomásakor előugró szerkesztő ablak
void MainWindow::editQuestion() {
  QPair<QString, QVector<QString>> szerkesztett;
  if (ui->questions->currentRow() >= 0 &&
      ui->questions->currentRow() < newQuiz.size()) {
    NewQuestionDialog *d = new NewQuestionDialog(
        this, newQuiz.getQuestion(ui->questions->currentRow()),
        newQuiz.getAnswears(ui->questions->currentRow()));
    d->setWindowTitle("Edit question");
    d->setModal(true);
    d->exec();
    if (d->isHidden() && d->isaccepted()) {
      szerkesztett = d->getQuestionAndAnswears();
      newQuiz.editQuestion(ui->questions->currentRow(), szerkesztett.first,
                           szerkesztett.second);
      update();
    }
    d->close();
    delete d;
  }
}

// Kérdés törlése
void MainWindow::removeQuestion() {
  if (ui->questions->currentRow() >= 0 &&
      ui->questions->currentRow() < quiz.size()) {
    newQuiz.removeQuestion(ui->questions->currentRow());
    update();
  }
}

// A szeresztő felületen a kviz kérdéseinek megjelenítése
void MainWindow::update() {
  ui->questions->clear();
    if (ui->stackedWidget->currentWidget() == ui->editor) {
      ui->quizName->setText(newQuiz.getQuitName());
        ui->timelimitEditor->setText(QString::number(newQuiz.getTimelimit()));
    for (int i = 0; i < newQuiz.size(); i++) {
      ui->questions->addItem(newQuiz.getQuestion(i));
    }
  }
}

// Szerkesztettek mentése
void MainWindow::saveQuiz() {
  QString filePath = QFileDialog::getExistingDirectory(
      this, tr("Mentés helyének kiválasztása"), tr("./kvizek"));
  qDebug() << newQuiz.exportQuiz(filePath);
  quiz = newQuiz;
}

// A kviznév mentése
void MainWindow::quizNameEditing() {
    newQuiz.addQuizName(ui->quizName->text());
}

// Időkorlát mentése
void MainWindow::timelimitEditing() {
    QString s = ui->timelimitEditor->text();
  int korlat = s.toInt();
  newQuiz.setTimelimit(korlat);
}

// Az információk előhívása
void MainWindow::goToInfo() {
  ui->stackedWidget->setCurrentWidget(ui->info);
}

// Eredménytábla újraméretezése az ablakméret változásával
void MainWindow::resizeEvent(QResizeEvent *event) {
  ui->scoreTable->setColumnWidth(0, ui->scoreTable->width() / 2);
  ui->scoreTable->setColumnWidth(1, ui->scoreTable->width() / 2);
}

// Korábbi eredmények megtekintése
void MainWindow::goToHighscores() {
  ui->scoreTable->setColumnWidth(0, ui->scoreTable->width() / 2);
  ui->scoreTable->setColumnWidth(1, ui->scoreTable->width() / 2);
  ui->stackedWidget->setCurrentWidget(ui->highscores);
  QVector<QString> eredmenyek = quiz.loadPreviousResults();
  ui->scoreTable->setRowCount(eredmenyek.size() / 2);
  for (int i = 0; i < eredmenyek.size(); i++) {

    ui->scoreTable->setItem(i / 2, 0, new QTableWidgetItem(eredmenyek[i]));
    i++;
    ui->scoreTable->setItem((i - 1) / 2, 1,
                            new QTableWidgetItem(eredmenyek[i]));
  }
}
