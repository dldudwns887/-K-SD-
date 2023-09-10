#include "RTClib.h"
#include <SoftwareSerial.h>
#include "kproject_gfx.h"
#include <SD.h>
#include <SPI.h>
// A는 이걸루
RTC_DS3231 rtc;

int val = A0;
int Value = 0;
int detail = 0;
const int arraysize = 50;
int A[arraysize];
int maxdetail(int arr[]);
int max = 0;
int count = -1;
File myFile;
SoftwareSerial soft(2, 3);

kproject_gfx display(soft, 64, 32);

void setup() {
  Serial.begin(9600);
  soft.begin(9600);
  display.begin();
  display.change_mode(0);
  display.fillRect(0, 0, 64, 16, KPROJECT_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(KPROJECT_YELLOW);
  display.print_string(" dB: ");
  display.setTextColor(KPROJECT_MAGENTA);
  display.setCursor(0, 16);
  display.print_string("상태:");
  display.refresh();
  pinMode(val, INPUT);

  // SD 카드 초기화
 while (!SD.begin(4)) {  // CS 핀은 4번 핀에 연결됨
    Serial.println("SD 카드 초기화 실패! 재시도 중...");
    delay(1000); // 1초 대기 후 재시도
  }

  Serial.println("SD 카드 초기화 성공!");

  rtc.begin(); // RTC 초기화
  if (rtc.lostPower()) {
    Serial.println("RTC 전원 손실, 시간 설정 중...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  Value = analogRead(val);
  detail = map(Value, 0, 200, 30, 80);

  display.fillRect(38, 0, 64, 16, KPROJECT_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(KPROJECT_YELLOW);
  display.print_string(" dB: ");
  display.setCursor(38, 0);
  display.setTextColor(KPROJECT_YELLOW);
  display.print_string(String(detail).c_str());

  display.fillRect(38, 16, 64, 32, KPROJECT_BLACK);
  display.setTextColor(KPROJECT_MAGENTA);
  display.setCursor(0, 16);
  display.print_string("상태:");
  display.setCursor(38, 16);

  count++;
  A[count] = detail;

  if (count == arraysize - 1) {
    count = -1;
    max = maxdetail(A);
    DateTime now = rtc.now();

    myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
      Serial.print(now.year(),DEC);
      Serial.print(' ');
      Serial.print(now.month(),DEC);
      Serial.print(' ');
      Serial.print(now.day(),DEC);
      Serial.print(' ');
      Serial.print(now.hour(),DEC);
      Serial.print(' ');
      Serial.print(now.minute(),DEC);
      Serial.print(' ');
      Serial.print(now.second(),DEC);
      Serial.print(' ');
      Serial.print(max);
      Serial.print(' ');
      Serial.println('A');

      myFile.print(now.year(), DEC);
      myFile.print(' ');
      myFile.print(now.month(), DEC);
      myFile.print(' ');
      myFile.print(now.day(), DEC);
      myFile.print(' ');
      myFile.print(now.hour(), DEC);
      myFile.print(' ');
      myFile.print(now.minute(), DEC);
      myFile.print(' ');
      myFile.print(now.second(), DEC);
      myFile.print(' ');
      myFile.print(max+10);
      myFile.print(' ');
      myFile.println("A");
      
      myFile.close();
    } else {
      Serial.println("data.txt 파일 열기 실패");
    }
  }

  if (max = 30) {
    display.setTextColor(KPROJECT_GREEN);
    display.print_string("^-^");
  } else if (max >= 31 && max <= 60) {
    display.setTextColor(KPROJECT_ORANGE);
    display.print_string("'-'");
  } else {
    display.setTextColor(KPROJECT_RED);
    display.print_string("-_-");
  }
  
  display.refresh();
}

int maxdetail(int arr[]) {
  int maxVal = arr[0];

  for (int i = 1; i < arraysize; i++) {
    if (arr[i] > maxVal) {
      maxVal = arr[i];
    }
  }

  return maxVal;
}
