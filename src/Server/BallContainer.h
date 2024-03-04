#pragma once

namespace onlinegameofnim
{
namespace Server
{

using namespace System::Collections::Generic;

ref class Ball;

public ref class BallContainer
{
public:
    typedef Dictionary<unsigned int, List<Ball^>^> BallsDictionary;

public:
    BallContainer(int minColumn, int maxColumn,
                  int minBallsPerColumn, int maxBallsPerColumn);

    void GenerateBalls();

    void RemoveBall(int columnIndex, int ballIndex);

    void PrintBalls();

    int GetColumnCount() { return _balls.Keys->Count; }
    int GetBallCount(int columnIndex) { return  _balls[columnIndex]->Count; }

private:
    BallsDictionary _balls;

    int _minColumn, _maxColumn;
    int _minBallsPerColumn, _maxBallsPerColumn;
};

}
}
