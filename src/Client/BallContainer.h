#pragma once

#include "Ball.h"
#include "Client.h"

namespace onlinegameofnim
{
namespace Client
{

using namespace System::Collections::Generic;

public ref class BallContainer
{
public:
    typedef Dictionary<unsigned int /* columnIndex */, List<Ball^>^ /* ballsInColumn */> BallsDictionary;

public:
    BallContainer();

    void AddBall(Ball^ ball);

    void ClearBalls();

    Microsoft::VisualBasic::PowerPacks::ShapeContainer^ GetContainer() { return _container; }

    void BallOnMouseEnter(Ball^ ballSelected);
    void BallOnMouseLeave(Ball^ ballSelected);
    void BallOnClick(Ball^ ballSelected);

    void SetClient(Client^ client) { _client = client;}

private:
    Microsoft::VisualBasic::PowerPacks::ShapeContainer^ _container;
    BallsDictionary _balls;

    Client^ _client;
};

}
}
