#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmath"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    numberSources = ui->spinBox->value();
    bufSize = ui->spinBox_2->value();
    numberOperators = ui->spinBox_3->value();
    kmin = ui->spinBox_4->value();
    lambda = ui->doubleSpinBox->value();
    alpha = ui->doubleSpinBox_2->value();
    beta = ui->doubleSpinBox_3->value();
    stepSmo = Simulation(numberSources, numberOperators, bufSize, kmin,lambda,alpha,beta,false);
    stepSmo.autoSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ApplyButton_clicked()
{
    int newNumberSources = ui->spinBox->value();
    int newBufSize = ui->spinBox_2->value();
    int newNumberOperators = ui->spinBox_3->value();
    int newKmin = ui->spinBox_4->value();
    double newLambda = ui->doubleSpinBox->value();
    double newAlpha = ui->doubleSpinBox_2->value();
    double newBeta = ui->doubleSpinBox_3->value();


    if(newNumberOperators != numberOperators || newBufSize != bufSize || newNumberSources != numberSources || newKmin != kmin ||
            newLambda != lambda || newAlpha != alpha || newBeta != beta)
    {
        numberSources = newNumberSources;
        numberOperators = newNumberOperators;
        bufSize = newBufSize;
        kmin = newKmin;
        lambda = newLambda;
        alpha = newAlpha;
        beta = newBeta;
        stepSmo = Simulation(numberSources, numberOperators, bufSize, kmin,lambda,alpha,beta, false);
        stepSmo.autoSimulation();
        actualStep = 0;
    }
}

void MainWindow::on_autoModeButton_clicked()
{
//========================set tables================
    QStringList qsl;
    qsl.push_back("All requests");
    qsl.push_back("Req. success");
    qsl.push_back("Req. fail(chance)");
    qsl.push_back("T buffer");
    qsl.push_back("T processing");
    qsl.push_back("T system");
    qsl.push_back("Disp. buffer");
    qsl.push_back("Disp. proc.");

    ui->tableWidget->setRowCount(numberSources);//+1);//temp
    ui->tableWidget->setColumnCount(qsl.size());
    ui->tableWidget->setHorizontalHeaderLabels(qsl);
    qsl.clear();

     for (int i = 0; i < numberSources; ++i) {
        QString numSrc;
        numSrc.setNum(i+1);
        qsl.push_back("   Source " + numSrc + "  ");
    }
    ui->tableWidget->setVerticalHeaderLabels(qsl);
    qsl.clear();

    qsl.push_back("Coefficient");
    ui->tableWidget_2->setRowCount(numberOperators);
    ui->tableWidget_2->setColumnCount(qsl.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(qsl);
    qsl.clear();

    for (int i = 0; i < numberOperators; ++i) {
        QString numSrc;
        numSrc.setNum(i+1);
        qsl.push_back("Processor " + numSrc);
    }
    ui->tableWidget_2->setVerticalHeaderLabels(qsl);
    qsl.clear();

    //===================================================
    Simulation smo(numberSources, numberOperators, bufSize, kmin, lambda, alpha, beta, true);
    smo.autoSimulation();
    int realNumberSources = smo.resultsAuto.allReq.size();
    //TEEEEEMP
    double sum = 0;
    double sum2 = 0;
    double sum3 = 0;

    for (int i = 0; i < realNumberSources; i++) {
         QString str;

         str.setNum(smo.resultsAuto.allReq[i]);
         ui->tableWidget->setItem(i, 0, new QTableWidgetItem(str));

         str.setNum(smo.resultsAuto.allReq[i] - smo.resultsAuto.reqFail[i]);
         ui->tableWidget->setItem(i, 1, new QTableWidgetItem(str));

         str.setNum(smo.resultsAuto.reqFail[i]) ;
         str += " (";
         if(smo.resultsAuto.reqFail[i] != 0)
             str +=QString::number(smo.resultsAuto.reqFail[i] / smo.resultsAuto.allReq[i], 'f', 2);
         else
             str+=QString::number(0.0);
         str += ")";
         ui->tableWidget->setItem(i, 2, new QTableWidgetItem(str));

         str.setNum(smo.resultsAuto.timeInBuf[i], 'f', 6);
         ui->tableWidget->setItem(i, 3, new QTableWidgetItem(str));

         str.setNum(smo.resultsAuto.timeInProc[i], 'f', 6);
         ui->tableWidget->setItem(i, 4, new QTableWidgetItem(str));

         str.setNum(smo.resultsAuto.timeInSyst[i], 'f', 6);
         ui->tableWidget->setItem(i, 5, new QTableWidgetItem(str));

         str.setNum(std::abs(smo.resultsAuto.dispBUF[i]), 'f', 6);
         ui->tableWidget->setItem(i, 6, new QTableWidgetItem(str));

         str.setNum(std::abs(smo.resultsAuto.dispPROC[i]), 'f', 6);
         ui->tableWidget->setItem(i, 7, new QTableWidgetItem(str));

         //TEEEEEEMP
         /*sum += smo.resultsAuto.reqFail[i] / smo.resultsAuto.allReq[i];
         sum2 += smo.resultsAuto.timeInSyst[i];*/
   }


     for (int i = 0; i < numberOperators; ++i) {
        QString str;
        str.setNum(smo.resultsAuto.usingCoef[i], 'f', 6);
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(str));
        //teeeeemp
        sum3 += smo.resultsAuto.usingCoef[i];
    }
     //TEEEEEEMP
    /* QString str;
     sum /= realNumberSources;
     sum2 /= realNumberSources;
     sum3 /= numberOperators;
     str.setNum(sum, 'f', 2);
     ui->tableWidget->setItem(realNumberSources, 2, new QTableWidgetItem(str));
     str.setNum(sum2, 'f', 6);
     ui->tableWidget->setItem(realNumberSources, 5, new QTableWidgetItem(str));
     str.setNum(sum3, 'f', 6);
     ui->tableWidget->setItem(realNumberSources, 6, new QTableWidgetItem(str));*/
}

void MainWindow::on_pushButtonForward_clicked()
{
    //========================set tables================
        //buffer
        QStringList qsl;
        qsl.push_back("State");
        qsl.push_back("Request(gen. time)");
        qsl.push_back("Source");

        ui->tableBufStep->setRowCount(bufSize);
        ui->tableBufStep->setColumnCount(qsl.size());
        ui->tableBufStep->setHorizontalHeaderLabels(qsl);
        qsl.clear();

        for (int i = 0; i < bufSize; ++i) {
            QString numSrc;
            numSrc.setNum(i+1);
            qsl.push_back("Buf " + numSrc + "  ");
        }
        ui->tableBufStep->setVerticalHeaderLabels(qsl);
        qsl.clear();

        //source
        qsl.push_back("Request №");
        qsl.push_back("Last gen. time");
        qsl.push_back("Denial count");
        ui->tableSrcStep->setRowCount(numberSources);
        ui->tableSrcStep->setColumnCount(qsl.size());
        ui->tableSrcStep->setHorizontalHeaderLabels(qsl);
        qsl.clear();

        for (int i = 0; i < numberSources; ++i) {
            QString numSrc;
            numSrc.setNum(i+1);
            qsl.push_back("Src " + numSrc);
        }
        ui->tableSrcStep->setVerticalHeaderLabels(qsl);
        qsl.clear();

        //device
        qsl.push_back("State");
        qsl.push_back("Req. №(gen. time)");
        qsl.push_back("Source");
        qsl.push_back("Release time");
        ui->tableOperatorStep->setRowCount(numberOperators);
        ui->tableOperatorStep->setColumnCount(qsl.size());
        ui->tableOperatorStep->setHorizontalHeaderLabels(qsl);
        qsl.clear();

        for (int i = 0; i < numberOperators; ++i) {
            QString numSrc;
            numSrc.setNum(i+1);
            qsl.push_back("Dev " + numSrc);
        }
        ui->tableOperatorStep->setVerticalHeaderLabels(qsl);
        qsl.clear();
     //===================================================
    if(actualStep < stepSmo.resultsStep.allSteps)
    {

        QString str2;
        str2.setNum(stepSmo.resultsStep.allSteps);
        ui->textBrowser_2->setText(str2);
        str2.setNum(actualStep + 1);
        ui->textBrowser->setText(str2);
//BUF
        for (int i = 0; i < bufSize; ++i) {
           QString str;
           if(stepSmo.resultsStep.buf[i].bufState[actualStep])
           {
               str = "Free";
               ui->tableBufStep->setItem(i, 0, new QTableWidgetItem(str));
               str = "-";
               ui->tableBufStep->setItem(i, 1, new QTableWidgetItem(str));
               ui->tableBufStep->setItem(i, 2, new QTableWidgetItem(str));
               ui->tableBufStep->setItem(i, 3, new QTableWidgetItem(str));
           }
           else
           {
               str = "Full";
               ui->tableBufStep->setItem(i, 0, new QTableWidgetItem(str));
               str.setNum(stepSmo.resultsStep.buf[i].bufRequest[actualStep]);
               str += " (";
               str +=QString::number(stepSmo.resultsStep.buf[i].bufGenTime[actualStep], 'f', 5);
               str += ")";
               ui->tableBufStep->setItem(i, 1, new QTableWidgetItem(str));
               str.setNum(stepSmo.resultsStep.buf[i].bufSource[actualStep] + 1);
               ui->tableBufStep->setItem(i, 2, new QTableWidgetItem(str));

           }
       }
//DEV
       for (int i = 0; i < numberOperators; ++i) {
           QString str;
           if(stepSmo.resultsStep.dev[i].devState[actualStep])
           {
               str = "Free";
               ui->tableOperatorStep->setItem(i, 0, new QTableWidgetItem(str));
               str = "-";
               ui->tableOperatorStep->setItem(i, 1, new QTableWidgetItem(str));
               ui->tableOperatorStep->setItem(i, 2, new QTableWidgetItem(str));
               ui->tableOperatorStep->setItem(i, 3, new QTableWidgetItem(str));
           }
           else
           {
               str = "Works";
               ui->tableOperatorStep->setItem(i, 0, new QTableWidgetItem(str));

               str.setNum(stepSmo.resultsStep.dev[i].devRequest[actualStep]);
               str += " (";
               str +=QString::number(stepSmo.resultsStep.dev[i].devGenTime[actualStep], 'f', 5);
               str += ")";
               ui->tableOperatorStep->setItem(i, 1, new QTableWidgetItem(str));

               str.setNum(stepSmo.resultsStep.dev[i].devSource[actualStep] + 1);
               ui->tableOperatorStep->setItem(i, 2, new QTableWidgetItem(str));
               str.setNum(stepSmo.resultsStep.dev[i].devReleaseTime[actualStep]);
               ui->tableOperatorStep->setItem(i, 3, new QTableWidgetItem(str));

           }
       }

//SRC
        int realNumberSources = stepSmo.resultsStep.src.size();
        for (int i = 0; i < realNumberSources; ++i) {
                   QString str;
                       str.setNum(stepSmo.resultsStep.src[i].srcRequest[actualStep]);
                       ui->tableSrcStep->setItem(i, 0, new QTableWidgetItem(str));
                       str.setNum(stepSmo.resultsStep.src[i].srcGenTime[actualStep]);
                       ui->tableSrcStep->setItem(i, 1, new QTableWidgetItem(str));
                       str.setNum(stepSmo.resultsStep.src[i].srcDenial[actualStep]);
                       ui->tableSrcStep->setItem(i, 2, new QTableWidgetItem(str));

               }
         if(realNumberSources < numberSources)
         {
             for (int i = realNumberSources; i < numberSources; ++i) {
                        QString str;
                            str = "-";
                            ui->tableSrcStep->setItem(i, 0, new QTableWidgetItem(str));
                            ui->tableSrcStep->setItem(i, 1, new QTableWidgetItem(str));
                            ui->tableSrcStep->setItem(i, 2, new QTableWidgetItem(str));
                    }
         }

        actualStep++;
    }
}

void MainWindow::on_pushButtonBackward_clicked()
{
    if(actualStep - 1 > 0)
    {
        actualStep--;


            QString str2;
            str2.setNum(stepSmo.resultsStep.allSteps);
            ui->textBrowser_2->setText(str2);
            str2.setNum(actualStep);
            ui->textBrowser->setText(str2);
    //BUF
            for (int i = 0; i < bufSize; ++i) {
               QString str;
               if(stepSmo.resultsStep.buf[i].bufState[actualStep - 1])
               {
                   str = "Free";
                   ui->tableBufStep->setItem(i, 0, new QTableWidgetItem(str));
                   str = "-";
                   ui->tableBufStep->setItem(i, 1, new QTableWidgetItem(str));
                   ui->tableBufStep->setItem(i, 2, new QTableWidgetItem(str));
                   ui->tableBufStep->setItem(i, 3, new QTableWidgetItem(str));
               }
               else
               {
                   str = "Full";
                   ui->tableBufStep->setItem(i, 0, new QTableWidgetItem(str));
                   str.setNum(stepSmo.resultsStep.buf[i].bufRequest[actualStep - 1]);
                   str += " (";
                   str +=QString::number(stepSmo.resultsStep.buf[i].bufGenTime[actualStep - 1], 'f', 5);
                   str += ")";
                   ui->tableBufStep->setItem(i, 1, new QTableWidgetItem(str));
                   str.setNum(stepSmo.resultsStep.buf[i].bufSource[actualStep - 1] + 1);
                   ui->tableBufStep->setItem(i, 2, new QTableWidgetItem(str));

               }
           }
    //DEV
           for (int i = 0; i < numberOperators; ++i) {
               QString str;
               if(stepSmo.resultsStep.dev[i].devState[actualStep - 1])
               {
                   str = "Free";
                   ui->tableOperatorStep->setItem(i, 0, new QTableWidgetItem(str));
                   str = "-";
                   ui->tableOperatorStep->setItem(i, 1, new QTableWidgetItem(str));
                   ui->tableOperatorStep->setItem(i, 2, new QTableWidgetItem(str));
                   ui->tableOperatorStep->setItem(i, 3, new QTableWidgetItem(str));
               }
               else
               {
                   str = "Works";
                   ui->tableOperatorStep->setItem(i, 0, new QTableWidgetItem(str));

                   str.setNum(stepSmo.resultsStep.dev[i].devRequest[actualStep - 1]) ;
                   str += " (";
                   str +=QString::number(stepSmo.resultsStep.dev[i].devGenTime[actualStep - 1], 'f', 5);
                   str += ")";
                   ui->tableOperatorStep->setItem(i, 1, new QTableWidgetItem(str));

                   str.setNum(stepSmo.resultsStep.dev[i].devSource[actualStep - 1] + 1);
                   ui->tableOperatorStep->setItem(i, 2, new QTableWidgetItem(str));
                   str.setNum(stepSmo.resultsStep.dev[i].devReleaseTime[actualStep - 1]);
                   ui->tableOperatorStep->setItem(i, 3, new QTableWidgetItem(str));

               }
           }

    //SRC
            int realNumberSources = stepSmo.resultsStep.src.size();
            for (int i = 0; i < realNumberSources; ++i) {
                       QString str;
                           str.setNum(stepSmo.resultsStep.src[i].srcRequest[actualStep - 1]);
                           ui->tableSrcStep->setItem(i, 0, new QTableWidgetItem(str));
                           str.setNum(stepSmo.resultsStep.src[i].srcGenTime[actualStep - 1]);
                           ui->tableSrcStep->setItem(i, 1, new QTableWidgetItem(str));
                           str.setNum(stepSmo.resultsStep.src[i].srcDenial[actualStep - 1]);
                           ui->tableSrcStep->setItem(i, 2, new QTableWidgetItem(str));

                   }
             if(realNumberSources < numberSources)
             {
                 for (int i = realNumberSources; i < numberSources; ++i) {
                            QString str;
                                str = "-";
                                ui->tableSrcStep->setItem(i, 0, new QTableWidgetItem(str));
                                ui->tableSrcStep->setItem(i, 1, new QTableWidgetItem(str));
                                ui->tableSrcStep->setItem(i, 2, new QTableWidgetItem(str));
                        }
             }
    }
}
