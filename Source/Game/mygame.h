/*
 * mygame.h: ���ɮ��x�C��������class��interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include <vector>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		void load_background();
		void draw_text();
		CMovingBitmap background;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		int lastX = 0;
		int lastY = 0;
		int speedX = 0;
		int speedY = 0;
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int phase = 2;
		int sub_phase = 1;
		int driection=0;
		int characterFrameCounter = 0;
		bool bag = false;
		bool battle = false;
		int propnum = 0;
		bool text = false;
		bool ran = true;
		bool accel = false;
		int temptop = 0;
		int templeft = 0;
		CMovingBitmap house_1f;
		CMovingBitmap house_2f;
		CMovingBitmap background;
		/*CMovingBitmap shoptext;
		CMovingBitmap pokemontext;*/			//�ȮɵL��
		CMovingBitmap character;
		CMovingBitmap textbox;
		CMovingBitmap menu;
		CMovingBitmap point;
		CMovingBitmap arrow;
		CMovingBitmap white;
		CMovingBitmap boss[5];
		CMovingBitmap battle_scr;
		std::vector<CMovingBitmap> pack;
		std::vector<CMovingBitmap> hitbox;				//���Ψ�ʭӪ���ݬݰ�����
		std::vector<CMovingBitmap> tppoint;
		std::vector<CMovingBitmap> grass;
		std::vector<CMovingBitmap> sign;
		std::vector<CMovingBitmap> dialog;
		bool overlapleft = 0;
		bool overlapright = 0;
		bool overlaptop = 0;
		bool overlapdown = 0;
		bool team = 0;
		bool confirmenter = false;
		bool confirmup = false;
		bool confirmdown = false;
		bool sell = false;
		bool usingprop = false;
		bool d = false;
		int useprop = 0;
		int storynum = 1;
		int shopnum = 0;
		int hitboxnum = 0;
		int grassnum = 0;
		int tppointnum = 0;
		int textnum = 0;
		int nownum = 0;
		int nownum1 = 0;
		int arrownum = 0;
		int signnum = 0;
		int pokemoninf[22][13];
		string pokemonname[22];
		int skilldam[35];
		string skillname[35];
		void show_image_by_phase();
		void show_text_by_phase();
		/*void phase_house();
		void phase_map();*/
		int battle_phase = 0;
		int judge;
		int turn = 0;
		void battle_start();
		void turn_end();
		void team_change();
		void battle_turn();
		int hp1;
		int hp2;
		int att1;
		int att2;
		int def1;
		int def2;
		/*int Satt1;
		int Satt2;
		int Sdef1;
		int Sdef2;*/
		int spe1;
		int spe2;
		int dam1;
		int dam2;
		int LV2;
		int pokeid = 0;
		int pokest = 0;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
		void OnKeyDown(UINT, UINT, UINT);
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap background;
		void load_background();
	};
	//struct pokemon {
	//	int level = 1;
	//	int experience = 0;
	//	int race = 0;										//�ر�attack life
	//	int pokemonnum = 0;
	//	int skill[4] = { 0,0,0,0 };
	//};
	//struct characterinf {
	//	int money = 0;
	//	int prop[17] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};									//�D��
	//	int dojonum = 0;
	//	int phase = 2;
	//	int sitex = 210;
	//	int sitey = 205;
	//	pokemon pokemoninpack[6];
	//};
	//int meninf[80];
	
	
}