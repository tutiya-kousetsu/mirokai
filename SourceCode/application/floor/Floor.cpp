#include "Floor.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Easing.h"
#include <fstream>

Floor::~Floor()
{
	if (collider) {
		//�R���W�����}�l�[�W������o�^������
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool Floor::Initialize(XMFLOAT3 position)
{
	object.reset(new TouchableObject());
	object->Initialize(Model::CreateFromOBJ("FloorBox"));
	object->SetScale({ 1.6f, 1.6f, 1.6f });
	shake.reset(new Shake());
	//this->position = position;
	object->SetPosition(position);
	return true;

}

void Floor::Update()
{
	object->Update();
}

void Floor::Draw()
{
	object->Draw();
}

void Floor::UpMove(bool landF)
{
	float move = 0.1f;
	//���n������
	if (landF) {
			//�C�[�W���O�ŉ����グ��
			if (easFrame < 1.0f) {
				easFrame += 0.0001f;
			}
			if (position.y <= -0.1f) {
				position.y = Ease(Out, Quad, easFrame, position.y, 0);
			}
			//�V�F�C�N�̃t���O�𗧂Ă�
			shake->SetShakeStart(true);
			//�|�W�V������y���W��-0.1�܂ōs������t���O��false�ɂ���
			if (position.y >= -0.1f) {
				shake->SetShakeStart(false);
				easFrame = 0;
			}
	}
	//�V�F�C�N�̍ő�l�ŏ��l�Ȃǂ�����
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//�t���O��false��������V�F�C�N���Ƃ߂�
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
		
	}
	//�|�W�V�����ɃV�F�C�N�̓����𑫂�
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
}

void Floor::DownMove(bool flag)
{
	float move = 0.1f;
	//���n������
	if (flag) {
			//�C�[�W���O�ŉ����グ��
			if (downFrame < 1.0f) {
				downFrame += 0.0001f;
			}
		if (position.y >= -9.9f) {
			position.y = Ease(Out, Quad, downFrame, position.y, -10.0f);
		}
			//�V�F�C�N�̃t���O�𗧂Ă�
			shake->SetShakeStart(true);
			//�|�W�V������y���W��-0.1�܂ōs������t���O��false�ɂ���
			if (position.y <= -9.9f) {
				shake->SetShakeStart(false);
				downFrame = 0;
			}
	}
	//�V�F�C�N�̍ő�l�ŏ��l�Ȃǂ�����
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//�t���O��false��������V�F�C�N���Ƃ���
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
		
	}
	//�|�W�V�����ɃV�F�C�N�̓����𑫂�
	position.x += shakePos.x;
	position.z += shakePos.z;

	SetPosition(position);
}

void Floor::OnCollision(bool flag)
{

	if (flag) {
		color = object->GetColor();
		color.w = 0.45f;
		object->SetColor({ 1.0f, 1.0f, 1.0f, color.w });
	}
	if (!flag) {
		object->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	}
}