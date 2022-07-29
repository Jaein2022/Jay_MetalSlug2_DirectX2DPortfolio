#pragma once
#include "GameEngineRes.h"

class GameEngineTexture;
class GameEngineDepthStencilTexture : public GameEngineRes<GameEngineDepthStencilTexture>
{
	//

	friend GameEngineRes<GameEngineDepthStencilTexture>;
	//GameEngineDepthStencilTexture Ŭ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.


private:
	GameEngineDepthStencilTexture();
	~GameEngineDepthStencilTexture();
	//�ܺο��� ���ڴ�� ���ҽ��� ����/�����ϴ°� ���� ���ؼ� ������/�Ҹ��ڸ� �����̺����� �����ؼ� �ܺ� ������ ���´�.
	//�� �����ӿ�ũ�� ���ҽ��� �ݵ�� �Ҹ��ڰ� �ƴ϶� ResourceDestroy()�Լ����� �����ؾ� �Ѵ�.
	//���α׷� ���������� ���ҽ������� ���ϸ� ������ �������� �׷��� �ִ��� ���Ƶд�.

	GameEngineDepthStencilTexture(const GameEngineDepthStencilTexture& _other) = delete;
	GameEngineDepthStencilTexture(GameEngineDepthStencilTexture&& _other) noexcept = delete;
	GameEngineDepthStencilTexture& operator=(const GameEngineDepthStencilTexture& _other) = delete;
	GameEngineDepthStencilTexture& operator=(const GameEngineDepthStencilTexture&& _other) = delete;


public:
	static GameEngineDepthStencilTexture* Create(const float4& _scale);
	ID3D11DepthStencilView* CreateDepthStencilView();


private:
	void CreateDepthTexture(const float4& _scale);

private:
	GameEngineTexture* depthBuffer_;	//���̵����͸� ������ �ؽ�ó.

};

