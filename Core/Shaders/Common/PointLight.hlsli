
float pointLightDistanceFalloff(float distance, float radiusStart, float radiusEnd)
{
	// Falloff function proposed by UE4, Real Shading in Unreal Engine 4
	float falloff = pow(saturate(1 - pow((distance/radiusEnd), 4)), 2) / (distance + 1);
	return falloff;
	/*
	if (distance <= radiusStart)
	{
		falloff = 1.0f;
	}
	else if (distance >= radiusEnd)
	{
		falloff = 0.0f;
	}
	else
	{
		falloff = (radiusEnd - distance) / (radiusEnd - radiusStart);
	}
	return falloff;
	*/
}

float3 pointLightIrradiance(float3 lightIntensity, float distance, float radiusStart, float radiusEnd)
{
	float distanceFalloff = pointLightDistanceFalloff(distance, radiusStart, radiusEnd);
	float3 irradiance = lightIntensity * distanceFalloff;
	return irradiance;	
}

int getFaceOfPointLightShadowMap(float3 lightPosition, float3 position)
{
	float3 LightVector = position - lightPosition;
	float3 AbsLightVector = abs(LightVector);
	float MaxCoordinate = max(AbsLightVector.x, max(AbsLightVector.y, AbsLightVector.z));
	int CubeFaceIndex = 0;
	if (MaxCoordinate == AbsLightVector.x)
	{
		CubeFaceIndex = AbsLightVector.x == LightVector.x ? 0 : 1;
	}
	else if (MaxCoordinate == AbsLightVector.y)
	{
		CubeFaceIndex = AbsLightVector.y == LightVector.y ? 2 : 3;
	}
	else
	{
		// we use right hand coordinate
		CubeFaceIndex = AbsLightVector.z == LightVector.z ? 5 : 4;
	}
	return CubeFaceIndex;
}