#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define OLED_ADDR 0x3C
#define SDA_PIN 16
#define SCL_PIN 17
Adafruit_SSD1306 display(128, 64, &Wire, -1);


#define ONE_WIRE_PIN 26
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);


#define RELAY_PIN 33

//high level trigger 哈哈
#define RELAY_ACTIVE_LEVEL   HIGH    
#define RELAY_INACTIVE_LEVEL LOW


#define TH_ON  26.0  
#define TH_OFF 25.8   

bool fanOn = false;
bool scrollOn = false;

void setFan(bool on) {
  fanOn = on;
  digitalWrite(RELAY_PIN, on ? RELAY_INACTIVE_LEVEL
                             : RELAY_ACTIVE_LEVEL);
}

const unsigned char epd_bitmap_39_398460_christmas_tree_icon [] PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x01,0x80,0x00,0x00,0x0f,0xf0,0x00,
  0x00,0x0f,0xf0,0x00,0x00,0x07,0xe0,0x00,0x00,0x07,0xe0,0x00,0x00,0x04,0x20,0x00,
  0x00,0x01,0x80,0x00,0x00,0x03,0xe0,0x00,0x00,0x07,0xf0,0x00,0x00,0x1f,0xf8,0x00,
  0x00,0x3f,0xfc,0x00,0x00,0x7f,0xfe,0x00,0x00,0xff,0xff,0x00,0x00,0x7f,0xff,0x00,
  0x00,0x38,0x00,0x00,0x00,0x7f,0xc2,0x00,0x00,0xff,0xff,0x00,0x01,0xff,0xff,0x80,
  0x03,0xff,0xff,0xc0,0x07,0xff,0xff,0xe0,0x0f,0xff,0xff,0xf0,0x03,0xff,0xff,0xc0,
  0x01,0xc0,0x00,0x00,0x07,0xfc,0x00,0x00,0x0f,0xff,0xff,0xf0,0x1f,0xff,0xff,0xf8,
  0x3f,0xff,0xff,0xfe,0x7f,0xff,0xff,0xff,0x0f,0xff,0xff,0xf0,0x00,0x00,0x00,0x00
};


struct Snow {
  int x;
  int y;
  int speed;
};

#define SNOW_COUNT 10
Snow snow[SNOW_COUNT];

void initSnow() {
  randomSeed(esp_random());
  for (int i = 0; i < SNOW_COUNT; i++) {
    snow[i].x = random(0, 128);
    snow[i].y = random(0, 64);
    snow[i].speed = random(1, 3);
  }
}

void drawAndUpdateSnow(bool faster) {
  for (int i = 0; i < SNOW_COUNT; i++) {


    if (snow[i].x > 34 && snow[i].y < 44) {
      display.drawPixel(snow[i].x, snow[i].y, SSD1306_WHITE);
    }

    int sp = snow[i].speed + (faster ? 1 : 0);
    snow[i].y += sp;

    if (snow[i].y > 63) {
      snow[i].y = 0;
      snow[i].x = random(34, 128);
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(300);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed");
    while (1);
  }
  display.setTextColor(SSD1306_WHITE);

  sensors.begin();

  pinMode(RELAY_PIN, OUTPUT);
  setFan(false);   

  initSnow();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.println("Battery Monitor");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("Hello sasa");
  display.display();
  delay(2000);
}


void loop() {
  sensors.requestTemperatures();
  float t = sensors.getTempCByIndex(0);

  if (t == DEVICE_DISCONNECTED_C) {
    setFan(false);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DS18B20 ERROR");
    display.display();
    delay(1000);
    return;
  }


  if (!fanOn && t > TH_ON) setFan(true);
  else if (fanOn && t < TH_OFF) setFan(false);

  display.clearDisplay();

  display.drawBitmap(0, 16, epd_bitmap_39_398460_christmas_tree_icon, 32, 32, SSD1306_WHITE);

  int x0 = 36;
  display.setTextSize(1);
  display.setCursor(x0, 5);
  display.println("Temperature");

  display.setTextSize(2);
  display.setCursor(x0, 20);
  display.print(t, 1);
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(0, 46);
  display.println("Merry X'mas");

  display.setCursor(86, 46);
  display.println(fanOn ? "FAN ON" : "FAN OFF");

  if (fanOn) {
    display.setCursor(0, 56);
    display.println("FAN activating");
  }


  drawAndUpdateSnow(fanOn);

  display.display();
  delay(200);
}
