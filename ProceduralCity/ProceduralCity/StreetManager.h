#pragma once
#ifndef Q_MOC_RUN 
#include <boost\geometry\index\rtree.hpp>
#endif
///Used by StreetGenerator to handle storing roads and crossings. Allows methods to add / remove / connect these items, as well as spatial queries.
class StreetManager {
private:
	//Internally managed by an rtree
	//boost::geometry::index::rtree<
	//Manage connected roads
public:
	StreetManager();
	~StreetManager();
};

