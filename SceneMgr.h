#pragma once
typedef enum {
	eScene_Menu,
	eScene_Standby,
	eScene_Game,
	eScene_Gameover,
	eScene_None,
} eScene ;

void SceneMgr_Initialize();
void SceneMgr_Finalize();
void SceneMgr_Update();
void SceneMgr_Draw();
void SceneMgr_ChangeScene(eScene nextScene);