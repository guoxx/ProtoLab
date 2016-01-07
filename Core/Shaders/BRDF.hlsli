// ----
// copy from UE4
// Clamp the base, so it's never <= 0.0f (INF/NaN).
float ClampedPow(float X, float Y)
{
	return pow(max(abs(X),0.000001f),Y);
}

float PhongShadingPow(float X, float Y)
{
	// The following clamping is done to prevent NaN being the result of the specular power computation.
	// Clamping has a minor performance cost.

	// In HLSL pow(a, b) is implemented as exp2(log2(a) * b).

	// For a=0 this becomes exp2(-inf * 0) = exp2(NaN) = NaN.

	// As seen in #TTP 160394 "QA Regression: PS3: Some maps have black pixelated artifacting."
	// this can cause severe image artifacts (problem was caused by specular power of 0, lightshafts propagated this to other pixels).
	// The problem appeared on PlayStation 3 but can also happen on similar PC NVidia hardware.

	// In order to avoid platform differences and rarely occuring image atrifacts we clamp the base.

	// Note: Clamping the exponent seemed to fix the issue mentioned TTP but we decided to fix the root and accept the
	// minor performance cost.

	return ClampedPow(X, Y);
}
//
// ----

float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * DIVPI;
}

float Distribution(float Roughness, float NdotH)
{
#if 1
	float a = Roughness * Roughness;
	float a2 = a * a;
	float n = 2 / a2 - 2;
	return (n+2) / (2*PI) * PhongShadingPow( NdotH, n );		// 1 mad, 1 exp, 1 mul, 1 log
#else
	float a = Roughness * Roughness;
	float a2 = a * a;
	float d = ( NdotH * a2 - NdotH ) * NdotH + 1;	// 2 mad
	return a2 / ( PI*d*d );					// 4 mul, 1 rcp
#endif
}

float3 Fresnel(float3 SpecularColor, float VoH)
{
#if 1
	return SpecularColor;
#else
	float Fc = pow(VoH, 5);
	return SpecularColor + (1 - SpecularColor) * Fc;
#endif
}

float GeometricVisibility(float Roughness, float NdotL, float NdotV)
{
#if 0
	return 0.25;
#else
	float k = sqrt(Roughness) * 0.5;
	float Vis_SchlickV = NdotV * (1 - k) + k;
	float Vis_SchlickL = NdotL * (1 - k) + k;
	return 0.25 / (Vis_SchlickV * Vis_SchlickL);
#endif
}

float3 MicrofacetSpecular(float3 SpecularColor, float Roughness, float3 V, float3 N, float3 L)
{
	float3 H = normalize(V + L);
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = saturate(dot(N, V));
	NdotV = max( 0.001, NdotV );
	NdotL = max( 0.001, NdotL );

	float D = Distribution(Roughness, NdotH);
	float3 F = Fresnel(SpecularColor, VdotH);
	float G = GeometricVisibility(Roughness, NdotL, NdotV);
	return D * F * G / (PI * NdotH * VdotH);
}