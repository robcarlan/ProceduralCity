#pragma once
#include <QColor>
#include <QImage>
#include <QRect>
#include "Point.h"

static QColor SKYSCRAPER_COL = QColor(90, 194, 255);
static QColor RESIDENTIAL_COL = QColor(255, 73, 63);
static QColor INDUSTRIAL_COL = QColor(45, 94, 39);
static QColor COMMERCIAL_COL = QColor(45, 94, 255);
static QColor NONE_COL = QColor(255, 255, 255);

static QRgb parkCol = qRgb(100, 170, 100);
static QRgb landCol = qRgb(200, 200, 200);
static QRgb waterCol = qRgb(140, 140, 220);

static float defaultRoadWidth = 5.0f;

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

static QRect smallestEnclosingRectangle(const std::list<Point> &bounds) {
	float minX, minY, maxX, maxY;
	minX = minY = 1000000;
	maxX = maxY = 0;
	auto pItr = bounds.begin();

	while (pItr != bounds.end()) {
		if (pItr->x() < minX) minX = pItr->x();
		if (pItr->y() < minY) minY = pItr->y();
		if (pItr->x() > maxX) maxX = pItr->x();
		if (pItr->y() > maxY) maxY = pItr->y();

		pItr++;
	}

	return QRect(minX, minY, maxX - minX, maxY - minY);
}

static geogType sampleGeog(int x, int y, QImage &geog) {
	QRgb val = geog.pixel(x, y);
	if (val == landCol) return geogType::LAND;
	else if (val == waterCol) return geogType::WATER;
	else if (val == parkCol) return geogType::PARK;
	else return geogType::LAND;
}

static float samplePop(int x, int y, QImage &pop) {
	QRgb lol = pop.pixel(x, y);
	return 1.0f - (lol % 256) / 256.0f;
	QColor val = QColor(pop.pixel(x, y));
	return (val.red() % 256) / 256.0f;
}