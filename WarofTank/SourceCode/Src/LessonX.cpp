/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"

////////////////////////////////////////////////////////////////////////////////
//
//

/*
const  float	GAME_TIME		 =	30.f;    // һ����Ϸʱ��
const  float 	CREATE_TANK_TIME = 	5.f;		// ÿ��������̹�˵�ʱ����
const  float	TANK_SPEED		 = 	5.f;     	// ̹���ٶ�
const  float 	BULLET_SPEED	 = 	8.f;		// �ӵ��ٶ�
const  float	FIRE_TIME		 = 	2.f;		// ̹�˿���ʱ����
*/
const  float	WORLD_LEFT	     = 	-26.f;	// ��Ϸ�����߽���ֵ
const  float	WORLD_TOP	     = 	-22.f;	// ��Ϸ�����߽�shangֵ
const  float	WORLD_RIGHT	 = 	26.f;		// ��Ϸ�����߽�youֵ
const  float	WORLD_BOTTOM  = 	22.f;		// ��Ϸ�����߽�xiaֵ

enum  Direction{
	UP		=	0,		// �Ϸ�
    RIGHT	=	1,		// �ҷ�
	DOWN	=	2,		// �·�
	LEFT	    =	3		// ��
};

enum  Role
{
	MYTANK		=	0,		// �ҷ�̹��
	ENEMYTANK	=	1,		// �з�̹��
	MYBULLET	=	2,		// �ҷ��ӵ�
	ENEMYBULLET	  =	3		// �з��ӵ�
};

int	g_iNum	=	0;		// ��¼ȫ���������Ŀ����������������

Weapon* g_pMyTank	=	NULL;	// �ҷ�̹��

Weapon* g_pHead	=	NULL;
/*
bool g_bStart;			// ��ʶһ����Ϸ��ʼ���ǽ���
int	g_iScore;			// һ����Ϸ�÷�
float g_fGameTime;	    // һ����Ϸ��ʣ��ʱ��
float g_fCreateTank;	// ����������һ���з�̹�˵�ʣ��ʱ��
// ��Ϸ��ͼ��0��ʾ�˴�Ϊ�գ�1��ʾ�˴���ǽ��������Ϸ�ռ��С��ǽ�Լ�̹�˴�С��
// ���ǰѵ�ͼ�ֳ�11�У�13�У�ÿ���С�պ÷�һ��ǽ��
*/

int	iMap[11][13];

int	g_iWallCount	=	0;		// ��¼ǽ������
int	g_iMap[11][13];			// ��ͼ����

void LoadMap()
{
    int i,j,x,y;
    char szName[128];

    int g_iMap[11][13]=
    {
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,1,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,0,0,0,0,0},
		{0,1,0,1,0,0,0,0,0,1,0,1,0},
		{0,1,0,1,0,0,0,0,0,1,0,1,0},
		{0,1,0,1,0,1,1,1,0,1,0,1,0},
		{0,0,0,0,0,1,0,1,0,0,0,0,0},
	};
	for(i=0;i<11;i++)
	{
		for(j=0;j<13;j++)
		{
			if(g_iMap[i][j]==1)
			{
				sprintf(szName, "wall%d", g_iWallCount++);
				dCloneSprite("wall",szName);
				// ��wall��Ϊ������ײ
				dSetSpriteCollisionReceive(szName, true);

				x = -24+4*j;
				y = -20+4*i;
				dSetSpritePosition(szName,x,y);
			}
		}
	}
	// ָ�Ӳ������ڵ�11�е�7��
	dSetSpriteVisible("goal", true);
	dSetSpritePosition("goal", 0.f, 20);

	dSetSpriteVisible("splash", false);

}

void  MoveMyTank(int iKey, bool bPress)
{
    if(bPress)
    {
        if(iKey == KEY_W)
        {
            dSetSpriteLinearVelocity( "player" , 0 , -TANK_SPEED );
            dSetSpriteRotation( "player" , 0 );
        }
        else if(iKey == KEY_S)
        {
            dSetSpriteLinearVelocity( "player" , 0 , TANK_SPEED );
            dSetSpriteRotation( "player" , 180 );
        }
        else if(iKey == KEY_A)
        {
            dSetSpriteLinearVelocity( "player" , -TANK_SPEED , 0 );
            dSetSpriteRotation( "player" , -90 );
        }
        else if(iKey == KEY_D)
        {
            dSetSpriteLinearVelocity( "player" , TANK_SPEED , 0 );
            dSetSpriteRotation( "player" , 90 );
        }
    }
    else{
        dSetSpriteLinearVelocity( "player" , 0 , 0);
    }

}


int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

bool bLoad = true;

void	OnMyTankColOther(const char* szMyTankName, const char* szOtherName)
{
	// �������ǽ��
	if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(szMyTankName, 0.f, 0.f);
}

void OnFire(char* szName, int iType)
{
    float du = dGetSpriteRotation(szName);
    float tankX = dGetSpritePositionX(szName);
    float tankY = dGetSpritePositionY(szName);
    dSetSpritePosition( "bullet" , tankX , tankY );
    dSetSpriteVisible("bullet", true);
    dSetSpriteRotation("bullet" , du);
    if(du == 0)
        {
            dSetSpriteLinearVelocity( "bullet" , 0 , -BULLET_SPEED );
        }
    if(du == 90)
        {
            dSetSpriteLinearVelocity( "bullet" , BULLET_SPEED , 0 );
        }
    if(du == 180)
        {
            dSetSpriteLinearVelocity( "bullet" , 0 , BULLET_SPEED );
        }
    if(du == -90)
        {
            dSetSpriteLinearVelocity( "bullet" , -BULLET_SPEED ,0 );
        }

}


//==============================================================================
//
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�


//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬.
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime )
{

    if(g_bStart)    //TODO
    {
        if(bLoad)  // һ����Ϸֻ��Ҫ����һ�ε�ͼ����
        {
            // �����ͼ
            LoadMap();
            bLoad = false;
        }

        g_fGameTime -= fDeltaTime;
        if(g_fGameTime > 0.f)  // һ����Ϸ������
        {

        }
        else  // һ����Ϸ����
        {
            g_bStart = false;
            char szName[128];
            for(int i=0; i<g_iWallCount; i++)
            {
                sprintf(szName, "wall%d", i);
                dDeleteSprite(szName);
            }
            g_iWallCount = 0;
            bLoad = true;
            // ����ָ�������ɼ�
            dSetSpriteVisible("goal", false);

        }
    }
    else  // ��Ϸ��������
    {

    }

	switch( g_iGameState )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void GameInit()
{
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
}
//==========================================================================
//
// ����ƶ�
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseMove( const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// �����
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// ��굯��
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// ���̰���
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
    if(iKey == KEY_SPACE && g_bStart == false) // ��Ϸδ��ʼ�����¿ո��
    {
        g_bStart = true;
        g_fGameTime = GAME_TIME;
        dSetSpriteVisible("kaishi", false);
        dSetTextValue("time", (int)g_fGameTime);
        dSetTextValue("score", g_iScore);
        dSetSpritePosition("player", -8 , 20);
    }
    // ���¼���
    if(g_bStart)  // һ����Ϸ������
    {
        if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
		{
			MoveMyTank(iKey, true);
		}
    }
    if(iKey == KEY_J && g_bStart == true)
    {
        OnFire( "player" , 2 );
    }

}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{
    // �ɿ�����
    if(g_bStart)
    {
        if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
        {
            MoveMyTank(iKey, false);
        }
    }


}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    OnMyTankColOther( szSrcName , szTarName );
}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    if(strcmp(szName, g_pMyTank->szName)==0)  // ���̹��
    {
        dSetSpriteLinearVelocity(g_pMyTank->szName, 0.f, 0.f);
    }
}
