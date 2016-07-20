/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//

bool        isGameContinue      =   false;

int         distanceByZhuzi     =   40;
int         speedOfZhuzi        =   -20;
int         score               =   0;


void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

void        dOnFrogColWorldLimit( const int iColSide );
void        dOnZhuziColWorldLimit( const int iColSide );

void        dCreateZhuzi();
void        dFrogFloat();
void        dMyPlaySound();
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
			if( isGameContinue )
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

//=============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void GameInit()
{
    distanceByZhuzi     =   40;
    speedOfZhuzi        =   -20;
    score               =   0;

    isGameContinue = true;

    dSetSpritePosition("frog" , -10 , 0 );

    dSetSpritePosition("GameOver" , 0 , 40 );

    dSetSpriteFlipY("frog" , 0 );

    if(dIsSpriteVisible("Title") == 1 || dIsSpriteVisible("title2") == 1)
    {
        dSetSpriteVisible("Title" , 0 );
        dSetSpriteVisible("title2" , 0 );
    }
    dSetSpriteMass("frog" , 10 );

    dSetSpriteWorldLimitMode("frog", WORLD_LIMIT_CLAMP);

    dCreateZhuzi();

    dSetSpriteVisible("result1" , 0 );

    dSetSpritePosition("score1" , -24 , -16 );
    dSetSpritePosition("score2" , -24 , -16 );



}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
    dSetSpriteConstantForceY("frog",900);   //����y����������
    dSetSpriteAngularVelocity("frog", 70 );     //��ת�ٶ�
    dSetTextValue("score",score);

    dMyPlaySound();

}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
    dSetSpriteConstantForceY("frog", 30);   //����y����������
    dSetSpriteAngularVelocity("frog", 0 );  //���þ���Ƕ���ת�ٶ�
    dSetSpriteRotation("frog", 0);  //���þ������ת�Ƕ�
    dSetSpriteFlipY( "frog" , 1);   //Y����ת
    dSetSpriteLinearVelocityY("frog", 0);     //y�����ٶ�
    dSetSpritePosition("GameOver" , 0 , -13 );
    dSetSpriteVisible("result1" , 1 );      //��ʾ���

    dSetTextValue("score1",score);
    dSetTextValue("score2",score);

    dSetSpritePosition("score1" , 7 , -1 );
    dSetSpritePosition("score2" , 7 , 3 );



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
    if(iKey == KEY_SPACE && g_iGameState == 0 )
    {
        g_iGameState = 1;

    }
    if(g_iGameState == 2)
    {
        if(iKey == KEY_SPACE)
        {
            dSetSpriteLinearVelocityY("frog", -40);
            dSetSpriteRotation("frog", -20);
            dPlaySound("flap.ogg", 0 , 1 );
        }
    }
}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{

}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    if( strcmp(szSrcName, "frog")==0 && strstr(szTarName, "zhuzi") != NULL )
    {
        dPlaySound("hurt5.ogg", 0 , 1 );
        g_iGameState =	0;
        isGameContinue = false;
        GameEnd();
    }

}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    if(strcmp(szName, "frog")==0 )
    {
        dOnFrogColWorldLimit(iColSide);
    }
    if(strstr(szName, "zhuzi") != NULL)
    {
        dOnZhuziColWorldLimit(iColSide);
    }
}

void dOnFrogColWorldLimit( const int iColSide )
{
    if(iColSide == 3)
    {
        g_iGameState =	0;
        isGameContinue = false;
        GameEnd();
    }
}

void dOnZhuziColWorldLimit( const int iColSide )
{
    if(iColSide == 0 && g_iGameState == 2 )
    {
        score++;
        dCreateZhuzi();
    }
}

void dCreateZhuzi()
{

    int heightZhuzi1 = dRandomRange( 17 , 42 );
    int heightZhuzi2 = heightZhuzi1-(35+distanceByZhuzi);

    dSetSpritePosition("zhuzi1", 24 , heightZhuzi1);
    dSetSpritePosition("zhuzi2", 24 , heightZhuzi2);

    dSetSpriteLinearVelocityX("zhuzi1", speedOfZhuzi);
    dSetSpriteLinearVelocityX("zhuzi2", speedOfZhuzi);

    if(distanceByZhuzi >= 30)
    {
        distanceByZhuzi--;
        if(speedOfZhuzi >= -25)
        {
            speedOfZhuzi--;
        }
    }
}

void dFrogFloat()
{
    dSetSpriteConstantForceY("frog", 2 );
    if(dGetSpriteLinearVelocityY("frog") == 5 )
    {
        dSetSpriteConstantForceY("frog", -2 );
    }
}

void dMyPlaySound()
{
    if(score % 3 == 0)
    {
        if(score % 6 == 0)
        {
            dPlaySound("score10.ogg" , 0 , 0.7 );
        }
        else
            dPlaySound("score8.ogg" , 0 , 0.5 );
    }
    else if(score % 5 == 0)
    {
        dPlaySound("hurt8.ogg" , 0 , 0.7 );
    }
}
