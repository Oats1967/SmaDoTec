//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericList
///
/// @file   GenericList.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "BASE/types.h"
#include "BASE/Utils/public/StringUtil.h"
#include "Logger/include/Logger.h"

namespace base
{
namespace utils
{

template < class T>
class CGenericList
{
protected:
	std::vector< T > m_ItemList;
	std::string		m_szPath;
	int32_t			m_lID;
protected:
	BOOL		SortID(void);

	virtual BOOL LoadStream(std::ifstream& file_in);
	virtual BOOL SaveStream(std::ofstream& file_out);


public:
	CGenericList(int32_t lID = -1L) : m_lID(lID), m_szPath("")
	{}
	virtual ~CGenericList(void)			= default;
	CGenericList(const CGenericList&)	= default;
	CGenericList(CGenericList&& rList) noexcept : m_ItemList{ std::move(rList.m_ItemList) }
										, m_szPath { std::move(rList.m_szPath) }
										, m_lID{ std::move(rList.m_lID) }
	{}

	CGenericList& operator = (const CGenericList& rList) = default;

	uint32_t GetCount(void) const
	{ return _U32(m_ItemList.size());	}
	const T& GetItem(int32_t lID = 1) const
	{	return m_ItemList[lID]; }
	T& GetItem(int32_t lID = 1)
	{	return m_ItemList[lID];	}

	int32_t	GetID(void) const
	{ return m_lID;	}
	void	SetID(int32_t lID)
	{	m_lID = lID; }

	T* GetItemID(const int32_t _id);

	// ----------------------------------------------------------------------------------------------
	virtual void SetPath(const std::string & _szPath)
	{	m_szPath = _szPath;	}
	// ----------------------------------------------------------------------------------------------
	virtual const std::string& GetPath() const 
	{	return m_szPath;	}

	virtual void	AddItem(const T& newitem);
	virtual BOOL	DeleteItem(int32_t lID);
	virtual BOOL	ReplaceItem(int32_t lID, const T&);
	virtual const T& GetLastItem(void) const;
	virtual T& GetLastItem(void);

	virtual BOOL Load(const std::string& filename);
	virtual BOOL Save(const std::string& filename);
	virtual BOOL SaveLastItem(const std::string& filename);
	virtual BOOL LoadAll(void);

	virtual BOOL SaveAll(void);
	virtual BOOL SaveLastItem(void);
	virtual const std::string GetFilename() const
	{ return "";	}

	void Clear()
	{	m_ItemList.clear();	}

	void Merge(const CGenericList<T>& rFirst, const CGenericList<T>& rSecond);
	CGenericList& operator += (const CGenericList& aP);

	virtual void Shrink(const int32_t MaxPoints);

	template < class UnaryPred >
	void Extract(UnaryPred  func);
};
//************************************************************************************************************
//************************************************************************************************************
template < class T>
inline CGenericList<T>& CGenericList<T>::operator += (const CGenericList<T>& aP)
{
	if (&aP != this)
	{
		for (auto& item : aP.m_ItemList)
		{
			m_ItemList.push_back(item);
		}
	}
	return *this;
}

//************************************************************************************************************
//************************************************************************************************************
template < class T>
const T& CGenericList<T>::GetLastItem(void) const
{
	assert(!m_ItemList.empty());
	return m_ItemList.back();
}
//************************************************************************************************************
//************************************************************************************************************
template < class T>
T& CGenericList<T>::GetLastItem(void)
{
	assert(!m_ItemList.empty());
	return m_ItemList.back();
}
//************************************************************************************************************
//************************************************************************************************************
template < class T>
void CGenericList<T>::AddItem(const T& newitem)
{
	T item = newitem;
	item.SetID(_S32(m_ItemList.size()));
	m_ItemList.push_back(item);
}
//************************************************************************************************************
//************************************************************************************************************
template < class T>
BOOL CGenericList<T>::DeleteItem(int32_t lID)
{
	BOOL result = FALSE;
	int32_t id = lID;
	auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [&id](const auto& item) -> BOOL { return (id == item.GetID()); });
	if (it != m_ItemList.end())
	{
		this->m_ItemList.erase(it);
		SortID();
		result = TRUE;
	}
	return result;
}
//************************************************************************************************************
//************************************************************************************************************
template < class T>
BOOL CGenericList<T>::ReplaceItem(int32_t lID, const T& item)
{
	auto pItem = GetItemID(lID);
	if (pItem)
	{
		*pItem = item;
		pItem->SetID(lID);
	}
	return (pItem) != nullptr;
}

//*********************************************************************************************************************************
//*********************************************************************************************************************************
template < class T>
T* CGenericList<T>::GetItemID(const int32_t lID)
{
	int32_t id = lID;
	auto it = std::find_if(m_ItemList.begin(), m_ItemList.end(), [&id](const auto& item) -> BOOL { return (id == item.GetID()); });
	if (it != m_ItemList.end())
	{
		return &(*it);
	}
	return nullptr;
}

//*********************************************************************************************************************************
//*********************************************************************************************************************************
template < class T>
BOOL CGenericList<T>::SortID(void)
{
	int32_t lSize = _S32(m_ItemList.size());
	if (lSize <= 0)
		return TRUE;

	// Minimalwert finden
	for (auto i = 0; i < lSize - 1; i++)
	{
		auto merke = i;
		for (auto j = i + 1; j < lSize; j++)
		{
			if (m_ItemList[j].GetID() < m_ItemList[merke].GetID())
			{
				merke = j;
			}
		}
		if (merke != i)
		{
			T temp = m_ItemList[merke];
			m_ItemList[merke] = m_ItemList[i];
			m_ItemList[i] = temp;
		}
	}
	int32_t id = 0;
	for (auto& item : m_ItemList)
	{
		item.SetID(id);
		id++;
	}
	return TRUE;
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::LoadStream(std::ifstream& file_in)
{
	m_ItemList.clear();
	while (file_in.good())
	{
		T aItem;
		file_in >> aItem;
		if (aItem.GetID() >= 0)
		{
			AddItem(aItem);
		}
	}
	file_in.close();
	return TRUE;
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::Load(const std::string& filename)
{
	std::ifstream file_in;

	file_in.open(filename);
	if (!file_in.is_open())
	{
		LOGERROR("File : " << filename << " cannot be opened !");
		m_ItemList.clear();
		return FALSE;
	}
	return LoadStream(file_in);
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::SaveStream(std::ofstream& file_out)
{
	for (const auto& item : m_ItemList)
	{
		file_out << item;
	}
	file_out.flush();
	file_out.close();
	return TRUE;
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::Save(const std::string& filename)
{
	BOOL result = FALSE;
	std::ofstream file_out;

	file_out.open(filename);
	if (!file_out.is_open())
	{
		LOGERROR("File : " << filename << "cannot be opened !");
	}
	else
	{
		result = SaveStream(file_out);
	}
	return result;
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::SaveLastItem(const std::string& filename)
{
	if (m_ItemList.empty())
	{
		LOGDEBUG("Noting zo save ...");
		return FALSE;
	}
	std::ofstream file_out;
	file_out.open(filename, std::fstream::out | std::fstream::app);
	if (!file_out.is_open())
	{
		LOGERROR("Cannot open filename " << filename);
		return FALSE;
	}
	file_out << GetLastItem();
	file_out.flush();
	file_out.close();
	return	TRUE;
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::LoadAll(void)
{
	std::ostringstream stringStream;
	stringStream << m_szPath << "\\" << GetFilename();
	return CGenericList::Load(stringStream.str());
}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::SaveAll(void)
{
	std::ostringstream stringStream;
	stringStream << m_szPath << "\\" << GetFilename();
	return CGenericList::Save(stringStream.str());

}
//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
BOOL CGenericList<T>::SaveLastItem(void)
{
	std::ostringstream stringStream;
	stringStream << m_szPath << "\\" << GetFilename();
	return CGenericList::SaveLastItem(stringStream.str());
}


//**********************************************************************************************
//**********************************************************************************************
template < class T>
void CGenericList<T>::Merge(const CGenericList<T>& rFirst, const CGenericList<T>& rSecond)
{
	Clear();

	uint32_t first = 0;
	uint32_t second = 0;

	while (( first < rFirst.GetCount()) && (second < rSecond.GetCount()))
	{
		const auto& rFirstItem = rFirst.GetItem(first);
		const auto& rSecondItem = rSecond.GetItem(second);
		if (rFirstItem < rSecondItem)
		{
			AddItem(rFirst.GetItem(first));
			first++;
		}
		else if (rFirstItem > rSecondItem)
		{
			AddItem(rSecond.GetItem(second));
			second++;
		}
		else
		{
			AddItem(rFirst.GetItem(first));
			first++;
			second++;
		}
	}
	while (first < rFirst.GetCount())
	{
		AddItem(rFirst.GetItem(first));
		first++;
	}
	while (second < rSecond.GetCount())
	{
		AddItem(rSecond.GetItem(second));
		second++;
	}
}


//******************************************************************************************************************
//******************************************************************************************************************
template < class T>
void CGenericList<T>::Shrink(const int32_t MaxPoints)
{
	auto count = GetCount();
	if (count > _U32(MaxPoints))
	{
		uint32_t step = count / _U32(MaxPoints);
		if (step > 1)
		{
			uint32_t index = 1;
			for (uint32_t i = step; i < count; i += step)
			{
				m_ItemList[index++] = m_ItemList[i];
			}
			m_ItemList.resize(index);
		}
	}
}
//************************************************************************************************************
//************************************************************************************************************
template < class T>
template < class UnaryPred >
void CGenericList<T>::Extract(UnaryPred  func)
{
	const auto count = GetCount();
	if (count > 0)
	{
		uint32_t index = 0;
		for (uint32_t i = 0; i < count; i++)
		{
			if (func(m_ItemList[i]))
			{
				if (i != index)
				{
					m_ItemList[index++] = m_ItemList[i];
				}
			}
		}
		m_ItemList.resize(index);
	}
}




};
};


