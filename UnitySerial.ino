namespace {
const float ratio = 0.9;
float x, y;
float prex, prey;
}

void setup()
{
  Serial.begin(9600);
  pinMode(12, OUTPUT); //GREEN LED
  pinMode(13, OUTPUT); //RED LED
  pinMode(7, OUTPUT); //リレー
  pinMode(1, OUTPUT); //信号用ピン 6番ピン
  pinMode(2, OUTPUT); //信号用ピン 5番ピン
}

void readAccelerometer()
{
  //平滑化フィルタ
  x = ratio * prex + (1 - ratio) * analogRead(3);
  y = ratio * prey + (1 - ratio) * analogRead(4);
  prex = x;
  prey = y;

  //センサ値を度に変換
  float rotateX = (x - 277) / 2.48 - 90;
  float rotateY = (y - 277) / 2.48 - 90;

  //シリアル送信
  Serial.print(rotateX);
  Serial.print(",");
  Serial.print(rotateY);
  Serial.println("");
}

void Receive()
{
  if ( Serial.available() ) {
    char mode = Serial.read();

    //Unityから送られてきた文字によって動作を変える
    switch (mode) {
      case 'H' :
        digitalWrite(7, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(13, HIGH);
        break;
      case 'L' :
        digitalWrite(7, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);
        break;
      case '0' :
        digitalWrite(7, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(12, HIGH);
        break;
    }

  }
}

void Motor()
{
  //モーターの強さ指定
  int val = 255; //-255~255の値にする

  if (val > 0) {
    // 正回転
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    //valが大きいほど出力値も大きくなる
    analogWrite( 3 , val ); //出力値:1~255
  }
  else if (val < 0) {
    // 負回転
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    //valが大きいほど出力値も大きくなる
    analogWrite( 3 , -val ); //出力値:1~255
  }
  else {
    // 静止
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
  }
}

void loop()
{
  readAccelerometer();
  Receive();
  Motor();
}
