// Henkilökohtaiset blynk tunnukset
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

//Tuodaan kirjastot
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

//Nimetään muuttuja taulukot blynkkiä varten
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";
BlynkTimer timer;

//Nimetään arduinon pinnit
const int buttonPin = 4;
const int ledPin0 = 0;
const int ledPin1 = 1;
const int ledPin2 = 2;

//Määritetään muuttujat
int ajastin = 0;
unsigned long AjanAlku = 0;
double Lasku = 0;
bool lastButtonState = 1;
double Ennatys = 10000000;

void setup() {
  // Saa arduinon internettiin
  Blynk.begin(auth, ssid, pass);

  //Määritetään pinnin modet
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  //Kutsutaan AjanKirjoittelu alaohjelmaa 10 kertaa sekunnissa 
  timer.setInterval(100L, AjanKirjoittelu);
}

void loop() {
  Blynk.run();
  timer.run();

//ButtonState lukee button pinniä ja saa tilan 1 tai 0  riippuen mitä ohjelma sille antaa 
  int buttonState = digitalRead(buttonPin);

// kysyy napin onko nappia painettu
  if (buttonState == 0 && lastButtonState == 1) {
    //Kun nappia painetaan  ja LastButtonstate on 1 ja buttonstate on 0 aloitetaan if 
    if (ajastin == 0) {
      //Kutsutaan Starttivalot aliohjelmaa
      Starttivalot();
      //Tallentaa tämän hetken millis ajan AjanAlku muuttujaan
      AjanAlku = millis();
      //Ajastin muuttuja vaihdetaan 0->1
      ajastin = 1;
    } else {
      //Kun ajastin lopetetaan vaihdetaan ajastin takisin = 0 
      ajastin = 0;
      //Lasketaan lopullinen aika ja kirjoitetaan se blynkkiin samalla kaavalla kuin Ajankirjoittelu ohjelmassa 
      Lasku = (millis() - AjanAlku) / 1000.0;
      Blynk.virtualWrite(V2, Lasku);
      //Kysytään EnnatyksenKirjoittelulta onko ennätysaika vai ei
      EnnatyksenKirjoittelu();
    }
  }

  lastButtonState = buttonState;
}

void AjanKirjoittelu() {
  // jos ajastin muuttuja on ==1 kirjoittaa kuluneen ajan blynkkiin
  if (ajastin == 1) {
    //Määritetään Kulunut aika ja annetaan sille arvo millis - AjanAlku joka laskee kuluneen ajan
    double kulunutAika = (millis() - AjanAlku) / 1000.0;
    //Kirjoittaa kuluneen ajan Blynkkiin
    Blynk.virtualWrite(V2, kulunutAika);
  }
}

void EnnatyksenKirjoittelu() {
  // jos lasku on hitaampi kuin nykyinen ennätys annetaan tylsät loppu valot
  if (Lasku >= Ennatys) {
    Loppuvalot();
  } else {
    //jos tulee ennätysaika tallennetaan se muuttujaan Ennatys ja kirjoitetaan se blynkin ennätys ruutuun 
    Ennatys = Lasku;
    Blynk.virtualWrite(V0, Ennatys);
    //kutsutaan ennätysvalo ohjelmaa
    Enkkavalot();
  }
}

void Starttivalot() {
  //Käy kolme kertaa tämän loopin läpi ja sen jälkeen menee eteenpäin int random kohtaan jossa satunnaisen ajan jälkeen se vilkuttaa sekunniksi kolmea lediä
  for (int i = 1; i <= 3; i++) {
    digitalWrite(ledPin0, 1);
    digitalWrite(ledPin1, 1);
    digitalWrite(ledPin2, 1);
    delay(500);
    digitalWrite(ledPin0, 0);
    digitalWrite(ledPin1, 0);
    digitalWrite(ledPin2, 0);
    delay(500);
  }
  int Randomi = random(0, 2001);
  delay(Randomi);
  digitalWrite(ledPin0, 1);
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 1);
  delay(1000);
  digitalWrite(ledPin0, 0);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
}

void Loppuvalot() {
  //Käy loopin läpi kolme kertaa 
  for (int i = 1; i <= 3; i++) {
    digitalWrite(ledPin0, 1);
    digitalWrite(ledPin1, 1);
    digitalWrite(ledPin2, 1);
    delay(500);
    digitalWrite(ledPin0, 0);
    digitalWrite(ledPin1, 0);
    digitalWrite(ledPin2, 0);
    delay(500);
  }
}

void Enkkavalot() {
  // käy tämän setin kerran läpi kutsuttaessa
  digitalWrite(ledPin0, 1);
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 1);
  delay(100);
  digitalWrite(ledPin0, 1);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
  delay(100);
  digitalWrite(ledPin0, 1);
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 1);
  delay(100);
  digitalWrite(ledPin0, 0);
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 0);
  delay(100);
  digitalWrite(ledPin0, 1);
  digitalWrite(ledPin1, 1);
  digitalWrite(ledPin2, 1);
  delay(100);
  digitalWrite(ledPin0, 0);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 1);
  delay(100);
  digitalWrite(ledPin0, 0);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
}
