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

	void on_comboBox_activated(const QString &arg1);

	void on_pushButton_clicked();
	void on_buttonHMap_clicked();
	void on_buttonPMap_clicked();
	void on_buttonGMap_clicked();
	void clearHeightmap();
	void clearGeogmap();
	void clearPopmap();

	void on_radPopClicked();
	void on_radGeogClicked();
	void on_radHeightClicked();
	void on_radNoneClicked();

private:
	Ui::StreetGenerator ui;
	StreetGen generator;
	QFileDialog fd;

	static const int DEFAULT_SIZE_X;
	static const int DEFAULT_SIZE_Y;

	QPoint size;

	void initialiseSystem();
	QString getFileChoice();


};

#endif // STREETGENERATOR_H
