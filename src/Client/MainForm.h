#pragma once

#include "BallContainer.h"
#include "ConnectToServerDialog.h"
#include "Client.h"

namespace onlinegameofnim {
namespace Client {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    #define CONNECTION_INFO_NOT_CONNECTED   "Connection info: not connected"
    #define CONNECTION_INFO_CONNECT         "Connection info: connected to {0}:{1}"

    #define NO_PLAYER_NAME                  ""
    #define NO_OPPONENT                     "Opponent doesn't connected"
    #define DC_OPPONENT                     "Your opponent has been disconnected"

    #define MAIN_FORM_WIDTH                 800
    #define MAIN_FORM_HEIGHT                600

    #define PROJECT_TITLE                   "Online Game of Nim"

    /// <summary>
    /// Summary for MainForm
    /// </summary>
    public ref class MainForm : public System::Windows::Forms::Form
    {
    private: System::Windows::Forms::Label^  labelConnectionInfo;
    private: System::Windows::Forms::Label^  labelPlayerName;
    private: System::Windows::Forms::Label^  labelOpponentName;

    public:
        MainForm(void)
        {
            InitializeComponent();
            // custom size
            ClientSize = System::Drawing::Size(MAIN_FORM_WIDTH, MAIN_FORM_HEIGHT);
            SetControlText(labelConnectionInfo, CONNECTION_INFO_NOT_CONNECTED);
            SetControlText(labelPlayerName, NO_PLAYER_NAME);
            SetControlText(labelOpponentName, NO_PLAYER_NAME);
            Controls->Add(_ballContainer.GetContainer());
        }

        void OnConnectionClosed();
        void OnServerIsFull();
        void OnConnectedToServer();
        void OnRecvOpponentName(String^ opponentName);
        void OnRecvBalls(List<int>^ _ballCountInColumns);

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        // a thread-safe way (windows forms control) to change a ToolStripMenuItem's Enabled property
        void SetToolStripMenuItemEnabled(ToolStripMenuItem^ menuItem, bool enabled);
        delegate void SetToolStripMenuItemEnabledDelegate(ToolStripMenuItem^ menuItem, bool enabled);

        // thread-safe mode to set the Text property of a Control type
        void SetControlText(Control^ control, String^ text);
        delegate void SetControlTextDelegate(Control^ control, String^ text);

        Client^ client_;

        bool hadOpponent_;

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;
    private: System::Windows::Forms::MenuStrip^  menuStrip1;
    private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  quitToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  connectToServerToolStripMenuItem;

             BallContainer _ballContainer;

             String^ playerName_;

#pragma region Windows Form Designer generated code
             /// <summary>
             /// Required method for Designer support - do not modify
             /// the contents of this method with the code editor.
             /// </summary>
             void InitializeComponent(void)
             {
                 this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
                 this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                 this->connectToServerToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                 this->quitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
                 this->labelConnectionInfo = (gcnew System::Windows::Forms::Label());
                 this->labelPlayerName = (gcnew System::Windows::Forms::Label());
                 this->labelOpponentName = (gcnew System::Windows::Forms::Label());
                 this->menuStrip1->SuspendLayout();
                 this->SuspendLayout();
                 // 
                 // menuStrip1
                 // 
                 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
                 this->menuStrip1->Location = System::Drawing::Point(0, 0);
                 this->menuStrip1->Name = L"menuStrip1";
                 this->menuStrip1->Size = System::Drawing::Size(800, 24);
                 this->menuStrip1->TabIndex = 0;
                 this->menuStrip1->Text = L"menuStrip1";
                 // 
                 // fileToolStripMenuItem
                 // 
                 this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                     this->connectToServerToolStripMenuItem,
                         this->quitToolStripMenuItem
                 });
                 this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
                 this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
                 this->fileToolStripMenuItem->Text = L"File";
                 // 
                 // connectToServerToolStripMenuItem
                 // 
                 this->connectToServerToolStripMenuItem->Name = L"connectToServerToolStripMenuItem";
                 this->connectToServerToolStripMenuItem->Size = System::Drawing::Size(167, 22);
                 this->connectToServerToolStripMenuItem->Text = L"Connect to server";
                 this->connectToServerToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::connectToServerToolStripMenuItem_Click);
                 // 
                 // quitToolStripMenuItem
                 // 
                 this->quitToolStripMenuItem->Name = L"quitToolStripMenuItem";
                 this->quitToolStripMenuItem->Size = System::Drawing::Size(167, 22);
                 this->quitToolStripMenuItem->Text = L"Quit";
                 this->quitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::quitToolStripMenuItem_Click);
                 // 
                 // labelConnectionInfo
                 // 
                 this->labelConnectionInfo->AutoSize = true;
                 this->labelConnectionInfo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular,
                     System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238)));
                 this->labelConnectionInfo->Location = System::Drawing::Point(281, 24);
                 this->labelConnectionInfo->Name = L"labelConnectionInfo";
                 this->labelConnectionInfo->Size = System::Drawing::Size(151, 20);
                 this->labelConnectionInfo->TabIndex = 1;
                 this->labelConnectionInfo->Text = L"labelConnectionInfo";
                 // 
                 // labelPlayerName
                 // 
                 this->labelPlayerName->AutoSize = true;
                 this->labelPlayerName->Location = System::Drawing::Point(12, 79);
                 this->labelPlayerName->Name = L"labelPlayerName";
                 this->labelPlayerName->Size = System::Drawing::Size(86, 13);
                 this->labelPlayerName->TabIndex = 2;
                 this->labelPlayerName->Text = L"labelPlayerName";
                 // 
                 // labelOpponentName
                 // 
                 this->labelOpponentName->AutoSize = true;
                 this->labelOpponentName->Location = System::Drawing::Point(680, 79);
                 this->labelOpponentName->Name = L"labelOpponentName";
                 this->labelOpponentName->Size = System::Drawing::Size(104, 13);
                 this->labelOpponentName->TabIndex = 3;
                 this->labelOpponentName->Text = L"labelOpponentName";
                 this->labelOpponentName->TextChanged += gcnew System::EventHandler(this, &MainForm::labelOpponentName_TextChanged);
                 // 
                 // MainForm
                 // 
                 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
                 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
                 this->ClientSize = System::Drawing::Size(800, 600);
                 this->Controls->Add(this->labelOpponentName);
                 this->Controls->Add(this->labelPlayerName);
                 this->Controls->Add(this->labelConnectionInfo);
                 this->Controls->Add(this->menuStrip1);
                 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
                 this->MainMenuStrip = this->menuStrip1;
                 this->MaximizeBox = false;
                 this->Name = L"MainForm";
                 this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
                 this->Text = "Online Game of Nim"; // PROJECT_TITLE, looks like macros can't used here
                 this->menuStrip1->ResumeLayout(false);
                 this->menuStrip1->PerformLayout();
                 this->ResumeLayout(false);
                 this->PerformLayout();

             }
#pragma endregion

    private:
        System::Void connectToServerToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        System::Void quitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
        {
            Application::Exit();
        }

        System::Void labelOpponentName_TextChanged(System::Object^ sender, System::EventArgs^ e)
        {
            System::Windows::Forms::Label^ label = (System::Windows::Forms::Label^)sender;
            label->Location = Point(780 - label->Text->Length * 5, 79);
        }
};

}
}
