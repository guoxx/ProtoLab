
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

float GetLinearDepth(float d, float zNear, float zFar)
{
    return - (zFar * zNear) / (d * (zFar - zNear) - zFar);
}

float3 ReconstructCameraSpacePosition(float2 screenPos, float depth, float4x4 mProjInv)
{
    // Normalized Device Coordinates
    float4 ndcPosition = float4(screenPos*2-1, depth, 1);
    float4 pos = mul(ndcPosition, mProjInv);
	pos /= pos.w;
	return pos.xyz;
}

float3 ReconstructWorldSpacePosition(float2 screenPos, float depth, float4x4 mProjInv, float4x4 mViewInv)
{
	float3 posCS = ReconstructCameraSpacePosition(screenPos, depth, mProjInv);
	float3 posWS = mul(float4(posCS, 1), mViewInv).xyz;
	return posWS;
}