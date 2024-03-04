#include "MainForm.h"
#include "../Server/Protocol.h"
#include "Client.h"

namespace onlinegameofnim
{
namespace Client
{

System::Void MainForm::connectToServerToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    ConnectToServerDialog^ dialog = gcnew ConnectToServerDialog();
    if (dialog->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
    {
        String^ serverAddress = dialog->GetServerAddress();
        playerName_ = dialog->GetPlayerName();
        serverAddress = "127.0.0.1:8429";

        int ipPortSeperatorPos = serverAddress->IndexOf(':');
        String^ ip = serverAddress->Substring(0, ipPortSeperatorPos);
        int port = Convert::ToInt32(serverAddress->Substring(ipPortSeperatorPos + 1, serverAddress->Length - ipPortSeperatorPos - 1));

        // TODO: check player name and server address

        client_ = gcnew Client(this, ip, port);
        _ballContainer.SetClient(client_);
        try
        {
            client_->Connect();
        }
        catch (SocketException^ ex)
        {
            MessageBox::Show(String::Format("Can't connect to the server: {0}\n\nError message: {1}", serverAddress, ex->Message));
            return;
        }
        // disables file->connect
        SetToolStripMenuItemEnabled(connectToServerToolStripMenuItem, false);
        hadOpponent_ = false;

        client_->BeginRead();

        Protocol::ClientMessages::MessageHello msgHello;
        msgHello.SetPlayerName(playerName_);
        client_->Write(msgHello.Serialize());
    }
}

void MainForm::OnConnectionClosed()
{
    MessageBox::Show("Connection to the server is closed!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);

    // enables file->connect, thread-safe
    SetToolStripMenuItemEnabled(connectToServerToolStripMenuItem, true);

    SetControlText(labelConnectionInfo, CONNECTION_INFO_NOT_CONNECTED);
    SetControlText(labelPlayerName, NO_PLAYER_NAME);
    SetControlText(labelOpponentName, NO_PLAYER_NAME);

    // title
    Text = PROJECT_TITLE + String::Format(" - not connected", playerName_);

    // TODO: reconnect
}

void MainForm::OnServerIsFull()
{
    MessageBox::Show("Server is full!");
    SetToolStripMenuItemEnabled(connectToServerToolStripMenuItem, true);
    // TODO: reconnect
}

void MainForm::OnConnectedToServer()
{
    // display client is connected and where
    SetControlText(labelConnectionInfo, String::Format(CONNECTION_INFO_CONNECT, client_->GetServerEndPoint()->Address, client_->GetServerEndPoint()->Port));
    // displays our nickname
    SetControlText(labelPlayerName, playerName_);
    // changes the form's title
    Text = PROJECT_TITLE + String::Format(" - connected as {0}", playerName_);
}

void MainForm::SetToolStripMenuItemEnabled(ToolStripMenuItem^ menuItem, bool enabled)
{
    ToolStripDropDownMenu^ menu = (ToolStripDropDownMenu^)menuItem->Owner;
    if (menu->InvokeRequired)
    {
        SetToolStripMenuItemEnabledDelegate^ d = gcnew SetToolStripMenuItemEnabledDelegate(this, &MainForm::SetToolStripMenuItemEnabled);
        Invoke(d, gcnew array<Object^>{menuItem, enabled});
    }
    else
        menuItem->Enabled = enabled;
}

void MainForm::SetControlText(Control^ control, String^ text)
{
    if (control->InvokeRequired)
    {
        SetControlTextDelegate^ d = gcnew SetControlTextDelegate(this, &MainForm::SetControlText);
        Invoke(d, gcnew array<Object^>{control, text});
    }
    else
        control->Text = text;
}

void MainForm::OnRecvOpponentName(String^ opponentName)
{
    // no opponent
    if (opponentName == "false")
    {
        if (hadOpponent_)
            SetControlText(labelOpponentName, DC_OPPONENT);
        else
            SetControlText(labelOpponentName, NO_OPPONENT);
    }
    else
    {
        hadOpponent_ = true;
        SetControlText(labelOpponentName, opponentName);
    }
}

void MainForm::OnRecvBalls(List<int /* ballCount */>^ _ballCountInColumns)
{
    _ballContainer.ClearBalls();
    int columnCount = _ballCountInColumns->Count;
    for (int column = 0; column < columnCount; ++column)
    {
        for (int ballCount = _ballCountInColumns[column]; ballCount > 0; --ballCount)
        {
            Ball^ ball = gcnew Ball(column,
                                    column * 200 + 100,
                                    MAIN_FORM_HEIGHT - Ball::FirstBallDummySpace - (ballCount * (Ball::BallRadius + Ball::BallBorderWidth + Ball::BallDummySpace)));
            _ballContainer.AddBall(ball);
        }
    }
}

}
}
