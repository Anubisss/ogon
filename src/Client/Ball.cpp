#include "Ball.h"
#include "BallContainer.h"

namespace onlinegameofnim
{
namespace Client
{

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

Ball::Ball(unsigned int column, int x, int y)
{
    _column = column;
    _ballContainer = nullptr;

    BorderWidth = BallBorderWidth;
    Cursor = System::Windows::Forms::Cursors::Hand;
    FillColor = System::Drawing::Color::Red;
    FillGradientStyle = Microsoft::VisualBasic::PowerPacks::FillGradientStyle::BackwardDiagonal;
    FillStyle = Microsoft::VisualBasic::PowerPacks::FillStyle::Solid;
    Location = System::Drawing::Point(x, y);
    SelectionColor = System::Drawing::Color::White;
    Size = System::Drawing::Size(BallRadius, BallRadius);
    Click += gcnew System::EventHandler(this, &Ball::OnClick);
    MouseEnter += gcnew System::EventHandler(this, &Ball::OnMouseEnter);
    MouseLeave += gcnew System::EventHandler(this, &Ball::OnMouseLeave);
}

System::Void Ball::OnMouseEnter(System::Object^ sender, System::EventArgs^ e)
{
    MarkSelected();
    _ballContainer->BallOnMouseEnter(this);
}

System::Void Ball::OnMouseLeave(System::Object^ sender, System::EventArgs^ e)
{
    MarkUnselected();
    _ballContainer->BallOnMouseLeave(this);
}

System::Void Ball::OnClick(System::Object^ sender, System::EventArgs^ e)
{
    _ballContainer->BallOnClick(this);
}

}
}
