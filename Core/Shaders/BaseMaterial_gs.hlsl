#include "BaseMaterial_common.h"

[maxvertexcount(3)]
void main(triangle VSOutput input[3], inout TriangleStream< GSOutput > output)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = input[i].position;
		element.positionCS = input[i].positionCS;
		element.positionWS = input[i].positionWS;
		element.normalCS = input[i].normalCS;
		element.texcoord = input[i].texcoord;
		output.Append(element);
	}
	output.RestartStrip();
}