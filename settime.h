#ifndef  SETTIME_H
#define  SETTIME_H
#include<string>
#include <time.h>
#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
class SetTime
{
public :
    static  SetTime*  GetInstance();
    bool setTime(char* );
    bool setSpeechTime(unsigned char* );
    std::string  getTime();
    std::string  getAccurateTime();
	std::string getAccurateTime4SameTime(struct timeval _gettv);
    std::string  getAccurateTimeFileName();
	unsigned int getAccurateTimeUsc();
	int getUTCTime();
	int getTime2Sec();
	int getHour();
	long GetSec();
	long GetSecWithValue(struct timeval _gettv);
    void loadTimeAndNum();
    void loadTimeAndCellInfo(int index);
	void SetTimer(int time);
	int GetTimer();
private:
    SetTime();
    struct  tm _tm;
    struct  timeval  _tv;
    time_t  _timep;
    FILE   *recordFile;
    FILE   *speechrecordFile;
	int m_Timer;
};

#endif
