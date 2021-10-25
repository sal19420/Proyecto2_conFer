
char com2;                         // Variable para la comunicación serial
const int PU = 2;                // arriba
const int PD  =3 ;                // abajo
const int  PA  =4 ;                // ataque
const int START  =5;                  // start

int FLAG = 0;                         // Banderas para arriba
int FLAG2 = 0;                        // Banderas para abajo
int FLAG3 = 0;                        // Banderas para ataque
int FLAG7 = 0;


unsigned char INICIO = 1;             // Flag para el Inicio


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
}

//*****************************************************************************************
//                               L O O P   P R I N C I P A L
//*****************************************************************************************
void loop() {
    delay(10);

    if (digitalRead(START) == HIGH){     // Primer botón
        Serial.write(0);   
        INICIO = 0;
        
          }
   

//                 P R I M E R   J U G A D O R

    if (INICIO == 0){                   // Si no se precionó START entonces no comienza
    if (Serial.available()){                      
        com2 = Serial.read();                   // Guardar lo leído en Message
        Serial.write(com2);                     // Escribir lo que se recibe
        delay(4);
        if(com2 == 49){                          // Recibe un 1
        INICIO = 1; 
    }
    
    }

    
    if (digitalRead(PU) == HIGH){   // Primer botón: primer jugador
        Serial.write(1);                     
        
    }


    if (digitalRead(PD) == HIGH){   // Segundo botón: Botón especial
          Serial.write(2);             // Se envía el dato ASCII = 2
          }
   


    if (digitalRead(PA) == HIGH){   // Tercer botón: EL otro especial
       Serial.write(3);            // Se envía el dato ASCII = 3
        
          }
  

    }
}
