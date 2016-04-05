//StructuredBuffer<PointLight> AllPointLights;
//
//ConstantsBuffer(LightTileAssignmentConsts, cs, 0)
//{
//	float4x4 g_mProjInv;
//	float4 g_ZNearFar;
//}
//
//[numthreads(PIXELS_PER_TILE, PIXELS_PER_TILE, 1)]
//void main(uint3 GroupId : SV_GroupID, uint3 DispatchThreadId : SV_DispatchThreadID)
//{
//	float zNear = g_ZNearFar.x;
//	float zFar = g_ZNearFar.y;
//	float xMin = GroupId.x * PIXELS_PER_TILE;
//	float yMin = GroupId.y * PIXELS_PER_TILE;
//	float xMax = xMin + PIXELS_PER_TILE;
//	float yMax = yMin + PIXELS_PER_TILE;
//
//	float4 leftBottomInScreenSpace = ConvertScreenToView(float(xMin, yMin), 1.0, 1.0);
//	float4 rightTopInScreenSpace = ConvertScreenToView(float(xMax, yMax), 1.0, 1.0);
//
//	//float4 frustum[6];
//}
[numthreads(1, 1, 1)]
void main()
{
}