//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseCurve
///
/// @file   DoseCurve.h
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

#include <cassert>
#include <array>
#include <limits>
#include "BASE/Base.h"
#include "BASE/math/public/DoseTriple.h"


namespace base
{
namespace math
{
template<uint32_t N>
class CDoseCurve
{
    using DoseArrayPair = std::array<DosePair, N>;
    using DoseArrayTriple = std::array<DoseTriple, N>;

    DoseArrayTriple  m_aC;
    DoseTriple       m_Dummy;
    uint32_t         m_lCount;
    BOOL             m_bModified;

private:
    //------------------------------------------------------------------------------------
    ///  @brief   Calc
    ///
    ///           This method calculates the interpolated value between two sampling points
    ///
    ///  @param[in] fNomwert , const float32_t
    ///  @param[in] ko , int32_t upper point
    ///  @param[in] ku , int32_t lower point
    ///  @return float32_t
    ///
    //------------------------------------------------------------------------------------
    template< eDoseTriple T>
    float32_t Calc(int32_t ku, int32_t ko, const float32_t fNomwert) const;

    //------------------------------------------------------------------------------------
    ///  @brief   SearchIndex
    ///
    ///           This method calculate the drive command using interpolation
    ///
    ///  @param[in] fNomwert , const float32_t
    ///  @return float32_t
    ///
    //------------------------------------------------------------------------------------
    void SearchIndex(const float32_t fNomwert, int32_t& ku, int32_t& ko) const;


public:
    CDoseCurve(void) : m_aC()
        , m_Dummy(std::make_tuple(0.0F, 0.0F, 0.0F))
        , m_lCount(0)
        , m_bModified(FALSE)
    {}

    ~CDoseCurve(void) = default;
    //------------------------------------------------------------------------------------
    ///  @brief   GetInterpolationValue
    ///
    ///           This method calculate the drive command using interpolation
    ///
    ///  @param[in] fNomwert , const float32_t
    ///  @return float32_t
    ///
    //------------------------------------------------------------------------------------
    float32_t GetDriveCommand(const float32_t fNomwert) const;
    float32_t GetDosePerformance(const float32_t fNomwert) const;

    void Set(const int32_t index, const DosePair& aC);
    DoseTriple& Get(const int32_t index);
    const DoseTriple& Get(const int32_t index) const;

    void SetCount(const uint32_t n);
    uint32_t GetCount(void) const
    { return m_lCount;  }

    void Init(void);
    void Push(const DosePair& aC);

    //------------------------------------------------------------------------------------
    ///  @brief   SortDriveCommand
    ///
    ///           This method calculate the drive command using interpolation
    ///
    ///  @param[in] fNomwert , const float32_t
    ///  @return float32_t
    ///
    //------------------------------------------------------------------------------------
    void SortDriveCommand();
};

//------------------------------------------------------------------------------------
///  @brief   Init
///
///           This method initializes local member variables
///
///  @param[in/out] none
///  @return none
///
//------------------------------------------------------------------------------------
template<uint32_t N>
inline void CDoseCurve<N>::Init(void)
{
    m_lCount = 0;
    m_aC.fill(std::make_tuple( 0.0F, 0.0F, 0.0F ));
}
//------------------------------------------------------------------------------------
///  @brief   Push
///
///           This method pushes a cure point on stack
///
///  @author  Detlef Hafer
///
///  @class        CDoseCurve
///
///  @param[in] aC , const CurvePair&
///  @return none
///
//------------------------------------------------------------------------------------
template<uint32_t N>
inline void CDoseCurve<N>::Push(const DosePair& aC)
{
    assert(m_lCount < N);
    if (m_lCount < N)
    {
        m_aC[m_lCount] = std::make_tuple(aC.first, aC.second, aC.first * aC.second);
        m_lCount++;
        m_bModified = TRUE;
    }
}
//-----------------------------------------------------------------------------------------------------
template<uint32_t N>
inline void CDoseCurve<N>::Set(const int32_t index, const DosePair& aC)
{
    assert(index < _S32(m_lCount));
    if (index < _S32(m_lCount))
    {
        m_aC[index] = std::make_tuple(aC.first, aC.second, aC.first * aC.second);;
        m_bModified = TRUE;
    }
}
//-----------------------------------------------------------------------------------------------------
template<uint32_t N >
inline void CDoseCurve<N>::SetCount(const uint32_t count)
{
    assert(count >= 0 && count <= N);
    m_lCount = count;
}

//-----------------------------------------------------------------------------------------------------
template<uint32_t N >
inline DoseTriple& CDoseCurve<N>::Get(const int32_t index)
{
    assert(index < _S32(m_lCount));
    if (index < _S32(m_lCount))
    {
        return m_aC[index];
    }
    return m_Dummy;
}
//-----------------------------------------------------------------------------------------------------
template<uint32_t N >
inline const DoseTriple& CDoseCurve<N>::Get(const int32_t index) const
{
    assert(index < _S32(m_lCount));
    if (index < _S32(m_lCount))
    {
        return m_aC[index];
    }
    return m_Dummy;
}

//******************************************************************************************************
//******************************************************************************************************
template<uint32_t N>
float32_t CDoseCurve<N> ::GetDriveCommand(const float32_t fNomwert) const
{
    if (m_lCount <= 0)
    {
        return 0.0F;
    }
    assert(!m_bModified);

    int32_t ku = 0;
    int32_t ko = 0;
    SearchIndex(fNomwert, ku, ko);

    float32_t fS = 0.0F;
    if (ku >= 0)
    {
        //! Linker Punkt existiert
        if (ko >= 0)
        {
            //! auch rechter Punkt gefunden => Interpolieren
            fS = Calc < eDoseTriple::ePosDriveCommand >(ku, ko, fNomwert);
        }
        else
        {
            //! nur linker Punkt, kein rechter Punkt
            if (ku > 0)
            {
                //! Zwei linke Punke => extrapolieren
                fS = Calc< eDoseTriple::ePosDriveCommand >(ku - 1, ku, fNomwert);
            }
            else
            {
                //! Nur einen linken Punkt, ku = 0
                const auto& fDosefactor = std::get<eDoseTriple::ePosDosePerformance>(m_aC[0]);
                fS = FDIV0(fNomwert, fDosefactor);
            }
        }
    }
    else
    {
        //! Linker Punkt existiert nicht
        if (ko >= 0)
        {
            if (ko > 0)
            {
                //! Zwei rechte Punke
                fS = Calc< eDoseTriple::ePosDriveCommand >(ko - 1, ko, fNomwert);
            }
            else
            {
                //! Nur einen rechten Punkt, ko = 0
                const auto& fDosefactor = std::get<eDoseTriple::ePosDosePerformance>(m_aC[0]);
                fS = FDIV0(fNomwert, fDosefactor);
            }
        }
        else
        {
            //! kein linker und kein rechter Punkt => unmoeglich
            assert(FALSE);
        }
    }
    return fS;
}
//******************************************************************************************************
//******************************************************************************************************
template<uint32_t N>
float32_t CDoseCurve<N> ::GetDosePerformance(const float32_t fNomwert) const
{
    if (m_lCount <= 0)
    {
        return 0.0F;
    }
    assert(!m_bModified);

    int32_t ku = -1;
    int32_t ko = -1;
    SearchIndex(fNomwert, ku, ko);

    float32_t fD = 0.0F;
    if (ku >= 0)
    {
        //! Linker Punkt existiert
        if (ko >= 0)
        {
            //! auch rechter Punkt gefunden => Interpolieren
            fD = Calc < eDoseTriple::ePosDosePerformance >(ku, ko, fNomwert);
        }
        else
        {
            //! nur linker Punkt, kein rechter Punkt
            fD = std::get<eDoseTriple::ePosDosePerformance>(m_aC[ku]);
        }
    }
    else
    {
        //! Linker Punkt existiert nicht
        if (ko >= 0)
        {
            assert(ko == 0);
            if (ko > 0)
            {
                //! Zwei rechte Punke ?
                fD = Calc< eDoseTriple::ePosDosePerformance >(ko - 1, ko, fNomwert);
            }
            else
            {
                //! Nur einen rechten Punkt, ko = 0
                fD = std::get<eDoseTriple::ePosDosePerformance>(m_aC[0]);
            }
        }
        else
        {
            //! kein linker und kein rechter Punkt => unmoeglich
            assert(FALSE);
        }
    }
    return fD;
}

//******************************************************************************************************
//******************************************************************************************************
template<uint32_t N>
template<eDoseTriple T>
float32_t CDoseCurve<N> ::Calc(const int32_t ku, const int32_t ko, const float32_t fNomwert) const
{
    const auto& fSU = std::get< T >(m_aC[ku]);
    const auto& fSO = std::get< T >(m_aC[ko]);
    const auto& fIstU = std::get< eDoseTriple::cPosThrougput>(m_aC[ku]);
    const auto& fIstO = std::get< eDoseTriple::cPosThrougput>(m_aC[ko]);
    const float64_t fM = FDIV0(fSO - fSU, fIstO - fIstU);
    const float64_t fB = fSU - fM * fIstU;
    const float32_t  fS = _F32(fM * fNomwert + fB);
    return fS;
}
//******************************************************************************************************
//******************************************************************************************************
template<uint32_t N>
void CDoseCurve<N>::SortDriveCommand()
{
    const int32_t count = _S32(m_lCount);

    m_bModified = FALSE;
    if (count <= 1)
    {
        return;
    }

    for (int32_t i = 0; i < (count - 1); i++)
    {
        int32_t k = i;
        for (int32_t j = i + 1; j < count; j++)
        {
            const auto& vk = std::get< eDoseTriple::ePosDriveCommand>(m_aC[k]);
            const auto& vj = std::get< eDoseTriple::ePosDriveCommand>(m_aC[j]);
            if (vj < vk)
            {
                k = j;
            }
        }
        if (k != i)
        {
            DoseTriple temp{ m_aC[k] };
            m_aC[k] = m_aC[i];
            m_aC[i] = temp;
        }
    }
}
//******************************************************************************************************
//******************************************************************************************************
template<uint32_t N>
void CDoseCurve<N> ::SearchIndex(const float32_t fNomwert, int32_t& ku, int32_t& ko) const
{
    const int32_t count = _S32(m_lCount);

    //! Zwei benachbarte Stuetzpunkte suchen
    ku = -1;
    ko = -1;
    float32_t fDeltaU = std::numeric_limits<float32_t>::max();
    float32_t fDeltaO = std::numeric_limits<float32_t>::max();

    for (int32_t i = 0; i < count; i++)
    {
        const auto& fIst = std::get<eDoseTriple::cPosThrougput>(m_aC[i]);
        if (fabs(fIst - fNomwert) < 1e-06)
        {
            ku = ko = i;
            break;
        }
        if (fIst < fNomwert)
        {
            const float32_t fD = fNomwert - fIst;
            if (fD < fDeltaU)
            {
                fDeltaU = fD;
                ku = i;
            }
        }
        else if (fIst > fNomwert)
        {
            const float32_t fD = fIst - fNomwert;
            if (fD < fDeltaO)
            {
                fDeltaO = fD;
                ko = i;
            }
        }
        else
        {
        }
    }
}

}; // namespace utils
}; // namespace math



