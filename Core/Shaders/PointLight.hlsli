
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