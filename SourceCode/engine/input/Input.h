#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"

using namespace Microsoft::WRL;

class Input
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

public://�����o�[�֐�
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Input* GetInstance();

	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();
	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="kyeNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="kyeNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseRight();

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();

	inline POINT GetMousePos() { return mousePos; };
	inline void SetMousePos(const POINT& pos) {
		mousePos = pos;
		// �N���C�A���g���W(�Q�[���E�B���h�E�̍��W)����X�N���[�����W(�p�\�R���̉��)�ɕϊ�
		ClientToScreen(winApp->GetHwnd(), &mousePos);
		// �ϊ���̍��W�̈ʒu�Ƀ}�E�X�J�[�\���̈ʒu���Z�b�g
		SetCursorPos(mousePos.x, mousePos.y);
		mousePos = pos;
	}
private://�����o�[�ϐ�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devKeyboard;
	ComPtr<IDirectInput8> dinput = nullptr;
	//����̑S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	POINT mousePos;

	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	ComPtr<IDirectInputDevice8> devJoyStick;
	DIJOYSTATE2 joyState = {};
	DIJOYSTATE2 joyStatePre = {};
private:
	// WindowsAPI
	WinApp* winApp = nullptr;
};