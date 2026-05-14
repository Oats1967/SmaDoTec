//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfControl.h
///
/// @file   LwfControl.h
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

#include "AlarmControl.h"
#include "AdsDriveControl.h"


class CDriveControl : public CAlarmControl
{
	static constexpr int32_t DREHGEBERINTERVALL =  5;

protected:
	CAdsDriveControl m_AdsClient;
	uint32_t		 m_tSampleTime;
	uint32_t		 m_tStartTime;
	BOOL			 m_bValidDriveCommand;

private:
	void SetDriveCommand(const float32_t value);
	void SetOperate(const BOOL bRun);
	void SetDriveRelease(const BOOL bRelease);
	void InternalStop();

	void Check(const uint32_t t);
	void CheckDriveError(const uint32_t t);
	void CheckDriveErrorPaddle(const uint32_t t);
	void CheckMotorLoad(const uint32_t t);
	uint32_t GetActEncoderSpeed() const;

	BOOL GetRunState() const;


public:
	CDriveControl(const int32_t id) : CAlarmControl{ id }
		, m_AdsClient { id }
		, m_tSampleTime{ 0 }
		, m_tStartTime{ 0 }
		, m_bValidDriveCommand{ FALSE }
	{}
	virtual ~CDriveControl(void) = default;
	CDriveControl(const CDriveControl&) = delete;
	CDriveControl& operator=(const CDriveControl&) = delete;

	BOOL Update(const uint32_t t) override;
	BOOL Start(const uint32_t t) override;
	BOOL Stop() override;

	//------------------------------------------------------------------------------------
	///  @brief   UpdateDriveCommand
	///
	///           This method ....
	///
	///  @param[in] f , float32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual void UpdateDriveCommand(const float32_t value);

	BOOL InitExecute() override;
	BOOL ExitExecute() override;
};



