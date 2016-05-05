
#include "stdafx.h"
#include "Result.h"


CResult::CResult(T_RESULT* res)
:m_RowCount(-1), m_pResult(res), m_pCurrentRow(NULL)
{}

CResult::~CResult()
{
	Close();
}

int CResult::Next()
{
	T_ROW row;

	if (m_pResult == NULL)
		return 0;

	row = mysql_fetch_row(m_pResult);
	if (!row)
	{
		m_pCurrentRow = NULL;
		return 0;
	}
	else
	{
		m_pCurrentRow = new CRow(m_pResult, row);
		return 1;
	}
}

CRow* CResult::GetCurrentRow()
{
	if (m_pResult == NULL)
		return NULL;

	return m_pCurrentRow;
}

void CResult::Close()
{
	if (m_pResult == NULL)
		return;

	mysql_free_result(m_pResult);

	m_pResult = NULL;
}

int CResult::GetRowCount()
{
	if (m_pResult == (T_RESULT*)NULL)
		return 0;


	m_RowCount = mysql_num_rows(m_pResult);

	if (m_RowCount >0)
		return m_RowCount;
	else
		return 0;
}