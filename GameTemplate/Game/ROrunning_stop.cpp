#include "stdafx.h"
#include "ROrunning_stop.h"

//スタート関数
bool ROrunning_stop::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/running.tkm", EN_RO_TYPE_RUNNING,
		"Assets/modelData/stop.tkm", EN_RO_TYPE_STOP);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROrunning_stop::QuerySub()
{
	//自身が「稼働」の時
	if (GetObjectType() == EN_RO_TYPE_RUNNING)
	{
		//障害オブジェクトの「壁」をクエリ
		QueryLOs<OOwall>(EN_OO_TYPE_WALL, [&](OOwall* wall) -> bool
			{
				//自身と「壁」が衝突したら
				if (IsHitLevelObject(*this, *wall))
				{
					//壁を稼働モードにする
					wall->Running();
					//衝突した壁のアドレスをポインタに保持
					m_pWall = wall;
					//ロック中にする
					SetLock(true);

					//衝突した壁に、すでに中身があるかどうか調べる
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
					{
						//中身があったら、その中身を消してから
						//自身を入れる
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//中身がなかったら、
						//そのまま自身を入れる
						wall->SetRun_stop(this);
					}
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					SetOverlapFlag(false);
				}
				return true;
			}
		);
	}
	//自身が「停止」の時
	else if (GetObjectType() == EN_RO_TYPE_STOP)
	{
		//障害オブジェクトの「壁」をクエリ
		QueryLOs<OOwall>(EN_OO_TYPE_WALL, [&](OOwall* wall) -> bool
			{
				//自身と「壁」が衝突したら
				if (IsHitLevelObject(*this, *wall))
				{
					//壁を停止モードにする
					wall->Stop();
					//衝突した壁のアドレスをポインタに保持
					m_pWall = wall;
					//ロック中にする
					SetLock(true);

					//衝突した壁に、すでに中身があるかどうか調べる
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
					{
						//中身があったら、その中身を消してから
						//自身を入れる
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//中身がなかったら、
						//そのまま自身を入れる
						wall->SetRun_stop(this);
					}
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					SetOverlapFlag(false);
				}
				return true;
			}
		);
	}
}

//アップデート関数
void ROrunning_stop::UpdateSub()
{
	//壁のアドレスを保持していたら、場所と回転を更新する
	if (m_pWall)
	{
		//壁の場所からどのくらいの上の場所にいるか
		const float upVecScale = 100.0f;

		//壁の回転を得る
		Quaternion qRot = m_pWall->GetRotation();
		//アップベクトル
		Vector3 upVec = g_VEC3_UP;
		//アップベクトルに壁の回転を乗算する
		qRot.Apply(upVec);
		//正規化する
		upVec.Normalize();
		//イイ感じの大きさにスケールする
		upVec.Scale(upVecScale);

		//場所を更新
		SetPosition(m_pWall->GetPosition() + upVec);
		//回転を更新
		SetRotation(qRot);
	}
}
