#pragma once
#include "physics/PhysicsStaticObject.h"
#include "ModelRender.h"


struct LevelObjectData;
class CMapChip
{
public://メンバ関数
	/// <summary>
	/// マップチップの作成
	/// レベルのデータをもとに
	/// モデルの表示と静的物理オブジェクトの初期化
	/// </summary>
	/// <param name="objData">レベルのオブジェクトのデータ</param>
	CMapChip(const LevelObjectData& objData);
	~CMapChip();

private://データメンバ
	PhysicsStaticObject m_physicsStaticObject;			//!<静的物理オブジェクト。
	CModelRender* m_modelRender = nullptr;				//モデルレンダラー
};

