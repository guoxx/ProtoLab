#include "stdafx.h"
#include "Viewport.h"


Viewport::Viewport()
{
}


Viewport::~Viewport()
{
}


void Viewport::setViewport(uint32_t leftTopX, uint32_t leftTopY, uint32_t width, uint32_t height)
{
	_leftTopX = leftTopX;
	_leftTopY = leftTopY;
	_width = width;
	_height = height;
}


void Viewport::getViewport(uint32_t& leftTopX, uint32_t& leftTopY, uint32_t& width, uint32_t& height) const
{
	leftTopX = _leftTopX;
	leftTopY = _leftTopY;
	width = _width;
	height = _height;
}
