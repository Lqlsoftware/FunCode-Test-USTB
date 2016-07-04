//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#define dtime 0.01
///////////////////////////////////////////////////////////////////////////////////////////
//
// ���������
//
//////////////////////////////////////////////////////////////////////////////////////////
float g_fSpeedLeft=0.f;  	// �����ٶ�
float g_fSpeedRight=0.f;  	// ��
float g_fSpeedTop=0.f;  	// ��
float g_fSpeedBottom=0.f;  	// ��

float g_fScreenLeft	= 0.f;    // ��Ļ��߽�ֵ
float g_fScreenRight = 0.f;    // ��
float g_fScreenTop = 0.f;    // ��
float g_fScreenBottom =	0.f;    // ��

void Swimming()
{
	if((g_fSpeedLeft + g_fSpeedRight) > 0)
		dSetSpriteFlipX("myfish", false);
	else if((g_fSpeedLeft + g_fSpeedRight) < 0)
		dSetSpriteFlipX("myfish", true);

	dSetSpriteLinearVelocity("myfish", g_fSpeedLeft + g_fSpeedRight, g_fSpeedTop + g_fSpeedBottom);
}


int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// ��ʼ����Ϸ����

	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : �ڴ�ʹ��API���Ĵ��ڱ���
	dSetWindowTitle("seaGameByJeason");

	// ������ѭ����������Ļͼ��ˢ�µȹ���
	while( dEngineMainLoop() )
	{

		// ��ȡ���ε���֮���ʱ�����ݸ���Ϸ�߼�����
		float	fTimeDelta	=	dGetTimeDelta();


		for(int i=0; i<4; i++)
        {
            char  szName[128];
            sprintf(szName, "fish%d", i);  // ע�⣬szName����ʱ�����ǿ�ָ��
        }


		// ִ����Ϸ��ѭ��
		GameMainLoop( fTimeDelta );
	};

	// �ر���Ϸ����
	dShutdownGameEngine();
	return 0;
}

//==========================================================================
//
// ���沶׽����ƶ���Ϣ�󣬽����õ�������
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// ���沶׽�������Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽��굯����Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// ���沶׽���̰�����Ϣ�󣬽����õ�������
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
//

float nextSpeed(float lastSpeed)
{
    float nextSpeed = lastSpeed + ( dtime * 5 );
    return nextSpeed;

}

void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{

    switch(iKey)
    {
    case KEY_W:
        g_fSpeedTop = -25.f;
        break;
    case KEY_A:
        g_fSpeedLeft = -30.f;
        break;
    case KEY_S:
        g_fSpeedBottom = 25.f;
        break;
    case KEY_D:
        g_fSpeedRight = 30.f;
        break;
    }

    Swimming();

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}
//==========================================================================
//
// ���沶׽���̵�����Ϣ�󣬽����õ�������
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
//
void dOnKeyUp( const int iKey )
{

    switch(iKey)
    {
        case KEY_W:
            g_fSpeedTop = 0.f;
            break;
        case KEY_A:
            g_fSpeedLeft = 0.f;
            break;
        case KEY_S:
            g_fSpeedBottom = 0.f;
            break;
        case KEY_D:
            g_fSpeedRight = 0.f;
            break;
    }

    Swimming();

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyUp(iKey);
}

//===========================================================================
//
// ���沶׽�������뾫����ײ֮�󣬵��ô˺���
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��鷢�ͼ�������ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// ���沶׽������������߽���ײ֮�󣬵��ô˺���.
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��������߽�����
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����

    g_fScreenLeft = dGetScreenLeft();
    g_fScreenRight = dGetScreenRight();
    g_fScreenTop = dGetScreenTop();
    g_fScreenBottom = dGetScreenBottom();

	for(int i=0; i<5; i++)
    {
        char  szName[128];
        float fPosY;
        bool bFlip;
        float fSpeedX;

        if(strstr(szName, "fish")!=NULL)
        {
            if(iColSide == 1)  // �ұ�
            {
                bFlip = true;
                fSpeedX = -dRandomRange(10, 20);	// ����ٶ�
            }
            else if(iColSide == 0)   // ���
            {
                bFlip = false;
                fSpeedX = dRandomRange(10, 20);     // ����ٶ�
            }
            dSetSpriteFlipX(szName, bFlip);
            dSetSpriteLinearVelocity(szName, fSpeedX, 0);
            // ÿ�ε�ͷʱ��λ���������
            fPosY = dRandomRange(g_fScreenTop+10.f, g_fScreenBottom-10.f);
            dSetSpritePositionY(szName, fPosY);

        }
        //dSetSpriteWorldLimit(szName, g_fScreenLeft-20.f, g_fScreenTop, g_fScreenRight+20.f,g_fScreenBottom);
    }

	OnSpriteColWorldLimit(szName, iColSide);
}

