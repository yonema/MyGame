#pragma once

//基本的に必要
#include "GameCamera.h"
#include "SoundCue.h"
#include "Sky.h"

//ポーズ画面用
#include "Pause.h"

//レベルのロードで必要
#include "Level.h"
#include "LevelObjectManager.h"

//基本オブジェクト
#include "Player.h"
#include "Mobius.h"

//「反転オブジェクト」ReversibleObject
#include "ROmizu_kori.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "ROkeymold_empty.h"

//「障害オブジェクト」ObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"


/// <summary>
///	ステージベースクラス
/// </summary>
class IStageBase : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;				//スタート関数
	~IStageBase();							//デストラクタ
	void Update()override final;			//アップデート関数
	void UpdateOnlyPaused()override final;	//ポーズ中のみ呼ばれるアップデート関数

private:	//privateなメンバ関数

	/// <summary>
	/// クリアした時の処理
	/// </summary>
	void Clear();

	/// <summary>
	/// リトライした時の処理
	/// </summary>
	void Retry();

	/// <summary>
	///	終了した時の処理
	/// </summary>
	void Quit();

	/// <summary>
	/// ゴールした時の処理
	/// </summary>
	void Goal();

	void BGMInteractive();

public: //インライン関数
	void SetStartUpStartDirecting(const bool b)
	{
		m_startUpStartDirecting = b;
	}
	void SetTitlePlayer(const bool b)
	{
		m_titlePlayer = b;
	}
	void SetStartBGM(const bool b)
	{
		m_startBGM = b;
	}

protected:	//protectedなメンバ関数
	/// <summary>
	/// レベルのロード
	/// </summary>
	/// <param name="filePath">tklのファイルパス</param>
	void LoadLevel(const char* tklFilePath);

	/// <summary>
	/// リリース関数
	/// </summary>
	void Release()
	{
		DeleteGO(this);
	}

protected:	//オーバーライドしてほしいメンバ関数

	/// <summary>
	/// オーバーライドしほしいスタート関数
	/// </summary>
	/// <returns></returns>
	virtual bool StartSub() = 0 { return true; };

	/// <summary>
	/// オーバーライドしてほしい、
	/// リトライした時の処理
	/// </summary>
	virtual void RetryStage() = 0 {};

private:	//データメンバ
	CLevel m_level;									//レベルロード用クラス
	CPause* m_pause = nullptr;						//ポーズクラス
	OOgoal* m_goal = nullptr;						//ゴールクラス
	CSky* m_sky = nullptr;							//空クラス
	int m_goalCounter = 0;							//ゴールした後の待ち時間をはかる

	StartDirecting* m_startDirecting = nullptr;
	bool m_startUpStartDirecting = true;
	bool m_titlePlayer = false;	


private:
	CSoundCue* m_bgmStage = nullptr;				//BGMのサウンドキュー
	CSoundCue* m_bgmStage2 = nullptr;				//BGMのサウンドキュー
	bool m_startBGM = true;
};

