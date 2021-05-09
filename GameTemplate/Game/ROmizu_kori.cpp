#include "stdafx.h"
#include "ROmizu_kori.h"

//スタート関数
bool ROmizu_kori::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/water.tkm", enWater,
		"Assets/modelData/fire.tkm", enFire);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,400.0f,100.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROmizu_kori::QuerySub()
{
	//自身が「水」の時
	if (GetObjectType() == enWater)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//自身と「でかい火」が衝突したら
				if (IsHitObject(*this, *bigFire))
				{
					//「でかい火」にダメージを与える
					bigFire->Damage();
					//自身は破棄する
					DeleteGO(this);
				}
				//trueを戻す
				return true;
			}
		);
		//反転オブジェクトの「火」をクエリ
		QueryLOs<ROmizu_kori>(enFire, [&](ROmizu_kori* Fire) -> bool
			{
				//自身と「火」が衝突したら
				if (IsHitObject(*this, *Fire))
				{
					if (m_actionFlag) {
						//相手を破棄する。
						DeleteGO(Fire);
						//自身は破棄する
						DeleteGO(this);
						//行動できなくする
						m_actionFlag = false;
					}
				}
				//trueを戻す
				return true;
			}
		);
	}
	//自身が「火」の時
	else if (GetObjectType() == enFire)
	{
		//反転オブジェクトの「水」をクエリ
		QueryLOs<ROmizu_kori>(enWater, [&](ROmizu_kori* Water) -> bool
			{
				//自身と「水」が衝突したら
				if (IsHitObject(*this, *Water))
				{
					if (m_actionFlag) {
						//相手を破棄する。
						DeleteGO(Water);
						//自身は破棄する
						DeleteGO(this);
						//行動できなくする
						m_actionFlag = false;
					}
				}
				//trueを戻す
				return true;
			}
		);
	}
}