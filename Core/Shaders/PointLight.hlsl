
float pointLightDistanceFalloff(float radius, float radiusStart, float radiusEnd)
{
	float falloff;
	if (radius <= radiusStart)
	{
		falloff = 1.0f;
	}
	else if (radius >= radiusEnd)
	{
		falloff = 0.0f;
	}
	else
	{
		falloff = (radiusEnd - radius) / (radiusEnd - radiusStart);
	}
	return falloff;
}

float3 pointLightIrradiance(float3 lightIntensity, float radius, float radiusStart, float radiusEnd)
{
	float falloff = pointLightDistanceFalloff(radius, radiusStart, radiusEnd);
	float3 irradiance = lightIntensity * falloff;
	return irradiance;	
}