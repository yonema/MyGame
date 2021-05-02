#include "stdafx.h"
#include "LevelObjectManager.h"
#include "LevelObjectBase.h"
#include "ReversibleObject.h"

//インスタンスの初期化
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

//コンストラクタ
CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
}

//デストラクタ
CLevelObjectManager::~CLevelObjectManager()
{
	//nullptrを入れておく
	m_instance = nullptr;
}

/// <summary>
/// ウェイポイントの「場所」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="posMap">場所のマップ</param>
void CLevelObjectManager::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vectorのサイズの確保
	m_wayPointPos.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_vecSize = vecSize;
	//m_wayPointPosにウェイポイントの「場所」を格納する
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}
}


/// <summary>
/// ウェイポイントの「回転」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="rotMap">回転のマップ</param>
void CLevelObjectManager::InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap)
{
	//vectorのサイズの確保
	m_wayPointRot.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_vecSize = vecSize;
	//m_wayPointRotにウェイポイントの「回転」を格納する
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}
}


/// <summary>
/// ウェイポイント上での移動先を計算する関数
/// </summary>
/// <param name="rpIndex">現在の右側のウェイポイントのインデックス</param>
/// <param name="pos">現在の座標</param>
/// <param name="dist">移動する距離</param>
/// <param name="leftOrRight">右側に移動するか左側い移動するか。0:左,1:右</param>
/// <returns>移動先の座標</returns>
const Vector3 CLevelObjectManager::CalcWayPointNextPos
(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex)
{
	int lpIndex = rpIndex + 1;
	const int maxWayPointIndex = m_vecSize - 1;
	if (lpIndex > maxWayPointIndex)
	{
		lpIndex = 0;
	}

	Vector3 addVec = g_vec3Zero;
	int nextIndex = 0;
	if (leftOrRight)
	{
		//右に飛ばす
		nextIndex = rpIndex;
	}
	else
	{
		//左に飛ばす
		nextIndex = lpIndex;
	}
	Vector3 originPos = pos;
	addVec = m_wayPointPos[nextIndex] - originPos;


	float addLen = addVec.Length();
	float rDist = dist;
	if (addLen <= rDist)
	{
		rDist -= addLen;
		int otherIndex = nextIndex;
		if (leftOrRight)
		{
			//左に飛ばす
			nextIndex--;
			if (nextIndex < 0)
				nextIndex = maxWayPointIndex;
		}
		else
		{
			//右に飛ばす
			nextIndex++;
			if (nextIndex > maxWayPointIndex)
				nextIndex = 0;
		}
		originPos = m_wayPointPos[otherIndex];
		addVec = m_wayPointPos[nextIndex] - originPos;
		addLen = addVec.Length();
	}
	addVec.Normalize();
	addVec.Scale(rDist);
	if (pNextIndex)
		*pNextIndex = nextIndex;
	return originPos + addVec;
}



/// <summary>
/// 自身（LevelObjectManager）にオブジェクトを追加する
/// </summary>
/// <param name="object">追加するオブジェクト</param>
void CLevelObjectManager::AddObject(ILevelObjectBase* object)
{
	m_levelObjects.push_back(object);
}

/// <summary>
/// 自身（LevelObjectManager）からオブジェクトを破棄する
/// </summary>
/// <param name="object">破棄するオブジェクト</param>
void CLevelObjectManager::RemoveObject(ILevelObjectBase* object)
{
	//一度に一個しか消せないよ
	std::vector<ILevelObjectBase*>::iterator itr;
	for (itr = m_levelObjects.begin(); itr != m_levelObjects.end(); itr++)
	{
		if ((*itr)->GetIsDead())
		{
			m_levelObjects.erase(itr);
			break;
		}
	}
}

/// <summary>
/// レベルオブジェクトをすべて消去する
/// </summary>
void CLevelObjectManager::AllDeleteLOs()
{
	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		DeleteGO(m_levelObjects[i]);
	}
}


bool CLevelObjectManager::QueryLevelAllObjects(ILevelObjectBase& thisObject, const int objectType)
{
	for (auto lo : m_levelObjects) {
		//自分自身じゃないとき
		if (lo->GetObjectType() != objectType) {
			//重なっているオブジェクトがある。
			if (IsHitObject(thisObject, *lo)) {
				return true;
			}
		}
	}
	return false;
}

const int CLevelObjectManager::GetNearestObjectType()
{
	//プレイヤーが何か持っていたら、
	if (m_player->GetHoldObject())
		return m_player->GetReversibleObject()->GetObjectType();

	const int maxWayPoint = m_vecSize - 1;
	int wayPoint = m_player->GetLeftPointIndex();
	int leftWayPoint = wayPoint + 1;
	if (leftWayPoint > maxWayPoint)
		leftWayPoint = 0;
	int rightWayPoint = wayPoint - 1;
	if (rightWayPoint < 0)
		rightWayPoint = maxWayPoint;

	const float threshold = 300.0f;
	float dist = threshold;

	int objectType = enEmpty;

	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		int objectsWayPoint = m_levelObjects[i]->GetLeftWayPointIndex();
		if (objectsWayPoint == leftWayPoint || objectsWayPoint == wayPoint ||
			objectsWayPoint == rightWayPoint)
		{
			Vector3 playerToObject = m_levelObjects[i]->GetPosition() - m_player->GetPosition();
			float length = playerToObject.Length();
			if (length <= dist)
			{
				objectType = m_levelObjects[i]->GetObjectType();
				dist = length;
			}
		}
		else
		{
			continue;
		}
	}
	
	return objectType;

}
