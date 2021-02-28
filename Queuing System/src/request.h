#ifndef REQUEST_H
#define REQUEST_H
#include <memory>

class Request
{
public:
    Request();
    Request(size_t numberOfSource, double startTime, size_t numberInSource);

    void denial();
    size_t getNumberOfSource();
    size_t getNumberOfOperator();
    double getGenTime();
    double getTimeInBuffer();
    double getTimeInOperator();
    void setTimeInBuffer(double time);
    void setTimeInOperator(double time);
    void setNumberOfOperator(size_t numberOfOperator);
    size_t numberInSource = 0;

private:
    size_t numberOfSource = 0;
    size_t numberOfOperator = 0;
    double startTime = 0;
    double timeInBuffer = 0;
    double timeInOperator = 0;
    bool failure = false;
};

#endif // REQUEST_H
