#include "streetgenerator.h"
#include "StreetGen.h"

StreetGenerator::StreetGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

StreetGenerator::~StreetGenerator()
{

}

void StreetGenerator::on_comboBox_activated(const QString &arg1) {
	QMessageBox *dialog = new QMessageBox();
	dialog->setText("Hello there");
	dialog->show();
}

void StreetGenerator::on_buttonHMap_clicked() {
	QString fName = getFileChoice();

	if (fName != QString::null) {
		QFile res(fName);
		QFileInfo info(res);
		ui.labelHMap->setText("Height Map: " + info.fileName());
	}

	StreetGen gen;
	gen.Run();
	std::vector<Road> generated = gen.generatedRoads;

	for (int i = 0; i < generated.size(); i++) {
		QString result(tr(generated.at(i).getEnd().printPoint().c_str()));
		QMessageBox *lol = new QMessageBox(QMessageBox::Icon::Information, QString("Box"), result);
		lol->show();
	}
}

QString StreetGenerator::getFileChoice() {
	QString fName = fd.getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
	return fName;
}
