//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AnlagenList.h
///
/// @file   AnlagenList.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "types.h"
#include "DynList.h"
#include "DoseServer.h"




//*****************************************************************************************
class CAnlagenItem
{
public:
	int32_t				m_lID;					// log-ID
	base::eDoseType		m_ulDoseType;			// Dosiertyp
	base::eLcTypes		m_ulDosierWaage;		// Waagentyp
	CString				m_szBemerkung;

protected:
	void Copy	(const CAnlagenItem&);
	void Clear	(void);

	friend class CAnlagenItemList;

public:
	CAnlagenItem					(int32_t _lID = -1L) :  m_lID(_lID)
														, m_ulDoseType(base::eDoseType::DOSETYPE_LWS)
														, m_ulDosierWaage(base::eLcTypes::LC_UNKNOWN)
														, m_szBemerkung("") 
	{}

	CAnlagenItem					(int32_t _lID, base::eDoseType _ulDoseType, base::eLcTypes _ulDosierWaage, const CString& _sz) : m_lID(_lID)
														, m_ulDoseType(_ulDoseType)
														, m_ulDosierWaage(_ulDosierWaage)
														, m_szBemerkung(_sz)	
	{}

	virtual ~CAnlagenItem(void) = default;
	CAnlagenItem(const CAnlagenItem&) = default;
	CAnlagenItem& operator =		(const CAnlagenItem&) = default;

	BOOL	Load					(LPCSTR);
	BOOL	Save					(LPCSTR);
	void	Serialize				(CArchive&);

	int32_t  GetID					(void) const
	{ return m_lID; }
	void  SetID						(int32_t lID)
	{ m_lID = lID; }

	void  SetDoseType				(const ULONG l)
	{ m_ulDoseType = l; }
	base::eDoseType  GetDoseType	(void) const
	{ return m_ulDoseType; }
	void  SetDosierWaage			(const base::eLcTypes l)
	{ m_ulDosierWaage = l; }
	base::eLcTypes  GetDosierWaage	(void) const
	{ return m_ulDosierWaage; }
	void  SetBemerkung				(const CString& sz)
	{ m_szBemerkung = sz; }
	void  GetBemerkung				(CString& sz) const
	{ sz  = m_szBemerkung; }
};




typedef CDynList<CAnlagenItem> _ANLAGENFILELIST;


class CAnlagenItemList : public _ANLAGENFILELIST
{
private:
	int32_t		m_lID;					// RadID
	CString		m_szAnlagenPath;
	CString		m_szAnlagenFilename;

private:
	BOOL		SortID			(void);
	BOOL		LoadItems		(void);
	void		Serialize			(CArchive&);


protected:
	void CreateList				(const char* pA, int32_t sN);

	void Copy	(const CAnlagenItemList&);
	// void Clear	(void);

public:
	CAnlagenItemList				(const CAnlagenItemList&);
	CAnlagenItemList& operator =	(const CAnlagenItemList&);
	BOOL			operator ==	(const CAnlagenItemList&) ;

	CAnlagenItem*					GetAnlagenItem(int32_t lID = 1);

	CAnlagenItemList				(int32_t lID = -1L);
	~CAnlagenItemList				(void);

	void	Create				(void);
	void CreateAnlagenFilename	(void);
	void SetAnlagenFilename		(const CString& );
	CString GetAnlagenFilename	(void) const;
	CString GetAnlagenPath		(void) const;
	void SetAnlagenPath			(const CString& _szAnlagenPath);

	int32_t GetFreeID				(void) const;

	int32_t	GetID				(void) const
	{ return m_lID; }
	void	SetID				(const int32_t lID)
	{ m_lID = lID; }



	BOOL	Load				(void);
	BOOL	Save				(void);


	BOOL	AddAnlagenItem		(const CAnlagenItem&);
	BOOL	DeleteAnlagenItem	(const int32_t lID);
	BOOL	ReplaceAnlagenItem	(const CAnlagenItem&);
	void	DeleteAllItems		(void)
	{ Clear(); }

	BOOL CreateList				(DEVICECFG&);
};



//------------------------------------------------------------------------------------
///  @brief   CreateAnlagenFilename
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAnlagenItemList
///
///  @param[in/out] none
///  @return none
///
//------------------------------------------------------------------------------------
inline void CAnlagenItemList :: CreateAnlagenFilename (void)
{
	ASSERT(m_szAnlagenPath.IsEmpty() == FALSE);
	m_szAnlagenFilename.Format("%s\\%s", LPCSTR(m_szAnlagenPath), LPCSTR(ANLAGENFILE));
}


//------------------------------------------------------------------------------------
///  @brief   SetAnlagenFilename
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAnlagenItemList
///
///  @param[in/out] _szAnlagenFilename , const CString&
///  @return none
///
//------------------------------------------------------------------------------------
inline void CAnlagenItemList :: SetAnlagenFilename (const CString& _szAnlagenFilename)
{
	ASSERT(_szAnlagenFilename.IsEmpty() == FALSE);
	m_szAnlagenFilename = _szAnlagenFilename;
}



//------------------------------------------------------------------------------------
///  @brief   GetAnlagenFilename
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAnlagenItemList
///
///  @param[in/out] none
///  @return CString
///
//------------------------------------------------------------------------------------
inline CString CAnlagenItemList :: GetAnlagenFilename (void) const
{
	return m_szAnlagenFilename;
}


//------------------------------------------------------------------------------------
///  @brief   GetAnlagenPath
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAnlagenItemList
///
///  @param[in/out] none
///  @return CString
///
//------------------------------------------------------------------------------------
inline CString CAnlagenItemList :: GetAnlagenPath (void) const
{
	return m_szAnlagenPath;
}


//------------------------------------------------------------------------------------
///  @brief   SetAnlagenPath
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAnlagenItemList
///
///  @param[in/out] _szAnlagenPath , const CString&
///  @return none
///
//------------------------------------------------------------------------------------
inline void CAnlagenItemList :: SetAnlagenPath (const CString& _szAnlagenPath)
{
	ASSERT(_szAnlagenPath.IsEmpty() == FALSE);
	m_szAnlagenPath = _szAnlagenPath;
}




extern CAnlagenItemList aAnlagenItemList;



