#include "Client.h"
#include "../Server/Protocol.h"
#include "MainForm.h"

namespace onlinegameofnim
{
namespace Client
{

using namespace System::IO;
using namespace System::Text;

Client::Client(MainForm^ mainForm, String^ ip, int port)
{
    _serverEndPoint = gcnew IPEndPoint(IPAddress::Parse(ip), port);
    _tcpClient = gcnew TcpClient();
    _mainForm = mainForm;
}

void Client::Connect()
{
    _tcpClient->Connect(_serverEndPoint);
}

void Client::BeginRead()
{
    _buffer = gcnew array<Byte>(1024);
    NetworkStream^ networkStream = _tcpClient->GetStream();
    networkStream->BeginRead(_buffer, 0, _buffer->Length, gcnew AsyncCallback(&Client::OnRead), _tcpClient);
}

void Client::OnRead(IAsyncResult^ result)
{
    TcpClient^ client = (TcpClient^)result->AsyncState;
    NetworkStream^ networkStream = client->GetStream();

    int bytesRead = 0;
    try
    {
        bytesRead = networkStream->EndRead(result);
    }
    catch (IOException^) {}
    if (bytesRead == 0)
    {
        Console::WriteLine("Connection closed!");
        client->Close();
        _mainForm->OnConnectionClosed();
        return;
    }

    String^ data = Encoding::ASCII->GetString(_buffer, 0, bytesRead);
    Console::WriteLine("Received data: {0}", data);

    try
    {
        ProcessReceivedData(data);
    }
    catch (SystemException^ ex)
    {
        HandleExceptions(ex, client, networkStream);
        return;
    }

    networkStream->BeginRead(_buffer, 0, _buffer->Length, gcnew AsyncCallback(&Client::OnRead), client);
}

void Client::Write(String^ data)
{
    Console::WriteLine("Sent data: {0}", data);
    array<Byte>^ bytes = Encoding::ASCII->GetBytes(data);
    NetworkStream^ networkStream = _tcpClient->GetStream();
    networkStream->BeginWrite(bytes, 0, bytes->Length, gcnew AsyncCallback(&Client::OnWrite), _tcpClient);
}

void Client::OnWrite(IAsyncResult^ result)
{
    TcpClient^ client = (TcpClient^)result->AsyncState;
    NetworkStream^ networkStream = client->GetStream();
    networkStream->EndWrite(result);
}

void Client::ProcessReceivedData(String^ data)
{
    array<String^>^ packetSeperators = gcnew array<String^>(1);
    packetSeperators[0] = PACKET_SEPARATOR;
    array<String^>^ packets = data->Split(packetSeperators, StringSplitOptions::RemoveEmptyEntries);
    for each(String^ packet in packets)
    {
        int payloadSeparatorPos = packet->IndexOf(PACKET_PAYLOAD_SEPARATOR);
        String^ opcode = packet->Substring(0, payloadSeparatorPos);
        String^ payload = packet->Substring(payloadSeparatorPos + 1, packet->Length - payloadSeparatorPos - 1);

        // Console::WriteLine("opcode: {0} payload: {1}", opcode, payload);

        if (opcode == Protocol::ServerMessages::MessageWelcome::GetOpcode())
        {
            // server is full
            if (payload == "true")
                throw gcnew ServerIsFullException();
            // connection is OK
            else
                _mainForm->OnConnectedToServer();
        }
        else if (opcode == Protocol::ServerMessages::MessageOpponentName::GetOpcode())
        {
            _mainForm->OnRecvOpponentName(payload);
        }
        else if (opcode == Protocol::ServerMessages::MessageBalls::GetOpcode())
        {
            List<int /* ballCount */>^ ballCountInColumns = gcnew List<int>();

            array<String^>^ ballCountSeparators = gcnew array<String^>(1);
            ballCountSeparators[0] = PACKET_PAYLOAD_DATA_SEPARATOR;
            array<String^>^ ballCounts = payload->Split(ballCountSeparators, StringSplitOptions::RemoveEmptyEntries);
            for each (String^ ballCount in ballCounts)
                ballCountInColumns->Add(Convert::ToInt32(ballCount));

            _mainForm->OnRecvBalls(ballCountInColumns);
        }
    }
}

void Client::HandleExceptions(SystemException^ ex, TcpClient^ client, NetworkStream^ networkStream)
{
    if (dynamic_cast<ServerIsFullException^>(ex) != nullptr)
    {
        networkStream->Close();
        client->Close();
        _mainForm->OnServerIsFull();
    }
    // should abort
    else
        throw;
}

}
}
