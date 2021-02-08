#include "cp56time2a.h"
#include <stdio.h>



void CP56Time2a(unsigned char *data, SYSTEMTIME *st)
{
    CP56Time2aToTime(data, st);
}


void CP56Time2aToTime(unsigned char *data, SYSTEMTIME *st)
{
    unsigned int mili = data[0] | data[1]<<8;
    st->wSecond         = mili / 1000;
    st->wMilliseconds = mili - st->wSecond*1000;

    if(data[2] & 0x40)
        st->genuine = true;
    else
        st->genuine = false;

    if(data[2] & 0x80)
        st->valid = true;
    else
        st->valid = false;

    st->wMinute         = data[2] & 0x3F;;
    st->wHour           = data[3] & 0x1F;

    if(data[3] & 0x80)
        st->summer = true;
    else
        st->summer = false;
        
    st->wDay            = data[4] & 0x1F;
    st->wDayOfWeek = (data[4] & 0xE0 ) >> 5;
    if(st->wDayOfWeek)  // if zero day of week not used.
        st->wDayOfWeek = (st->wDayOfWeek + 1)%7;
    st->wMonth          = data[5] & 0x0F;
    st->wYear           = 2000 + (data[6] & 0x7F);
}

//      convert SYSTEMTIME to CP56Time2a
//
void TimeToCP56Time2a(SYSTEMTIME *st, unsigned char *data)
{
    unsigned int m;
    
    m = st->wMilliseconds + 1000 * st->wSecond;
    data[0] = m & 0xFF;
    data[1] = (m & 0xFF00)>>8;
    data[2] = st->wMinute & 0x00FF;     // add valid flag and genuine flag
    data[3] = st->wHour & 0x00FF;       // add summer flag
    /* data[4] = ((st->wDayOfWeek%7)&0x03)<<5 | (st->wDay&0x1F); */
    data[4] = (st->wDay);
    data[5] = st->wMonth & 0x0F;
    data[6] = st->wYear - 2000;
}
