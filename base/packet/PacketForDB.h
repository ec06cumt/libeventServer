
#pragma once 
#include <Windows.h>
#include <assert.h>

#define MAX_PACKETDB_SIZE 8192

class CPacketForDB
{
public:
	CPacketForDB();
	~CPacketForDB();

	const bool HasData() const { return m_bHasData; };
	const TCHAR* GetData() { return m_pData; };
	const UINT16 GetLength() const { return m_iLen; };
	void Init(const unsigned int iPacketID);

	void SetKey(unsigned short usKey );

	void PushBack(const LPVOID pData,const unsigned short iLen);
	void PushBack(const TCHAR* pData, const unsigned short iLen);

	void PushBack(const TCHAR value);
	void PushBack(const short value);
	void PushBack(const int value);
	void PushBack(const long value);
	void PushBack(const float value);
	void PushBack(const double value);

	void PushBack(const unsigned char value);
	void PushBack(const unsigned short value);
	void PushBack(const unsigned int value);
	void PushBack(const unsigned long value);
	void PushBack(const SYSTEMTIME timeDate);
	void PushBack(const INT64 value);
	void PushBack(const UINT64 value);

protected:
	unsigned short m_iLen;
	unsigned short m_iKey;
	TCHAR* m_pBuffer;
	TCHAR* m_pData;
	bool m_bHasData;

	void InitPacketDB();


};