#include "PointLightShadowMapMinFilter_common.h"

[maxvertexcount(18)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> cubeMapStream)
{
	for (uint f = 0; f < 6; ++f)
	{
		for (uint i = 0; i < 3; ++i)
		{
			GSOutput result;
			result.RTIndex = f;
			result.position = input[i].position;
			result.texcoord = input[i].texcoord;
			cubeMapStream.Append(result);
		}
		cubeMapStream.RestartStrip();
	}
}