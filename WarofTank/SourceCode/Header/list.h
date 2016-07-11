#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"

bool g_bStart	    =	false;  	// ����һ����Ϸ��ʼtrue�����false
float g_fGameTime	=	0.f;    	// һ����Ϸ��ʣ��ʱ��
int	g_iScore	    =	0;		// һ����Ϸ�÷�
const float	GAME_TIME	=	30.f;		// һ����Ϸ��ʱ��
const float	TANK_SPEED	=	5.f;		// ̹���ٶ�
const  float 	BULLET_SPEED	 = 	8.f;		// �ӵ��ٶ�
const  float	FIRE_TIME		 = 	2.f;		// ̹�˿���ʱ����
const  float 	CREATE_TANK_TIME = 	5.f;		// ÿ��������̹�˵�ʱ����
float g_fCreateTank;	// ����������һ���з�̹�˵�ʣ��ʱ��


struct Weapon{
	char		szName[128];			// ��������
	float		fPosX,  fPosY;			// ��������
	float		fSpeedX,  fSpeedY;	// X��Y�������ٶ�
	float		fFireTime;			// �з�̹�˾���һ�ο��ڵ�ʣ��ʱ��
	int		iHp;					// ����ֵ
	int		iDir;		            // ����0 - �Ϸ���1 - �ҷ���2 - �·���3 - ��
	int		iType;               // ���ͣ� 0 - �ҷ�̹�ˣ�1 - �з�̹�ˣ�2 - �ҷ�
// �ӵ��� 3 - �з��ӵ�
	Weapon* pNext;			    // ָ����һ���ڵ��ָ��
};
