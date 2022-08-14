#pragma once

#include <sql.h>
#include <sqlext.h>
#include <array>
#include <memory>
#include <optional>
#include "sqktd.h"

class CQuery {
public:
	static SQLINTEGER sqlNts;

	struct Credentials {
		char Name[256] = "";
		char Id[256]   = "";
		char Pass[256] = "";
	};

	class ColString {
	private:
		SQLULEN m_size;
		std::unique_ptr<char[]> m_buffer;

	public:
		ColString();

		[[nodiscard]] int AsInt() const;
		[[nodiscard]] std::int64_t AsInt64() const;
		[[nodiscard]] float AsFloat() const;
		[[nodiscard]] char AsChar() const;
		[[nodiscard]] const char * GetRawString() const;
		std::pair<char *, SQLLEN> GetInfoForBind();
	};

public:
	Credentials DBCredentials;

	// �ִ� �÷���, BLOB ����� ����, NULL �ʵ尪
	static constexpr SQLSMALLINT MAXCOL = 256;
	
	enum { BLOBBATCH=10000, CQUERYNULL=-100, CQUERYEOF=-101, 
		CQUERYNOCOL=-102, CQUERYERROR=-103 };
private:
	SQLHENV hEnv = nullptr;							// ȯ�� �ڵ�
	SQLHDBC hDbc = nullptr;							// ���� �ڵ�
	std::array<ColString, MAXCOL> Col; // ���ε��� �÷� ����

	[[nodiscard]] int FindCol(const char *name) const;				// �÷��� �̸����κ��� ��ȣ�� ã���ش�.

	std::optional<CString> m_storeBindedParameters = std::nullopt;

public:
	SQLHSTMT hStmt = nullptr;							// ��� �ڵ�. ���� ����� ���� �����Ƿ� public���� ����
	SQLSMALLINT nCol;						// �÷� ����
	SQLCHAR ColName[MAXCOL][50];			// �÷��� �̸���
	SQLINTEGER lCol[MAXCOL];				// �÷��� ����/���� ����

	CQuery() = default;
	CQuery(const CQuery &) = delete;
	CQuery & operator=(const CQuery &) = delete;
	~CQuery();								// �ı���:���� �ڵ��� �����Ѵ�.

	void PrintDiag( LPCTSTR szSQL, SQLSMALLINT type = SQL_HANDLE_DBC );						// ���� ���� ���
	void PrintQuery(const char * query);
	BOOL Connect(int Type, const char *ConStr, const char *UID=NULL, const char *PWD=NULL);	// ������ �ҽ��� �����Ѵ�.
	BOOL Connect(int Type, const Credentials & credentials) {
		return Connect(Type, credentials.Name, credentials.Id, credentials.Pass);
	}
	void DisConnect();						// ������ �ҽ� ������ ���´�
	BOOL Exec(LPCTSTR szSQL);				// SQL���� �����Ѵ�.
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

	template<SQLSMALLINT ParameterType = SQL_VARCHAR>
	requires ((ParameterType == SQL_VARCHAR || ParameterType == SQL_CHAR))
	bool BindParameter(SQLUSMALLINT parameterNumber,
		char * parameterValuePtr,
		SQLUINTEGER columnSize = 0
	) {
		return BindParameterImpl(parameterNumber, SQL_PARAM_INPUT, SQL_C_CHAR, ParameterType, columnSize, 0,
			parameterValuePtr, 0, &sqlNts);
	}

	template<typename IntegerType>
		requires (sqktd::IsOneOf<std::remove_volatile_t<IntegerType>, int, unsigned int, long, unsigned long>)
	bool BindParameter(SQLUSMALLINT parameterNumber, IntegerType * valuePtr) {
		return BindParameterImpl(parameterNumber, SQL_PARAM_INPUT,
			SQL_C_LONG, SQL_INTEGER, 0, 0, valuePtr, 0, 0
		);
	}

	template<typename FloatType>
		requires (sqktd::IsOneOf<std::remove_volatile_t<FloatType>, float>)
	bool BindParameter(SQLUSMALLINT parameterNumber, FloatType * valuePtr) {
		return BindParameterImpl(parameterNumber, SQL_PARAM_INPUT,
			SQL_C_FLOAT, SQL_REAL, 0, 0, valuePtr, 0, 0
		);
	}

	template<typename IntegerType>
		requires (sqktd::IsOneOf<std::remove_volatile_t<IntegerType>, long long>)
	bool BindParameter(SQLUSMALLINT parameterNumber, IntegerType * valuePtr) {
		return BindParameterImpl(parameterNumber, SQL_PARAM_INPUT,
			SQL_C_SBIGINT, SQL_BIGINT, 0, 0, valuePtr, 0, 0
		);
	}

	BOOL MoreResults( void );
	

	void WriteLogFile(const char *strLog,...);
	static BOOL EnableConnectionPooling();

	private:
		
	BOOL BindParameterImpl(SQLUSMALLINT parameterNumber,
                           SQLSMALLINT inputOutputType,
                           SQLSMALLINT valueType,
                           SQLSMALLINT parameterType,
                           SQLUINTEGER columnSize,
                           SQLSMALLINT decimalDigits,
                           SQLPOINTER  parameterValuePtr,
                           SQLINTEGER bufferLength,
                           SQLINTEGER *strLen_or_IndPtr);
};
// TODO: ��� �и� 
// ��ȣȭ�� ��ū�� �ص��ؼ� �н������ ��´�.
// ��: ��ū - 3A08DB22
extern BOOL GetPWDFromToken( const char* szToken, char* szPWD );
