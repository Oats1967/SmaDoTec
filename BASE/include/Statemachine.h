//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Statemachine.h
///
/// @file   Statemachine.h
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

#include "BASE/Utils/public/Timer.h"



namespace base
{
    template <typename T = int32_t>
    class CStatemachine
    {
    private:
        BOOL        m_bRunning;
        BOOL        m_bInit;

    protected:
        int32_t	            m_sID;
        T		            m_sStep;
        uint32_t	        m_st;

    protected:
        virtual BOOL Update(const uint32_t t);
    
    public:
        CStatemachine(int32_t sID = -1);
        virtual ~CStatemachine() = default;

        virtual BOOL Execute(void);
        virtual BOOL InitExecute(void);
        virtual BOOL ExitExecute(void);
        virtual BOOL IsInit(void) const
        {   return m_bInit;      }
        virtual BOOL IsStarted(void) const
        {   return m_bRunning;   }

        void SetID(int32_t s)
        {  m_sID = s; }

        int32_t GetID(void) const
        {  return m_sID;   }

        void SetOperatingMode(const T mE)
        {
            m_sStep = mE;
        }
        const T GetOperatingMode(void) const
        {   return m_sStep;    }

        virtual BOOL Start(const uint32_t t = 0);
        virtual BOOL Stop(void);
    };


    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline CStatemachine<T>::CStatemachine(int32_t sID) : m_bRunning(FALSE)
        , m_bInit(FALSE)
        , m_sID(sID)
        , m_sStep{ static_cast<T>(0) }
        , m_st { 0 }
    {}

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline BOOL CStatemachine<T>::Execute(void)
    {
        assert(m_bInit);
        return m_bInit;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CStatemachine<T>::InitExecute(void)
    {
        assert(!m_bInit);
        BOOL result = FALSE;
        if ( ! m_bInit)
        {
            m_bInit = TRUE;
            m_sStep = static_cast<T>(0);
            m_bRunning = FALSE;
            m_st = 0;
            result = TRUE;
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CStatemachine<T>::ExitExecute(void)
    {
        assert(m_bInit);
        BOOL result = FALSE;
        if (m_bInit)
        {
            m_bInit = FALSE;
            m_sStep = static_cast<T>(0);
            m_bRunning = FALSE;
            m_st = 0;
            result = TRUE;
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CStatemachine<T>::Start(const uint32_t t)
    {
        assert(m_bInit);
        BOOL result = FALSE;
        if ( ! m_bRunning)
        {
            m_bRunning = TRUE;
            m_sStep = static_cast<T>(0);
            m_st = t;
            result = TRUE;
        }
        return result;
    }



    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CStatemachine<T>::Stop(void)
    {
        assert(m_bInit);
        BOOL result = FALSE;
        if ( m_bRunning )
        {
            m_bRunning = FALSE;
            m_sStep = static_cast<T>(0);
            m_st = 0;
            result = TRUE;
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CStatemachine<T>::Update(const uint32_t t)
    {
        assert(m_bInit);
        assert(m_bRunning);
        m_st = t;
        return TRUE;
    }

//******************************************************************************************************************************************
//******************************************************************************************************************************************

    template <typename T = int32_t>
    class CTimerStatemachine : public CStatemachine<T>
    {
    protected:
        base::utils::CTimer	m_aTimer;
        uint32_t            m_zt;

    protected:
        virtual BOOL Clock(void);

    public:
        CTimerStatemachine(int32_t sID = -1);
        virtual ~CTimerStatemachine() = default;

        virtual BOOL Execute(void) override;
        virtual BOOL InitExecute(void) override;
        virtual BOOL ExitExecute(void) override;
        virtual BOOL Start(const uint32_t t = 0) override;
        virtual BOOL Stop(void) override;
    };


    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline CTimerStatemachine<T>::CTimerStatemachine(int32_t sID) : CStatemachine<T>(sID)
        , m_zt(0L)
    {}

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline BOOL CTimerStatemachine<T>::Execute(void)
    {
        auto result = CStatemachine<T>::Execute();
        if (result)
        {
            result = Clock();
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CTimerStatemachine<T>::InitExecute(void)
    {
        auto result = CStatemachine<T>::InitExecute();
        if (result)
        {
            m_zt = 0;
            m_aTimer.Start();
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CTimerStatemachine<T>::ExitExecute(void)
    {
        auto result = CStatemachine<T>::IsInit();
        if (result)
        {
            m_aTimer.Stop();
            result = CStatemachine<T>::ExitExecute();
        }
        return result;
    }


    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CTimerStatemachine<T>::Start(const uint32_t t)
    {
        auto result = CStatemachine<T>::Start(t);
        if (result)
        {
            result = Clock();
            assert(result);
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CTimerStatemachine<T>::Stop(void)
    {
        auto result = CStatemachine<T>::IsStarted();
        if (result)
        {
            Clock();
            result = CStatemachine<T>::Stop();
        }
        return result;
    }

    //--------------------------------------------------------------------------------------------------
    template <typename T>
    inline	BOOL CTimerStatemachine<T>::Clock(void)
    {
        auto result = m_aTimer.Readzs(m_zt);
        if (result)
        {
            this->m_st = m_zt / 10U;
        }
        else
        {
            this->m_st = m_zt = 0;
        }
        return result;
    }


};


