// https://rasino.tistory.com/tag/MPU6050%20Roll%20Pitch%20Yaw%20%EA%B0%92data%20%EA%B5%AC%ED%95%98%EA%B8%B0%20%EC%B6%9C%EB%A0%A5%ED%95%98%EA%B8%B0
//    Roll과 Pitch 각도 구하기 실습  》                                               */
//  가속도 센서만 가지고 롤(Roll)과 피치(Pitch)를 구하는 예제입니다.                          
//  본 코드는 드론과 같은 움직임이 심한 경우가 아닌 비교적 정적인 상태에서 안정된 값을 얻을 수 있으며,
//  복잡한 외부라이브러리를 사용하지 않아 코드가 심플해서 파악후 적용하기가 쉽습니다.
/* 아래 코드관련 실습에 대한 설명과 회로도 및 자료는 https://rasino.tistory.com/ 에 있습니다    */

double getAngleXY() {
  getData();  
  // 삼각함수를 이용한 롤(Roll)의 각도 구하기 
  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
  angleAcX *= RADIAN_TO_DEGREE;
  // 삼각함수를 이용한 피치(Pitch)의 각도 구하기
  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
  // angleAcZ값(Yaw)은 아래의 삼각함수 공식은 있으나, 가속도 센서만 이용해서는 원하는 데이터를 얻을 수 없어 생략
// angleAcZ = atan(sqrt(pow(AcX, 2) + pow(AcY, 2)) / AcZ );
// angleAcZ *= RADIAN_TO_DEGREE;
}

void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);   // I2C 통신용 어드레스(주소)
  Wire.write(0x6B);    // MPU6050과 통신을 시작하기 위해서는 0x6B번지에    
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);   // AcX 레지스터 위치(주소)를 지칭합니다
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  // AcX 주소 이후의 14byte의 데이터를 요청
  AcX = Wire.read() << 8 | Wire.read(); //두 개의 나뉘어진 바이트를 하나로 이어 붙여서 각 변수에 저장
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}
