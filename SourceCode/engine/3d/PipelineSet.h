#pragma once

#include <wrl.h>
#include <d3d12.h>

// �p�C�v���C���Z�b�g
struct PipelineSet
{
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
};

