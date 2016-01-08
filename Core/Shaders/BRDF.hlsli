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

float sqr(float x) { return x*x; }

float Distribution(float Roughness, float NdotH)
{
	float m = Roughness;
	// Divide by PI is apply later
	float m2 = m * m;
	float f = ( NdotH * m2 - NdotH ) * NdotH + 1;
	return m2 / (f * f);
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
	float alphaG = Roughness;
	// This is the optimize version
	float alphaG2 = alphaG * alphaG ;
	float NdotL2 = NdotL * NdotL;
	float NdotV2 = NdotV * NdotV;
	// Original formulation of G_SmithGGX Correlated
	float lambda_v = (-1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5f;
	float lambda_l = (-1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5f;
	float G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
	float V_SmithGGXCorrelated = G_SmithGGXCorrelated / ( 4.0f * NdotL * NdotV );
	return V_SmithGGXCorrelated;
#endif
}

float3 MicrofacetSpecular(float3 SpecularColor, float Roughness, float3 V, float3 N, float3 L)
{
	float3 H = normalize(V + L);
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = abs( dot (N, V)) + 1e-5f;

	float D = Distribution(Roughness, NdotH);
	float3 F = Fresnel(SpecularColor, VdotH);
	float G = GeometricVisibility(Roughness, NdotL, NdotV);
	return D * F * G / PI;
}