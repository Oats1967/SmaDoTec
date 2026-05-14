//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaratiob of module VKSocket
///
/// @file   VKSocket.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
/// 
//-----------------------------------"-------------------------------------------------


enum eSocketError
{
    eSocket_OK = 0,
    eSocket_NotOpen,
    eSocket_WSAStartup_failed,
    eSocket_getaddrinfo_failed,
    eSocket_ConnectSocket_invalid,
    eSocket_shutdown_failed,
    eSocket_send_failed,
};

eSocketError VKSocket_Open();
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
eSocketError VKSocket_Close();
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
eSocketError VKSocket_SendBuffer(LPCSTR send_buf, int length);
