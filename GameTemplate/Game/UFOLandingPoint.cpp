#include "stdafx.h"
#include "UFOLandingPoint.h"

//�X�^�[�g�֐�
bool CUFOLandingPoint::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/grilled_chicken.tkm", enEmpty);

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });

	//OBB�̃p�����[�^�[��ݒ肷��
	GetOBB().SetDirectionLength({ 2000.0f,750.0f,500.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	//�[��OBB�̃p�����[�^�[�̐ݒ������
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		m_sideOBB[i].SetDirectionLength({ 10.0f,750.0f,500.0f });
		m_sideOBB[i].SetPivot({ 0.5f,0.0f,0.5f });
		m_sideOBB[i].SetPosition(m_position);
		m_sideOBB[i].SetRotation(m_rotation);
	}




	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f���̐����Ə�����
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//�[��OBB�����邽�߂̃��f���̐����Ə�����
		vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgSidePosMR[i][j] = NewGO<CModelRender>(0);
			m_dbgSidePosMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
			m_dbgSidePosMR[i][j]->SetPosition(vertPos[j]);
			m_dbgSidePosMR[i][j]->SetRotation(m_rotation);
		}
	}
	//�f�o�b�N�p�����܂�

	//�[��OBB�̃A�b�v�f�[�g
	UpdateSideOBB();

	return true;
}

//�f�X�g���N�^
CUFOLandingPoint::~CUFOLandingPoint()
{
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f���̔j��
		DeleteGO(m_dbgVertPosMR[i]);
	}

	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//�[��OBB�����邽�߂̃��f���̔j��
		for (int j = 0; j < m_vertNum; j++)
		{
			DeleteGO(m_dbgSidePosMR[i][j]);
		}
	}
	//�f�o�b�N�p�����܂�
}

/// <summary>
/// �[��OBB�̃A�b�v�f�[�g
/// </summary>
void CUFOLandingPoint::UpdateSideOBB()
{
	//����
	const float sideLen = 150.0f;
	//���E�ւ̃x�N�g��
	Vector3 sideVec[enLeftAndRightNum] = { g_vec3Left, g_vec3Right };

	//���ƉE�̓�񕪌J��Ԃ�
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		//���E�̃x�N�g�������g�̉�]�ŉ�
		m_rotation.Apply(sideVec[i]);
		//�����̑傫�����|����
		sideVec[i].Scale(sideLen);
		//�Ȃ񂩋t�ɂȂ��Ă邩��A���]������
		sideVec[i].Scale(-1.0f);

		//OBB�̍X�V
		m_sideOBB[i].SetPosition(m_position + sideVec[i]);
		m_sideOBB[i].SetRotation(m_rotation);
	}
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		Vector3* vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgSidePosMR[i][j]->SetPosition(vertPos[j]);
			m_dbgSidePosMR[i][j]->SetRotation(m_rotation);
		}
	}
	//�f�o�b�N�p�����܂�

}