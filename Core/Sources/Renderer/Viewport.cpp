#include "stdafx.h"
#include "Viewport.h"


Viewport::Viewport()
{
}


Viewport::~Viewport()
{
}


void Viewport::setViewport(float leftTopX, float leftTopY, float width, float height)
{
	_leftTopX = leftTopX;
	_leftTopY = leftTopY;
	_width = width;
	_height = height;
}


void Viewport::getViewport(float& leftTopX, float& leftTopY, float& width, float& height) const
{
	leftTopX = _leftTopX;
	leftTopY = _leftTopY;
	width = _width;
	height = _height;
}
