//***************************************************************************************************************************************
/* Código de ejemplo de videojuego:
   Detección de colisiones con objetos rectangulares.
   Emplea estructuras para definir clases e instanciar objetos.
   Incorpora actualización de cuadros cada 42ms ( aprox. 24fps).
   Movimiento de sprite utilizando botones integrados.
   Basado en librería para el uso de la pantalla ILI9341 en modo 8 bits
   Autor: Diego Morales
   IE3027: Electrónica Digital 2 - 2021
*/
//***************************************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);
bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

//***************************************************************************************************************************************
// Variables y arrays
//***************************************************************************************************************************************
extern uint8_t nave_sprite[]; // cargar bitmap desde memoria flash
extern uint8_t alien_sprite[];
extern uint8_t vida_sprite[];
extern uint8_t disparo_nave_sprite[];
extern uint8_t disparo_alien_sprite[];

struct Sprite { // estructura para sprites
  int x; // posicion x
  int y; // posicion y
  int width; // ancho de bitmap
  int height; // altura de bitmap
  int columns; // columna sprite sheet
  int index; // indice sprite sheet
  int flip; // voltear imagen
  int offset; // desfase
} nave, alien;

struct Rectangle { // estructura para rectangulos
  int x; // posicion x
  int y; // posicion y
  int width; // ancho de rectángulo
  int height; // altura de rectángulo
  int color; // color de relleno
} rect;

bool rectUp = false; // dirección rectángulo
bool collision = false; // detección de colisión

unsigned long previousMillis = 0;
const long interval = 42;

char side, side2;
int vida1, vida2;

int m1, m2;
int ataque_activo1, ataque_activo2;
int yataque, yataque2;

int colision1, colision2;

char com;
char com2;
#define BP PA_6
int buzzerPin = BP;

char up1;
char up2;

char down1;
char down2;

char ataque1;
char ataque2;

char start1;
char start2;

char comienzo;
//***************************************************************************************************************************************
// Initialization
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);

  Serial.begin(9600);                           // Inicializar C. serial a 9600 bits per second
  Serial3.begin(9600);                          // Inicializar C. serial a 9600 bits per second
  Serial2.begin(9600);

  Serial2.setTimeout(50);
  Serial3.setTimeout(50);                        // T para la transf. de datos
  delay(100);

  pinMode(buzzerPin, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP); // botones integrados con como entrada pull-up
  pinMode(PUSH2, INPUT_PULLUP);

  nave.x = 0;
  nave.y = 150;
  nave.width = 81;
  nave.height = 36;
  nave.columns = 1;
  nave.index = 0;
  nave.flip = 0;
  nave.offset = 0;

  alien.x = 269;
  alien.y = 150;
  alien.width = 51;
  alien.height = 45;
  alien.columns = 1;
  alien.index = 0;
  alien.flip = 0;
  alien.offset = 0;

  rect.x = 140;
  rect.y = 80;
  rect.width = 40;
  rect.height = 40;
  rect.color = 0x07E0;

  //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
  FillRect(0, 0, 320, 240, 0x0400);

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  String text1 = "Defender Battle";
  LCD_Print(text1, 50, 110, 2, 0xffff, 0x0400);

  String text2 = "Press Start";
  LCD_Print(text2, 80, 130, 2, 0xffff, 0x0400);

  comienzo = 1;

}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {
  unsigned long currentMillis = millis();

  //---------Comunicación Serial entre los controles y la Tiva-----------
  if (Serial3.available()) {
    com = Serial3.read();                   // Guardar lo leído en Message
    Serial.write(com);                     // Escribir lo que se recibe

    delay(4);
    if (com == 48) {                        // Recibe un 1
      start1 = 1;                 // Start1
      //song1();
      //noTone(buzzerPin);
    }
    if (com == 49) {                        // Recibe un 1
      up1 = 1;                 //Up1
      //song();
      //noTone(buzzerPin);
    }

    if (com == 50) {                        // Recibe un 2
      down1 = 1;                  //Down1
      //song();
      //noTone(buzzerPin);
    }

    if (com == 51) {                        // Recibe un 3
      ataque1 = 1;                  //Ataque1
      //song();
      //noTone(buzzerPin);

    }
  }

  if (Serial2.available()) {
    com2 = Serial2.read();                   // Guardar lo leído en Message
    Serial.write(com2);                     // Escribir lo que se recibe

    delay(4);
    if (com2 == 0) {                        // Recibe un 0
      start2 = 1;                 //Start2
      //song1();
      //noTone(buzzerPin);
    }
    if (com2 == 1) {                        // Recibe un 1
      up2 = 1;                 //Up2
      //song();
      //noTone(buzzerPin);
    }

    if (com2 == 2) {                        // Recibe un 2
      down2 = 1;                  //Down2
      //song();
      //noTone(buzzerPin);
    }

    if (com2 == 3) {                        // Recibe un 3
      ataque2 = 1;                  //Ataque2
      //song();
      //noTone(buzzerPin);

    }
  }


  if (start1 == 1 & start2 == 1) {

    if (comienzo == 1) {
      //*****************Configuraciones iniciales del juego********************
      //Se resetean los contadores de la vida
      FillRect(0, 0, 320, 240, 0x0000);
      vida2 = vida1 = 0;
      LCD_Sprite(0, 0, 130, 26, vida_sprite, 3, 0, 0, 0);
      LCD_Sprite(190, 0, 130, 26, vida_sprite, 3, 0, 1, 0);
    }


    if (vida1 < 3 & vida2 < 3) {
      // actualización de frame cada 42ms = 24fps
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        //--------Control del movimiento del J1 por medio de botones
        if (down1 == 1) { // modificación de atributos de sprite
          nave.y += 4;
          nave.index++;
          nave.index %= 3;
          side = 0;
          down1 = 0;
        }
        if (up1 == 1) {
          nave.y -= 4;
          nave.index++;
          nave.index %= 3;
          side = 1;
          up1 = 0;
        }

        //--------Control de movimiento del J2 por medio de botones---------
        if (down2 == 1) { // modificación de atributos de sprite
          alien.y += 4;
          alien.index++;
          alien.index %= 3;
          side2 = 0;
          down2 = 0;
        }
        if (up2 == 1) {
          alien.y -= 4;
          alien.index++;
          alien.index %= 3;
          side2 = 1;
          up2 = 0;
        }

        //----------------------Coloreo del resto del sprite en su frame anterior-------------------------
        if (side == 0) { // dependiendo de la dirección, se colorea resto del sprite del frame anterior
          FillRect(nave.x, nave.y - 4, nave.width, 4, 0x0000);
        }
        else {
          FillRect(nave.x, nave.y + nave.height, nave.width, 4, 0x0000);
        }

        if (side2 == 0) { // dependiendo de la dirección, se colorea resto del sprite del frame anterior
          FillRect(alien.x, alien.y - 4, alien.width, 4, 0x0000);
        }
        else {
          FillRect(alien.x, alien.y + alien.height, alien.width, 4, 0x0000);
        }

        //----------------Creación de los sprites en la pantalla TFT------------------
        LCD_Sprite(nave.x, nave.y, nave.width, nave.height, nave_sprite, nave.columns, nave.index, nave.flip, nave.offset);

        LCD_Sprite(alien.x, alien.y, alien.width, alien.height, alien_sprite, alien.columns, alien.index, alien.flip, alien.offset);

        //-----Control del ataque por medio de los botones-----------
        //Para cuando J1 ataca
        if (ataque1 == 1) {//Los botones estan config. como Pull-Ups
          yataque = nave.y;
          ataque_activo1 = 1;
          colision1 = 1;
          m1 = 81;
          ataque1 = 0;
        }
        if (ataque_activo1 == 1) {
          int anim3 = (m1 / 39) % 1;
          LCD_Sprite(m1, yataque, 39, 24, disparo_nave_sprite, 1, anim3, 0, 0);
          FillRect(m1 - 10, yataque, 10, 24, 0x0000);
          // a través de Vline se borra el rastro que deja el ataque
          m1 += 10;
          if (m1 < 320) {
            ataque_activo1 = 1;
          }
          else {
            ataque_activo1 = 0;
            colision1 = 0;
          }
        }

        if (colision1  == 1 ) {
          FillRect(m1, yataque, 39, 24, 0x0000);
          int h = alien.y - nave.y;
          int d = alien.x - nave.x;
          int r = m1 - alien.x;
          int hit = 0;
          if ((r >= 0) & (d >= 0))hit++;
          if ((((h < 39) & (d >= 0) & (h >= 0)) | (h <= 0) & (h > -24)) & (hit == 1))vida2++;
          LCD_Sprite(190, 0, 130, 26, vida_sprite, 3, vida2, 1, 0);

          if (hit == 1)colision1 = 0;
        }

        //Para cuando J2 ataca
        if (ataque2 == 1) {//Los botones estan config. como Pull-Ups
          yataque2 = alien.y;
          ataque_activo2 = 1;
          colision2 = 1;
          m2 = 269 - 26;
          ataque2 = 0;
        }
        if (ataque_activo2 == 1) {
          int anim2 = (m2 / 28) % 1;
          LCD_Sprite(m2, yataque2, 28, 26, disparo_alien_sprite, 1, anim2, 0, 0);
          FillRect(m2 + 11, yataque2, 10, 26, 0x0000);
          // a través de FillRect se borra el rastro que deja el ataque
          m2 -= 10;
          if (m2 > 28) {
            ataque_activo2 = 1;
          }
          else {
            ataque_activo2 = 0;
            colision2 = 0;
          }
        }

        if (colision2  == 1 ) {
          int h = alien.y - nave.y; //se obtiene la posicion en el eje Y de J2 respecto a J1
          int d = nave.x - alien.x; //se obtiene la posicion en el eje X de J1 respecto a J2
          int r = m2 - 81; //se obtiene la posicion en el eje X de J1 respecto al Ataque
          int hit2 = 0; //sirve para saber si el ataque impacto o no en J1
          if ((r <= 0) & (d <= 0)) {
            hit2++; //para acertar el ataque el jugador viendo hacia la izquierda
          }
          //ambas distancias tienen que ser negativas.
          if ((((h < 30) & (h >= 0)) | (h <= 0) & (h > -21)) & (hit2 == 1))vida1++; //para acertar el personaje atacado
          //tiene que estar como máximo 29 unidades arriba del ataque y 21 por debajo del ataque
          LCD_Sprite(0, 0, 130, 26, vida_sprite, 3, vida1, 0, 0); //el sprite de la vida cambia solo si se cumplen la condiciones ant.

          if (hit2 == 1) {
            colision2 = 0;
          }
        }

        comienzo = 0;

        if (vida1 == 3) {
          String textj1 = "Ganador J2";
          LCD_Print(textj1, 70, 110, 2, 0xffff, 0x0000);
          comienzo = 1;
          start1 = 0;
          start2 = 0;
          delay(1000);
          //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
          FillRect(0, 0, 320, 240, 0x0400);

          //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
          String text3 = "Defender Battle";
          LCD_Print(text3, 50, 110, 2, 0xffff, 0x0400);

          String text4 = "Press Start";
          LCD_Print(text4, 80, 130, 2, 0xffff, 0x0400);
        }

        if (vida2 == 3) {
          String textj2 = "Ganador J1";
          LCD_Print(textj2, 70, 110, 2, 0xffff, 0x0000);
          comienzo = 1;
          start1 = 0;
          start2 = 0;
          delay(1000);
          //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
          FillRect(0, 0, 320, 240, 0x0400);

          //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
          String text5 = "Defender Battle";
          LCD_Print(text5, 50, 110, 2, 0xffff, 0x0400);

          String text6 = "Press Start";
          LCD_Print(text6, 80, 130, 2, 0xffff, 0x0400);
        }

      }
    }
  }

}

//----------------------Musicon del Juego----------------------
/*void song(){

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
  }*/


//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      k = k - 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  }
  else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }
  }
  digitalWrite(LCD_CS, HIGH);
}

bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2);
}
