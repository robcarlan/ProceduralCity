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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glimagepreview.h"
#include "qstreetrenderwidget.h"

QT_BEGIN_NAMESPACE

class Ui_StreetGenerator
{
public:
    QAction *actionBuilding_Generator;
    QAction *actionTexture_Generator;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    GLImagePreview *hMapRender;
    GLImagePreview *popMapRender;
    GLImagePreview *geogMapRender;
    QLabel *labelHMap;
    QPushButton *buttonHMap;
    QLabel *labelPMap;
    QPushButton *buttonPMap;
    QLabel *labelGMap;
    QPushButton *buttonGMap;
    QPushButton *cmdClearHeight;
    QPushButton *cmdClearPop;
    QPushButton *cmdClearGeog;
    QPushButton *buttonSMap;
    GLImagePreview *patternMapRender;
    QPushButton *cmdClearStreetMap;
    QLabel *labelSMap;
    QGroupBox *groupBox_2;
    QPushButton *cmdLoadImageDirectory;
    QGroupBox *groupBox_5;
    QRadioButton *radUseMajorityStreet;
    QRadioButton *radUseWeightedStreet;
    QGroupBox *groupBox_7;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QSpinBox *spinMaxBridgeLength;
    QLabel *label_17;
    QLabel *label_16;
    QLabel *label_15;
    QSlider *slideManhattanBlockHeight;
    QSlider *slideManhattanBlocKWidth;
    QLabel *label_14;
    QLabel *label_13;
    QLabel *label_19;
    QSpinBox *spinMaxRoadAngleChange;
    QLabel *label_22;
    QLabel *label_23;
    QSpinBox *spinStreetLength;
    QSpinBox *spinHighwayLength;
    QDoubleSpinBox *spinRoadBranchProb;
    QGroupBox *groupBox_8;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QSpinBox *spinRoadExtendLength;
    QLabel *label_10;
    QLabel *label_11;
    QSpinBox *spinMinRoadLength;
    QLabel *label_12;
    QSpinBox *spinRoadSnapRadius;
    QLabel *label_18;
    QDoubleSpinBox *spinRoadPruneFactor;
    QGroupBox *groupBox_6;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_24;
    QSlider *slideRoadSearchAngle;
    QLabel *label_7;
    QSlider *sliderRoadSampleInterval;
    QSlider *sliderSearchAngleIncrease;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_8;
    QLabel *label_20;
    QSlider *sliderPopSearchRadius;
    QLabel *label_21;
    QLabel *label_25;
    QDoubleSpinBox *highwayGrowthFactor;
    QDoubleSpinBox *streetGrowthFactor;
    QGroupBox *groupBox_3;
    QPushButton *cmdSaveOutput;
    QPushButton *cmdSaveOutputImage;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *radShowPopulation;
    QRadioButton *radShowGeography;
    QRadioButton *radShowHeightmap;
    QRadioButton *radShowPatternmap;
    QRadioButton *radShowNone;
    QCheckBox *chkShowVerts;
    QCheckBox *chkShowRegions;
    QCheckBox *chkShowLots;
    QWidget *layoutWidget2;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *intSeedValue;
    QCheckBox *chkTimeSeed;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QPushButton *cmdReset;
    QPushButton *cmdStep;
    QPushButton *cmdGenerateStreets;
    QPushButton *cmdFilterRoads;
    QPushButton *cmdGenerateRegions;
    QPushButton *cmdCreateBuildingLots;
    QStreetRenderWidget *streetRender;
    QMenuBar *menuBar;
    QMenu *menuWindows;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QButtonGroup *buttonGroup;

    void setupUi(QMainWindow *StreetGenerator)
    {
        if (StreetGenerator->objectName().isEmpty())
            StreetGenerator->setObjectName(QStringLiteral("StreetGenerator"));
        StreetGenerator->resize(1821, 1034);
        actionBuilding_Generator = new QAction(StreetGenerator);
        actionBuilding_Generator->setObjectName(QStringLiteral("actionBuilding_Generator"));
        actionTexture_Generator = new QAction(StreetGenerator);
        actionTexture_Generator->setObjectName(QStringLiteral("actionTexture_Generator"));
        centralWidget = new QWidget(StreetGenerator);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 710, 1371, 271));
        hMapRender = new GLImagePreview(groupBox);
        hMapRender->setObjectName(QStringLiteral("hMapRender"));
        hMapRender->setGeometry(QRect(20, 20, 300, 200));
        popMapRender = new GLImagePreview(groupBox);
        popMapRender->setObjectName(QStringLiteral("popMapRender"));
        popMapRender->setGeometry(QRect(340, 20, 300, 200));
        geogMapRender = new GLImagePreview(groupBox);
        geogMapRender->setObjectName(QStringLiteral("geogMapRender"));
        geogMapRender->setGeometry(QRect(660, 20, 300, 200));
        labelHMap = new QLabel(groupBox);
        labelHMap->setObjectName(QStringLiteral("labelHMap"));
        labelHMap->setGeometry(QRect(30, 230, 201, 21));
        buttonHMap = new QPushButton(groupBox);
        buttonHMap->setObjectName(QStringLiteral("buttonHMap"));
        buttonHMap->setGeometry(QRect(250, 230, 61, 23));
        labelPMap = new QLabel(groupBox);
        labelPMap->setObjectName(QStringLiteral("labelPMap"));
        labelPMap->setGeometry(QRect(350, 230, 201, 21));
        buttonPMap = new QPushButton(groupBox);
        buttonPMap->setObjectName(QStringLiteral("buttonPMap"));
        buttonPMap->setGeometry(QRect(570, 230, 61, 23));
        labelGMap = new QLabel(groupBox);
        labelGMap->setObjectName(QStringLiteral("labelGMap"));
        labelGMap->setGeometry(QRect(670, 230, 201, 21));
        buttonGMap = new QPushButton(groupBox);
        buttonGMap->setObjectName(QStringLiteral("buttonGMap"));
        buttonGMap->setGeometry(QRect(890, 230, 61, 23));
        cmdClearHeight = new QPushButton(groupBox);
        cmdClearHeight->setObjectName(QStringLiteral("cmdClearHeight"));
        cmdClearHeight->setGeometry(QRect(190, 230, 61, 23));
        cmdClearPop = new QPushButton(groupBox);
        cmdClearPop->setObjectName(QStringLiteral("cmdClearPop"));
        cmdClearPop->setGeometry(QRect(510, 230, 61, 23));
        cmdClearGeog = new QPushButton(groupBox);
        cmdClearGeog->setObjectName(QStringLiteral("cmdClearGeog"));
        cmdClearGeog->setGeometry(QRect(830, 230, 61, 23));
        buttonSMap = new QPushButton(groupBox);
        buttonSMap->setObjectName(QStringLiteral("buttonSMap"));
        buttonSMap->setGeometry(QRect(1210, 230, 61, 23));
        patternMapRender = new GLImagePreview(groupBox);
        patternMapRender->setObjectName(QStringLiteral("patternMapRender"));
        patternMapRender->setGeometry(QRect(976, 20, 300, 200));
        cmdClearStreetMap = new QPushButton(groupBox);
        cmdClearStreetMap->setObjectName(QStringLiteral("cmdClearStreetMap"));
        cmdClearStreetMap->setGeometry(QRect(1146, 230, 61, 23));
        labelSMap = new QLabel(groupBox);
        labelSMap->setObjectName(QStringLiteral("labelSMap"));
        labelSMap->setGeometry(QRect(986, 230, 201, 21));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 331, 701));
        cmdLoadImageDirectory = new QPushButton(groupBox_2);
        cmdLoadImageDirectory->setObjectName(QStringLiteral("cmdLoadImageDirectory"));
        cmdLoadImageDirectory->setGeometry(QRect(180, 660, 141, 31));
        groupBox_5 = new QGroupBox(groupBox_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 610, 151, 81));
        radUseMajorityStreet = new QRadioButton(groupBox_5);
        radUseMajorityStreet->setObjectName(QStringLiteral("radUseMajorityStreet"));
        radUseMajorityStreet->setGeometry(QRect(10, 30, 209, 17));
        radUseWeightedStreet = new QRadioButton(groupBox_5);
        radUseWeightedStreet->setObjectName(QStringLiteral("radUseWeightedStreet"));
        radUseWeightedStreet->setGeometry(QRect(10, 50, 209, 17));
        radUseWeightedStreet->setChecked(true);
        groupBox_7 = new QGroupBox(groupBox_2);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 230, 301, 221));
        gridLayoutWidget_2 = new QWidget(groupBox_7);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(9, 19, 281, 195));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        spinMaxBridgeLength = new QSpinBox(gridLayoutWidget_2);
        spinMaxBridgeLength->setObjectName(QStringLiteral("spinMaxBridgeLength"));
        spinMaxBridgeLength->setMaximum(1000);
        spinMaxBridgeLength->setValue(200);

        gridLayout_2->addWidget(spinMaxBridgeLength, 0, 1, 1, 2);

        label_17 = new QLabel(gridLayoutWidget_2);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_2->addWidget(label_17, 3, 0, 1, 1);

        label_16 = new QLabel(gridLayoutWidget_2);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_2->addWidget(label_16, 1, 2, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 2, 2, 1, 1);

        slideManhattanBlockHeight = new QSlider(gridLayoutWidget_2);
        slideManhattanBlockHeight->setObjectName(QStringLiteral("slideManhattanBlockHeight"));
        slideManhattanBlockHeight->setMinimum(10);
        slideManhattanBlockHeight->setMaximum(200);
        slideManhattanBlockHeight->setValue(50);
        slideManhattanBlockHeight->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(slideManhattanBlockHeight, 2, 1, 1, 1);

        slideManhattanBlocKWidth = new QSlider(gridLayoutWidget_2);
        slideManhattanBlocKWidth->setObjectName(QStringLiteral("slideManhattanBlocKWidth"));
        slideManhattanBlocKWidth->setMinimum(10);
        slideManhattanBlocKWidth->setMaximum(200);
        slideManhattanBlocKWidth->setValue(30);
        slideManhattanBlocKWidth->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(slideManhattanBlocKWidth, 1, 1, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 2, 0, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 1, 0, 1, 1);

        label_19 = new QLabel(gridLayoutWidget_2);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_2->addWidget(label_19, 4, 0, 1, 1);

        spinMaxRoadAngleChange = new QSpinBox(gridLayoutWidget_2);
        spinMaxRoadAngleChange->setObjectName(QStringLiteral("spinMaxRoadAngleChange"));
        spinMaxRoadAngleChange->setMaximum(360);
        spinMaxRoadAngleChange->setValue(30);

        gridLayout_2->addWidget(spinMaxRoadAngleChange, 4, 1, 1, 1);

        label_22 = new QLabel(gridLayoutWidget_2);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_2->addWidget(label_22, 5, 0, 1, 1);

        label_23 = new QLabel(gridLayoutWidget_2);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_2->addWidget(label_23, 6, 0, 1, 1);

        spinStreetLength = new QSpinBox(gridLayoutWidget_2);
        spinStreetLength->setObjectName(QStringLiteral("spinStreetLength"));
        spinStreetLength->setMinimum(5);
        spinStreetLength->setMaximum(200);
        spinStreetLength->setSingleStep(5);
        spinStreetLength->setValue(25);

        gridLayout_2->addWidget(spinStreetLength, 5, 1, 1, 1);

        spinHighwayLength = new QSpinBox(gridLayoutWidget_2);
        spinHighwayLength->setObjectName(QStringLiteral("spinHighwayLength"));
        spinHighwayLength->setMinimum(5);
        spinHighwayLength->setMaximum(500);
        spinHighwayLength->setSingleStep(5);
        spinHighwayLength->setValue(150);

        gridLayout_2->addWidget(spinHighwayLength, 6, 1, 1, 1);

        spinRoadBranchProb = new QDoubleSpinBox(gridLayoutWidget_2);
        spinRoadBranchProb->setObjectName(QStringLiteral("spinRoadBranchProb"));
        spinRoadBranchProb->setMaximum(1);
        spinRoadBranchProb->setSingleStep(0.05);
        spinRoadBranchProb->setValue(0.75);

        gridLayout_2->addWidget(spinRoadBranchProb, 3, 1, 1, 1);

        groupBox_8 = new QGroupBox(groupBox_2);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 460, 301, 141));
        gridLayoutWidget_3 = new QWidget(groupBox_8);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(10, 20, 281, 100));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        spinRoadExtendLength = new QSpinBox(gridLayoutWidget_3);
        spinRoadExtendLength->setObjectName(QStringLiteral("spinRoadExtendLength"));
        spinRoadExtendLength->setMaximum(500);
        spinRoadExtendLength->setValue(20);

        gridLayout_3->addWidget(spinRoadExtendLength, 0, 1, 1, 1);

        label_10 = new QLabel(gridLayoutWidget_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        spinMinRoadLength = new QSpinBox(gridLayoutWidget_3);
        spinMinRoadLength->setObjectName(QStringLiteral("spinMinRoadLength"));
        spinMinRoadLength->setMaximum(200);
        spinMinRoadLength->setValue(10);
        spinMinRoadLength->setDisplayIntegerBase(10);

        gridLayout_3->addWidget(spinMinRoadLength, 1, 1, 1, 1);

        label_12 = new QLabel(gridLayoutWidget_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_3->addWidget(label_12, 2, 0, 1, 1);

        spinRoadSnapRadius = new QSpinBox(gridLayoutWidget_3);
        spinRoadSnapRadius->setObjectName(QStringLiteral("spinRoadSnapRadius"));
        spinRoadSnapRadius->setMaximum(500);
        spinRoadSnapRadius->setValue(25);

        gridLayout_3->addWidget(spinRoadSnapRadius, 2, 1, 1, 1);

        label_18 = new QLabel(gridLayoutWidget_3);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_3->addWidget(label_18, 3, 0, 1, 1);

        spinRoadPruneFactor = new QDoubleSpinBox(gridLayoutWidget_3);
        spinRoadPruneFactor->setObjectName(QStringLiteral("spinRoadPruneFactor"));
        spinRoadPruneFactor->setMaximum(1);
        spinRoadPruneFactor->setSingleStep(0.01);
        spinRoadPruneFactor->setValue(0.1);

        gridLayout_3->addWidget(spinRoadPruneFactor, 3, 1, 1, 1);

        groupBox_6 = new QGroupBox(groupBox_2);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 20, 301, 201));
        gridLayoutWidget = new QWidget(groupBox_6);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 20, 281, 171));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_24 = new QLabel(gridLayoutWidget);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout->addWidget(label_24, 4, 0, 1, 1);

        slideRoadSearchAngle = new QSlider(gridLayoutWidget);
        slideRoadSearchAngle->setObjectName(QStringLiteral("slideRoadSearchAngle"));
        slideRoadSearchAngle->setMaximum(360);
        slideRoadSearchAngle->setValue(70);
        slideRoadSearchAngle->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slideRoadSearchAngle, 0, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        sliderRoadSampleInterval = new QSlider(gridLayoutWidget);
        sliderRoadSampleInterval->setObjectName(QStringLiteral("sliderRoadSampleInterval"));
        sliderRoadSampleInterval->setMinimum(1);
        sliderRoadSampleInterval->setMaximum(10);
        sliderRoadSampleInterval->setValue(2);
        sliderRoadSampleInterval->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderRoadSampleInterval, 2, 1, 1, 1);

        sliderSearchAngleIncrease = new QSlider(gridLayoutWidget);
        sliderSearchAngleIncrease->setObjectName(QStringLiteral("sliderSearchAngleIncrease"));
        sliderSearchAngleIncrease->setMinimum(1);
        sliderSearchAngleIncrease->setMaximum(10);
        sliderSearchAngleIncrease->setOrientation(Qt::Horizontal);
        sliderSearchAngleIncrease->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(sliderSearchAngleIncrease, 1, 1, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 2, 2, 1, 1);

        label_20 = new QLabel(gridLayoutWidget);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout->addWidget(label_20, 3, 0, 1, 1);

        sliderPopSearchRadius = new QSlider(gridLayoutWidget);
        sliderPopSearchRadius->setObjectName(QStringLiteral("sliderPopSearchRadius"));
        sliderPopSearchRadius->setMinimum(25);
        sliderPopSearchRadius->setMaximum(1000);
        sliderPopSearchRadius->setSingleStep(25);
        sliderPopSearchRadius->setValue(200);
        sliderPopSearchRadius->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sliderPopSearchRadius, 3, 1, 1, 1);

        label_21 = new QLabel(gridLayoutWidget);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout->addWidget(label_21, 3, 2, 1, 1);

        label_25 = new QLabel(gridLayoutWidget);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout->addWidget(label_25, 5, 0, 1, 1);

        highwayGrowthFactor = new QDoubleSpinBox(gridLayoutWidget);
        highwayGrowthFactor->setObjectName(QStringLiteral("highwayGrowthFactor"));
        highwayGrowthFactor->setMaximum(1);
        highwayGrowthFactor->setSingleStep(0.05);
        highwayGrowthFactor->setValue(0.75);

        gridLayout->addWidget(highwayGrowthFactor, 4, 1, 1, 1);

        streetGrowthFactor = new QDoubleSpinBox(gridLayoutWidget);
        streetGrowthFactor->setObjectName(QStringLiteral("streetGrowthFactor"));
        streetGrowthFactor->setMaximum(1);
        streetGrowthFactor->setSingleStep(0.05);
        streetGrowthFactor->setValue(0.5);

        gridLayout->addWidget(streetGrowthFactor, 5, 1, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(1390, 710, 411, 271));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        cmdSaveOutput = new QPushButton(groupBox_3);
        cmdSaveOutput->setObjectName(QStringLiteral("cmdSaveOutput"));
        cmdSaveOutput->setGeometry(QRect(130, 230, 121, 31));
        cmdSaveOutputImage = new QPushButton(groupBox_3);
        cmdSaveOutputImage->setObjectName(QStringLiteral("cmdSaveOutputImage"));
        cmdSaveOutputImage->setGeometry(QRect(11, 230, 111, 31));
        layoutWidget = new QWidget(groupBox_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 161, 201));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox_4 = new QGroupBox(layoutWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        layoutWidget1 = new QWidget(groupBox_4);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 14, 141, 180));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        radShowPopulation = new QRadioButton(layoutWidget1);
        buttonGroup = new QButtonGroup(StreetGenerator);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(radShowPopulation);
        radShowPopulation->setObjectName(QStringLiteral("radShowPopulation"));

        verticalLayout_2->addWidget(radShowPopulation);

        radShowGeography = new QRadioButton(layoutWidget1);
        buttonGroup->addButton(radShowGeography);
        radShowGeography->setObjectName(QStringLiteral("radShowGeography"));

        verticalLayout_2->addWidget(radShowGeography);

        radShowHeightmap = new QRadioButton(layoutWidget1);
        buttonGroup->addButton(radShowHeightmap);
        radShowHeightmap->setObjectName(QStringLiteral("radShowHeightmap"));

        verticalLayout_2->addWidget(radShowHeightmap);

        radShowPatternmap = new QRadioButton(layoutWidget1);
        buttonGroup->addButton(radShowPatternmap);
        radShowPatternmap->setObjectName(QStringLiteral("radShowPatternmap"));

        verticalLayout_2->addWidget(radShowPatternmap);

        radShowNone = new QRadioButton(layoutWidget1);
        buttonGroup->addButton(radShowNone);
        radShowNone->setObjectName(QStringLiteral("radShowNone"));

        verticalLayout_2->addWidget(radShowNone);

        chkShowVerts = new QCheckBox(layoutWidget1);
        chkShowVerts->setObjectName(QStringLiteral("chkShowVerts"));
        chkShowVerts->setChecked(true);

        verticalLayout_2->addWidget(chkShowVerts);

        chkShowRegions = new QCheckBox(layoutWidget1);
        chkShowRegions->setObjectName(QStringLiteral("chkShowRegions"));
        chkShowRegions->setEnabled(false);
        chkShowRegions->setCheckable(true);
        chkShowRegions->setChecked(false);

        verticalLayout_2->addWidget(chkShowRegions);

        chkShowLots = new QCheckBox(layoutWidget1);
        chkShowLots->setObjectName(QStringLiteral("chkShowLots"));
        chkShowLots->setEnabled(false);
        chkShowLots->setCheckable(true);
        chkShowLots->setChecked(false);

        verticalLayout_2->addWidget(chkShowLots);


        verticalLayout->addWidget(groupBox_4);

        layoutWidget2 = new QWidget(groupBox_3);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(236, 20, 171, 51));
        formLayout = new QFormLayout(layoutWidget2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget2);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        intSeedValue = new QSpinBox(layoutWidget2);
        intSeedValue->setObjectName(QStringLiteral("intSeedValue"));
        intSeedValue->setMinimum(-1000000000);
        intSeedValue->setMaximum(1000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, intSeedValue);

        chkTimeSeed = new QCheckBox(layoutWidget2);
        chkTimeSeed->setObjectName(QStringLiteral("chkTimeSeed"));

        formLayout->setWidget(1, QFormLayout::FieldRole, chkTimeSeed);

        widget = new QWidget(groupBox_3);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(260, 80, 141, 181));
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        cmdReset = new QPushButton(widget);
        cmdReset->setObjectName(QStringLiteral("cmdReset"));

        gridLayout_4->addWidget(cmdReset, 0, 0, 1, 1);

        cmdStep = new QPushButton(widget);
        cmdStep->setObjectName(QStringLiteral("cmdStep"));

        gridLayout_4->addWidget(cmdStep, 1, 0, 1, 1);

        cmdGenerateStreets = new QPushButton(widget);
        cmdGenerateStreets->setObjectName(QStringLiteral("cmdGenerateStreets"));

        gridLayout_4->addWidget(cmdGenerateStreets, 2, 0, 1, 1);

        cmdFilterRoads = new QPushButton(widget);
        cmdFilterRoads->setObjectName(QStringLiteral("cmdFilterRoads"));
        cmdFilterRoads->setEnabled(false);
        cmdFilterRoads->setCheckable(false);

        gridLayout_4->addWidget(cmdFilterRoads, 3, 0, 1, 1);

        cmdGenerateRegions = new QPushButton(widget);
        cmdGenerateRegions->setObjectName(QStringLiteral("cmdGenerateRegions"));
        cmdGenerateRegions->setEnabled(false);
        cmdGenerateRegions->setDefault(false);
        cmdGenerateRegions->setFlat(false);

        gridLayout_4->addWidget(cmdGenerateRegions, 4, 0, 1, 1);

        cmdCreateBuildingLots = new QPushButton(widget);
        cmdCreateBuildingLots->setObjectName(QStringLiteral("cmdCreateBuildingLots"));
        cmdCreateBuildingLots->setEnabled(false);

        gridLayout_4->addWidget(cmdCreateBuildingLots, 5, 0, 1, 1);

        streetRender = new QStreetRenderWidget(centralWidget);
        streetRender->setObjectName(QStringLiteral("streetRender"));
        streetRender->setGeometry(QRect(350, 20, 1451, 681));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(streetRender->sizePolicy().hasHeightForWidth());
        streetRender->setSizePolicy(sizePolicy1);
        streetRender->setFrameShape(QFrame::Box);
        streetRender->setFrameShadow(QFrame::Sunken);
        StreetGenerator->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StreetGenerator);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1821, 21));
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
        QObject::connect(buttonPMap, SIGNAL(clicked()), StreetGenerator, SLOT(on_buttonPMap_clicked()));
        QObject::connect(buttonGMap, SIGNAL(clicked()), StreetGenerator, SLOT(on_buttonGMap_clicked()));
        QObject::connect(cmdGenerateStreets, SIGNAL(clicked()), StreetGenerator, SLOT(onClickGenerate()));
        QObject::connect(radShowPopulation, SIGNAL(toggled(bool)), StreetGenerator, SLOT(on_radPopClicked()));
        QObject::connect(radShowGeography, SIGNAL(toggled(bool)), StreetGenerator, SLOT(on_radGeogClicked()));
        QObject::connect(radShowHeightmap, SIGNAL(toggled(bool)), StreetGenerator, SLOT(on_radHeightClicked()));
        QObject::connect(cmdReset, SIGNAL(clicked()), StreetGenerator, SLOT(onClickReset()));
        QObject::connect(cmdStep, SIGNAL(clicked()), StreetGenerator, SLOT(onClickStep()));
        QObject::connect(cmdSaveOutput, SIGNAL(clicked()), StreetGenerator, SLOT(onClickSave()));
        QObject::connect(cmdSaveOutputImage, SIGNAL(clicked()), StreetGenerator, SLOT(onClickSaveImage()));
        QObject::connect(radShowNone, SIGNAL(clicked()), StreetGenerator, SLOT(on_radNoneClicked()));
        QObject::connect(cmdClearHeight, SIGNAL(clicked()), StreetGenerator, SLOT(clearHeightmap()));
        QObject::connect(cmdClearPop, SIGNAL(clicked()), StreetGenerator, SLOT(clearPopmap()));
        QObject::connect(cmdClearGeog, SIGNAL(clicked()), StreetGenerator, SLOT(clearGeogmap()));
        QObject::connect(cmdLoadImageDirectory, SIGNAL(clicked()), StreetGenerator, SLOT(cmdLoadDirectory()));
        QObject::connect(radShowPatternmap, SIGNAL(clicked()), StreetGenerator, SLOT(on_radPatternClicked()));
        QObject::connect(sliderSearchAngleIncrease, SIGNAL(sliderMoved(int)), label_5, SLOT(setNum(int)));
        QObject::connect(slideRoadSearchAngle, SIGNAL(valueChanged(int)), label_3, SLOT(setNum(int)));
        QObject::connect(sliderRoadSampleInterval, SIGNAL(valueChanged(int)), label_8, SLOT(setNum(int)));
        QObject::connect(slideManhattanBlocKWidth, SIGNAL(valueChanged(int)), label_16, SLOT(setNum(int)));
        QObject::connect(slideManhattanBlockHeight, SIGNAL(valueChanged(int)), label_15, SLOT(setNum(int)));
        QObject::connect(sliderPopSearchRadius, SIGNAL(valueChanged(int)), label_21, SLOT(setNum(int)));
        QObject::connect(cmdCreateBuildingLots, SIGNAL(clicked()), StreetGenerator, SLOT(onClickCreateLots()));
        QObject::connect(cmdGenerateRegions, SIGNAL(clicked()), StreetGenerator, SLOT(onClickCreateRegions()));
        QObject::connect(chkShowRegions, SIGNAL(toggled(bool)), StreetGenerator, SLOT(onClickRenderRegions(bool)));
        QObject::connect(chkShowLots, SIGNAL(toggled(bool)), StreetGenerator, SLOT(onClickRenderLots(bool)));
        QObject::connect(cmdClearStreetMap, SIGNAL(clicked()), StreetGenerator, SLOT(clearStreetmap()));
        QObject::connect(buttonSMap, SIGNAL(clicked()), StreetGenerator, SLOT(on_button_SMap_Clicked()));
        QObject::connect(chkShowVerts, SIGNAL(toggled(bool)), StreetGenerator, SLOT(onClickRenderVerts(bool)));
        QObject::connect(cmdFilterRoads, SIGNAL(clicked()), StreetGenerator, SLOT(onClickFilterRoads()));

        QMetaObject::connectSlotsByName(StreetGenerator);
    } // setupUi

    void retranslateUi(QMainWindow *StreetGenerator)
    {
        StreetGenerator->setWindowTitle(QApplication::translate("StreetGenerator", "StreetGenerator", 0));
        actionBuilding_Generator->setText(QApplication::translate("StreetGenerator", "Building Generator", 0));
        actionTexture_Generator->setText(QApplication::translate("StreetGenerator", "Texture Generator", 0));
        groupBox->setTitle(QApplication::translate("StreetGenerator", "Inputs", 0));
        labelHMap->setText(QApplication::translate("StreetGenerator", "Height Map: ......", 0));
        buttonHMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        labelPMap->setText(QApplication::translate("StreetGenerator", "Population Density...", 0));
        buttonPMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        labelGMap->setText(QApplication::translate("StreetGenerator", "Geography: ...", 0));
        buttonGMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        cmdClearHeight->setText(QApplication::translate("StreetGenerator", "Clear", 0));
        cmdClearPop->setText(QApplication::translate("StreetGenerator", "Clear", 0));
        cmdClearGeog->setText(QApplication::translate("StreetGenerator", "Clear", 0));
        buttonSMap->setText(QApplication::translate("StreetGenerator", "Browse", 0));
        cmdClearStreetMap->setText(QApplication::translate("StreetGenerator", "Clear", 0));
        labelSMap->setText(QApplication::translate("StreetGenerator", "Street Pattern: ...", 0));
        groupBox_2->setTitle(QApplication::translate("StreetGenerator", "Inputs", 0));
        cmdLoadImageDirectory->setText(QApplication::translate("StreetGenerator", "Load from Directory", 0));
        groupBox_5->setTitle(QApplication::translate("StreetGenerator", "Street Pattern Evaluation", 0));
        radUseMajorityStreet->setText(QApplication::translate("StreetGenerator", "Majority ", 0));
        radUseWeightedStreet->setText(QApplication::translate("StreetGenerator", "Weighted Values", 0));
        groupBox_7->setTitle(QApplication::translate("StreetGenerator", "Road Properties", 0));
        label_9->setText(QApplication::translate("StreetGenerator", "Max Bridge Length", 0));
        label_17->setText(QApplication::translate("StreetGenerator", "Road Branch Probability", 0));
        label_16->setText(QApplication::translate("StreetGenerator", "30", 0));
        label_15->setText(QApplication::translate("StreetGenerator", "50", 0));
        label_14->setText(QApplication::translate("StreetGenerator", "Manhattan Block Height", 0));
        label_13->setText(QApplication::translate("StreetGenerator", "Manhattan Block Width", 0));
        label_19->setText(QApplication::translate("StreetGenerator", "Road Max Angle Change", 0));
        label_22->setText(QApplication::translate("StreetGenerator", "Street Length", 0));
        label_23->setText(QApplication::translate("StreetGenerator", "Highway Length", 0));
        groupBox_8->setTitle(QApplication::translate("StreetGenerator", "Intersection Fitting", 0));
        label_10->setText(QApplication::translate("StreetGenerator", "Road Extend Length", 0));
        label_11->setText(QApplication::translate("StreetGenerator", "Min Road Length", 0));
        label_12->setText(QApplication::translate("StreetGenerator", "Road Snap Radius", 0));
        label_18->setText(QApplication::translate("StreetGenerator", "Road Prune Factor", 0));
        groupBox_6->setTitle(QApplication::translate("StreetGenerator", "Search Parameters", 0));
        label_24->setText(QApplication::translate("StreetGenerator", "HIghway Growth Score", 0));
        label_7->setText(QApplication::translate("StreetGenerator", "Road Sample Interval", 0));
        label_5->setText(QApplication::translate("StreetGenerator", "1", 0));
        label_4->setText(QApplication::translate("StreetGenerator", "Angle Search Increment", 0));
        label_3->setText(QApplication::translate("StreetGenerator", "70", 0));
        label_2->setText(QApplication::translate("StreetGenerator", "Road Search Angle", 0));
        label_8->setText(QApplication::translate("StreetGenerator", "2", 0));
        label_20->setText(QApplication::translate("StreetGenerator", "Pop Search Radius", 0));
        label_21->setText(QApplication::translate("StreetGenerator", "200", 0));
        label_25->setText(QApplication::translate("StreetGenerator", "Street Growth Score", 0));
        groupBox_3->setTitle(QApplication::translate("StreetGenerator", "Controls", 0));
        cmdSaveOutput->setText(QApplication::translate("StreetGenerator", "Save...", 0));
        cmdSaveOutputImage->setText(QApplication::translate("StreetGenerator", "Save Image", 0));
        groupBox_4->setTitle(QApplication::translate("StreetGenerator", "Render Options", 0));
        radShowPopulation->setText(QApplication::translate("StreetGenerator", "Show Population", 0));
        radShowGeography->setText(QApplication::translate("StreetGenerator", "Show Geography", 0));
        radShowHeightmap->setText(QApplication::translate("StreetGenerator", "Show Heightmap", 0));
        radShowPatternmap->setText(QApplication::translate("StreetGenerator", "Show Street Patterns", 0));
        radShowNone->setText(QApplication::translate("StreetGenerator", "Show None", 0));
        chkShowVerts->setText(QApplication::translate("StreetGenerator", "Show Vertices", 0));
        chkShowRegions->setText(QApplication::translate("StreetGenerator", "Render Building Regions", 0));
        chkShowLots->setText(QApplication::translate("StreetGenerator", "Render Building Lots", 0));
        label->setText(QApplication::translate("StreetGenerator", "Seed:", 0));
        chkTimeSeed->setText(QApplication::translate("StreetGenerator", "Use system time", 0));
        cmdReset->setText(QApplication::translate("StreetGenerator", "Initialise / Reset", 0));
        cmdStep->setText(QApplication::translate("StreetGenerator", "Step...", 0));
        cmdGenerateStreets->setText(QApplication::translate("StreetGenerator", "Generate Street Network", 0));
        cmdFilterRoads->setText(QApplication::translate("StreetGenerator", "Filter Roads", 0));
        cmdGenerateRegions->setText(QApplication::translate("StreetGenerator", "Create Regions", 0));
        cmdCreateBuildingLots->setText(QApplication::translate("StreetGenerator", "Create Building Lots", 0));
        menuWindows->setTitle(QApplication::translate("StreetGenerator", "Windows", 0));
    } // retranslateUi

};

namespace Ui {
    class StreetGenerator: public Ui_StreetGenerator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STREETGENERATOR_H
