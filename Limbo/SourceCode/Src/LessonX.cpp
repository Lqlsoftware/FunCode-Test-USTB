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
// 陷阱类型定义
#define		TRAP_WATER			 1
//
// 游戏状态: 0 -- 加载 ; 1 -- 主菜单 ; 2 -- 教程 ; 3 -- 初始化游戏 ; 4 -- 游戏中 ; 5 -- 游戏暂停 ; 6 -- 游戏结束 ; 7 -- 职员表 ; 8 -- 选择关卡
int			g_iGameState		=	0;
//
//
// 全局变量定义
//
// 设置
float		userWaitTime = 0.0;						// 用户在读取界面的等待时间
bool		userNeedPressKey = false;				// 用户是否需要按键进入游戏
bool		didLoadMap = false;						// 判断是否加载了地图
float		creditWaitTime = 0;						// 职员表等待时间
//
// 菜单
int			mainMenuState = 1;						// 主菜单状态
int			mainMenuMusicID = 0;					// 主菜单背景音乐ID(用来结束音乐)
int			PauseState = 1;							// 暂停菜单状态
int			loadChapterState = 0;					// 关卡选择状态
//
// 游戏中
int			gravityG = 30;							// 主角重力
bool		didCol = false ;						// 判断主角是否与地面发生碰撞
bool		onGround = true ;						// 判断主角是否在地面上
bool		didInTrap = false;						// 检测是否进入陷阱
int			trapType = 0;							// 陷阱类型
int			boxTouch = 0;							// 判断主角与哪个箱子接触
bool		ctrlPress = false;						// 判断是否按下Ctrl
int			keyRightLeftPress = 0;					// 左右键是否按下---0 未按键 1 按下左键 2 按下右键
bool		didOnBox = false;						// 站在箱子上
bool		cStop = true;							// 主角是否静止
bool		firstOnLine = false;					// 判断主角是否是第一次与绳子接触 主角抓住绳子
bool		secondOnLine = false;					// 判断主角是否第二次与绳子接触	防止主角卡住
bool		onLine = false;							// 是否在绳子上
bool		didPlayStandAnimation = true;			// 是否播放了站立动画
int			runningSoundID = 0;						// 主角跑动时音乐ID
int			waterFluSoundID = 0;					// 流水声ID
bool		didPlayWaterSound = false;				// 检测是否已经开始播放流水声音
bool		didEnd = false;							// 判断是否到达终点
bool		didPlayEndAnimation = false;			// 记录是否播放结束动画
bool		characterDie = false;					// 主角是否死亡
int			checkPoint;								// 记录存储点
int			tCheckPoint = 0;						// 玩家通过的关卡信息
bool		didDown = false;						// 是否下斜坡了
//
//
// 常量定义
const int	characterMoveSpeed = 12;				// 主角移动的速度
const int	background1MoveSpeed = 11;				// 背景层1移动的速度
//
//
// 函数定义
void		Loading( float fDeltaTime );			// 设置Loading界面
void		setMainMenu();							// 设置主菜单界面
void		setPauseMenu();							// 设置暂停菜单界面
void		delPauseMenu();							// 删除暂停菜单界面
void		setTutorials();							// 设置教程界面
void		setGameWorld();							// 设置游戏世界
void		setCredit();							// 设置职员表界面
void		setLoadChapter();						// 设置选择关卡界面
void		setCamera();							// 设置镜头位置
void		setWaterSound();						// 设置流水声
void		checkTrap();							// 检测陷阱
void		checkBox(int boxNumber);				// 检测箱子
void		credit(float fDeltaTime);				// 处理职员表
void		MainMenuProcess(const int iKey);		// 处理主菜单的按键消息
void		LoadChapterProcess(const int iKey);		// 处理关卡选择界面的按键消息
void		PauseProcess(const int iKey);			// 处理游戏暂停时的按键消息
void		GameProcess(const int iKey,const bool bCtrlPress,bool KeyUpOrDown); // 处理游戏中的按键消息
void		characterMove(int direct);				// 角色移动
void		characterStop();						// 角色停止移动
void		staffMove(float velocity,int direct);	// 箱子等移动
void		setChangeAnimation(int x);				// 切换场景之间的过渡动画
void		delChangeAnimation();					// 过渡动画结束后删除
void		GameRun( float fDeltaTime );			// 游戏过程处理
void		GameEnd();								// 游戏结束处理
void		dCheckPoint();							// 判断角色死亡位置
//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{
	switch( g_iGameState )
	{
		// 显示加载中，加载主菜单UI，等待用户摁键进入
	case 0:
	{
		Loading(fDeltaTime);
		break;
	}
		// 等待用户选择进入游戏还是教程
	case 1:
	{
		// 渐变动画隐藏
		delChangeAnimation();
		break;
	}
		// 教程
	case 2:
	{
		// 渐变动画隐藏
		delChangeAnimation();
		break;
	}
		// 初始化游戏
	case 3:
	{
		if (dIsAnimateSpriteAnimationFinished("ChangeAnimation"))
		{
			setGameWorld();
			g_iGameState	=	4; // 初始化之后，将游戏状态设置为进行中
		}
	}
	break;
		// 游戏进行中，处理游戏逻辑
	case 4:
	{
		if (didEnd == false)
		// 渐变动画隐藏
			delChangeAnimation();
		// 游戏循环条件
		if( true )
		{
			GameRun( fDeltaTime );
		}
		else
		{
		// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			g_iGameState	=	5;
			GameEnd();
		}
	}
	break;
		// 游戏暂停
	case 5:
	{
		GameRun( fDeltaTime );
		break;
	}
		// 游戏结束
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
// 加载界面
void Loading(float	fDeltaTime)
{

	// 加载场景
	if (didLoadMap == false)
	{
		dLoadMap("Loading.t2d");
		// 令按键提示的字符隐藏
		dSetSpriteVisible("PressKeysText",0);

		// 读取玩家信息
			//创建文件指针
		FILE *fp = fopen("user", "r+");
			//检查是否正确读入文件
		if (!fp) {                            
			return;
		}
			// 读取玩家闯关信息
		if (!feof(fp))
		{
			fscanf(fp, "%d", &tCheckPoint);       
		}
		fclose(fp);	

		didLoadMap= true;
	}

	// 等待用户输入按键
	if (userWaitTime >= 3)
	{
		userNeedPressKey = true;
		dSetSpriteVisible("PressKeysText",1);
		return;
	}
	// 累计用户等待时间
	userWaitTime += fDeltaTime;
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
	// 检查点
	dCheckPoint();
	// 到达终点
	if (didEnd == true)
	{
		// 主角与船开向远方
		characterMove(0);
		dSetSpriteLinearVelocityX("Boat",0);
		dSetSpriteLinearVelocityY("Character",0);
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		// 停止脚步声
		dStopSound(runningSoundID);
		runningSoundID = 0;
		// 累加用户等待时间
		creditWaitTime += fDeltaTime;
		// 到达一定位置播放渐变动画
		if (dGetSpritePositionX("Character") - dGetSpritePositionX("Map") >= 833 && didPlayEndAnimation == false)
		{
			didPlayEndAnimation = true;
			setChangeAnimation(0);
		}
		// 等待5秒后开始播放职员表
		if (creditWaitTime >= 5)
		{
			didPlayEndAnimation = false;
			creditWaitTime = 0;
			setCredit();
		}
		return;
	}

	// 检测模块
	checkTrap();
	checkBox(1);
	checkBox(2);
	
	// 检测主角下落速度
	if (dGetSpriteLinearVelocityY( "Character") >= 50)
		characterDie = true;
	// 主角摔死了
	if ( characterDie == true && onGround == true )
	{
		dSetSpriteLinearVelocityY( "Character" , 0);
		dAnimateSpritePlayAnimation( "Character", "DieAnimation", 1 );
		GameEnd();
		characterDie = false;
	}

	// 设置镜头位置
	setCamera();

	// 设置流水声
	setWaterSound();

	// 主角竖直方向重力对速度的影响
	float characterDvy = dGetSpriteLinearVelocityY("Character");
	characterDvy += gravityG * fDeltaTime;
	dSetSpriteLinearVelocityY( "Character",characterDvy );

	// 在箱子上时
	if (didOnBox == true)
	{
		dSetSpriteLinearVelocityY( "Character",0 );
		dSetSpritePositionY( "Character",dGetSpritePositionY("Character")-0.01);
		didOnBox = false;
	}
	// 在地面上
	if (didCol)
	{
		dSetSpriteLinearVelocityY( "Character",-0.25 );
		dSetSpritePositionY( "Character",dGetSpritePositionY("Character")-0.25);
		didCol = false;
	}

	// 在地面且静止时播放静止动画
	if (onGround && cStop && didPlayStandAnimation)
	{
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		didPlayStandAnimation = false;
	}

	// 在空中时停止播放脚步声
	if (onGround == false)
	{
		dStopSound(runningSoundID);
		runningSoundID = 0;
	}
	// 在地面且移动时播放跑动声音
	if ( onGround && cStop == false && runningSoundID == 0)
	{
			runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
	}

	// 主角遇到障碍物停下
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
// 本局游戏结束
void GameEnd()
{
	// 重新加载游戏
	setChangeAnimation(0);
	g_iGameState = 3;
	mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{
}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{	
}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
	// 检测处于哪个游戏状态
	switch (g_iGameState){		
	// loading界面
	case 0:
		if(userNeedPressKey)
		{
			// 播放确认音效
			dPlaySound("ConfirmSound",0,1);

			// 设置主菜单
			g_iGameState = 1;
			setMainMenu();
		}
		break;
	// 主菜单
	case 1:
		MainMenuProcess(iKey);
		break;
	// 教程中按任意键返回
	case 2:
		setMainMenu();
		g_iGameState = 1;
		break;
	// 游戏世界加载
	case 3:
		break;
	// 游戏过程中
	case 4:
		GameProcess(iKey,bCtrlPress,true);
		break;
	// 暂停菜单
	case 5:
		PauseProcess(iKey);
		break;
	// 游戏结束
	case 6:
		break;
	// 职员表
	case 7:
		if(iKey == KEY_ESCAPE)
		{
			dStopAllSound();
			setMainMenu();
			g_iGameState = 1;
		}
		break;
	// 选择关卡界面
	case 8:
		LoadChapterProcess(iKey);
		break;
	};
}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
	// 检测处于哪个游戏状态
	switch (g_iGameState){		
	// 处理游戏中按键弹起
	case 4:
		GameProcess(iKey,0,false);
		break;
	default:
		break;
	};
}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// 检测是否进入陷阱
	if ( strstr(szSrcName,"Trap") != NULL && strcmp(szTarName,"Character")==0 )
	{
		// 进入陷阱
		didInTrap = true;
		if (strstr(szSrcName,"water") != NULL)
			trapType = TRAP_WATER;
		return;
	}

	// 检测绳子
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
			// 设置重力为0
			gravityG = 0;
		}
	}
	// 离开绳子
	if ( strcmp(szTarName,"lineCol") == 0 && strcmp(szSrcName,"Character")==0 )
	{
		// 重置重力状态
		gravityG = 30;
		onLine = false;
		cStop = true;
		dAnimateSpritePlayAnimation("Character", "CharacterRunningJumpAnimation", 0);
	}

	// 人物与平台地面发生碰撞
	if ( strcmp(szTarName,"Character") == 0 && strcmp(szSrcName,"Platform") == 0 )
	{
		didCol = true;
		onGround = true;
		// 重置绳子状态
		firstOnLine = false;
		secondOnLine = false;
	}
	// 人物与地面发生碰撞
	if ( strcmp(szTarName,"Character") == 0 )
	{
		didCol = true;
		onGround = true;
		// 重置绳子状态
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

	// 主角与箱子上边界碰撞
	if( strcmp(szSrcName,"Box_1") == 0 || strcmp( szSrcName,"Box_2") == 0 )
	{
		// 在箱子上
		didOnBox = true;
	}

	if( strcmp(szTarName,"Boat" ) == 0 && strcmp(szSrcName,"Character") == 0)
	{
		// 游戏结束
		didEnd = true;
	}
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
}
//===========================================================================
//
// 主菜单的按键消息处理
void MainMenuProcess(const int iKey)
{
	switch (iKey)
	{
	// 向上键
	case KEY_UP:
		// 由菜单1->菜单5
		if (mainMenuState == 1)
			{	
				dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation5", 0);
				mainMenuState = 5;
			}
		// 由菜单2->菜单1
		else if (mainMenuState == 2)
			{
				dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation1", 0);
				mainMenuState = 1;
			}
		// 由菜单3->菜单2
		else if (mainMenuState == 3)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation2", 0);
			mainMenuState = 2;
		}
		// 由菜单4->菜单3
		else if (mainMenuState == 4)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation3", 0);
			mainMenuState = 3;
		}
		// 由菜单5->菜单4
		else if (mainMenuState == 5)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation4", 0);
			mainMenuState = 4;
		}
		// 播放切换声音
		dPlaySound("MenuChangeSound",0,1);
		break;
	// 向下键
	case KEY_DOWN:
		// 由菜单1->菜单2
		if (mainMenuState == 1)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation2", 0);
			mainMenuState = 2;
		}
		// 由菜单2->菜单3
		else if (mainMenuState == 2)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation3", 0);
			mainMenuState = 3;
		}
		// 由菜单3->菜单4
		else if (mainMenuState == 3)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation4", 0);
			mainMenuState = 4;
		}
		// 由菜单4->菜单5
		else if (mainMenuState == 4)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation5", 0);
			mainMenuState = 5;
		}
		// 由菜单5->菜单1
		else if (mainMenuState == 5)
		{
			dAnimateSpritePlayAnimation("MainMenu", "MainMenuTextAnimation1", 0);
			mainMenuState = 1;
		}
		// 播放切换音效
		dPlaySound("MenuChangeSound",0,1);
		break;
	// 回车
	case KEY_ENTER:
		// 进入游戏
		if (mainMenuState == 1)
		{
			setChangeAnimation(0);
			g_iGameState = 3;
		}
		// 选择关卡
		else if (mainMenuState == 2)
		{
			setLoadChapter();
			g_iGameState = 8;
		}
		// 加载教程
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
		// 退出游戏
		else if (mainMenuState == 5)
		{	
			exit(0);
		}
		// 播放确认音效
		dPlaySound("ConfirmSound",0,1);
		break;
	}
}
//===========================================================================
//
// 选择关卡的按键消息处理
void LoadChapterProcess(const int iKey)
{
	switch (iKey)
	{
	// 向左键
	case KEY_LEFT:
		if (loadChapterState > 0)
		{
			// 提示未被选中
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation1",0);
			// 状态变为上一关
			loadChapterState = loadChapterState - 1;
			// 提示被选中
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation2",0);
			// 关卡图片移动
			dSpriteMoveTo("Chapter_0", -loadChapterState * 85, 2, 450, 1 );
			// 播放切换音效
			dPlaySound("MenuChangeSound",0,1);
		}
		break;
	// 向右键
	case KEY_RIGHT:
		if (loadChapterState < tCheckPoint)
		{
			// 提示未被选中
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation1",0);
			// 状态变为上一关
			loadChapterState = loadChapterState + 1;
			// 提示被选中
			dAnimateSpritePlayAnimation(dMakeSpriteName("Choose", loadChapterState),"ChooseAnimation2",0);
			// 关卡图片移动
			dSpriteMoveTo("Chapter_0", -loadChapterState * 85, 2, 450, 1 );
			// 播放切换音效
			dPlaySound("MenuChangeSound",0,1);
		}
		break;
	// 回车
	case KEY_ENTER:
		// 进入游戏章节
		setChangeAnimation(0);
		g_iGameState = 3;
		// 导入存储点
		checkPoint = loadChapterState;
		// 播放确认音效
		dPlaySound("ConfirmSound",0,1);
		break;
	// 回到主菜单
	case KEY_ESCAPE:
		setMainMenu();
		g_iGameState = 1;
		break;
	}
}
//===========================================================================
//
// 暂停菜单按键消息处理
void PauseProcess(const int iKey)
{	
	switch (iKey)
	{
	case KEY_UP:
		// 由菜单1->菜单3
		if (PauseState == 1)
			{	
				dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation3", 0);
				PauseState = 3;
			}
		// 由菜单2->菜单1
		else if (PauseState == 2)
			{
				dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
				PauseState = 1;
			}
		// 由菜单3->菜单2
		else if (PauseState == 3)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation2", 0);
			PauseState = 2;
		}
		dPlaySound("MenuChangeSound",0,1);
		break;
	case KEY_DOWN:
		// 由菜单1->菜单2
		if (PauseState == 1)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation2", 0);
			PauseState = 2;
		}
		// 由菜单2->菜单3
		else if (PauseState == 2)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation3", 0);
			PauseState = 3;
		}
		// 由菜单3->菜单1
		else if (PauseState == 3)
		{
			dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
			PauseState = 1;
		}
		// 播放切换音效
		dPlaySound("MenuChangeSound",0,1);
		break;
	case KEY_ENTER:
		// 继续游戏
		if (PauseState == 1)
		{
			delPauseMenu();
			g_iGameState = 4;
		}
		// 重新开始游戏
		else if (PauseState == 2)
		{
			dStopAllSound();
			mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
			checkPoint = 0;
			g_iGameState = 3;
		}
		// 返回主菜单
		else if (PauseState == 3)
		{	
			dStopAllSound();
			checkPoint = 0;
			setMainMenu();
			g_iGameState = 1;
		}
		// 播放确认音效
		dPlaySound("ConfirmSound",0,1);
		break;
	default:
		break;
	}
}
//===========================================================================
//
// 游戏中按键消息处理
void GameProcess(const int iKey,const bool bCtrlPress,bool KeyUpOrDown)
{
	// 游戏暂停
	if ( iKey == KEY_ESCAPE )
	{
		// 
		g_iGameState = 5;
		setPauseMenu();
		return;
	}
	// 判断按下还是弹起
	if ( KeyUpOrDown == true)
	{
		switch(iKey)
		{
		// 主角向右移动
		case KEY_RIGHT:
			keyRightLeftPress = 2;
			characterMove(0);
			break;
		// 主角向左移动
		case KEY_LEFT: 
			keyRightLeftPress = 1;
			characterMove(1);
			break;
		// 主角向上跳跃
		case KEY_UP:
			// 在绳子上向上爬
			if (onLine)
			{
				dAnimateSpritePlayAnimation("Character", "ClimbLineAnimation", 1);
				dSetSpriteLinearVelocityY( "Character", -10 );
			}
			// 向上跳
			else
			{
				characterMove(2);
			}
			break;
		case KEY_DOWN:
			// 在绳子上向下爬
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
		// 主角停止运动
		case KEY_RIGHT:
			characterStop();
			keyRightLeftPress = 0;
			break;
		case KEY_LEFT:
			keyRightLeftPress = 0;
			characterStop();
			break;
		case KEY_UP:
			// 在绳子上停止爬动
			if (onLine)
			{
				dSetSpriteLinearVelocityY( "Character", 0 );
			}
			break;
		case KEY_DOWN:
			// 在绳子上停止爬动
			if (onLine)
			{
				dSetSpriteLinearVelocityY( "Character", 0 );
			}
			break;
		}
	}
	// 主角推拉箱子
	if(bCtrlPress == 1 && boxTouch != 0)
	{
		// 拉动或推动箱子)
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
// 设置主菜单
void setMainMenu() 
{
	// 加载主菜单
	dLoadMap("MainMenu.t2d");
	// 播放渐变动画 -- 渐显
	setChangeAnimation(1);
	// 设置主菜单选项
	dAnimateSpritePlayAnimation( "MainMenu", "MainMenuTextAnimation1", 0 );
	mainMenuState = 1;
	// 背景音乐										     （是否循环，音量)
	mainMenuMusicID = dPlaySound( "MainMenu_0.wav", 1, 1 );
}
//==============================================================================
//
// 设置暂停菜单
void setPauseMenu()
{	
	dSetSpritePosition("PauseBackground",0,0);
	// 设置暂停菜单选项
	dSetSpritePosition("PauseMenu",-15,0);
	dAnimateSpritePlayAnimation("PauseMenu", "PauseMenuAnimation1", 0);
	PauseState = 1;
	// 令主角静止
	characterStop();
}
//==============================================================================
//
// 删除暂停菜单
void delPauseMenu()
{
	// 将暂停菜单移出屏幕
	dSetSpritePosition("PauseBackground",0,-75);
	dSetSpritePosition("PauseMenu",-15,-75);
}
//==============================================================================
//
// 设置教程
void setTutorials()
{
	// 加载教程界面
	dLoadMap("Tutorial.t2d");
	// 播放渐变动画 -- 渐显
	setChangeAnimation(1);
}
//===========================================================================
//
// 设置游戏世界
void setGameWorld()
{
	// 加载游戏世界
	dLoadMap("GameWorld.t2d");
	// 播放渐变动画 -- 渐显
	setChangeAnimation(1);
	// 设置主角
	dSetSpriteLinearVelocityX("Character",0);
	// 检查存档点
	switch (checkPoint)
	{
		// 存档点0
	case 0:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		break;
		// 存档点1
	case 1:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-70,0);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// 存档点2
	case 2:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-255,-57);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// 存档点3
	case 3:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		dSetSpritePosition("Map",-415,-31.5);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// 存档点4
	case 4:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,0.5);
		dSetSpritePosition("Map",-575,0);
		dSetSpritePosition("Background1_0",dGetSpritePositionX("Map") / 12 * 11 - 100,dGetSpritePositionY("Map"));
		dSetSpritePosition( "Box_1",dGetSpritePositionX("Map") + 303.5,dGetSpritePositionY("Map") + 75);
		dSetSpritePosition( "Box_2",dGetSpritePositionX("Map") + 640.5,dGetSpritePositionY("Map") + 0.5);
		dSetSpritePosition( "Boat",dGetSpritePositionX("Map") + 782,dGetSpritePositionY("Map") + 91);
		break;
		// 存档点5
	case 5:
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
		dSetSpritePosition("Character",0,17);
		break;
	}
	// 重置陷阱状态
	didInTrap = false;
	// 重置箱子状态
	boxTouch = 0;
	// 重置人物状态
	cStop = true;
	didPlayStandAnimation = true;
	characterDie = false;
	didDown = false;
	didEnd = false;
	// 重置绳子状态
	firstOnLine = false;
	secondOnLine = false;
	onLine = false;
	// 重置声音
	dStopSound(waterFluSoundID);
	waterFluSoundID = 0;
	dStopSound(runningSoundID);
	runningSoundID = 0;
	didPlayWaterSound = false;
}
//===========================================================================
//
// 设置职员表界面
void setCredit()
{
	// 加载地图
	dLoadMap("Credit.t2d");
	// 更改状态
	g_iGameState = 7;
	// 播放渐变动画
	setChangeAnimation(1);
	// 设置职员表速度
	dSetSpriteLinearVelocityY("Credit", -10);
	// 设置ESC提示
	dSetSpritePosition("ESC",-38,-27.5);
}
//===========================================================================
//
// 设置选择关卡界面
void setLoadChapter()
{
	// 加载地图
	dLoadMap("LoadChapter.t2d");
	// 播放渐变动画
	setChangeAnimation(1);
	loadChapterState = tCheckPoint;
	// 根据玩家信息设置关卡界面
	switch (tCheckPoint)
	{
	case 0:
		break;
	case 1:
		// 设置上方的关卡是否可选提示
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation2", 0 );
		// 设置关卡图片
		dSetSpritePosition("Chapter_0",-85,2);
		break;
	case 2:
		// 设置上方的关卡是否可选提示
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation2", 0 );
		// 设置关卡图片
		dSetSpritePosition("Chapter_0",-170,2);
		break;
	case 3:
		// 设置上方的关卡是否可选提示
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose3", "ChooseAnimation2", 0 );
		// 设置关卡图片
		dSetSpritePosition("Chapter_0",-255,2);
		break;
	case 4:
		// 设置上方的关卡是否可选提示
		dAnimateSpritePlayAnimation( "Choose0", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose1", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose2", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose3", "ChooseAnimation1", 0 );
		dAnimateSpritePlayAnimation( "Choose4", "ChooseAnimation2", 0 );
		// 设置关卡图片
		dSetSpritePosition("Chapter_0",-340,2);
		break;
	}
}
//===========================================================================
//
// 界面切换的渐变动画 x = 0 渐隐，x = 1渐显
void setChangeAnimation(int x)
{
	if (x)
	{
		// 将渐变动画放置在屏幕中央
		dSetSpritePosition("ChangeAnimation",0,0);
		dAnimateSpritePlayAnimation( "ChangeAnimation", "ChangeAnimation1", 0 );
	}
	else 
	{
		// 将渐变动画放置在屏幕中央
		dSetSpritePosition("ChangeAnimation",0,0);
		dAnimateSpritePlayAnimation( "ChangeAnimation", "ChangeAnimation2", 0 );
	}
}
//===========================================================================
//
// 播放结束后隐藏界面切换的渐变动画(移出屏幕)
void delChangeAnimation()
{
	if (dIsAnimateSpriteAnimationFinished( "ChangeAnimation" ))
	{
		// 将渐变动画移出屏幕
		dSetSpritePosition("ChangeAnimation",-100,0);
	}
}
//===========================================================================
//
// 主角移动	direct = 0 向右移动 direct = 1 向左移动 direct = 2 向上跳跃
void characterMove(int direct)
{
	cStop = false;
	// 向右移动
	if ( direct == 0 )
	{
		// 背景移动
		dSetSpriteLinearVelocityX( "Map", -characterMoveSpeed );
		dSetSpriteLinearVelocityX( "Background1_0", -background1MoveSpeed );
		staffMove(-characterMoveSpeed,1);
		// 设置主角方向是否翻转
		dSetSpriteFlipX( "Character", false );
		// 如果在拉箱子播放拉动箱子动画
		if ( ctrlPress == false && boxTouch != 0 )
		{
			if (dGetSpritePositionX("Character") < dGetSpritePositionX(dMakeSpriteName("Box_", boxTouch)))
				dAnimateSpritePlayAnimation( "Character", "PushBoxAnimation", 0 );
			else
				dAnimateSpritePlayAnimation( "Character", "PullBoxAnimation", 0 );
			return;
		}
		// 播放主角跑动动画
		else if (onGround)
		{
			if (runningSoundID == 0)
				runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
			dAnimateSpritePlayAnimation( "Character", "CharacterRunningAnimation", 1 );
		}
	}
	// 向左移动
	else if (direct == 1)
	{
		// 背景移动
		dSetSpriteLinearVelocityX( "Map", characterMoveSpeed );
		dSetSpriteLinearVelocityX( "Background1_0", background1MoveSpeed );
		staffMove(characterMoveSpeed,1);
		// 设置主角方向是否翻转
		dSetSpriteFlipX( "Character", true );

		// 如果在拉箱子播放拉动箱子动画
		if ( ctrlPress == false && boxTouch != 0 )
		{
			if (dGetSpritePositionX("Character") > dGetSpritePositionX(dMakeSpriteName("Box_", boxTouch)))
				dAnimateSpritePlayAnimation( "Character", "PushBoxAnimation", 1 );
			else
				dAnimateSpritePlayAnimation( "Character", "PullBoxAnimation", 1 );
			return;
		}
		// 播放主角跑动动画
		else if (onGround)
		{
			if (runningSoundID == 0)
				runningSoundID = dPlaySound("CharacterRunning.wav",1,1);
			dAnimateSpritePlayAnimation( "Character", "CharacterRunningAnimation", 1 );
		}
	}
	// 向上跳跃
	else
	{
		// 只有在地面时才能跳跃
		if( onGround )
		{
			dSetSpriteLinearVelocityY( "Character", - 20 );
			// 原地起跳
			if (dGetSpriteLinearVelocityX("Map") == 0 )
				dAnimateSpritePlayAnimation( "Character", "CharacterStandingJumpAnimation", 1 );
			// 移动起跳
			else	
				dAnimateSpritePlayAnimation( "Character", "CharacterRunningJumpAnimation", 0 );
			onGround = false;

		}
	}
	
}
//===========================================================================
//
// 主角停止
void characterStop()
{
	// 设置背景速度为0
	dSetSpriteLinearVelocityX( "Map", 0 );
	dSetSpriteLinearVelocityX( "Background1_0", 0 );
	staffMove(0,1);
	// 主角停止
	cStop = true;
	didPlayStandAnimation = true;
	dStopSound(runningSoundID);
	runningSoundID = 0;
	// 在地面时主角播放站立动画
	if (onGround)
		dAnimateSpritePlayAnimation( "Character", "CharacterStandAnimation", 1 );
}
//===========================================================================
//
// 箱子等移动 direct = 1 X方向 direct = 2 Y方向
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
// 设置镜头位置
void setCamera()
{
	// 主角位置在屏幕以上3/4时镜头上移
	if  (dGetSpritePositionY("Character") < -37.5 * 3/4)
	{
		dSetSpriteLinearVelocityY( "Character", 10 );
		dSetSpriteLinearVelocityY( "Map", 20 );
		dSetSpriteLinearVelocityY( "Background1_0", 20 );
		staffMove(20,2);
	}
	// 主角位置在屏幕以下3/4时镜头下移
	else if (dGetSpritePositionY("Character") > 37.5 * 3/4)
	{
		// 斜坡下降
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
		// 主角下降速度不快时
		if (characterDie == false)
			dSetSpriteLinearVelocityY( "Character", -5 );
		dSetSpriteLinearVelocityY( "Map", -10 );
		dSetSpriteLinearVelocityY( "Background1_0", -10 );
		staffMove(-10,2);
	}
	// 主角位置在屏幕中央
	else if (dGetSpritePositionY("Character") < 37.5 * 1/2 && dGetSpritePositionY("Character") > -37.5 * 1/2 || dGetSpritePositionY("Map") > 0 )
	{
		dSetSpriteLinearVelocityY( "Map", 0 );
		dSetSpriteLinearVelocityY( "Background1_0", 0 );
		staffMove(0,2);
	}
}
//===========================================================================
//
// 设置流水声
void setWaterSound()
{
	float cfposx = dGetSpritePositionX("Character");
	float mfposx = dGetSpritePositionX("Map");
	// 距离水源近的时候播放声音
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
		// 停止播放水声
			dStopSound(waterFluSoundID);
			didPlayWaterSound = false;
	}
}
//===========================================================================
//
// 检测是否进入陷阱
void checkTrap()
{
	if (didInTrap == true)
	{
		// 设置人物状态
		switch (trapType)
		{
		case 1:
			// 被水淹没
			dSetSpriteLinearVelocityY("Character",7);
			// 停止播放生意
			dStopAllSound();
			// 播放掉入水中音效
			dPlaySound("DropToWater.wav",0,1);
			// 重置背景音乐
			mainMenuMusicID = dPlaySound("MainMenu_0.wav",1,1);
			break;
		}
		// 进入游戏结束阶段
		g_iGameState = 6;
	}
}
//===========================================================================
//
// 检测是否与箱子接触
void checkBox(int boxNumber)
{
	// 得到箱子名称
	char* Name = dMakeSpriteName("Box_", boxNumber);
	// 获取人物与箱子的位置
	float cFposY = dGetSpritePositionY("Character");
	float cFposX = dGetSpritePositionX("Character");
	float bFposY = dGetSpritePositionY(Name);
	float bFposX = dGetSpritePositionX(Name);
	//判断主角是否与箱子接触
	if( fabs(cFposX - bFposX) < 4.5 && cFposX < bFposX && ctrlPress == false && bFposY - cFposY  < 5 )
	{
		if( keyRightLeftPress != 1 )
		{
			//主角与箱子接触
			characterStop();
			boxTouch = boxNumber;	
		}
	}
	else if( fabs(cFposX - bFposX) < 4.5 && cFposX > bFposX && ctrlPress == false && bFposY - cFposY < 5 )
	{
		if( keyRightLeftPress != 2 )
		{
			//主角与箱子接触
			characterStop();
			boxTouch = boxNumber;
		}
	}
	else
	{
		//主角与箱子不接触
		if (boxNumber == 1)
			boxTouch = 0;
	}
	
}
//===========================================================================
//
// 处理职员表
void credit(float fDeltaTime)
{
	// 播放渐变动画
	delChangeAnimation();
	// 到达底端停止滚动字幕
	if (dGetSpritePositionY("Credit") <= -141)
	{
		dSetSpriteLinearVelocityY("Credit",0);
		creditWaitTime += fDeltaTime;
	}
	// 等待5秒后回到主菜单
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
// 存档点检查
void dCheckPoint()
{
	float deltaPosX = dGetSpritePositionX("Character") - dGetSpritePositionX("Map");
	// 第1个储存点
	if (deltaPosX <= 229)
	{
		checkPoint = 1;
	}
	// 第2个储存点
	else if (deltaPosX <= 368)
	{
		checkPoint = 2;
	}
	// 第3个储存点
	else if (deltaPosX <= 538)
	{
		checkPoint = 3;
	}
	// 第4个储存点
	else if (deltaPosX <= 700)
	{
		checkPoint = 4;
	}
	// 记录通过关卡
	if (checkPoint > tCheckPoint)
	{
		FILE *fp = fopen("user", "w+");		 //创建文件指针
		if (!fp) {									  //检查是否正确创建文件
			return;
		}
		fprintf(fp, "%d", checkPoint);       //向文件中写入结果
		fclose(fp);
	}
}