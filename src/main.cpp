#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
#include <OneButton.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

// 'image', 128x64px
const unsigned char logo_image [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe7, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc3, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x81, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfd, 0xc3, 0xef, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0xe7, 0x8f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x7f, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x03, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x01, 0x80, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x01, 0xc0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0xf0, 0xfe, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe3, 0xf8, 0xf8, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0xf0, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0xf0, 0x39, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf9, 0xe0, 0x7c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x03, 0xc0, 0xfe, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x03, 0xc0, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x07, 0x81, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x03, 0x03, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0x00, 0x07, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xf0, 0x7f, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xff, 0xfc, 0x7f, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xff, 0xfc, 0x77, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7f, 0xfc, 0x37, 0xf8, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7f, 0xfc, 0x33, 0xf0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x3f, 0xfc, 0x30, 0x80, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x1f, 0xfc, 0x30, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x0f, 0xf8, 0x30, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x83, 0xc0, 0x78, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x7c, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0xff, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x01, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define buzzerPin 0

#define buttonPin 1
#define rotaryAPin 3
#define rotaryBPin 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OneButton button(buttonPin, true, true);

RotaryEncoder encoder(rotaryAPin, rotaryBPin, RotaryEncoder::LatchMode::TWO03);

enum menus {MAIN, NORMAL, NORMAL_CD, CYCLES, SERIES, SETTINGS};
byte selectedMenu = MAIN;

bool updateDisplay = true;

static unsigned long lastTime = 0;

int normalSeconds = 0;
int normalMinutes = 0;

void click() {}
void doubleClick() {}
void longPressStart() {}
void longPress() {}
void longPressStop() {}


void rotaryTurned() {
  encoder.tick();
}

int rotaryEncoder() {
  int position = encoder.getPosition();
  if (position > 1 || position < -1) {
    encoder.setPosition(0);
    if (position > 0) {
      return 1;
    } else {
      return -1;
    }
  } else {
    return 0;
  }
}

bool displayMenu() {
  static byte lastSelectedMenu;
  if (selectedMenu != lastSelectedMenu || updateDisplay) {
    Serial.println("Updating display");
    lastSelectedMenu = selectedMenu;
    updateDisplay = false;
    return true;
  }
  else {
    return false;
  }
}

void Main() {
  #define NBMENUS 4
  const String menus[NBMENUS + 2] = {"", "Normal", "Cycles", "Series", "Reglage", ""};
  static int selection;
  static byte lastSelection;
  String displayedText[3];
  byte locationOfText[3];
  
  if (selection != lastSelection || displayMenu()) {
    for (byte i = 0; i < 3; i++) {
    displayedText[i] = menus[selection + i];
    locationOfText[i] = displayedText[i].length();
    }
    lastSelection = selection;

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - locationOfText[0] * 12) / 2, 0);
    display.print(displayedText[0]);
    display.setTextSize(3);
    display.setCursor((SCREEN_WIDTH - locationOfText[1] * 18) / 2, 20);
    display.print(displayedText[1]);
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - locationOfText[2] * 12) / 2, 48);
    display.print(displayedText[2]);
    display.display();
  }

  selection = selection + rotaryEncoder();
  if (selection < 0) {
    selection = 0;
  } else if (selection >= NBMENUS) {
    selection = NBMENUS - 1;
  }
  if (button.rose()) {
    selectedMenu = selection + 1;
  }
}


void Normal() {
  String time;

  if (normalMinutes == 0) {
    normalSeconds = normalSeconds + rotaryEncoder() * 5;
    Serial.println(normalSeconds);
    if (normalSeconds > 59) {
      normalSeconds = 0;
      normalMinutes = 1;
    } else if (normalSeconds < 0) {
      normalSeconds = 0;
    }
  } else {
    normalMinutes = normalMinutes + rotaryEncoder();
    if (normalMinutes > 60) {
      normalMinutes = 60;
    } else if (normalMinutes <= 0) {
      normalSeconds = 55;
      normalMinutes = 0;
    }
  }
  
  
  if (button.rose()) {
    while (button.read() == HIGH) {
      button.tick();
      if (button.currentDuration() > 1000) {
        selectedMenu = MAIN;
        break;
      }
    }
    selectedMenu = NORMAL_CD;
  }

  updateDisplay = true;
  if (displayMenu()) {
    time = String(normalMinutes) + ":";
    if (normalMinutes < 10) {
      time = "0" + time;
    }
    if (normalSeconds < 10) {
      time = time + "0";
    }
    time = time + String(normalSeconds);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - 6 * 12) / 2, 0);
    display.print("Normal");
    display.setTextSize(3);
    display.setCursor((SCREEN_WIDTH - 5 * 18) / 2, 20);
    display.print(time);
    display.display();
  }
}

void Normal_CD() {
  bool paused = false;
  String time;

  if (button.rose()) {
    while (button.read() == HIGH) {
      button.tick();
      if (button.currentDuration() > 1000) {
        selectedMenu = MAIN;
        break;
      }
    }
    paused = !paused;
  }
  while (normalSeconds == 0 && normalMinutes == 0) {
    button.tick();
    digitalWrite(buzzerPin, HIGH);
    if (button.rose()) {
      digitalWrite(buzzerPin, LOW);
      selectedMenu = MAIN;
      break;
    }
  }
 
  if (millis() - lastTime >= 1000 && !paused) {
    lastTime = millis();
    if (normalSeconds == 0) {
      normalSeconds = 59;
      normalMinutes = normalMinutes - 1;
    } else {
      normalSeconds = normalSeconds - 1;
    }
  }

  updateDisplay = true;
  if (displayMenu()) {
    time = String(normalMinutes) + ":";
    if (normalMinutes < 10) {
      time = "0" + time;
    }
    if (normalSeconds < 10) {
      time = time + "0";
    }
    time = time + String(normalSeconds);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor((SCREEN_WIDTH - 6 * 12) / 2, 0);
    display.print("Normal");
    display.setTextSize(3);
    display.setCursor((SCREEN_WIDTH - 5 * 18) / 2, 20);
    display.print(time);
    display.display();
  }
}


void menu() {
  switch (selectedMenu) {
  case MAIN:
    Main();
    break;
  case NORMAL:
    Normal();
    break;
  case NORMAL_CD:
    Normal_CD();
    break;
  case CYCLES:
        
    break;
  case SERIES:

    break;
  case SETTINGS:

    break;
  default:
    selectedMenu = MAIN;
    break;
  }
}


void setup() {
  Serial.begin(9600);
  
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo_image, 128, 64, 1);
  display.display();
  delay(1500);

  attachInterrupt(digitalPinToInterrupt(rotaryAPin), rotaryTurned, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryBPin), rotaryTurned, CHANGE);

  button.attachClick(click);
  button.attachDoubleClick(doubleClick);
  button.attachLongPressStart(longPressStart);
  button.attachDuringLongPress(longPress);
  button.attachLongPressStop(longPressStop);


  pinMode(buzzerPin, OUTPUT);
  PORT->Group[g_APinDescription[0].ulPort].PINCFG[g_APinDescription[0].ulPin].bit.DRVSTR = 1;
  

  display.setTextColor(WHITE);
}

void loop() {
  menu();
  button.tick();
  delay(10);
}