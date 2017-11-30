//======= Bibliotecas Auxiliares  ========================================================================================================================================

#include <EEPROM.h>              // Biblioteca do Arduino para utilizar a memória EEPROM da placa.

#include <IRremote.h>            // Biblioteca para Receptopr / Emissor Infravermelho.

#include <DHT.h>                 // Biblioteca para Sensor de Temperatura e Umidade DHT11.

#include <MFRC522.h>             // Biblioteca Para Módulo RFID RC-522
#include <SPI.h>                 // Biblioteca do Arduino para Comunicação SPI ( Serial Peripheral Interface - Interface Periféricos Seriais) para ser utilizada com o Módulo RFID RC-522.

#include <LiquidCrystal_I2C.h>   // Biblioteca para LCD com I2C integrado.
#include <Wire.h>                // Biblioteca do Arduino para Comunicação I2C ( Inter-Integrated Circuit - Circuito Inter-Integrado )

#include <DS3231.h>              // Biblioteca para RTC ( Real Time Clock ) DS3231.

//======= Mapeamento de Hardware - Portas Analógicas  ===================================================================================================================

#define DHTPIN         A1        // Pino Analógico A1 do Arduino para ler o valor do Senor DHT11.
#define Sensor_Alarme  A2        // Pino Analógico A2 do Arduino para ler o valor do Senor de Alarme ( EDT002 - Barreira Ótica http://www.educatronica.com.br/kits.html ) 

//======= Mapeamento de Hardware - Portas Digitais  =====================================================================================================================

#define Botao_Menu_Mais    2    // Botão para selecionar menu avança             no pino digital 2
#define Botao_Menu_Menos   3    // Botão para selecionar menu volta              no pino digital 3
#define Botao_OK           4    // Botão rearmar alarme quando detectado invasão no pino digital 4
#define Botao_Liga         5    // Botão para Ligar.                             no pino digital 5
#define Botao_Desliga      6    // Botão para Desligar.                          no pino digital 6

#define Receptor_Infravermelho   19    // Receptor Infravermelho no pino digital 19 paracomunicação serial (RX do arduino).
#define Led_Sistema              7     // LED para indicar que sistema está ligadono pino digital 7
#define Led_Alarme               22    // LED para indicar quando houver uma invasão detectada pelo sensor de alarme, está conectado no pino digital 22.
#define Buzzer                   23    // Buzzer(buzina) para indicar quando houver uma invasão detectada pelo sensor de alarme, está conectado no pino digital 23.

#define Led_RFID_Liberado        24    // LED para indicar que TAG liga no Módulo RFID RC522 é válida e cadastrada.
#define Led_RFID_Negado          25    // LED para indicar que TAG liga no Módulo RFID RC522 é inválida e não cadastrada.
#define rele_RFID_Liberado       26    // Pino que irá ligar um RELÉ de 5V para acionar Fechadura Eletrônica da porta.

// ===== Instanciando métodos e funções das Bibliotecas  =====================================================================================================================

IRrecv irrecv(Receptor_Infravermelho); // Comando da Biblioteca <IRremote.h>
decode_results results;                // Comando da Biblioteca <IRremote.h>

#define DHTTYPE DHT11                  // Comando da Biblioteca <DHT.h>
DHT dht(DHTPIN, DHTTYPE);              // Comando da Biblioteca <DHT.h>


// Arduino MEGA para ligação do RFID-RC552 :
// Pino Digital 49 - RESET(RST_RFID),
// Pino Digital 50 - MISO(Entrada de DADOS),
// Pino Digital 51 - MOSI(Saída de DADOS),
// Pino Digital 52 - SCK(Clock),
// Pino Digital 53 - SDA(SS_RFID).

#define RST_RFID 49                    // Comando da Biblioteca <MFRC522.h>             
#define SS_RFID 53                     // Comando da Biblioteca <MFRC522.h>                 
MFRC522 mfrc522(SS_RFID, RST_RFID);    // Comando da Biblioteca <MFRC522.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);    // Comando da Biblioteca <LiquidCrystal_I2C.h>

DS3231 Clock(SDA, SCL);                // Comando da Biblioteca <DS3231.h>
Time  tempo;                           // Comando da Biblioteca <DS3231.h>


// ====== Protótipo dos Métodos/Funções Auxiliares   =================================================================================================================================

void Menu_Atual();                  // Método para modificar o menu atual.
void Menu_Lcd();                    // Método para mostrar o menu atual no lcd.
void menu1_Relogio();               // Método do menu 1, mostrar a hora e data com RTC DS3231.
void menu2_Temperatura();           // Método do menu 2, mostrar a temperatura e umidade com DHT11.
void menu3_Umidade();               // Método do menu 3, mostrar a umidade com DHT11..
void menu4__Alarme_Seguranca();     // Método do menu 4, mostrar o alarme de segurança.
void Leitura_Sensores();            // Método para leitura de todos os sensores.
void Acesso_RFID();   // Método para acionamento da Fechadura Eletrônica da Casa (RFID).

// ======== Variáveis Globais   =============================================================================================================================================

char menu = 0x01;   // Variável para selecionar os  menus. Inicia no menu 1.

boolean Flag_Botao_mais,       //
        Flag_Botao_menos,      //   Flags para
        Flag_Botao_OK,         //   dos botões
        Flag_Botao_Ligar,      //    do menu.
        Flag_Botao_Desligar,   //

        Ligar_Alarme,          //  Flags para controle
        Invasao_Detectada;     //  do Sistema de alarme.


// Variáveis que farão as leituras dos sensores
int  alarmeSeguranca;
double temperatura, umidade;

double atualizacao_dados;  // Variável para contagem de Dados a cada 2 segundos.

// Variável do tipo vetor que armazenará as TAGS cadastradas do RFID.
String tagsCadastradass[2] = {"26 F1 F4 48", "B3 61 8C A9"};

// ===== Método que será executada uma vez quando ligar ou resetar o Arduino.  ==============================================================================================
void setup()
{
  // --- Habilita comunicações do Arduino ---
  Serial.begin(9600);    // Inicia Taxa de Comunicação Serial de 9600 bits por segundo.
  SPI.begin();           // Inicia Protocolo de Comunicação SPI.
  Wire.begin();          // Inicia Protocolo de Comunicação I2C.

  // --- DS3231 ---
  Clock.begin();        // Habilita DS3231.
  // --- RFID ---
  mfrc522.PCD_Init();   // Habilita RFID.
  // --- Infravermelho ---
  irrecv.enableIRIn();  // Habilita Rececptor Infravermelho.
  // --- DHT11 ---
  dht.begin();          // Habilita Sensor DHT11.
  // --- LCD 16x2 I2C ---
  lcd.init();           // Habilita LCD.
  lcd.backlight();      // Habilita LED BackLight.


  for (byte a = 2; a < 7; a++)  {
    pinMode(a, INPUT_PULLUP);  // Define como entrada os pinos digitais 2 até 6 (Botões do Menu) com pull-ups internos do arduino.
    pinMode(a + 20, OUTPUT);   // Define como saída os pinos digitais 22 até 26, que irá ligar LEDs,Buzzer,Relés,etc.

  }

  pinMode(Led_Sistema, OUTPUT);

  // Flags para Controle dos Botões do Menu iniciadas em falso(0)
  Flag_Botao_mais = false;
  Flag_Botao_menos = false;
  Flag_Botao_OK = false;
  Flag_Botao_Ligar = false;
  Flag_Botao_Desligar = false;

  // Flags para Controle do sensor de alarme.
  Ligar_Alarme = EEPROM.read(0);  // faz leitura da memória EEPROM da placa de posição 0 ( para valor 0 (Desligado) ou valor 1 (Ligado) recebido na leitura )
  Invasao_Detectada = false;

  // -- Ligando SISTEMA ---
  lcd.setCursor(0, 0);   // Seleciona Coluna 0, Linha 0
  lcd.print("LIGANDO");  // Imprimi no display.
  lcd.setCursor(0, 1);   // Seleciona Coluna 0, Linha 1
  lcd.print("SISTEMA");  // Imprimi no display.

  // Faz a impressão dos pontinhos quando está ligando o sistema.
  for (byte i = 7; i < 10; i++) {
    delay(500);
    lcd.setCursor(i, 1);
    lcd.print(".");
  }

  delay(1000);  // Espera 1000 milissegundos ou seja, 1 segundo.

  lcd.clear();                     // Limpa display.
  lcd.print("SISTEMA LIGADO !");   // Imprimi na 1º coluna e na 1º linha sempre que, não houver específicação de posição de impressão.

  digitalWrite(Led_Sistema, HIGH);  ;// Liga o led que indica que o sistema está ligado.

  // Faz buzzer fazer 2 bips indicando que o sistema foi ligado.
  for (short a = 0; a < 4; a++) {
    digitalWrite(Buzzer, !digitalRead(Buzzer));
    delay(100);
  }

  delay(1500);  // Espera 1,5 segundos para entrar no sistema.
  lcd.clear();  // Limpa display.

} // --- Fim do void setup ---


// ======== Loop infinito do arduino  ======================================================================================================================================

void loop() {
  //--- Lendo RTC DS3231 ---

  Menu_Atual();
  Menu_Lcd();
  Leitura_Sensores();
  Acesso_RFID();

}

// ======== Menu Atual ===================================================================================================================================

void Menu_Atual()  //Modifica o menu atual.
{
  // Botão Botão_Menu_Mais pressionado ou Tecla do Controle Remoto pressionada? Sim, a Seta flag.
  if (!digitalRead(Botao_Menu_Mais) || results.value == 1086277815 ) Flag_Botao_mais   = true;

  //  Botão Botão_Menu_Menos pressionado ou Tecla do Controle Remoto pressionada? Sim, a Seta flag
  if (!digitalRead(Botao_Menu_Menos) || results.value == 1086285465 ) Flag_Botao_menos = true;

  //  Botão_Menu_Mais solto ou Tecla do Controle Remoto ainda pressionada e Flag_Botao_mais setada? Faz troca do menu.
  if (digitalRead(Botao_Menu_Mais) && results.value != 1086277815 && Flag_Botao_mais == true )
  {
    Flag_Botao_mais = false;                     // Limpa flag
    lcd.clear();                                 // Limpa display
    menu++;                                      // Incrementa menu
    if (menu > 0x04) menu = 0x01;                // Se menu maior que 4, volta a ser 1

  } // --- troca menu Botao_Menu_Mais ---

  // Botao_Menu_Menos solto ou Tecla do Controle Remoto ainda pressionada e Flag_Botao_menos setada? Faz troca do menu.
  if (digitalRead(Botao_Menu_Menos) && results.value != 1086285465 && Flag_Botao_menos)
  {
    Flag_Botao_menos = false;                    // Limpa flag
    lcd.clear();                                 // Limpa display
    menu--;                                      // Decrementa menu
    if (menu < 0x01) menu = 0x04;                // Se menu menor que 1, volta a ser 4

  } // --- troca menu Botao_Menu_Mais ---

} // Fim do void Menu_Atual.

// ======== Menu Atual no Display LCD  ==========================================================================================================================================

void Menu_Lcd()  //Mostra o menu atual no LCD.
{
  switch (menu) //Controle da variável menu
  {
    case 0x01: menu1_Relogio();              // Caso 1 - Chama a função para mostrar o relógio na tela.
      break; //break

    case 0x02: menu2_Temperatura();          // Caso 2 - Chama a função para mostrar a Temperatura na tela.
      break; //break

    case 0x03:  menu3_Umidade();;            // Caso 3 - Chama a função para mostrar a Umidade na tela.
      break;  //break

    case 0x04:  menu4_Alarme_Seguranca();   // Caso 4 - Chama função para mostrar o Alarme de Segurança na tela.
      break; //break

  } //Fim do switch menu
} // Fim do void Menu_Lcd.


// ======== Menus do Sistema ===================================================================================================================================

void menu1_Relogio() {

  lcd.setCursor(4, 0); // Primeira Linha do Display
  if (tempo.hour < 10) lcd.print("0");
  lcd.print(tempo.hour); // Imprimi Hora
  lcd.print(":");
  if (tempo.min < 10)  lcd.print("0");
  lcd.print(tempo.min);  // Imprimi minuto
  lcd.print(":");
  if (tempo.sec < 10)  lcd.print("0");
  lcd.print(tempo.sec);  // Imprimi segundo
  lcd.print(" ");

  // Segunda Linha do Display.
  lcd.setCursor(0, 1);
  lcd.print(" ");

  switch (tempo.dow)
  {
    case 1:  lcd.print("SEG"); break;
    case 2:  lcd.print("TER"); break;
    case 3:  lcd.print("QUA"); break;
    case 4:  lcd.print("QUI"); break;
    case 5:  lcd.print("SEX"); break;
    case 6:  lcd.print("SAB"); break;
    case 7:  lcd.print("DOM"); break;
    default: lcd.print("Erro"); break;
  }
  lcd.print(" ");
  if (tempo.date < 10) lcd.print("0");
  lcd.print(tempo.date);
  lcd.print("/");
  switch (tempo.mon)
  {
    case 1:  lcd.print("01"); break;
    case 2:  lcd.print("02"); break;
    case 3:  lcd.print("03"); break;
    case 4:  lcd.print("04"); break;
    case 5:  lcd.print("05"); break;
    case 6:  lcd.print("06"); break;
    case 7:  lcd.print("07"); break;
    case 8:  lcd.print("08"); break;
    case 9:  lcd.print("09"); break;
    case 10: lcd.print("10"); break;
    case 11: lcd.print("11"); break;
    case 12: lcd.print("12"); break;
    default: lcd.print("Erro"); break;
  }
  lcd.print("/");
  lcd.print(tempo.year);

} // Fim do void menu1_Relogio.

void menu2_Temperatura() {

  lcd.setCursor(0, 0);
  lcd.print("   TEMPERATURA  ");

  lcd.setCursor(6, 1);
  lcd.print(temperatura);
  lcd.write(B11011111); //Simbolo de graus celsius
  lcd.print("C");

} // Fim do void menu2_Temperatura.


void menu3_Umidade()
{
  lcd.setCursor(0, 0);
  lcd.print("    UMIDADE     ");
  lcd.setCursor(5, 1);
  lcd.print(umidade);
  lcd.print("%");

} // Fim do void menu3_Umidade.


void menu4_Alarme_Seguranca() {

  if (!digitalRead(Botao_Liga) || results.value == 1086279855 ) Flag_Botao_Ligar   = true;
  if (!digitalRead(Botao_Desliga) || results.value == 1086302295 ) Flag_Botao_Desligar = true;
  if (!digitalRead(Botao_OK) || results.value == 1086296175 ) Flag_Botao_OK = true;

  if (digitalRead(Botao_Liga) && results.value != 1086279855 && Flag_Botao_Ligar == true)
  {
    Flag_Botao_Ligar = false;
    Ligar_Alarme = true;
    EEPROM.write(0, Ligar_Alarme);
    lcd.clear();
  }
  if (digitalRead(Botao_Desliga) && results.value != 1086302295 && Flag_Botao_Desligar == true)
  {
    Flag_Botao_Desligar = false;
    Ligar_Alarme = false;
    EEPROM.write(0, Ligar_Alarme);
    lcd.clear();
  }

  if (Ligar_Alarme == true && Invasao_Detectada == false)
  {
    lcd.setCursor(0, 0);
    lcd.print("ALARME DE SEGUR.");
    lcd.setCursor(0, 1);
    lcd.print("LIGADO");
  }
  else if ( Ligar_Alarme == false)
  {
    lcd.setCursor(0, 0);
    lcd.print("ALARME DE SEGUR.");
    lcd.setCursor(0, 1);
    lcd.print("DESLIGADO");
    digitalWrite(Buzzer, LOW);
    digitalWrite(Led_Alarme, LOW);
    Invasao_Detectada = false;
  }
  else if (Ligar_Alarme == true && Invasao_Detectada == true)
  {
    lcd.setCursor(0, 0);
    lcd.print("   INVASAO      ");
    lcd.setCursor(0, 1);
    lcd.print("  DETECTADA !! ");

    if (digitalRead(Botao_OK) && results.value != 1086296175 && Flag_Botao_OK == true)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  RESTAURANDO");
      lcd.setCursor(0, 1);
      lcd.print("    ALARME");
      delay(500);
      lcd.setCursor(10, 1);
      lcd.print(".");
      delay(500);
      lcd.setCursor(11, 1);
      lcd.print(".");
      delay(500);
      lcd.setCursor(12, 1);
      lcd.print(".");
      delay(500);
      digitalWrite(Buzzer, LOW);
      digitalWrite(Led_Alarme, LOW);
      Flag_Botao_OK = false;
      Invasao_Detectada = false;
      lcd.clear();
    }
  }
} // Fim do menu4_Alarme_Seguranca.

//
void Leitura_Sensores()
{

  //--- Lendo RTC DS3231 ---
  tempo = Clock.getTime();
  Clock.getTemp();

  // --- Lendo Sensor Infravermelho ---
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, DEC);
    irrecv.resume(); // Recebe o próximo valor.
  }

  // --- Lendo Sensor DHT11, Temperatura e Umidade a cada 2 segundos ---
  if (millis() - atualizacao_dados >= 2000 )
  {
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();
    atualizacao_dados = millis();
  }

  // --- Lendo Alarme de Segurança ---
  if ( Ligar_Alarme == true )
  {
    alarmeSeguranca = analogRead(Sensor_Alarme);
    if ( alarmeSeguranca >= 1015)
    {
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Led_Alarme, HIGH);
      Invasao_Detectada = true;
    }
  }
}

//======= Acesso RFID (Fechadura Eletrônica) ===========================================================================================================================

void Acesso_RFID() {

  // Aguarda proximidade do cartão/tag
  if ( !mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona o cartão/tag
  if ( !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // String que armazenará o UID da TAG
  String Codigo_TAG = "";

  // Dentro deste FOR, armazena byte a byte e concatena na variavel Codigo_TAG
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);  // Im
    Codigo_TAG.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    Codigo_TAG.concat(String(mfrc522.uid.uidByte[i], HEX));  // Concatena os números em forma de números Hexadecimais.
  }
  // Coloca o valor da variavel Codigo_TAG toda em Maisculo
  Codigo_TAG.toUpperCase();
  Serial.println();

  // Tipo de verificações que podem ser feitas !
  // if (Codigo_TAG.substring(1) == tagsCadastrada[0]) //UID 2 - "26 F1 F4 48" - TAG Chaveiro.
  // if (Codigo_TAG.substring(1) == tagsCadastrada[1]) //UID 2 - "B3 61 8C A9" - TAG Cartão Faculdade - Thales L. de Assis.
  // if (Codigo_TAG.substring(1) == "26 F1 F4 48")
  // if (Codigo_TAG.substring(1) == "B3 61 8C A9")

  if (Codigo_TAG.substring(1) == "26 F1 F4 48" || Codigo_TAG.substring(1) == "B3 61 8C A9" ) // TAG Chaveiro e TAG Cartão Faculdade - Thales L. de Assis.
  {
    // Se Tag aproximada é cadastrada liga Led de acesso Liberado e Relé para acionamento de trava eletrônica.
    digitalWrite(Led_RFID_Liberado, HIGH);
    digitalWrite(rele_RFID_Liberado, LOW);
    delay(400);
    digitalWrite(Led_RFID_Liberado, LOW);
    digitalWrite(rele_RFID_Liberado, HIGH);
  }
  else
  {
    digitalWrite(rele_RFID_Liberado, HIGH);
    digitalWrite(Led_RFID_Negado, HIGH);
    delay(400);
    digitalWrite(Led_RFID_Negado, LOW);
  }
} // Fim do void Fechadura_Eletronica_RFID.
