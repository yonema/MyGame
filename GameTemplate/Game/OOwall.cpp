#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"


bool OOwall::StartSub()
{
	Init("Assets/modelData/wall.tkm", enWall);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 400.0f,1000.0f,100.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

void OOwall::UpdateSub()
{
	if (m_moveFlag)
	{
		Vector3 moveSpeed = g_vec3Up;
		if (m_moveCounter >= 120)
		{
			moveSpeed = g_vec3Down;
			if (m_moveCounter >= 240)
			{
				m_moveCounter = 0;
			}
		}
		m_rotation.Apply(moveSpeed);
		moveSpeed.Scale(5.0f);

		m_position += moveSpeed;
		m_moveCounter++;



	}

	return;
}