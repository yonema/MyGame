#pragma once
#include "Player.h"

class GameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	Player* m_pPlayer = nullptr;	//プレイヤーのポインタ
	Vector3 m_toCameraPos;				//注視点から視点へのベクトル
};

