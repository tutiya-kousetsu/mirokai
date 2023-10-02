#pragma once
#include "Camera.h"
#include "GameObject.h"

class FollowingCamera : public Camera
{
public:
	using Camera::Camera;

	//�R���X�g���N�^
	FollowingCamera();

	//�v���C���[�ɃZ�b�g����
	inline void SetFollowingTarget(Object3d* followingTarget) { this->followingTarget = followingTarget; }

	void startUpdate()override;

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

private:
	Object3d* followingTarget;

	// �^�[�Q�b�g����̃J�����̍���
	float CameraTargetHeight = 5;

	XMFLOAT3 CameraTarget = { 0,5,0 };

	// �J�����̎��_���璍���_�܂ł̋���
	float CameraTargetLengthY = 15;
	float CameraTargetLengthZ = -15;
};