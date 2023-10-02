#pragma once
#include "GameObject.h"
#include "Fbx_Object3d.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "TouchableObject.h"
#include "Shake.h"
#include "Audio.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class Floor : TouchableObject
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	~Floor();

	bool Initialize(XMFLOAT3 pos);

	void Update();

	void Draw();

	//��̉����グ
	void UpMove(bool landF = false);

	void DownMove(bool flag = false);

	void OnCollision(bool flag = false);
public:
	inline void SetPosition(const XMFLOAT3& position) { object->SetPosition(position); }
	inline const XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetColor(const XMFLOAT4& position) { object->SetColor(color); }
	inline const XMFLOAT4& GetColor() const { return object->GetColor(); }

	inline void SetScale(const XMFLOAT3& scale) { object->SetScale(scale); }
	inline const XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

protected:
	XMFLOAT3 position;
	std::unique_ptr<TouchableObject> object;
	bool alive = true;
	XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	//�C�[�W���O�p
	float easFrame = 0.0f;
	float downFrame = 0.0f;
	//�V�F�C�N�p
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
	Audio* audio = nullptr;
};

