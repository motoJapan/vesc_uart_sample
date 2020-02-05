#include <VescUart.h>

VescUart UART;
float current = 1.5;
int rpm = 0;

void setup() {
  Serial.begin(115200);
  //Serial2.begin(115200);
  Serial2.begin(19200);
  
  while (!Serial) {;}

  UART.setSerialPort(&Serial2);
}

void loop() {
  if ( UART.getVescValues() ) {
    String command = Serial.readStringUntil('\n');
    int cmd = command.toInt();
    Serial.print("[command] VESC cmd:");
    Serial.println(cmd);
    // setRPMと併用する必要はないはず
    // UART.setCurrent(current);
    // UART.setBrakeCurrent(current);
    Serial.print("VESC rpm:");
    Serial.println(UART.data.rpm);
    Serial.print("VESC inpVoltage:");
    Serial.println(UART.data.inpVoltage);
    Serial.print("VESC ampHours:");
    Serial.println(UART.data.ampHours);
    Serial.print("VESC tachometerAbs:");
    Serial.println(UART.data.tachometerAbs);
    // Serial.print("VESC [set] current:");
    // Serial.println(current);
    Serial.print("VESC avgMotorCurrent:");
    Serial.println(UART.data.avgMotorCurrent);
    Serial.print("VESC avgInputCurrent:");
    Serial.println(UART.data.avgInputCurrent);
    Serial.print("VESC dutyCycleNow:");
    Serial.println(UART.data.dutyCycleNow);

    if (rpm == 0) {
      // 入力がない間は0を設定
      Serial.println("[setRPM] brake");
      UART.setRPM(0);
    }

    if (cmd == 0) {
      // 入力タイムアウトをした場合 or 入力がIntではない場合、何もしない
      Serial.println("[setRPM] repeat");
      return; // これは一応うまく動いている
      // 常時入力する必要はないはず？
      //Serial.print("[command keep] VESC rpm:");
      //Serial.println(rpm);
      //UART.setRPM(rpm);
      //return;
    }
    else {
      // 入力があった場合、入力値をそのまま、setRPMする
      Serial.println("[setRPM] update");
      rpm = cmd;
      Serial.print("[command new] VESC rpm:");
      Serial.println(rpm);
      UART.setRPM(rpm);
    }
  }
  else
  {
    Serial.println("Failed to get VESC data!");
  }
  delay(50);
}
