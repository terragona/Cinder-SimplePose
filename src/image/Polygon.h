/*
* Copyright (c) 2015-2016, Lasse Farnung Laursen - http://www.lasselaursen.com
*
* This file is a part of SimplePose, and subject to the new and revised BSD license.
* Please see the LICENSE file for more information.
* All other rights reserved.
*/

#ifndef SIMPLEPOSE_POLYGON_H_INCLUDED
#define SIMPLEPOSE_POLYGON_H_INCLUDED

#include <vector>

#include "cinder/Color.h"
#include "cinder/PolyLine.h"

namespace SimplePose {

struct Polygon {

	Polygon();
	Polygon( std::vector<int> & coords );
	//Polygon( std::vector<ci::vec2> & coords );
	~Polygon();

	void draw() const;

	bool isConvex() const;

	inline bool isSquare() const { return mCoords.size() == 5; };

	float getContainedArea() const;

	std::vector<ci::vec2> mCoords;

	// For rendering purposes
	ci::Color mColor;
	ci::PolyLine2 mPolyLine;
};

};

#endif /* SIMPLEPOSE_POLYGON_H_INCLUDED */