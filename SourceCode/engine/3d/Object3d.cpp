﻿#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Sprite.h"
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Object3d::device = nullptr;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3d::rootsignature;
ComPtr<ID3D12PipelineState> Object3d::pipelinestate;
XMMATRIX Object3d::matView{};
XMMATRIX Object3d::matProjection{};
XMFLOAT3 Object3d::eye = { 0, 5.0f, -50.0f };
XMFLOAT3 Object3d::target = { 0, 0, 0 };
XMFLOAT3 Object3d::up = { 0, 1, 0 };
Object3d::PipelineSet Object3d::pipelineSet;
Camera* Object3d::camera = nullptr;

bool Object3d::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	Object3d::device = device;
	Object3d::cmdList = cmdList;

	// モデルの静的初期化
	Model::StaticInitialize(device);

	// パイプライン初期化
	InitializeGraphicsPipeline();
	return true;
}

void Object3d::PreDraw()
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	//assert(Object3d::cmdList == nullptr);

	// コマンドリストをセット
	//Object3d::cmdList = cmdList;

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	// コマンドリストを解除
	//Object3d::cmdList = nullptr;
}

std::unique_ptr<Object3d> Object3d::Create() {
	// 3Dオブジェクトのインスタンスを生成
	Object3d* object3d = new Object3d();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	// ユニークポインタを作成してreturn
	return std::unique_ptr<Object3d>(object3d);
}
bool Object3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		//L"Resources/shaders/BasicVertexShader.hlsl",	// シェーダファイル名
		L"Resources/shaders/OBJVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		//L"Resources/shaders/BasicPixelShader.hlsl",
		L"Resources/shaders/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

Object3d::~Object3d()
{
	if (collider) {
		//コリジョンマネージャから登録を解除
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
	delete parent;
//	delete model;
}

bool Object3d::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;

	// 定数バッファの生成B0
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffB0));

	//クラス名の文字列を取得
	name = typeid(*this).name();

	return true;
}

void Object3d::Update()
{
	assert(camera);

	HRESULT result;
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = this->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->mat = matWorld * matViewProjection;	// 行列の合成
	constMap->color = color;
	constMap->viewproj = matViewProjection;
	constMap->world = this->matWorld;
	constMap->cameraPos = cameraPos;
 	this->constBuffB0->Unmap(0, nullptr);
	
	//当たり判定更新
	if (collider) {
		collider->Update();
	}
}

void Object3d::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Object3d::cmdList);

	//モデルがセットされていなければ描画をストップ
	if (this->model == nullptr) {
		return;
	}
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//// ルートシグネチャの設定
 	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuffB0->GetGPUVirtualAddress());

	//モデルを描画
	this->model->Draw(cmdList);
}

void Object3d::UpdateWorldMatrix()
{
	assert(camera);

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	if (isBillboard && camera) {
		const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

		matWorld = XMMatrixIdentity();
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matBillboard;
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}
	else {
		matWorld = XMMatrixIdentity(); // 変形をリセット
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}
}

void Object3d::Pop(XMFLOAT3 position)
{
	this->position = position;
}

void Object3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//コリジョンマネージャに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	UpdateWorldMatrix();
	//コライダーを更新しておく
	collider->Update();
}
