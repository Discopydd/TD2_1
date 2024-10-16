#include <Novice.h>
#include "Player.h"
#include "MapChip.h"
const char kWindowTitle[] = "4757";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1980,1080);


	Player player;
	MapChip mapChip;

	 mapChip.LoadMap("NoviceResources/map.csv");

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		player.Update(mapChip);

		mapChip.UpdatePlatforms(); 

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		 player.Draw();
		  mapChip.DrawPlatforms(); 
		 mapChip.DrawMap();
		/// 
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
