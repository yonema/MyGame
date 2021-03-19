#include "stdafx.h"
#include "StartDirecting.h"

#include "GameCamera.h"
#include "Player.h"
#include "Pause.h"

bool StartDirecting::Start()
{
	m_onWayPosition = m_position;
	m_mobius = FindGO<Mobius>("Mobius");

	m_startPosition = m_position;

	return true;
}

StartDirecting::~StartDirecting()
{

}

void StartDirecting::Update()
{
	//�J�����̃^�[�Q�b�g�������Ȃ��`�œ�������
	//�J�����𓮂����B
	
	//way�|�C���g[16]�܂œ������B

	if (m_startDirecting == false) {
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonA) == true) {
		m_startDirecting = false;
		//�v���C���[�̑���t���O��true�ɂ���B
		Player* player = FindGO<Player>("Player");
		player->SetOperationFlag(true);
		//�|�[�Y�̑���t���O��true�ɂ���B
		CPause* pause = FindGO<CPause>("Pause");
		pause->SetCanPause(true);
	}

	if (check == false) {

		//�J�����̈ړ���
		m_padLStickXF = 1.0f;

		//�E�F�C�|�C���g�̍X�V����
		CheckWayPoint();
		//�ړ�����
		Move();
		//���f���̉�]����
		Rotation();

		m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;
		GetOnStage();
	}
	else {
		if (count < startDirectingTime) {
			++count;
		}
		else {
			m_startDirecting = false;
			//�v���C���[�̑���t���O��true�ɂ���B
			Player* player = FindGO<Player>("Player");
			player->SetOperationFlag(true);
			//�|�[�Y�̑���t���O��true�ɂ���B
			CPause* pause = FindGO<CPause>("Pause");
			pause->SetCanPause(true);
		}
	}
}

void StartDirecting::CheckWayPoint()
{
	///
	///
	/// m_wayPointState�͍������0���瑝���Ă����B
	/// m_wayPointState��0�̎��Am_lpIndex��0,m_rpIndex��1�ɂȂ�B
	/// �܂�Am_lpIndex�͏��m_wayPointState�Ɠ����l�ɂȂ�A
	/// m_rpIndex��m_wayPointState��1���Z�����l�ɂȂ�B
	/// ������m_maxWayPointState��m_wayPointState�̍ő吔��\���B
	/// m_maxWayPointState��31��������m_wayPointState��31�܂ő��݂���B


	//1.�v���C���[���g�̍��E�̃E�F�C�|�C���g��ݒ肷��
	//m_wayPointState�����ƂɃE�F�C�|�C���g��ݒ肷��B
	m_rpIndex = m_wayPointState;	//�E�̃E�F�C�|�C���g�͂�m_wayPointState�͓����l
	m_lpIndex = m_rpIndex + 1;		//���̃E�F�C�|�C���g�͉E�̃E�F�C�|�C���g��1��̒l

	if (m_lpIndex > m_maxWayPointState)
	{
		//���̃E�F�C�|�C���g��MAX���傫��������
		//��������Ƃ������Ƃ�����A�X�^�[�g��0�ɂ���
		m_lpIndex = 0;
	}


	//2.m_wayPointState�̍X�V�B

	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
	LpToRpVec.Normalize();
	Vector3 LpToPlayerVec = m_position - (*m_wayPointPos)[m_lpIndex];
	LpToPlayerVec.Normalize();
	Vector3 RpToPlayerVec = m_position - (*m_wayPointPos)[m_rpIndex];
	RpToPlayerVec.Normalize();

	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);


	//���E�̃E�F�C�|�C���g�Ƃ̋����𒲂ׂ�
	float f = 0.35f;
	if (LpDotPlayer > f && RpDotPlayer < -f)
	{
		//���̃E�F�C�|�C���g�̊Ԃɂ���
	}
	else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
	{
		//���̃E�F�C�|�C���g�̊Ԃ���A�����ɏo�Ă�����
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			//m_wayPointState��MAX���傫��������
			//��������Ƃ������Ƃ�����A�X�^�[�g��0�ɂ���
			m_wayPointState = 0;
		}
	}
	else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
	{
		//���̃E�F�C�|�C���g�̊Ԃ���A�E������o�Ă�����
		//m_wayPointState�����Z���ĉE�ɐi�߂�B
		m_wayPointState -= 1;
		if (m_wayPointState < 0)
		{
			//m_wayPointState��0��菬����������
			//��������Ƃ������Ƃ�����AMAX�̒l�ɂ���
			m_wayPointState = m_maxWayPointState;
		}

		//�J�����̈ړ����~�߂�ʒu�ɗ������̊m�F
		if (m_wayPointState == 15) {
			check = true;
			//�J�����̊�鏈���̃t���O��true�ɂ���B
			GameCamera* gameCamera = FindGO<GameCamera>("GameCamera");
			gameCamera->StartDirectingZoomInCamera(true);
		}
	}



	return;

}

void StartDirecting::Move()
{
	//�ړ���������͖��t���[���v�Z�������������̂��ȁH
	//����Ƃ��Am_wayPointState�̐؂�ւ̎��ɂ������������̂��ȁH
	//����A���̂������Ɩ��t���[�����Ȃ��Ă͂����Ȃ��C������

	//1.���E�ւ̈ړ�����������v�Z����B

	//���ֈړ��������
	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
	moveToLeft.Normalize();
	//�E�ֈړ��������
	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
	moveToRight.Normalize();


	//2.�ړ�����

	//�Ƃ肠�����̏���
	//�d�͂�A�����x�A��R����������Ƃ��͕ʂ̂�����
	m_moveSpeed = g_vec3Zero;

	//�J�����̈ړ��X�s�[�h
	float moveLen = 1000.0f;


	if (m_padLStickXF < 0.0f)
	{
		//���ւ̈ړ��̓��͂���������
		//���ւ̈ړ��̌v�Z����
		m_moveSpeed += moveToLeft * m_padLStickXF * -moveLen;
	}
	else if (m_padLStickXF > 0.0f)
	{
		//�E�ւ̈ړ��̓��͂���������
		//�E�ւ̈ړ��̌v�Z������
		m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
	}


	return;
}

void StartDirecting::GetOnStage()
{
	m_upVec = g_vec3AxisY;
	m_finalWPRot.Apply(m_upVec);
	m_upVec.Scale(150.0f);
	if (m_mobius)
	{
		if (m_mobius->GetModel()->InIntersectLine(m_onWayPosition + m_upVec, m_onWayPosition - m_upVec))
		{
			m_position = m_mobius->GetModel()->GetIntersectPos();
		}
	}
	else
	{
		m_mobius = FindGO<Mobius>("Mobius");
	}
	auto hitPos = m_mobius->GetModel()->GetIntersectPos();

	//m_modelRender->SetPosition(hitPos);
	return;

}

void StartDirecting::Rotation()
{
	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];

	//���̃E�F�C�|�C���g����v���C���[�ւ̃x�N�g��
	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];

	//�⊮��
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	m_finalWPRot.Slerp(ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]);

	return;
}




void StartDirecting::SetWayPointPos
(const std::size_t vecSize, std::vector<Vector3>* const posMap)
{
	//vector�̃T�C�Y�̊m��
	//m_wayPointPos->resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPos�ɃE�F�C�|�C���g�́u�ꏊ�v���i�[����
	m_wayPointPos = posMap;
	//std::vector<Vector3>::iterator it = posMap->begin();
	//for (int index = 0; it != posMap->end(); index++, it++)
	//{
	//	m_wayPointPos[index] = &it;
	//}
}

void StartDirecting::SetWayPointRot
(const std::size_t vecSize, std::vector<Quaternion>* rotMap)
{
	//vector�̃T�C�Y�̊m��
	//m_wayPointRot->resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	m_wayPointRot = rotMap;
	//std::map<int, Quaternion>::iterator it = rotMap->begin();
	//for (int index = 0; it != rotMap->end(); index++, it++)
	//{
	//	m_wayPointRot[index] = &it->second;
	//}
}