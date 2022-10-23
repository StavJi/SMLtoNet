#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

//Tick count that takes longer to roll over
class TickCounter
{
  private:
  
    unsigned long long _tickCounter = 0;
    unsigned long _lastCpuTickCount = 0;

  public:
  
    unsigned long long getMillis()
    {
      return millis();
    }

    unsigned long getSeconds()
    {
      return millis() / 1000;
    }
};

class PollDelay
{
  private:
    
    unsigned long long _startMillis;
    TickCounter* _tickCounter;

  public:
    
    PollDelay(TickCounter& tickCounter)
    {
      _tickCounter = &tickCounter;
      _startMillis = _tickCounter->getMillis();
    }
  
    void reset()
    {
      _startMillis = _tickCounter->getMillis();
    }

    //Call this once every 26 seconds or it'll roll over
    int compare(unsigned int millisSinceStart)
    {
      return (int)( _tickCounter->getMillis() - (_startMillis + millisSinceStart) );
    }
};

#endif
