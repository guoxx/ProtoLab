#include "stdafx.h"
#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
	RHI::GetInst().destroyResource(_positionBuffer);
	RHI::GetInst().destroyResource(_albedoBuffer);
	RHI::GetInst().destroyResource(_normalBuffer);
	RHI::GetInst().destroyResource(_texcoordBuffer);
	RHI::GetInst().destroyResource(_indexBuffer);
	RHI::GetInst().destroyVertexDeclaration(_vertexDeclaration);
}
