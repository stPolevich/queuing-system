#include <QtTest/QtTest>
#include <buffer.h>
#include <request.h>
#include <QDebug>

class TestBuffer: public QObject
{
    Q_OBJECT

private slots:
    void isFreePlaceTest();
    void isEmptyTest();
    void addRequesTest();
    void replaceRequestTest();
    void removeElementTest();
    void getRequestTest();
    void getCountRequestsTest();
};

void TestBuffer::isFreePlaceTest()
{
    //Initialize Buffer
    Buffer buftest;
    
    //Verify equal
    QVERIFY(buftest.isFreePlace() == true);
}

void TestBuffer::isEmptyTest()
{
    //Initialize Buffer
    Buffer buftest;
    
    //Verify equal
    QVERIFY(buftest.isEmpty() == true);
}

void TestBuffer::addRequesTest()
{
    //Initialize Buffer and Request
    Buffer buftest;
    Request reqtest;

    //add request in buffer
    buftest.addRequest(reqtest);
    
    //Verify equal
    QCOMPARE(buftest.elements[1], reqtest);
    QCOMPARE(buftest.countRequests, 1);
}

void TestBuffer::replaceRequestTest()
{
    //Initialize Buffer and Request
    Buffer buftest;
    Request reqtest;
    Request reqtest2;

    //add and replace request in buffer
    buftest.addRequest(reqtest);
    buftest.replaceRequest(reqtest2);
    
    //Verify equal
    QCOMPARE(buftest.elements[1], reqtest2);
}

void TestBuffer::removeElementTest()
{
    //Initialize Buffer and Request
    Buffer buftest;
    Request reqtest;
    Request reqtest2;

    //add and remove request in buffer
    buftest.addRequest(reqtest);
    buftest.removeRequest(reqtest2);
    
    //Verify equal
    QCOMPARE(buftest.elements[1], reqtest);
    QCOMPARE(buftest.countRequests, 1);
}

void TestBuffer::getRequestTest()
{
    //Initialize Buffer and Request
    Buffer buftest;
    Request reqtest;
    Request reqtest2;

    //add request in buffer
    buftest.addRequest(reqtest);
    
    //Verify equal
    QCOMPARE(buftest.getRequest(), reqtest);
}

void TestBuffer::getCountRequestsTest()
{
    //Initialize Buffer and Request
    Buffer buftest;
    Request reqtest;
    Request reqtest2;

    //add and replace request in buffer
    buftest.addRequest(reqtest);
    buftest.addRequest(reqtest2);
    
    //Verify equal
    QCOMPARE(buftest.getCountRequests(), 2);
}

QTEST_MAIN(TestBuffer)
#include "testBuffer.moc"
