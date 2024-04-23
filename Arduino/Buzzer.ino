
void Buzzer(){
  for (int i = 0; i < 2; i++) {
    ledcWriteTone(PWM_CHANNEL0, 2000);
    delay(500);
    ledcWriteTone(PWM_CHANNEL0, 0); // Turn off the tone
    delay(500);
  }
}