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
		"resources/map/pokemon.bmp"
		});//,RGB(100,100,100));
	background.SetTopLeft(235, 170);
	/*background.SetTopLeft(-215, -1885);*/
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);
	textbox.LoadBitmapByString({ "Resources/text.bmp" });
	textbox.SetTopLeft(0, 378);
	menu.LoadBitmapByString({ "Resources/menu.bmp" });
	menu.SetTopLeft(400, 40);
	arrow.LoadBitmapByString({ "Resources/arrow.bmp" });
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
	overlapleft = 0;
	overlapright = 0;
	overlaptop = 0;
	overlapdown = 0;
	if (bag == false && textnum == 0) {
		for (auto i = hitbox.begin(); i != hitbox.end(); i++) {                                   //判定卡hitbox
			if (CMovingBitmap::IsOverlap(character, *i)) {
				if (character.GetLeft() > i->GetLeft()) {
					overlapleft = 1;
				}
				if (character.GetLeft() < i->GetLeft()) {                                         //不同地方放else會出現一些很神奇的效果
					overlapright = 1;
				}
				if (character.GetTop() < i->GetTop()) {
					overlaptop = 1;
				}
				if (character.GetTop() > i->GetTop()) {
					overlapdown = 1;
				}
			}
		}
		//判定卡hitbox，不同地方放else會出現一些很神奇的效果
		int frame = character.GetFrameIndexOfBitmap();

		if (overlapleft && speedX > 0) {
			speedX = 0;//background.SetTopLeft(background.GetLeft(), background.GetTop() + speedY);
		}
		if (overlapright && speedX < 0) {
			speedX = 0;// background.SetTopLeft(background.GetLeft(), background.GetTop() + speedY);
		}
		if (overlaptop && speedY < 0) {
			speedY = 0; //background.SetTopLeft(background.GetLeft() + speedX, background.GetTop());
		}
		if (overlapdown && speedY > 0) {
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
				Sleep(500);
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
				Sleep(500);
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
				Sleep(500);
			}
		}
		else if (phase == 3) {
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
				Sleep(500);
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
				Sleep(500);
			}
		}
		else if (phase == 4) {
			if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(870, -115);
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
				Sleep(500);
			}
			else if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
				phase = 3;
				background.SetFrameIndexOfBitmap(2);
				background.SetTopLeft(-240, -115);
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
				Sleep(500);
			}
		}
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_ESCAPE) {	//暫停並顯示箭頭
		if (textnum == 0) {
			bag = !bag;
			arrow.SetTopLeft(430, 75);
		}
		else {
			textnum = 0;
		}
	}
	if (nChar == VK_RETURN) {
		if (bag == true) {
			int arrow_y = arrow.GetTop();
			if (arrow_y == 255)		//離開功能
				exit(0);
		}
		if (phase == 4) {
			if (CMovingBitmap::IsOverlap(character, dialog[0])) {
				textnum = 1;
				arrow.SetTopLeft(100, 415);
			}
			else if (CMovingBitmap::IsOverlap(character, dialog[1])) {
				textnum = 100;
			}
		}
		/*if (textnum == 1) {
			if (arrow.GetTop() == 415) {
				textnum = 2;
			}
			else if (arrow.GetTop() == 465) {
				textnum = 3;
			}
			else {
				textnum = 0;
			}
		}*/
		if (textnum == 2) {
			if (arrow.GetTop() == 415) {
				
			}
			else if (arrow.GetTop() == 465) {
				
			}
			else {
				textnum = 1;
			}
		}
	}

	if(nChar == VK_LEFT )
	{	
		speedX = 5;
		speedY = 0;
	}
	if (nChar == VK_UP )
	{
		if (bag == false && textnum == 0) {
			speedY = 5;
			speedX = 0;
		}
		if (bag == true) {
			if (arrow.GetTop() != 75)		//選單箭頭  -->選單功能之後會加在上面的enter裡
				arrow.SetTopLeft(430, arrow.GetTop() - 60);
		}
		if (textnum == 1|| textnum == 2) {
			if (arrow.GetTop() != 415)		
				arrow.SetTopLeft(100, arrow.GetTop() - 50);
		}
														//三項選單
	}
	if (nChar == VK_DOWN )
	{
		if (bag == false && textnum == 0) {
			speedY = -5;
			speedX = 0;
		}
		if (bag == true) {
			if (arrow.GetTop() != 400)					//選單箭頭
				arrow.SetTopLeft(430, arrow.GetTop() + 60);
		}
		if (textnum == 1 || textnum == 2) {
			if (arrow.GetTop() != 515)		
				arrow.SetTopLeft(100, arrow.GetTop() + 50);
		}
														//三項選單
	}
	if (nChar == VK_RIGHT )
	{
		speedX = -5;
		speedY = 0;
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
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
	else if (nChar == VK_RIGHT)
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
	if (textnum != 0) {
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
	
}

void CGameStateRun::show_text_by_phase() {

	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());

	CTextDraw::ChangeFontLog(pDC, 21, "微軟正黑體", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);
	if (bag == true) {				//選單文字
		CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 800);
		CTextDraw::Print(pDC, 450, 60, "寶可夢");
		CTextDraw::Print(pDC, 450, 120, "背包");
		CTextDraw::Print(pDC, 450, 180, "存檔");
		CTextDraw::Print(pDC, 450, 240, "離開");
	}
	if (textnum == 1) {
		CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 1000);
		CTextDraw::Print(pDC, 150, 400, "購買");
		CTextDraw::Print(pDC, 150, 450, "出售");
		CTextDraw::Print(pDC, 150, 500, "返回");
	}
	if (textnum == 2) {
		CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 1000);
		CTextDraw::Print(pDC, 150, 400, "初級球   $100");
		CTextDraw::Print(pDC, 150, 450, "中級球   $300");
		CTextDraw::Print(pDC, 150, 500, "高級球   $500");
	}
	if (textnum == 3) {
		CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 1000);
		CTextDraw::Print(pDC, 150, 400, "初級球   $100");
		CTextDraw::Print(pDC, 150, 450, "中級球   $300");
		CTextDraw::Print(pDC, 150, 500, "高級球   $500");
	}
	if (textnum == 100) {
		CTextDraw::ChangeFontLog(pDC, 40, "微軟正黑體", RGB(0, 0, 0), 1000);
		CTextDraw::Print(pDC, 150, 400, "治療寶可夢");
		CTextDraw::Print(pDC, 150, 450, "肢解寶可夢");
		CTextDraw::Print(pDC, 150, 500, "返回");
	}
	CDDraw::ReleaseBackCDC();
}

