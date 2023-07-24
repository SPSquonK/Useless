#include "StdAfx.h"
#include "Query.h"

#ifdef __MONITORSRVR
#include "..\monitor\dbmanager.h"
#endif	// __MONITORSRVR

SQLINTEGER CQuery::sqlNts = SQL_NTS;

extern "C"
{
	#include "des.h"
}

void Decrpyt( unsigned char* pEncrypted, char* szResult )
{
	static unsigned char keys[3][8] =
	{
		{ 0x32, 0x89, 0x45, 0x9A, 0x89, 0xAB, 0xCD, 0x11 },
		{ 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01 },
		{ 0x1F, 0x67, 0x89, 0x54, 0xCD, 0xEF, 0x00, 0x23 }
	};

    des3_context ctx3;
    des3_set_3keys( &ctx3, keys[0], keys[1], keys[2] );

    unsigned char* input = pEncrypted;
    unsigned char* result = (unsigned char*)szResult;
	for( int i=0; i<3; i++ )
		des3_decrypt( &ctx3, input + i*8, result + i*8 );
}

BOOL GetPWDFromToken( const char* szToken, char* szPWD )
{
	constexpr auto ToInt = [](char c) -> BYTE {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		return 0;
	};
	int nCount = strlen( szToken ) / 2;
	if (nCount <= 0) return FALSE;

	BYTE byPWD[256];
	char szBuf[3] = {0, };
	for( int i=0; i<nCount; i++ )
	{
		memcpy( szBuf, szToken + i*2, 2 );						
		byPWD[i] = ToInt(szBuf[0]) * 0x10 + ToInt(szBuf[1]);
	}
	Decrpyt( byPWD, szPWD );
	return TRUE;
}

CQuery::ColString::ColString() {
	m_size = 8182;
	m_buffer = std::make_unique<char[]>(m_size);
}

int CQuery::ColString::AsInt() const { return std::atoi(m_buffer.get()); }
std::int64_t CQuery::ColString::AsInt64() const { return _atoi64(m_buffer.get()); }
float CQuery::ColString::AsFloat() const { return static_cast<float>(std::atof(m_buffer.get())); }
char CQuery::ColString::AsChar() const { return m_buffer[0]; }
const char * CQuery::ColString::GetRawString() const { return m_buffer.get(); }

std::pair<char *, SQLLEN> CQuery::ColString::GetInfoForBind() {
	return std::pair<char *, SQLLEN>(m_buffer.get(), static_cast<SQLLEN>(m_size));
}

// �ı���:���� �ڵ��� �����Ѵ�.
CQuery::~CQuery() { DisConnect(); }

void CQuery::DisConnect() {
	if (hStmt)	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);	hStmt = nullptr;
	if (hDbc)	SQLDisconnect(hDbc);
	if (hDbc)	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);		hDbc = nullptr;
	if (hEnv)	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);		hEnv = nullptr;
}

BOOL CQuery::EnableConnectionPooling()
{
	SQLRETURN result = SQLSetEnvAttr( NULL,  // make process level cursor pooling
					   			     SQL_ATTR_CONNECTION_POOLING,
						             (SQLPOINTER)SQL_CP_ONE_PER_DRIVER,
						             SQL_IS_INTEGER );

	return SQL_SUCCEEDED( result ); 
}

// ���� �ڵ��� �Ҵ��ϰ� ������ �� ����ڵ���� ���� �Ҵ��Ѵ�.
// Type=1:ConStr�� MDB ������ ��θ� ������. ��� ������ ���� ���丮���� MDB�� ã�´�.
// Type=2:ConStr�� SQL ������ ���� ������ ������ DSN ������ ��θ� ������. 
//        ��δ� �ݵ�� ���� ��η� �����ؾ� �Ѵ�.
// Type=3:SQLConnect �Լ��� DSN�� ���� �����Ѵ�.
// ���� �Ǵ� ��� �ڵ� �Ҵ翡 �����ϸ� FALSE�� �����Ѵ�.
BOOL CQuery::Connect(int Type, const char *ConStr, const char *UID, const char *PWD)
{
	strcpy(DBCredentials.Name, ConStr );
	strcpy(DBCredentials.Id, UID );
	strcpy(DBCredentials.Pass, PWD );
	
	SQLCHAR InCon[255];
	SQLCHAR OutCon[255];
   SQLSMALLINT cbOutCon;

	SQLRETURN ret;

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
	SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
	
	// ���� Ÿ�Կ� ���� MDB �Ǵ� SQL ����, �Ǵ� DSN�� �����Ѵ�.
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);
	switch (Type) 
	{
	case 1:
		wsprintf((char *)InCon,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;",ConStr);
		ret=SQLDriverConnect(hDbc,NULL,(SQLCHAR *)InCon,sizeof(InCon),OutCon,
			sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);
		break;
	case 2:
		wsprintf((char *)InCon, "FileDsn=%s",ConStr);
		ret=SQLDriverConnect(hDbc,NULL,(SQLCHAR *)InCon,sizeof(InCon),OutCon,
			sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);
		break;
	case 3:
		ret=SQLConnect(hDbc,(SQLCHAR *)ConStr,SQL_NTS,(SQLCHAR *)UID,SQL_NTS,
			(SQLCHAR *)PWD,SQL_NTS);
		break;
	}

	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) 
	{
		PrintDiag( ConStr );
		return FALSE;
	}

	// ��� �ڵ��� �Ҵ��Ѵ�.
	ret=SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) 
	{
		PrintDiag( ConStr );
		hStmt=0;
		return FALSE;
	}

//	::SQLSetStmtOption(hStmt, SQL_QUERY_TIMEOUT, 10);  // 10�� Ÿ�Ӿƿ� 
	return TRUE;
}

BOOL CQuery::BindParameterImpl(SQLUSMALLINT parameterNumber,
                           SQLSMALLINT inputOutputType,
                           SQLSMALLINT valueType,
                           SQLSMALLINT parameterType,
                           SQLUINTEGER columnSize,
                           SQLSMALLINT decimalDigits,
                           SQLPOINTER  parameterValuePtr,
                           SQLINTEGER bufferLength,
                           SQLINTEGER *strLen_or_IndPtr)
{
	SQLRETURN result = ::SQLBindParameter( hStmt, parameterNumber, inputOutputType, valueType, parameterType, columnSize,
		                                decimalDigits, parameterValuePtr, bufferLength, strLen_or_IndPtr );
	if( result != SQL_SUCCESS )
	{
		TRACE( "CQuery::BindParameter\n" );
		WriteLog( "CQuery::BindParameter - result : %d, ThreadID : %d", result, ::GetCurrentThreadId() );
		return FALSE;
	} else {
		if (m_storeBindedParameters) {
			switch (parameterType) {
				case SQL_VARCHAR:
				case SQL_CHAR:
					if (columnSize > 2) {
						m_storeBindedParameters->AppendFormat("%d = (char) %s", parameterNumber, reinterpret_cast<const char *>(parameterValuePtr));
					}
					break;
				case SQL_INTEGER:
				case SQL_BIGINT:
				case SQL_SMALLINT:
					m_storeBindedParameters->AppendFormat("%d = (number) %d", parameterNumber, *(reinterpret_cast<int *>(parameterValuePtr)));
					break;
				case SQL_REAL:
					m_storeBindedParameters->AppendFormat("%d = (number) %f", parameterNumber, *(reinterpret_cast<float *>(parameterValuePtr)));
					break;
			}
		}
		
		return TRUE;
	}
}

// SQL���� �����Ѵ�. ���н� ���� ������ ����ϰ� FALSE�� �����Ѵ�.
BOOL CQuery::Exec(LPCTSTR szSQL)
{
	Clear();	// ���������� �׻� Ŭ���� �� ���·� ����ǰ� �Ѵ�.

	SQLRETURN ret = SQLExecDirect( hStmt, (SQLCHAR*)szSQL, lstrlen( szSQL ) );
	switch( ret )
	{
	case SQL_SUCCESS_WITH_INFO:
#ifdef _DEBUG
		PrintDiag( szSQL, SQL_HANDLE_STMT );	
#endif
		// �Ʒ� ��� ���� 
	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
		break;
	default:
		{
			PrintDiag( szSQL, SQL_HANDLE_STMT );	
			DisConnect();
			if( Connect( 3, DBCredentials.Name, DBCredentials.Id, DBCredentials.Pass ) )
			{
				ret = SQLExecDirect( hStmt, (SQLCHAR *)szSQL, SQL_NTS );
				if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA)) 
					return FALSE;
			}
			else
				return FALSE;
		}
	}

	m_storeBindedParameters = std::nullopt;

	return PrepareFetch();
}

BOOL CQuery::MoreResults()
{
//	Clear();	// ���������� �׻� Ŭ���� �� ���·� ����ǰ� �Ѵ�.
	
	SQLRETURN ret = SQLMoreResults( hStmt );
	switch( ret ) {
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		break;
	default:
		PrintDiag( "SQLMoreResults", SQL_HANDLE_STMT );	
		return FALSE;
	}

	return PrepareFetch();
}

// ����¿��� �� ���� �����´�.
BOOL CQuery::Fetch()
{
	SQLRETURN result = SQLFetch( hStmt );

	switch( result )
	{
	case SQL_SUCCESS_WITH_INFO:
		#ifdef _DEBUG
			PrintDiag( NULL, SQL_HANDLE_STMT );
		#endif
	case SQL_SUCCESS:
	case SQL_NEED_DATA:
		return TRUE;

	case SQL_NO_DATA_FOUND:		// == SQL_NO_DATA
		return FALSE;
	}

#ifdef __INTERNALSERVER
	PrintDiag( NULL, SQL_HANDLE_STMT );
#endif

	return FALSE;
}

// Ŀ���� �ݰ� ���ε� ������ �����Ѵ�.
void CQuery::Clear()
{
	SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
}

// �÷� �̸����κ��� �÷� �ε����� ã�´�. ���� ��� -1�� �����Ѵ�.
int CQuery::FindCol(const char *name) const
{
	int i;
	for (i=0;i<nCol;i++) 
	{
		if (stricmp(name,(LPCTSTR)ColName[i])==0)
			return i+1;
	}
	return -1;
}

// nCol�� �÷����� ������ �о��ش�. NULL�� ��� CQUERYNULL�� �����Ѵ�.
int CQuery::GetInt(int nCol)
{
	if (nCol > this->nCol)
		return CQUERYNOCOL;

	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		return CQUERYNULL;
	} else {
		return Col[nCol - 1].AsInt();
	}
}

// sCol�� �÷����� ������ �о��ش�.
int CQuery::GetInt(const char *sCol)
{
	int n;
	n=FindCol(sCol);
	if (n==-1) 
	{
		return CQUERYNOCOL;
	} else 
	{
		return GetInt(n);
	}
}

// nCol�� �÷����� ������ �о��ش�. NULL�� ��� CQUERYNULL�� �����Ѵ�.
__int64 CQuery::GetInt64(int nCol)
{
	if (nCol > this->nCol)
		return CQUERYNOCOL;

	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		return CQUERYNULL;
	} else {
		return Col[nCol - 1].AsInt64();
	}
}

// sCol�� �÷����� ������ �о��ش�.
__int64 CQuery::GetInt64(const char *sCol )
{
	int n;
	n=FindCol(sCol);
	if (n==-1) 
	{
		return CQUERYNOCOL;
	} else 
	{
		return GetInt64( n );
	}
}

// nCol�� �÷����� �Ǽ��� �о��ش�. NULL�� ��� CQUERYNULL�� �����Ѵ�.
float CQuery::GetFloat(int nCol)
{
	if (nCol > this->nCol)
		return CQUERYNOCOL;
	
	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		return CQUERYNULL;
	} else {
		return Col[nCol - 1].AsFloat();
	}
}


float CQuery::GetFloat(const char *sCol)
{
	int n;
	n = FindCol(sCol);
	if (n==-1)
		return CQUERYNOCOL;
	else
		return GetFloat(n);
}


char CQuery::GetChar(int nCol)					// �Ǽ��� �÷� �б�
{
	if (nCol > this->nCol)
		return CQUERYNOCOL;
	
	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		return CQUERYNULL;
	} else {
		return Col[nCol - 1].AsChar();
	}
}

char CQuery::GetChar(const char *sCol)				// �Ǽ��� �÷� �б�
{
	int n;
	n = FindCol(sCol);
	if (n==-1)
		return CQUERYNOCOL;
	else
		return GetChar(n);
}

// nCol�� �÷����� ���ڿ��� �о��ش�. NULL�� ��� ���ڿ��� NULL�� ä���ش�. 
// buf�� ���̴� �ּ��� 256�̾�� �ϸ� ���� ������ ���� �ʴ´�.
void CQuery::GetStr(int nCol, char *buf)
{
	if (nCol > this->nCol) 
	{
		strcpy(buf, "CQUERYNOCOL");
		return;
	}

	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		lstrcpy(buf,"NULL");
	} else {
		 strcpy(buf,Col[nCol-1].GetRawString());
	}
}

// sCol�� �÷����� ���ڿ��� �о��ش�.
void CQuery::GetStr(const char *sCol, char *buf)
{
	int n;
	n=FindCol(sCol);
	if (n==-1) 
	{
		lstrcpy(buf,"ERROR:Colume not found");
	} else 
	{
		GetStr(n, buf);
	}
}

const char * CQuery::GetStrPtr(int nCol) const {
	if (nCol <= 0 || nCol > this->nCol) return nullptr;
	return Col[nCol - 1].GetRawString();
}

const char * CQuery::GetStrPtr(const char * sCol) const {
	const int n = FindCol(sCol);
	if (n == -1) {
		Error(__FUNCTION__": column %s does not exist", sCol);
		return nullptr;
	}
	return GetStrPtr(n);
}

// ���� �߻��� ���� ������ ����� �ش�.
void CQuery::PrintDiag( LPCTSTR szSQL, SQLSMALLINT type )
{

/*
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6], Msg[1024];
	SQLSMALLINT MsgLen;

	int ii=1;
	SQLRETURN Ret = SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
	if( Ret != SQL_NO_DATA )
	{
		if( szSQL )
			WriteLogFile("query:%s\nSQLSTATE:%s error:%s", szSQL, (LPCTSTR)SqlState,(LPCTSTR)Msg );
		else
			WriteLogFile("SQLSTATE:%s error:%s", (LPCTSTR)SqlState,(LPCTSTR)Msg );
	}
*/
	UCHAR szMsg[SQL_MAX_MESSAGE_LENGTH];
	UCHAR szState[SQL_SQLSTATE_SIZE+1];
	SQLSMALLINT nLen;
	SQLINTEGER nError;
	SQLRETURN ret;

	SQLHANDLE handle = hDbc;
	if( type == SQL_HANDLE_STMT )
		handle = hStmt;

	int nIndex=1;
	while( TRUE )
	{
		ret = SQLGetDiagRec( type, handle, nIndex, szState, &nError, szMsg, SQL_MAX_MESSAGE_LENGTH-1, &nLen );
		if( (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) &&	lstrcmp((LPCTSTR)szState, "00000") != 0 )
		{
			if( szSQL )
				WriteLogFile("query:%s\nSQLSTATE:%s error:%s", szSQL, (LPCTSTR)szState,(LPCTSTR)szMsg );
			else {
				WriteLogFile("SQLSTATE:%s error:%s", (LPCTSTR)szState, (LPCTSTR)szMsg);

				if (LPCTSTR(szState) == std::string_view("22001 NativeError : 0,")) {
					PrintQuery(szSQL);
				}
			}
			nIndex++;
		}
		else
			break;	
	}
}

void CQuery::PrintQuery(const char * const query) {
	CString str = "Query= ";
	str += query;
	str += "\n";
	if (!m_storeBindedParameters) {
		str += "Binded parameters were not stored";
	} else {
		str += "Binded parameters=\n";
		str += m_storeBindedParameters.value();
	}

	WriteLogFile("%s", str.GetString());
}

void CQuery::WriteLogFile(const char* lpszFormat,...)
{	
	TCHAR szBuffer[1024* 64];

	va_list args;
	va_start( args, lpszFormat );
	int n = _vsntprintf( szBuffer, sizeof(szBuffer)-1, lpszFormat, args );
	va_end( args );

	if( n == -1 ) // if the number of bytes to write exceeds buffer, then count bytes are written and -1 is returned
		return;

	OutputDebugString( szBuffer );

	FILE* fp = fopen( MakeFileNameDate( "..\\CQuery-LogFile.txt" ), "a" );
	if( fp == NULL )
		return;

	// time
	SYSTEMTIME	time;
	GetLocalTime( &time );
	fprintf( fp, "%d/%02d/%02d\t%02d:%02d:%02d\n%s\n", 
		     time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
		     szBuffer );
	fclose( fp );
}


BOOL CQuery::PrepareFetch()
{
	SQLNumResultCols(hStmt,&nCol);
	if (nCol > MAXCOL)
		return FALSE;
	
	// nCol�� 0�� ���� Select�� �̿��� �ٸ� ����� ������ ����̹Ƿ� 
	// ���ε��� �� �ʿ䰡 ����.
	if (nCol == 0) 
	{
		Clear();
		return TRUE;
	}
	
	SWORD	nActualLen;
	SWORD	m_nSQLType;
	SQLULEN m_nPrecision;
	SWORD	m_nScale;
	SWORD	m_nNullability;
 
	// ��� �÷��� ���ڿ��� ���ε��� ���´�. Col�迭�� zero base, 
	// �÷� ��ȣ�� one base�ӿ� ������ ��
	for (int c=0;c<nCol;c++) 
	{
		auto [buffer, size] = Col[c].GetInfoForBind();

		SQLBindCol(hStmt, c + 1, SQL_C_CHAR, buffer, size, &lCol[c]);

		SQLDescribeCol(hStmt, c + 1, ColName[c], 30,
			&nActualLen,
			&m_nSQLType,
			&m_nPrecision,
			&m_nScale,
			&m_nNullability);
	}

	return TRUE;
}

BOOL CQuery::Execute( LPCTSTR lpszFormat, ... )
{
	char szQuery[1024]		= { 0,};
	va_list	args;
	va_start( args, lpszFormat );
	int n	= _vsntprintf( szQuery, 1024, lpszFormat, args );
	va_end( args );
	return n > 0 && Exec( szQuery );
}