#include "stdafx.h"
#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	RHI::destroyResource(_positionBuffer);
	RHI::destroyResource(_albedoBuffer);
	RHI::destroyResource(_normalBuffer);
	RHI::destroyResource(_texcoordBuffer);
	RHI::destroyResource(_indexBuffer);
	RHI::destroyVertexDeclaration(_vertexDeclaration);
}
