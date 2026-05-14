//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Logger
///
/// @file   Logger.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <fstream>
#include "BASE/types.h"
#include "LogSocketClient.h"


//#define SERVERNAME "localhost"

//#define USEFILE
#ifdef USEFILE
FILE* out = nullptr;
#endif

static BOOL init = FALSE;


static int readIPAddress(std::string& ipaddress)
{
    int32_t result = -98;
    std::ifstream ifs;

    ifs.open("logger.cfg", std::istream::in);
    std::string line;
    BOOL found = FALSE;
    while (ifs.good())
    {
        std::getline(ifs, line);
        if (line.length() > 5)
        {
            found = TRUE;
            break;
        }
    }
    if (found)
    {
        ipaddress = line;
        result = 0;
    }
    ifs.close();
    return result;
}


static int writeIPAddress(std::string& ipaddress)
{
    int32_t result = 0;
    std::ofstream ofs;

    ofs.open("logger.bak");
    std::string line;
    ofs << ipaddress;
    ofs.close();
    return result;
}

#ifdef USEFILE
int OpenDebug()
{
     auto err = fopen_s(&out, "C:\\temp\\ECLogfile.log", "wt");
    if (!out)
    {
        return -1;
    }
    return 0;
}

void OutputDebug(const char* message)
{
    if (out)
    {
        fprintf(out, "%s", message);
    }
}


int CloseDebug()
{
    if (out)
    {
        fclose(out);
        out = nullptr;
    }
    return 0;
}
#else
int OpenDebug()
{
    int result = -1;
    if (!init)
    {
        init = TRUE;
        std::string ipaddress;
        result = readIPAddress(ipaddress);
        writeIPAddress(ipaddress);
        if (result == 0)
        {
            result = logsocket::open(ipaddress.c_str());
        }
    }
    return result;
}


void OutputDebug(const char* message)
{
    (void)logsocket::write(message);
}

int CloseDebug()
{
    int result = -1;
    if (init)
    {
        init = FALSE;
        result = logsocket::close();
    }
    return result;
}

#endif