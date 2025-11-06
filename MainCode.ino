#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int VRx = A0;
const int VRy = A1;
const int buzzer = 8;
const int buttonPin = 7;
const int SWPin = 10;

int cursorX = 0;
int cursorY = 0;
int targetX;
int targetY;
int score = 0;
int tab = 0;

int buzzerTone = 1000;
int toneIndex = 0;
int tones[] = {800, 1000, 1200, 1500};

int lastSWState = HIGH; 

void setup() {

  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(SWPin, INPUT_PULLUP);
  randomSeed(analogRead(A5));

  showTab();
}

void loop() {
  // Switch tabs with external button
  if (digitalRead(buttonPin) == LOW) {
    delay(300); 
    tab = (tab + 1) % 3;
    showTab();
  }

  // Switch buzzer tone using joystick button
  int swState = digitalRead(SWPin);
  if (swState == LOW && lastSWState == HIGH) {
    toneIndex = (toneIndex + 1) % (sizeof(tones) / sizeof(tones[0]));
    buzzerTone = tones[toneIndex];
    delay(300); 
  }
  lastSWState = swState;

  // Only play game when on tab 0
  if (tab == 0) {
    playGame();
  }
}

void showTab() {
  lcd.clear();

  if (tab == 0) {
    placeNewTarget();
    lcd.setCursor(targetX, targetY);
    lcd.print("X");
    lcd.setCursor(cursorX, cursorY);
    lcd.print("*");
  }

  else if (tab == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Score Tab");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
  }

  else if (tab == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Move * to hit X");
    lcd.setCursor(0, 1);
    lcd.print("SWBtn=Sound Btn");
  }
}

void playGame() {
  int xVal = analogRead(VRx);
  int yVal = analogRead(VRy);

  // Horizontal movement
  if (xVal > 700 && cursorX < 15) {
    lcd.setCursor(cursorX, cursorY);
    lcd.print(" ");
    cursorX++;
    lcd.setCursor(cursorX, cursorY);
    lcd.print("*");
    delay(150);
  }
  else if (xVal < 300 && cursorX > 0) {
    lcd.setCursor(cursorX, cursorY);
    lcd.print(" ");
    cursorX--;
    lcd.setCursor(cursorX, cursorY);
    lcd.print("*");
    delay(150);
  }

  // Vertical movement
  if (yVal > 700 && cursorY < 1) {
    lcd.setCursor(cursorX, cursorY);
    lcd.print(" ");
    cursorY++;
    lcd.setCursor(cursorX, cursorY);
    lcd.print("*");
    delay(150);
  }
  else if (yVal < 300 && cursorY > 0) {
    lcd.setCursor(cursorX, cursorY);
    lcd.print(" ");
    cursorY--;
    lcd.setCursor(cursorX, cursorY);
    lcd.print("*");
    delay(150);
  }

  // Check for target hit
  if (cursorX == targetX && cursorY == targetY) {
    tone(buzzer, buzzerTone, 200);
    score++;

    lcd.setCursor(cursorX, cursorY);
    lcd.print("*"); // Keep cursor visible

    placeNewTarget();
    lcd.setCursor(targetX, targetY);
    lcd.print("X");

    delay(300);
  }
}

void placeNewTarget() {
  do {
    targetX = random(0, 16);
    targetY = random(0, 2);
  } while (targetX == cursorX && targetY == cursorY);
}
