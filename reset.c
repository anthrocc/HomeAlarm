#include <wiringPi.h>
#include <stdio.h>
int main (int argc, char *argv[])
{
  wiringPiSetup () ;
  pinMode (1, OUTPUT) ;
  pinMode (2, OUTPUT) ;
  pinMode (4, OUTPUT) ;

  digitalWrite (1, LOW);
  digitalWrite (2, LOW);
  digitalWrite (4, LOW);  
  return 0 ;
}

