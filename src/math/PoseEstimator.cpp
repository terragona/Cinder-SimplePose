/*
* Copyright (c) 2015, Lasse Farnung Laursen - http://www.lasselaursen.com
*
* This file is a part of SimplePose, and subject to the new and revised BSD license.
* Please see the LICENSE file for more information.
* All other rights reserved.
*/

#include "PoseEstimator.h"
#include "Homography.h"

PoseEstimator::PoseEstimator( ci::mat3 const & intrinsicCameraParameters ) :
	mMatIntrinsicCameraParameters( intrinsicCameraParameters )
{
	mHomographyCalculator = std::unique_ptr<Homography>( new Homography() );

	mMatIntrinsicCameraParametersInverse = glm::inverse( mMatIntrinsicCameraParameters );

	// Set scale to identity matrix
	mMatScale = ci::mat3( 1.0f );

	auto tagMarkerWidth = 1.0f;
	auto tagMarkerHeight = 1.0f;
	
	// Long long ago, Daniel Wagner suggested using the following virtual coordinates
	mTagVirtualCoords[0].x = tagMarkerWidth / 2; mTagVirtualCoords[0].y = tagMarkerHeight / 2;
	mTagVirtualCoords[1].x = tagMarkerWidth / 2; mTagVirtualCoords[1].y = - tagMarkerHeight / 2;
	mTagVirtualCoords[2].x = - tagMarkerWidth / 2; mTagVirtualCoords[2].y = - tagMarkerHeight / 2;
	mTagVirtualCoords[3].x = - tagMarkerWidth / 2; mTagVirtualCoords[3].y = tagMarkerHeight / 2;
	

}

PoseEstimator::~PoseEstimator()
{

}

void PoseEstimator::estimatePose( ci::vec2 const tagCornerScreenCoords[4] )
{
	auto estimatedViewMatrix = estimateViewMatrix( tagCornerScreenCoords );

	extractPositionAndOrientationFromViewMatrix( estimatedViewMatrix );
}

ci::mat4 PoseEstimator::estimateViewMatrix( ci::vec2 const tagCornerScreenCoords[4] )
{
	// homography = mat3x3
	auto homography = mHomographyCalculator->getHomography( mTagVirtualCoords.data(), tagCornerScreenCoords );

	// Verify: does matrix multiplication work the way we expect it to?
	mMatRRTUnscaled = mMatIntrinsicCameraParametersInverse * homography;

	// Scale is more or less a NOOP for now
	mMatRRT = mMatRRTUnscaled * mMatScale;

	// Extract rotation and translation vectors
	ci::vec3 rotVec1 = glm::column( mMatRRT, 0 );
	ci::vec3 rotVec2 = glm::column( mMatRRT, 1 );
	ci::vec3 transVec = glm::column( mMatRRT, 2 );

	// Long long ago, Daniel Wagner Suggested the following operations to achieve the proper transform!
	// We also "negate" in this operation since a view matrix expects the translation to be negative.
	auto r1length = ci::length( rotVec1 );
	auto r2length = ci::length( rotVec2 );
	double transAvrg = ( r1length + r2length ) / 2;

	// Calculate the remaining rotation vector
	ci::vec3 rotVec3 = glm::cross( rotVec1, rotVec2 );

	// Ensure orthogonality - Is this needed? - On first look, yes due to difference in vector lengths...?
	rotVec1 = glm::cross( rotVec2, rotVec3 );

	//ci::mat3 transVecScaler = ci::mat3( -transAvrg );
	//transVec = transVec / transVecScaler;

	transVec.x /= -transAvrg;
	transVec.y /= -transAvrg;
	transVec.z /= -transAvrg;

	// Make rotation Matrix ortho-normal
	glm::normalize( rotVec1 );
	glm::normalize( rotVec2 );
	glm::normalize( rotVec3 );

	return ci::mat4(
		rotVec1.x, rotVec2.x, rotVec3.x, transVec.x,
		rotVec1.y, rotVec2.y, rotVec3.y, transVec.y,
		rotVec1.z, rotVec2.z, rotVec3.z, transVec.z,
		0, 0, 0, 1
		);
}

void PoseEstimator::extractPositionAndOrientationFromViewMatrix( ci::mat4 const & viewMatrix )
{

}