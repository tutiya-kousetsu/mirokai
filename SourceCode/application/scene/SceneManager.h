#pragma once
#include "BaseScene.h"
#include "Audio.h"
/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
public:
	~SceneManager();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	//���V�[���\��
	void SetNextScene(BaseScene* nextScene) { this->nextScene = nextScene; }
private:
	//���̃V�[��
	BaseScene* scene = nullptr;
	//���̃V�[��
	BaseScene* nextScene = nullptr;
};

