
#include "stdafx.h"
#include "Row.h"

CRow::CRow(T_RESULT* pRes, T_ROW row) :m_pResult(pRes), m_Fields(row)
{

}

CRow::~CRow()
{
	if (!IsClosed())
		Close();
}

void CRow::Close()
{
	if (IsClosed())
		return;
	m_Fields = NULL;
	m_pResult = NULL;
}

int CRow::GetFieldCount()
{
	if (IsClosed())
		throw "Row closed.";

	return mysql_num_fields(m_pResult);

}

char* CRow::GetField(int field)
{
	if (IsClosed())
		return NULL;

	if (field<0 || field>GetFieldCount() - 1)
		return NULL;

	return m_Fields[field];
}

int CRow::IsClosed() const
{
	return (m_Fields == NULL);
};

