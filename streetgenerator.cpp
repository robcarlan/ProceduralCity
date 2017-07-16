#pragma once
#include "streetgenerator.h"
#include "Enums.cpp"

#include <QSettings>

const int StreetGenerator::DEFAULT_SIZE_X = 2048;
const int StreetGenerator::DEFAULT_SIZE_Y = 2048;

extern float minBaseHeight;
extern float maxBaseheight;
extern float minRoofHeight;
extern float maxRoofHeight;
extern float minRoofSizeFactor;
extern float minTowerHeight;
extern float maxTowerHeight;
extern float minCommercialHeight;
extern float maxCommercialHeight;
extern float skyscraperHeightAddition;

extern float minExtrusionDepth;

extern float doubleTowerPossibility;
extern float createBaseProb;
extern float createRoofProb;
extern float createTowerProb;
extern float minTowerSizeReduceFactor; //Min amount to reduce size by

									   //House variables
extern float suburbanHouseHeight;
extern float minHousePieceSize;
extern float urbanHouseHeightMin;
extern float urbanHouseHeightMax;
extern float urbanHouseMinPopDensity;

//Industrial Variables
extern float industrialMinHeight;
extern float industrialMaxHeight;
extern float chimneyMinHeight;
extern float chimneyMaxHeight;

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
		ui.cmdFilterRoads->setEnabled(true);

}

void StreetGenerator::onClickSave() {
	//To implement
}

void StreetGenerator::onClickSaveImage() {
	//MsgBox to save whole view or just what can be seen
}

void StreetGenerator::onClickFilterRoads() {
	assert(streetsGenerated);

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
		ui.spnBaseHeight->value(), 
		ui.spnHeightScale->value()	,
		ui.chkTimeSeed->isChecked() ? time(NULL) : ui.intSeedValue->value()
		);

	QElapsedTimer lotTimer;
	lotTimer.start();

	cityView.createLots();

	qDebug() << cityView.getLots().size() << " lots created.";
	qDebug() << "Lots created in: " << lotTimer.elapsed() / 1000.0f << " seconds.";

	view.addLots(cityView.getLots());
	view.setDrawLots(true);
	view.setDrawRegions(false);
	
	ui.chkShowRegions->setChecked(false);
	ui.chkShowLots->setEnabled(true);
	ui.chkShowLots->setChecked(true);
	lotsGenerated = true;

	//Enable 3d view
	ui.cmdCreateView->setEnabled(true);
}

void StreetGenerator::onClickCreateRegions() {
	assert(streetsFiltered);

	cityView.resetRegions();
	cityView.getRegionGenerator().setParams(
		ui.spinMinBuildingArea->value(),
		ui.spinMaxBuildingArea->value(),
		ui.spinLotGenerationOffset->value(),
		ui.spinMinLotDim->value(),
		ui.spinMaxLotDim->value(),
		ui.spinHighwayWidth->value(),
		ui.spinStreetWidth->value(),
		ui.chkAllowLotMerge->isChecked(),
		ui.spnBaseHeight->value(),
		ui.spnHeightScale->value(),
		ui.chkTimeSeed->isChecked() ? time(NULL) : ui.intSeedValue->value()
		);
	cityView.setImageData(pop, bType, height, geog);

	cityView.createRegions();

	//Now indicate this to 2D view
	view.addRegions(cityView.getRegions());
	view.setDrawRegions(true);

	ui.chkShowRegions->setEnabled(true);
	ui.chkShowRegions->setChecked(true);
	ui.cmdCreateBuildingLots->setEnabled(true);
	regionsGenerated = true;
}

void StreetGenerator::onClickShowRendered() {
	assert(lotsGenerated);

	//Prepare new form, pass geometry
	//Build geometry, show as progres dialog
	//Open view
	setBuildingParameters();

	BOOST_FOREACH(BuildingLot* lot, cityView.getLots()) {
		int seed = rand();
		lot->setSeed(seed);

		if (lot->getPopDensity() > 0.0f) {
			if (getBetween(rand(), 0.0f, 1.0f) < ui.spnSkyscraperPlacementChance->value()) 
				lot->setSkyscraper(true);
		}

		lot->createBuilding();
	}

	CityView3D *cview = new CityView3D();
	cview->setData(&cityView, &height, &geog, ui.spnBaseHeight->value(), ui.spnHeightScale->value());
	cview->setRoadData(ui.spinStreetWidth->value(), ui.spinHighwayWidth->value());
	cview->setAnimating(true);
	cview->resize(1600, 1000);
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
	QElapsedTimer genTime;
	genTime.start();

	if (!generator.isReady())
		initialiseSystem();
	//generate the stuff
	generator.Run();

	ui.cmdFilterRoads->setEnabled(true);
	streetsGenerated = true;

	qDebug() << "Streets generated in: " << genTime.elapsed() / 1000.0f << " seconds.";
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
	QString working_dir = QDir::currentPath();
	QString dName = dir.getExistingDirectory(this, tr("Open Image"), working_dir);
	QDir directory = QDir(dName);
	QStringList files = directory.entryList();

	bool gotPop, gotHeight, gotGeog, gotPattern, gotBuildings, gotConfig;
	gotPop = gotHeight = gotGeog = gotPattern = gotBuildings = gotConfig = false;

	//Find files by exact filename, update render views.
	BOOST_FOREACH(QString file, files) {
		if (file == tr("population.png")) {
			gotPop = true;
			QFile res(dName + "/population.png");
			QFileInfo info(res);
			ui.labelPMap->setText("Population Map: " + info.fileName());
			ui.popMapRender->setImage(res.fileName());
		}
		else if (file == tr("heightmap.png")) {
			gotHeight = true;
			QFile res(dName + "/heightmap.png");
			QFileInfo info(res);
			ui.labelHMap->setText("Heightmap: " + info.fileName());
			ui.hMapRender->setImage(res.fileName());
		}
		else if (file == tr("pattern.png")) {
			gotPattern = true;
			QFile res(dName + "/pattern.png");
			QFileInfo info(res);
			ui.labelSMap->setText("Street Patterns: " + info.fileName());
			ui.patternMapRender->setImage(res.fileName());
		}
		else if (file == tr("geography.png")) {
			gotGeog = true;
			QFile res(dName + "/geography.png");
			QFileInfo info(res);
			ui.labelGMap->setText("Geography Map: " + info.fileName());
			ui.geogMapRender->setImage(res.fileName());
		}
		else if (file == tr("buildings.png")) {
			gotGeog = true;
			QFile res(dName + "/buildings.png");
			QFileInfo info(res);
			ui.labelBMap->setText("Building Map: " + info.fileName());
			ui.buildingTypeRender->setImage(res.fileName());
		}
		else if (file == tr("parameters.ini")) {
			//Load config
			gotConfig = true;

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
	geog =  geogSet ?
		QImage(ui.geogMapRender->getImage()->scaled(size.x(), size.y())) : white;
	height = heightSet ? 
		QImage(ui.hMapRender->getImage()->scaled(size.x(), size.y())) : white;
	pop = popSet ? 
		QImage(ui.popMapRender->getImage()->scaled(size.x(), size.y())) : white;
	pattern = patternSet ?
		QImage(ui.patternMapRender->getImage()->scaled(size.x(), size.y())) : white;
	bType = buildingSet ?
		QImage(ui.buildingTypeRender->getImage()->scaled(size.x(), size.y())) : white;
	setStreetParameters();
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

void StreetGenerator::setStreetParameters() {
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
	generator.setStreetDelay(ui.spnStreetDelay->value());
	generator.setMainroadFollowLength(ui.spnMainroadFollowLength->value());
	generator.setStreetVariation(ui.spnLengthVariation->value());
	generator.setStartParams(
		Point(ui.spnStartX->value(), ui.spnStartY->value()),
		Point(ui.spnEndX->value(), ui.spnEndY->value()));
}

void StreetGenerator::setBuildingParameters() {
	float skyscraperHeight = ui.spnSkyscraperHeight->value();
	float commercialHeight = ui.spnCommercialHeight->value();
	float residentialHeight = ui.spnCommercialHeight->value();
	float industrialHeight = ui.spnIndustrialHeight->value();
	float highriseHeight = ui.spnHighriseHeight->value();

	minBaseHeight = 4.0f;
	maxBaseheight = 6.0f;
	minRoofHeight = 1.0f;
	maxRoofHeight = 3.0f;
	minRoofSizeFactor = 0.5f;
	minTowerHeight = commercialHeight - minBaseHeight;
	maxTowerHeight = commercialHeight - minBaseHeight;
	minCommercialHeight = commercialHeight * 0.7f;
	maxCommercialHeight = commercialHeight * 1.3f;
	skyscraperHeightAddition = skyscraperHeight - commercialHeight;

	minExtrusionDepth = 4.0f;

	doubleTowerPossibility = 0.10f;
	createBaseProb = 0.8f;
	createRoofProb = 0.5f;
	createTowerProb = 0.5f;
	minTowerSizeReduceFactor = 1.0f; //Min amount to reduce size by

	//House variables
	suburbanHouseHeight = residentialHeight;
	minHousePieceSize = 5.0f;
	urbanHouseHeightMin = highriseHeight * 0.7f;
	urbanHouseHeightMax = highriseHeight * 1.3f;
	urbanHouseMinPopDensity = 0.3f;

	//Industrial Variables
	industrialMinHeight = industrialHeight * 0.7f;
	industrialMaxHeight = industrialHeight * 1.3f;
	chimneyMinHeight = 1.0f;
	chimneyMaxHeight = 4.0f;

}

void StreetGenerator::loadConfig(QString fpath)
{
	QSettings fwrite(fpath, QSettings::Format::IniFormat);
}

void StreetGenerator::saveConfig(QString destPath)
{
	QSettings toWrite;

	toWrite.setValue("key", "value");


	toWrite.setPath(QSettings::Format::IniFormat, QSettings::Scope::UserScope, destPath);
	//Writes to permanent storage on destruction.
}

