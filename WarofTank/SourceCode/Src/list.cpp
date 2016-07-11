#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"



void AddToList(Weapon *pSprite)
{
		Weapon* pTemp = (Weapon*)malloc(sizeof(Weapon)) ;
		strcpy(pTemp->szName, pSprite->szName);
		pTemp->fPosX = pSprite->fPosX;
		pTemp->fPosY = pSprite->fPosY;
		pTemp->fSpeedX = pSprite->fSpeedX;
		pTemp->fSpeedY = pSprite->fSpeedY;
		pTemp->iDir = pSprite->iDir;
		pTemp->iType = pSprite->iType;
		pTemp->pNext = NULL;

		Weapon* pNode = g_pHead;

		if(g_pHead == NULL) // ��ͷΪ�գ�˵��������ǵ�һ���ڵ�
		{
			g_pHead = pTemp;
		}
		else{
			// ѭ��������������һ���ڵ�
			while(pNode->pNext != NULL)
			{
				pNode = pNode->pNext;
			}
			pNode->pNext = pTemp;
		}
}
