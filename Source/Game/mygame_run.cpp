#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <string>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	background.LoadBitmapByString({
		"resources/map/home_2f.bmp",
		"resources/map/home_1f.bmp",
		"resources/map/town.bmp",
		"resources/map/pokemon.bmp",
		"resources/map/gym1.bmp",
		"resources/map/road.bmp"
		});//,RGB(100,100,100));
	background.SetTopLeft(235, 170);
	/*background.SetTopLeft(-215, -1885);*/
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);	
	textbox.LoadBitmapByString({ "Resources/text.bmp" });
	textbox.SetTopLeft(0, 378);
	menu.LoadBitmapByString({ 
		"Resources/menu.bmp" ,
		"Resources/white.bmp"
		});
	menu.SetTopLeft(400, 40);
	arrow.LoadBitmapByString({ "Resources/arrow.bmp"});
	ifstream ifs1("resources/files/character.txt");
		for (int i = 0; i < 97; i++) {
			ifs1 >> characterinf[i];
		}
	ifstream ifs2("resources/files/pokemonlib.txt");
		for (int i = 0; i < 67; i++) {
			for (int j = 0; j < 13; j++) {
				ifs2 >> pokemoninf[j][i];
			}
		}
	ifstream ifs3("resources/files/pokemonname.txt");
		for (int i = 0; i < 67; i++) {
			ifs3 >> pokemonname[i];
		}
	ifstream ifs("Resources/mapdoc/home2f.txt");
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			int a = 0;
			ifs >> a;
			home1fmap.push_back(a);
		}
	}
	ifs.close();							//讀地圖txt進來
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			int a = 0;
			a = home1fmap.front();
			home1fmap.erase(home1fmap.begin());
			if (a == 1) {
				hitbox.insert(hitbox.begin(), CMovingBitmap());
				hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
				hitbox.begin()->SetTopLeft(203 + j * 32, 170 + i * 32);
			}
			else if (a == 2) {
				tppoint.insert(tppoint.begin(), CMovingBitmap());
				tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
				tppoint.begin()->SetTopLeft(203 + j * 32, 170 + i * 32);
			}
		}
	}
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{	
	int nowtop = character.GetTop();
	int nowleft = character.GetLeft();	
	if (nownum > 3){
		nownum = 0;
	}
	nownum++;
	overlapleft = 0;
	overlapright = 0;
	overlaptop = 0;
	overlapdown = 0;
	if (bag == false && shopnum == 0) {
		for (auto i = hitbox.begin(); i != hitbox.end(); i++) { 
			if (CMovingBitmap::IsOverlap(character, *i)) {
				if (character.GetLeft() > i->GetLeft()) {
					overlapleft = 1;
				}
				else if (character.GetLeft() < i->GetLeft()) {     
					overlapright = 1;
				}
				if (character.GetTop() < i->GetTop()) {
					overlaptop = 1;
				}
				else if (character.GetTop() > i->GetTop()) {
					overlapdown = 1;
				}
			}
		}
		//判定卡hitbox
		if (overlapleft && speedX > 0) {
			speedX = 0;//background.SetTopLeft(background.GetLeft(), background.GetTop() + speedY);
		}
		else if (overlapright && speedX < 0) {
			speedX = 0;// background.SetTopLeft(background.GetLeft(), background.GetTop() + speedY);
		}
		if (overlaptop && speedY < 0) {
			speedY = 0; //background.SetTopLeft(background.GetLeft() + speedX, background.GetTop());
		}
		else if (overlapdown && speedY > 0) {
			speedY = 0; //background.SetTopLeft(background.GetLeft() + speedX, background.GetTop());
		}
		background.SetTopLeft(background.GetLeft() + speedX, background.GetTop() + speedY);    //舊的移動角色
		for (auto i = hitbox.begin(); i != hitbox.end(); i++) {                                 //新的移動物件
			i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
		}
		for (auto i = tppoint.begin(); i != tppoint.end(); i++) {
			i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
		}
		for (auto i = grass.begin(); i != grass.end(); i++) {
			i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
		}
		for (auto i = dialog.begin(); i != dialog.end(); i++) {
			i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
		}
		for (auto i = sign.begin(); i != sign.end(); i++) {
			i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
		}
		if (phase == 5) {
			boss[0].SetTopLeft(boss[0].GetLeft() + speedX, boss[0].GetTop() + speedY);
		}
		//物件移動
/*for (int i = 0; i < hitbox.size(); i++) {
	hitbox[i].SetTopLeft(hitbox[i].GetLeft() + speedX, hitbox[i].GetTop() + speedY);         //舊的移動物件
}
for (int i = 0; i < tppointnum; i++) {
	tppoint[i].SetTopLeft(tppoint[i].GetLeft() + speedX, tppoint[i].GetTop() + speedY);
}*/
		int frame = character.GetFrameIndexOfBitmap();
		if (speedX != 0 || speedY != 0) {
			if (characterFrameCounter == 4) {
				if (speedY < 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 1);
				}
				else if (speedY > 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap() + 1) % 2 + 6);
				}
				else if (speedX < 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap() + 1) % 2 + 8);
				}
				else if (speedX > 0) {
					character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 3);
				}
				characterFrameCounter = 0;
			}
			else {
				characterFrameCounter += 1;
			}
		}   //背景地圖移動
		else
		{
			if (frame == 1 || frame == 2) {
				character.SetFrameIndexOfBitmap(0);
			}
			else if (frame == 3 || frame == 4) {
				character.SetFrameIndexOfBitmap(3);
			}
			else if (frame == 6 || frame == 7) {
				character.SetFrameIndexOfBitmap(5);
			}
			else if (frame == 8 || frame == 9) {
				character.SetFrameIndexOfBitmap(8);
			}
		}//
		//角色移動動畫
		//角色停止畫面
		if (phase == 1) {
			if (CMovingBitmap::IsOverlap(character, tppoint[1])) {
				phase = 2;
				background.SetFrameIndexOfBitmap(0);
				background.SetTopLeft(170, 170);
				character.SetFrameIndexOfBitmap(3);
				hitbox.clear();
				tppoint.clear();
				ifstream ifs("Resources/mapdoc/home2f.txt");
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 8; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 8; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(138 + j * 32, 170 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(138 + j * 32, 170 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-245, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-277 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-277 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-277 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/sign.bmp" });
							sign.begin()->SetTopLeft(-277 + j * 32, -147 + i * 32);
						}
					}
				}
			}
		}
		else if (phase == 2) {
			if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(140, 205);
				character.SetFrameIndexOfBitmap(3);
				hitbox.clear();
				tppoint.clear();
				ifstream ifs("Resources/mapdoc/home1f.txt");
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(108 + j * 32, 205 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(108 + j * 32, 205 + i * 32);
						}
					}
				}
			}
		}
		else if (phase == 3) {
			if (CMovingBitmap::IsOverlap(character, tppoint[5])) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(205, 80);
				character.SetFrameIndexOfBitmap(5);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/home1f.txt");
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(173 + j * 32, 80 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(173 + j * 32, 80 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[4])) {
				phase = 4;
				background.SetFrameIndexOfBitmap(3);
				background.SetTopLeft(105, 50);
				character.SetFrameIndexOfBitmap(5);	
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/pokemon.txt");
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 14; j++) {
						int a = 1;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 14; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(73 + j * 32, 50 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(73 + j * 32, 50 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/sign.bmp" });
							sign.begin()->SetTopLeft(73 + j * 32, 50 + i * 32);
						}
						else if (a == 5) {
							dialog.insert(dialog.begin(), CMovingBitmap());
							dialog.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							dialog.begin()->SetTopLeft(73 + j * 32, 50 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[6])) {
				phase = 5;
				background.SetFrameIndexOfBitmap(4);
				background.SetTopLeft(230,144);
				character.SetFrameIndexOfBitmap(5);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/gym1.txt");
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 1;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(198 + j * 32, 112 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(198 + j * 32, 112 + i * 32);
						}
					}
				}
				boss[0].LoadBitmapByString({ "Resources/boss/boss1.bmp " }, RGB(100, 100, 100));
				boss[0].SetTopLeft(285, 176);
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[7])) {
				phase = 6;
				background.SetFrameIndexOfBitmap(5);
				background.SetTopLeft(-760, -650);
				character.SetFrameIndexOfBitmap(8);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/road.txt");
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-792 + j * 32, -682 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-792 + j * 32, -682 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-792 + j * 32, -682 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/sign.bmp" });
							sign.begin()->SetTopLeft(-792 + j * 32, -682 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[8])) {
				phase = 6;
				background.SetFrameIndexOfBitmap(5);
				background.SetTopLeft(-728, -650);
				character.SetFrameIndexOfBitmap(8);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/road.txt");
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-760 + j * 32, -682 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-760 + j * 32, -682 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-760 + j * 32, -682 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/sign.bmp" });
							sign.begin()->SetTopLeft(-760 + j * 32, -682 + i * 32);
						}
					}
				}
				}
			else if (CMovingBitmap::IsOverlap(character, tppoint[9])) {
				phase = 6;
				background.SetFrameIndexOfBitmap(5);
				background.SetTopLeft(-696, -650);
				character.SetFrameIndexOfBitmap(8);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/road.txt");
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 32; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-728 + j * 32, -682 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-728 + j * 32, -682 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-728 + j * 32, -682 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/sign.bmp" });
							sign.begin()->SetTopLeft(-728 + j * 32, -682 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, sign[0])) {
				signnum = 2;
			}
			else if (CMovingBitmap::IsOverlap(character, sign[1])) {
				signnum = 3;
			}
			else {
				signnum = 0;
			}
		}
		else if (phase == 4) {
			if (CMovingBitmap::IsOverlap(character, tppoint[1])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-848, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-880 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-880 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-880 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-880 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, sign[0])) {
				signnum = 1;
			}
			else {
				signnum = 0;
			}
		}
		else if (phase == 5) {
			if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-1080, 140);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-1112 + j * 32, 108 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-1112 + j * 32, 108 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-1112 + j * 32, 108 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-1112 + j * 32, 108+ i * 32);
						}
					}
				}
			}
		}
		else if (phase == 6) {
			if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[1])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[2])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[3])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[4])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-816, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				sign.clear();
				ifstream ifs("Resources/mapdoc/town.txt");
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 24; i++) {
					for (int j = 0; j < 52; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/tp.bmp" });
							tppoint.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
						else if (a == 4) {
							sign.insert(sign.begin(), CMovingBitmap());
							sign.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							sign.begin()->SetTopLeft(-848 + j * 32, -147 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, sign[0])) {
				signnum = 4;
			}
			else if (CMovingBitmap::IsOverlap(character, sign[1])) {
				signnum = 5;
			}
			else {
				signnum = 0;
			}
			}
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_ESCAPE) {	//暫停並顯示箭頭
		if (shopnum == 0) {
			bag = !bag;
			arrow.SetTopLeft(430, 75);
			arrownum = 1;
			menu.SetFrameIndexOfBitmap(0);
			menu.SetTopLeft(400, 30);
			propnum = 0;
		}
		// stop the game
		else if (shopnum)
		{
			shopnum = 0;
			arrownum = 0;
		}
		// turn off shop
		else {
			shopnum = 0;
			arrownum = 0;
		}
		//close the window
		if (team) {
			shopnum = 0;
			team = false;
			arrow.SetTopLeft(430, 75);
			arrownum = 0;
			menu.SetFrameIndexOfBitmap(0);
			menu.SetTopLeft(400, 40);
		}
		//team?
	}
	if (nChar == VK_RETURN && confirmenter == false) {
		if (shopnum == 0) {
			confirmenter = true;
		}
		if (bag == true) {
			int arrow_y = arrow.GetTop();
			if (arrow_y == 255) {
				exit(0);
			}
			//離開功能
			else if (arrow_y == 195) {
				ofstream ofs("resources/files/character.txt");
				for (int i = 0; i < 97; i++) {
					ofs << characterinf[i];
					ofs << " ";
				}
				
				ofs.close();
			}
			//存檔
			else if (arrow_y == 135) {
				propnum = 1;
				arrow.SetTopLeft(100, 415);
				arrownum = 2;
			}
			//查看背包
			else if (arrow_y == 75) {
				team = true;
				arrow.SetTopLeft(30, 30);
				arrownum = 3;//暫定隊伍
				menu.SetFrameIndexOfBitmap(1);
				menu.SetTopLeft(0, 0);
			}
			//查看隊伍
		}
		//stop function
		
		if (phase == 4 && bag == false) {
			if (CMovingBitmap::IsOverlap(character, dialog[1]) && shopnum == 0) {
				shopnum = 1;
				arrow.SetTopLeft(100, 415);
				arrownum = 2;
			}
			//into shop
			else if (CMovingBitmap::IsOverlap(character, dialog[0])) {
				
			}
			//into pokemon
		}
		//into shop and pokemon
		if (shopnum == 1 && confirmenter == false) {
			if (arrow.GetTop() == 415) {
				shopnum = 2;
			}
			//innto buy
			else if (arrow.GetTop() == 465) {
				
			}
			//into sell
			else {
				shopnum = 0;
				arrownum = 0;
			}
			//exit shop
		}
		//in shop
		if (shopnum == 2) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[1]++;
			}
			else if (arrow.GetTop() == 465) {
				characterinf[0] = characterinf[0] - 300;
				characterinf[2]++;
			}
			else {
				characterinf[0] = characterinf[0] - 500;
				characterinf[3]++;
			}
		}
		else if (shopnum == 3) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[4]++;
			}
			else if (arrow.GetTop() == 465) {
				characterinf[0] = characterinf[0] - 300;
				characterinf[5]++;
			}
			else {
				characterinf[0] = characterinf[0] - 500;
				characterinf[6]++;
			}
		}
		else if (shopnum == 4) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[7]++;
			}
			else if (arrow.GetTop() == 465) {
				characterinf[0] = characterinf[0] - 300;
				characterinf[8]++;
			}
			else {
				characterinf[0] = characterinf[0] - 500;
				characterinf[9]++;
			}
		}
		else if (shopnum == 5) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[10]++;
			}
			else if (arrow.GetTop() == 465) {
				characterinf[0] = characterinf[0] - 300;
				characterinf[11]++;
			}
			else {
				characterinf[0] = characterinf[0] - 500;
				characterinf[12]++;
			}
		}
		else if (shopnum == 6) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[13]++;
			}
			else if (arrow.GetTop() == 465) {
				characterinf[0] = characterinf[0] - 300;
				characterinf[14]++;
			}
			else {
				characterinf[0] = characterinf[0] - 500;
				characterinf[15]++;
			}
		}
		if (shopnum == 7) {
			if (arrow.GetTop() == 415) {
				characterinf[0] = characterinf[0] - 100;
				characterinf[16]++;
			}
		}
		//buy
	}

	if (nChar == VK_UP)
	{
		if (bag == false && shopnum == 0 ) {
			speedY = 8;
			speedX = 0;
		}
		//walk
		if (propnum > 1 && arrow.GetTop() == 415 && shopnum == 0) {
			propnum--;
			arrow.SetTopLeft(100, 565);
		}
		//prop page up	多五十補正
		
		if (shopnum > 2 && arrow.GetTop() == 415 && propnum == 0) {
			shopnum--;
			arrow.SetTopLeft(100, 565);
		}
		//shop page up	多五十補正
		//隨著對話增加要增加篩選條件
		if (arrownum == 1) {
			if (arrow.GetTop() != 75 && propnum == 0)
				arrow.SetTopLeft(430, arrow.GetTop() - 60);
		}
		// 右上四項暫停箭頭
		else if (arrownum == 2) {
			if (arrow.GetTop() != 415)
				arrow.SetTopLeft(100, arrow.GetTop() - 50);
		}
		// 下方三項箭頭
		
		//if (team) {
		//	if (arrow.GetTop() != 30)
		//		arrow.SetTopLeft(30, arrow.GetTop() - 60);
		//}
		////team?
	}
	//記得補正
	if (nChar == VK_DOWN )
	{
		if (bag == false && shopnum == 0) {
			speedY = -8;
			speedX = 0;
		}
		//walk
		if (propnum < 6 && arrow.GetTop() == 515 && shopnum == 0) {
			propnum++;
			arrow.SetTopLeft(100, 365);
		}
		//prop page down  少五十補正 

		if (shopnum < 6 && arrow.GetTop() == 515 && propnum == 0) {
			shopnum++;
			arrow.SetTopLeft(100, 365);
		}
		//shop page down  少五十補正
		//隨著對話增加要增加篩選條件
		if (arrownum == 1) {
			if (arrow.GetTop() != 255 && propnum == 0)
				arrow.SetTopLeft(430, arrow.GetTop() + 60);
		}
		// 右上四項暫停箭頭
		else if (arrownum == 2) {
			if (arrow.GetTop() != 515)
				arrow.SetTopLeft(100, arrow.GetTop() + 50);
		}
		// 下方三項箭頭
		
	}
	//記得補正
	if (nChar == VK_LEFT)
	{
		speedX = 8;
		speedY = 0;
		/*if (nChar == VK_UP) {
			switch (nownum) {
				case 1:
					speedY = -8;
					break;
				case 2:
					speedY = -16;
					break;
				case 3:
					speedY = 8;
					break;
				case 4:
					speedY = 16;
					break;

			}
		}*/
		// try to finish walking bug
	}
	if (nChar == VK_RIGHT)
	{
			speedX = -8;
			speedY = 0;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN) {
		confirmenter = false;
	}
	if (nChar == VK_UP)
	{
		speedY = 0;
	}
	if (nChar == VK_DOWN)
	{
		speedY = 0;
	}
	if (nChar == VK_LEFT)
	{
		speedX = 0;

	}
	if (nChar == VK_RIGHT)
	{
		speedX = 0;
	}
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	show_image_by_phase();
	show_text_by_phase();
}

void CGameStateRun::show_image_by_phase() {
	/*if (phase = 1) {
		
		background.SetFrameIndexOfBitmap(0);
		background.ShowBitmap();
	}
	else if (phase = 2) {
		
		
	}*/
	background.ShowBitmap();
	character.ShowBitmap();
	if (phase == 5) {
		boss[0].ShowBitmap();
	}
	if (shopnum || propnum) {
		textbox.ShowBitmap();
		arrow.ShowBitmap();	
	}
	if (signnum) {
		textbox.ShowBitmap();
	}
	if (bag && !team) {
		menu.ShowBitmap();
		arrow.ShowBitmap();
	}
	else if (team) {
		menu.ShowBitmap();
	}
	for (auto i = hitbox.begin(); i != hitbox.end(); i++) {
		i->ShowBitmap();
	}
	for (auto i = tppoint.begin(); i != tppoint.end(); i++) {
		i->ShowBitmap();
	}
	for (auto i = grass.begin(); i != grass.end(); i++) {
		i->ShowBitmap();
	}
	for (auto i = dialog.begin(); i != dialog.end(); i++) {
		i->ShowBitmap();
	}	
	for (auto i = sign.begin(); i != sign.end(); i++) {
		i->ShowBitmap();
	}
}

void CGameStateRun::show_text_by_phase() {

	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());
	string moneyout = to_string(characterinf[0]);
	string pokemon[6] ;
	for (int i = 0; i < 6; i++) {
		pokemon[i].append(to_string(19 + 13 * i));
		pokemon[i].append(to_string(20 + 13 * i));
		pokemon[i].append(to_string(21 + 13 * i));
		pokemon[i].append(to_string(22 + 13 * i));
		pokemon[i].append(to_string(23 + 13 * i));
		pokemon[i].append(to_string(24 + 13 * i));
		pokemon[i].append(to_string(25 + 13 * i));
		pokemon[i].append(to_string(26 + 13 * i));
		pokemon[i].append(to_string(27 + 13 * i));
		pokemon[i].append(to_string(28 + 13 * i));
		pokemon[i].append(to_string(29 + 13 * i));
		pokemon[i].append(to_string(30 + 13 * i));
		pokemon[i].append(to_string(31 + 13 * i));
	}
	
	

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);
	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 255, 51), 800);
	//CTextDraw::Print(pDC, 535, 0, "$");
	CTextDraw::Print(pDC, 300, 0, moneyout);//550
	if (bag == true) {				//選單文字
		if (shopnum == 0 && team == false) {
			CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 800);
			CTextDraw::Print(pDC, 450, 60, "隊伍");
			CTextDraw::Print(pDC, 450, 120, "背包");
			CTextDraw::Print(pDC, 450, 180, "存檔");
			CTextDraw::Print(pDC, 450, 240, "離開");
		}
		if (propnum == 1) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "代幣");
			CTextDraw::Print(pDC, 400, 400, moneyout);
			CTextDraw::Print(pDC, 150, 450, "精靈球");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[1]));
			CTextDraw::Print(pDC, 150, 500, "超級球");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[2]));
		}
		else if (propnum == 2) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "高級球");
			CTextDraw::Print(pDC, 400, 400, to_string(characterinf[3]));
			CTextDraw::Print(pDC, 150, 450, "大師球");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[4]));
			CTextDraw::Print(pDC, 150, 500, "神奇糖果");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[5]));
		}
		else if (propnum == 3) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "傷藥");
			CTextDraw::Print(pDC, 400, 400, to_string(characterinf[6]));
			CTextDraw::Print(pDC, 150, 450, "好傷藥");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[7]));
			CTextDraw::Print(pDC, 150, 500, "厲害傷藥");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[8]));
		}
		else if (propnum == 4) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "全滿藥");
			CTextDraw::Print(pDC, 400, 400, to_string(characterinf[9]));
			CTextDraw::Print(pDC, 150, 450, "解毒藥");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[10]));
			CTextDraw::Print(pDC, 150, 500, "灼傷藥");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[11]));
		}
		else if (propnum == 5) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "解凍藥");
			CTextDraw::Print(pDC, 400, 400, to_string(characterinf[12]));
			CTextDraw::Print(pDC, 150, 450, "解麻藥");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[13]));
			CTextDraw::Print(pDC, 150, 500, "解眠藥");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[14]));
		}
		else if (propnum == 6) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "萬靈藥");
			CTextDraw::Print(pDC, 400, 400, to_string(characterinf[15]));
			CTextDraw::Print(pDC, 150, 450, "活力碎片");
			CTextDraw::Print(pDC, 400, 450, to_string(characterinf[16]));
			CTextDraw::Print(pDC, 150, 500, "活力塊");
			CTextDraw::Print(pDC, 400, 500, to_string(characterinf[17]));
		}
	}
	else if (shopnum){
		if (shopnum == 1) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "購買");
			CTextDraw::Print(pDC, 150, 450, "出售");
			CTextDraw::Print(pDC, 150, 500, "退出");
		}
		else if (shopnum == 2) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "精靈球    $100");
			CTextDraw::Print(pDC, 150, 450, "超級球    $300");
			CTextDraw::Print(pDC, 150, 500, "高級球    $500");
		}
		else if (shopnum == 3) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "神奇糖果$1000");
			CTextDraw::Print(pDC, 150, 450, "傷藥        $300");
			CTextDraw::Print(pDC, 150, 500, "好傷藥    $500");
		}
		else if (shopnum == 4) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "厲害傷藥$100");
			CTextDraw::Print(pDC, 150, 450, "全滿藥    $300");
			CTextDraw::Print(pDC, 150, 500, "解毒藥    $500");
		}
		else if (shopnum == 5) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "灼傷藥    $100");
			CTextDraw::Print(pDC, 150, 450, "解凍藥    $300");
			CTextDraw::Print(pDC, 150, 500, "解麻藥    $500");
		}
		else if (shopnum == 6) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "解眠藥    $100");
			CTextDraw::Print(pDC, 150, 450, "萬靈藥    $300");
			CTextDraw::Print(pDC, 150, 500, "活力碎片$500");
		}
		else if (shopnum == 7) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "活力塊    $100");
			CTextDraw::Print(pDC, 150, 450, "");
			CTextDraw::Print(pDC, 150, 500, "");
		}
		else if (shopnum == 100) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 150, 400, "治療寶可夢");
			CTextDraw::Print(pDC, 150, 450, "肢解寶可夢");
			CTextDraw::Print(pDC, 150, 500, "返回");
		}
	}
	else if (signnum) {
		if (signnum == 1) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 50, 400, "還是不要亂進去店裡面好了");
		}
		else if (signnum == 2) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 50, 400, "往森林");
		}
		else if (signnum == 3) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 50, 400, "往豪宅");
		}
		else if (signnum == 4) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 50, 400, "往並不是真新鎮");
		}
		else if (signnum == 5) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 50, 400, "往荒野");
		}
	}
	if (team) {
		CTextDraw::ChangeFontLog(pDC, 30, "微軟正黑體", RGB(0, 0, 0), 1000);
		CTextDraw::Print(pDC, 30, 30, pokemon[0]);
		CTextDraw::Print(pDC, 30, 100, pokemon[1]);
		CTextDraw::Print(pDC, 30, 170, pokemon[2]);
		CTextDraw::Print(pDC, 30, 240, pokemon[3]);
		CTextDraw::Print(pDC, 30, 310, pokemon[4]);
		CTextDraw::Print(pDC, 30, 380, pokemon[5]);

	}
	
	CDDraw::ReleaseBackCDC();
}

