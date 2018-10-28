#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include "ifttt.h"

int main(int argc, char *argv[]) 
{
  char state = 'A';
  time_t start, end;
  double elapsed;
  int count;

  wiringPiSetup();
  pinMode(0, INPUT);	/* IR Detector */
  pinMode(1, OUTPUT);	/* LED 1 */
  pinMode(2, OUTPUT);	/* LED 2 */
  pinMode(3, INPUT);	/* BUTTON 3 */
  pinMode(4, OUTPUT);	/* LED 4 */

  pullUpDnControl(3, PUD_UP);

  while (1) {
    switch(state)
    {
      /* ALARM_OFF */
      case 'A' :
        printf("ALARM_OFF");
        digitalWrite(1, HIGH);
        digitalWrite(2, LOW);
        digitalWrite(4, LOW);
        delay(1000);
        while(1)
        {
          if(digitalRead(3) == 0)
          {
            state = 'B';
            break;
          }  
        }
        digitalWrite(1, LOW);
        break;

      /* ALARM_ARMING */
      case 'B' :
        printf("ALARM_ARMING");
        time(&start);
        do
        {
          time(&end);
          elapsed = difftime(end, start);
          if((int)(elapsed) % 2 == 0)
          {
            digitalWrite(1, HIGH);
          }
          else 
          {
            digitalWrite(1, LOW);
          }
        } while(elapsed < 10);
        digitalWrite(1, LOW);
        digitalWrite(2, HIGH);
        state = 'C';
        break;
 
      /* ALARM_ARMED */
      case 'C' :
        printf("ALARM_ARMED");
        while(1)
        {
          if(digitalRead(3) == 0) 
          {
            state = 'A';
            break;
          } 
          if(digitalRead(0) == 1)
          {
            state = 'D';
            break;
          }
        }
        break;

      /* ALARM_TRIGGERED */
      case 'D' :
        printf("ALARM_TRIGGERED");
        time(&start);
        do
        {
          time(&end);
          elapsed = difftime(end, start);
          if((int)(elapsed) % 2 == 0)
          {
            digitalWrite(1, HIGH);
            digitalWrite(2, HIGH);
          }
          else
          {
            digitalWrite(1, LOW);
            digitalWrite(2, LOW);
          }
          
          if(digitalRead(3) == 0) 
          {
            state = 'A';
            break;
          }
        } while(elapsed < 10);
        state = 'E';
        break;

      /* ALARM_SOUNDING */
      case 'E' :
        printf("ALARM_SOUNDING");
        ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/mZZA4uLIfY-Bj2Xc-VoBu0AtVIzTle82sjLHYNs9nA_", "my1", "my 2", "my 33333");
        count = 0;
        while(digitalRead(3) == 1) 
        {
          time(&start);
          do 
          {
            time(&end);
            elapsed = difftime(end, start);
            if(count % 2 == 0) 
            {
              digitalWrite(1, HIGH);
              digitalWrite(2, HIGH);
              digitalWrite(4, HIGH);
            }
            else 
            {
              digitalWrite(1, LOW);
              digitalWrite(2, LOW);
              digitalWrite(4, LOW);
            }
          } while(elapsed < 2);
          count++;
        }
        state = 'A';
        break;

      default :
        printf("ERROR\n" );
     }
  }
  return 0;  
}
