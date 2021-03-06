#pragma once
#include "ObstacleObject.h"
#include "SoundCue.h"
class ROrunning_stop;

/// <summary>
/// 障害オブジェクト
/// 壁オブジェクト
/// </summary>
class OOwall :public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	OOwall();						//コンストラクタ
	bool StartSub() override final;	//スタート関数
	~OOwall();						//デストラクタ
	void UpdateSub()override final;	//アップデート関数

public:		//メンバ関数

	/// <summary>
	/// 稼働モード
	/// </summary>
	void Running()
	{
		m_moveFlag = true;
	}

	/// <summary>
	/// 停止モード
	/// </summary>
	void Stop()
	{
		m_moveFlag = false;
	}

	/// <summary>
	/// 持っている
	/// 反転オブジェクトの「稼働、停止オブジェクト」を設定する
	/// </summary>
	/// <param name="run_stop">稼働、停止オブジェクト</param>
	void SetRun_stop(ROrunning_stop* run_stop)
	{
		m_pRun_stop = run_stop;
	}

	/// <summary>
	/// 持っている
	/// 反転オブジェクトの「稼働、停止オブジェクト」を得る
	/// </summary>
	/// <returns></returns>
	ROrunning_stop* GetRun_stop()
	{
		return m_pRun_stop;
	}

	void MoveSE();

private:	//privateなメンバ関数

	/// <summary>
	/// 一回目のアップデートでだけ呼ばれる関数
	/// </summary>
	void FirstUpdate();

private:	//データメンバ
	bool m_moveFlag = false;				//稼働状態か？
	float m_moveTimer = 0;					//稼働中のカウンター
	Vector3 m_startPosition = g_VEC3_ZERO;	//移動前の初期位置
	Vector3 m_endPosition = g_VEC3_ZERO;		//移動先の終端位置
	ROrunning_stop* m_pRun_stop = nullptr;	//稼働、停止オブジェクトのポインタ
	bool m_firstUpdateFlag = true;			//一回目のアップデートか？
	CSoundCue* m_wallmoveSE = nullptr;	//wallmoveSEのサウンドキュー
};

