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
#include<iostream>
#include<cstdlib>
#include<ctime>

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
		"resources/map/pokemon.bmp"
		});//,RGB(100,100,100));
	background.SetTopLeft(235, 170);
	/*background.SetTopLeft(-215, -1885);*/
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);
	ifstream ifs1("resources/files/character.txt");
	for (int i = 0; i < 80; i++) {
		ifs1 >> characterinf[i];
	}
	textbox.LoadBitmapByString({ "Resources/text.bmp" });
	textbox.SetTopLeft(0, 378);
	menu.LoadBitmapByString({ "Resources/menu.bmp","Resources/team.bmp" });
	menu.SetTopLeft(400, 40);
	arrow.LoadBitmapByString({ "Resources/arrow.bmp"});
	battle_scr.LoadBitmapByString({ "Resources/battle2.bmp","Resources/battle.bmp" });

	ifstream ifs("mapdoc/home2f.txt");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			int a = 0;
			ifs >> a;
			home2fmap.push_back(a);
		}
	}
	ifs.close();							//讀地圖txt進來
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			int a = 0;
			a = home2fmap.front();
			home2fmap.erase(home2fmap.begin());
			if (a == 1) {
				hitbox.insert(hitbox.begin(), CMovingBitmap());
				hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
				hitbox.begin()->SetTopLeft(235 + j * 32, 170 + i * 32);
			}
			else if (a == 2) {
				tppoint.insert(tppoint.begin(), CMovingBitmap());
				tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
				tppoint.begin()->SetTopLeft(235 + j * 32, 170 + i * 32);
			}
		}
	}
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{	
	int nowtop = background.GetTop();
	int nowleft = background.GetLeft();	
	if (nownum > 3){
		nownum = 0;
	}
	
	nownum++;
	overlapleft = 0;
	overlapright = 0;
	overlaptop = 0;
	overlapdown = 0;
	if (bag == false && battle == false && textnum == 0) {
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
				ifstream ifs("mapdoc/home2f.txt");
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						int a = 0;
						ifs >> a;
						home2fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						int a = 0;
						a = home2fmap.front();
						home2fmap.erase(home2fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(170 + j * 32, 170 + i * 32);
						}

						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(170 + j * 32, 170 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-240, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				ifstream ifs("mapdoc/town.txt");
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-240 + j * 32, -115 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(-240 + j * 32, -115 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-240 + j * 32, -115 + i * 32);
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
				ifstream ifs("mapdoc/home1f.txt");
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(140 + j * 32, 205 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(140 + j * 32, 205 + i * 32);
						}
					}
				}
			}
		}
		else if (phase == 3) {
			if (abs(nowtop - temptop) == 30 || abs(nowleft - templeft) == 30) {
				ran = true;
			}
			for (auto i = grass.begin(); i != grass.end(); i++) {
				if (CMovingBitmap::IsOverlap(character, *i)) {
					if (ran) {
						judge = rand() / 100 % 10;
						ran = false;
						if (judge > 6) {
							battle = true;
							temptop = nowtop;
							templeft = nowleft;
							battle_phase = 1;
							battle_scr.SetFrameIndexOfBitmap(1);
							battle_scr.SetTopLeft(0, 0);
							arrow.SetTopLeft(300, 440);
							arrownum = 4;
							battle_start();
						}
					}
					//程式碼放這邊
				}
			}
			// 遍歷草叢進入戰鬥
			if (CMovingBitmap::IsOverlap(character, tppoint[1])) {
				phase = 1;
				background.SetFrameIndexOfBitmap(1);
				background.SetTopLeft(205, 80);
				character.SetFrameIndexOfBitmap(5);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				ifstream ifs("mapdoc/home1f.txt");
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 7; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(205 + j * 32, 80 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(205 + j * 32, 80 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 4;
				background.SetFrameIndexOfBitmap(3);
				background.SetTopLeft(105, 50);
				character.SetFrameIndexOfBitmap(5);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				ifstream ifs("mapdoc/pokemon.txt");
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 12; j++) {
						int a = 1;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 12; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(105 + j * 32, 50 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(105 + j * 32, 50 + i * 32);
						}
						else if (a == 5) {
							dialog.insert(dialog.begin(), CMovingBitmap());
							dialog.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							dialog.begin()->SetTopLeft(105 + j * 32, 50 + i * 32);
						}
					}
				}
			}
		}
		else if (phase == 4) {
			if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-870, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				ifstream ifs("mapdoc/town.txt");
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
					}
				}
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-870, -115);
				character.SetFrameIndexOfBitmap(0);
				hitbox.clear();
				tppoint.clear();
				grass.clear();
				dialog.clear();
				ifstream ifs("mapdoc/town.txt");
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						ifs >> a;
						home1fmap.push_back(a);
					}
				}
				ifs.close();							//讀地圖txt進來
				for (int i = 0; i < 22; i++) {
					for (int j = 0; j < 50; j++) {
						int a = 0;
						a = home1fmap.front();
						home1fmap.erase(home1fmap.begin());
						if (a == 1) {
							hitbox.insert(hitbox.begin(), CMovingBitmap());
							hitbox.begin()->LoadBitmapByString({ "Resources/air.bmp" });// , RGB(100, 100, 100));
							hitbox.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
						else if (a == 2) {
							tppoint.insert(tppoint.begin(), CMovingBitmap());
							tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							tppoint.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
						else if (a == 3) {
							grass.insert(grass.begin(), CMovingBitmap());
							grass.begin()->LoadBitmapByString({ "Resources/air.bmp" });
							grass.begin()->SetTopLeft(-870 + j * 32, -115 + i * 32);
						}
					}
				}
			}
		}
	}
}


void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_ESCAPE) {	//暫停並顯示箭頭
		if (shopnum == 0 && battle!=true) {
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
		else if (team) {
			shopnum = 0;
			arrow.SetTopLeft(430, 75);
			arrownum = 0;
			menu.SetFrameIndexOfBitmap(0);
			menu.SetTopLeft(400, 40);
		}
		//team?
		else {
			shopnum = 0;
			arrownum = 0;
		}
		//close the window
		if (battle) {
			if (arrownum != 4) {
				battle_scr.SetFrameIndexOfBitmap(1);
				battle_scr.SetTopLeft(0, 0);
				arrow.SetTopLeft(300, 440);
				arrownum = 4;
				propnum = 0;
			}
		}
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
				for (int i = 0; i < 80; i++) {
					ofs << characterinf[i];
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
				team = 1;
				arrow.SetTopLeft(30, 30);
				arrownum = 3;//暫定隊伍
				menu.SetFrameIndexOfBitmap(1);
				menu.SetTopLeft(0, 0);
			}
			//查看隊伍
		}
		//stop function
		//for (auto i = grass.begin(); i != grass.end(); i++) {
		//	if (CMovingBitmap::IsOverlap(character, *i)) {
		//		battle = !battle;
		//		battle_phase = 1;
		//		battle_scr.SetFrameIndexOfBitmap(1);
		//		battle_scr.SetTopLeft(0, 0);
		//	}
		//}
		//// 遍歷草叢enter進入戰鬥
		if (phase == 4 && bag == false) {
			if (CMovingBitmap::IsOverlap(character, dialog[0]) && shopnum == 0) {
				shopnum = 1;
				arrow.SetTopLeft(100, 415);
				arrownum = 2;
			}
			//into shop
			else if (CMovingBitmap::IsOverlap(character, dialog[1])) {
				
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
		if (arrownum == 4){
			if (arrow.GetLeft() == 300) {
				if (arrow.GetTop() == 440) {
					arrownum = 5;
					battle_scr.SetFrameIndexOfBitmap(0);
					battle_scr.SetTopLeft(0, 0);
					arrow.SetTopLeft(40, 440);
				}
				else {
					propnum = 1;
					battle_scr.SetFrameIndexOfBitmap(0);
					battle_scr.SetTopLeft(0, 0);
					arrow.SetTopLeft(100, 415);
					arrownum = 2;
				}
			}
			else {
				if (arrow.GetTop() == 440) {
					arrownum = 3;
				}
				else {
					battle = false;
				}
			}
		}
	}


	if (nChar == VK_UP)
	{
		if (bag == false && battle == false && shopnum == 0 ) {
			if (accel) {
				speedY = 10;
				speedX = 0;
			}
			else {
				speedY = 5;
				speedX = 0;
			}
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
		else if (arrownum == 4) {
			if (arrow.GetTop() != 440)
				arrow.SetTopLeft(arrow.GetLeft(), arrow.GetTop() - 60);
		}
		else if (arrownum == 5) {
			if (arrow.GetTop() != 440)
				arrow.SetTopLeft(arrow.GetLeft(), arrow.GetTop() - 60);
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
			if (accel) {
				speedY = -10;
				speedX = 0;
			}
			else {
				speedY = -5;
				speedX = 0;
			}

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
		else if (arrownum == 4) {
			if (arrow.GetTop() != 500)
				arrow.SetTopLeft(arrow.GetLeft(), arrow.GetTop() + 60);
		}
		else if (arrownum == 5) {
			if (arrow.GetTop() != 500)
				arrow.SetTopLeft(arrow.GetLeft(), arrow.GetTop() + 60);
		}
		// 下方三項箭頭
		
	}
	//記得補正
	if (nChar == VK_LEFT)
	{
		if(accel){
			speedX = 10;
			speedY = 0;
		}
		else {
			speedX = 5;
			speedY = 0;
		}
		if (arrownum == 4) {
			if (arrow.GetLeft() != 300)
				arrow.SetTopLeft(arrow.GetLeft() - 140, arrow.GetTop());
		}
		else if (arrownum == 5) {
			if (arrow.GetLeft() != 40)
				arrow.SetTopLeft(arrow.GetLeft() - 300, arrow.GetTop());
		}
		
		// try to finish walking bug
	}
	if (nChar == VK_RIGHT)
	{
		if (accel) {
			speedX = -10;
			speedY = 0;
		}
		else {
			speedX = -5;
			speedY = 0;
		}
		if (arrownum == 4) {
			if (arrow.GetLeft() != 440)
				arrow.SetTopLeft(arrow.GetLeft() + 140, arrow.GetTop());
		}
		else if (arrownum == 5) {
			if (arrow.GetLeft() != 340)
				arrow.SetTopLeft(arrow.GetLeft() + 300, arrow.GetTop());
		}
	}
	if (nChar == VK_SHIFT)
	{
		accel = !accel;
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN) {
		confirmenter = false;
	}
	if (nChar == VK_LEFT)
	{
		speedX = 0;

	}
	else if (nChar == VK_RIGHT)
	{
		speedX = 0;
	}
	else if (nChar == VK_UP)
	{
		speedY = 0;
	}
	else if (nChar == VK_DOWN)
	{
		speedY = 0;
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
	
	if (shopnum || propnum) {
		textbox.ShowBitmap();
		arrow.ShowBitmap();
	}
	if (bag == true) {
		menu.ShowBitmap();
		arrow.ShowBitmap();
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
	if (battle) {
		battle_scr.ShowBitmap();
		arrow.ShowBitmap();
	}
}
void CGameStateRun::battle_start() {
	/*
	*	資料讀入
	*	turn = 0; //回合清零	
		我方HP = (((種族值+7)*LV)/50+10+LV)/100*目前血量
		對面HP = ((種族值+7)*LV)/50+10+LV)
		其他數值 = ((種族值+7)*LV)/50+5
		
		battle_turn();
	*/
}
void CGameStateRun::battle_turn() {
	/*
	*	turn += 1;
	*	if(不使用道具){ 
			判斷雙方速度值
			進行攻守
			if(睡眠){
				if (turn-tempturn >= 2)
					睡眠解除
				傷害 = (((2*攻擊方等級+10)/250) * (攻擊方攻擊/防守方防禦)*招式威力 +2)*屬性加成 ,灼燒造成的物理招式減弱一半
				我方HP = HP - 傷害;
				if(傷害==0)
					提示無效果 結束該回合
				if(我方HP<=0)
					我方HP　= 0;
					if(我方血量歸0)
						if(背包中有可以戰鬥的寶可夢)
							選擇上場的寶可夢
						else
							退出戰鬥
							扣錢500
			}
			if(對方先攻){
				傷害 = (((2*攻擊方等級+10)/250) * (攻擊方攻擊/防守方防禦)*招式威力 +2)*屬性加成 ,灼燒造成的物理招式減弱一半
				我方HP = HP - 傷害;
				if(我方HP<=0)
					我方HP　= 0;
					if(我方血量歸0)
						if(背包中有可以戰鬥的寶可夢)
							選擇上場的寶可夢
						else
							退出戰鬥 
							扣錢500
				else
					是否會造成異常狀態
					if(睡眠)
						tempturn = turm

				傷害 = (((2*攻擊方等級+10)/250) * (攻擊方攻擊/防守方防禦)*招式威力 +2)*屬性加成 ,灼燒造成的物理招式減弱一半
				對面HP = HP - 傷害;
				if(對面HP<=0)
						對面HP　= 0;
						if(對面血量歸0)
								退出戰鬥
								經驗值增加以等級*100
				else
					是否會造成異常狀態
	
			}
			else{
				傷害 = (((2*攻擊方等級+10)/250) * (攻擊方攻擊/防守方防禦)*招式威力 +2)*屬性加成 ,灼燒造成的物理招式減弱一半
				對面HP = HP - 傷害;
				if(對面HP<=0)
						對面HP　= 0;
						if(對面血量歸0)
								退出戰鬥
								經驗值增加以等級*100
				else
					是否會造成異常狀態
				傷害 = (((2*攻擊方等級+10)/250) * (攻擊方攻擊/防守方防禦)*招式威力 +2)*屬性加成 ,灼燒造成的物理招式減弱一半
				我方HP = HP - 傷害;
				if(我方HP<=0)
					我方HP　= 0;
					if(我方血量歸0)
						if(背包中有可以戰鬥的寶可夢)
							選擇上場的寶可夢
						else
							退出戰鬥
							扣錢500
				else
					是否會造成異常狀態
			}

		}
		else{
			判斷道具使用類型

		}
	*/
}
void CGameStateRun::battle_end() {
	/*
		

	*/
}
void CGameStateRun::show_text_by_phase() {

	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());
	string tx = to_string(templeft);
	string ty = to_string(temptop);
	string moneyout = to_string(characterinf[0]);
	string j = to_string(judge % 1000);
	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);
	CTextDraw::Print(pDC, 0, 20, tx);
	CTextDraw::Print(pDC, 90, 20, ty);
	CTextDraw::Print(pDC, 180, 0, j);
	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 255, 51), 800);
	//CTextDraw::Print(pDC, 535, 0, "$");
	CTextDraw::Print(pDC, 300, 0, moneyout);//550
	if (bag == true || battle == true) {				//選單文字
		if (shopnum == 0 && battle == false) {
			CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 800);
			CTextDraw::Print(pDC, 450, 60, "隊伍");
			CTextDraw::Print(pDC, 450, 120, "背包");
			CTextDraw::Print(pDC, 450, 180, "存檔");
			CTextDraw::Print(pDC, 450, 240, "離開");
		}
		if (team) {
			CTextDraw::ChangeFontLog(pDC, 30, "微軟正黑體", RGB(0, 0, 0), 800);
			CTextDraw::Print(pDC, 90, 10, "名字");
			CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 800);
			CTextDraw::Print(pDC, 30, 400, "請選擇一隻寶可夢");
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
	else {
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
	if (battle == true) {
		CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
		/*CTextDraw::Print(pDC, 330, 430, "戰鬥");
		CTextDraw::Print(pDC, 470, 430, "寶可夢");
		CTextDraw::Print(pDC, 330, 490, "背包");
		CTextDraw::Print(pDC, 470, 490, "逃跑");*/
		if (arrownum == 4) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 330, 430, "戰鬥");
			CTextDraw::Print(pDC, 470, 430, "寶可夢");
			CTextDraw::Print(pDC, 330, 490, "背包");
			CTextDraw::Print(pDC, 470, 490, "逃跑");
		}
		else if (arrownum == 5) {
			CTextDraw::ChangeFontLog(pDC, 35, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 60, 430, "技能1");
			CTextDraw::Print(pDC, 360, 430, "技能2");
			CTextDraw::Print(pDC, 60, 490, "技能3");
			CTextDraw::Print(pDC, 360, 490, "技能4");
			CTextDraw::ChangeFontLog(pDC, 30, "微軟正黑體", RGB(0, 0, 0), 1000);
			CTextDraw::Print(pDC, 220, 430, "pp/pp");
			CTextDraw::Print(pDC, 520, 430, "pp/pp");
			CTextDraw::Print(pDC, 220, 490, "pp/pp");
			CTextDraw::Print(pDC, 520, 490, "pp/pp");
		}
	}
	
	CDDraw::ReleaseBackCDC();
}

