QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/new_question_dialog.cpp \
    src/quiz.cpp

HEADERS += \
    include/mainwindow.h \
    include/new_question_dialog.h \
    include/quiz.h

FORMS += \
    ui/mainwindow.ui \
    ui/new_question_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

copydata.commands = $(COPY_DIR) $$PWD/quizzes $$OUT_PWD | $(COPY_DIR) $$PWD/saved_data $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.quizzes)
export(copydata.saved_data)
QMAKE_EXTRA_TARGETS += first copydata

RESOURCES += \
    res.qrc
RC_FILE = quiz.rc
