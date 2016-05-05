
#include "..\stdafx.h"
#include "JobDBMsg.h"

CJobDBMsg::CJobDBMsg() :m_uiMsgID(0), m_stCreateTime(0L)
{

}

CJobDBMsg::~CJobDBMsg()
{

}

void CJobDBMsg::Init(const unsigned int uiPacketID)
{
	m_uiMsgID = 0;
	m_stCreateTime = 0;
	CPacketForDB::Init(uiPacketID);
}

