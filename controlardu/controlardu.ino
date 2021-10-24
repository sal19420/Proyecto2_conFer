
char com;                         // Variable para la comunicación serial
const int PU = 2;                 // arriba
const int PD = 3;                 // abajo
const int PA = 4;                 // ataque

const int START = 5;                  // start

int FLAG = 0;                         // Banderas para arriba
int FLAG2 = 0;                        // Banderas para abajo
int FLAG3 = 0;                        // Banderas para ataque
int FLAG7 = 0;


unsigned char INICIO = 1;             // Flag para el Inicio
const int LED1 = 11;
const int LED2 = 12;
const int LED3 = 13;
const int LEDSTART = 6;

//******************************************************************************************
//                                     S E T   U P 
//******************************************************************************************
void setup() {
  Serial.begin(9600);                 // Inicializar C. serial a 9600 bits per second
  delay(100);
  pinMode(PU, INPUT_PULLUP);      // Hacer el pin un input pull up  
  pinMode(PD, INPUT_PULLUP);        
  pinMode(PA, INPUT_PULLUP);      
  pinMode(START, INPUT_PULLUP);       
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LEDSTART, OUTPUT);


}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
void loop() {
    delay(10);

    if (digitalRead(START) == HIGH){     // Primer botón
        FLAG7 = 1;                       // Bandera para el antirrebote
        digitalWrite(LEDSTART, LOW);
          }
    else{
        if(FLAG7 == 1){                  // Si la FLAG está encendida entonces apgarla
           FLAG7 = 0;                    // Apagar bandera   
           Serial.write(0);
           digitalWrite(LEDSTART, HIGH);


           INICIO = 0;
           }
       }

//                 P R I M E R   J U G A D O R

    if (INICIO == 0){                   // Si no se precionó START entonces no comienza
    if (Serial.available()){                      
        com = Serial.read();                   // Guardar lo leído en Message
        Serial.write(com);                     // Escribir lo que se recibe
        delay(4);
        if(com == 49){                          // Recibe un 1
        INICIO = 1; 
    }
    
    }

    
    if (digitalRead(PU) == HIGH){   // Primer botón: primer jugador
        FLAG = 1;                       // Bandera para el antirrebote
        digitalWrite(LED1, LOW);
          }
    else{
        if(FLAG == 1){                  // Si la FLAG está encendida entonces apgarla
           FLAG = 0;                    // Apagar bandera   
           digitalWrite(LED1, HIGH);
           Serial.write(1);             // Se envía el dato ASCII = 1

             
           }
        }


    if (digitalRead(PD) == HIGH){   // Segundo botón: Botón especial
        FLAG2 = 1;                      // Bandera para el antirrebote
        digitalWrite(LED2, LOW);
          }
    else{
        if(FLAG2 == 1){                 // Si la FLAG está encendida entonces apgarla
           FLAG2 = 0;                   // Apagar bandera   
           digitalWrite(LED2, HIGH);
           Serial.write(2);             // Se envía el dato ASCII = 2
                     
         
           }
        }


    if (digitalRead(PA) == HIGH){   // Tercer botón: EL otro especial
        FLAG3 = 1;                      // Bandera para el antirrebote
        digitalWrite(LED3, LOW);
          }
    else{
        if(FLAG3 == 1){                 // Si la FLAG está encendida entonces apgarla
           FLAG3 = 0;                   // Apagar bandera   
           digitalWrite(LED3, HIGH);
           Serial.write(3);            // Se envía el dato ASCII = 3

           
           }
        }

    }
}
