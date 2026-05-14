//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Identify.cpp
///
///         IDENTIFY-Daten einer IDE-Festplatte anzeigen
///
/// @file   Identify.cpp
///
///
/// @coypright(c)  2001 c't/Matthias Withopf
///                GNU General Public License
///
/// @author        Matthias Withopf, modified and bugfix by Detlef Hafer
///
//------------------------------------------------------------------------------------

//----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <stdlib.h>
#include <cassert>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#include "hostid.h"
#include "Logger/include/Logger.h"


// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define ADAPTER L"Port3"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#if 1
DWORD GetHostID()
{
    const ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
    // default to unspecified address family (both)
    const ULONG family = AF_INET;
    ULONG outBufLen = WORKING_BUFFER_SIZE;

    DWORD lAddress = 0L;
    PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)MALLOC(outBufLen);
    DWORD dwRetVal =  GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
    if (dwRetVal == NO_ERROR)
    {
        // If successful, output some information from the data we received
        PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
        std::wstring adaptername{ ADAPTER };
        while ( pCurrAddresses )
        {
            if (_wcsicmp(pCurrAddresses->FriendlyName, adaptername.c_str()) == 0)
            {
                PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = pCurrAddresses->FirstUnicastAddress;
                while (pUnicast)
                {
                    SOCKADDR* pSockAddr = pUnicast->Address.lpSockaddr;
                    if (pSockAddr->sa_family == AF_INET)
                    {
                        sockaddr_in* pInAddr = (sockaddr_in*)pSockAddr;
                        lAddress = pInAddr->sin_addr.S_un.S_addr;
                        break;
                    }
                    pUnicast = pUnicast->Next;
                }
                assert(lAddress);
                if ( lAddress )
                {
                    break;
                }
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    FREE(pAddresses);
    return lAddress;
}
#else
static BOOL getHostByName(LPSTR szLclHost, hostent* hst)
{
    addrinfo hints = { 0 };
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_CANONNAME;

    addrinfo* p_addrs = NULL;
    int result = getaddrinfo(szLclHost, NULL, &hints, &p_addrs);
    if (result == 0)
    {
        static std::vector<in_addr*> in_addrs;

        in_addrs.clear();
        for (addrinfo* p_addr = p_addrs; p_addr != NULL; p_addr = p_addr->ai_next)
        {
            in_addrs.push_back(&reinterpret_cast<sockaddr_in*>(p_addr->ai_addr)->sin_addr);
        }
        in_addrs.push_back(NULL);

        memset(hst, 0, sizeof(*hst));
        hst->h_name = p_addrs->ai_canonname;
        hst->h_aliases = NULL;
        hst->h_addrtype = AF_INET;
        hst->h_length = sizeof(in_addr);
        hst->h_addr_list = reinterpret_cast<char**>(&in_addrs[0]/* or: in_addrs.data() */);
    }
    freeaddrinfo(p_addrs);
    return BOOL(result == 0);
}

DWORD GetHostID()
{
    char szLclHost[120];
    SOCKADDR_IN stLclAddr;
    int nAddrSize = sizeof(SOCKADDR);
    /* Init local address (to zero) */
    stLclAddr.sin_addr.s_addr = INADDR_ANY;
    /* Get the local hostname */
    int nRet = gethostname(szLclHost, sizeof(szLclHost));
    if (nRet != SOCKET_ERROR)
    {
        HOSTENT     Hostent;
        auto result = getHostByName((LPSTR)szLclHost, &Hostent);
        if (result)
        {
            stLclAddr.sin_addr.s_addr = *((u_long FAR*) (Hostent.h_addr));
        }
    }
    /* If still not resolved, then try second strategy */
    if (stLclAddr.sin_addr.s_addr == INADDR_ANY)
    {
        /* Get a UDP socket */
        SOCKET hSock = socket(AF_INET, SOCK_DGRAM, 0);
        if (hSock != INVALID_SOCKET)
        {
            SOCKADDR_IN stRmtAddr;
            /* Connect to arbitrary port and address (NOT loopback) */
            stRmtAddr.sin_family = AF_INET;
            stRmtAddr.sin_port = htons(IPPORT_ECHO);
            //stRmtAddr.sin_addr.s_addr = inet_addr("128.127.50.1");

            InetPton(AF_INET, "128.127.50.1", &stRmtAddr.sin_addr.s_addr);
            nRet = connect(hSock, (LPSOCKADDR)&stRmtAddr, sizeof(SOCKADDR));
            if (nRet != SOCKET_ERROR)
            {
                /* Get local address */
                getsockname(hSock, (LPSOCKADDR)&stLclAddr, (int FAR*) & nAddrSize);
            }
            closesocket(hSock);   /* we're done with the socket */
        }
    }
    return stLclAddr.sin_addr.s_addr;
}
#endif
