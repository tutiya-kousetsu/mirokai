#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

Audio* Audio::GetInstance()
{
	static Audio instance;

	return &instance;
}

bool Audio::Initialize()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

/// <summary>
/// �T�E���h�ǂݍ���
/// </summary>
/// <param name="filename"> �t�@�C���� </param>
void Audio::SoundLoadWave(const char* filename)
{
	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(baseDirectory + filename, std::ios_base::binary);
	// �t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	// �`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// �`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Wave�t�@�C�������
	file.close();

	// return����ׂ̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	// �t�@�C�����œo�^
	soundDatas.emplace(filename, soundData);

	HRESULT result;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundDatas[filename].wfex);
	assert(SUCCEEDED(result));

	// �t�@�C�����œo�^
	pSourceVoices.emplace(filename, pSourceVoice);
}

/// <summary>
/// 
/// </summary>
void Audio::SoundUnload()
{
	// �o�b�t�@�̃����������
	soundDatas.clear();
	pSourceVoices.clear();
}

/// <summary>
/// �Đ�
/// </summary>
/// <param name="filename">�t�@�C����</param>
/// <param name="loopFlag">���[�v�t���O</param>
void Audio::SoundPlayWave(const char* filename, bool loopFlag)
{
	HRESULT result;

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};

	buf.pAudioData = soundDatas[filename].pBuffer;
	buf.AudioBytes = soundDatas[filename].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//  buf.PlayBegin = 0;
	if (loopFlag) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// �g�`�f�[�^�̍Đ�
	result = pSourceVoices[filename]->FlushSourceBuffers(); //�ŏ�����Đ�����
	result = pSourceVoices[filename]->SubmitSourceBuffer(&buf);
	result = pSourceVoices[filename]->Start();
}

/// <summary>
/// �Đ����~�߂�
/// </summary>
/// <param name="filename"></param>
void Audio::SoundStop(const char* filename)
{
	pSourceVoices[filename]->Stop();
}
