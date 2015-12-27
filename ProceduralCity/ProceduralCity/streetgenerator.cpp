#include "streetgenerator.h"

StreetGenerator::StreetGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	generator = StreetGen::StreetGen();
	//Initially bind Street Renderer to the street gen object
	ui.streetRender->initialiseRenderer(&generator);
}

StreetGenerator::~StreetGenerator()
{

}

void StreetGenerator::onClickGenerate() {
	//generate the stuff
	generator.Run();
	ui.streetRender->initialiseRenderer(&generator);
	ui.streetRender->repaint();
}

void StreetGenerator::on_comboBox_activated(const QString &arg1) {
	QMessageBox *dialog = new QMessageBox();
	dialog->setText("Hello there");
	dialog->show();
}

void StreetGenerator::on_pushButton_clicked() {
}

void StreetGenerator::on_buttonHMap_clicked() {
	QString fName = getFileChoice();

	if (fName != QString::null) {
		QFile res(fName);
		QFileInfo info(res);
		ui.labelHMap->setText("Height Map: " + info.fileName());
		ui.hMapRender->setImage(res.fileName());
	}
}

void StreetGenerator::on_buttonPMap_clicked() {
	QString fName = getFileChoice();

	if (fName != QString::null) {
		QFile res(fName);
		QFileInfo info(res);
		ui.labelPMap->setText("Population Map: " + info.fileName());
		ui.popMapRender->setImage(res.fileName());
	}
}

void StreetGenerator::on_buttonGMap_clicked() {
	QString fName = getFileChoice();

	if (fName != QString::null) {
		QFile res(fName);
		QFileInfo info(res);
		ui.labelGMap->setText("Geography Map: " + info.fileName());
		ui.geogMapRender->setImage(res.fileName());
	}
}

QString StreetGenerator::getFileChoice() {
	QString fName = fd.getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
	return fName;
}
