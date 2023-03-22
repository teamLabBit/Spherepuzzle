#include "Standby.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;

void Standby_Initialize() {
	mImageHandle = LoadGraph("images/Standby.png");
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
}