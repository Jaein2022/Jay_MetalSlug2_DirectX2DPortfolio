#pragma once
#include "GameEngineRes.h"
#include "GameEngineShaderResourceHelper.h"

class GameEnginePostEffect
{
	bool isUpdate_;
public:
	virtual ~GameEnginePostEffect(){};
	virtual void EffectInit() = 0;
	virtual void Effect(class GameEngineRenderTarget* _renderTarget) = 0;

public:
	inline bool IsUpdate()
	{
		return isUpdate_;
	}

	virtual void On()
	{
		isUpdate_ = true;
	}

	virtual void Off()
	{
		isUpdate_ = false;
	}
};

class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineRes<GameEngineRenderTarget>
{
	//렌더타겟뷰를 전문적으로 관리하는 클래스.
	//렌더타겟: 렌더링 파이프라인을 전부 거친 결과물로서의 색상값 2차원 배열을 받는데 사용되는 그래픽 카드 내 메모리 영역.
	//GameEngineTexture 클래스 형태로 저장된 텍스쳐들과 각각의 텍스쳐들에 연동된 렌더타겟뷰들을 이 클래스로 통제한다. 

	friend GameEngineRes<GameEngineRenderTarget>;
	//GameEngineRenderTarget클래스의 프라이빗 소멸자를 GameEngineRes클래스에서 호출하기 위한 방법.

	friend class GameEngineStatusWindow;
	//프렌드 이유??

	//게임엔진렌더타겟과 게임엔진텍스쳐 클래스를 분리한 이유: 
	//게임엔진텍스쳐는 자기가 가진 텍스쳐와 그 서브리소스들만 가지고 관리하는 클래스이고,
	//게임엔진렌더타겟은 이 프레임워크내 모든 렌더타겟뷰를 관리하는 클래스이다. 


	static ID3D11RenderTargetView* prevRenderTargetView_;
	static ID3D11DepthStencilView* prevDepthStencilView_;

private:
	GameEngineRenderTarget();
	~GameEngineRenderTarget();
	//외부에서 제멋대로 리소스를 생성/삭제하는걸 막기 위해서 생성자/소멸자를 프라이빗으로 지정해서 외부 접근을 막는다.
	//이 프레임워크의 리소스는 반드시 소멸자가 아니라 ResourceDestroy()함수에서 제거해야 한다.
	//프로그램 끝날때까지 리소스삭제를 안하면 끝나는 문제지만 그래도 최대한 막아둔다.

	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete;

public:
	static GameEngineRenderTarget* Create(const std::string& _name);
	static GameEngineRenderTarget* Create();

	void CreateRenderTargetTexture(	//렌더타겟으로 쓸 게임엔진텍스처 객체를 생성, 저장하는 함수.
		ID3D11Texture2D* _texture,
		const float4& _clearColor);
	void CreateRenderTargetTexture(
		const float4& _size,
		const float4& _color);
	void CreateRenderTargetTexture(
		const float4& _size,
		DXGI_FORMAT _format,
		const float4& _color);
	void CreateRenderTargetTexture(
		D3D11_TEXTURE2D_DESC _desc,
		const float4& _color);
	void CreateRenderTargetTexture(
		GameEngineTexture* _texture,
		const float4& _color);
	GameEngineTexture* GetRenderTargetTexture(size_t _index);

	void CreateDepthTexture(int _index);
	void SetDepthTexture(GameEngineTexture* _depthTexture);

	//백버퍼 렌더타겟뷰를 한 색상으로 덮어서 초기화하는 함수.
	void Clear();	

	//이 렌더타겟뷰를 디바이스 컨텍스트에 연결하는 함수.
	void Setting();

	void Copy(GameEngineRenderTarget* _otherRenderTarget, int _index = 0);
	void Merge(GameEngineRenderTarget* _otherRenderTarget, int _index = 0);
	void Effect(	//기존 렌더타겟에, 다른 렌더타겟에 있던 픽셀정보들을 복사하는 함수. 
		GameEngineRenderingPipeLine* _otherPipeLine,
		GameEngineShaderResourceHelper* _shaderResourceHelper);
	void EffectProcess();

	static void GetPrevRenderTarget();
	static void SetPrevRenderTarget();

public:
	GameEngineTexture* GetDepthTexture()
	{
		return depthTexture_;
	}

	template<typename EffectType>
	EffectType* AddEffect()
	{
		GameEnginePostEffect* newEffect = new EffectType();
		newEffect->EffectInit();
		allEffects_.push_back(newEffect);
		return reinterpret_cast<EffectType*>(newEffect);
	}

private:
	std::vector<GameEngineTexture*> renderTargets_;
	//렌더타겟으로 사용할 텍스쳐들.

	std::vector<ID3D11RenderTargetView*> renderTargetViews_;
	//위 텍스쳐에서 파생된 렌더타겟뷰들.

	std::vector<ID3D11ShaderResourceView*> shaderResourceViews_;
	//위 텍스쳐에서 파생된 셰이더리소스뷰들

	std::vector<float4> clearColors_;
	//렌더타겟뷰를 초기화할때 쓸 색상값들.

	//벡터에 넣었다고 반드시 여러개 사용한다는 보장은 없음.

	ID3D11DepthStencilView* depthStencilView_;
	//

	GameEngineTexture* depthTexture_;

	GameEngineShaderResourceHelper mergeShaderResourceHelper_;
	GameEngineRenderingPipeLine* mergePipeLine_;

	std::list<GameEnginePostEffect*> allEffects_;

};