#include "PreCompile.h"
#include "GameEngineCore.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineVertexes.h"
#include "GameEngineConstantBuffer.h"

#include "GameEngineRenderingPipeLine.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineInputLayout.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineBlend.h"

void EngineInputLayout()
{
	GameEngineVertex::inputLayoutInfo_.AddInputLayout("POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::inputLayoutInfo_.AddInputLayout("COLOR", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::inputLayoutInfo_.AddInputLayout("TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void EngineSubSetting()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	GameEngineRasterizer::Create("EngineRasterizer", rasterizerDesc);


	//typedef struct D3D11_BLEND_DESC
	//{
	//	BOOL AlphaToCoverageEnable;							알파블렌딩에도 멀티샘플링을 하는 기술인 알파 투 커버리지를 적용할 지 여부.
	//	BOOL IndependentBlendEnable;						렌더타겟들이 각자의 독자적인 블렌딩 설정값을 가지게 할 지 여부. 
	//														 true: 8가지 블렌딩 설정값을 가짐.
	//														 false: 0번 설정값만 블렌딩에 사용함.
	// 
	//	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];		렌더타겟에 적용될 알파블렌딩 설정값 정보를 저장할 D3D11_RENDER_TARGET_BLEND_DESC구조체 배열.
	//														 최대 8개 렌더타겟에 각각 다른 알파블렌딩 방식을 저장할 수 있다.

	//	typedef struct D3D11_RENDER_TARGET_BLEND_DESC
	//	{
	//		BOOL BlendEnable;						알파블렌딩을 할 지 여부.
	//		D3D11_BLEND SrcBlend;					알파블렌딩 공식의 srcFactor의 4개 원소를 어떻게 채울지 선택하는 옵션.
	//		D3D11_BLEND DestBlend;					알파블렌딩 공식의 destFactor의 4개 원소를 어떻게 채울지 선택하는 옵션.
	//		D3D11_BLEND_OP BlendOp;					알파블렌딩 공식의 옵션 부분에 어떤 연산을 적용할지 선택하는 옵션.
	//		D3D11_BLEND SrcBlendAlpha;				srcColor의 알파값 설정.
	//		D3D11_BLEND DestBlendAlpha;				destColor의 알파값 설정.
	//		D3D11_BLEND_OP BlendOpAlpha;			알파블렌딩 공식의 옵션 항목에 더하기 적용. 알파값 계산에만 적용된다.
	//		UINT8 RenderTargetWriteMask;			블렌딩을 적용할 색상. RGBA중 몇가지만 선택해서 알파블렌딩을 적용할 수도 있다.
	//	} 	D3D11_RENDER_TARGET_BLEND_DESC;
	//
	//} 	D3D11_BLEND_DESC;

	D3D11_BLEND_DESC blendDesc = { 0 };



	blendDesc.AlphaToCoverageEnable = false;		//알파투커버리지 비적용.
	blendDesc.IndependentBlendEnable = false;		//0번 렌더타겟의 알파블렌딩 설정값을 모든 렌더타겟에 적용.
	blendDesc.RenderTarget[0].BlendEnable = true;	//알파블렌딩 함.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//D3D11_COLOR_WRITE_ENABLE_ALL: RGBA 모든 색상에 블렌딩 적용.

	//알파블렌딩 공식.
	//output: 최종 결과물.
	//dest: 백버퍼의 현재 색상.
	//src: 백버퍼의 색상과 블렌드할 원본의 색상.
	//옵션: BlendOp으로 지정하는 블렌드 연산 방식.
	//outputColor = (srcColor * srcFactor) 옵션 (destColor * destFactor)

	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//옵션 연산을 더하기로 한다.

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//srcFactor에 srcColor의 알파값을 일괄 대입.

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//destFactor에 (1 - srcColor의 알파값)을 일괄 대입.

	//알파쪽은 따로 처리.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	//이 세 줄만 주석처리해 볼 것.->하면 블렌드 스테이트 생성 실패.

	GameEngineBlend::Create("AlphaBlend", blendDesc);





	D3D11_BLEND_DESC transparentBlendDesc = { 0 };

	transparentBlendDesc.AlphaToCoverageEnable = false;		//알파투커버리지 비적용.
	transparentBlendDesc.IndependentBlendEnable = false;		//0번 렌더타겟의 알파블렌딩 설정값을 모든 렌더타겟에 적용.
	transparentBlendDesc.RenderTarget[0].BlendEnable = true;	//알파블렌딩 함.
	transparentBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//D3D11_COLOR_WRITE_ENABLE_ALL: RGBA 모든 색상에 블렌딩 적용.

	transparentBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//??

	transparentBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//??

	transparentBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	//??

	//알파쪽은 따로 처리.
	transparentBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	transparentBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	GameEngineBlend::Create("TransparentBlend", transparentBlendDesc);







	//typedef struct D3D11_DEPTH_STENCIL_DESC
	//{
	//	BOOL DepthEnable;						깊이테스트 활성화 여부.
	//	D3D11_DEPTH_WRITE_MASK DepthWriteMask;	깊이테스트 결과를 기존 깊이 데이터와 비교할 지 여부??
	//	D3D11_COMPARISON_FUNC DepthFunc;		깊이테스트 방식.
	//	BOOL StencilEnable;						스텐실테스트를 활성화 여부.
	//	UINT8 StencilReadMask;					???
	//	UINT8 StencilWriteMask;					???
	//	D3D11_DEPTH_STENCILOP_DESC FrontFace;	???
	//	D3D11_DEPTH_STENCILOP_DESC BackFace;	???
	//} 	D3D11_DEPTH_STENCIL_DESC;

	D3D11_DEPTH_STENCIL_DESC engineBaseDepthStencilDesc = { 0 };

	engineBaseDepthStencilDesc.DepthEnable = true;
	//true: 깊이테스트 함.

	engineBaseDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL: 기존 깊이데이터와 비교 함.
	//이걸 제로로 바꾸면 왜 깊이테스트가 제대로 안 되는 걸까??

	engineBaseDepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	//D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS: 비교했을때 값이 작은것을 통과시킴.

	engineBaseDepthStencilDesc.StencilEnable = false;
	//false: 스텐실 테스트 안함.

	GameEngineDepthStencil::Create("EngineBaseDepth", engineBaseDepthStencilDesc);




	D3D11_DEPTH_STENCIL_DESC alwaysDepthStencilDesc = { 0 };

	alwaysDepthStencilDesc.DepthEnable = true;
	//true: 깊이테스트 함.

	alwaysDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL: 기존 깊이데이터와 비교 함.
	//이걸 제로로 바꾸면 왜 깊이테스트가 제대로 안 되는 걸까??

	alwaysDepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	//D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS: 항상 통과시킴.

	alwaysDepthStencilDesc.StencilEnable = false;
	//false: 스텐실 테스트 안함.

	GameEngineDepthStencil::Create("AlwaysDepth", alwaysDepthStencilDesc);
}

void EngineTextureLoad()
{

	//typedef struct D3D11_SAMPLER_DESC
	//	D3D11_FILTER Filter;					텍스쳐를 샘플링할때 적용할 필터.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressU;	UV좌표 0~1범위를 가로로 벗어난 메쉬에 대한 처리 방법.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressV;	UV좌표 0~1범위를 세로로 벗어난 메쉬에 대한 처리 방법.
	//	D3D11_TEXTURE_ADDRESS_MODE AddressW;	??
	//	FLOAT MipLODBias;						밉맵 시작 오프셋??
	// 
	//	UINT MaxAnisotropy;						비등방성 필터링 최대값. 1~16 범위로 가능. 1은 비등방성 필터링 사용 안함.
	//최대값인 16으로 하면 1/16로 축소된 밉맵 이미지까지 만들어서 사용한다는 건가??
	//Filter가 D3D11_FILTER_ANISOTROPIC, D3D11_FILTER_COMPARISON_ANISOTROPIC일때만 적용.
	// 
	//	D3D11_COMPARISON_FUNC ComparisonFunc;	샘플링 데이터를 기존 샘플링 데이터와 비교할 때 적용되는 옵션??
	//필터 옵션이 D3D11_FILTER_COMPARISON_~ 으로 시작할 때만 적용.
	// 
	//	FLOAT BorderColor[4];					rgba 0~1 범위내에서 경계선 색상을 정한다.
	//AddressU, AddressV, AddressW 중 D3D11_TEXTURE_ADDRESS_BORDER를 선택한 곳에만 적용된다.
	// 
	//	FLOAT MinLOD;							밉맵 레벨 범위 최저 제한. 0이면 가장 크고 상세한 밉맵 레벨.
	//	FLOAT MaxLOD;							밉맵 레벨 범위 최고 제한. 0이면 가장 크고 상세한 밉맵 레벨. 반드시 MinLOD보다 크거나 같은 값이어야 한다.
	//											
	//} 	D3D11_SAMPLER_DESC;


	//밉맵: 원본 텍스처의 해상도를 줄인 작은 버전 여럿을 미리 만들어 저장해두고 필요할 때 꺼내다 쓰는 것. 
	// 커다란 원본 텍스처만 사용한다면 카메라에서 멀리 있는 텍스처의 경우 텍스처의 정보를 띄엄띄엄 읽게 되어서 보기에도 좋지 않고 
	// 메모리 캐시의 효율도 급격하게 떨어진다. 따라서 텍스처 단계에서 다운스케일링을 통한 안티앨리어싱을 해줄 필요가 있는데, 
	// 이것을 미리 해두는 것이 밉매핑, 밉매핑을 위한 텍스처 모음이 밉맵이다.

	D3D11_SAMPLER_DESC pointSamplerDesc = { };

	pointSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT: 
	// 축소(Minification), 확대(Magnification), 밉 레벨 샘플링을 전부 포인터 샘플링으로 한다.
	//포토샵 확대/축소의 최단입점(Nearest Neighbor. 주변 색을 섞지 않고 그 중 하나만 골라서 씀) 옵션과 같은 기능.

	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR:
	//축소(Minification), 확대(Magnification)는 포인터 샘플링으로, 밉 레벨 샘플링은 선형보간한다.

	//D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR:
	//축소(Minification), 확대(Magnification), 밉 레벨 샘플링을 전부 선형보간 한다. 

	//D3D11_FILTER::D3D11_FILTER_ANISOTROPIC:
	// 비등방성 필터링: 상하좌우를 동일한 비율로 확대 축소한 밉맵 이미지를 사용하는 것이 아니라, 
	// 상하나 좌우 비율을 다른 비율로 확대 축소한 밉맵이미지를 사용해서 카메라가 다양한 위치로 움직여도 그에 맞는 적절한 밉맵을 사용하는 기법.

	//이 외에도 많은 필터링 옵션들이 있다. 


	//typedef
	//enum D3D11_TEXTURE_ADDRESS_MODE			UV좌표범위 0~1을 벗어난 좌표의 메쉬 공간을 덮는 방식.
	//{
	//	D3D11_TEXTURE_ADDRESS_WRAP = 1,			텍스쳐를 그대로 복사해서 그린다.	
	//	D3D11_TEXTURE_ADDRESS_MIRROR = 2,		좌우/상하 뒤집어서 그린다.
	//	D3D11_TEXTURE_ADDRESS_CLAMP = 3,		그리지 않는다.
	//	D3D11_TEXTURE_ADDRESS_BORDER = 4,		텍스쳐 해당 방향 경계선의 색을 늘려 그린다
	//	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE = 5	좌우/상하를 뒤집어서 한번만 그린다.
	//} 	D3D11_TEXTURE_ADDRESS_MODE;

	pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	//D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP가 기본인 이유: 
	// 실수 오차로 UV값이 1을 초과해도 더 그리지 않고 무시해 버리므로 가장 깔끔하게 나온다.


	pointSamplerDesc.MipLODBias = 0.f;
	pointSamplerDesc.MaxAnisotropy = 1;	//1: 비등방성 밉맵 생성 안함.

	//	typedef 
	//	enum D3D11_COMPARISON_FUNC
	//	{
	//		D3D11_COMPARISON_NEVER = 1,			아무것도 통과시키지 않음.
	//		D3D11_COMPARISON_LESS = 2,			값이 작은 것만 통과.
	//		D3D11_COMPARISON_EQUAL = 3,			값이 같은 것만 통과.
	//		D3D11_COMPARISON_LESS_EQUAL = 4,	값이 작거나 같은 것만 통과.
	//		D3D11_COMPARISON_GREATER = 5,		값이 큰 것만 통과.
	//		D3D11_COMPARISON_NOT_EQUAL = 6,		값이 같지 않은 것만 통과.
	//		D3D11_COMPARISON_GREATER_EQUAL = 7,	값이 크거나 같은 것만 통과.
	//		D3D11_COMPARISON_ALWAYS = 8			언제나 통과.
	//	} 	D3D11_COMPARISON_FUNC;	

	pointSamplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	//애초에 필터 옵션을 비교하지 않는 옵션으로 했기 때문에 뭘 골라도 어차피 적용되지 않는다.

	pointSamplerDesc.MinLOD = -FLT_MAX;
	pointSamplerDesc.MaxLOD = FLT_MAX;

	GameEngineSampler::Create("EngineSampler_Point", pointSamplerDesc);


	D3D11_SAMPLER_DESC linearSamplerDesc = {};
	linearSamplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	linearSamplerDesc.MipLODBias = 0.f;
	linearSamplerDesc.MaxAnisotropy = 1;
	linearSamplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	linearSamplerDesc.MinLOD = -FLT_MAX;
	linearSamplerDesc.MaxLOD = FLT_MAX;
	GameEngineSampler::Create("EngineSampler_Linear", linearSamplerDesc);

	//D3D11 ERROR: ID3D11DeviceContext::DrawIndexed: 
	//The Pixel Shader unit expects a Sampler configured for default filtering to be set at Slot 0, 
	//but the sampler bound at this slot is configured for comparison filtering.  
	//This mismatch will produce undefined behavior if the sampler is used (e.g. it is not skipped due to shader code branching).
	//[ EXECUTION ERROR #390: DEVICE_DRAW_SAMPLER_MISMATCH]
	//->샘플러 필터 옵션은 D3D11_FILTER::D3D11_FILTER_COMPARISON_~으로 세팅했지만, 
	// HLSL코드상으로는 저 필터 옵션을 활용하지 않았을 때 나오는 경고라고 한다..


	GameEngineDirectory currentDir;

	currentDir.MoveParentToExistChildDirectory("GameEngineResource");
	currentDir.MoveToChild("GameEngineResource");
	currentDir.MoveToChild("Texture");

	std::vector<GameEngineFile> textures = currentDir.GetAllFiles();

	for (size_t i = 0; i < textures.size(); i++)
	{
		GameEngineTexture::Load(textures[i].GetFullPath());
	}
}

void EngineRenderingPipeLine()
{

	GameEngineRenderingPipeLine* newRenderingPipeLine1 
		= GameEngineRenderingPipeLine::Create("Color");
	newRenderingPipeLine1->SetVertexBuffer_InputAssembler1("RectVertex");
	newRenderingPipeLine1->SetVertexShader("Color.hlsl");
	newRenderingPipeLine1->SetIndexBuffer_InputAssembler2("RectIndex");
	newRenderingPipeLine1->SetPixelShader("Color.hlsl");
	newRenderingPipeLine1->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine1->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine1->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineRenderingPipeLine* newRenderingPipeLine2 
		= GameEngineRenderingPipeLine::Create("Texture");
	newRenderingPipeLine2->SetVertexBuffer_InputAssembler1("RectVertex");
	newRenderingPipeLine2->SetVertexShader("Texture.hlsl");
	newRenderingPipeLine2->SetIndexBuffer_InputAssembler2("RectIndex");
	newRenderingPipeLine2->SetPixelShader("Texture.hlsl");
	newRenderingPipeLine2->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine2->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine2->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineRenderingPipeLine* newRenderingPipeLine3 
		= GameEngineRenderingPipeLine::Create("TextureAtlas");
	newRenderingPipeLine3->SetVertexBuffer_InputAssembler1("RectVertex");
	newRenderingPipeLine3->SetVertexShader("TextureAtlas.hlsl");
	newRenderingPipeLine3->SetIndexBuffer_InputAssembler2("RectIndex");
	newRenderingPipeLine3->SetPixelShader("TextureAtlas.hlsl");
	newRenderingPipeLine3->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine3->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine3->SetDepthStencil_OutputMerger("EngineBaseDepth");


	GameEngineRenderingPipeLine* newRenderingPipeLine4 
		= GameEngineRenderingPipeLine::Create("3DDebug");
	newRenderingPipeLine4->SetVertexBuffer_InputAssembler1("BoxVertex");
	newRenderingPipeLine4->SetVertexShader("Debug3D.hlsl");
	newRenderingPipeLine4->SetIndexBuffer_InputAssembler2("BoxIndex");
	newRenderingPipeLine4->SetPixelShader("Debug3D.hlsl");
	newRenderingPipeLine4->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine4->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine4->SetDepthStencil_OutputMerger("AlwaysDepth"); 
	
	
	GameEngineRenderingPipeLine* newRenderingPipeLine5 
		= GameEngineRenderingPipeLine::Create("DebugTexture");
	newRenderingPipeLine5->SetVertexBuffer_InputAssembler1("RectVertex");
	newRenderingPipeLine5->SetVertexShader("DebugTexture.hlsl");
	newRenderingPipeLine5->SetIndexBuffer_InputAssembler2("RectIndex");
	newRenderingPipeLine5->SetPixelShader("DebugTexture.hlsl");
	newRenderingPipeLine5->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine5->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine5->SetDepthStencil_OutputMerger("EngineBaseDepth");	
	
	
	
	GameEngineRenderingPipeLine* newRenderingPipeLine6 
		= GameEngineRenderingPipeLine::Create("TargetMerge");
	newRenderingPipeLine6->SetVertexBuffer_InputAssembler1("FullrectVertex");
	newRenderingPipeLine6->SetVertexShader("TargetMerge.hlsl");
	newRenderingPipeLine6->SetIndexBuffer_InputAssembler2("FullrectIndex");
	newRenderingPipeLine6->SetPixelShader("TargetMerge.hlsl");
	newRenderingPipeLine6->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine6->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine6->SetDepthStencil_OutputMerger("AlwaysDepth");



	GameEngineRenderingPipeLine* newRenderingPipeLine7
		= GameEngineRenderingPipeLine::Create("Blur");
	newRenderingPipeLine7->SetVertexBuffer_InputAssembler1("FullrectVertex");
	newRenderingPipeLine7->SetVertexShader("Blur.hlsl");
	newRenderingPipeLine7->SetIndexBuffer_InputAssembler2("FullrectIndex");
	newRenderingPipeLine7->SetPixelShader("Blur.hlsl");
	newRenderingPipeLine7->SetRasterizer("EngineRasterizer");
	newRenderingPipeLine7->SetBlend_OutputMerger("AlphaBlend");
	newRenderingPipeLine7->SetDepthStencil_OutputMerger("EngineBaseDepth");

}

void EngineMesh()
{
	//기본 사각형.
	std::vector<GameEngineVertex> rectVertex;
	rectVertex.reserve(4);
	rectVertex.push_back({ float4(-0.5f, 0.5f), float4(), float4(0, 0) });	//0번 점.
	rectVertex.push_back({ float4(0.5f, 0.5f), float4(), float4(1, 0) });	//1번 점.
	rectVertex.push_back({ float4(0.5f, -0.5f), float4(), float4(1, 1) });	//2번 점.
	rectVertex.push_back({ float4(-0.5f, -0.5f), float4(), float4(0, 1) });	//3번 점.
	GameEngineVertexBuffer::Create("RectVertex", rectVertex);

	std::vector<int> rectIndex;
	rectIndex.reserve(6);
	//1번 삼각형.
	rectIndex.push_back(0);
	rectIndex.push_back(1);
	rectIndex.push_back(2);
	//2번 삼각형.
	rectIndex.push_back(0);
	rectIndex.push_back(2);
	rectIndex.push_back(3);
	GameEngineIndexBuffer::Create("RectIndex", rectIndex);




	//윈도우 전체 크기 사각형.
	std::vector<GameEngineVertex> fullrectVertex;
	fullrectVertex.reserve(4);
	fullrectVertex.push_back({ float4(-1.f, 1.f), float4(), float4(0, 0) });	//0번 점.
	fullrectVertex.push_back({ float4(1.f, 1.f), float4(), float4(1, 0) });		//1번 점.
	fullrectVertex.push_back({ float4(1.f, -1.f), float4(), float4(1, 1) });	//2번 점.
	fullrectVertex.push_back({ float4(-1.f, -1.f), float4(), float4(0, 1) });	//3번 점.
	GameEngineVertexBuffer::Create("FullrectVertex", fullrectVertex);

	std::vector<int> fullrectIndex;
	fullrectIndex.reserve(6);
	//1번 삼각형.
	fullrectIndex.push_back(0);
	fullrectIndex.push_back(1);
	fullrectIndex.push_back(2);
	//2번 삼각형.
	fullrectIndex.push_back(0);
	fullrectIndex.push_back(2);
	fullrectIndex.push_back(3);
	GameEngineIndexBuffer::Create("FullrectIndex", fullrectIndex);



	//기본 육면체.
	std::vector<GameEngineVertex> boxVertex;
	boxVertex.reserve(8);
	boxVertex.push_back({ float4(-0.5f, 0.5f, -0.5f), float4(), float4(0, 0) });	//0번 점.
	boxVertex.push_back({ float4(0.5f, 0.5f, -0.5f), float4(), float4(1, 0) });		//1번 점.
	boxVertex.push_back({ float4(0.5f, -0.5f, -0.5f), float4(), float4(1, 1) });	//2번 점.
	boxVertex.push_back({ float4(-0.5f, -0.5f, -0.5f), float4(), float4(0, 1) });	//3번 점.

	boxVertex.push_back({ float4(0.5f, 0.5f, 0.5f), float4(), float4(0, 1) });		//4번 점.
	boxVertex.push_back({ float4(-0.5f, 0.5f, 0.5f), float4(), float4(1, 1) });		//5번 점.
	boxVertex.push_back({ float4(-0.5f, -0.5f, 0.5f), float4(), float4(1, 0) });	//6번 점.
	boxVertex.push_back({ float4(0.5f, -0.5f, 0.5f), float4(), float4(0, 0) });		//7번 점.
	GameEngineVertexBuffer::Create("BoxVertex", boxVertex);

	std::vector<int> boxIndex;
	rectIndex.reserve(36);

	//정면 1번 삼각형.
	boxIndex.push_back(0);
	boxIndex.push_back(1);
	boxIndex.push_back(2);
	//정면 2번 삼각형.
	boxIndex.push_back(0);
	boxIndex.push_back(2);
	boxIndex.push_back(3);
	//후면 1번 삼각형.
	boxIndex.push_back(4);
	boxIndex.push_back(5);
	boxIndex.push_back(6);
	//후면 2번 삼각형.
	boxIndex.push_back(4);
	boxIndex.push_back(6);
	boxIndex.push_back(7);

	//좌면 1번 삼각형.
	boxIndex.push_back(5);
	boxIndex.push_back(0);
	boxIndex.push_back(3);
	//좌면 2번 삼각형.
	boxIndex.push_back(5);
	boxIndex.push_back(3);
	boxIndex.push_back(6);
	//우면 1번 삼각형.
	boxIndex.push_back(1);
	boxIndex.push_back(4);
	boxIndex.push_back(7);
	//우면 2번 삼각형.
	boxIndex.push_back(1);
	boxIndex.push_back(7);
	boxIndex.push_back(2);

	//상면 1번 삼각형.
	boxIndex.push_back(5);
	boxIndex.push_back(4);
	boxIndex.push_back(1);
	//상면 2번 삼각형.
	boxIndex.push_back(5);
	boxIndex.push_back(1);
	boxIndex.push_back(0);
	//하면 1번 삼각형.
	boxIndex.push_back(2);
	boxIndex.push_back(3);
	boxIndex.push_back(6);
	//하면 2번 삼각형.
	boxIndex.push_back(2);
	boxIndex.push_back(6);
	boxIndex.push_back(7);

	GameEngineIndexBuffer::Create("BoxIndex", boxIndex);






	//GameEngineFont::Load("돋움");
}

void ShaderCompile()
{
	GameEngineDirectory engineResourceDir;

	engineResourceDir.MoveParentToExistChildDirectory("GameEngineResource");
	engineResourceDir.MoveToChild("GameEngineResource");
	engineResourceDir.MoveToChild("Shader");

	std::vector<GameEngineFile> shaders = engineResourceDir.GetAllFiles("hlsl");

	for (size_t i = 0; i < shaders.size(); i++)
	{
		GameEngineShader::AutoCompile(shaders[i].GetFullPath());
	}

}

void GameEngineCore::EngineResourceInitialize()
{
	EngineTextureLoad();	//샘플러를 생성하고 텍스처를 불러오는 함수.
	EngineInputLayout();	//엔진 기본제공 인풋 레이아웃을 저장하는 함수.
	EngineMesh();			//엔진 기본제공 사각형과 육면체 메쉬를 생성하는 함수.
	EngineSubSetting();		//엔진 기본제공 래스터라이저, 블렌드, 깊이스텐실을 생성하는 함수.
	ShaderCompile();		//엔진 기본제공 HLSL코드를 컴파일해서 셰이더와 셰이더리소스세터를 생성하고, 
	//셰이더리소스세터에 리소스를 연결하는 함수.

	EngineRenderingPipeLine();	//엔진 기본제공 렌더링 파이프라인들을 생성하는 함수.
}

void GameEngineCore::EngineResourceDestroy()
{
	//사각형, 육면체, 에러텍스쳐 등등, 엔진 수준에서 기본적으로 지원되어야 하는 리소스를 삭제하는 함수.
	GameEngineRenderingPipeLine::ResourceDestroy();

	GameEngineInputLayout::ResourceDestroy();
	GameEngineVertexBuffer::ResourceDestroy();
	GameEngineVertexShader::ResourceDestroy();
	GameEngineIndexBuffer::ResourceDestroy();
	GameEngineRasterizer::ResourceDestroy();
	GameEnginePixelShader::ResourceDestroy();
	GameEngineDepthStencil::ResourceDestroy();
	GameEngineBlend::ResourceDestroy();

	GameEngineConstantBuffer::ResourceDestroy();

	GameEngineRenderTarget::ResourceDestroy();
	GameEngineTexture::ResourceDestroy();
	GameEngineFolderTexture::ResourceDestroy();
	GameEngineSampler::ResourceDestroy();
	GameEngineSound::ResourceDestroy();

	GameEngineDevice::Destroy();
	//모든 리소스들은 다이렉트X 디바이스의 지원이 있어야 존재할 수 있으므로,
	// 리소스를 전부 정리한 후에 디바이스를 정리한다.
}
