#include <QtTest/QtTest>
#include <sourcedevice.h>
#include <request.h>
#include <QDebug>

class TestSourceDevice: public QObject
{
    Q_OBJECT

private slots:
    void generateRequestTest();
    void getRequestTest();
    void getCountRequestTest();

};

void TestSourceDevice::generateRequestTest()
{
    //Initialize Source Device
    std::vector<SourceDevice> sources;

    //Generate Request
    sources[1].generateRequest();

    //Verify equal
    QCOMPARE(sources[1].countRequests, 1);
}

void TestSourceDevice::getRequestTest()
{
    //Initialize Source Device
    std::vector<SourceDevice> sources;

    //Generate Request
    sources[1].generateRequest();
    
    //Verify equal
    QVERIFY(sources[1].haveActualRequest == true);
    QCOMPARE(getRequest(1), getMinRequest());
}

void TestSourceDevice::getCountRequestTest()
{
    //Initialize Source Device
    std::vector<SourceDevice> sources;

    //Generate Request
    sources[1].generateRequest();
    sources[1].generateRequest();

    //Verify equal
    QCOMPARE(sources[1].getCountRequests(), 2);
}


//QTEST_MAIN(TestSourceDevice)
//#include "testSourceDevice.moc"
