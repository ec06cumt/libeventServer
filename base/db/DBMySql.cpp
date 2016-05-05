
#include "stdafx.h"
#include "DBMySql.h"

CDBMySql::CDBMySql(const char* host, const char* dataBase) :m_iAffectedRows(0)
{
	m_pConnection = NULL;
	Connect(host, dataBase, NULL, NULL);
}

CDBMySql::CDBMySql(const char* host, const char* dataBase,
	const char* userID, const char* password)
	:m_iAffectedRows(0)
{

	m_pConnection = NULL;
	Connect(host, dataBase, userID, password);
}

int CDBMySql::Connect(const char* host, const char* dataBase,
	const char* userID, const char* password)
{
	int state = -1;

	if (IsConnected())
		return 0;		//已经连接上

	mysql_init(&m_Mysql);
	m_pConnection = mysql_real_connect(&m_Mysql, host, userID, password,
		dataBase, 0, NULL, 0);

	if (!IsConnected())
		return -1;	//连接失败

	if (state <0 )
		return -2;	//连接状态失败

	return 0;
}

CDBMySql::~CDBMySql()
{
	if (IsConnected())
		Close();
}

void CDBMySql::Close()
{
	if (!IsConnected())
		return;

	mysql_close(m_pConnection);
	m_pConnection = NULL;
}

CResult* CDBMySql::Query(const char* sqlCommand)
{
	T_RESULT* result = NULL;
	int state;

	if (!IsConnected())
		return NULL;

	if (!m_pConnection)
	{
		return NULL;
	}

	state = mysql_query(m_pConnection, sqlCommand);

	if (state != 0)
		return NULL;;

	result = mysql_store_result(m_pConnection);

	//if the result was null,it was an update or an error occurred
	if (result == NULL)
	{

		int fieldCount = mysql_field_count(m_pConnection);
		if (fieldCount != 0)
			return NULL;
		else
			m_iAffectedRows = mysql_affected_rows(m_pConnection);

		//return NULL for updates
		return NULL;
	}
	//return a Result instance for queries
	return new CResult(result);
}

int CDBMySql::GetAffectedRows() const
{
	return m_iAffectedRows;
}

const char* CDBMySql::GetError()
{
	if (IsConnected())
		return mysql_error(m_pConnection);
	else
		return mysql_error(&m_Mysql);
}

int CDBMySql::IsConnected() const
{
	return (m_pConnection != NULL);
}