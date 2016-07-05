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
int	g_iGameState = 0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void GameInit();
void GameRun( float fDeltaTime );
void GameEnd();

const char	*szGoldName	=	NULL;

char g_szCurGetGold[64];	// ��ǰץȡ���Ľ�������
float g_fEmptyHookSpeed; // �յĹ��ӵ��ƶ��ٶ�

int finalScore = 0;

// ץȡ����״̬��0 -- �ȴ�������ʼץ���ӣ�1 -- �������ȥ��2 -- ��������// �죬δץ��������3 -- ���������죬���Ž���
int g_iGetGoldState	;
// ���ӵ�ǰת���ĳ���
float g_fHookRotation;
// ���ӵ�ǰ������ڶ��������Ұڶ�( 1 ��-�� 0 ��-�� )
int g_iHookRotToLeft;
// ���ӳ�ʼλ�á��ڸտ�ʼ��ʱ���ʼ��һ�Σ����ڹ��Ӹ�λ
float g_fHookStartPosX;
float g_fHookStartPosY;

// �������ɵĽ���������ÿץȡһ�����ӣ������ݼ�һ����Ϊ0ʱ���ֽ���
int	g_iGoldCount;
	//�����ĸ�����Ϊ�����ܳ����ڵ�ͼ�ϵ��������ұ߽����ֵ����Сֵ
int	g_iGoldBornMinX	;
int	g_iGoldBornMaxX	;
int	g_iGoldBornMinY	;
int	g_iGoldBornMaxY	;


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
    g_iGoldBornMinX	=	0;
    g_iGoldBornMaxX	=	0;
    g_iGoldBornMinY	=	0;
    g_iGoldBornMaxY	=	0;
    g_iGoldCount = 20;

    g_fEmptyHookSpeed = 15.f;

    // ���±���ֻ��Ҫ��ʼ��һ��
	static int	iInitedHookPos	=	0;
	if( 0 == iInitedHookPos )
	{
	    //���ӿ��Է�����ײ
        dSetSpriteCollisionSend("GoldHook",true);
        //���ӿ��Խ�����ײ
        dSetSpriteCollisionReceive("GoldTemplate",true);

		iInitedHookPos		=	1;
		// ���ӳ�ʼλ��ֵ��ʼ��
		g_fHookStartPosX	=	dGetSpritePositionX( "GoldHook" );
		g_fHookStartPosY	=	dGetSpritePositionY( "GoldHook" );

		// ���ӿ��Գ��ֵı߽緶Χ��ʼ��
		g_iGoldBornMinX		=	dGetScreenLeft() + 5;
		g_iGoldBornMaxX		=	dGetScreenRight() - 5;
		g_iGoldBornMinY		=	dGetScreenTop() + 20;
		g_iGoldBornMaxY		=	dGetScreenBottom() - 5;
	}

	int	iLoop	=	0;
    char	*szName	=	NULL;
    int	iSize =	4, iPosX = 0, iPosY = 0;
    for( iLoop = 0; iLoop < g_iGoldCount; iLoop++ )
    {
        if( iLoop < 10 )
        {
            iSize	=	4;

        }
        else if( iLoop >= 10 && iLoop < 16 )
        {
            iSize	=	6;

        }
        else
        {
            iSize	=	8;

        }
        szName	=	dMakeSpriteName( "GoldBlock", iLoop );

        dCloneSprite( "GoldTemplate", szName );
        dSetSpriteWidth( szName, (float)iSize );
        dSetSpriteHeight( szName, (float)iSize );
        // ���һ��λ��
        iPosX	=	dRandomRange( g_iGoldBornMinX, g_iGoldBornMaxX );
        iPosY	=	dRandomRange( g_iGoldBornMinY, g_iGoldBornMaxY );
        dSetSpriteMass(szName,5*iSize);
        dSetSpritePosition( szName, (float)iPosX, (float)iPosY );
    }


    // �����ڽ��ߵĶ���(�ָ���ʼ�غ���)
    dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation2", 0 );
    // ���� ��Ϸ��ʼ����ʾ
    dSetSpriteVisible( "GameBegin", 0 );
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
    if( 0 == g_iGetGoldState )
	{
    // ҡ���ٶȣ���λ ��/��
		const  float	fRotateSpeed	=	45.f;
    // ������ת�Ķ���
		float fThisRotate = fRotateSpeed * fDeltaTime;
		if( g_iHookRotToLeft )
		{
			g_fHookRotation	+= fThisRotate;
			if( g_fHookRotation >= 180.f )
			{
				g_fHookRotation		=	180.f;
				g_iHookRotToLeft	=	0;
			}
		}
		else
		{
			g_fHookRotation	-= fThisRotate;
			if( g_fHookRotation <= 0.f )
			{
				g_fHookRotation		=	0.f;
				g_iHookRotToLeft	=	1;
			}
		}
		dSetSpriteRotation( "GoldHook", g_fHookRotation );
	}

	// �����ǰΪ�������ع�λ�����ж��Ƿ��Ѿ��˶����յ㡣�����յ㣬���ֿ�ʼҡ// �ڡ��ȴ�
    else if( 2 == g_iGetGoldState || 3 == g_iGetGoldState )
    {
		// �ж��Ƿ��ƶ����յ�(�жϵ�������XY������ƶ��ٶ�Ϊ0���������Ƿ�		// �Ѿ�ֹͣ�ƶ�)
		float	fSpeedX	=	dGetSpriteLinearVelocityX( "GoldHook" );
		float	fSpeedY	=	dGetSpriteLinearVelocityY( "GoldHook" );
		// ��ǰ�ٶȲ�Ϊ0�������˶���
		// �������Ƿ�Ϊ0������ֱ���ж� == != 0
		if( fSpeedX > 0.00001f || fSpeedX < -0.00001f || fSpeedY > 0.00001f || fSpeedY < -0.00001f )
        {

            return;
            // �ٶ�Ϊ0�����ӻص���ʼ�㣬��ʼ��һ�ֵ�ץȡ
            // ��ǰץȡ�����ӣ���ץȡ���Ľ����ͷŲ�ɾ��֮��Ȼ�󽫽���������һ
        }

		if( 3 == g_iGetGoldState )
		{
		    finalScore += dGetSpriteMass(szGoldName);
            dSetTextValue("score",finalScore);
            //��������ӷ���
			g_iGoldCount--;
			dSpriteDismount( g_szCurGetGold );
			dDeleteSprite( g_szCurGetGold );
		}
		// �ָ��ȴ�״̬�벥�ŵȴ�����
		g_iGetGoldState	=	0;
		dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation2", 0 );
	}


}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
    dSetSpriteVisible( "GameBegin", 1 );
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
    if( KEY_SPACE == iKey && 0 == g_iGameState )
    {
        g_iGameState	=	1;
    }
    // ��ǰ������Ϸ�����У��������µķ�������������ץȡ����
	else if( KEY_DOWN == iKey && 2 == g_iGameState && 0 == g_iGetGoldState )
    {
		// ����ץȡ״̬Ϊ������������
		g_iGetGoldState	=	1;
		// �Ե�ǰ���������һ����ǰ���ٶ�
		dSetSpriteLinearVelocityPolar("GoldHook",g_fEmptyHookSpeed, g_fHookRotation );
		// �����ڽ��ߵĶ���(һ���첲����ѹ�Ķ���)
		dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation1", 0 );
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
    // �����жϹ����Ƿ�������Ѿ���Ϸ�Ƿ����ڽ��У������Ƴ�������
    if( 2 != g_iGameState || 1 != g_iGetGoldState )
        return;
    //�ж��Ƿ��ǹ�������ӽ�����ײ(�������������ַ��������һ���ǽ�������һ	//���ǹ�����)
    if( stricmp( "GoldHook", szSrcName ) != 0 && stricmp( "GoldHook", szTarName ) != 0 )
        return;;
    // �ҵ��ĸ��ǽ��ӵ�����
    //const char	*szGoldName	=	NULL;
    if( strstr( szSrcName, "GoldBlock" ) )
        szGoldName = szSrcName;
    else if( strstr( szTarName, "GoldBlock" ) )
        szGoldName = szTarName;
    else
        return;

    // �����ӹҽӵ����ӵ�2�Źҽӵ���
    dSpriteMountToSpriteLinkPoint( szGoldName, "GoldHook", 2 );
    // ����ץȡ״̬Ϊ�����������գ�ץȡ����������¼��������
    g_iGetGoldState	=	3;
    strcpy( g_szCurGetGold, szGoldName );
    // ���ݽ��Ӵ�С�����㹳�ӵ������յ��ٶȣ�Խ���Խ����
    // �㷨��֮ǰ���õĽ��Ӵ�СΪ4,6,8����10��ȥ�ô�С�ٳ���10���õ�3��С// ����0.6,0.4,0.2.��С�����Կչ��ӵ��ٶȼ��õ��ҽӲ�ͬ���Ӻ��ʵ���ٶ�
    float	fWidth	=	dGetSpriteWidth( szGoldName );
    float	fSpeed	=	((10.f - fWidth) / 10.f) * g_fEmptyHookSpeed;
    // ��������ʼλ���ƶ�
    dSpriteMoveTo( "GoldHook", g_fHookStartPosX, g_fHookStartPosY, fSpeed, 1 );
    // �����ڽ��ߵĶ���(�첲���ض��Ķ���)
    dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation3", 0 );

}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    // �ж��Ƿ��ǹ��������߽磬������
    if( stricmp( szName, "GoldHook" ) == 0 )
    {
        // ����ץȡ״̬Ϊ�����������գ�δץȡ������
        g_iGetGoldState	=	2;
        // ��������ʼλ���ƶ�
        dSpriteMoveTo( "GoldHook", g_fHookStartPosX, g_fHookStartPosY, 				g_fEmptyHookSpeed, 1 );
        // �����ڽ��ߵĶ���(�첲���ض��Ķ���)
        dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation3", 0 );
     }
}
