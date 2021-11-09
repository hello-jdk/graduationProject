#include<Wire.h> // MPU I2C통신을 위한
#include<SoftwareSerial.h> //GPS
#include "Adafruit_VL53L0X.h" //거리센서

//1 MPU6050
//2 GPS
//3 VL53L0X 거리센서
//4 HC-06
//10 buzzer

//const----------------------------------------------------------------------------
//1
const int  MPU_ADDR = 0x68;    // I2C통신을 위한 MPU6050의 주소
int16_t  AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;   // 가속도(Acceleration)와 자이로(Gyro)
double  angleAcX;
double  angleAcY;

// double angleAcZ; 필요하다면 추가
const double  RADIAN_TO_DEGREE = 180 / 3.14159;

//2
SoftwareSerial gpsSerial(5,4);
char c = ""; // Wn 인지 구분 및 str에 저장.
String str = ""; // \n 전까지 c 값을 저장.
String targetStr = "GPGGA"; // str의 값이 NMEA의 GPGGA 값인지 타겟
double LatF;
double LongF;

double LatF_rs;
double LongF_rs;

int gps_a = 0;
int gps_b = 0;

//3
Adafruit_VL53L0X lox = Adafruit_VL53L0X(); 
int VL53_ARRD;
int rang_num;

//4
SoftwareSerial bluetooth(3,2);

//9 ON OFF
bool power = true;
int clock;
int botton = 11; 
int LED =13;

//10
int buzzer = 12;


bool GPS_Semaphore = true;

//setup--------------------------------------------------------------------------
void setup() {

  Serial.begin(9600);

//1
  initSensor();
  if(Wire.endTransmission()==0){              //Found address: 104 (0x68)
    Serial.println(F("MPU6050 연결완료"));
  }else{
    Serial.println(F("MPU6050연결실패"));
  }  

//2
  gpsSerial.begin(9600);                      // tx,rx = 5,4
  Serial.println(F("gps start"));


//3

  if(!lox.begin()){                           // Found address: 41 (0x29)
    Serial.println(F("VL53 거리센서 연결실패"));
  }else{
    Serial.println(F("VL53 거리센서 연결완료"));
  }

//4
  bluetooth.begin(9600);                      // tx,rx = 3,2


//9
  pinMode(botton, INPUT);
  pinMode(LED, OUTPUT);
  clock = digitalRead(botton);

//10 
  pinMode(buzzer,OUTPUT);                     // buzzer단자를 핀모드로 설정
}

//loop----------------------------------------------------------------------------
void loop() {
  int read = digitalRead(botton);

  if(read != clock){
    if(read == HIGH){
        if(power == true){
           power = false;
        }else{
           power = true;
        }
     }    
  }

  if(power){
    digitalWrite(LED,HIGH);
  }else{
    digitalWrite(LED,LOW);
  }


  //2
   gpsSerial.listen();
   if(gpsSerial.available()) 
    {
      delay(300);
      while(GPS_Semaphore)
      {
        c=gpsSerial.read();
        Serial.print(c);
        if(c == '\n')
          { 
           result();
            str = "";
          }else
          { 
            str += c;
          }
       }

       if(gps_a != gps_b){
       Serial.print(F("위도: "));
       Serial.print(LatF, 15);
       Serial.print(F(", 경도 :"));
       Serial.println(LongF, 15);

       gps_b = gps_a;
       }
    }
  
  //1
  //자이로 roll각도(yx,기울기)와 pitch각도(yz,앞뒤)를 계산 
  getAngleXY(); 
  Serial.print(F("앞뒤 : "));
  Serial.print(angleAcX);
  Serial.print(F("\t\t 양옆 : "));
  Serial.println(angleAcY);

  
  //3
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); 
  rang_num = measure.RangeMilliMeter;
  if (rang_num !=0) {  
    Serial.print(F("길이(mm): ")); Serial.println(rang_num);     
  } else {
    rang_num = 8140;
    Serial.println(F(" 허용 범위 초과 (최대 50cm) "));       //허용 각도를 벗어나면 "out of range"  시리얼모니터에 출력
  }
  
  GPS_Semaphore = true;
  
  // 각도중 60도 이상 변화 or 10cm 이내로 접근시 2000hz 소리 그 이외에는 부저 끄기
  if(angleAcX > 60 || angleAcX < -60 || angleAcY > 60 || angleAcY < -60 || rang_num< 100){
  if(power){    
  tone(buzzer,2000);
      if(angleAcX > 60 || angleAcX < -60 || angleAcY > 60 || angleAcY < -60 ){
         bluetooth.listen();
         bluetooth.print(LatF,15);
         bluetooth.print(",");
         bluetooth.print(LongF,15);
         bluetooth.print("\n");
         delay(3000);
      }
  }else{
    noTone(buzzer);
  }
  }else{ 
  noTone(buzzer);
  }

  //delay(200);
}
