QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += /std:c++17
QMAKE_CXXFLAGS += /O2


SOURCES += \
    src/addbookdialog.cpp \
    src/addbooksdialog.cpp \
    src/bulkdetailsdialog.cpp \
    src/cleanebooksdialog.cpp \
    src/colorpickerwidget.cpp \
    src/common.cpp \
    src/dataviewerwindow.cpp \
    src/extselectiondialog.cpp \
    src/insertlinkdialog.cpp \
    src/inserttabledialog.cpp \
    src/linkcollectiondialog.cpp \
    src/linkmanagerwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/queries.cpp \
    src/searchnamedialog.cpp \
    src/settingswindow.cpp \
    src/summarywindow.cpp \
    src/yesnodialog.cpp \


HEADERS += \
    include/addbookdialog.h \
    include/addbooksdialog.h \
    include/bulkdetailsdialog.h \
    include/cleanebooksdialog.h \
    include/colorpickerwidget.h \
    include/common.h \
    include/dataviewerwindow.h \
    include/extselectiondialog.h \
    include/insertlinkdialog.h \
    include/inserttabledialog.h \
    include/linkcollectiondialog.h \
    include/linkmanagerwindow.h \
    include/mainwindow.h \
    include/queries.h \
    include/searchnamedialog.h \
    include/settingswindow.h \
    include/summarywindow.h \
    include/yesnodialog.h \

FORMS += \
    ui/addbookdialog.ui \
    ui/addbooksdialog.ui \
    ui/bulkdetailsdialog.ui \
    ui/cleanebooksdialog.ui \
    ui/dataviewerwindow.ui \
    ui/extselectiondialog.ui \
    ui/insertlinkdialog.ui \
    ui/inserttabledialog.ui \
    ui/linkcollectiondialog.ui \
    ui/linkmanagerwindow.ui \
    ui/mainwindow.ui \
    ui/searchnamedialog.ui \
    ui/settingswindow.ui \
    ui/summarywindow.ui \
    ui/yesnodialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/styles/style.qss \
    resources/styles/summarystyle.qss \
    resources/styles/textEditorStyle.qss \


RESOURCES += \
    resources/images.qrc \
    resources/style.qrc


RC_ICONS = ./resources/icons/icon.ico


