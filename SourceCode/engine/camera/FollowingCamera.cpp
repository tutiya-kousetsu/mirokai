#include "FollowingCamera.h"
#include "WinApp.h"

#include <DirectXMath.h>

using namespace DirectX;

FollowingCamera::FollowingCamera() :Camera(WinApp::window_width, WinApp::window_height)
{

}

void FollowingCamera::startUpdate()
{
	if (followingTarget) {
		XMFLOAT3 target = followingTarget->GetPosition();

		float sinNum = sinf(XMConvertToRadians(followingTarget->GetRotation().x));
		float cosNum = cosf(XMConvertToRadians(followingTarget->GetRotation().x));

		// x����]�𔽉f�����ʒu
		XMFLOAT3 tempPosition = { 0,CameraTargetLengthZ ,CameraTargetLengthY };

		sinNum = sinf(XMConvertToRadians(-followingTarget->GetRotation().y));
		cosNum = cosf(XMConvertToRadians(-followingTarget->GetRotation().y));

		// y����]�𔽉f�����ʒu
		XMFLOAT3 tempPosition2 = {
			cosNum * tempPosition.x - sinNum * tempPosition.y,
			tempPosition.z,
			sinNum * tempPosition.x + cosNum * tempPosition.y
		};

		//�^�[�Q�b�g�ɔ��f�����ʒu�𑫂�
		XMFLOAT3 eye = {
		target.x + tempPosition2.x,
		target.y + tempPosition2.y,
		target.z + tempPosition2.z };

		// �ړ��O�̍��W
		XMFLOAT3 old = GetEye();
		// �ړ��� = �ړ���̍��W - �ړ��O�̍��W
		XMFLOAT3 velocity ={ (eye.x - old.x),(eye.y - old.y),(eye.z - old.z) };
		// �ړ���̍��W = �ړ��O�̍��W + �ړ���
		eye = { old.x + velocity.x,old.y + velocity.y ,old.z + velocity.z };
		// �ړ���̍��W��K�p
		SetEye(eye);

		XMFLOAT3 Destination;//�s���悫
		XMStoreFloat3(&Destination,
			XMVector3Transform(XMVectorSet(CameraTarget.x,CameraTarget.y,CameraTarget.z, 1),
			followingTarget->GetMatRot()));

		target.x += Destination.x;
		target.y += Destination.y;
		target.z += Destination.z;
		SetTarget(target);
	}
}

XMVECTOR FollowingCamera::GetWorldPosition()
{
	XMVECTOR worldPos{};

	worldPos.m128_f32[0] = target.x;
	worldPos.m128_f32[1] = target.y;
	worldPos.m128_f32[2] = target.z;

	return worldPos;
}
