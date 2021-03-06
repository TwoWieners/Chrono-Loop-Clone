/// Header file for storing vertex declarations for input and out from vertex shaders

#ifndef VERTEX_LAYOUTS_HLSLI
#define VERTEX_LAYOUTS_HLSLI

////////////////////////////////////////////////////////////
// Input vertices

struct VERTEX_POS
{
	float4 position :	POSITION0;
};

struct VERTEX_POSCOLOR
{
	float4 position :	POSITION0;
	float4 color	:	COLOR0;
};

struct VERTEX_POSTEX
{
	float4 position :	POSITION0;
	float4 texCoord :	TEXCOORD0;
};

struct VERTEX_POSNORMTEX
{
	float4 position :	POSITION0;
	float4 normal	:	NORMAL0;
	float4 texCoord :	TEXCOORD0;
};

struct VERTEX_POSNORMTANTEX
{
	float4 position		: POSITION0;
	float4 normal		: NORMAL0;
	float4 texCoord		: TEXCOORD0;
	float4 tangent		: TANGENT0;
};

struct VERTEX_POSBONEWEIGHT
{
	float4 position :	POSITION0;
	int4   bone     :   BONEINDICES0;
	float4 weights  :   BONEWEIGHTS0;
};

struct VERTEX_POSBONEWEIGHTNORMTEX
{
	float4 position :	POSITION0;
	int4   bone     :   BONEINDICES0;
	float4 weights  :   BONEWEIGHTS0;
	float4 normal	:	NORMAL0;
	float4 texCoord :	TEXCOORD0;
};

struct VERTEX_POSBONEWEIGHTNORMTANTEX
{
	float4 position :	POSITION0;
	float4 normal	:	NORMAL0;
	float4 texCoord :	TEXCOORD0;
	int4   bone     :   BONEINDICES0;
	float4 weights  :   BONEWEIGHTS0;
	float4 tangent		: TANGENT0;
	float determinant	: TANGENT1;
};

struct VERTEX_TERRAIN
{
	float2 position : POSITION0;
	float negXScale : NEIGHTBOR_SCALE0;
	float negYScale : NEIGHTBOR_SCALE1;
	float posXScale : NEIGHTBOR_SCALE2;
	float posYScale : NEIGHTBOR_SCALE3;
    uint  vertexId   : SV_VertexID;
};

////////////////////////////////////////////////////////////
// Output vertices
struct TwoDVertexOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

struct TwoDVertexWithPositionOut
{
	float4 position			: SV_POSITION;
	float4 pixelPosition	: TEXCOORD0;
	float2 texCoord			: TEXCOORD1;
};

struct PosColorOut
{
	float4 position			: SV_POSITION;
	float4 color			: COLOR0;
};

struct CubeVertOut
{
	float4 position			: SV_POSITION;
	float3 texCoord			: TEXCOORD0;
};

struct VertexWithPositionOut
{
	float4 position			: SV_POSITION;
	float4 pixelPosition	: TEXCOORD0;
};

struct GBufferVertexOut
{
	float4 position			: SV_POSITION;
	float3 normalW			: NORMAL0;
	float2 texCoord			: TEXCOORD0;
	float2 depthDiv			: TEXCOORD1;
};

struct GBufferTessVertexOut
{
	float3 positionW		: POSITION;
	float  tessFactor		: TESS;	
	float3 normalW			: NORMAL0;
	float2 texCoord			: TEXCOORD0;
};

struct GBufferHullOut
{
	float3 positionW		: POSITION;
	float3 normalW			: NORMAL0;
	float2 texCoord			: TEXCOORD0;
};

struct HullConstTriOut
{
	float EdgeTessFactor[3]	: SV_TessFactor; 
	float InsideTessFactor	: SV_InsideTessFactor; 
};

struct GBufferBumpVertexOut
{
	float4 position			: SV_POSITION;
	float2 texCoord			: TEXCOORD0;
	float2 depthDiv			: TEXCOORD1;
	float3x3 tbn			: TEXCOORD2;
};

struct GBufferBumpTessVertexOut
{
	float3 positionW		: POSITION;
	float  tessFactor		: TESS;	
	float3 normalW			: NORMAL0;
	float3 tangentW			: TANGENT0;
	float determinant		: TANGENT1;
	float2 texCoord			: TEXCOORD0;
};

struct GBufferBumpTessHullOut
{
	float3 positionW		: POSITION;
	float3 normalW			: NORMAL0;
	float3 tangentW			: TANGENT0;
	float determinant		: TANGENT1;
	float2 texCoord			: TEXCOORD0;
};

struct PointDepthVertOut
{
	float4 position : SV_POSITION;
	float3 normalW	: NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct DepthVertOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float2 depth	: TEXCOORD1;
};

struct DepthGeoOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float2 depth	: TEXCOORD1;
    uint RTIndex    : SV_RenderTargetArrayIndex;
};


struct ForwardRenderVertexOut
{
	float4 position		: SV_POSITION;
	float4 normal		: TEXCOORD0;
	float2 texCoord		: TEXCOORD1;
	float4 screenPos	: TEXCOORD2;
	float4 worldPos		: WORLDPOS;
};

struct TESSELATION_TEST_VS_OUT
{
	float3 position : POSITION;
	float4 normal	: NORMAL0;
	float2 texCoord	: TEXCOORD0;
};

struct TESSELATION_TEST_HS_OUT
{
	float4 position : SV_POSITION;
	float4 normal	: NORMAL0;
	float2 texCoord	: TEXCOORD0;
};

struct TESS_TERRAIN_VS_OUT
{
	float3 position : POSITION;
	float2 worldXZ	: TEXCOORD0;

	float negXScale : NEIGHTBOR_SCALE0;
	float negYScale : NEIGHTBOR_SCALE1;
	float posXScale : NEIGHTBOR_SCALE2;
	float posYScale : NEIGHTBOR_SCALE3;
};

struct TESS_TERRAIN_HS_OUT
{
    float3 position : POSITION;
};

struct TESS_TERRAIN_HS_DATA_OUT
{
    float edges[4]        : SV_TessFactor;
	float inside[2]       : SV_InsideTessFactor;
    float2 worldXZ[4]    : TEXCOORD4;
	float3 debugColor[5] : COLOR;			// 5th is centre

};

struct TESS_TERRAIN_DS_OUT
{
	float4 position			: SV_POSITION;
	float2 worldXZ			: TEXCOORD0;
	float2 depthDiv			: TEXCOORD1;
	float3 debugColor      : COLOR;

};

////////////////////////////////////////////////////////////
// Output pixels
struct GBufferFragOut
{
	float4 diffuse : SV_TARGET0;
	float4 specular : SV_TARGET1;
	float4 normal : SV_TARGET2;
	float4 depth : SV_TARGET3;
};

// Output fragment for forward rendering, normal and depth values kept for post-process use
struct ForwardFragOut
{
	float4 diffuse : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
};
#endif //VERTEX_LAYOUTS_HLSLI