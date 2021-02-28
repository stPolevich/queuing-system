#ifndef BUFFER_H
#define BUFFER_H
#include "request.h"
#include <memory>
#include <vector>

class Buffer
{
public:
    Buffer();
    Buffer(size_t bufSize, size_t numberSources);

    bool isFreePlace();
    bool isEmpty();
    void addRequest(Request *request);
    void replaceRequest(Request *request);
    void removeElement(Request *request);
    Request* getRequest();
    Request* getRequest(size_t i);
    size_t getCountRequests();

private:
    size_t bufSize = 0;
    size_t numberSources = 0;
    std::vector<Request*> elements;
    size_t countRequests = 0;
    size_t packageRequests = 0;
};

#endif // BUFFER_H
