#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <wrl.h>
#include <chrono>
#include"WinApp.h"


class DirectXCommon
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static DirectXCommon* GetInstance();

	void Initialize(WinApp* winApp);

	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDev() { return dev.Get(); }

	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

private:
	void InitializeDevice();

	void InitializeCommand();

	void InitializeSwapchain();

	void InitializeRecderTargetView();

	void InitializeDepthBuffer();

	void InitializeFence();

	//FPS�Œ菉����
	void InitializeFixFPS();

	//FPS�Œ�X�V
	void UpdateFixFPS();

public:
	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);


	//WindowsAPI
	WinApp* winApp = nullptr;

	//�f�o�C�X
	ComPtr<ID3D12Device> dev;
	//DXGI�t�@�N�g��
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	ComPtr<ID3D12Resource> depthBuffer;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
private:
	//�L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;
};

