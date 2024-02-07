#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
#include "DHT.h" //DHT Bibliothek laden

#define DHTPIN 2 //Der Sensor wird an PIN 2 angeschlossen    

#define DHTTYPE DHT22    // Es handelt sich um den DHT22 Sensor

DHT dht(DHTPIN, DHTTYPE); //Der Sensor wird ab jetzt mit „dth“ angesprochen

int sound_din=4;
int sound_ain=A0;
int ad_value;


#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

void setup() {

   pinMode(sound_din,INPUT);
  pinMode(sound_ain,INPUT);
  Serial.begin(9600);

  dht.begin();
  display.begin();
  display.fillScreen(RED);
  delay(300);
  display.fillScreen(GREEN);
  delay(300);
  display.fillScreen(BLUE);
  delay(300);
  display.fillScreen(BLACK);
  delay(1000);
  display.setCursor(2,5);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("Your Env is");

  display.setCursor(2,17);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("temp: ");

    display.setCursor(2,29);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("humid: ");

      display.setCursor(2,41);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("sound: ");


  display.drawRect(0,0,96,64,WHITE);
  delay(1000);
}

void loop() 
{


  delay(2000);

  String db;
uint16_t soundColor;

  boolean val =digitalRead(sound_din);
  Serial.println (val);

    if (val==HIGH) {
    db = "loud";
    soundColor = RED;
  }
  else {
    db = "quiet";
    soundColor = GREEN;
  }


/*


    ad_value=analogRead(sound_ain);
     db = (ad_value+83.2073) / 11.003;//-16.801 * log(ad_value/1023.) + 9.872;*/
  Serial.println(db);


    String soundstring = db;

    display.fillRect(35,17, 40, 7, BLACK);
    display.fillRect(40,29, 40, 7, BLACK);
    display.fillRect(40,41, 40, 7, BLACK);
    display.fillRect(70,5, 25, 7, BLACK);

    float Luftfeuchtigkeit = dht.readHumidity(); //die Luftfeuchtigkeit auslesen und unter „Luftfeutchtigkeit“ speichern
  
  float Temperatur = dht.readTemperature();//die Temperatur auslesen und unter „Temperatur“ speichern

  String tempstring = String(Temperatur);
  tempstring += " gC";

   String  humidstring = String(Luftfeuchtigkeit);
  humidstring += " %";

  int rating = calcEnv(Temperatur, Luftfeuchtigkeit, db);

  String ratingString;
  uint16_t ratingColor;
switch(rating){
  case 0: ratingString = "Good"; ratingColor = hexcolor(0,255,0);break;
  case 1: ratingString = "Ok"; ratingColor = hexcolor(255,255,0); break;
  case 2: ratingString = "Bad";ratingColor = hexcolor(255,0,0); break;
  default : ratingString = ""; break;
}
 
  
  Serial.print("Luftfeuchtigkeit: "); //Im seriellen Monitor den Text und 
  Serial.print(Luftfeuchtigkeit); //die Dazugehörigen Werte anzeigen
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(Temperatur);
  Serial.println(" Grad Celsius");

   display.setCursor(70,5);
  display.setTextColor(ratingColor);
  display.setTextSize(1);
  display.print(ratingString);

   display.setCursor(35,17);
  display.setTextColor(calcTempColor(Temperatur));
  display.setTextSize(1);
  display.print(tempstring);

     display.setCursor(40,29);
  display.setTextColor(calcHumidColor(Luftfeuchtigkeit));
  display.setTextSize(1);
  display.print(humidstring);

     display.setCursor(40, 41);
  display.setTextColor(soundColor);
  display.setTextSize(1);
  display.print(soundstring);
  
  
  display.fillRect(10,50, 75, 10, ratingColor);
       display.setCursor(30,51);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.print("Rating");

    display.drawRect(0,0,96,64,ratingColor);

}

uint16_t hexcolor(int r, int g, int b)
{
    uint16_t Rgb565 = 0;
    Rgb565 = (((r & 0xf8)<<8) + ((g & 0xfc)<<3) + (b>>3));
    return Rgb565;
}

uint16_t calcTempColor(float input){

  int currentTemp = input;

  int red = 0, green = 0, blue = 0;


  if(currentTemp >= 0 && currentTemp <= 17){
    red = 0;
    green = 0;
    blue = 255;

    green = green + (currentTemp * 15);
    blue = blue - (currentTemp * 15);


  }
  else if(currentTemp > 17 && currentTemp <= 34){
    red = 0;
    green = 255;
    blue = 0;

    green = green - ((currentTemp - 17) * 15);
    red = red + ((currentTemp - 17) * 15);

  }
  else if(currentTemp < 0){red = 0; green = 0; blue = 255;}
  else if(currentTemp > 34){red = 255; green = 0; blue = 0;}


  return hexcolor(red, green, blue);


}

uint16_t calcHumidColor(float input){

    int currentHumid = input;

    int red = 0, green = 0, blue = 0;

    if(currentHumid < 50 && currentHumid > 0){

          red = 255; green = 0; blue = 0;
          
          red = red - (((currentHumid * 5)));
          green = green + (((currentHumid * 5)));

    }
    else

    if(currentHumid > 50 && currentHumid < 100){

          red = 0; green = 255; blue = 0;

          green = green - (((currentHumid - 50) * 5));
          red = red + (((currentHumid - 50) * 5));

    }
    else
    if(currentHumid == 50){
      red = 0; green = 255; blue = 0;
    }
    else
    if(currentHumid == 0){
      red = 255; green = 0; blue = 0;
    }
    else
    if(currentHumid == 100){
      red = 0; green = 0; blue = 255;
    }

      return hexcolor(red, green, blue);

}

uint16_t calcSoundColor(double input){

    int currentSound = input;

    int red = 0, green = 0, blue = 0;

    if(currentSound <= 60){
      red = 0;
      green = 255;

      red = red + currentSound;
      green = green - currentSound;
                
    }

    else if (currentSound <= 100){
      red = 0;
      green = 255;

      red = red + currentSound * 2.5;
      green = green - currentSound * 2.5;
    }

    else if (currentSound > 100){
      red = 255;
      green = 0;

    }

      return hexcolor(red, green, blue);

}

int calcEnv(float temp, float humid, String sound){

  int temprating = 0, humidrating = 0, soundrating = 0;

  int overallRating = 0;

  if(temp >= 14 && temp < 23) {
    temprating = 0;
  } else if((temp >= 23 && temp < 30) || (temp < 14 && temp > 7)) {
    temprating = 1;
  }
  else {temprating = 2;}

    if(humid >= 45 && humid < 56) {
    humidrating = 0;
  } else if((humid >= 56 && humid < 80) || (humid < 45 && humid >= 20)) {
    humidrating = 1;
  }
  else {humidrating = 2;}

  if(sound == "loud"){
    soundrating = 2;
  }else {
    soundrating = 0;
  }



  overallRating = temprating + humidrating + soundrating;

  if (overallRating < 2){
    return 0;
  } else if (overallRating >= 2 && overallRating < 5){
    return 1;
  } else if (overallRating <= 5){
    return 2;
  }

}
