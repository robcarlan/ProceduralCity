#pragma once
#include <QColor>

static QColor SKYSCRAPER_COL = QColor(90, 194, 255);
static QColor RESIDENTIAL_COL = QColor(255, 73, 63);
static QColor INDUSTRIAL_COL = QColor(45, 94, 93);
static QColor COMMERCIAL_COL = QColor(45, 94, 255);
static QColor NONE_COL = QColor(255, 255, 255);

static float defaultRoadWidth = 5.0f;

//BuildingVariables
static float minBaseHeight = 5.0f;
static float maxBaseheight = 10.0f;
static float minRoofHeight = 2.0f;
static float maxRoofHeight = 5.0f;
static float minRoofSizeFactor = 0.5f;
static float minTowerHeight = 10.0f;
static float maxTowerHeight = 25.0f;
static float minCommercialHeight = 10.0f;
static float maxCommercialHeight = 20.0f;
static float skyscraperHeightAddition = 20.0f;

static float minExtrusionDepth = 4.0f;

static float doubleTowerPossibility = 0.10f;
static float createBaseProb = 0.8f;
static float createRoofProb = 0.5f;
static float createTowerProb = 0.5f;
static float minTowerSizeReduceFactor = 0.7f; //Min amount to reduce size by

//House variables
static float suburbanHouseHeight = 7.0f;
static float minHousePieceSize = 5.0f;
static float urbanHouseHeightMin = 10.0f;
static float urbanHouseHeightMax = 30.0f;
static float urbanHouseMinPopDensity = 0.3f;

//Industrial Variables
static float industrialMinHeight = 10.0f;
static float industrialMaxHeight = 40.0f;
static float chimneyMinHeight = 1.0f;
static float chimneyMaxHeight = 4.0f;

static float getBetween(int random, float min, float max) {
	return min + (max - min) * (static_cast<float>(random) / static_cast<float>(RAND_MAX));
}

enum class LOD {
	LOD_MIN,
	LOD_MEDIUM,
	LOD_DETAILED,
	LOD_MAX
};

enum class geogType {
	LAND,
	WATER,
	PARK
};

enum class roadType {
	STREET,
	MAINROAD
};

enum class roadPattern {
	NOPATTERN,
	MANHATTAN,
	RADIAL,
	SANFRAN
};

enum buildingStyle {
	RESIDENTIAL,
	COMMERCIAL,
	INDUSTRIAL,
	NONE
};

static QColor getLotColour(buildingStyle style) {
	switch (style) {
	case COMMERCIAL:
		return COMMERCIAL_COL;
	case INDUSTRIAL:
		return INDUSTRIAL_COL;
	case RESIDENTIAL:
		return RESIDENTIAL_COL;
	case NONE:
		return NONE_COL;
	default:
		return NONE_COL;
	}
}