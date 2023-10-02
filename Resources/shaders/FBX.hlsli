cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos : POSITION;//�ʒu
	float3 normal :NORMAL;//���_�@��
	float2 uv : TEXCOORD;//�e�N�X�`���[���W
	uint4 boneIndices : BONEINDICES;//�{�[���ԍ�
	float4 boneWeights : BONEWEIGHTS;//�{�[���̃X�L���E�F�C�g
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

//�{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer skinning:register(b3)//�{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
}