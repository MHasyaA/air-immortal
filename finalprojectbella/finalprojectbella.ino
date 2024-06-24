/*********************************************************
 * BELLA NABILA
 * UNIVERSITAS AL-AZHAR INDONESIA
 *********************************************************/
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6rvymfhhh"
#define BLYNK_TEMPLATE_NAME "Air Immortal"
#define BLYNK_AUTH_TOKEN "mF5YyCMK1644s4dMMWynic0vN01XaEf9"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <AverageValue.h>

char ssid[] = "Kuykuy";
char pass[] = "belidong";

#define R 26
#define G 32
#define B 33
#define MQ135 34
#define BZR 27

int Rload = 1000; // nilai resistansi bawaan pabrik
float rO = 3200; // hasil uji coba diruangan tertentu
float a = 110.7432567; // hasil perhitungan
float b = -2.856935538; // hasil perhitungan
float minppm = 0;
float maxppm = 0;
const long MAX_VALUE_NUM = 10;
AverageValue<long> averageValue(MAX_VALUE_NUM);

BlynkTimer timer;
String st;
double ppm = 0; // Mengubah deklarasi ppm menjadi global

void myTimer(){
  Blynk.virtualWrite(V5, ppm);
  Blynk.virtualWrite(V6, st);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimer);

  pinMode(MQ135, INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(BZR, OUTPUT);
  
  minppm = pow((1000 / 110.7432567), 1 / -2.856935538);
  maxppm = pow((10 / 110.7432567), 1 / -2.856935538);
}

void loop()
{
  Blynk.run();
  timer.run();

  int adcRaw = analogRead(MQ135);
  double rS = ((1024.0 * Rload) / adcRaw) - Rload;
  float rSrO = rS / rO;
  ppm = a * pow((float)rS / (float)rO, b);
  averageValue.push(ppm);
  Serial.println(ppm);
  delay(1000);

  if(ppm >= 1000){
    tone(BZR, 2000);
    setColor(255, 0, 0); // Merah
    st = "Udara Buruk";
  } else {
    noTone(BZR);
    setColor(0, 255, 0); // Hijau
    st = "Udara Baik";
  }
}

void setColor(int red, int green, int blue) {
  analogWrite(R, red);
  analogWrite(G, green);
  analogWrite(B, blue);
}
