#include "stdafx.h"
#include "OOReverseALL.h"

//スタート関数
bool OOReverseALL::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/grilled fish.tkm", enEmpty);

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//自己発光色を設定
	SetEmissionColor({ 0.5f,0.0f,0.5f,1.0f });

	//OBBのパラメーターを設定する
	GetOBB().SetDirectionLength({ 10.0f,400.0f,400.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });


	//デバック用
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの生成と初期化
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}
	//デバック用ここまで

	return true;
}

//デストラクタ
OOReverseALL::~OOReverseALL()
{

	//デバック用
	//後で消す
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの破棄
		DeleteGO(m_dbgVertPosMR[i]);
	}
	//デバック用ここまで
}

//アップデート関数
void OOReverseALL::UpdateSub()
{
	//アップデートステートで処理を振り分ける
	switch (m_updateState)
	{
	case enBeforHitPlayer:
		//プレイヤーと衝突前の処理
		BeforeHitPlayer();
		break;
	case enHitPlayer:
		//プレイヤーと衝突時の処理
		HitPlayer();
		break;
	case enAfterHitPlayer:
		//プレイヤーと衝突後の処理
		AfterHitPlayer();
		break;
	}

}

//プレイヤーと衝突前の処理
void OOReverseALL::BeforeHitPlayer()
{
	//プレイヤーとの衝突を調べる
	if (IsHitPlayer())
	{
		//衝突している

		//衝突時のプレイヤーの座標を保治
		m_playerHitPosition = m_pPlayer->GetPosition();
		//アップデートステートをプレイヤーと衝突時の状態へ
		m_updateState = enHitPlayer;
	}
	else
	{
		//衝突していない

		//衝突前のプレイヤーの座標を保持
		m_playerBeforePosition = m_pPlayer->GetPosition();
	}
}

//プレイヤーと衝突時の処理
void OOReverseALL::HitPlayer()
{
	//プレイヤーとの衝突を調べる
	if (IsHitPlayer())
	{
		//衝突している

		//衝突時のプレイヤーの座標を保治
		m_playerHitPosition = m_pPlayer->GetPosition();
	}
	else
	{
		//衝突していない

		//アップデートステートをプレイヤーと衝突後の状態へ
		m_updateState = enAfterHitPlayer;
	}
}

//プレイヤーと衝突後の処理
void OOReverseALL::AfterHitPlayer()
{
	//衝突後のプレイヤーの
	m_playerAfterPosition = m_pPlayer->GetPosition();

	Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
	Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
	beforeToHit.Normalize();
	hitToAfter.Normalize();
	float inner = Dot(beforeToHit, hitToAfter);

	if (inner >= 0.0f)
	{
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		for (int i = 0; i < levelObjects.size(); i++)
		{
			CReversibleObject* revers = dynamic_cast<CReversibleObject*>(levelObjects[i]);
			if (revers)
				revers->Reverse();
		}

	}
	m_updateState = enBeforHitPlayer;
}