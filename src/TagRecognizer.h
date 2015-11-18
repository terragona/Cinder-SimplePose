/*
* Copyright (c) 2015, Lasse Farnung Laursen - http://www.lasselaursen.com
*
* This file is a part of SimplePose, and subject to the new and revised BSD license.
* Please see the LICENSE file for more information.
* All other rights reserved.
*/

/*

TagRecognizer takes care of knowing what sized tags we can recognize (bit pattern),

*/

#ifndef SIMPLEPOSE_TAG_RECOGNIZER
#define SIMPLEPOSE_TAG_RECOGNIZER

#include "cinder/Surface.h"

struct Tag;

class TagRecognizer {

public:

	TagRecognizer( unsigned short const &bitPatternSize );
	~TagRecognizer();

	ci::Surface8uRef getTagTex( unsigned int const &numTags );

private:

	void generateTags();

	std::vector<std::unique_ptr<Tag>> mRecognizedTags;
	//std::vector<Tag> mRecognizedTags;
	std::vector<unsigned long long> mInvalidTags;

	unsigned short const kmBitPatternSize;

	static unsigned short const kmMaxBitPattern = 6;
	static unsigned short const kmMinBitPattern = 2;

};

#endif /* SIMPLEPOSE_TAG_RECOGNIZER */