#include <Servo.h>
#include <NewPing.h>
//includem fisierul cu defini?iile pentru tonuri
#define NOTE_G6 1568

//-----------------------------------------------------DEFINIE----------------------------------------------------------------------------
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11
#define TRIGGER_PIN  2     // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9    // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 250 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 250cm.
#define vit 80          //stabilita in urma testelor, pentru alimentarea de la cablu
//-----------------------------------------------------DEFINIE----------------------------------------------------------------------------

Servo srv;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//int noteDurations[] = {3,4,4,4}; 

void setup() {
  pinMode(13, OUTPUT); 
  playWithServo(8);
  Serial.begin(115200); 
 
 digitalWrite(mpin00, 0);// configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin01, 0);// configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin10, 0);// configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin11, 0);// configurarea pinilor motor ca iesire, initial valoare 0
 
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
 
 pinMode(13, OUTPUT);
  
 StartMotor (mpin00, mpin01, 0, vit);
 StartMotor (mpin10, mpin11, 0, vit);
 
 int distt = ping_citire();    // Get Ping Distance.
 delay(100);               // Wait for 100ms.
}


// Functie pentru controlul unui motor
// Intrare: pinii m1 si m2, directia si viteza
void StartMotor (int m1, int m2, int forward, int speed)
{
  if (speed == 0) // oprire
 {
    digitalWrite(m1, 0); 
    digitalWrite(m2, 0);
 }
 else
 {
 if (forward)
 {
    digitalWrite(m2, 0);
    analogWrite (m1, speed); // folosire PWM
 }
 else
 {
    digitalWrite(m1, 0);
    analogWrite(m2, speed);
 }
 }
}

// Executa oprire motoare, urmata de delay
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0); //opresc motorul, dandu-i 0 la ultimul parametru ce reprezinta viteza
 StartMotor (mpin10, mpin11, 0, 0);//opresc motorul, dandu-i 0 la ultimul parametru ce reprezinta viteza
 delay(ms);                       //cat sa dureze oprirea motorului in milisecunde
}

void playWithServo(int pin)//aceasta functie va readuce pozitia motorului serva la 90 de grade, adica directia inainte
{
 srv.attach(pin); //atasam pinul
 srv.write(90);  //trimitem unghiul la care vrem sa ajunga motorul
 delay(50);
 srv.detach(); //detasam pinul
}

void uitate_stanga(int pin){//aceasta functie va duce pozitia motorului serva la 45 de grade, adica directia stanga
  srv.attach(pin);//atasam pinul
  srv.write(45); //trimitem unghiul la care vrem sa ajunga motorul
  delay(500);
  srv.detach();//detasam pinul
}


void uitate_dreapta( int pin){//aceasta functie va duce pozitia motorului serva la 135 de grade, adica directia dreapta
  srv.attach(pin);//atasam pinul
  srv.write(135);//trimitem unghiul la care vrem sa ajunga motorul
  delay(500);
  srv.detach();//detasam pinul
}


int ping_citire(){ //cu aceasta functie citim distanta returnata de senzorul ultrasonic atasat la servo
  int cm = sonar.ping_cm(); //aceasta functie converteste ceea ce primeste de la senzor in centrimetri
  delay(200);
  Serial.print("cm: ");
  Serial.println(cm);
  if (cm == 0)// se mai intampla sa apara erori de citire, iar atunci facem ca robotul sa NU se opreasca, dandu i distanta maxima
  {
    cm = 250; //distanta maxima calculata de senzor (declarata sus in sectiunea ---DEFINE----)
    Serial.print("EROARE CITIRE! ");
  }
  return cm;
}

void daibataie(int del){//aceasta functie face ca robotul sa mearga inainte cu viteza declarata in sectiunea --DEFINE----
   StartMotor (mpin00, mpin01, 0, vit);//StartMotor (mpin00, mpin01, 0, vit+40); vit 150 baterii
   StartMotor (mpin10, mpin11, 0, vit);
   delay(del);
}


void mergi_inapoi(){//aceasta functie face ca robotul sa mearga inapoi cu viteza declarata in sectiunea --DEFINE----
   StartMotor (mpin00, mpin01, 1, vit);//parametrul al III-lea stabileste directia in care merge robotul: 1 -> inapoi
   StartMotor (mpin10, mpin11, 1, vit);//                                                                 0 -> inainte
 }

 
void mergi_stanga(){//aceasta functie face ca robotul sa mearga la stanga cu viteza declarata in sectiunea --DEFINE----
   StartMotor (mpin00, mpin01, 0, vit); //motorul din drepta merge
   StartMotor (mpin10, mpin11, 0, 0);  //motorul din stanga sta
   delay(500);                        //va merge in stanga jumatate de secunda
 } 
 
 void mergi_dreapta(){//aceasta functie face ca robotul sa mearga la dreapta cu viteza declarata in sectiunea --DEFINE----
   StartMotor (mpin00, mpin01, 0, 0);    //motorul din dreapta sta
   StartMotor (mpin10, mpin11, 0, vit); //motorul din stanga merge
   delay(500);                         //va merge in dreapta jumatate de secunda
 }

void loop() { 

 daibataie(50);           //merge robotul in fata
 int dist = 50;          //dam o valoare initiala a distantei dintre robot si obstacol 
 dist = ping_citire();  //citim distanta
 Serial.println(dist); //afisam distanta in Serial Monitor
 daibataie(50);       //merge robotul in fata pentru ca atunci cand citeste distanta, se opreste
  
 if(dist < 28 )     //daca distanta e mai mica decat cea stabilita, 28, robotul se va opri si va merge in alta directie
 {  
       
        delayStopped(100); //robotul se opreste

        //---------------------------------------------STANGA------------------------------------------
        uitate_stanga(8);
        delay(100);
        int distance_left = ping_citire();
        Serial.print("Stanga: ");
        Serial.print(distance_left);
        delay(100);
        //---------------------------------------------STANGA------------------------------------------

        //---------------------------------------------DREAPTA------------------------------------------

        uitate_dreapta(8);
        delay(100);
        int distance_right = ping_citire();
        Serial.print("Dreapta: ");
        Serial.print(distance_right);
        delay(100);
        //---------------------------------------------DREAPTA------------------------------------------
   
       if(distance_right >= distance_left ) //daca distanta din dreapta e mai mare decat ce din stanga
       {
            mergi_inapoi();      //masina merge inapoi
            delay(800);         //0,8 secunde
            delayStopped(300); //sta oprita 0,3 secunde
            mergi_dreapta();  //merge in dreapta
            delay(100);
       }
       else //daca distanta din stanta e mai mare decat ce din dreapta
       {  
          mergi_inapoi();      //masina merge inapoi
          delay(800);         //0,8 secunde
          delayStopped(300); //sta oprita 0,3 secunde
          mergi_stanga();   //merge in stanga
          delay(100);

       }
      delay(100);
      playWithServo(8); //"privirea" robotului este inainte(90 de grade)
      delay(50);
      daibataie(50);//robotul merge in fata
 }   
} 
