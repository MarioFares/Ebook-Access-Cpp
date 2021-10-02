QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#QMAKE_CXXFLAGS += -std=c++17


SOURCES += \
    addbookdialog.cpp \
    addbooksdialog.cpp \
    bookdetailswindow.cpp \
    bulkdetailsdialog.cpp \
    cleanebooksdialog.cpp \
    common.cpp \
    extselectiondialog.cpp \
    insertlinkdialog.cpp \
    inserttabledialog.cpp \
    linkcollectiondialog.cpp \
    linkmanagerwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    queries.cpp \
    searchnamedialog.cpp \
    summarywindow.cpp

HEADERS += \
    addbookdialog.h \
    addbooksdialog.h \
    bookdetailswindow.h \
    bulkdetailsdialog.h \
    cleanebooksdialog.h \
    common.h \
    extselectiondialog.h \
    insertlinkdialog.h \
    inserttabledialog.h \
    linkcollectiondialog.h \
    linkmanagerwindow.h \
    mainwindow.h \
    queries.h \
    searchnamedialog.h \
    summarywindow.h

FORMS += \
    addbookdialog.ui \
    addbooksdialog.ui \
    bookdetailswindow.ui \
    bulkdetailsdialog.ui \
    cleanebooksdialog.ui \
    extselectiondialog.ui \
    insertlinkdialog.ui \
    inserttabledialog.ui \
    linkcollectiondialog.ui \
    linkmanagerwindow.ui \
    mainwindow.ui \
    searchnamedialog.ui \
    summarywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    style.qss \
    todo

RESOURCES += \
    images.qrc \
    style.qrc

RC_ICONS = ./icons/icon.ico


