#pragma once

namespace logsocket
{
    int open(const char *servername);
    int write(const char* sendbuf);
    int close();
};