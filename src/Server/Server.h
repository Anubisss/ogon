#pragma once

#include "Client.h"
#include "Protocol.h"
#include "BallContainer.h"

namespace onlinegameofnim
{
namespace Server
{

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;
using namespace System::Collections::Generic;

public ref class Server
{
public:
    Server(IPAddress^ ip, Int32 port) { _ip = ip; _port = port; }

    // starts the server
    void Start();

    static bool IsServerFull() { return _clients.Count > SERVER_MAX_PLAYERS; }

    static void DebugMessage(Client^ client, String^ message)
    {
        String^ clientName = nullptr;
        if (client && !(clientName = client->GetName()))
            clientName = "NO_NAME";
        Console::WriteLine("[{0} {1}] {2}", DateTime::Now.ToString("HH:mm:ss"), clientName ? clientName : "SERVER", message);
    }

private:
    // starts accepting connections asynchronously
    void AcceptConnections();

    // called if a client connected
    static void OnConnectionAccept(IAsyncResult^ result);

    // called if a client sent a packet and it's read by the server
    static void OnRead(IAsyncResult^ result);

    // sends packets to the given client
    static void Write(Client^ client, String^ data);

    // sends packets to all the connected clients
    static void Broadcast(String^ data);

    // called when data writing is ready
    static void OnWrite(IAsyncResult^ result);

private:
    IPAddress^ _ip;
    Int32 _port;
    TcpListener^ _listener;
    static List<Client^> _clients;

    static BallContainer^ _ballContainer;
};

}
}
