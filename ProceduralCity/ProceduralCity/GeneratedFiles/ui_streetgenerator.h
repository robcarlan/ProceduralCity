/********************************************************************************
** Form generated from reading UI file 'streetgenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STREETGENERATOR_H
#define UI_STREETGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StreetGenerator
{
public:
    QAction *actionBuilding_Generator;
    QAction *actionTexture_Generator;
    QWidget *centralWidget;
    QFrame *frame;
    QGridLayout *gridLayout;
    QScrollBar *horizontalScrollBar;
    QSlider *horizontalSlider;
    QScrollBar *verticalScrollBar;
    QGroupBox *groupBox;
    QOpenGLWidget *openGLWidget;
    QOpenGLWidget *openGLWidget_2;
    QOpenGLWidget *openGLWidget_3;
    QLabel *labelHMap;
    QPushButton *buttonHMap;
    QLabel *labelPMap;
    QPushButton *buttonPMap;
    QLabel *labelGMap;
    QPushButton *buttonGMap;
    QGroupBox *groupBox_2;
    QComboBox *comboBox;
    QLabel *label_4;
    QLabel *label_5;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLabel *label_6;
    QPushButton *pushButton_6;
    QMenuBar *menuBar;
    QMenu *menuWindows;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *StreetGenerator)
    {
        if (StreetGenerator->objectName().isEmpty())
            StreetGenerator->setObjectName(QStringLiteral("StreetGenerator"));
        StreetGenerator->resize(1384, 988);
        actionBuilding_Generator = new QAction(StreetGenerator);
        actionBuilding_Generator->setObjectName(QStringLiteral("actionBuilding_Generator"));
        actionTexture_Generator = new QAction(StreetGenerator);
        actionTexture_Generator->setObjectName(QStringLiteral("actionTexture_Generator"));
        centralWidget = new QWidget(StreetGenerator);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(270, 10, 1091, 641));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalScrollBar = new QScrollBar(frame);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setMaximum(10000);
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalScrollBar, 1, 1, 1, 1);

        horizontalSlider = new QSlider(frame);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 2, 1, 1, 1);

        verticalScrollBar = new QScrollBar(frame);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setMaximum(10000);
        verticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verticalScrollBar, 0, 2, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 650, 941, 271));
        openGLWidget = new QOpenGLWidget(groupBox);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 20, 300, 200));
        openGLWidget_2 = new QOpenGLWidget(groupBox);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));
        openGLWidget_2->setGeometry(QRect(320, 20, 300, 200));
        openGLWidget_3 = new QOpenGLWidget(groupBox);
        openGLWidget_3->setObjectName(QStringLiteral("openGLWidget_3"));
        openGLWidget_3->setGeometry(QRect(630, 20, 300, 200));
        labelHMap = new QLabel(groupBox);
        labelHMap->setObjectName(QStringLiteral("labelHMap"));
        labelHMap->setGeometry(QRect(20, 230, 201, 21));
        buttonHMap = new QPushButton(groupBox);
        buttonHMap->setObjectName(QStringLiteral("buttonHMap"));
        buttonHMap->setGeometry(QRect(230, 230, 75, 23));
        labelPMap = new QLabel(groupBox);
        labelPMap->setObjectName(QStringLiteral("labelPMap"));
        labelPMap->setGeometry(QRect(330, 230, 201, 21));
        buttonPMap = new QPushButton(groupBox);
        buttonPMap->setObjectName(QStringLiteral("buttonPMap"));
        buttonPMap->setGeometry(QRect(540, 230, 75, 23));
        labelGMap = new QLabel(groupBox);
        labelGMap->setObjectName(QStringLiteral("labelGMap"));
        labelGMap->setGeometry(QRect(640, 230, 201, 21));
        buttonGMap = new QPushButton(groupBox);
        buttonGMap->setObjectName(QStringLiteral("buttonGMap"));
        buttonGMap->setGeometry(QRect(850, 230, 75, 23));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 251, 641));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(90, 30, 141, 21));
        comboBox->setLayoutDirection(Qt::LeftToRight);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 30, 71, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 90, 131, 51));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(960, 650, 411, 271));
        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(260, 190, 141, 31));
        pushButton_5 = new QPushButton(groupBox_3);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(260, 230, 141, 31));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 151, 41));
        pushButton_6 = new QPushButton(groupBox_3);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(140, 230, 111, 31));
        StreetGenerator->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StreetGenerator);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1384, 21));
        menuWindows = new QMenu(menuBar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        StreetGenerator->setMenuBar(menuBar);
        mainToolBar = new QToolBar(StreetGenerator);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        StreetGenerator->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(StreetGenerator);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        StreetGenerator->setStatusBar(statusBar);

        menuBar->addAction(menuWindows->menuAction());
        menuWindows->addAction(actionBuilding_Generator);
        menuWindows->addAction(actionTexture_Generator);

        retranslateUi(StreetGenerator);

        QMetaObject::connectSlotsByName(StreetGenerator);
    } // setupUi

    void retranslateUi(QMainWindow *StreetGenerator)
    {
        StreetGenerator->setWindowTitle(QApplication::translate("StreetGenerator", "StreetGenerator", 0));
        actionBuilding_Generator->setText(QApplication::translate("StreetGenerator", "Building Generator", 0));
        actionTexture_Generator->setText(QApplication::translate("StreetGenerator", "Texture Generator", 0));
#ifndef QT_NO_TOOLTIP
        frame->setToolTip(QApplication::translate("StreetGenerator", "<html><head/><body><p><br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("StreetGenerator", "Inputs", 0));
        labelHMap->setText(QApplication::translate("StreetGenerator", "Height Map: ......", 0));
        buttonHMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        labelPMap->setText(QApplication::translate("StreetGenerator", "Population Density...", 0));
        buttonPMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        labelGMap->setText(QApplication::translate("StreetGenerator", "Geography: ...", 0));
        buttonGMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        groupBox_2->setTitle(QApplication::translate("StreetGenerator", "Inputs", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("StreetGenerator", "Grid", 0)
        );
        label_4->setText(QApplication::translate("StreetGenerator", "Street Style", 0));
        label_5->setText(QApplication::translate("StreetGenerator", "Various Inputs....", 0));
        groupBox_3->setTitle(QApplication::translate("StreetGenerator", "Controls", 0));
        pushButton_4->setText(QApplication::translate("StreetGenerator", "Generate", 0));
        pushButton_5->setText(QApplication::translate("StreetGenerator", "Save...", 0));
        label_6->setText(QApplication::translate("StreetGenerator", "Various Output Parameters", 0));
        pushButton_6->setText(QApplication::translate("StreetGenerator", "Save Image", 0));
        menuWindows->setTitle(QApplication::translate("StreetGenerator", "Windows", 0));
    } // retranslateUi

};

namespace Ui {
    class StreetGenerator: public Ui_StreetGenerator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STREETGENERATOR_H
