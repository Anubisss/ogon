#pragma once

namespace onlinegameofnim
{
namespace Server
{

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;

public ref class Client
{
public:
    enum class ConnectionState
    {
        CONNECTION_STATE_CONNECTED,
        CONNECTION_STATE_DISCONNECTED
    };

public:
    Client(TcpClient^ tcpClient, array<Byte>^ buffer)
    {
        _tcpClient = tcpClient;
        _buffer = buffer;
        _name = nullptr;
        _authenticated = false;

        _connectionState = ConnectionState::CONNECTION_STATE_CONNECTED; // always connected when a Client created

        _opponent = nullptr;
    }

    array<Byte>^ GetBuffer() { return _buffer; }

    NetworkStream^ GetStream() { return _tcpClient->GetStream(); }

    TcpClient^ GetTcpClient() { return _tcpClient; }

    bool IsAuthenticated() { return _authenticated; }
    void SetAuthenticated() { _authenticated = true; }

    bool IsConnected() { return _connectionState == ConnectionState::CONNECTION_STATE_CONNECTED; }
    void SetDisconnected() { _connectionState = ConnectionState::CONNECTION_STATE_DISCONNECTED; }

    String^ GetName() { return _name; }
    void SetName(String^ name) { _name = name; }

    Client^ GetOpponent() { return _opponent; }
    // sets the opponent in both way
    void SetOpponent(Client^ opponent)
    {
        if (!_opponent)
            _opponent = opponent;
        if (opponent && !opponent->GetOpponent())
            opponent->SetOpponent(this);
    }

private:
    TcpClient^ _tcpClient;
    array<Byte>^ _buffer;

    ConnectionState _connectionState;

    bool _authenticated;
    String^ _name;

    Client^ _opponent;
};

}
}
