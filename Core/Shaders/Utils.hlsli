
float4 ConvertProjToView(float4 posInProj, float4x4 mProjInv)
{
	float4 p = mul(posInProj, mProjInv);
	p /= p.w;
	return p;
}

float2 ConvertScreenToView(float2 screenPos, float2 viewport)
{
	return (screenPos / viewport) * 2.0f + 1.0f;
}