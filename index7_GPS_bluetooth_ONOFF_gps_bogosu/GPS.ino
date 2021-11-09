void result() { 
  GPS_Semaphore = false;
  if(targetStr.equals(str.substring(1, 6))){
    gps_a++;

    int first = str.indexOf(",");
    int two = str.indexOf(",", first+1);
    int three = str.indexOf(",", two+1);
    int four = str.indexOf(",", three+1);
    int five = str.indexOf(",", four+1);

    // Lat과 Long 위치에 있는 값들을 index로 추출

    String Lat = str.substring(two+1, three);
    String Long = str.substring(four+1, five);

    // Lat의 앞값과 뒷값을 구분

    String Lat1 = Lat.substring(0, 2);
    String Lat2 = Lat.substring(2);

    // Long의 앞값과 뒷값을 구분

    String Long1 = Long.substring(0, 3);
    String Long2 = Long.substring(3);

    // 좌표 계산.
    LatF = Lat1.toDouble() + Lat2.toDouble()/60;
    LongF = Long1.toFloat() + Long2.toFloat()/60;
  
  }  
}
