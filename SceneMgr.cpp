#include "DxLib.h"
#include "Standby.h"
#include "Game.h"
#include "Menu.h"
#include "SceneMgr.h"

static eScene mScene = eScene_Menu;
static eScene mNextScene = eScene_Menu;

static void SceneMgr_InitializeModule(eScene scene);
static void SceneMgr_FinalizeModule(eScene scene);

void SceneMgr_Initialize() {
	SceneMgr_InitializeModule(mScene);
}

void SceneMgr_Finalize() {
	SceneMgr_FinalizeModule(mScene);
}

void SceneMgr_Update() {
	if (mNextScene != eScene_None) {
		SceneMgr_FinalizeModule(mScene);
		mScene = mNextScene;
		mNextScene = eScene_None;
		SceneMgr_InitializeModule(mScene);
	}
	switch (mScene) {
	case eScene_Menu:
		Menu_Update();
		break;
	case eScene_Standby:
		Standby_Update();
		break;
	case eScene_Game:
		Game_Update();
		break;
	}
}

void SceneMgr_Draw() {
	switch (mScene) {
	case eScene_Menu:
		Menu_Draw();
		break;
	case eScene_Standby:
		Standby_Draw();
		break;
	case eScene_Game:
		break;
	}
}

void SceneMgr_ChangeScene(eScene NextScene) {
	mNextScene = NextScene;
}

static void SceneMgr_InitializeModule(eScene scene) {
	switch (scene) {
	case eScene_Menu:
		Menu_Initialize();
		break;
	case eScene_Standby:
		Standby_Initialize();
		break;
	case eScene_Game:
		Game_Initialize();
		break;
	}
}

static void SceneMgr_FinalizeModule(eScene scene) {
	switch (scene) {
	case eScene_Menu:
		Menu_Finalize();
		break;
	case eScene_Standby:
		Standby_Finalize();
		break;
	case eScene_Game:
		Game_Finalize();
		break;
	}
}

bool GameEnd_Check() {
	if (mNextScene == eScene_End) {
		return true;
	}
	return false;
}