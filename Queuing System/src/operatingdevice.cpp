#include "operatingdevice.h"
#include <cstdlib>
#include <QTime>
#include <math.h>

OperatingDevice::OperatingDevice()
{
}

OperatingDevice::OperatingDevice(double lambda, double a, double b, size_t operatorNumber)
{
    this->lambda = lambda;
    this->operatorNumber = operatorNumber;
    this->a = a;
    this->b = b;
}

void OperatingDevice::processRequest(Request* request)
{
    if(allTime < request->getGenTime())
        allTime = request->getGenTime();

    request->setTimeInBuffer(allTime - request->getGenTime());
    double rand_time = (double)a+(double)(b-a)*(rand()%100)/100;
    request->setTimeInOperator(rand_time);
    request->setNumberOfOperator(operatorNumber);
    allTime += rand_time;
    actualReqGenTime = request->getGenTime();
    actualReqSource = request->getNumberOfSource();
    actualReqNumberInSource = request->numberInSource;
    requestsCount++;
}

double OperatingDevice::getTimeNextRequest()
{
    return allTime;
}
