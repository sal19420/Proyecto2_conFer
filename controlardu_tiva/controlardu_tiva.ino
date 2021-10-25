
char com2;                         // Variable para la comunicación serial
#define PU  PB_2              // arriba
#define PD  PB_3                 // abajo
#define PA  PB_4                 // ataque
char zero = 48;
char uno = 49;
char dos = 50; 
char tres = 51;
#define START  PB_5                  // start

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
  Serial2.begin(9600);
  Serial2.setTimeout(50);
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
        FLAG7 = 1;                       // Bandera para el antirrebote
        
          }
    else{
        if(FLAG7 == 1){                  // Si la FLAG está encendida entonces apgarla
           FLAG7 = 0;                    // Apagar bandera   
           Serial.write(zero);
           


           INICIO = 0;
           }
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
        FLAG = 1;                       // Bandera para el antirrebote
        
          }
    else{
        if(FLAG == 1){                  // Si la FLAG está encendida entonces apgarla
           FLAG = 0;                    // Apagar bandera   
          
           Serial.write(uno);             // Se envía el dato ASCII = 1

             
           }
        }


    if (digitalRead(PD) == HIGH){   // Segundo botón: Botón especial
        FLAG2 = 1;                      // Bandera para el antirrebote
        
          }
    else{
        if(FLAG2 == 1){                 // Si la FLAG está encendida entonces apgarla
           FLAG2 = 0;                   // Apagar bandera   
           
           Serial.write(dos);             // Se envía el dato ASCII = 2
                     
         
           }
        }


    if (digitalRead(PA) == HIGH){   // Tercer botón: EL otro especial
        FLAG3 = 1;                      // Bandera para el antirrebote
        
          }
    else{
        if(FLAG3 == 1){                 // Si la FLAG está encendida entonces apgarla
           FLAG3 = 0;                   // Apagar bandera   
          
           Serial.write(tres);            // Se envía el dato ASCII = 3

           
           }
        }

    }
}
