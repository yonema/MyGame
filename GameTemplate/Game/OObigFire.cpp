#include "stdafx.h"
#include "OObigFire.h"


bool OObigFire::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/flame.tkm", enBigFire);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,100.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	//ポイントライトを生成
	m_pointLight = NewGO<CPointLight>(0);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Scale(300.0f);
	ptLigPos += up;
	m_pointLight->SetPosition(ptLigPos);
	m_pointLight->SetColor({ 500.0f,1.0f,1.0f,1.0f });
	m_pointLight->SetRange(500.0f);

	//自己発光色を設定
	SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });

	return true;
}

OObigFire::~OObigFire()
{
	DeleteGO(m_pointLight);
}


void OObigFire::Damage()
{
	if (--m_hp <= 0)
	{
		//HPが0以下だったら、このオブジェクトを消去
		DeleteGO(this);
	}
	else
	{
		//OBBのサイズをHPに応じて小さくする
		Vector3 obbSize;
		obbSize = { 300.0f,300.0f,100.0f };
		//でかい火の大きさ補正
		//大きさを1/3にすると、小さすぎてゴールできちゃうから、
		//大きさを5/5→4/5→3/5→0と変化させる
		const int sizeComplement = 3;

			//m_hpとm_maxHpはint型。
			//int型同士の割り算だから小数が切り捨てられて
			//評価した値が0になってしまう。
			//だから、m_hpをfloatにキャストする
		obbSize.Scale(static_cast<float>(m_hp + sizeComplement) / (m_maxHp + sizeComplement));
		GetOBB().SetDirectionLength(obbSize);


		//モデルのサイズをHPに応じて小さくする
			//これが上のみたいに切り捨てにならない理由は
			//m_hp / m_maxHpより先にg_vec3One * m_hpが呼ばれ
			//Vector3の演算子のオーバーライドの引数がfloat型だから
			//m_hpが渡されるときに強制型変換が起きてfloat型になるから
			//これがSetScale(g_vec3One * (m_hp / m_maxHp))
			//だったらうまくいかない。
		SetScale(g_vec3One * (m_hp + sizeComplement) / (m_maxHp + sizeComplement));
	}
}