#include "BallContainer.h"
#include "Ball.h"
#include "Server.h"

namespace onlinegameofnim
{
namespace Server
{

BallContainer::BallContainer(int minColumn, int maxColumn,
                             int minBallsPerColumn, int maxBallsPerColumn)
{
    _minColumn = minColumn;
    _maxColumn = maxColumn;
    _minBallsPerColumn = minBallsPerColumn;
    _maxBallsPerColumn = maxBallsPerColumn;
}

void BallContainer::GenerateBalls()
{
    Server::DebugMessage(nullptr, "----------------------------------------------------------------------");
    Server::DebugMessage(nullptr, "Generating random balls...");
    Server::DebugMessage(nullptr, String::Format("minColumn: {0} maxColumn: {1} minBallsPerColumn: {2} maxBallsPerColumn: {3}",
                                                 _minColumn, _maxColumn, _minBallsPerColumn, _maxBallsPerColumn));

    Random^ r = gcnew Random();
    int columnsCount = r->Next(_minColumn, _maxColumn + 1);

    for (int i = 0; i < columnsCount; ++i)
    {
        List<Ball^>^ ballList = gcnew List<Ball^>();
        _balls.Add(i, ballList);

        int ballsCount = r->Next(_minBallsPerColumn, _maxBallsPerColumn + 1);
        for (int j = 0; j < ballsCount; ++j)
        {
            Ball^ ball = gcnew Ball();
            ballList->Add(ball);
        }
    }

    PrintBalls();

    Server::DebugMessage(nullptr, "----------------------------------------------------------------------");
}

void BallContainer::RemoveBall(int columnIndex, int ballIndex)
{
    List<Ball^>^ ballsInColumn = _balls[columnIndex];
    ballsInColumn->RemoveRange(0, ballIndex + 1);
    /* SHOULDN'T NEED MAYBE
    if (ballsInColumn->Count == 0)
        _balls.Remove(columnIndex);
    */
}

void BallContainer::PrintBalls()
{
    for (int i = 0; i < _balls.Keys->Count; ++i)
        Server::DebugMessage(nullptr, String::Format("{0}. column has {1} balls", i, _balls[i]->Count));
}

}
}
