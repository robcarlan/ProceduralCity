#include "streetgenerator.h"

const int StreetGenerator::DEFAULT_SIZE_X = 2048;
const int StreetGenerator::DEFAULT_SIZE_Y = 2048;

StreetGenerator::StreetGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	cityView = City();
	view = CityView2D();
	size.setX(DEFAULT_SIZE_X);
	size.setY(DEFAULT_SIZE_Y);
	generator = StreetGen(&view, Point(DEFAULT_SIZE_X, DEFAULT_SIZE_Y));

	//Initially bind Street Renderer to the street gen object
	ui.streetRender->initialiseRenderer(&view);
	dir.setFileMode(QFileDialog::DirectoryOnly);
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

	if (generator.isFinished())
		ui.cmdGenerateRegions->setEnabled(true);

}

void StreetGenerator::onClickSave() {
	//To implement
}

void StreetGenerator::onClickSaveImage() {
	//MsgBox to save whole view or just what can be seen
}

void StreetGenerator::onClickFilterRoads() {
	assert(streetsGenerated);
	generator.filterStreets();

	cityView.resetStreets();

	SimpleStreetGeometryCreator toSimple = SimpleStreetGeometryCreator();
	std::list<RoadIntersection*> intersections = generator.getGeneratedIntersections();
	std::list<Road*> roads = generator.getGeneratedRoads();
	cityView.getGeometry(toSimple.toGeometry(intersections, roads));

	//Reset view to display updated geometry.
	view.clearGeometry();
	view.addIntersections(cityView.intersections);
	view.addRoads(cityView.roads);

	streetsFiltered = true;
	ui.cmdGenerateRegions->setEnabled(true);
}

void StreetGenerator::onClickCreateLots() {
	assert(regionsGenerated);
	//Set params here
	cityView.getRegionGenerator().setParams(
		ui.spinMinBuildingArea->value(),
		ui.spinMaxBuildingArea->value(),
		ui.spinLotGenerationOffset->value(),
		ui.spinMinLotDim->value(),
		ui.spinMaxLotDim->value(),
		ui.spinHighwayWidth->value(),
		ui.spinStreetWidth->value(),
		ui.chkAllowLotMerge->isChecked(),
		ui.chkTimeSeed->isChecked() ? time(NULL) : ui.intSeedValue->value()
		);

	cityView.setImageData(pop, bType);
	cityView.createLots();
	view.addLots(cityView.getLots());
	view.setDrawLots(true);
	view.setDrawRegions(false);
	
	ui.chkShowLots->setEnabled(true);
	lotsGenerated = true;

	//Enable 3d view
	ui.cmdCreateView->setEnabled(true);
}

void StreetGenerator::onClickCreateRegions() {
	assert(streetsFiltered);

	cityView.resetRegions();
	cityView.createRegions();

	//Now indicate this to 2D view
	view.addRegions(cityView.getRegions());
	view.setDrawRegions(true);

	ui.chkShowRegions->setEnabled(true);
	ui.cmdCreateBuildingLots->setEnabled(true);
	regionsGenerated = true;
}

void StreetGenerator::onClickShowRendered() {
	assert(lotsGenerated);

	//Prepare new form, pass geometry
	//Build geometry, show as progres dialog
	//Open view
	CityView3D *cview = new CityView3D();
	cview->setData(&cityView);
	cview->setAnimating(true);
	cview->resize(1200, 1200);
	cview->show();
	
}

void StreetGenerator::onClickRenderRegions(bool render) {
	view.setDrawRegions(render);
}

void StreetGenerator::onClickRenderLots(bool render) {
	view.setDrawLots(render);
}

void StreetGenerator::onClickRenderVerts(bool render) {
	view.renderVertices(render);
}

void StreetGenerator::onClickGenerate() {
	if (!generator.isReady())
		initialiseSystem();
	//generate the stuff
	generator.Run();

	ui.cmdFilterRoads->setEnabled(true);
	streetsGenerated = true;
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

void StreetGenerator::on_buttonBMap_clicked() {
}

void StreetGenerator::on_buttonSMap_clicked() {
	QString fName = getFileChoice();

	if (fName != QString::null) {
		QFile res(fName);
		QFileInfo info(res);
		ui.labelSMap->setText("Pattern Map: " + info.fileName());
		ui.patternMapRender->setImage(res.fileName());
	}
}

//Load all images from directory by choosing based on filename. 
void StreetGenerator::cmdLoadDirectory() {
	QString dName = dir.getExistingDirectory(this, tr("Open Image"), "/");
	QDir directory = QDir(dName);
	QStringList files = directory.entryList();

	bool gotPop, gotHeight, gotGeog, gotPattern, gotBuildings;
	gotPop = gotHeight = gotGeog = gotPattern = gotBuildings = false;

	//Find files by exact filename, update render views.
	BOOST_FOREACH(QString file, files) {
		if (file == tr("population.png")) {
			gotPop = true;
			QFile res(dName + "/population.png");
			QFileInfo info(res);
			ui.labelPMap->setText("Population Map: " + info.fileName());
			ui.popMapRender->setImage(res.fileName());
		} else if (file == tr("heightmap.png")) {
			gotHeight = true;
			QFile res(dName + "/heightmap.png");
			QFileInfo info(res);
			ui.labelHMap->setText("Heightmap: " + info.fileName());
			ui.hMapRender->setImage(res.fileName());
		} else if (file == tr("pattern.png")) {
			gotPattern = true;
			QFile res(dName + "/pattern.png");
			QFileInfo info(res);
			ui.labelSMap->setText("Street Patterns: " + info.fileName());
			ui.patternMapRender->setImage(res.fileName());
		} else if (file == tr("geography.png")) {
			gotGeog = true;
			QFile res(dName + "/geography.png");
			QFileInfo info(res);
			ui.labelGMap->setText("Geography Map: " + info.fileName());
			ui.geogMapRender->setImage(res.fileName());
		} else if (file == tr("buildings.png")) {
			gotGeog = true;
			QFile res(dName + "/buildings.png");
			QFileInfo info(res);
			ui.labelBMap->setText("Building Map: " + info.fileName());
			ui.buildingTypeRender->setImage(res.fileName());
		}
	}

}

void StreetGenerator::clearBuildingMap() {
	ui.buildingTypeRender->clear();
}

void StreetGenerator::clearStreetmap() {
	ui.patternMapRender->clear();
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
	view.renderPop();
}

void StreetGenerator::on_radGeogClicked() {
	view.renderGeog();
}

void StreetGenerator::on_radHeightClicked() {
	view.renderHeight();
}

void StreetGenerator::on_radPatternClicked() {
	view.renderPattern();
}

void StreetGenerator::on_radNoneClicked() {
	view.renderNone();
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
	bool patternSet = ui.patternMapRender->isSet();
	bool buildingSet = ui.buildingTypeRender->isSet();

	//Reset ui elements
	ui.chkShowLots->setEnabled(false);
	ui.chkShowRegions->setEnabled(false);
	ui.cmdGenerateRegions->setEnabled(false);
	ui.cmdCreateBuildingLots->setEnabled(false);
	ui.cmdFilterRoads->setEnabled(false);
	ui.cmdCreateView->setEnabled(false);
	streetsFiltered = lotsGenerated = regionsGenerated = streetsGenerated = false;

	//Scale images to fit 
	QImage geog =  geogSet ?
		QImage(ui.geogMapRender->getImage()->scaled(size.x(), size.y())) : white;
	QImage height = heightSet ? 
		QImage(ui.hMapRender->getImage()->scaled(size.x(), size.y())) : white;
	pop = popSet ? 
		QImage(ui.popMapRender->getImage()->scaled(size.x(), size.y())) : white;
	QImage pattern = patternSet ?
		QImage(ui.patternMapRender->getImage()->scaled(size.x(), size.y())) : white;
	bType = buildingSet ?
		QImage(ui.buildingTypeRender->getImage()->scaled(size.x(), size.y())) : white;
	setParameters();
	generator.initialise();
	generator.setGeogMap(geog, geogSet);
	qDebug() << "Geog: " << ui.geogMapRender->isSet() << "\n";
	generator.setHeightMap(height, heightSet);
	qDebug() << "Height: " << ui.hMapRender->isSet() << "\n";
	generator.setPopMap(pop, popSet);
	qDebug() << "Pop: " << ui.popMapRender->isSet() << "\n";
	generator.setPatternMap(pattern, patternSet);
	qDebug() << "Pattern: " << ui.patternMapRender->isSet() << "\n";

	view.setGeog(&geog);
	view.setHeight(&height);
	view.setPattern(&pattern);
	view.setPop(&pop);

	//Set seed
	if (ui.chkTimeSeed->isChecked())
		generator.setSeed();
	else generator.setSeed(ui.intSeedValue->value());

	view.renderVertices(ui.chkShowVerts->isChecked());
}

QString StreetGenerator::getFileChoice() {
	QString fName = fd.getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
	return fName;
}

void StreetGenerator::setParameters() {
	generator.setExtendRadius(ui.spinRoadExtendLength->value());
	generator.setRoadSnap(math::sqr(ui.spinRoadSnapRadius->value()));
	generator.setMinLength(ui.spinMinRoadLength->value());
	generator.setRoadBranchProb(ui.spinRoadBranchProb->value());
	generator.setMaxAngleSearch(ui.slideRoadSearchAngle->value() * generator.d2rFactor);
	generator.setManhattanBlockWidth(ui.slideManhattanBlocKWidth->value());
	generator.setManhattanBlockHeight(ui.slideManhattanBlockHeight->value());
	generator.setMaxRoadRotate(ui.spinMaxRoadAngleChange->value() * generator.d2rFactor);
	generator.setRoadRotateInterval(ui.sliderSearchAngleIncrease->value() * generator.d2rFactor);
	generator.setMaxWaterTraverse(ui.spinMaxBridgeLength->value());
	generator.setMaxPruneFactor(ui.spinRoadPruneFactor->value());
	generator.setRoadSampleInterval(ui.sliderRoadSampleInterval->value());
	
	generator.setUsePatternWeighting(ui.radUseWeightedStreet->isChecked());

	generator.setRoadLength(ui.spinStreetLength->value());
	generator.setHighwayLength(ui.spinHighwayLength->value());
	generator.setPopRadiusSearch(ui.sliderPopSearchRadius->value());
	generator.setHighwayGrowthFactor(ui.highwayGrowthFactor->value());
	generator.setStreetGrowthFactor(ui.streetGrowthFactor->value());
}


