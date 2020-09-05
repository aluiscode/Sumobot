#include <Ultrasonic.h>

/* (Trig PIN,Echo PIN, Max.TimeOut in µsec )
 *  centímetros * 58 = Max.TimeOut
 */
Ultrasonic ultraback(12, 13, 2610);
Ultrasonic ultralright(10, 11, 2610);
Ultrasonic ultrafront(6, 7, 2610);
Ultrasonic ultraleft(8, 9, 2610);

boolean rin = 1;                  //Rin color 1=black 0=white
int umbral=550;                   //Umbral 1024=black 0=white

// IR sensors positions in sumobot
boolean IFrontRight;
boolean IFrontLeft;
boolean IBackRight;
boolean IBackLeft;

// Arrays to save data sensors
byte SensorValue[4];             //Array ultrasonic
int  InfraValue[4];              //Array ir sensors

byte RobotEnemigo;
byte referencia;
byte menor;
int contador;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); //Motor izquierdo
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT); //Motor derecho
  pinMode(5, OUTPUT);
  delay(4900);
}

void loop() {
  RobotEnemigo = sensado();///lectura de lis 8 

  if (RobotEnemigo == 0)
  {
    atacarAdelante();
    posicion(umbral);
    if(IFrontRight!=rin || IFrontLeft!=rin)  //MODIFICAR A DETECTA LINEA
    {
      detenerMotores();
      atacarAtras();
      delay(1000);
      detenerMotores();
    }
  }

  if(RobotEnemigo == 1)
  {
    referencia = menor;
    SensorValue[0] = ultrafront.Ranging(CM);
    SensorValue[3] = ultraback.Ranging(CM);

    while ( SensorValue[0] > referencia  && SensorValue[3] > referencia )
      {
        giroDerecha();
        SensorValue[0] = ultrafront.Ranging(CM);
        SensorValue[3] = ultraback.Ranging(CM);
        
        contador += contador;
        if (contador >= 40)
        {
          detenerMotores();
          delay(200);
          break;
        }
      }
      detenerMotores();
   }
contador=0;
RobotEnemigo = sensado();

 if(RobotEnemigo == 2 )
  {
    referencia = menor;
    SensorValue[0] = ultrafront.Ranging(CM);
    SensorValue[3] = ultraback.Ranging(CM);

    while ( SensorValue[0] < referencia  && SensorValue[3] < referencia  )
      {
        giroIzquierda();
        SensorValue[0] = ultrafront.Ranging(CM);
        SensorValue[3] = ultraback.Ranging(CM);
        
        contador += contador;
        if (contador >= 40)
        {
          detenerMotores();
          delay(200);
          break;
        }
      }
      detenerMotores();
   }
  contador=0;
  RobotEnemigo = sensado();

 if (RobotEnemigo == 3)          
  {  
      atacarAtras();
      posicion(umbral);
      if(IBackRight!=rin || IBackLeft!=rin)
      {
      detenerMotores();
      atacarAdelante();
      delay(800);
      detenerMotores();
      }
  }

  if (menor>=40)
  {
    giroDerecha();
    delay(500);
    detenerMotores();
  }
   
}

byte sensado() {

  byte posicion = 0;

  SensorValue[0] = ultrafront.Ranging(CM);      //0 Enfrente
  SensorValue[1] = ultralright.Ranging(CM);     //1 Derecha
  SensorValue[2] = ultraleft.Ranging(CM);       //2 Izquierda
  SensorValue[3] = ultraback.Ranging(CM);       //3 Atras

  menor = SensorValue[0];

  for (byte i = 0; i < 4; i++) {
    if (SensorValue[i] < menor)
    {
      menor = SensorValue[i];
      posicion = i;
    }
  }
  if (posicion == 200)
  {
    posicion = 0;
  }

  return posicion;
}

void posicion(int umbra){
  InfraValue[0]= analogRead(2);
  InfraValue[1]= analogRead(1);
  InfraValue[2]= analogRead(4);
  InfraValue[3]= analogRead(3);

  for(int y=0;y<4;y++){
     if (InfraValue[y]<=umbra)
     {
      InfraValue[y]=0;    
     }
     else
     {
      InfraValue[y]=1;
     }
  }

  IFrontRight=InfraValue[0];
  IFrontLeft =InfraValue[1];
  IBackRight =InfraValue[2];
  IBackLeft  =InfraValue[3];
   
}

void atacarAdelante()
{
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(5, HIGH);
}

void atacarAtras()
{
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
}

void giroDerecha()
{
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
}

void giroIzquierda()
{
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(5, HIGH);
}

void detenerMotores()
{
  for (byte i = 2; i < 6; i++)
  {
    digitalWrite(i, LOW);
  }
}
