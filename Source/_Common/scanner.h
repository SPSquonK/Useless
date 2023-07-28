#ifndef __SCANNER_H
#define __SCANNER_H

#include "data.h"
#include <bitset>
#include <concepts>
#include <type_traits>

#ifndef NULL_ID
#define NULL_ID 0xffffffff
#endif

#define MAX_TOKENSTR 2048

/*
 *  Add additional C keyword tokens here
 */
enum Toke
{
	NONE_,ARG,VAR,IF,ELSE,FOR,DO,WHILE,BREAK,SWITCH,ANSWER,SELECT,YESNO,
	CASE,DEFAULT,GOTO,RETURN,EOL,DEFINE,INCLUDE,ENUM,FINISHED,END
};
/*
 *  Add additional double operatoes here (such as->)
 */
enum DoubleOps 
{ 
	LT = 1, LE,GT,GE,EQ,NE,NT 
};

enum TokenType 
{ 
	TEMP,DELIMITER,IDENTIFIER,NUMBER,HEX,KEYWORD,STRING,BLOCK 
};


////////////////////////////////////////////////////////////////////////////////
//
// Token : ��ū�� �����Ͽ� �о�帲.
//
class CScanner 
{
public:
	CScanner( BOOL bComma = FALSE );
	CScanner( LPVOID p, BOOL bComma = FALSE );
	virtual ~CScanner();

protected:
	BOOL		m_bComma;			// �����ڰ� �޸��ΰ�?
	BYTE		m_bMemFlag;			// 0�� �ε�, 1�� �ܺ� ����Ʈ 
	int			m_nProgSize;
	DWORD		m_dwDef;
	BOOL		Read( CFileIO* pFile, BOOL );

public:
	CHAR*		m_lpMark;						
	CHAR*		m_pProg;						
	CHAR*		m_pBuf;	
	
	CString		Token;
	char*		token;							
	char		m_mszToken[ MAX_TOKENSTR ];	
	int			tokenType;					
	int			tok;					
	int			m_nDoubleOps;
	CString 	m_strFileName;
	BOOL		m_bErrorCheck;

	virtual void	SetMark() { m_lpMark = m_pProg; }
	virtual void	GoMark() { if( m_lpMark ) m_pProg = m_lpMark; }
	virtual int		GetLineNum( LPVOID lpProg = NULL );
	virtual void	PutBack();
	virtual	int		GetToken( BOOL bComma = FALSE );
	
	void			SetErrorCheck( BOOL bErrorCheck ) { m_bErrorCheck = bErrorCheck; } 
	BOOL			GetErrorCheck() { return m_bErrorCheck; }
	void			GetLastFull(); // ������� ������ �ѹ��� �д´�.
	void			Free();
	BOOL			Load( LPCTSTR lpszFileName, BOOL bMultiByte = TRUE );
	BOOL			Load_FileIO( LPCTSTR lpszFileName, BOOL bMultiByte = TRUE );
	void			SetProg( LPVOID pProg );
	int				GetNumber( BOOL bComma = FALSE );
	DWORD GetDWORD(BOOL bComma = FALSE);
	__int64			GetInt64( BOOL bComma = FALSE );
	FLOAT			GetFloat( BOOL bComma = FALSE );
	DWORD			GetHex( BOOL bComma = FALSE );
	SERIALNUMBER	GetSerialNumber( BOOL bComma = FALSE );
	EXPINTEGER		GetExpInteger(  BOOL bComma = FALSE )	{ return GetInt64( bComma ); }


	/*
	Return the bitset corresponding to the next read value.

	Triggers an Error if the value can not fit in the bitset.

	Example of usage: myBitset = scanner.GetBitset<decltype(myBitset)>();

	The type of the bitset is passed instead of the size so when the bitset type changes,
	the GetBitset instruction is automatically updated.
	*/
	template<typename BitsetType>
	BitsetType GetBitset(BOOL bComma = FALSE) {
		static_assert(std::is_same_v<BitsetType, std::bitset<BitsetType().size()>>);

		const int number = GetNumber(bComma);
		if (number == -1 || number == 0) return number;

		static constexpr auto maximumSupportedNumber = BitsetType(std::numeric_limits<unsigned long long>::max()).to_ullong();

		if (static_cast<unsigned int>(number) >= maximumSupportedNumber) {
			Error("Read bitset with value %d but the maximum is %lld", number, maximumSupportedNumber);
		}

		return number;
	
	}

	template<std::integral T = int>
	std::vector<T> GetNumbers(const char terminator) {
		return GetNumbers<std::vector<T>>(terminator);
	}

	template<typename /* VectorLike */ TS>
	TS GetNumbers(const char terminator)
	requires (std::is_integral<typename TS::value_type>::value) {
		TS retval{};
		using T = typename TS::value_type;

		GetToken(); // starter ({)
		T value = static_cast<T>(GetNumber());
		while (*token != terminator) {
			retval.emplace_back(value);
			value = static_cast<T>(GetNumber());
		}

		return retval;
	}
};

/////////////////////////////////////////////////////////////////////////////

extern WORD		g_codePage;

bool	IsComposibleTh(BYTE prev, BYTE curr, int mode);
const char*	CharNextEx(const char* strText, WORD codePage = g_codePage);

/////////////////////////////////////////////////////////////////////////////
int WideCharToMultiByteEx(
    UINT     CodePage,
    DWORD    dwFlags,
    LPCWSTR  lpWideCharStr,
    int      cchWideChar,
    LPSTR    lpMultiByteStr,
    int      cchMultiByte,
    LPCSTR   lpDefaultChar,
    LPBOOL   lpUsedDefaultChar);

int MultiByteToWideCharEx(
    UINT     CodePage,
    DWORD    dwFlags,
    LPCSTR   lpMultiByteStr,
    int      cchMultiByte,
    LPWSTR   lpWideCharStr,
    int      cchWideChar);


BOOL IsCyrillic( const char chSrc );

#endif
