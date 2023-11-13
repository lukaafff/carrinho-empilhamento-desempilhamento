#include <Wire.h>
#include <hd44780.h>						// main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>

//inclusão das bibliotecas necessárias para o programa
//controlar o display LCD usando I2C
#include <LiquidCrystal_I2C.h>

//comunicação SPI
#include <SPI.h>

//interação com o módulo RFID
#include <MFRC522.h>

//definição dos pinos para o módulo RFID e configuração do display LCD
#define SS_PIN 10
#define RST_PIN 9
#define col 16 //número de colunas do display LCD
#define lin 2  //número de linhas do display LCD.
#define ende 0x27 //endereço I2C do display LCD.

//inicialização do LCD e do módulo RFID
LiquidCrystal_I2C lcdd(ende, col, lin); //passando parâmetros para controlar o display
MFRC522 rfid(SS_PIN, RST_PIN);          //passando parâmetros para controlar o módulo RFID
MFRC522::MIFARE_Key key;                //chave de autenticação padrão

hd44780_I2Cexp lcd;

unsigned long startTime;       //armazena o tempo inicial para controle de tempo
const int tempoLimite = 30000; //define o limite de tempo (em milissegundos) para operações

//lista de IDs de tags RFID permitidos
const String tagsPermitidas[] = {
    "ED:75:38:04",
    "CC:07:39:04",
    "D6:60:38:04",
    "76:26:38:04"
    };

String ultimaTagLida = ""; //armazena o ID da última tag RFID lida

//estrutura da pilha
struct Pilha
{
  int dados[4]; //array para armazenar os dados
  int topo;      //indice do topo da pilha
};

Pilha pilha; //declaração da pilha

void inicializarPilha()
{
  //topo da pilha como -1, indicando que a pilha está vazia
  pilha.topo = -1; //inicializa o topo da pilha
}

void empilhar(int dado)
{
  //verifica se a pilha não está cheia (limite de 4 IDs) antes de empilhar o novo ID
  if (pilha.topo < 3)
  {
    pilha.topo++;
    pilha.dados[pilha.topo] = dado;
  }
}

int desempilhar()
{
  //verifica se a pilha não está vazia antes de desempilhar
  if (pilha.topo >= 0)
  {
    int dado = pilha.dados[pilha.topo];
    pilha.topo--;
    //retorna o ID desempilhado ou -1 se a pilha estiver vazia
    return dado;
  }
  else
  {
    return -1; //pilha vazia
  }
}

unsigned long desempilhandoStartTime = 0;             //tempo inicial durante o processo de desempilhamento
const unsigned long desempilhandoDuration = 30000;    //duração do processo de desempilhamento (em milissegundos)
unsigned long tempoBloqueio = 0;                       //variável para controlar o tempo de bloqueio após desempilhar (em milissegundos)
const unsigned long tempoBloqueioDuracao = 1000;       //5 segundos de bloqueio após desempilhar (em milissegundos)
unsigned long empilhandoStartTime = 0;                 //tempo inicial durante o processo de empilhamento
const unsigned long empilhandoDuration = 10000;        //duração do processo de empilhamento (em milissegundos)

String mensagemPadrao = "Buscando a tag..."; //mensagem padrão exibida no LCD

void setup()
{
  //inicialização do LCD, Serial, SPI e do módulo RFID
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  //exibição da mensagem inicial no LCD e no Serial Monitor
  Serial.println("Buscando a tag...");
  lcd.setCursor(1, 0);
  lcd.print(mensagemPadrao);

  //inicialização da pilha e do tempo inicial
  inicializarPilha();
  startTime = millis();

  lcd.lineWrap();
}

void loop()
{
  //obtém o tempo atual
  unsigned long tempoAtual = millis();

  //verifica se o tempo de bloqueio após desempilhar foi atingido
  if (tempoAtual - tempoBloqueio >= tempoBloqueioDuracao)
  {
    //permite a leitura das tags novamente
    //verifica se um novo cartão RFID está presente ou se o cartão atual foi lido com sucesso
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("testando:");
      //lcd.setCursor(0, 1);
      lcd.print(mensagemPadrao);
      return;
    }

    //lê o identificador (UID) da tag
    String strID = "";
    for (byte i = 0; i < rfid.uid.size; i++)
    {
      strID +=
          (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(rfid.uid.uidByte[i], HEX) + (i != rfid.uid.size - 1 ? ":" : "");
    }

    //converte o UID para letras maiúsculas
    strID.toUpperCase();

    //limpa o LCD e exibe o UID da tag no Serial Monitor
    lcd.clear();
    Serial.print("Identificador (UID) da tag: ");
    Serial.println(strID);

    //verifica se a tag é permitida ou repetida
    bool tagPermitida = false;
    bool tagRepetida = false;

    //verifica se a tag lida está na lista de tags permitidas e não é a mesma que foi lida anteriormente
    if (strID != ultimaTagLida)
    {
      for (int i = 0; i < sizeof(tagsPermitidas) / sizeof(tagsPermitidas[0]); i++)
      {
        if (strID == tagsPermitidas[i])
        {
          tagPermitida = true;
          break;
        }
      }
    }
    else
    {
      tagRepetida = true;
    }

    //processa a tag lida
if (tagPermitida)
{
  if (strID != ultimaTagLida) //verifica se a tag atual é diferente da tag anterior
  {
    if (pilha.topo < 3)
    {
      empilhar(strtol(strID.c_str(), NULL, 16)); //converte a string hexadecimal para inteiro e empilha
    }
    ultimaTagLida = strID; //atualiza a última tag lida
    tempoBloqueio = tempoAtual; //atualiza o tempo de bloqueio
  }
  else
  {
    //exibe mensagem de "Tag já lida" no LCD e aguarda 2 segundos antes de voltar à mensagem padrão
    lcd.setCursor(0, 0);
    lcd.print("Tag ja lida");
    delay(2000);
  }
}
else if (tagRepetida)
{
  //exibe mensagem de "Tag já lida" no LCD e aguarda 2 segundos antes de voltar à mensagem padrão
  lcd.setCursor(0, 0);
  lcd.print("Tag ja lida");
  delay(2000);
}
else
{
  //exibe mensagem de "Tag não permitida" no LCD e aguarda 2 segundos antes de voltar à mensagem padrão
  lcd.setCursor(0, 0);
  lcd.print("Tag nao permitida");
  delay(2000);
}
  }
  else
  {
    //aguarda o tempo de bloqueio antes de permitir a leitura das tags novamente
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aguarde...");
    //lcd.setCursor(0, 1);
    lcd.print((tempoBloqueio + tempoBloqueioDuracao - tempoAtual) / 1000);
    lcd.print("s");
  }

  //exibe o estado atual da pilha no LCD
  lcd.setCursor(0, 0);
  lcd.print("Empilhar:");
  //lcd.setCursor(0, 1);

  //exibe os dados empilhados no LCD
  if (pilha.topo >= 0)
  {
    for (int i = 0; i <= pilha.topo; i++)
    {
      lcd.print(pilha.dados[i]);
      lcd.print(" ");
    }

    //quando a pilha estiver cheia, aguarda 2 segundos antes de desempilhar e exibir os números
    if (pilha.topo == 3)
    {
      empilhandoStartTime = tempoAtual; // Registra o tempo inicial de empilhamento

      //mostra a mensagem "Empilhando:" junto com os números por 10 segundos
      while (tempoAtual - empilhandoStartTime < empilhandoDuration)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Empilhar:");
        //lcd.setCursor(0, 1);

        for (int i = 0; i <= pilha.topo; i++)
        {
          lcd.print(pilha.dados[i]);
          lcd.print(" ");
        }

        delay(500); //aguarda 500 milissegundos (0,5 segundos) entre cada exibição
        tempoAtual = millis(); //atualiza o tempo atual
      }

      //desempilha e exibe os números
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Desempilhar:");
      //lcd.setCursor(0, 1);

      while (pilha.topo >= 0)
      {
        lcd.print(desempilhar());
        lcd.print(" ");
        delay(500); //aguarda 500 milissegundos (0,5 segundos) entre cada número para tornar o desempilhamento mais dinâmico
      }

      // auarda 10 segundos antes de voltar a empilhar novas tags
      delay(10000);
    }
  }
  else
  {
    //quando a pilha estiver vazia, exibe mensagem de "Buscando a tag" no LCD
    lcd.print("Buscando a tag");
  }

  rfid.PICC_HaltA();
  delay(2000); //aguarda 2 segundos antes de exibir a mensagem novamente

  //exibe mensagem padrão no LCD
  //lcd.setCursor(0, 1);
  lcd.print("Buscando a tag...");

  // Quando a pilha estiver vazia, exibe "Empilhando:" no LCD
  if (pilha.topo == -1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Empilhar:");
  }
}