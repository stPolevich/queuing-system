#include "buffer.h"
Buffer::Buffer()
{
}

Buffer::Buffer(size_t bufSize, size_t numberSources)
{
    this->bufSize = bufSize;
    this->numberSources = numberSources;
    elements.resize(bufSize);
    for(size_t i = 0; i < bufSize; i++)
    {
        elements[i] = nullptr;
    }
}

bool Buffer::isFreePlace()
{
    for(size_t i = 0; i < bufSize; i++)
    {
        if(elements[i] == nullptr)
        {
            return true;
        }
    }
    return false;
}

bool Buffer::isEmpty()
{
    for(size_t i = 0; i < bufSize; i++)
        {
            if(elements[i] != nullptr)
            {
                return false;
            }
        }
    return true;
}

void Buffer::addRequest(Request* request)
{
    if(isFreePlace())
    {
        for(size_t freePlace = 0; freePlace < bufSize; freePlace++)
        {
            if(elements[freePlace] == nullptr)
            {
             elements[freePlace] = request;
             countRequests++;
             return;
            }
        }
    }
}

void Buffer::replaceRequest(Request *request)
{
    size_t oldestRequest = 0;
    for(size_t i = 0; i < bufSize; i++)
    {
        if(elements[i]->getGenTime() < elements[oldestRequest]->getGenTime())
        {
            oldestRequest = i;
        }
    }
    elements[oldestRequest]->denial();
    elements[oldestRequest] = nullptr;
    addRequest(request);
}

void Buffer::removeElement(Request *request)
{
    for(size_t i = 0; i < bufSize; i++)
        if(elements[i] == request)
        {
            elements[i] = nullptr;
            return;
        }
}

Request* Buffer::getRequest()
{
    if(!isEmpty())
    {
        for(size_t i = 0; i < numberSources; i++)
        {
            for(size_t j = 0; j < bufSize; j++)
            {
                if(elements[j] != nullptr)
                {
                    if(elements[j]->getNumberOfSource() == packageRequests)
                    {
                        return elements[j];
                    }
                }
            }
            packageRequests++;
            if(packageRequests == numberSources)
            {
                packageRequests = 0;
            }
        }
    }
    return nullptr;
}


Request* Buffer::getRequest(size_t i)
{
    if(i < elements.size())
    {
         return elements[i];
    }
    else
        return nullptr;
}

size_t Buffer::getCountRequests()
{
    return countRequests;
}
