#include "sourcedevice.h"
#include <cstdlib>
#include <QTime>
#include <math.h>

SourceDevice::SourceDevice()
{
}

SourceDevice::SourceDevice(double lambda, size_t sourceNumber)
{
    this->lambda = lambda;
    this->sourceNumber = sourceNumber;
}

void SourceDevice::generateRequest()
{
    lastTimeOfGen += (-1/lambda)*log((double)rand()/RAND_MAX);
    countRequests++;
    requests.push_back(Request(sourceNumber, lastTimeOfGen, countRequests));
    nextRequest++;
    haveActualRequest = true;
}

Request* SourceDevice::getRequest(size_t requestNumber)
{
    return &requests[requestNumber];
}

Request* SourceDevice::getMinRequest()
{
    return &requests[nextRequest-1];
}

int SourceDevice::getCountRequests()
{
    return countRequests;
}
