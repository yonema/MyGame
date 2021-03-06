#pragma once
#include "SoundSource.h"

//3Dサウンドは試してない
//たぶんできない


/// <summary>
/// サウンドキュー。
/// サウンドを再生するときはこのクラスを使ってね。
/// 別にCSoundSourceを使ってもいいけど、できればこっち使ってほしいな！
/// </summary>
class CSoundCue : public IGameObject
{
public:		//自動で呼ばれる関数
	~CSoundCue();	//デストラクタ
public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// 初期化用関数
	/// wavファイルのパスを指定して初期化する
	/// </summary>
	/// <param name="filePath">wavファイルパス</param>
	/// <param name="is3DSound">3D再生するか？（たぶんできない）</param>
	void Init(const wchar_t* filePath, const bool is3DSound = false);

	/// <summary>
	/// サウンドを再生
	/// </summary>
	/// <param name="isLoop">ループ再生するか？</param>
	void Play(const bool isLoop);

	/// <summary>
	/// ボリュームを設定
	/// 1.0fが通常。
	/// 負の値を入れると勝手に絶対値になるから注意
	/// </summary>
	/// <param name="volume">ボリューム</param>
	void SetVolume(const float volume);

	/// <summary>
	/// ボリュームを取得
	/// </summary>
	/// <returns>ボリューム</returns>
	const float GetVolume()const
	{
		return m_volume;
	}

	/// <summary>
	/// 再生を停止する。
	/// これで停止すると、次に再生した時、
	/// 最初からの再生になる。
	/// 今の仕様だと、ループ再生は停止できるが、
	/// ワンショット再生の停止はできない。
	/// </summary>
	void Stop();

	/// <summary>
	/// 再生を一時停止する。
	/// これで一時停止すると、次に再生した時、
	/// 途中からの再生になる。
	/// 今の仕様だと、ループ再生は一時停止できるが、
	/// ワンショット再生の一時停止はできない。
	/// </summary>
	void Pause();

	/// <summary>
	/// 今再生中かどうか？
	/// 一時停止の検知はIsPaused()を使うが、
	/// 停止の検知にはこのIsPlaying()を使う。
	/// 今の仕様だと、ループ再生は検知できるが、
	/// ワンショット再生の検知はできない。
	/// </summary>
	/// <returns>再生中か？</returns>
	const bool IsPlaying()const;

	/// <summary>
	/// 一時停止中かどうか？
	/// 停止の検知はIsPlaying()を使うが、
	/// 一時停止の検知はこのIsPaused()を使う。
	/// 今の仕様だと、ループ再生は検知できるが、
	/// ワンショット再生の検知はできない。
	/// </summary>
	/// <returns>停止中か？</returns>
	const bool IsPaused()const
	{
		return m_isPaused;
	}

	/// <summary>
	/// ループする曲が途中のところに飛ぶ場合、
	/// この関数の返り値を使用して判定する。
	/// </summary>
	/// <returns>１回以上曲の終わりにきたか</returns>
	const bool IsLoop()const
	{
		return m_loopSoundSource->GetLoopStart();
	}

	/// <summary>
	/// サウンドタイプを設定する
	/// CSoundCue::enBGMかCSoundCue::enSEを引数に指定してね。
	/// </summary>
	/// <param name="soundType">サウンドタイプ</param>
	void SetSoundType(const int soundType)
	{
		m_soundType = soundType;
	}



public:
	//サウンドタイプ
	enum EnSountType
	{
		enBGM,	//BGM
		enSE,	//SE
	};
private:	//privateなメンバ関数
	void PlayLoop();		//ループ再生の処理
	void PlayOneShot();		//ワンショット再生の処理
	float GetTypeVolume();	//タイプ別のボリュームを取得

private:	//データメンバ
	wchar_t m_filePath[256];					//wavファイルパス
	bool m_is3DSound = false;					//3Dサウンドで再生するか？
	float m_volume = 1.0f;						//全体のボリューム
	int m_soundType = enSE;						//サウンドのタイプ
	bool m_isPaused = false;					//一時停止中か？
	CSoundSource* m_loopSoundSource = nullptr;	//ループ再生用のサウンドソース

public:		//staticなメンバ関数

	/// <summary>
	/// BGMのボリュームを設定する
	/// </summary>
	/// <param name="BGMVolume">BGMのボリューム</param>
	static void SetBGMVolume(const float BGMVolume)
	{
		m_BGMVolume = BGMVolume;
	}

	/// <summary>
	/// SEのボリュームを設定する
	/// </summary>
	/// <param name="SEVolume">SEのボリューム</param>
	static void SetSEVolume(const float SEVolume)
	{
		m_SEVolume = SEVolume;
	}

private:	//staticなデータメンバ
	static float m_BGMVolume;					//BGMのボリューム
	static float m_SEVolume;					//SEのボリューム
};

