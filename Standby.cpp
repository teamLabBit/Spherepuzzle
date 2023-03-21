#include "Standby.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;

void Standby_Initialize() {
	mImageHandle = LoadGraph("images/Scene_Game.png");
}

void Standby_Finalize() {
	DeleteGraph(mImageHandle);
}

void Standby_Update() {
	if (GetAsyncKeyState(VK_SPACE) & 0x01) {
		SceneMgr_ChangeScene(eScene_Game);
	}
}

void Standby_Draw() {
	DrawGraph(0, 0, mImageHandle, FALSE);
	DrawString(0, 0, "スタンバイ画面です。", GetColor(255, 255, 255));
	DrawString(0, 20, "SPACEキーを押すとゲーム画面に進みます。", GetColor(255, 255, 255));
}