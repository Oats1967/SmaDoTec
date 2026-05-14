//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteConfig
///
/// @file   RemoteConfig.h
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

#include <array>
#include "BASE/types.h"

namespace base
{
	enum class RemoteServerError : uint32_t
	{
		REMOTE_OK = 0,
		REMOTE_ERROR_STARTUP_ERROR,
		REMOTE_ERROR_ILLEGAL_COMMAND,
		REMOTE_ERROR_NOT_OPEN_ERROR,
		REMOTE_ERROR_OPEN_ERROR,
		REMOTE_ERROR_LOOP_FAILED,
		REMOTE_ERROR_XMLREADER_FAILED,
		REMOTE_ERROR_READ_DEVICE,
		REMOTE_ERROR_WRITE_DEVICE,
	};

	enum class RemoteServerStatus : uint32_t
	{
		REMOTE_STATUS_DISABLED = 0,
		REMOTE_STATUS_OPEN,
		REMOTE_STATUS_CONNECTED,
		REMOTE_STATUS_DISCONNECTED,
		REMOTE_STATUS_CLOSED,
		REMOTE_STATUS_INPUTOFFLINE,
		REMOTE_STATUS_ERROR
	};


	enum class RemoteServerProtocol :int32_t
	{
		REMOTE_PROTOCOL_NONE = 0,
		REMOTE_PROTOCOL_OPENMODBUS,
		REMOTE_PROTOCOL_PROFINET,
		REMOTE_PROTOCOL_S7SNAP,
		REMOTE_PROTOCOL_S7SIMULATOR,
		REMOTE_PROTOCOL_PROFIBUS,
		REMOTE_PROTOCOL_MAX
	};
};

#define DECLARE_REMOTESERVERPROTOCOL(field) \
	const std::array<base::RemoteServerProtocol, _S32(base::RemoteServerProtocol::REMOTE_PROTOCOL_MAX)> field = { \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_NONE, \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_OPENMODBUS, \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFINET, \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SNAP, \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_S7SIMULATOR, \
    base::RemoteServerProtocol::REMOTE_PROTOCOL_PROFIBUS, \
};




