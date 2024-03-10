#include <QApplication>
#include <QtDebug>
#include <QMainWindow>
#include <QPushButton>

#include "MainWindow.h"
#include <QDateTime>
#include<chrono>

int main(int argc, char *argv[])
{

//    static unsigned long snFrameCount = 0;
//    static unsigned long snprevFrameCount = 0;
//    static uint64_t  diff_ms=9999;//时间差
//    static double fps=0;
//    std::chrono::high_resolution_clock::time_point prevRealTime;
//    prevRealTime=std::chrono::high_resolution_clock::now();
//    //    auto prevRealTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
//    while(true)
//    {


//        auto curRealTime = std::chrono::high_resolution_clock::now();
//        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
//                    curRealTime-prevRealTime
//                    );
//        diff_ms= diff.count();
//        if(diff_ms>=1000)
//        {

//            fps= (snFrameCount-snprevFrameCount)*1000.0/diff_ms;
//            snprevFrameCount=snFrameCount;
//            diff_ms=0;
//            prevRealTime=std::chrono::high_resolution_clock::now();
//        }
//        snFrameCount++;

//    }


    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
