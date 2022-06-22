#pragma once

#include <sql.h>
#include <sqlext.h>
#include <optional>

struct QUERY_BINDINFO
{
	SQLSMALLINT targetType;
	SQLPOINTER	targetValue;
	SQLINTEGER	bufferLength;
	SQLINTEGER*	strLen_or_Ind;
};

class CQuery
{
public:
	struct Credentials {
		char Name[256] = "";
		char Id[256]   = "";
		char Pass[256] = "";
	};

public:
//	char *DBName, *DBId, *DBPass;	
	char DBName[256];
	char DBId[256];
	char DBPass[256];

	// �ִ� �÷���, BLOB ����� ����, NULL �ʵ尪
	enum { MAXCOL=256, BLOBBATCH=10000, CQUERYNULL=-100, CQUERYEOF=-101, 
		CQUERYNOCOL=-102, CQUERYERROR=-103 };
private:
	SQLHENV hEnv;							// ȯ�� �ڵ�
	SQLHDBC hDbc;							// ���� �ڵ�
	char*	Col[MAXCOL];					// ���ε��� �÷� ����

	[[nodiscard]] int FindCol(const char *name) const;				// �÷��� �̸����κ��� ��ȣ�� ã���ش�.

	std::optional<CString> m_storeBindedParameters = std::nullopt;

public:
	SQLHSTMT hStmt;							// ��� �ڵ�. ���� ����� ���� �����Ƿ� public���� ����
	SQLSMALLINT nCol;						// �÷� ����
	SQLCHAR ColName[MAXCOL][50];			// �÷��� �̸���
	SQLINTEGER lCol[MAXCOL];				// �÷��� ����/���� ����

	CQuery();								// ������
	~CQuery();								// �ı���:���� �ڵ��� �����Ѵ�.

	void PrintDiag( LPCTSTR szSQL, SQLSMALLINT type = SQL_HANDLE_DBC );						// ���� ���� ���
	void PrintQuery(const char * query);
	BOOL Connect(int Type, const char *ConStr, const char *UID=NULL, const char *PWD=NULL);	// ������ �ҽ��� �����Ѵ�.
	BOOL Connect(int Type, const Credentials & credentials) {
		return Connect(Type, credentials.Name, credentials.Id, credentials.Pass);
	}
	void DisConnect();						// ������ �ҽ� ������ ���´�
	BOOL Exec(LPCTSTR szSQL);				// SQL���� �����Ѵ�.
	BOOL Exec(LPCTSTR szSQL, int nCount, QUERY_BINDINFO info[]);
	BOOL PrepareFetch();
	void StartLogBindedParameters() { m_storeBindedParameters = CString(); }

	BOOL	Execute( LPCTSTR lpszFormat, ... );

	int ExecGetInt(LPCTSTR szSQL);			// SQL���� �����ϰ� ù��° �÷��� ���� ����
	void ExecGetStr(LPCTSTR szSQL, char *buf);		// SQL���� �����ϰ� ù��° �÷��� ���ڿ� ����
	BOOL Fetch();						// �� �� ��������
	void Clear();							// Ŀ�� ���� �� �� ���ε�
	int GetInt(int nCol);					// ������ �÷� �б�
	int GetInt(const char *sCol);					// ������ �÷� �б�

	__int64		GetInt64( int nCol );
	__int64		GetInt64( const char* sCol );

	EXPINTEGER	GetExpInteger( int nCol )	{ return GetInt64( nCol );	}
	EXPINTEGER	GetExpInteger(	const char* sCol ) { return GetInt64( sCol ); 	}
	SERIALNUMBER	GetSerialNumber( int nCol )
		{
			return GetInt( nCol );
		}
	SERIALNUMBER	GetSerialNumber( const char* sCol )
		{
			return GetInt( sCol );
		}

	float GetFloat(int nCol);					// �Ǽ��� �÷� �б�
	float GetFloat(const char *sCol);				// �Ǽ��� �÷� �б�
	char GetChar(int nCol);					// �Ǽ��� �÷� �б�
	char GetChar(const char *sCol);				// �Ǽ��� �÷� �б�
	void GetStr(int nCol, char *buf);		// ���ڿ��� �÷� �б�
	void GetStr(const char *sCol, char *buf);		// ���ڿ��� �÷� �б�
	const char * GetStrPtr(int nCol) const;
	const char * GetStrPtr(const char * sCol) const;
	int ReadBlob(LPCTSTR szSQL, void *buf);
	void WriteBlob(LPCTSTR szSQL, void *buf, int size);
	BOOL BindParameter(SQLUSMALLINT parameterNumber,
                           SQLSMALLINT inputOutputType,
                           SQLSMALLINT valueType,
                           SQLSMALLINT parameterType,
                           SQLUINTEGER columnSize,
                           SQLSMALLINT decimalDigits,
                           SQLPOINTER  parameterValuePtr,
                           SQLINTEGER bufferLength,
                           SQLINTEGER *strLen_or_IndPtr);
	
	BOOL MoreResults( void );
	

	void WriteLogFile(const char *strLog,...);
	static BOOL EnableConnectionPooling();
};
// TODO: ��� �и� 
// ��ȣȭ�� ��ū�� �ص��ؼ� �н������ ��´�.
// ��: ��ū - 3A08DB22
extern BOOL GetPWDFromToken( const char* szToken, char* szPWD );
