/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#define	BLOCK_COUNT	4		// N * N �ľ��󷽿�,һ��N�Ĵ�С
////////////////////////////////////////////////////////////////////////////////
//
//
int  IsGameWin();
int			g_iGameState		=	0;		// ��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����Ϸ��2 -- ��Ϸ������
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

// 	�������С���ڱ༭����ڷŵĵ�һ�鷽�����ʼ����
const float	g_fBlockStartX		=	-40.625f;
const float	g_fBlockStartY		=	-28.125f;
// 	��Ļ�߶�75 / 4�� = 18.75ÿ��Ĵ�С.�༭����Ԥ�Ȱڷźõķ����͸�  //	�������ֵһ��
const float		g_fBlockSize		=	18.75f;

int	iLoopX		= 0, iLoopY		= 0, iLoop = 0;
int	iOneIndex	= 0, iRandIndex = 0;

int	iDataCount	=	BLOCK_COUNT * BLOCK_COUNT - 1;
int	iRandData[BLOCK_COUNT * BLOCK_COUNT - 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

int XYToOneIndex( const int iIndexX, const int iIndexY )
{
	return ((iIndexX * BLOCK_COUNT) + iIndexY);
}
int OneIndexToX( const int iIndex )
{
    return (iIndex / BLOCK_COUNT);
}
// һά��������ת������ά��������Y��ע����2�������С����һ��
int OneIndexToY( const int iIndex )
{
    return (iIndex % BLOCK_COUNT);
}

// 	��Ϸ״̬��0 -- ��Ϸ�����ȴ���ʼ״̬��1 -- ���¿ո����ʼ����ʼ����//	Ϸ��2 -- ��Ϸ������

//	��ά���飬�洢N*N�ľ��󷽿���Ϣ
int	g_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
//	һά���飬�洢�����ά�����еķ��龫������֡�TO DO ˼��һ����	    //	���Сһ���Ķ�ά����������һά������������໥ת����
char	g_szBlockName[BLOCK_COUNT * BLOCK_COUNT][64];


void MoveSpriteToBlock( const char *szName, const int iIndexX, const int iIndexY )
{
    float	fPosX	=	g_fBlockStartX + iIndexX * g_fBlockSize;
    float	fPosY	=	g_fBlockStartY + iIndexY * g_fBlockSize;
    dSetSpritePosition( szName, fPosX, fPosY );
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
    for( iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++ )
        {
            for( iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++ )
            {
                iOneIndex	=	XYToOneIndex( iLoopX, iLoopY );

                // ��������һ��
                if( BLOCK_COUNT - 1 == iLoopX && BLOCK_COUNT - 1 == iLoopY )
                {
                    g_iBlockState[iLoopY][iLoopX] = 0;
                    g_szBlockName[iOneIndex][0] = '\0';
                }
                else
                {
                    // �ڵ�ǰʣ��δʹ�õ�����ֵ�����һ������,��ֵ����ά����
                    iRandIndex	=	dRandomRange( 0, iDataCount - 1 );
                    g_iBlockState[iLoopY][iLoopX] = iRandData[iRandIndex];
                    strcpy( g_szBlockName[iOneIndex], dMakeSpriteName( "PictureBlock", 	g_iBlockState[iLoopY][iLoopX] ) );
                    // ���þ����ƶ�����Ӧ��λ��
                    MoveSpriteToBlock( g_szBlockName[iOneIndex], iLoopX, iLoopY );
                }
                for( iLoop = iRandIndex; iLoop < iDataCount - 1; iLoop++ )
                {
                    iRandData[iLoop] = iRandData[iLoop + 1];
                }
                // ʣ����Чֵ������һ
                iDataCount--;
            }
        }

        for( iLoop = iRandIndex; iLoop < iDataCount - 1; iLoop++ )
        {
            iRandData[iLoop] = iRandData[iLoop + 1];
        }
        // ʣ����Чֵ������һ
        iDataCount--;
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
    g_iGameState = IsGameWin();
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
    if( 2 != g_iGameState )
        return;
    int	iClickIndex	=	-1;
    int	iLoop	=	0;
    for( iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++ )
    {
        if( '\0' == g_szBlockName[iLoop][0] )
            continue;
        // ʹ��API dIsPointInSprite �ж�ָ�������Ƿ�λ��ĳ�����ֵľ����ڲ�
        if( dIsPointInSprite( g_szBlockName[iLoop], fMouseX, fMouseY ) )
        {
            iClickIndex	=	iLoop;
            break;
        }
    }
    // �ж�����Ƿ���з���
    if( -1 == iClickIndex )
        return;
    // ����һά�����Indexת���ɶ�ά�����X,Y
    int	iIndexX	=	OneIndexToX( iClickIndex );
    int	iIndexY	=	OneIndexToY( iClickIndex );
    // TODO �ڶ�ά���������������ķ�����������4���������Ƿ��п�λ��
    // ע��߽��жϣ�����������ʻ�Խ�硣�����ж����ʱ����Ҫ�ж��Ƿ��Ѿ���	//����ߵ�����(iIndexX == 0)
    // ����п�λ(ֵΪ0)���򽫸ÿ�λ��������ֵ��������2������
    int	iEmptyIndexX = -1, iEmptyIndexY = -1;

    // X ����(4���������Ҫ�ж��Ƿ���λ�ڱ�Ե��iIndexX > 0 ���������)
    if( iIndexX > 0 )
    {
        if( 0 == g_iBlockState[iIndexY][iIndexX - 1] )
        {
            iEmptyIndexX = iIndexX - 1;
            iEmptyIndexY = iIndexY;
        }
    }
    // X �ҷ���
    if( -1 == iEmptyIndexX && iIndexX < BLOCK_COUNT - 1 )
    {
        if( 0 == g_iBlockState[iIndexY][iIndexX + 1] )
        {
        iEmptyIndexX = iIndexX + 1;
        iEmptyIndexY = iIndexY;
        }
    }
    // Y �Ϸ���
    if( -1 == iEmptyIndexY && iIndexY > 0 )
    {
        if( 0 == g_iBlockState[iIndexY - 1][iIndexX] )
        {
            iEmptyIndexX = iIndexX;
            iEmptyIndexY = iIndexY - 1;
        }
    }
    // Y �·���
    if( -1 == iEmptyIndexY && iIndexY < BLOCK_COUNT - 1 )
    {
        if( 0 == g_iBlockState[iIndexY + 1][iIndexX] )
        {
            iEmptyIndexX = iIndexX;
            iEmptyIndexY = iIndexY + 1;
        }
    }
    // �ж��Ƿ��ҵ���λ
    if( -1 == iEmptyIndexX || -1 == iEmptyIndexY )
    return;
    // �п�λ���ڶ�ά���������������Ӧ��ֵ���н���
    g_iBlockState[iEmptyIndexY][iEmptyIndexX] = g_iBlockState[iIndexY][iIndexX];
    g_iBlockState[iIndexY][iIndexX]	=	0;
    // ��Ӧ������Ҳ���н���
    int	iOneIndex	=	XYToOneIndex( iEmptyIndexX, iEmptyIndexY );
    strcpy( g_szBlockName[iOneIndex], g_szBlockName[iClickIndex] );
    g_szBlockName[iClickIndex][0]	=	'\0';
    // ���þ����ƶ�����Ӧ��λ��
    MoveSpriteToBlock( g_szBlockName[iOneIndex], iEmptyIndexX, iEmptyIndexY );

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
		g_iGameState = 1;
        dSetSpriteVisible("GameBegin" , false);
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

}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}

int  IsGameWin()
{
    int	iLoopX	= 0, iLoopY = 0;
	int	iResult	= 1;
	for( iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++ )
	{
		for( iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++ )
		{
			// ��������һ��
			if( BLOCK_COUNT - 1 == iLoopX && BLOCK_COUNT - 1 == iLoopY )
				break;
			// ����һ��ֵ�����ڣ���ô��û��ʤ��
			if( g_iBlockState[iLoopY][iLoopX] != iResult )
				return 2;
			iResult++;
		}
	}
	return 0;
}
