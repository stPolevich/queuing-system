#include <QtTest/QtTest>
#include <request.h>
#include <sourcedevice.h>
#include <QDebug>

class TestRequest: public QObject
{
    Q_OBJECT

private slots:
    void denialTest();
    void geNumberOfSourceTest();
    void getGenTimeTest();
    void setTimeInBufferTest();
    void setTimeInOperatorTest();
    void getTimeInBufferTest();
    void getTimeInOperatorTest();
    void getNumberOfOperatorTest();
    void setNumberOfOperatorTest();
};

void TestRequest::denialTest()
{
    //Initialize Requests
    std::vector<Request> requests;

    //Run denial()
    requests[1].denial();
    
    //Verify equal
    QVERIFY(requests[1].failure == true);
}

void TestRequest::getNumberOfSourceTest()
{
    //Initialize Requests
    Request requestest(1,2.0,3);
    
    //Verify equal
    QCOMPARE(requestest.getNumberOfSource(), 3);
}

void TestRequest::getGenTimeTest()
{
    //Initialize Requests
    Request requestest(1,2.0,3);
    
    //Verify equal
    QCOMPARE(requestest.getNumberOfSource(), 2.0);
}

void TestRequest::setTimeInBufferTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Time In Buffer
    requestest.setTimeInBuffer(10.0);
    
    //Verify equal
    QCOMPARE(requestest.timeInBuffer, 10.0);
}

void TestRequest::setTimeInOperatorTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Time In Operator
    requestest.setTimeInOperator(20.0);
    
    //Verify equal
    QCOMPARE(requestest.timeInOperator, 20.0);
}

void TestRequest::getTimeInBufferTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Time In Buffer
    requestest.setTimeInBuffer(10.0);
    
    //Verify equal
    QCOMPARE(requestest.getTimeInBuffer(), 10.0);
}

void TestRequest::getTimeInOperatorTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Time In Operator
    requestest.setTimeInOperator(20.0);
    
    //Verify equal
    QCOMPARE(requestest.getTimeInOperator(), 20.0);
}

void TestRequest::getNumberOfOperatorTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Number Of Operator
    requestest.setNumberOfOperator(5);
    
    //Verify equal
    QCOMPARE(requestest.getNumberOfOperator(), 5);
}

void TestRequest::setNumberOfOperatorTest()
{
    //Initialize Requests
    Request requestest;

    //Run set Number Of Operator
    requestest.setNumberOfOperator(5);
    
    //Verify equal
    QCOMPARE(requestest.numberOfOperator, 5);
}

//QTEST_MAIN(TestRequest)
//#include "testRequest.moc"
