/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "math.h"
#include "CommonAPI.h"
#include "LessonX.h"
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
// �������Ͷ���
#define		TRAP_WATER			 1
//
// ��Ϸ״̬: 0 -- ���� ; 1 -- ���˵� ; 2 -- �̳� ; 3 -- ��ʼ����Ϸ ; 4 -- ��Ϸ�� ; 5 -- ��Ϸ��ͣ ; 6 -- ��Ϸ���� ; 7 -- ְԱ�� ; 8 -- ѡ��ؿ�
int			g_iGameState		=	0;
//
//
// ȫ�ֱ�������
//
// ����
float		userWaitTime = 0.0;						// �û��ڶ�ȡ����ĵȴ�ʱ��
bool		userNeedPressKey = false;				// �û��Ƿ���Ҫ����������Ϸ
bool		didLoadMap = false;						// �ж��Ƿ�����˵�ͼ
float		creditWaitTime = 0;						// ְԱ��ȴ�ʱ��
//
// �˵�
int			mainMenuState = 1;						// ���˵�״̬
int			mainMenuMusicID = 0;					// ���˵���������ID(������������)
int			PauseState = 1;							// ��ͣ�˵�״̬
int			loadChapterState = 0;					// �ؿ�ѡ��״̬
//
// ��Ϸ��
int			gravityG = 30;							// ��������
bool		didCol = false ;						// �ж������Ƿ�����淢����ײ
bool		onGround = true ;						// �ж������Ƿ��ڵ�����
bool		didInTrap = false;						// ����Ƿ��������
int			trapType = 0;							// ��������
int			boxTouch = 0;							// �ж��������ĸ����ӽӴ�
bool		ctrlPress = false;						// �ж��Ƿ���Ctrl
int			keyRightLeftPress = 0;					// ���Ҽ��Ƿ���---0 δ���� 1 ������� 2 �����Ҽ�
bool		didOnBox = false;						// վ��������
bool		cStop = true;							// �����Ƿ�ֹ
bool		firstOnLine = false;					// �ж������Ƿ��ǵ�һ�������ӽӴ� ����ץס����
bool		secondOnLine = false;					// �ж������Ƿ�ڶ��������ӽӴ�	��ֹ���ǿ�ס
bool		onLine = false;							// �Ƿ���������
bool		didPlayStandAnimation = true;			// �Ƿ񲥷���վ������
int			runningSoundID = 0;						// �����ܶ�ʱ����ID
int			waterFluSoundID = 0;					// ��ˮ��ID
bool		didPlayWaterSound = false;				// ����Ƿ��Ѿ���ʼ������ˮ����
bool		didEnd = false;							// �ж��Ƿ񵽴��յ�
bool		didPlayEndAnimation = false;			// ��¼�Ƿ񲥷Ž�������
bool		characterDie = false;					// �����Ƿ�����
int			checkPoint;								// ��¼�洢��
int			tCheckPoint = 0;						// ���ͨ���Ĺؿ���Ϣ
bool		didDown = false;						// �Ƿ���б����
//
//
// ��������
const int	characterMoveSpeed = 12;				// �����ƶ����ٶ�
const int	background1MoveSpeed = 11;				// ������1�ƶ����ٶ�
//
//
// ��������
void		Loading( float fDeltaTime );			// ����Loading����
void		setMainMenu();							// �������˵�����
void		setPauseMenu();							// ������ͣ�˵�����
void		delPauseMenu();							// ɾ����ͣ�˵�����
void		setTutorials();							// ���ý̳̽���
void		setGameWorld();							// ������Ϸ����
void		setCredit();							// ����ְԱ�����
void		setLoadChapter();						// ����ѡ��ؿ�����
void		setCamera();							// ���þ�ͷλ��
void		setWaterSound();						// ������ˮ��
void		checkTrap();							// �������
void		checkBox(int boxNumber);				// �������
void		credit(float fDeltaTime);				// ����ְԱ��
void		MainMenuProcess(const int iKey);		// �������˵��İ�����Ϣ
void		LoadChapterProcess(const int iKey);		// ����ؿ�ѡ�����İ�����Ϣ
void		PauseProcess(const int iKey);			// ������Ϸ��ͣʱ�İ�����Ϣ
void		GameProcess(const int iKey,const bool bCtrlPress,bool KeyUpOrDown); // ������Ϸ�еİ�����Ϣ
void		characterMove(int direct);				// ��ɫ�ƶ�
void		characterStop();						// ��ɫֹͣ�ƶ�
void		staffMove(float velocity,int direct);	// ���ӵ��ƶ�
void		setChangeAnimation(int x);				// �л�����֮��Ĺ��ɶ���
void		delChangeAnimation();					// ���ɶ���������ɾ��
void		GameRun( float fDeltaTime );			// ��Ϸ���̴���
void		GameEnd();								// ��Ϸ��������
void		dCheckPoint();							// �жϽ�ɫ����λ��
//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// ��ʾ�����У��������˵�UI���ȴ��û���������
	case 0:
	{
		Loading(fDeltaTime);
		break;
	}
		// �ȴ��û�ѡ�������Ϸ���ǽ̳�
	case 1:
	{
		// ���䶯������
		delChangeAnimation();
		break;
	}
		// �̳�
	case 2:
	{
		// ���䶯������
		delChangeAnimation();
		break;
	}
		// ��ʼ����Ϸ
	case 3:
	{
		if (dIsAnimateSpriteAnimationFinished("ChangeAnimation"))
		{
			setGameWorld();
			g_iGameState	=	4; // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
	}
	break;
		// ��Ϸ�����У�������Ϸ�߼�
	case 4:
	{
		if (didEnd == false)
		// ���䶯������
			delChangeAnimation();
		// ��Ϸѭ������
		if( true )
		{
			GameRun( fDeltaTime );
		}
		else
		{
		// ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
			g_iGameState	=	5;
			GameEnd();
		}
	}
	break;
		// ��Ϸ��ͣ
	case 5:
	{
		GameRun( fDeltaTime );
		break;
	}
		// ��Ϸ����
	case 6:
	{
		GameEnd();
		break;
	}
	case 7:
	{
		credit(fDeltaTime);
		break;
	}
	default:
	break;
	};
}
//==============================================================================
//
// ���ؽ���
void Loading(float	fDeltaTime)
{

	// ���س���
	if (didLoadMap == false)
	{
		dLoadMap("Loading.t2d");
		// �����ʾ���ַ�����
		dSetSpriteVisible("PressKeysText",0);

		// ��ȡ�����Ϣ
			//�����ļ�ָ��
		FILE *fp = fopen("user", "r+");
			//����Ƿ���ȷ�����ļ�
		if (!fp) {                            
			return;
		}
			// ��ȡ��Ҵ�����Ϣ
		if (!feof(fp))
		{
			fscanf(fp, "%d", &tCheckPoint);       
		}
		fclose(fp);	

		didLoadMap= true;
	}

	// �ȴ��û����밴��
	if (userWaitTime >= 3)
	{
		userNeedPressKey = true;
		dSetSpriteVisible("PressKeysText",1);
		return;
	}
	// �ۼ��û��ȴ�ʱ��
	userWaitTime += fDeltaTime;
}
//==============================================================================
//
// ÿ����Ϸ������
void GameRun( float fDeltaTime )
{
	// ����
	dCheckPoint();
	// �����յ�
	if (didEnd == true)
	{
		// �����봬����Զ��
		characterMove(0);
		dSetSpriteLinearVelocityX("Boat",0);
		dSetSpriteLinearVelocityY("Character",0);
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		// ֹͣ�Ų���
		dStopSound(runningSoundID);
		runningSoundID = 0;
		// �ۼ��û��ȴ�ʱ��
		creditWaitTime += fDeltaTime;
		// ����һ��λ�ò��Ž��䶯��
		if (dGetSpritePositionX("Character") - dGetSpritePositionX("Map") >= 833 && didPlayEndAnimation == false)
		{
			didPlayEndAnimation = true;
			setChangeAnimation(0);
		}
		// �ȴ�5���ʼ����ְԱ��
		if (creditWaitTime >= 5)
		{
			didPlayEndAnimation = false;
			creditWaitTime = 0;
			setCredit();
		}
		return;
	}

	// ���ģ��
	checkTrap();
	checkBox(1);
	checkBox(2);
	
	// ������������ٶ�
	if (dGetSpriteLinearVelocityY( "Character") >= 50)
		characterDie = true;
	// ����ˤ����
	if ( characterDie == true && onGround == true )
	{
		dSetSpriteLinearVelocityY( "Character" , 0);
		dAnimateSpritePlayAnimation( "Character", "DieAnimation", 1 );
		GameEnd();
		characterDie = false;
	}

	// ���þ�ͷλ��
	setCamera();

	// ������ˮ��
	setWaterSound();

	// ������ֱ�����������ٶȵ�Ӱ��
	float characterDvy = dGetSpriteLinearVelocityY("Character");
	characterDvy += gravityG * fDeltaTime;
	dSetSpriteLinearVelocityY( "Character",characterDvy );

	// ��������ʱ
	if (didOnBox == true)
	{
		dSetSpriteLinearVelocityY( "Character",0 );
		dSetSpritePositionY( "Character",dGetSpritePositionY("Character")-0.01);
		didOnBox = false;
	}
	// �ڵ�����
	if (didCol)
	{
		dSetSpriteLinearVelocityY( "Character",-0.25 );
		dSetSpritePositionY( "Character",dGetSpritePositionY("Character")-0.25);
		didCol = false;
	}

	// �ڵ����Ҿ�ֹʱ���ž�ֹ����
	if (onGround && cStop && didPlayStandAnimation)
	{
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		didPlayStandAnimation = false;
	}

	// �ڿ���ʱֹͣ���ŽŲ���
	if (onGround == false)
	{
		dStopSound(runningSoundID);
		runningSoundID = 0;
	}
	// �ڵ������ƶ�ʱ�����ܶ�����
	if ( onGround && cStop == false && runningSoundID == 0)
	{
			runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
	}

	// ���������ϰ���ͣ��
	if ( dGetSpriteLinearVelocityX("Map") > 0 && dGetSpritePositionX("Character") - dGetSpritePositionX("Map") <= -88 )
		characterStop();
	if ( dGetSpriteLinearVelocityX("Map") > 0 && (dGetSpritePositionX("Character") - dGetSpritePositionX("Map") <= 191 && dGetSpritePositionX("Character") - dGetSpritePositionX("Map") >= 190.5) )
		characterStop();
	if ( dGetSpriteLinearVelocityX("Map") < 0 && (dGetSpritePositionX("Character") - dGetSpritePositionX("Map") <= 370 && dGetSpritePositionX("Character") - dGetSpritePositionX("Map") >= 360.5) )
		if (dGetSpritePositionY("Character") - dGetSpritePositionY("Map") >= 51 )
			characterStop();
	if ( dGetSpriteLinearVelocityX("Map") > 0 && (dGetSpritePositionX("Character") - dGetSpritePositionX("Map") <= 642.5 && dGetSpritePositionX("Character") - dGetSpritePositionX("Map") >= 635) )
		if (dGetSpritePositionY("Character") - dGetSpritePositionY("Map") >= 9.5 )
		characterStop();
	if (dGetSpritePositionX("Map") <= -190 && dGetSpritePositionX("Map") >= -190.5 && didDown == false)
	{
		dSetSpriteLinearVelocityX( "Map", 0 );
		dSetSpriteLinearVelocityX( "Background1_0", 0 );
		staffMove(0,1);
		dSetSpriteLinearVelocityY( "Map", 0 );
		dSetSpriteLinearVelocityY( "Background1_0", 0 );
		staffMove(0,2);
		didDown = true;
	}
	
}
//==============================================================================
//
// ������Ϸ����
void GameEnd()
{
	// ���¼�����Ϸ
	setChangeAnimation(0);
	g_iGameState = 3;
	mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
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
	// ��⴦���ĸ���Ϸ״̬
	switch (g_iGameState){		
	// loading����
	case 0:
		if(userNeedPressKey)
		{
			// ����ȷ����Ч
			dPlaySound("ConfirmSound",0,1);

			// �������˵�
			g_iGameState = 1;
			setMainMenu();
		}
		break;
	// ���˵�
	case 1:
		MainMenuProcess(iKey);
		break;
	// �̳��а����������
	case 2:
		setMainMenu();
		g_iGameState = 1;
		break;
	// ��Ϸ�������
	case 3:
		break;
	// ��Ϸ������
	case 4:
		GameProcess(iKey,bCtrlPress,true);
		break;
	// ��ͣ�˵�
	case 5:
		PauseProcess(iKey);
		break;
	// ��Ϸ����
	case 6:
		break;
	// ְԱ��
	case 7:
		if(iKey == KEY_ESCAPE)
		{
			dStopAllSound();
			setMainMenu();
			g_iGameState = 1;
		}
		break;
	// ѡ��ؿ�����
	case 8:
		LoadChapterProcess(iKey);
		break;
	};
}
//==========================================================================
//
// ���̵���
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
void OnKeyUp( const int iKey )
{
	// ��⴦���ĸ���Ϸ״̬
	switch (g_iGameState){		
	// ������Ϸ�а�������
	case 4:
		GameProcess(iKey,0,false);
		break;
	default:
		break;
	};
}
//===========================================================================
//
// �����뾫����ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// ����Ƿ��������
	if ( strstr(szSrcName,"Trap") != NULL && strcmp(szTarName,"Character")==0 )
	{
		// ��������
		didInTrap = true;
		if (strstr(szSrcName,"water") != NULL)
			trapType = TRAP_WATER;
		return;
	}

	// �������
	if(secondOnLine == false)
	{
		if (strcmp(szTarName,"Line") == 0 && strcmp(szSrcName,"Character")==0)
		{
			secondOnLine = true;
			if(firstOnLine == false)
			{
				characterStop();
				firstOnLine = true;
			}
			dSetSpriteLinearVelocityY( "Character", 0 );
			dAnimateSpritePlayAnimation("Character", "ClimbStillAnimation", 0);
			onLine = true;
			// ��������Ϊ0
			gravityG = 0;
		}
	}
	// �뿪����
	if ( strcmp(szTarName,"lineCol") == 0 && strcmp(szSrcName,"Character")==0 )
	{
		// ��������״̬
		gravityG = 30;
		onLine = false;
		cStop = true;
		dAnimateSpritePlayAnimation("Character", "CharacterRunningJumpAnimation", 0);
	}

	// ������ƽ̨���淢����ײ
	if ( strcmp(szTarName,"Character") == 0 && strcmp(szSrcName,"Platform") == 0 )
	{
		didCol = true;
		onGround = true;
		// ��������״̬
		firstOnLine = false;
		secondOnLine = false;
	}
	// ��������淢����ײ
	if ( strcmp(szTarName,"Character") == 0 )
	{
		didCol = true;
		onGround = true;
		// ��������״̬
		firstOnLine = false;
		secondOnLine = false;
		if (strcmp(dGetAnimateSpriteAnimationName("Character"),"CharacterRunningJumpAnimation") == 0)
		{
			if (keyRightLeftPress != 0)
				dAnimateSpritePlayAnimation("Character", "CharacterRunningAnimation", 0);
			else 
				dAnimateSpritePlayAnimation("Character", "CharacterStandAnimation", 0);
		}
	}

	// �����������ϱ߽���ײ
	if( strcmp(szSrcName,"Box_1") == 0 || strcmp( szSrcName,"Box_2") == 0 )
	{
		// ��������
		didOnBox = true;
	}

	if( strcmp(szTarName,"Boat" ) == 0 && strcmp(szSrcName,"Character") == 0)
	{
		// ��Ϸ����
		didEnd = true;
	}
}
//===========================================================================
//
// ����������߽���ײ
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
}
//===========================================================================
//
// ���˵��İ�����Ϣ����
void MainMenuProcess(const int iKey)
{
	switch (iKey)
	{
	// ���ϼ�
	case KEY_UP:
		// �ɲ˵�1->�˵�5
		if (mainMenuState == 1)
			{	
				dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation5", 0);
				mainMenuState = 5;
			}
		// �ɲ˵�2->�˵�1
		else if (mainMenuState == 2)
			{
				dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation1", 0);
				mainMenuState = 1;
			}
		// �ɲ˵�3->�˵�2
		else if (mainMenuState == 3)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation2", 0);
			mainMenuState = 2;
		}
		// �ɲ˵�4->�˵�3
		else if (mainMenuState == 4)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation3", 0);
			mainMenuState = 3;
		}
		// �ɲ˵�5->�˵�4
		else if (mainMenuState == 5)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation4", 0);
			mainMenuState = 4;
		}
		// �����л�����
		dPlaySound("MenuChangeSound",0,1);
		break;
	// ���¼�
	case KEY_DOWN:
		// �ɲ˵�1->�˵�2
		if (mainMenuState == 1)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation2", 0);
			mainMenuState = 2;
		}
		// �ɲ˵�2->�˵�3
		else if (mainMenuState == 2)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation3", 0);
			mainMenuState = 3;
		}
		// �ɲ˵�3->�˵�4
		else if (mainMenuState == 3)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation4", 0);
			mainMenuState = 4;
		}
		// �ɲ˵�4->�˵�5
		else if (mainMenuState == 4)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation5", 0);
			mainMenuState = 5;
		}
		// �ɲ˵�5->�˵�1
		else if (mainMenuState == 5)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation1", 0);
			mainMenuState = 1;
		}
		// �����л���Ч
		dPlaySound("MenuChangeSound",0,1);
		break;
	// �س�
	case KEY_ENTER:
		// ������Ϸ
		if (mainMenuState == 1)
		{
			setChangeAnimation(0);
			g_iGameState = 3;
		}
		// ѡ��ؿ�
		else if (mainMenuState == 2)
		{
			setLoadChapter();
			g_iGameState = 8;
		}
		// ���ؽ̳�
		else if (mainMenuState == 3)
		{
			setTutorials();
			g_iGameState = 2;
		}
		// Credit
		else if (mainMenuState == 4)
		{	
			setCredit();
		}
		// �˳���Ϸ
		else if (mainMenuState == 5)
		{	
			exit(0);
		}
		// ����ȷ����Ч
		dPlaySound("ConfirmSound",0,1);
		break;
	}
}
//===========================================================================
//
// ѡ��ؿ��İ�����Ϣ����
void LoadChapterProcess(const int iKey)
{
	switch (iKey)
	{
	// �����
	case KEY_LEFT:
		if (loadChapterState > 0)
		{
			// ��ʾδ��ѡ��
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation1",0);
			// ״̬��Ϊ��һ��
			loadChapterState = loadChapterState - 1;
			// ��ʾ��ѡ��
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation2",0);
			// �ؿ�ͼƬ�ƶ�
			dSpriteMoveTo("Chapter_0", -loadChapterState * 85, 2, 450, 1 );
			// �����л���Ч
			dPlaySound("MenuChangeSound",0,1);
		}
		break;
	// ���Ҽ�
	case KEY_RIGHT:
		if (loadChapterState < tCheckPoint)
		{
			// ��ʾδ��ѡ��
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation1",0);
			// ״̬��Ϊ��һ��
			loadChapterState = loadChapterState + 1;
			// ��ʾ��ѡ��
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation2",0);
			// �ؿ�ͼƬ�ƶ�
			dSpriteMoveTo("Chapter_0", -loadChapterState * 85, 2, 450, 1 );
			// �����л���Ч
			dPlaySound("MenuChangeSound",0,1);
		}
		break;
	// �س�
	case KEY_ENTER:
		// ������Ϸ�½�
		setChangeAnimation(0);
		g_iGameState = 3;
		// ����洢��
		checkPoint = loadChapterState;
		// ����ȷ����Ч
		dPlaySound("ConfirmSound",0,1);
		break;
	// �ص����˵�
	case KEY_ESCAPE:
		setMainMenu();
		g_iGameState = 1;
		break;
	}
}
//===========================================================================
//
// ��ͣ�˵�������Ϣ����
void PauseProcess(const int iKey)
{	
	switch (iKey)
	{
	case KEY_UP:
		// �ɲ˵�1->�˵�3
		if (PauseState == 1)
			{	
				dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation3", 0);
				PauseState = 3;
			}
		// �ɲ˵�2->�˵�1
		else if (PauseState == 2)
			{
				dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
				PauseState = 1;
			}
		// �ɲ˵�3->�˵�2
		else if (PauseState == 3)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation2", 0);
			PauseState = 2;
		}
		dPlaySound("MenuChangeSound",0,1);
		break;
	case KEY_DOWN:
		// �ɲ˵�1->�˵�2
		if (PauseState == 1)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation2", 0);
			PauseState = 2;
		}
		// �ɲ˵�2->�˵�3
		else if (PauseState == 2)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation3", 0);
			PauseState = 3;
		}
		// �ɲ˵�3->�˵�1
		else if (PauseState == 3)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
			PauseState = 1;
		}
		// �����л���Ч
		dPlaySound("MenuChangeSound",0,1);
		break;
	case KEY_ENTER:
		// ������Ϸ
		if (PauseState == 1)
		{
			delPauseMenu();
			g_iGameState = 4;
		}
		// ���¿�ʼ��Ϸ
		else if (PauseState == 2)
		{
			dStopAllSound();
			mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
			checkPoint = 0;
			g_iGameState = 3;
		}
		// �������˵�
		else if (PauseState == 3)
		{	
			dStopAllSound();
			checkPoint = 0;
			setMainMenu();
			g_iGameState = 1;
		}
		// ����ȷ����Ч
		dPlaySound("ConfirmSound",0,1);
		break;
	default:
		break;
	}
}
//===========================================================================
//
// ��Ϸ�а�����Ϣ����
void GameProcess(const int iKey,const bool bCtrlPress,bool KeyUpOrDown)
{
	// ��Ϸ��ͣ
	if ( iKey == KEY_ESCAPE )
	{
		// 
		g_iGameState = 5;
		setPauseMenu();
		return;
	}
	// �жϰ��»��ǵ���
	if ( KeyUpOrDown == true)
	{
		switch(iKey)
		{
		// ���������ƶ�
		case KEY_RIGHT:
			keyRightLeftPress = 2;
			characterMove(0);
			break;
		// ���������ƶ�
		case KEY_LEFT: 
			keyRightLeftPress = 1;
			characterMove(1);
			break;
		// ����������Ծ
		case KEY_UP:
			// ��������������
			if (onLine)
			{
				dAnimateSpritePlayAnimation("Character", "ClimbLineAnimation", 1);
				dSetSpriteLinearVelocityY( "Character", -10 );
			}
			// ������
			else
			{
				characterMove(2);
			}
			break;
		case KEY_DOWN:
			// ��������������
			if (onLine)
			{
				dAnimateSpritePlayAnimation("Character", "ClimbLineAnimation", 1);
				dSetSpriteLinearVelocityY( "Character", 10 );
			}
			break;
		}
	}
	else if ( KeyUpOrDown == false )
	{
		switch(iKey)
		{
		// ����ֹͣ�˶�
		case KEY_RIGHT:
			characterStop();
			keyRightLeftPress = 0;
			break;
		case KEY_LEFT:
			keyRightLeftPress = 0;
			characterStop();
			break;
		case KEY_UP:
			// ��������ֹͣ����
			if (onLine)
			{
				dSetSpriteLinearVelocityY( "Character", 0 );
			}
			break;
		case KEY_DOWN:
			// ��������ֹͣ����
			if (onLine)
			{
				dSetSpriteLinearVelocityY( "Character", 0 );
			}
			break;
		}
	}
	// ������������
	if(bCtrlPress == 1 && boxTouch != 0)
	{
		// �������ƶ�����)
		dSetSpriteLinearVelocityX(dMakeSpriteName("Box_", boxTouch), 0 );
		ctrlPress = true;
	}
	else if( bCtrlPress == 0)
	{	
		ctrlPress = false;
	}
}
//==============================================================================
//
// �������˵�
void setMainMenu() 
{
	// �������˵�
	dLoadMap("MainMenu.t2d");
	// ���Ž��䶯�� -- ����
	setChangeAnimation(1);
	// �������˵�ѡ��
	dAnimateSpritePlayAnimation( "MainMenu", "MainMenuTextAnimation1", 0 );
	mainMenuState = 1;
	// ��������										     ���Ƿ�ѭ��������)
	mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
}
//==============================================================================
//
// ������ͣ�˵�
void setPauseMenu()
{	
	dSetSpritePosition("PauseBackground",0,0);
	// ������ͣ�˵�ѡ��
	dSetSpritePosition("PauseMenu",-15,0);
	dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
	PauseState = 1;
	// �����Ǿ�ֹ
	characterStop();
}
//==============================================================================
//
// ɾ����ͣ�˵�
void delPauseMenu()
{
	// ����ͣ�˵��Ƴ���Ļ
	dSetSpritePosition("PauseBackground",0,-75);
	dSetSpritePosition("PauseMenu",-15,-75);
}
//==============================================================================
//
// ���ý̳�
void setTutorials()
{
	// ���ؽ̳̽���
	dLoadMap("Tutorial.t2d");
	// ���Ž��䶯�� -- ����
	setChangeAnimation(1);
}
//===========================================================================
//
// ������Ϸ����
void setGameWorld()
{
	// ������Ϸ����
	dLoadMap("GameWorld.t2d");
	// ���Ž��䶯�� -- ����
	setChangeAnimation(1);
	// ��������
	dSetSpriteLinearVelocityX("Character",0);
	// ���浵��
	switch (checkPoint)
	{
		// �浵��0
	case 0:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		break;
		// �浵��1
	case 1:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-70,0);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// �浵��2
	case 2:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-255,-57);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// �浵��3
	case 3:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-415,-31.5);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// �浵��4
	case 4:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,0.5);
		dSetSpritePosition("Map",-575,0);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// �浵��5
	case 5:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		break;
	}
	// ��������״̬
	didInTrap = false;
	// ��������״̬
	boxTouch = 0;
	// ��������״̬
	cStop = true;
	didPlayStandAnimation = true;
	characterDie = false;
	didDown = false;
	didEnd = false;
	// ��������״̬
	firstOnLine = false;
	secondOnLine = false;
	onLine = false;
	// ��������
	dStopSound(waterFluSoundID);
	waterFluSoundID = 0;
	dStopSound(runningSoundID);
	runningSoundID = 0;
	didPlayWaterSound = false;
}
//===========================================================================
//
// ����ְԱ�����
void setCredit()
{
	// ���ص�ͼ
	dLoadMap("Credit.t2d");
	// ����״̬
	g_iGameState = 7;
	// ���Ž��䶯��
	setChangeAnimation(1);
	// ����ְԱ���ٶ�
	dSetSpriteLinearVelocityY("Credit", -10);
	// ����ESC��ʾ
	dSetSpritePosition("ESC",-38,-27.5);
}
//===========================================================================
//
// ����ѡ��ؿ�����
void setLoadChapter()
{
	// ���ص�ͼ
	dLoadMap("LoadChapter.t2d");
	// ���Ž��䶯��
	setChangeAnimation(1);
	loadChapterState = tCheckPoint;
	// ���������Ϣ���ùؿ�����
	switch (tCheckPoint)
	{
	case 0:
		break;
	case 1:
		// �����Ϸ��Ĺؿ��Ƿ��ѡ��ʾ
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation2", 0 );
		// ���ùؿ�ͼƬ
		dSetSpritePosition("Chapter_0",-85,2);
		break;
	case 2:
		// �����Ϸ��Ĺؿ��Ƿ��ѡ��ʾ
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation2", 0 );
		// ���ùؿ�ͼƬ
		dSetSpritePosition("Chapter_0",-170,2);
		break;
	case 3:
		// �����Ϸ��Ĺؿ��Ƿ��ѡ��ʾ
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose3", "ChooseAnimation2", 0 );
		// ���ùؿ�ͼƬ
		dSetSpritePosition("Chapter_0",-255,2);
		break;
	case 4:
		// �����Ϸ��Ĺؿ��Ƿ��ѡ��ʾ
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose3", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose4", "ChooseAnimation2", 0 );
		// ���ùؿ�ͼƬ
		dSetSpritePosition("Chapter_0",-340,2);
		break;
	}
}
//===========================================================================
//
// �����л��Ľ��䶯�� x = 0 ������x = 1����
void setChangeAnimation(int x)
{
	if (x)
	{
		// �����䶯����������Ļ����
		dSetSpritePosition("ChangeAnimation",0,0);
		dAnimateSpritePlayAnimation( "ChangeAnimation", "ChangeAnimation1", 0 );
	}
	else 
	{
		// �����䶯����������Ļ����
		dSetSpritePosition("ChangeAnimation",0,0);
		dAnimateSpritePlayAnimation( "ChangeAnimation", "ChangeAnimation2", 0 );
	}
}
//===========================================================================
//
// ���Ž��������ؽ����л��Ľ��䶯��(�Ƴ���Ļ)
void delChangeAnimation()
{
	if (dIsAnimateSpriteAnimationFinished( "ChangeAnimation" ))
	{
		// �����䶯���Ƴ���Ļ
		dSetSpritePosition("ChangeAnimation",-100,0);
	}
}
//===========================================================================
//
// �����ƶ�	direct = 0 �����ƶ� direct = 1 �����ƶ� direct = 2 ������Ծ
void characterMove(int direct)
{
	cStop = false;
	// �����ƶ�
	if ( direct == 0 )
	{
		// �����ƶ�
		dSetSpriteLinearVelocityX( "Map", -characterMoveSpeed );
		dSetSpriteLinearVelocityX( "Background1_0", -background1MoveSpeed );
		staffMove(-characterMoveSpeed,1);
		// �������Ƿ����Ƿ�ת
		dSetSpriteFlipX( "Character", false );
		// ����������Ӳ����������Ӷ���
		if ( ctrlPress == false && boxTouch != 0 )
		{
			if (dGetSpritePositionX("Character") < dGetSpritePositionX(dMakeSpriteName("Box_", boxTouch)))
				dAnimateSpritePlayAnimation( "Character", "PushBoxAnimation", 0 );
			else
				dAnimateSpritePlayAnimation( "Character", "PullBoxAnimation", 0 );
			return;
		}
		// ���������ܶ�����
		else if (onGround)
		{
			if (runningSoundID == 0)
				runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
			dAnimateSpritePlayAnimation( "Character", "CharacterRunningAnimation", 1 );
		}
	}
	// �����ƶ�
	else if (direct == 1)
	{
		// �����ƶ�
		dSetSpriteLinearVelocityX( "Map", characterMoveSpeed );
		dSetSpriteLinearVelocityX( "Background1_0", background1MoveSpeed );
		staffMove(characterMoveSpeed,1);
		// �������Ƿ����Ƿ�ת
		dSetSpriteFlipX( "Character", true );

		// ����������Ӳ����������Ӷ���
		if ( ctrlPress == false && boxTouch != 0 )
		{
			if (dGetSpritePositionX("Character") > dGetSpritePositionX(dMakeSpriteName("Box_", boxTouch)))
				dAnimateSpritePlayAnimation( "Character", "PushBoxAnimation", 1 );
			else
				dAnimateSpritePlayAnimation( "Character", "PullBoxAnimation", 1 );
			return;
		}
		// ���������ܶ�����
		else if (onGround)
		{
			if (runningSoundID == 0)
				runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
			dAnimateSpritePlayAnimation( "Character", "CharacterRunningAnimation", 1 );
		}
	}
	// ������Ծ
	else
	{
		// ֻ���ڵ���ʱ������Ծ
		if( onGround )
		{
			dSetSpriteLinearVelocityY( "Character", - 20 );
			// ԭ������
			if (dGetSpriteLinearVelocityX("Map") == 0 )
				dAnimateSpritePlayAnimation( "Character", "CharacterStandingJumpAnimation", 1 );
			// �ƶ�����
			else	
				dAnimateSpritePlayAnimation( "Character", "CharacterRunningJumpAnimation", 0 );
			onGround = false;

		}
	}
	
}
//===========================================================================
//
// ����ֹͣ
void characterStop()
{
	// ���ñ����ٶ�Ϊ0
	dSetSpriteLinearVelocityX( "Map", 0 );
	dSetSpriteLinearVelocityX( "Background1_0", 0 );
	staffMove(0,1);
	// ����ֹͣ
	cStop = true;
	didPlayStandAnimation = true;
	dStopSound(runningSoundID);
	runningSoundID = 0;
	// �ڵ���ʱ���ǲ���վ������
	if (onGround)
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
}
//===========================================================================
//
// ���ӵ��ƶ� direct = 1 X���� direct = 2 Y����
void staffMove(float velocity,int direct)
{
	if (direct == 1)
	{
		dSetSpriteLinearVelocityX( "Box_1", velocity );
		dSetSpriteLinearVelocityX( "Box_2", velocity );
		dSetSpriteLinearVelocityX( "Boat", velocity );
	}
	if (direct == 2)
	{
		dSetSpriteLinearVelocityY( "Box_1", velocity );
		dSetSpriteLinearVelocityY( "Box_2", velocity );
		dSetSpriteLinearVelocityY( "Boat", velocity );
	}
}
//===========================================================================
//
// ���þ�ͷλ��
void setCamera()
{
	// ����λ������Ļ����3/4ʱ��ͷ����
	if  (dGetSpritePositionY("Character") < -37.5 * 3/4)
	{
		dSetSpriteLinearVelocityY( "Character", 10 );
		dSetSpriteLinearVelocityY( "Map", 20 );
		dSetSpriteLinearVelocityY( "Background1_0", 20 );
		staffMove(20,2);
	}
	// ����λ������Ļ����3/4ʱ��ͷ����
	else if (dGetSpritePositionY("Character") > 37.5 * 3/4)
	{
		// б���½�
		if ( dGetSpritePositionX("Map") <= -166 && dGetSpritePositionX("Map") >= -189)
		{
			if (onGround)
			{
				dSetSpriteLinearVelocityY( "Character", -5 );
				dSetSpriteLinearVelocityY( "Map", -23 );
				dSetSpriteLinearVelocityY( "Background1_0", -23 );
				staffMove(-23,2);
				if (dGetSpritePositionX("Map") > -190)
				{
					dSetSpriteLinearVelocityX( "Map", -15 );
					dSetSpriteLinearVelocityX( "Background1_0", -15 );
					staffMove(-15,1);
				}
			}
			return;
		}
		// �����½��ٶȲ���ʱ
		if (characterDie == false)
			dSetSpriteLinearVelocityY( "Character", -5 );
		dSetSpriteLinearVelocityY( "Map", -10 );
		dSetSpriteLinearVelocityY( "Background1_0", -10 );
		staffMove(-10,2);
	}
	// ����λ������Ļ����
	else if (dGetSpritePositionY("Character") < 37.5 * 1/2 && dGetSpritePositionY("Character") > -37.5 * 1/2 || dGetSpritePositionY("Map") > 0 )
	{
		dSetSpriteLinearVelocityY( "Map", 0 );
		dSetSpriteLinearVelocityY( "Background1_0", 0 );
		staffMove(0,2);
	}
}
//===========================================================================
//
// ������ˮ��
void setWaterSound()
{
	float cfposx = dGetSpritePositionX("Character");
	float mfposx = dGetSpritePositionX("Map");
	// ����ˮԴ����ʱ�򲥷�����
	if (cfposx - mfposx > 190 && cfposx - mfposx < 255)
	{
		if (didPlayWaterSound == false)
		{
			waterFluSoundID = dPlaySound("Waterflu.wav",1,1);
			didPlayWaterSound = true;
		}
	}
	else if (cfposx - mfposx > 473 && cfposx - mfposx < 560)
	{
		if (didPlayWaterSound == false)
		{
			waterFluSoundID = dPlaySound("Waterflu.wav",1,1);
			didPlayWaterSound = true;
		}
	}
	else if (cfposx - mfposx > 736)
	{
		if (didPlayWaterSound == false)
		{
			waterFluSoundID = dPlaySound("Waterflu.wav",1,1);
			didPlayWaterSound = true;
		}
	}
	else if (didPlayWaterSound == true)
	{
		// ֹͣ����ˮ��
			dStopSound(waterFluSoundID);
			didPlayWaterSound = false;
	}
}
//===========================================================================
//
// ����Ƿ��������
void checkTrap()
{
	if (didInTrap == true)
	{
		// ��������״̬
		switch (trapType)
		{
		case 1:
			// ��ˮ��û
			dSetSpriteLinearVelocityY("Character",7);
			// ֹͣ��������
			dStopAllSound();
			// ���ŵ���ˮ����Ч
			dPlaySound("DropToWater.wav",0,1);
			// ���ñ�������
			mainMenuMusicID = dPlaySound("MainMenu_0.wav",1,1);
			break;
		}
		// ������Ϸ�����׶�
		g_iGameState = 6;
	}
}
//===========================================================================
//
// ����Ƿ������ӽӴ�
void checkBox(int boxNumber)
{
	// �õ���������
	char* Name = dMakeSpriteName("Box_", boxNumber);
	// ��ȡ���������ӵ�λ��
	float cFposY = dGetSpritePositionY("Character");
	float cFposX = dGetSpritePositionX("Character");
	float bFposY = dGetSpritePositionY(Name);
	float bFposX = dGetSpritePositionX(Name);
	//�ж������Ƿ������ӽӴ�
	if( fabs(cFposX - bFposX) < 4.5 && cFposX < bFposX && ctrlPress == false && bFposY - cFposY  < 5 )
	{
		if( keyRightLeftPress != 1 )
		{
			//���������ӽӴ�
			characterStop();
			boxTouch = boxNumber;	
		}
	}
	else if( fabs(cFposX - bFposX) < 4.5 && cFposX > bFposX && ctrlPress == false && bFposY - cFposY < 5 )
	{
		if( keyRightLeftPress != 2 )
		{
			//���������ӽӴ�
			characterStop();
			boxTouch = boxNumber;
		}
	}
	else
	{
		//���������Ӳ��Ӵ�
		if (boxNumber == 1)
			boxTouch = 0;
	}
	
}
//===========================================================================
//
// ����ְԱ��
void credit(float fDeltaTime)
{
	// ���Ž��䶯��
	delChangeAnimation();
	// ����׶�ֹͣ������Ļ
	if (dGetSpritePositionY("Credit") <= -141)
	{
		dSetSpriteLinearVelocityY("Credit",0);
		creditWaitTime += fDeltaTime;
	}
	// �ȴ�5���ص����˵�
	if (creditWaitTime >= 5)
	{
		dStopAllSound();
		setMainMenu();
		g_iGameState = 1;
		creditWaitTime = 0;
		didEnd = false;
	}
}
//===========================================================================
//
// �浵����
void dCheckPoint()
{
	float deltaPosX = dGetSpritePositionX("Character") - dGetSpritePositionX("Map");
	// ��1�������
	if (deltaPosX <= 229)
	{
		checkPoint = 1;
	}
	// ��2�������
	else if (deltaPosX <= 368)
	{
		checkPoint = 2;
	}
	// ��3�������
	else if (deltaPosX <= 538)
	{
		checkPoint = 3;
	}
	// ��4�������
	else if (deltaPosX <= 700)
	{
		checkPoint = 4;
	}
	// ��¼ͨ���ؿ�
	if (checkPoint > tCheckPoint)
	{
		FILE *fp = fopen("user", "w+");		 //�����ļ�ָ��
		if (!fp) {									  //����Ƿ���ȷ�����ļ�
			return;
		}
		fprintf(fp, "%d", checkPoint);       //���ļ���д����
		fclose(fp);
	}
}