#include "Shake.h"
#include <random>
using namespace std;

void Shake::ShakePos(float& pos, const int shakeMax, const int shakeMin, const int TargetTime, const int Division)
{
	if (shakeStart) {
		if (shakeTime < TargetTime) {
			//minstd_rand->�����ɑ΂��āA��Z�A���Z�A��]�Z����񂸂s��
			//minstd_rand��minstd_rand0���4�{�قǑ����B
			mt19937 mt{ std::random_device{}() };
			//�w�肳�ꂽ�͈͂̒l�����m���Ŕ�������悤���U���z����
			uniform_int_distribution<int> l_dist(shakeMin, shakeMax);
			pos = float(l_dist(mt)) / Division;
			shakeTime++;
		}
		else {
			shakeStart = false;
			shakeTime = 0;
			pos = 0.0f;
		}
	}
}