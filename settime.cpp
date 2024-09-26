#include "settime.h"
#include <string.h>
#include <string>
#include <unistd.h>

SetTime * SetTime::GetInstance()
{
    static  SetTime * _instance = NULL;
    if(_instance == NULL) {
        _instance = new SetTime();
    }
    return _instance;
}
SetTime::SetTime()
{
    //memset(&_tm,0,sizeof(_tm));
    //memset(&_tv,0,sizeof(_tm));
    //memset(&_timep,0,sizeof(_tm));
    //memset(&_now,0,sizeof(_tm));
    //memset(&_timenow,0,sizeof(_tm));
    recordFile = NULL;
    speechrecordFile = NULL;
}
bool SetTime::setTime(char *temptime)
{
    char temp[5] = {0};
    memcpy(temp, temptime, 4);
    printf("temp = %s\n", temp);
    _tm.tm_year = atoi(temp) - 1900;
    memset(temp, 0, 5);
    memcpy(temp, temptime + 4, 2);
    _tm.tm_mon = atoi(temp) - 1;
    memset(temp, 0, 5);
    memcpy(temp, temptime + 6, 2);
    _tm.tm_mday = atoi(temp);
    memset(temp, 0, 5);
    memcpy(temp, temptime + 8, 2);
    _tm.tm_hour = atoi(temp);
    memset(temp, 0, 5);
    memcpy(temp, temptime + 10, 2);
    _tm.tm_min = atoi(temp);
    _tm.tm_sec = 0;

    _timep = mktime(&_tm);
    _tv.tv_sec = _timep;
    _tv.tv_usec = 0;
    if(settimeofday(&_tv, (struct timezone*)8) < 0) {
        printf("!!!SetTime::SetTime Error\n");
        return false;
    }
    return true;
}

bool SetTime::setSpeechTime(unsigned char *temptime)
{
    _tm.tm_year = temptime[2] * 16 + temptime[3] - 1900;
    _tm.tm_mon = temptime[4] - 1;
    _tm.tm_mday = temptime[5];
    _tm.tm_hour = temptime[6];
    _tm.tm_min = temptime[7];
    _tm.tm_sec = temptime[8];

    _timep = mktime(&_tm);
    _tv.tv_sec = _timep;
    _tv.tv_usec = 0;
    if(settimeofday(&_tv, (struct timezone*)8) < 0) {
        printf("!!!SetTime::SetTime Error\n");
        return false;
    }
    return true;
}
std::string SetTime::getTime()
{
    time_t _now;
    struct tm *_timenow;
    time(&_now);
    _timenow =  localtime(&_now);
    char temp[20] = {0};
    snprintf(temp, 16, "%04d-%02d%02d-%02d_%02d", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min);
    std::string tempstring;
    tempstring.assign(temp, 0, 15);
    return tempstring;
}
int SetTime::getUTCTime()
{
    time_t _now;
    struct tm *_timenow;
    time(&_now);
    _timenow =  gmtime(&_now);
	time_t utc = mktime(_timenow);
    return (int)utc;
}
int SetTime::getTime2Sec()
{
	for(int i=0;i<30;i++){
		struct timeval _gettv;
		struct tm *_timenow;
		gettimeofday(&_gettv,NULL);
		_timenow =  localtime(&_gettv.tv_sec);
		time_t utc = mktime(_timenow);
		if(utc<0){
			sleep(1);
		}else{
			return (int)utc;
		}
	}
	return -1;
}
int SetTime::getHour()
{
	time_t _now;
	struct tm *_timenow;
	time(&_now);
	_timenow =  localtime(&_now);
	return _timenow->tm_hour;
}
long SetTime::GetSec(){
	struct timeval _gettv;
	gettimeofday(&_gettv,NULL);
	return _gettv.tv_sec;
}


long SetTime::GetSecWithValue(struct timeval _gettv) {
	return _gettv.tv_sec;
}

std::string SetTime::getAccurateTime4SameTime(struct timeval _gettv)
{
	//struct timeval _gettv;
	struct tm *_timenow;
	/*gettimeofday(&_gettv,NULL);
	_gettv.tv_usec += 4*index*4900;
	if(_gettv.tv_usec >= 1000000) {
		(_gettv.tv_sec)++;
		_gettv.tv_usec -= 1000000;
	}*/
	_timenow =  localtime(&_gettv.tv_sec);
	char temp[30] = {0};
	snprintf(temp, 24, "%04d-%02d-%02d %02d:%02d:%02d %03ld", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min, _timenow->tm_sec,_gettv.tv_usec/1000);
	std::string tempstring;
	tempstring.assign(temp, 0, 24);
	return tempstring;
}


unsigned int SetTime::getAccurateTimeUsc()
{
	struct timeval _gettv;
	struct tm *_timenow;
	gettimeofday(&_gettv,NULL);
	_timenow =  localtime(&_gettv.tv_sec);
	int uscTime = (_timenow->tm_sec * 1000000) + _gettv.tv_usec;
	//char temp[30] = {0};
	//snprintf(temp, 27, "%04d-%02d-%02d %02d:%02d:%02d %06ld", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min, _timenow->tm_sec,_gettv.tv_usec);
	//std::string tempstring;
	//tempstring.assign(temp, 0, 27);
	return uscTime;
}


std::string SetTime::getAccurateTime()
{
	struct timeval _gettv;
	struct tm *_timenow;
	gettimeofday(&_gettv,NULL);
	_timenow =  localtime(&_gettv.tv_sec);
	char temp[30] = {0};
	//snprintf(temp, 24, "%04d-%02d-%02d %02d:%02d:%02d %03ld", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min, _timenow->tm_sec,_gettv.tv_usec/1000);
	snprintf(temp, 24, "%04d-%02d-%02d %02d:%02d:%02d", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min, _timenow->tm_sec);
	std::string tempstring;
	tempstring.assign(temp, 0, 24);
	return tempstring;
}
std::string SetTime::getAccurateTimeFileName()
{
	struct timeval _gettv;
	struct tm *_timenow;
	gettimeofday(&_gettv,NULL);
	_timenow =  localtime(&_gettv.tv_sec);
	char temp[30] = {0};
	snprintf(temp, 24, "%04d-%02d-%02d_%02d-%02d-%02d_%03ld", _timenow->tm_year + 1900, _timenow->tm_mon + 1, _timenow->tm_mday, _timenow->tm_hour, _timenow->tm_min, _timenow->tm_sec,_gettv.tv_usec/1000);
	std::string tempstring;
	tempstring.assign(temp, 0, 24);
	return tempstring;
}
void SetTime::loadTimeAndNum()
{
	// recordFile = FileLog::GetInstance()->GetLocateRecordFile();
	// std::string TARGETPHONENUM;
	// std::string TAPHNUM;
	// std::string temp = "T" + getTime() + "I" + TARGETPHONENUM + "P" + TAPHNUM + "\n";
	// printf("Save Time And Num\n");
	// fwrite(temp.c_str(), sizeof(char), strlen(temp.c_str()) + 1, recordFile);
	// fflush(recordFile);
	//fclose(recordFile);
}

void SetTime::SetTimer(int time) {
	m_Timer = time;
}

int SetTime::GetTimer() {
	return m_Timer;
}