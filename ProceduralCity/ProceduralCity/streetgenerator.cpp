#include "streetgenerator.h"

const int StreetGenerator::DEFAULT_SIZE_X = 2048;
const int StreetGenerator::DEFAULT_SIZE_Y = 2048;

StreetGenerator::StreetGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	generator = StreetGen::StreetGen();
	size.setX(DEFAULT_SIZE_X);
	size.setY(DEFAULT_SIZE_Y);

	//Initially bind Street Renderer to the street gen object
	ui.streetRender->initialiseRenderer(&generator);
}

StreetGenerator::~StreetGenerator()
{

}

void StreetGenerator::onClickReset() {
	initialiseSystem();
}

void StreetGenerator::onClickStep() {
	if (generator.isReady())
		generator.nextIteration();
}

void StreetGenerator::onClickSave() {
	//To implement
}

void StreetGenerator::onClickSaveImage() {
	//MsgBox to save whole view or just what can be seen
}

void StreetGenerator::onClickGenerate() {
	if (!generator.isReady())
		initialiseSystem();
	//generate the stuff
	generator.Run();
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

void StreetGenerator::clearHeightmap() {
	ui.hMapRender->clear();
}

void StreetGenerator::clearGeogmap() {
	ui.geogMapRender->clear();
}

void StreetGenerator::clearPopmap() {
	ui.popMapRender->clear();
}

void StreetGenerator::on_radPopClicked() {
	generator.streets.renderPop();
}

void StreetGenerator::on_radGeogClicked() {
	generator.streets.renderGeog();
}

void StreetGenerator::on_radHeightClicked() {
	generator.streets.renderHeight();
}

void StreetGenerator::on_radNoneClicked() {
	generator.streets.renderNone();
}

void StreetGenerator::initialiseSystem() {
	//Set images, set seed
	//Any images that have not been set should be replaced with a white image
	QImage white = QImage(1, 1, QImage::Format::Format_ARGB32);
	white.setPixel(0, 0, 0);
	white = white.scaled(size.x(), size.y());

	bool geogSet = ui.geogMapRender->isSet();
	bool heightSet = ui.hMapRender->isSet();
	bool popSet = ui.popMapRender->isSet();

	//Scale images to fit 
	QImage *geog =  geogSet ?
		new QImage(ui.geogMapRender->getImage()->scaled(size.x(), size.y())) : &white;
	QImage *height = heightSet ? 
		new QImage(ui.hMapRender->getImage()->scaled(size.x(), size.y())) : &white;
	QImage *pop = popSet ? 
		new QImage(ui.popMapRender->getImage()->scaled(size.x(), size.y())) : &white;

	generator.initialise();
	generator.setGeogMap(*geog, geogSet);
	qDebug() << "Geog: " << ui.geogMapRender->isSet() << "\n";
	generator.setHeightMap(*height, heightSet);
	qDebug() << "Height: " << ui.hMapRender->isSet() << "\n";
	generator.setPopMap(*pop, popSet);
	qDebug() << "Pop: " << ui.popMapRender->isSet() << "\n";

	//Set seed
	if (ui.chkTimeSeed->isChecked())
		generator.setSeed();
	else generator.setSeed(ui.intSeedValue->value());
}

QString StreetGenerator::getFileChoice() {
	QString fName = fd.getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
	return fName;
}


