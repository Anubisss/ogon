#include "Server.h"
#include "Client.h"

namespace onlinegameofnim
{
namespace Server
{

using namespace System::IO;
using namespace System::Text;

void Server::Start()
{
    DebugMessage(nullptr, String::Format("Server starting at {0}:{1}", _ip, _port));
    _listener = gcnew TcpListener(_ip, _port);
    try
    {
        _listener->Start();
    }
    catch (SocketException^ ex)
    {
        DebugMessage(nullptr, "Server can't start.");
        DebugMessage(nullptr, String::Format("Error: {0}", ex->Message));
        return;
    }
    _ballContainer = gcnew BallContainer(3, 6, 2, 8);
    AcceptConnections();
}

void Server::AcceptConnections()
{
    DebugMessage(nullptr, "Waiting for a connection...");
    _listener->BeginAcceptTcpClient(gcnew AsyncCallback(&Server::OnConnectionAccept), _listener);
}

void Server::OnConnectionAccept(IAsyncResult^ result)
{
    TcpListener^ listener = (TcpListener^)result->AsyncState;
    TcpClient^ tcpClient = listener->EndAcceptTcpClient(result);
    DebugMessage(nullptr, String::Format("Client connected from {0}", tcpClient->Client->RemoteEndPoint->ToString()));

    // creates a new client
    array<Byte>^ buffer = gcnew array<Byte>(tcpClient->ReceiveBufferSize);
    Client^ client = gcnew Client(tcpClient, buffer);
    _clients.Add(client);

    NetworkStream^ networkStream = client->GetStream();
    networkStream->BeginRead(client->GetBuffer(), 0, client->GetBuffer()->Length, gcnew AsyncCallback(&Server::OnRead), client);

    listener->BeginAcceptTcpClient(gcnew AsyncCallback(&Server::OnConnectionAccept), listener);
}

void Server::OnRead(IAsyncResult^ result)
{
    Client^ client = (Client^)result->AsyncState;
    if (!client->IsConnected())
    {
        DebugMessage(client, "NOT CONNECTED Server::OnRead");
        System::Diagnostics::Debug::Assert(false);
    }

    NetworkStream^ networkStream = client->GetStream();

    int bytesRead = 0;
    try
    {
        bytesRead = networkStream->EndRead(result);
    }
    catch (IOException^)
    {
        if (bytesRead == 0)
        {
            DebugMessage(client, "Client connection closed!");
            client->SetDisconnected();
            _clients.Remove(client);
            client->GetTcpClient()->Close();

            // if a connection stay alive server has to reset its opponent
            // TODO: send a packet to the stayalive connection which tells it its opponent is disconnected
            if (_clients.Count > 0 && _clients.Count < SERVER_MAX_PLAYERS)
            {
                _clients[0]->SetOpponent(nullptr);
                Protocol::ServerMessages::MessageOpponentName msgOppName; // no opponent name means opponent disconnected
                Write(_clients[0], msgOppName.Serialize());
            }
            return;
        }
        else
            throw;
    }

    if (bytesRead == 0)
        System::Diagnostics::Debug::Assert(false);

    String^ data = Encoding::ASCII->GetString(client->GetBuffer(), 0, bytesRead);
    DebugMessage(client, String::Format("Received data: {0}", data));
    networkStream->BeginRead(client->GetBuffer(), 0, client->GetBuffer()->Length, gcnew AsyncCallback(&Server::OnRead), client);

    array<String^>^ seperators = gcnew array<String^>(1);
    seperators[0] = PACKET_SEPARATOR;
    array<String^>^ packets = data->Split(seperators, StringSplitOptions::RemoveEmptyEntries);
    for each(String^ packet in packets)
    {
        int payloadSeparatorPos = packet->IndexOf(PACKET_PAYLOAD_SEPARATOR);
        String^ opcode = packet->Substring(0, payloadSeparatorPos);
        String^ payload = packet->Substring(payloadSeparatorPos + 1, packet->Length - payloadSeparatorPos - 1);

        // DebugMessage(client, String::Format("opcode: {0} payload: {1}", opcode, payload));

        // TODO: create opcode handler functions
        if (opcode == Protocol::ClientMessages::MessageHello::GetOpcode())
        {
            Protocol::ServerMessages::MessageWelcome msgWelcome;
            // server is full
            if (IsServerFull())
                msgWelcome.SetServerFull();
            else if (_clients.Count > 1)
                _clients[0]->SetOpponent(client);

            Write(client, msgWelcome.Serialize());

            client->SetAuthenticated();
            client->SetName(payload);

            if (IsServerFull())
                break;

            Protocol::ServerMessages::MessageOpponentName msgOppName;
            if (Client^ opponent = client->GetOpponent())
            {
                // DebugMessage(client, String::Format("{0} {1}", client->GetName(), opponent->GetName()));
                if (opponent->IsAuthenticated())
                {
                    msgOppName.SetOpponentName(opponent->GetName());

                    // sends to the opponent also
                    Protocol::ServerMessages::MessageOpponentName msgOppName2;
                    msgOppName2.SetOpponentName(client->GetName());
                    Write(opponent, msgOppName2.Serialize());
                }
            }
            Write(client, msgOppName.Serialize());

            // this means there are exactly 2 players on the server
            // so we can generate the balls
            if (Client^ opponent = client->GetOpponent())
                if (opponent->IsAuthenticated())
                {
                    _ballContainer->GenerateBalls();

                    List<int /* ballCount */>^ ballCountInColumns = gcnew List<int>();
                    for (int i = 0; i < _ballContainer->GetColumnCount(); ++i)
                        ballCountInColumns->Add(_ballContainer->GetBallCount(i));

                    Protocol::ServerMessages::MessageBalls balls;
                    balls.SetBallCountInColumns(ballCountInColumns);
                    Broadcast(balls.Serialize());
                }
        }
        else if (!client->IsAuthenticated())
            throw;
        else if (opcode == Protocol::ClientMessages::MessageRemoveBall::GetOpcode())
        {
            int column = -1;
            int ball = -1;

            array<String^>^ ballCountSeparators = gcnew array<String^>(1);
            ballCountSeparators[0] = PACKET_PAYLOAD_DATA_SEPARATOR;
            array<String^>^ columnBall = payload->Split(ballCountSeparators, StringSplitOptions::RemoveEmptyEntries);
            column = Convert::ToInt32(columnBall[0]);
            ball = Convert::ToInt32(columnBall[1]);

            _ballContainer->PrintBalls();
            DebugMessage(client, String::Format("MessageRemoveBall column: {0} ball: {1}", column, ball));

            _ballContainer->RemoveBall(column, ball);

            _ballContainer->PrintBalls();

            List<int /* ballCount */>^ ballCountInColumns = gcnew List<int>();
            for (int i = 0; i < _ballContainer->GetColumnCount(); ++i)
                ballCountInColumns->Add(_ballContainer->GetBallCount(i));

            Protocol::ServerMessages::MessageBalls balls;
            balls.SetBallCountInColumns(ballCountInColumns);
            Broadcast(balls.Serialize());
        }
    }
}

void Server::Write(Client^ client, String^ data)
{
    if (!client->IsConnected())
    {
        DebugMessage(client, "NOT CONNECTED Server::Write");
        System::Diagnostics::Debug::Assert(false);
    }

    DebugMessage(client, String::Format("Sent data: {0}", data));
    array<Byte>^ bytes = Encoding::ASCII->GetBytes(data);
    NetworkStream^ networkStream = client->GetStream();
    networkStream->BeginWrite(bytes, 0, bytes->Length, gcnew AsyncCallback(&Server::OnWrite), client);
}

void Server::Broadcast(String^ data)
{
    DebugMessage(nullptr, String::Format("Broadcast data: {0}", data));
    // iterates over the clients
    for each (Client^ client in _clients)
        Write(client, data);
}

void Server::OnWrite(IAsyncResult^ result)
{
    Client^ client = (Client^)result->AsyncState;

    if (!client->IsConnected())
    {
        // DebugMessage(client, "NOT CONNECTED Server::OnWrite");
        return;
    }

    NetworkStream^ networkStream = client->GetStream();
    networkStream->EndWrite(result);
}

}
}
