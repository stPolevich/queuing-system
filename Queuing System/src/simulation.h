#ifndef SIMULATION_H
#define SIMULATION_H
#include "buffer.h"
#include "request.h"
#include "sourcedevice.h"
#include "operatingdevice.h"
#include <vector>

struct autoResults
{
    std::vector<double> allReq;
    std::vector<double> reqFail;
    std::vector<double> timeInSyst;
    std::vector<double> timeInBuf;
    std::vector<double> timeInProc;
    std::vector<double> dispBUF;
    std::vector<double> dispPROC;
    std::vector<double> usingCoef;
};
struct bufStepStat
{
    std::vector <bool> bufState;
    std::vector <size_t> bufRequest;
    std::vector <double> bufGenTime;
    std::vector <size_t> bufSource;
};
struct srcStepStat
{
    std::vector <size_t> srcRequest;
    std::vector <double> srcGenTime;
    std::vector <size_t> srcDenial;
};
struct devStepStat
{
    std::vector <bool> devState;
    std::vector <size_t> devRequest;
    std::vector <double> devGenTime;
    std::vector <size_t> devSource;
    std::vector <double> devReleaseTime;
};
struct stepResults
{
    std::vector <bufStepStat> buf;
    std::vector <srcStepStat> src;
    std::vector <devStepStat> dev;
    size_t allSteps = 0;
};

class Simulation
{
public:
    Simulation();
    Simulation(size_t numberSources, size_t numberOperators,
               size_t bufSize, int kmin, double lambda, double a, double b, bool autoMode);

    void autoSimulation();
    void calculateAutoResults();
    void calculateStepResults();
    void searchMinRequest();
    void findFreeOperator(Request* request);
    void setNextOperator();
    bool sourcesIsFree();
    void startOperator(Request* request, size_t operatorNumber);
    autoResults resultsAuto;
    stepResults resultsStep;

private:
    bool endSimulation();

    Buffer buffer;
    size_t bufSize = 0;
    std::vector<SourceDevice> sources;
    std::vector<OperatingDevice> operators;
    double sysTime = 0;
    Request* minGenRequest = nullptr;
    size_t nextOperator = 0;
    int kmin = 0;
    int ksig = 0;
    bool stopGeneration = false;
    bool autoMode = true;
};

#endif // SIMULATION_H
