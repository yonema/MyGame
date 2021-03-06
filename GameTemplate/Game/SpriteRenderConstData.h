#pragma once

/**
 * @brief スプライト描画クラスの定数データ
*/
namespace spriteRenderConstData
{
	const Vector2 SPRITE_PIVOT_DEFALUT = { 0.5f,0.5f };			//デフォルトのピボット
	const Vector3 SPRITE_SCALE_DEFAULT = { -1.0f,1.0f,1.0f };	//デフォルトの拡大率
	constexpr float FLIP_HORIZONTAL = -1.0f;						//左右反転の値

	constexpr float ALPHA_VALUE_TRANSPARENT = 0.0f;					//透明のアルファ値
	constexpr float ALPHA_VALUE_OPACITY = 1.0f;						//不透明のアルファ値

	//シェーダーのデフォルトのファイルパス
	constexpr const char* const SHADER_FILEPATH_DEFAULT = "Assets/shader/sprite.fx";
	//ピクセルシェーダ―のデフォルトのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_PS_DEFAULT = "PSMain";
	//頂点シェーダーのデフォルトのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_VS_DEFAULT = "VSMain";
}