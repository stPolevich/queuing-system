#ifndef OPERATINGDEVICE_H
#define OPERATINGDEVICE_H
#include "request.h"
#include <memory>

class OperatingDevice
{
public:
    OperatingDevice();
    OperatingDevice(double lambda, double a, double b, size_t operatorNumber);
    void processRequest(Request* request);
    double getTimeNextRequest();
    double actualReqGenTime = 0;
    size_t actualReqSource = 0;
    size_t actualReqNumberInSource = 0;
    size_t requestsCount = 0;

private:
    size_t operatorNumber = 0;
    double lambda = 0;
    double a = 0;
    double b = 0;
    double allTime = 0;
};

#endif // OPERATINGDEVICE_H
