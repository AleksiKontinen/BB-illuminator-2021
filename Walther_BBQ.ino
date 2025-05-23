#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
 
#define buzzerPin 6
#define mosfet 10
#define valonKesto 500
#define clockSpeed 1000

int buzzer = buzzerPin;
double yAkseli, zAkseli, xAkseli, vanhaY;
bool buzzerState = true;

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(mosfet, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("F");
    while (1)
      ;
  }
  
  accel.setRange(LSM303_RANGE_4G);   // copy
  accel.setMode(LSM303_MODE_NORMAL); //paskaa

}

void loop() {
  
  
  sensors_event_t event; // who
  accel.getEvent(&event);// knows

  xAkseli = event.acceleration.x;// pyytää kiihtyvyyden tietyllä akselilla 2 desimaalin tarkkuudella
  yAkseli = event.acceleration.y;// säilöö kutakin sen omassa muuttujassa, helpottaakseen vertailua ja matematiikkaa
  zAkseli = event.acceleration.z;// datatyyppi on double

  delay(clockSpeed);
  
  if(vanhaY == yAkseli){ // jos muutosta ei tapahdu 
    Serial.println("R");
    delay(100);
    return;
  }

  
  if(yAkseli >= 9 && abs(zAkseli) < 2){ // jos y akselin kiihtyvyys on yli tai yhtäkuin 9 m/s²   JA   zAkselin kiihtyvyyden itseisarvo on alle 2
    
    /*digitalWrite(buzzer, HIGH); // mosfetti aukeaa, ja valo välähtää
    digitalWrite(mosfet, HIGH); // ääntä lähtee myös
    delay(valonKesto);       // muuttuja vilauttusViive säätää väläytyksen pituutta default on .5s
    digitalWrite(buzzer, LOW);
    digitalWrite(mosfet, LOW);*/
    Serial.println("flash");
    delay(1000);
  
    vanhaY = yAkseli;
  
  }

  //tiedonlähetys tietokoneelle       <---     POISTETTAVA!!!
  Serial.print("X: "); 
  Serial.println(xAkseli);
  Serial.print("Y: ");
  Serial.println(yAkseli);
  Serial.print("Z: ");
  Serial.println(zAkseli);
  //tähän saakka


  
  if(zAkseli < -9 && abs(xAkseli) + abs(yAkseli) < 2){ //jos aseen osottaa taivaalle se vaihtuu äänettömälle
                                //buzzerState ohjaa onko äänettömällä vai ei.
    buzzerState = !buzzerState; // Tämä kohta vaitaa äänettömältä äänille ja toisinpäin
    
    if(buzzerState == true){ // äänet päälle
      
      Serial.println("on");
      
      buzzer = buzzerPin; // Vaihtaa buzzerin virrat takaisin buzzerin kohdalle

      /*digitalWrite(buzzer, HIGH);
      delay(200);               // piip
      digitalWrite(buzzer,LOW);
      delay(300);
      digitalWrite(buzzer, HIGH);
      delay(200);               // piip
      digitalWrite(buzzer,LOW);*/
      
    }else{                // äänettömälle
      Serial.println("off");
      /*
      digitalWrite(buzzer, HIGH);
      delay(750);               // piiiiiiip
      digitalWrite(buzzer,LOW);*/

      buzzer = LED_BUILTIN; // vaihtaa buzzeriin menevän virran sisäiseen lediin
      
    }
  delay(2000); // antaa 2s rauhaa, ja estää tämän rämpyttelyä
  } 
}
