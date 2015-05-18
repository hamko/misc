#define LED_PIN 13
 
void setup(){
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}
 
void loop(){
  int inputchar = Serial.read();
 
  if(inputchar != -1 ){
    switch(inputchar){
      case 'o':
        Serial.print("LED ON\n");
        digitalWrite(LED_PIN, HIGH);
        break;
      case 'p':  
        Serial.print("LED OFF\n");
        digitalWrite(LED_PIN, LOW);
        break;
    }
  }
}
