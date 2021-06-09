#pragma once
#include "StageBase.h"

/**
 * @brief タイトルステージクラス
*/
class CStage_title : public IStageBase
{
public:
	bool StartSub() override final;
	~CStage_title();
	void RetryStage()override final
	{
		NewGO<CStage_title>(0);
	}


private:	//データメンバ
};

