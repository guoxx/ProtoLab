#pragma once

class Viewport
{
public:

	Viewport();
	~Viewport();

	void setViewport(float leftTopX, float leftTopY, float width, float height);
	void getViewport(float& leftTopX, float& leftTopY, float& width, float& height) const;

	float getMinDepth() const
	{
		return _minDepth;
	}

	float getMaxDepth() const
	{
		return _maxDepth;
	}

private:

	float _leftTopX{0};	
	float _leftTopY{0};
	float _width{0}; 
	float _height{0};
	float _minDepth{0};
	float _maxDepth{1};
};

