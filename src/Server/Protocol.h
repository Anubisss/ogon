#pragma once

/*
    The packets are separated with PACKET_SEPARATOR.
    Opcode and payload is separated with PACKET_PAYLOAD_SEPARATOR.
    If a packet doesn't have payload use PACKET_NO_PAYLOAD.

    Every message (packet) have to be inherited from abstract class SocketMessage.

    When a client connects to the server the client sends the MessageHello packet.
    Server responses with this MessageWelcome.
    When the client gets the MessageWelcome the client becomes "authenticated".
    If the server is full the payload contains "true" otherwise contains "false".
*/

namespace onlinegameofnim
{
namespace Protocol
{

using namespace System;
using namespace System::Collections::Generic;

#define SERVER_MAX_PLAYERS              2

#define PACKET_SEPARATOR                ";"
#define PACKET_PAYLOAD_SEPARATOR        ":"
#define PACKET_PAYLOAD_DATA_SEPARATOR   ","
#define PACKET_NO_PAYLOAD              "no_payload"

public ref class SocketMessage abstract
{
public:
    //virtual String^ GetOpcode() = 0;
    virtual String^ Serialize() = 0;
};

namespace ClientMessages
{

public ref class MessageHello : SocketMessage
{
public:
    MessageHello() { _playerName = nullptr; }

    static String^ GetOpcode() { return "hello"; }
    virtual String^ Serialize() override
    {
        return GetOpcode() + PACKET_PAYLOAD_SEPARATOR + _playerName + PACKET_SEPARATOR;
    }

    void SetPlayerName(String^ playerName) { _playerName = playerName; }

private:
    String^ _playerName;
};

public ref class MessageRemoveBall : SocketMessage
{
public:
    MessageRemoveBall() { _column = -1; _ball = -1; }

    static String^ GetOpcode() { return "remove_ball"; }

    virtual String^ Serialize() override
    {
        return GetOpcode() + PACKET_PAYLOAD_SEPARATOR +
               _column + PACKET_PAYLOAD_DATA_SEPARATOR + _ball +
               PACKET_SEPARATOR;
    }

    void SetColumn(int column) { _column = column; }
    void SetBall(int ball) { _ball = ball; }

private:
    int _column;
    int _ball;
};

}

namespace ServerMessages
{

public ref class MessageWelcome : SocketMessage
{
public:
    MessageWelcome() { _serverFull = false; }

    static String^ GetOpcode() { return "welcome"; }
    virtual String^ Serialize() override
    {
        String^ serverFullStr = _serverFull == true ? "true" : "false";
        return GetOpcode() + PACKET_PAYLOAD_SEPARATOR + serverFullStr + PACKET_SEPARATOR;
    }

    void SetServerFull() { _serverFull = true; }

private:
    bool _serverFull;
};

public ref class MessageOpponentName : SocketMessage
{
public:
    MessageOpponentName() { _opponentName = "false"; }

    static String^ GetOpcode() { return "opponent_name"; }
    virtual String^ Serialize() override
    {
        return GetOpcode() + PACKET_PAYLOAD_SEPARATOR + _opponentName + PACKET_SEPARATOR;
    }

    void SetOpponentName(String^ opponentName) { _opponentName = opponentName; }

private:
    String^ _opponentName;
};

// this packet contains literally the server's ball container
// so it stores the container has how many columns and each column has how many balls righ now
public ref class MessageBalls : SocketMessage
{
public:
    MessageBalls() { _ballCountInColumns = nullptr; }

    static String^ GetOpcode() { return "balls"; }
    virtual String^ Serialize() override
    {
        String^ serializedData;
        serializedData = GetOpcode() + PACKET_PAYLOAD_SEPARATOR;

        for each (int ballCount in _ballCountInColumns)
            serializedData += ballCount + PACKET_PAYLOAD_DATA_SEPARATOR;

        // removes the BallCountSeparator at the end of the string
        serializedData += ",",
        serializedData = serializedData->TrimEnd(PACKET_PAYLOAD_DATA_SEPARATOR[0]);

        serializedData += PACKET_SEPARATOR;

        return serializedData;
    }

    void SetBallCountInColumns(List<int>^ ballCountInColumns)
    {
        _ballCountInColumns = ballCountInColumns;
    }

private:
    List<int /* ballCount */>^ _ballCountInColumns;
};

}

}
}
