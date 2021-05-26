#include "simulation.h"
#include <cmath>

Simulation::Simulation()
{
}

Simulation::Simulation(size_t numberSources, size_t numberOperators,
           size_t bufSize, int kmin, double lambda, double a, double b, bool autoMode)
{
    if(numberSources > kmin)
        numberSources = kmin;

    this->buffer = Buffer(bufSize, numberSources);
    this->kmin = kmin;
    this->bufSize = bufSize;
    this->autoMode = autoMode;

    //generation sources
    for (size_t i = 0; i < numberSources; i++){
            sources.push_back(SourceDevice(lambda, i));
    }
    //generation operators
    for (size_t i = 0; i< numberOperators; i++){
            operators.push_back(OperatingDevice(lambda, a, b, i));
    }


    resultsAuto.allReq.resize(numberSources);
    resultsAuto.reqFail.resize(numberSources);
    resultsAuto.timeInSyst.resize(numberSources);
    resultsAuto.timeInBuf.resize(numberSources);
    resultsAuto.timeInProc.resize(numberSources);
    resultsAuto.dispBUF.resize(numberSources);
    resultsAuto.dispPROC.resize(numberSources);
    resultsAuto.usingCoef.resize(numberOperators);
    resultsStep.buf.resize(bufSize);
    resultsStep.dev.resize(numberOperators);
    resultsStep.src.resize(numberSources);

    //generation request
    for (size_t i = 0; i < sources.size(); i++){
        if(ksig != kmin)
        {
            sources[i].generateRequest();
            resultsAuto.allReq[i]++;
            ksig++;
        }
    }

    //find max time
    sysTime = sources[0].getRequest(0)->getGenTime();
    for(size_t i = 1; i < sources.size(); i++)
    {
        if(sources[i].getRequest(0)->getGenTime() > sysTime)
            sysTime = sources[i].getRequest(0)->getGenTime();
    }
    if(!autoMode)
        calculateStepResults();
}

void Simulation::autoSimulation()
{
    do{
        if(ksig == kmin)
        {
            stopGeneration = true;
        }

        if(!sourcesIsFree()){

            //search min request
            searchMinRequest();

            //add request in buffer
            if (buffer.isFreePlace()){
                buffer.addRequest(minGenRequest);
                sources[minGenRequest->getNumberOfSource()].haveActualRequest = false;
                if(!autoMode)
                    calculateStepResults();
            } else {
                buffer.replaceRequest(minGenRequest);
                sources[minGenRequest->getNumberOfSource()].haveActualRequest = false;
                sources[minGenRequest->getNumberOfSource()].denial++;
                resultsAuto.reqFail[minGenRequest->getNumberOfSource()]++;
                if(!autoMode)
                    calculateStepResults();
            }
        }

        //process request in operating device
        findFreeOperator(buffer.getRequest());

        //generate new request
        if(!stopGeneration){
            sources[minGenRequest->getNumberOfSource()].generateRequest();
            if(sources[minGenRequest->getNumberOfSource()].getMinRequest()->getGenTime() > sysTime)
                sysTime = sources[minGenRequest->getNumberOfSource()].getMinRequest()->getGenTime();
            resultsAuto.allReq[minGenRequest->getNumberOfSource()]++;
            ksig++;
        }

    if(!autoMode)
        calculateStepResults();
    }while(!endSimulation());

        calculateAutoResults();
}

void Simulation::searchMinRequest()
{
    for(size_t i = 0; i < sources.size(); i++)
    {
        if(sources[i].haveActualRequest)
        {
           minGenRequest = sources[i].getMinRequest();
           break;
        }
    }

    for(size_t i = 1; i < sources.size(); i++)
    {
        if(sources[i].haveActualRequest)
           if(sources[i].getMinRequest()->getGenTime() < minGenRequest->getGenTime())
            {
                minGenRequest = sources[i].getMinRequest();
            }
    }
}

void Simulation::setNextOperator()
{
    nextOperator++;

    if(nextOperator == operators.size())
    {
        nextOperator = 0;
    }
}

void Simulation::findFreeOperator(Request *request)
{
    if(request != nullptr)
        for (size_t i = nextOperator; i < operators.size(); i++)
        {
            if(sourcesIsFree())
            {
                startOperator(request,i);
                return;
            }
            else
            {
                if(operators[i].getTimeNextRequest() <= sysTime)
                {
                    startOperator(request,i);
                    return;
                }
            }

        }

        if(nextOperator != 0)
            for(size_t i = 0; i < nextOperator; i++)
            {
                if(sourcesIsFree())
                {
                     startOperator(request,i);
                     return;
                }
                else
                {
                    if(operators[i].getTimeNextRequest() <= sysTime)
                    {
                        startOperator(request,i);
                        return;
                    }
                }
            }
}

void Simulation::startOperator(Request* request, size_t operatorNumber)
{
    operators[operatorNumber].processRequest(request);
    setNextOperator();
    buffer.removeElement(request);
}

bool Simulation::sourcesIsFree()
{
    for(size_t i = 0; i < sources.size(); i++)
    {
        if(sources[i].haveActualRequest)
            return false;
    }
    return true;
}

//bool Simulation::endSimulation()
//{
//    if (ksig == kmin && buffer.isEmpty() && stopGeneration && sourcesIsFree())
//    {
//        return true;
//    }
//    return false;
//}

bool Simulation::endSimulation()
{
    if (ksig == kmin && buffer.isEmpty() && stopGeneration && sourcesIsFree())
    {
        //Для зацикливания выполнения автоматического мода устанавливаем флажок endSimulation всегда в состояние false
        return false;
    }
    return false;
}

void Simulation::calculateAutoResults()
{
    //calculate average time & dispersion in buffer
    for(size_t i = 0; i < sources.size(); i++)
    {
        resultsAuto.timeInBuf[i] = 0;
        //double timeInBuf2 = 0;
        double numReqInSystem = resultsAuto.allReq[i] - resultsAuto.reqFail[i];
        for(size_t j = 0; j < sources[i].getCountRequests(); j++)
            resultsAuto.timeInBuf[i] +=  sources[i].getRequest(j)->getTimeInBuffer();

        resultsAuto.timeInBuf[i] /= numReqInSystem;
        //timeInBuf2 = resultsAuto.timeInBuf[i] / numReqInSystem;

        //Dispersion
        for(size_t j = 0; j < sources[i].getCountRequests(); j++)
            resultsAuto.dispBUF[i] += pow(sources[i].getRequest(j)->getTimeInBuffer() - resultsAuto.timeInBuf[i], 2);
        resultsAuto.dispBUF[i] /= numReqInSystem;
    }

    //calculate average time & dispersion in processor
    for(size_t i = 0; i < sources.size(); i++)
    {
        resultsAuto.timeInProc[i] = 0;
        double numReqInSystem = resultsAuto.allReq[i] - resultsAuto.reqFail[i];
        for(size_t j = 0; j < sources[i].getCountRequests(); j++)
            resultsAuto.timeInProc[i] += sources[i].getRequest(j)->getTimeInOperator();
        resultsAuto.timeInProc[i] = resultsAuto.timeInProc[i] / numReqInSystem;

        //Dispersion
        for(size_t j = 0; j < sources[i].getCountRequests(); j++)
            resultsAuto.dispPROC[i] += pow(sources[i].getRequest(j)->getTimeInOperator() - resultsAuto.timeInProc[i], 2);
        resultsAuto.dispPROC[i] /= numReqInSystem;
    }

    //calculate average time in system
    for(size_t i = 0; i < sources.size(); i++)
    {
        resultsAuto.timeInSyst[i] = resultsAuto.timeInBuf[i] + resultsAuto.timeInProc[i];
    }

    //calculate operator using coef.
    for(size_t i = 0; i < sources.size(); i++)
    {
        for(size_t j = 0; j < sources[i].getCountRequests(); j++)
        {
            resultsAuto.usingCoef[sources[i].getRequest(j)->getNumberOfOperator()] +=
                    sources[i].getRequest(j)->getTimeInOperator();
        }
    }
    for(size_t i = 0; i < operators.size(); i++)
    {
        resultsAuto.usingCoef[i] = resultsAuto.usingCoef[i] / operators[i].getTimeNextRequest();
    }
}

void Simulation::calculateStepResults()
{
    resultsStep.allSteps++;
//========Buf
    for(size_t i = 0; i < bufSize; i++)
    {

        if(buffer.getRequest(i) != nullptr)
        {
            resultsStep.buf[i].bufState.push_back(false);
            resultsStep.buf[i].bufGenTime.push_back(buffer.getRequest(i)->getGenTime());
            resultsStep.buf[i].bufRequest.push_back(buffer.getRequest(i)->numberInSource);
            resultsStep.buf[i].bufSource.push_back(buffer.getRequest(i)->getNumberOfSource());
        }
        else
        {
            resultsStep.buf[i].bufState.push_back(true);
            resultsStep.buf[i].bufRequest.push_back(0);
            resultsStep.buf[i].bufGenTime.push_back(0);
            resultsStep.buf[i].bufSource.push_back(0);
        }
    }
//========Dev
    for(size_t i = 0; i < operators.size(); i++)
    {

        if(operators[i].getTimeNextRequest() != 0.0)
        {
            resultsStep.dev[i].devState.push_back(false);
            resultsStep.dev[i].devRequest.push_back(operators[i].actualReqNumberInSource);
            resultsStep.dev[i].devGenTime.push_back(operators[i].actualReqGenTime);
            resultsStep.dev[i].devSource.push_back(operators[i].actualReqSource);
            resultsStep.dev[i].devReleaseTime.push_back(operators[i].getTimeNextRequest());
        }
        else
        {
            resultsStep.dev[i].devState.push_back(true);
            resultsStep.dev[i].devRequest.push_back(0);
            resultsStep.dev[i].devGenTime.push_back(0);
            resultsStep.dev[i].devSource.push_back(0);
            resultsStep.dev[i].devReleaseTime.push_back(0);
        }
    }
//========Src
    for(size_t i = 0; i < sources.size(); i++)
    {
            resultsStep.src[i].srcRequest.push_back(sources[i].getCountRequests());
            resultsStep.src[i].srcGenTime.push_back(sources[i].getMinRequest()->getGenTime());
            resultsStep.src[i].srcDenial.push_back(sources[i].denial);
    }
}
