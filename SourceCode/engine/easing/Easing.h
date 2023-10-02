#pragma once

//�C�[�W���O�̃^�C�v
enum  type {
	In = 0,
	Out,
	InOut
};

enum category {
	Quad = 0, Cubic, Quart, Quint, Sine, Exp, Circ, Back, SoftBack, Elastic, Bounce, Linear
};

//���ꂼ��̃C�[�W���O�֐�
namespace EasingFunction {
	float InQuad(float t);
	float InCubic(float t);
	float InQuart(float t);
	float InQuint(float t);
	float InSine(float t);
	float InExp(float t);
	float InCirc(float t);
	float InBack(float t);
	float InSoftBack(float t);
	float InElastic(float t);
	float InBounce(float t);
	float InLinear(float t);
}

//�C�[�W���O�^�C�v�ɂ���ē������ς��
float Ease(type Type, category Ease, float t, float start, float end);

