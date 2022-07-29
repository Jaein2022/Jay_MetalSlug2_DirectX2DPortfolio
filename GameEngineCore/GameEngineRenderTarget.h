#pragma once
#include "GameEngineRes.h"

class GameEngineDepthStencilTexture;
class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineRes<GameEngineRenderTarget>
{
	//����Ÿ�ٺ並 ���������� �����ϴ� Ŭ����.
	//����Ÿ��: ������ ������������ ���� ��ģ ������μ��� ���� 2���� �迭�� �޴µ� ���Ǵ� �׷��� ī�� �� �޸� ����.
	//GameEngineTexture Ŭ���� ���·� ����� �ؽ��ĵ�� ������ �ؽ��ĵ鿡 ������ ����Ÿ�ٺ���� �� Ŭ������ �����Ѵ�. 

	friend GameEngineRes<GameEngineRenderTarget>;
	//GameEngineRenderTargetŬ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.


	//���ӿ�������Ÿ�ٰ� ���ӿ����ؽ��� Ŭ������ �и��� ����: 
	//���ӿ����ؽ��Ĵ� �ڱⰡ ���� �ؽ��Ŀ� �� ���긮�ҽ��鸸 ������ �����ϴ� Ŭ�����̰�,
	//���ӿ�������Ÿ���� �� �����ӿ�ũ�� ��� ����Ÿ�ٺ並 �����ϴ� Ŭ�����̴�. 

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

	void CreateRenderTarget(	//����Ÿ������ �� ���ӿ����ؽ�ó ��ü�� ����, �����ϴ� �Լ�.
		ID3D11Texture2D* _texture,
		const float4& _clearColor
	);
	void CreateDepthTexture(int _index);
	void Clear();	//��ü�� ������� ����Ÿ�ٺ並 �� �������� ��� �ʱ�ȭ�ϴ� �Լ�.
	void Setting();	//�ش� ���ҽ��� ������ ���������ο� �����ϴ� �Լ�.

private:
	std::vector<GameEngineTexture*> renderTargets_;
	//����Ÿ������ ����� �ؽ��ĵ�.

	std::vector<ID3D11RenderTargetView*> renderTargetViews_;
	//�� �ؽ��Ŀ��� �Ļ��� ����Ÿ�ٺ��.

	std::vector<float4> clearColors_;
	//����Ÿ�ٺ並 �ʱ�ȭ�Ҷ� �� ���󰪵�.

	//���Ϳ� �־��ٰ� �ݵ�� ������ ����Ѵٴ� ������ ����.

	ID3D11DepthStencilView* depthStencilView_;
	//

	GameEngineDepthStencilTexture* depthTexture_;
	//

};