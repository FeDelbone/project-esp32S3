#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <RTClib.h>
#include <Preferences.h>
#include <ESP32Servo.h>

//Ligção dos Hardware
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
Preferences prefs;
Servo travaPorta;

//Tamanho do teclado no caso 16 teclas ou 4x4
const byte ROWS = 4;
const byte COLS = 4;
//Desenho do teclado
 char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
//definindo pinos do ESP32-S3 ROWS(linhas) e COLS(colunas)
byte rowPins[ROWS] = {10, 11, 12, 13};
byte colPins[COLS] = {14, 15, 16, 17};
//criação do objeta teclado 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Definição dos pinos do led buzzer e servo
const int LED_VERDE = 5;
const int LED_VERMELHO = 4; 
const int BUZZER = 6;
const int SERVO_PIN = 7;

// Ângulos do motor servo
const int ANGULO_FECHADO = 0;
const int ANGULO_ABERTO = 90;

// Variáveis da senha 
String senhaDigitada = ""; 
String senhaCorreta = "908070"; 
int totalAcessos = 0;

void setup() {
    //comunicação com o monitor serial
    Serial.begin(115200);
    delay(500);

    //define como os leds e o buzzer vão iniciar(desligado)
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_VERMELHO, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    //ativa o envio de pulsos PWM para controlar o motor
    travaPorta.attach(SERVO_PIN);
    //define em qual angulo o motor deve começãr(0 graus)
    travaPorta.write(ANGULO_FECHADO);

    // Define o pino 8 como SDA (linha de dados) e o pino 9 como SCL (linha de relógio, os dois pinos são compartilhados com o RTC e o display
    Wire.begin(8, 9);

    
    lcd.init();// Inicia o display
    lcd.backlight();//acende o backlight dele 
    lcd.setCursor(0, 0);//coloca o texto na esquerda 
    lcd.print("Controle Acesso");//exibe o texto na primeira linha 
    lcd.setCursor(0, 1);//move o texto para a linha de baixo 
    lcd.print("Iniciando...");// exibe outro texto na linha de baixo 

    // RTC
     if (!rtc.begin()) {
            Serial.println("[ERRO] RTC DS1307 nao encontrado!"); // ele faz a verificação de comunicação com o chip caso ele nao responda ele retorna essa mensagem 
    } else if (!rtc.isrunning()) {
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // caso ele retorne algo ele ajusta a hora e data que foi compilado o programa 
    }

    //memoria
    prefs.begin("controle", false); // abre espaço na memoria flash do esp32 com o nome de controle e deixando ela aberta para leitura e escrita 
    senhaCorreta = prefs.getString("senha", "908070"); // le a senha gravada, caso nao tenha nada gravado antes ele pega essa senha como padrao 
    totalAcessos = prefs.getInt("contador", 0); // le as tentativas de aceeso, caso nao tenha nenhuma ele assume o 0

    delay(1500);
}

void loop(){

}