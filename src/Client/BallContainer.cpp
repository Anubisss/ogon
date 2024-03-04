#include "BallContainer.h"
#include "../Server/Protocol.h"

namespace onlinegameofnim
{
namespace Client
{

BallContainer::BallContainer() : _client(nullptr)
{
    _container = gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer();
    _container->Location = System::Drawing::Point(0, 0);
    _container->Margin = System::Windows::Forms::Padding(0);
    _container->Size = System::Drawing::Size(1136, 527);
    _container->TabIndex = 0;
    _container->TabStop = false;
}

void BallContainer::AddBall(Ball^ ball)
{
    ball->SetBallContainer(this);
    // adds the ball to the GUI
    _container->Shapes->Add(ball);
    _container->Refresh();

    unsigned int column = ball->GetColumn();
    // the list which stores the balls for the specific column
    List<Ball^>^ ballColumnList = nullptr;
    // no key yet
    if (!_balls.ContainsKey(column))
    {
        ballColumnList = gcnew List<Ball^>();
        _balls.Add(column, ballColumnList);
    }
    // key already exists so let's get it
    else
        ballColumnList = _balls[column];
    // adds the ball to the specific column (key)
    ballColumnList->Add(ball);
}

void BallContainer::ClearBalls()
{
    for each(KeyValuePair<unsigned int /* columnIndex */, List<Ball^>^ /* ballsInColumn */> pair in _balls)
    {
        pair.Value->Clear();
    }
    _balls.Clear();
    _container->Shapes->Clear();
    _container->Refresh();
}

void BallContainer::BallOnMouseEnter(Ball^ ballSelected)
{
    // if a ball selected all balls above in the same column should be selected also
    unsigned int column = ballSelected->GetColumn();
    List<Ball^>^ ballsInColumn = _balls[column];
    for each (Ball^ ball in ballsInColumn)
    {
        ball->MarkSelected();
        if (ball == ballSelected)
            break;
    }
}

void BallContainer::BallOnMouseLeave(Ball^ ballSelected)
{
    // this should be OK and it's easier in that way
    // if the cursor leaves a ball just mark all the balls as unselected in the same column
    unsigned int column = ballSelected->GetColumn();
    List<Ball^>^ ballsInColumn = _balls[column];
    for each (Ball^ ball in ballsInColumn)
        ball->MarkUnselected();
}

void BallContainer::BallOnClick(Ball^ ballSelected)
{
    // if clicked on a ball this removes the ball itself
    // and all the balls which are above in the same column
    unsigned int column = ballSelected->GetColumn();
    int ballIndex = -1;
    for each (Ball^ ball in _balls[column])
    {
        ++ballIndex;
        if (ball == ballSelected)
        {
            break;
        }
    }

    Protocol::ClientMessages::MessageRemoveBall msg;
    msg.SetColumn(column);
    msg.SetBall(ballIndex);
    _client->Write(msg.Serialize());
}

}
}
