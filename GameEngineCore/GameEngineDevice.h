#pragma once

class GameEngineRenderTarget;
class GameEngineDevice
{
	//그래픽카드의 메모리에 접근해 각종 리소스를 생성하는데 필요한 ID3D11Device, 
	// 생성한 각종 리소스들로 렌더링 작업을 수행할 ID3D11DeviceContext,
	// 렌더링파이프라인을 거친 백버퍼 렌더타겟뷰를 더블버퍼링해서 화면에 띄우는데 필요한 IDXGISwapChain 객체들을 
	// 생성, 관리하는 클래스.

	//여기저기 쓸 곳이 많으므로 변수, 함수 전부 스태틱으로 한다.

	friend class GameEngineCore;

private:
	GameEngineDevice() {};
	~GameEngineDevice() {};

	GameEngineDevice(const GameEngineDevice& _other) = delete;
	GameEngineDevice(GameEngineDevice&& _other) noexcept = delete;
	GameEngineDevice& operator=(const GameEngineDevice& _other) = delete;
	GameEngineDevice& operator=(const GameEngineDevice&& _other) = delete;


public:
	//디바이스 생성.
	static void Initialize();	

	//한 싸이클이 끝나고 백버퍼 렌더타겟을 리셋하는 함수.
	static void RenderStart();	

	//모든 렌더링 과정을 거친 백버퍼를 전면 버퍼와 교체하는 함수.
	static void RenderEnd();	

public:

	static void Destroy()
	{
		if (nullptr != swapChain_)
		{
			swapChain_->Release();
			swapChain_ = nullptr;
		}
		if (nullptr != deviceContext_)
		{
			deviceContext_->Release();
			deviceContext_ = nullptr;
		}
		if (nullptr != device_)
		{
			device_->Release();
			device_ = nullptr;
		}
	}

	static ID3D11Device* GetDevice()
	{
		return device_;
	}

	static ID3D11DeviceContext* GetContext()
	{
		return deviceContext_;
	}

	static GameEngineRenderTarget* GetBackBuffer()
	{
		return backBufferRenderTarget_;
	}

private:
	static void CreateDevice();
	//인터페이스 구조체 ID3D11Device타입 포인터 객체와 ID3D11DeviceContext타입 포인터 객체를 생성해서,
	// 각각 멤버변수 device_와 deviceContext_로 저장/관리하는 함수. 

	static void CreateSwapChain();
	//인터페이스 구조체 IDXGISwapChain타입 포인터 객체를 생성해서 멤버변수 swapChain_으로 저장/관리하는 함수.


private:
	static ID3D11Device* device_;
	//다이렉트X 디바이스: 그래픽카드에 메모리 공간을 할당하려고 할 때 필요한 것.
	//그래픽카드의 메모리에 new연산을 할 수 있는 권한을 가지고 있다.

	static ID3D11DeviceContext* deviceContext_;	//다이렉트X의 디바이스 컨텍스트.
	//그래픽카드에 렌더링 관련 연산을 명령할 때 필요하다. 
	//다이렉트 9때는 없었지만 11때는 device와 deviceContext가 분리되었다.

	static IDXGISwapChain* swapChain_;	//수직동기화와 다중 버퍼링을 하는데 사용되는 다이렉트X의 인터페이스.
	//필요한 윈도우 개수만큼 만들면 된다.

	static GameEngineRenderTarget* backBufferRenderTarget_;
	//백버퍼로 쓰이는 렌더타겟. 여기에 그려지는것만이 더블버퍼링 과정을 거쳐 화면에 그려진다.
};

