#include "Server.h"

using namespace onlinegameofnim::Server;

int main()
{
    Server server(IPAddress::Any, 8429);
    server.Start();
    while (1)
        Threading::Thread::Sleep(100); // 100 ms
    return 0;
}
