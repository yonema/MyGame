#include "stdafx.h"
#include "Tips.h"

//コンストラクタ
CTips::CTips()
{
	//「TipsのUI」のスプライトレンダラーの生成と初期化
	m_tipsUISR = NewGO<CSpriteRender>(1);
	const float tipsUIX = 400.0f;
	const float tipsUIY = 260.0f;
	m_tipsUISR->Init("Assets/image/Tips_UI.DDS",
		512.0f * 1.5f, 256.0f * 0.8f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_tipsUISR->SetPosition({ tipsUIX, tipsUIY, 0.0f});
	m_tipsUISR->SetPostRenderFlag(true);

	//「Tips」のフォントレンダーの生成と初期化
	m_tipsFR = NewGO<CFontRender>(1);
	const float tipsX = 200.0f;
	const float tipsY = 330.0f;
	m_tipsFR->Init(L"T ip s", 
		{ tipsX,tipsY },			//座標
		{ 1.0f,1.0f,0.0f,1.0f },	//カラー
		0.0f,						//回転
		0.8f						//拡大
	);
	//ポストレンダラーで描画する
	m_tipsFR->SetPostRenderFlag(true);

	//Tipsの文章を表示するフォントレンダラーの生成と初期化
	m_sentenceFR = NewGO<CFontRender>(1);
	const float sentenceDistX = 20.0f;
	const float sentenceDistY = -50.0f;
	m_sentenceFR->Init(L"", { tipsX + sentenceDistX,tipsY + sentenceDistY });
	//ポストレンダラーで描画する
	m_sentenceFR->SetPostRenderFlag(true);
	m_sentenceFR->SetScale(0.7f);

	//テキストの初期化
	InitText();

}

//デストラクタ
CTips::~CTips()
{
	DeleteGO(m_tipsUISR);
	DeleteGO(m_tipsFR);
	DeleteGO(m_sentenceFR);
}


/// <summary>
/// テキストの初期化
/// </summary>
void CTips::InitText()
{	
	//オブジェクトタイプの数だけ繰り返す
	for (int objectType = 0; objectType < enObjectTypeNum; objectType++)
	{
		//オブジェクトタイプによってロードするテキストを振り分ける
		switch (objectType)
		{
		case enEmpty:	//空っぽ
			LoadText(objectType, "empty");
			break;
		case enWater:	//水
			LoadText(objectType, "water");
			break;
		case enFire:	//火
			LoadText(objectType, "fire");
			break;
		case enRunning:	//稼働
			LoadText(objectType, "running");
			break;
		case enStop:	//停止
			LoadText(objectType, "stop");
			break;
		case enNail:	//釘
			LoadText(objectType, "nail");
			break;
		case enBar:		//バール
			LoadText(objectType, "bar");
			break;
		case enKey:		//鍵
			LoadText(objectType, "key");
			break;
		case enPadlock:	//南京錠
			LoadText(objectType, "padlock");
			break;
		case enLeftType://左
			LoadText(objectType, "left");
			break;
		case enRightType://右
			LoadText(objectType, "right");
			break;
		case enBigFire:		//炎
			LoadText(objectType, "bigFire");
			break;
		case enWall:		//壁
			LoadText(objectType, "wall");
			break;
		case enGoal:		//ゴール
			LoadText(objectType, "goal");
			break;
		case enNotHavePadlock:	//持てない南京錠
			LoadText(objectType, "notHavePadlock");
			break;
		case enBox:			//箱
			LoadText(objectType, "box");
			break;
		case enOneway:		//一方通行
			LoadText(objectType, "oneWay");
			break;
		case enTransparentSwitch:	//一方通行
			LoadText(objectType, "switch");
			break;
		case enReverseAll:	//全反転
			LoadText(objectType, "reverseAll");
			break;
		case enUFOCapture:	//UFOに捕まった
			LoadText(objectType, "UFOCapture");
			break;
		case enUFOLandingPoint:	//UFOに捕まった
			LoadText(objectType, "UFOLandingPoint");
			break;
		default:		//デフォルト
			LoadText(objectType, "empty");
			break;
		}
	}
}


/// <summary>
/// テキストをロードする
/// </summary>
/// <param name="objectType">オブジェクトタイプ</param>
/// <param name="fileName">テキストファイルの名前</param>
void CTips::LoadText(const int objectType, const char* fileName)
{
	//日本語をセット
	setlocale(LC_CTYPE, "ja_JP.UTF-8");
	//setlocale(LC_CTYPE, "ja_JP.SJIS");
	//std::locale("");

	//ファイルパスの頭につける文字列
	const char* headFilePath = "Assets/text/";
	//ファイルパスの後ろにつける文字列
	const char* tailFilePath = ".txt";
	//ファイルパスの文字列
	char FilePath[m_maxTextSize];
	//ないとは思うけど、最大サイズ以上なら切り捨て
	const int maxSize = m_maxTextSize - 1;
	FilePath[maxSize] = 0;

	//ファイルパスが長すぎるときのエラー
	if (sizeof(headFilePath) + sizeof(fileName) + sizeof(tailFilePath) > maxSize)
	{
		MessageBoxA(nullptr, "ファイルパスが長すぎます。", "エラー", MB_OK);
		return;
	}

	//ファイルパスにファイルパスの頭につける文字列をコピー
	strncpy(FilePath, headFilePath, maxSize);
	//ファイルパスにファイルの名前を追加
	strncat(FilePath, fileName, maxSize - sizeof(headFilePath));
	//ファイルパスにファイルパスの後ろにつける文字列を追加
	strncat(FilePath, tailFilePath, maxSize - sizeof(FilePath));

	//ファイルを読み取りモードで開く
	auto fp = fopen(FilePath, "r");

	if (fp == nullptr)
	{
		//ファイルないよ
		const char* cantOpen = "のファイルが開けません。";
		strncat(FilePath, cantOpen,sizeof(FilePath) + sizeof(cantOpen));
		MessageBoxA(nullptr, FilePath, "エラー", MB_OK);
		return;
	}

	//ファイルの文字を入れておく文字
	char c;
	//改行する文字数
	const int newLineNum = 12;
	//改行するか？
	bool newLineFlag = true;

	//ファイルを一文字ずつ読み取る
	//EOFまで読み続ける
	while ((c = fgetc(fp)) != EOF)
	{
		//文字数をカウントする関数は1バイト目で判定してるから、
		//まずバッファに次の文字を入れて、改行するの文字数の一つ多い
		//文字数になったら、m_textに改行を入れてから次の文字を入れる

		//ロードした文字に改行があるか？
		if (c == '\n')
			//あったら、もうこれ以上改行しないようにする
			newLineFlag = false;

		//改行するか？
		if (newLineFlag)
		{
			//改行する

			//バッファ
			char buf[m_maxTextSize];
			//バッファに前フレームまでのロードしたテキストをコピー
			strncpy(buf, m_text[objectType], maxSize);
			//バッファに次の文字を入れる
			strncat(buf, &c, sizeof(char));
			//バッファの文字数が、改行する文字数の一つ多い文字数か？
			if (u8len(buf) % (newLineNum + 1) == 0)
				//多い場合
				//m_textに改行を入れる
				strncat(m_text[objectType], "\n", sizeof(char));
		}

		//ロードした文章を、データメンバに保持しておく
		strncat(m_text[objectType], &c, sizeof(char));

	}

	//ファイルを閉じる
	fclose(fp);
}

//u8len関数（UTF8の文字数をカウントする関数）
int u8len(const char* str)
{
	int cnt = 0;
	while (*str != '\0') {
		cnt++;
		str += u8mb(*str);
	}
	return cnt;
}

//u8mb関数：UTF-8文字の1バイト目を判定して文字のバイト数を返す関数
int u8mb(const char chr)
{
	int byt = 1;
	if ((chr & 0x80) == 0x00) { //1byte文字は何もしない（[byt = 1]のまま）
	}
	else if ((chr & 0xE0) == 0xC0) { //2byte文字
		byt = 2;
	}
	else if ((chr & 0xF0) == 0xE0) { //3byte文字
		byt = 3;
	}
	else if ((chr & 0xF8) == 0xF0) { //4byte文字
		byt = 4;
	}
	else if ((chr & 0xFC) == 0xF8) { //5byte文字
		byt = 5;
	}
	else if ((chr & 0xFE) == 0xFC) { //6byte文字
		byt = 6;
	}
	return byt;
}



/// <summary>
/// ロード済みのオブジェクトタイプのテキストをセットする
/// </summary>
/// <param name="textType">オブジェクトタイプ</param>
void CTips::SetText(const int textType)
{
	//フォントレンダラーにセットするテキストはwchar_t型だらか
	//char型のデータメンバを変換する

	//wchar_t型のテキスト
	wchar_t text[m_maxTextSize];
	//char型のデータメンバをwchar_t型に変換する

	if (textType != enWater)
		int a = 1;
	
	std::string str = m_text[textType];
	mbstowcs(text, str.c_str(), m_maxTextSize);

	//mbstowcs(text, m_text[textType], m_maxTextSize);
	//テキストをフォントレンダラーにセット
	m_sentenceFR->SetText(text);
}

/// <summary>
/// テキストを何にも表示しない
/// </summary>
void CTips::ClearText()
{
	//空っぽのテキストをセット
	SetText(enEmpty);
}

void CTips::SetTitleMode()
{
	m_tipsUISR->Deactivate();
	m_tipsFR->Deactivate();
	m_sentenceFR->Deactivate();
}