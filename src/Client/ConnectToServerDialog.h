#pragma once

namespace onlinegameofnim {
namespace Client {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for ConnectToServerDialog
    /// </summary>
    public ref class ConnectToServerDialog : public System::Windows::Forms::Form
    {
    public:
        ConnectToServerDialog(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
        }

        String^ GetServerAddress() { return textBoxServerAddress->Text; }
        String^ GetPlayerName() { return textBoxPlayerName->Text; }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~ConnectToServerDialog()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::Button^  buttonConnect;
    protected:
    private: System::Windows::Forms::Button^  buttonCancel;
    private: System::Windows::Forms::Label^  labelServerAddress;

    private: System::Windows::Forms::Label^  labelPlayerName;
    private: System::Windows::Forms::TextBox^  textBoxServerAddress;



    private: System::Windows::Forms::TextBox^  textBoxPlayerName;




    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->buttonConnect = (gcnew System::Windows::Forms::Button());
            this->buttonCancel = (gcnew System::Windows::Forms::Button());
            this->labelServerAddress = (gcnew System::Windows::Forms::Label());
            this->labelPlayerName = (gcnew System::Windows::Forms::Label());
            this->textBoxServerAddress = (gcnew System::Windows::Forms::TextBox());
            this->textBoxPlayerName = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // buttonConnect
            // 
            this->buttonConnect->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->buttonConnect->Location = System::Drawing::Point(309, 34);
            this->buttonConnect->Name = L"buttonConnect";
            this->buttonConnect->Size = System::Drawing::Size(75, 23);
            this->buttonConnect->TabIndex = 0;
            this->buttonConnect->Text = L"Connect";
            this->buttonConnect->UseVisualStyleBackColor = true;
            // 
            // buttonCancel
            // 
            this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->buttonCancel->Location = System::Drawing::Point(309, 87);
            this->buttonCancel->Name = L"buttonCancel";
            this->buttonCancel->Size = System::Drawing::Size(75, 23);
            this->buttonCancel->TabIndex = 1;
            this->buttonCancel->Text = L"Cancel";
            this->buttonCancel->UseVisualStyleBackColor = true;
            // 
            // labelServerAddress
            // 
            this->labelServerAddress->AutoSize = true;
            this->labelServerAddress->Location = System::Drawing::Point(34, 39);
            this->labelServerAddress->Name = L"labelServerAddress";
            this->labelServerAddress->Size = System::Drawing::Size(81, 13);
            this->labelServerAddress->TabIndex = 2;
            this->labelServerAddress->Text = L"Server address:";
            this->labelServerAddress->Click += gcnew System::EventHandler(this, &ConnectToServerDialog::labelServerAddress_Click);
            // 
            // labelPlayerName
            // 
            this->labelPlayerName->AutoSize = true;
            this->labelPlayerName->Location = System::Drawing::Point(34, 87);
            this->labelPlayerName->Name = L"labelPlayerName";
            this->labelPlayerName->Size = System::Drawing::Size(68, 13);
            this->labelPlayerName->TabIndex = 3;
            this->labelPlayerName->Text = L"Player name:";
            this->labelPlayerName->Click += gcnew System::EventHandler(this, &ConnectToServerDialog::labelPlayerName_Click);
            // 
            // textBoxServerAddress
            // 
            this->textBoxServerAddress->Location = System::Drawing::Point(135, 37);
            this->textBoxServerAddress->Name = L"textBoxServerAddress";
            this->textBoxServerAddress->Size = System::Drawing::Size(141, 20);
            this->textBoxServerAddress->TabIndex = 4;
            // 
            // textBoxPlayerName
            // 
            this->textBoxPlayerName->Location = System::Drawing::Point(135, 87);
            this->textBoxPlayerName->Name = L"textBoxPlayerName";
            this->textBoxPlayerName->Size = System::Drawing::Size(141, 20);
            this->textBoxPlayerName->TabIndex = 5;
            // 
            // ConnectToServerDialog
            // 
            this->AcceptButton = this->buttonConnect;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->CancelButton = this->buttonCancel;
            this->ClientSize = System::Drawing::Size(440, 150);
            this->ControlBox = false;
            this->Controls->Add(this->textBoxPlayerName);
            this->Controls->Add(this->textBoxServerAddress);
            this->Controls->Add(this->labelPlayerName);
            this->Controls->Add(this->labelServerAddress);
            this->Controls->Add(this->buttonCancel);
            this->Controls->Add(this->buttonConnect);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"ConnectToServerDialog";
            this->ShowIcon = false;
            this->ShowInTaskbar = false;
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"Connect to server";
            this->TopMost = true;
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        System::Void labelServerAddress_Click(System::Object^ sender, System::EventArgs^ e)
        {
            textBoxServerAddress->Focus();
        }
        System::Void labelPlayerName_Click(System::Object^sender, System::EventArgs^ e)
        {
            textBoxPlayerName->Focus();
        }
    };
}
}
