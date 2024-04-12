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
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
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

void CGameStateRun::OnMove()							// ���ʹC������
{	
	overlapleft = 0;
	overlapright = 0;
	overlaptop = 0;
	overlapdown = 0;

	for (auto i = hitbox.begin(); i != hitbox.end(); i++) {                                   //�P�w�dhitbox
		if (CMovingBitmap::IsOverlap(character, *i)) {
			if (character.GetLeft() > i->GetLeft()) {
				overlapleft = 1;
			}
			if (character.GetLeft() < i->GetLeft()) {                                         //���P�a���else�|�X�{�@�ǫܯ��_���ĪG
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
	background.SetTopLeft(background.GetLeft() + speedX, background.GetTop() + speedY);    //�ª����ʨ���
	
	for (auto i = hitbox.begin(); i != hitbox.end(); i++) {                                 //�s�����ʪ���
		i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
	}
	for (auto i = tppoint.begin(); i != tppoint.end(); i++) {
		i->SetTopLeft(i->GetLeft() + speedX, i->GetTop() + speedY);
	}

	/*for (int i = 0; i < hitbox.size(); i++) {
		hitbox[i].SetTopLeft(hitbox[i].GetLeft() + speedX, hitbox[i].GetTop() + speedY);         //�ª����ʪ���
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
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()+1) % 2 + 6);
			}
			else if (speedX < 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()+1) % 2 + 8);
			}
			else if (speedX > 0) {
				character.SetFrameIndexOfBitmap((character.GetFrameIndexOfBitmap()) % 2 + 3);
			}
			characterFrameCounter = 0;
		}
		else {
			characterFrameCounter+=1;
		}
	}   //�I���a�ϲ���
	else
	{
		if (frame == 1 || frame ==2) {
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
			ifs.close();							//Ū�a��txt�i��

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
			background.SetTopLeft(-215, -1885);
			character.SetFrameIndexOfBitmap(0);
			hitbox.clear();
			tppoint.clear();
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
			ifs.close();							//Ū�a��txt�i��

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
		if (CMovingBitmap::IsOverlap(character, tppoint[0])) {
			phase = 1;
			background.SetFrameIndexOfBitmap(1);
			background.SetTopLeft(170, 20);
			character.SetFrameIndexOfBitmap(5);
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
			ifs.close();							//Ū�a��txt�i��

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
}


void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	background.LoadBitmapByString({
		"resources/map/home_2f.bmp",
		"resources/map/home_1f.bmp",
		"resources/map1_1.5.bmp",
		});//,RGB(100,100,100));
	background.SetTopLeft(235, 170);
	/*background.SetTopLeft(-215, -1885);*/
	
	character.LoadBitmapByString({ "Resources/character/red_front.bmp","resources/character/red_front1.bmp","resources/character/red_front2.bmp","resources/character/red_left.bmp","resources/character/red_left1.bmp","resources/character/red_back.bmp","resources/character/red_back1.bmp","resources/character/red_back2.bmp","resources/character/red_right.bmp","resources/character/red_right1.bmp" },RGB(255,255,255));
	character.SetTopLeft(300, 272);
	character.SetAnimation(1000, true);

	textbox.LoadBitmapByString({"Resources/text.bmp"});
	textbox.SetTopLeft(0,378);
	
	ifstream ifs("mapdoc/home2f.txt");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			int a = 0;
			ifs >> a;
			home2fmap.push_back(a);
		}
	}
	ifs.close();							//Ū�a��txt�i��

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			int a = 0;
			a = home2fmap.front();
			home2fmap.erase(home2fmap.begin());

			if (a == 1) {
				hitbox.insert(hitbox.begin(),CMovingBitmap());
				hitbox.begin()->LoadBitmapByString({"Resources/air.bmp"});// , RGB(100, 100, 100));
				hitbox.begin()->SetTopLeft(235 + j * 32, 170 + i * 32);
			}

			else if (a == 2) {
				tppoint.insert(tppoint.begin(),CMovingBitmap());
				tppoint.begin()->LoadBitmapByString({ "Resources/air.bmp" });
				tppoint.begin()->SetTopLeft(235 + j * 32, 170 + i * 32);
			}
		}
	}
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == VK_RETURN) {
	}

	if(nChar == VK_LEFT )
	{	
		speedX = 5;
		speedY = 0;
	}
	if (nChar == VK_UP )
	{
		speedY = 5;
		speedX = 0;
	}
	if (nChar == VK_DOWN )
	{
		speedY = -5;
		speedX = 0;
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

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
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
	//textbox.ShowBitmap();
	for (auto i = hitbox.begin(); i != hitbox.end(); i++) {
		i->ShowBitmap();
	}
	for (auto i = tppoint.begin(); i != tppoint.end(); i++) {
		i->ShowBitmap();
	}
}

void CGameStateRun::show_text_by_phase() {
	CDC* pDC = CDDraw::GetBackCDC();
	string x = to_string(background.GetLeft());
	string y = to_string(background.GetTop());

	CTextDraw::ChangeFontLog(pDC, 21, "�L�n������", RGB(255, 0, 0), 800);
	CTextDraw::Print(pDC, 0, 0, x);
	CTextDraw::Print(pDC, 90, 0, y);

	CDDraw::ReleaseBackCDC();
}

