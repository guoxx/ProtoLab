#include "stdafx.h"
#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	RHI::getInst().destroyResource(_positionBuffer);
	RHI::getInst().destroyResource(_albedoBuffer);
	RHI::getInst().destroyResource(_normalBuffer);
	RHI::getInst().destroyResource(_texcoordBuffer);
	RHI::getInst().destroyResource(_indexBuffer);
	RHI::getInst().destroyInputLayout(_vertexDeclaration);
}
