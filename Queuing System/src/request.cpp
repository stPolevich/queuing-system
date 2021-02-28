#include "request.h"

Request::Request()
{

}

Request::Request(size_t numberOfSource, double startTime, size_t numberInSource)
{
    this->numberOfSource = numberOfSource;
    this->startTime = startTime;
    this->numberInSource = numberInSource;
}

void Request::denial()
{
    failure = true;
}

size_t Request::getNumberOfSource()
{
    return numberOfSource;
}

double Request::getGenTime()
{
    return startTime;
}

void Request::setTimeInBuffer(double time)
{
    timeInBuffer = time;
}

void Request::setTimeInOperator(double time)
{
    timeInOperator = time;
}

double Request::getTimeInBuffer()
{
    return timeInBuffer;
}

double Request::getTimeInOperator()
{
    return timeInOperator;
}

size_t Request::getNumberOfOperator()
{
    return numberOfOperator;
}

void Request::setNumberOfOperator(size_t numberOfOperator)
{
    this->numberOfOperator = numberOfOperator;
}
