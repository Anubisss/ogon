#pragma once

namespace onlinegameofnim
{
namespace Client
{

using namespace System::Drawing;

ref class BallContainer;

public ref class Ball : public Microsoft::VisualBasic::PowerPacks::OvalShape
{
public:
    static const unsigned int BallRadius = 40;
    static const unsigned int BallBorderWidth = 3;
    static const unsigned int FirstBallDummySpace = 10;
    static const unsigned int BallDummySpace = 1;

public:
    // some defaults
    Ball(unsigned int column, int x, int y);

    System::Void Ball::OnMouseEnter(System::Object^ sender, System::EventArgs^ e);
    System::Void Ball::OnMouseLeave(System::Object^ sender, System::EventArgs^ e);
    System::Void Ball::OnClick(System::Object^ sender, System::EventArgs^ e);

    void SetBallContainer(BallContainer^ ballContainer) { _ballContainer = ballContainer; }

    unsigned int GetColumn() { return _column; }

    void MarkSelected() { FillColor = Color::Yellow; }
    void MarkUnselected() { FillColor = Color::Red; }

private:
    BallContainer^ _ballContainer;
    unsigned int _column;
};

}
}
