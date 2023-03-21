#pragma once
#include <time.h>
#include <vector>
using namespace std;

void Game_Initialize();
void Game_Finalize();
void Game_Update();
void Game_Draw(vector<vector<int>>, long long, int, int, int, clock_t);
void initRand();
int sphere_Gene();
bool is_gameover(vector<vector<int>>);
int CheckCombine(vector<vector<int>>, int, int, int, int);
void gameover_disp(long long);