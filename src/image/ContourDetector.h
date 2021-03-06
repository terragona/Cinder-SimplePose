/*
* Copyright (c) 2015-2016, Lasse Farnung Laursen - http://www.lasselaursen.com
*
* This file is a part of SimplePose, and subject to the new and revised BSD license.
* Please see the LICENSE file for more information.
* All other rights reserved.
*/

#ifndef SIMPLEPOSE_CONTOURDETECTOR_H_INCLUDED
#define SIMPLEPOSE_CONTOURDETECTOR_H_INCLUDED

#include "cinder/Surface.h"
#include "Contour.h"	// Required due to enum
#include "ContourMap.h"	// Required due to enum

#include "cinder/PolyLine.h"

namespace SimplePose {

class ContourDetector {

public:

	ContourDetector( unsigned int incomingImagesWidth, unsigned int incomingImagesHeight );
	~ContourDetector();

	void process( ci::Channel8uRef const& surface );

	ci::Channel8uRef getDebugImg() { return mImageDebug; };

	void drawAllContours();

	void testContourCalculation();

	std::vector<Contour> const & getContours() const { return mContours; }

protected:

	void detectContours();
	Contour annotateContour( ci::ivec2 const &pos, Contour::TYPE const & borderType, ContourMap::NeighborDirectionCW const & startingDirCW );

	inline unsigned int const posToIndex( ci::ivec2 const &pos ) { return ( pos.y * kmImgBorderedWidth ) + pos.x; };
	inline unsigned int const posToIndex( unsigned int const & x, unsigned int const & y ) { return ( y * kmImgBorderedWidth ) + x; };

	unsigned int mContourCounter;
	unsigned int mLatestBorderEncountered;
	//unsigned int mLastPixelValue;

	ci::Surface8uRef mImageProcessed;
	ci::Surface8uRef mImageProcessedBordered;

	ci::Channel8uRef mImageDebug;

	std::unique_ptr<ContourMap> mContourMap;
	std::vector<Contour> mContours;

	unsigned int const kmIncomingImgsWidth, kmIncomingImgsHeight;
	unsigned int const kmImgBorderedWidth, kmImgBorderedHeight;
};

};

#endif /* SIMPLEPOSE_CONTOURDETECTOR_H_INCLUDED */