#pragma once

class Viewport
{
public:

	Viewport();
	~Viewport();

	void setViewport(uint32_t leftTopX, uint32_t leftTopY, uint32_t width, uint32_t height);
	void getViewport(uint32_t& leftTopX, uint32_t& leftTopY, uint32_t& width, uint32_t& height) const;

private:

	uint32_t _leftTopX{0};	
	uint32_t _leftTopY{0};
	uint32_t _width{0}; 
	uint32_t _height{0};
};

