#include <VescUart.h>
#define _INTERACTIVE_ 0

VescUart UART;
const float DUTY_MAX = 0.15;
float duty = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(19200);
  
  while (!Serial) {;}

  UART.setSerialPort(&Serial2);
}

void loop() {
  if ( UART.getVescValues() ) {
#if _INTERACTIVE_
    String command = Serial.readStringUntil('\n');
    float cmd = command.toFloat();
#else
    String command = "";
    float cmd = 0.02;
#endif
    if (cmd > DUTY_MAX) {
      cmd = DUTY_MAX;
    } else if (cmd < -DUTY_MAX) {
      cmd = -DUTY_MAX;
    }
    Serial.print("[command] VESC cmd:");
    Serial.println(cmd);
    // setDutyと併用する必要はないはず
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

    if (duty == 0) {
      // 起動直後からコマンドが来るまで
      Serial.println("[setDuty] brake");
      UART.setDuty(0);
    }

      // 入力タイムアウトをした場合 or 入力がIntではない場合、何もしない
#if _INTERACTIVE_
    if (command == "" && cmd == 0) {
      Serial.println("[setDuty] repeat");
      // 常時入力する必要はないはず？
      Serial.print("[command keep] VESC duty:");
      Serial.println(duty);
      UART.setDuty(duty);
#else
    if (cmd == 0) {
      Serial.println("[setDuty] repeat");
#endif
      return;
    }
    else {
      // 入力があった場合、入力値をそのまま、setDutyする
      Serial.println("[setDuty] update");
      duty = cmd;
      Serial.print("[command new] VESC duty:");
      Serial.println(duty);
      UART.setDuty(duty);
    }
  }
  else
  {
    Serial.println("Failed to get VESC data!");
  }
  delay(50);
}
