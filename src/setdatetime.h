#ifndef SETDATETIME_H
#define SETDATETIME_H

#pragma once

#include<QDateTime>
#include<QTimer>

class SetDateTime
{
public:
    static QDateTime *DT;
    static QDateTime *getinstance(){
        if(nullptr == DT){
            DT = new QDateTime;
            DT->setDate(QDate::fromString("20210901","yyyyMMdd"));
            DT->setTime(QTime::fromString("070000000","hhmmsszzz"));
        }
        return DT;
    }
};

#endif // SETDATETIME_H
