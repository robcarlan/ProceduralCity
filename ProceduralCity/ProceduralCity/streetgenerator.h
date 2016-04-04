#ifndef STREETGENERATOR_H
#define STREETGENERATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_streetgenerator.h"

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include "StreetGen.h"
#include "City.h"
#include "SimpleStreetGeometryCreator.h"
#include "CityView2D.h"
#include "cityview3d.h"

class StreetGenerator : public QMainWindow
{
	Q_OBJECT

public:
	StreetGenerator(QWidget *parent = 0);
	~StreetGenerator();

private slots:
	
	void onClickGenerate();
	void onClickReset();
	void onClickStep();
	void onClickSave();
	void onClickSaveImage();
	void onClickFilterRoads();
	void onClickCreateLots();
	void onClickCreateRegions();
	void onClickShowRendered();
	void onClickRenderRegions(bool render);
	void onClickRenderLots(bool render);
	void onClickRenderVerts(bool render);

	void on_comboBox_activated(const QString &arg1);

	void on_pushButton_clicked();
	void on_buttonHMap_clicked();
	void on_buttonPMap_clicked();
	void on_buttonGMap_clicked();
	void on_buttonSMap_clicked();
	void cmdLoadDirectory();
	void clearStreetmap();
	void clearHeightmap();
	void clearGeogmap();
	void clearPopmap();

	void on_radPopClicked();
	void on_radGeogClicked();
	void on_radHeightClicked();
	void on_radPatternClicked();
	void on_radNoneClicked();

private:
	Ui::StreetGenerator ui;
	CityView2D view;
	StreetGen generator;
	City cityView;
	QFileDialog fd;
	QFileDialog dir;

	static const int DEFAULT_SIZE_X;
	static const int DEFAULT_SIZE_Y;

	QPoint size;

	void initialiseSystem();
	QString getFileChoice();
	void setParameters();

	bool streetsGenerated, streetsFiltered, regionsGenerated, lotsGenerated;

};

#endif // STREETGENERATOR_H
