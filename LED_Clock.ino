int latchPin = 4;
int dataPin = 8;
int clockPin = 7;

#define SET_BTN A1
#define MINUS_BTN A2
#define PLUS_BTN A3

uint8_t sBtnPressed = 0;
uint8_t mBtnPressed = 0;
uint8_t pBtnPressed = 0;
unsigned long long sUnpressTimer;
unsigned long long pUnpressTimer;
unsigned long long mUnpressTimer;

int ora = 0;
int minut = 0;
int secunda = 0;
char state = 's';
char setState = 'h';
byte numbers[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};

unsigned long long now = 0;
unsigned long long stateTimer;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
}

void checkSBtn(){
  unsigned long long pressTimer;
  if ((digitalRead(SET_BTN) == LOW) && (sBtnPressed == 0)){
    sBtnPressed = 1;
    pressTimer = millis();
  }
  if ((digitalRead(SET_BTN) == HIGH) && (sBtnPressed == 1) && (millis() - pressTimer > 200))
    sBtnPressed = 0;
  if (sBtnPressed == 2 && (millis() - sUnpressTimer > 300))
    sBtnPressed = 0;
}

void checkMBtn(){
  unsigned long long pressTimer;
  if ((digitalRead(MINUS_BTN) == LOW) && (mBtnPressed == 0)){
    mBtnPressed = 1;
    pressTimer = millis();
  }
  if ((digitalRead(MINUS_BTN) == HIGH) && (mBtnPressed == 1) && (millis() - pressTimer > 200))
    mBtnPressed = 0;
  if (mBtnPressed == 2 && (millis() - mUnpressTimer > 300))
    mBtnPressed = 0;
}

void checkPBtn(){
  unsigned long long pressTimer;
  if ((digitalRead(PLUS_BTN) == LOW) && (pBtnPressed == 0)){
    pBtnPressed = 1;
    pressTimer = millis();
  }
  if ((digitalRead(PLUS_BTN) == HIGH) && (pBtnPressed == 1) && (millis() - pressTimer > 200))
    pBtnPressed = 0;
  if (pBtnPressed == 2 && (millis() - pUnpressTimer > 300))
    pBtnPressed = 0;
}

void displayMinute(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[minut%10]);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00001000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[minut/10]);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000100);
  digitalWrite(latchPin, HIGH);
}

void displayHour(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[ora%10]);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000010);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[ora/10]);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000001);
  digitalWrite(latchPin, HIGH);
}

void loop() {
  if (state == 's'){
    if (setState == 'h'){
      if (pBtnPressed == 1){
        ora += 1;
        if (ora > 23)
          ora = 0;
        pBtnPressed = 2;
        pUnpressTimer = millis();
      } // Plus ora
      if (mBtnPressed == 1){
        if (ora != 0)
          ora -= 1;
        else ora = 23;
        mBtnPressed = 2;
        mUnpressTimer = millis();
      } // Minus ora
      displayHour();
      if (sBtnPressed == 1 && (millis() - stateTimer > 300)){
        setState = 'm';
        sBtnPressed = 2;
        sUnpressTimer = millis();
      } // Trece in set minute
    }
    else if (setState == 'm'){
      if (pBtnPressed == 1){
        minut += 1;
        if (minut > 59)
          minut = 0;
        pBtnPressed = 2;
        pUnpressTimer = millis();
      } // plus minute
      if (mBtnPressed == 1){
        if (minut != 0)
          minut -= 1;
        else minut = 59;
        mBtnPressed = 2;
        mUnpressTimer = millis();
      } // minus minute
      displayMinute();
      if (sBtnPressed == 1){
        secunda = 0;
        state = 'd';
        setState = 'h';
        sBtnPressed = 2;
        sUnpressTimer = millis();
      } // trece in "display state"
    }
  }
  else if (state == 'd'){
    if (sBtnPressed == 1){
      setState = 'h';
      state = 's';
      stateTimer = millis();
    } // Trece in "set hour state" daca butonul 1 e apasat
    if (millis() - now >= 1000){
      now = millis();
      secunda += 1;
      if (secunda > 59){
        secunda = 0;
        minut += 1;
      }
      if (minut > 59){
        minut = 0;
        ora += 1;
      }
      if (ora > 23){
        ora = 0;
      }
      Serial.print("h ");
      Serial.println(ora);
      Serial.print("m ");
      Serial.println(minut);
      Serial.print("s ");
      Serial.println(secunda);
    } // Update ora
    displayMinute();
    displayHour();
  }
  checkSBtn();
  checkMBtn();
  checkPBtn();
  // Verifica ce butoane sunt apasate
}
