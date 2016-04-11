#pragma once
#include <QColor>

static QColor SKYSCRAPER_COL = QColor(90, 194, 255);
static QColor RESIDENTIAL_COL = QColor(255, 73, 63);
static QColor INDUSTRIAL_COL = QColor(45, 94, 93);
static QColor COMMERCIAL_COL = QColor(45, 94, 255);
static QColor NONE_COL = QColor(255, 255, 255);

static float defaultRoadWidth = 5.0f;

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