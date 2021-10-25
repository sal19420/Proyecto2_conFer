
char com;
char com2;                
#define BP PB_5
int buzzerPin = BP;

//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************

void setup() {
  Serial.begin(9600);                           // Inicializar C. serial a 9600 bits per second
  Serial3.begin(9600);                          // Inicializar C. serial a 9600 bits per second
  Serial2.begin(9600);
  
  Serial2.setTimeout(50);
  Serial3.setTimeout(50);                        // T para la transf. de datos
  delay(100);
  pinMode(PB_2, OUTPUT);                        // Pin para el led
  pinMode(PE_0, OUTPUT);                        // Pin para el led
  pinMode(PF_0, OUTPUT);                        // Pin para el led
  pinMode(PB_3, OUTPUT);                        // Pin para el led
  pinMode(PE_1, OUTPUT);                        // Pin para el led
  pinMode(PF_1, OUTPUT);                        // Pin para el led
  pinMode(buzzerPin,OUTPUT);
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
void loop() {
         
  if (Serial3.available()){                      
    com = Serial3.read();                   // Guardar lo leído en Message
    Serial.write(com);                     // Escribir lo que se recibe
 
  delay(4);
  if(com == 0){                          // Recibe un 1
      digitalWrite(PB_2, HIGH);                 // Se enciende el Led
      digitalWrite(PF_0, HIGH);
      digitalWrite(PE_0, HIGH);
      song1();
      delay(10);
      digitalWrite(PB_2, LOW);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
      noTone(buzzerPin);
  }
    if(com == 1){                          // Recibe un 1
      digitalWrite(PB_2, HIGH);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
      song();
      delay(10);
      digitalWrite(PB_2, LOW);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
      noTone(buzzerPin);
  }

    if(com == 2){                          // Recibe 0
      digitalWrite(PE_0, HIGH);                  // Se apaga el LED
      digitalWrite(PF_0, LOW);
      digitalWrite(PB_2, LOW);
      song();
      delay(10);
      digitalWrite(PB_2, LOW);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
      noTone(buzzerPin);
  }

   if(com == 3){                          // Recibe 0
      digitalWrite(PF_0, HIGH);                  // Se apaga el LED
      digitalWrite(PE_0, LOW);
      digitalWrite(PB_2, LOW); 
      song();
      delay(10);
      digitalWrite(PB_2, LOW);                 // Se enciende el Led
      digitalWrite(PF_0, LOW);
      digitalWrite(PE_0, LOW);
      noTone(buzzerPin);
      
  }
   }
   if (Serial2.available()){                      
    com2 = Serial2.read();                   // Guardar lo leído en Message
    Serial.write(com2);                     // Escribir lo que se recibe
 
  delay(4);
  if(com2 == 0){                          // Recibe un 1
      digitalWrite(PB_3, HIGH);                 // Se enciende el Led
      digitalWrite(PF_1, HIGH);
      digitalWrite(PE_1, HIGH);
      song1();
      delay(10);
      digitalWrite(PB_3, LOW);                 // Se enciende el Led
      digitalWrite(PF_1, LOW);
      digitalWrite(PE_1, LOW);
      noTone(buzzerPin);
  }
    if(com2 == 1){                          // Recibe un 1
      digitalWrite(PB_3, HIGH);                 // Se enciende el Led
      digitalWrite(PF_1, LOW);
      digitalWrite(PE_1, LOW);
      song();
      delay(10);
      digitalWrite(PB_3, LOW);                 // Se enciende el Led
      digitalWrite(PF_1, LOW);
      digitalWrite(PE_1, LOW);
      noTone(buzzerPin);
  }

    if(com2 == 2){                          // Recibe 0
      digitalWrite(PE_1, HIGH);                  // Se apaga el LED
      digitalWrite(PF_1, LOW);
      digitalWrite(PB_3, LOW);
      song();
      delay(10);
      digitalWrite(PB_3, LOW);                 // Se enciende el Led
      digitalWrite(PF_1, LOW);
      digitalWrite(PE_1, LOW);
      noTone(buzzerPin);
  }

   if(com2 == 3){                          // Recibe 0
      digitalWrite(PF_1, HIGH);                  // Se apaga el LED
      digitalWrite(PE_1, LOW);
      digitalWrite(PB_3, LOW); 
      song();
      delay(10);
      digitalWrite(PB_3, LOW);                 // Se enciende el Led
      digitalWrite(PF_1, LOW);
      digitalWrite(PE_1, LOW);
      noTone(buzzerPin);
      
  }
   }

   
}
void song(){
  
//  tone(buzzerPin, 233);
//  delay(150);
//  noTone(buzzerPin);
//
//  tone(buzzerPin, 131);
//  delay(150);
//  noTone(buzzerPin);
//
//  tone(buzzerPin, 147);
//  delay(150);
//  noTone(buzzerPin);
//
//  tone(buzzerPin, 175);
//  delay(150);
//  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(150);
  noTone(buzzerPin);
}
void song1(){
  
  tone(buzzerPin, 233);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 131);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 147);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 175);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 233);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 262);
  delay(150);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(150);
  noTone(buzzerPin);
}
