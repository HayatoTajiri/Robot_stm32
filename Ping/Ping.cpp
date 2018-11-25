#include "Ping.h"

#include "mbed.h"

Ping::Ping(PinName PING_PIN)
        : _event(PING_PIN)
        , _cmd(PING_PIN)
        , _timer()
        {
            _event.rise(this,&Ping::_Starts);
            _event.fall(this,&Ping::_Stops);
            _SPEED_OF_SOUND_CM = 33;
        } 
        
void Ping::_Starts(void)
{
      _Valid = false;  // start the timere, and invalidate the current time.
      _Busy = true;
      _timer.start();
      _Time = _timer.read_us();      
}

void Ping::_Stops(void)
{
      _Valid = true;  // When it stops, update the time
      _Busy = false;
      _Time = _timer.read_us()-_Time;
}

void Ping::Send()
{
     
     _cmd.output();
     _cmd.write(0);  // see the ping documentation http://www.parallax.com/Portals/0/Downloads/docs/prod/acc/28015-PING-v1.6.pdf
     wait_us(3);
     _cmd.write(1);
     wait_us(3);
     _cmd.write(0);
     _cmd.input();
     
}
void Ping::Set_Speed_of_Sound(int SoS_ms )
{
     _SPEED_OF_SOUND_CM = SoS_ms;
}

int Ping::Read_cm()
// -1 means not valid.
{
    if(_Valid && ~_Busy) 
        return ((_Time*_SPEED_OF_SOUND_CM)/1000);
    else 
        return -1;
}