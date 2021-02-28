#ifndef SOURCEDEVICE_H
#define SOURCEDEVICE_H
#include "request.h"
#include <memory>
#include <vector>

class SourceDevice
{
public:
    SourceDevice();
    SourceDevice(double lambda, size_t sourceNumber);

    void generateRequest();
    Request* getRequest(size_t requestNumber);
    Request* getMinRequest();
    int getCountRequests();
    bool haveActualRequest = false;
    size_t denial = 0;

private:
    std::vector<Request> requests;
    size_t sourceNumber = 0;
    size_t nextRequest = 0;
    double lambda = 0;
    double lastTimeOfGen = 0;
    int countRequests = 0;
};

#endif // SOURCEDEVICE_H
