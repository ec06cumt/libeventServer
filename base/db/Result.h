
#pragma once
#include<windows.h>
#include "Row.h"
class CResult
{
public:
	CResult(T_RESULT*);
	~CResult();

	void Close();
	CRow* GetCurrentRow();
	int GetRowCount();
	int Next();

private:
	int m_RowCount;
	T_RESULT* m_pResult;
	CRow* m_pCurrentRow;
};