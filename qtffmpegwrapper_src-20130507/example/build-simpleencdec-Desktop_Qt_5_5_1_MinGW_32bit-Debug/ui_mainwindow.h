/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad_video;
    QAction *actionSave_synthetic_video;
    QAction *actionSave_synthetic_variable_frame_rate_video;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelVideoFrame;
    QLabel *labelVideoInfo;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonNextFrame;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonSeekFrame;
    QLineEdit *lineEditFrame;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonSeekMillisecond;
    QLineEdit *lineEditMillisecond;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuVideo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(600, 400);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo3.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLoad_video = new QAction(MainWindow);
        actionLoad_video->setObjectName(QStringLiteral("actionLoad_video"));
        actionSave_synthetic_video = new QAction(MainWindow);
        actionSave_synthetic_video->setObjectName(QStringLiteral("actionSave_synthetic_video"));
        actionSave_synthetic_variable_frame_rate_video = new QAction(MainWindow);
        actionSave_synthetic_variable_frame_rate_video->setObjectName(QStringLiteral("actionSave_synthetic_variable_frame_rate_video"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelVideoFrame = new QLabel(centralWidget);
        labelVideoFrame->setObjectName(QStringLiteral("labelVideoFrame"));

        verticalLayout->addWidget(labelVideoFrame);

        labelVideoInfo = new QLabel(centralWidget);
        labelVideoInfo->setObjectName(QStringLiteral("labelVideoInfo"));

        verticalLayout->addWidget(labelVideoInfo);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButtonNextFrame = new QPushButton(centralWidget);
        pushButtonNextFrame->setObjectName(QStringLiteral("pushButtonNextFrame"));

        horizontalLayout_3->addWidget(pushButtonNextFrame);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonSeekFrame = new QPushButton(centralWidget);
        pushButtonSeekFrame->setObjectName(QStringLiteral("pushButtonSeekFrame"));

        horizontalLayout->addWidget(pushButtonSeekFrame);

        lineEditFrame = new QLineEdit(centralWidget);
        lineEditFrame->setObjectName(QStringLiteral("lineEditFrame"));

        horizontalLayout->addWidget(lineEditFrame);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButtonSeekMillisecond = new QPushButton(centralWidget);
        pushButtonSeekMillisecond->setObjectName(QStringLiteral("pushButtonSeekMillisecond"));

        horizontalLayout_2->addWidget(pushButtonSeekMillisecond);

        lineEditMillisecond = new QLineEdit(centralWidget);
        lineEditMillisecond->setObjectName(QStringLiteral("lineEditMillisecond"));

        horizontalLayout_2->addWidget(lineEditMillisecond);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuVideo = new QMenu(menuBar);
        menuVideo->setObjectName(QStringLiteral("menuVideo"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuVideo->menuAction());
        menuFile->addAction(actionQuit);
        menuVideo->addAction(actionLoad_video);
        menuVideo->addAction(actionSave_synthetic_video);
        menuVideo->addAction(actionSave_synthetic_variable_frame_rate_video);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QTFFmpegWrapper demonstration", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionLoad_video->setText(QApplication::translate("MainWindow", "Load video", 0));
        actionLoad_video->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", 0));
        actionSave_synthetic_video->setText(QApplication::translate("MainWindow", "Save synthetic video", 0));
        actionSave_synthetic_video->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_synthetic_variable_frame_rate_video->setText(QApplication::translate("MainWindow", "Save synthetic variable frame rate video", 0));
        labelVideoFrame->setText(QApplication::translate("MainWindow", "Video frames are displayed here...", 0));
        labelVideoInfo->setText(QString());
        pushButtonNextFrame->setText(QApplication::translate("MainWindow", "Next frame", 0));
        pushButtonSeekFrame->setText(QApplication::translate("MainWindow", "Seek to frame", 0));
        pushButtonSeekMillisecond->setText(QApplication::translate("MainWindow", "Seek to millisecond", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuVideo->setTitle(QApplication::translate("MainWindow", "Video", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
