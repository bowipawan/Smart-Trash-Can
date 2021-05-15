String readString;

void setup() {
  Serial.begin(9600);
  Serial.println("serial test 0021"); // so I can keep track of what is loaded
}

void loop() {

  while (Serial.available()>0) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() >0) {
    Serial.println(readString);

    readString="";
  } 
}
