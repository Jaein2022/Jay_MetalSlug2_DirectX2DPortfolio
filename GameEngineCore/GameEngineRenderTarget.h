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
	//����Ÿ�ٺ並 ���������� �����ϴ� Ŭ����.
	//����Ÿ��: ������ ������������ ���� ��ģ ������μ��� ���� 2���� �迭�� �޴µ� ���Ǵ� �׷��� ī�� �� �޸� ����.
	//GameEngineTexture Ŭ���� ���·� ����� �ؽ��ĵ�� ������ �ؽ��ĵ鿡 ������ ����Ÿ�ٺ���� �� Ŭ������ �����Ѵ�. 

	friend GameEngineRes<GameEngineRenderTarget>;
	//GameEngineRenderTargetŬ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.

	friend class GameEngineStatusWindow;
	//������ ����??

	//���ӿ�������Ÿ�ٰ� ���ӿ����ؽ��� Ŭ������ �и��� ����: 
	//���ӿ����ؽ��Ĵ� �ڱⰡ ���� �ؽ��Ŀ� �� ���긮�ҽ��鸸 ������ �����ϴ� Ŭ�����̰�,
	//���ӿ�������Ÿ���� �� �����ӿ�ũ�� ��� ����Ÿ�ٺ並 �����ϴ� Ŭ�����̴�. 


	static ID3D11RenderTargetView* prevRenderTargetView_;
	static ID3D11DepthStencilView* prevDepthStencilView_;

private:
	GameEngineRenderTarget();
	~GameEngineRenderTarget();
	//�ܺο��� ���ڴ�� ���ҽ��� ����/�����ϴ°� ���� ���ؼ� ������/�Ҹ��ڸ� �����̺����� �����ؼ� �ܺ� ������ ���´�.
	//�� �����ӿ�ũ�� ���ҽ��� �ݵ�� �Ҹ��ڰ� �ƴ϶� ResourceDestroy()�Լ����� �����ؾ� �Ѵ�.
	//���α׷� ���������� ���ҽ������� ���ϸ� ������ �������� �׷��� �ִ��� ���Ƶд�.

	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete;
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete;

public:
	static GameEngineRenderTarget* Create(const std::string& _name);
	static GameEngineRenderTarget* Create();

	void CreateRenderTargetTexture(	//����Ÿ������ �� ���ӿ����ؽ�ó ��ü�� ����, �����ϴ� �Լ�.
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

	//����� ����Ÿ�ٺ並 �� �������� ��� �ʱ�ȭ�ϴ� �Լ�.
	void Clear();	

	//�� ����Ÿ�ٺ並 ����̽� ���ؽ�Ʈ�� �����ϴ� �Լ�.
	void Setting();

	void Copy(GameEngineRenderTarget* _otherRenderTarget, int _index = 0);
	void Merge(GameEngineRenderTarget* _otherRenderTarget, int _index = 0);
	void Effect(	//���� ����Ÿ�ٿ�, �ٸ� ����Ÿ�ٿ� �ִ� �ȼ��������� �����ϴ� �Լ�. 
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
	//����Ÿ������ ����� �ؽ��ĵ�.

	std::vector<ID3D11RenderTargetView*> renderTargetViews_;
	//�� �ؽ��Ŀ��� �Ļ��� ����Ÿ�ٺ��.

	std::vector<ID3D11ShaderResourceView*> shaderResourceViews_;
	//�� �ؽ��Ŀ��� �Ļ��� ���̴����ҽ����

	std::vector<float4> clearColors_;
	//����Ÿ�ٺ並 �ʱ�ȭ�Ҷ� �� ���󰪵�.

	//���Ϳ� �־��ٰ� �ݵ�� ������ ����Ѵٴ� ������ ����.

	ID3D11DepthStencilView* depthStencilView_;
	//

	GameEngineTexture* depthTexture_;

	GameEngineShaderResourceHelper mergeShaderResourceHelper_;
	GameEngineRenderingPipeLine* mergePipeLine_;

	std::list<GameEnginePostEffect*> allEffects_;

};