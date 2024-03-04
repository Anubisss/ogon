#pragma once

namespace onlinegameofnim
{
namespace Client
{

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;

ref class MainForm;

public ref class Client
{
public:
    ref class ServerIsFullException : public SystemException {};

public:
    Client(MainForm^ mainForm, String^ ip, int port);

    void Connect();
    void BeginRead();

    // called when data ready to read
    static void OnRead(IAsyncResult^ result);

    void Write(String^ data);

    // called when data writing is ready
    static void OnWrite(IAsyncResult^ result);

    IPEndPoint^ GetServerEndPoint() { return _serverEndPoint; }

private:
    // processes the received data/packets
    static void ProcessReceivedData(String^ data);
    // handles if there is any exception in OnRead
    static void HandleExceptions(SystemException^ ex, TcpClient^ client, NetworkStream^ networkStream);

private:
    IPEndPoint^ _serverEndPoint;
    TcpClient^ _tcpClient;
    static array<Byte>^ _buffer;

    static MainForm^ _mainForm;
};

}
}
