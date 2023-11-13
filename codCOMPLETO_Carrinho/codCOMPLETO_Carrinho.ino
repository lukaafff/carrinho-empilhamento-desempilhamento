//inclusão das bibliotecas necessárias para o programa
#include <Wire.h>
#include <hd44780.h>						// main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>

//controlar o display LCD usando I2C
#include <LiquidCrystal_I2C.h>

//comunicação SPI
#include <SPI.h>

//interação com o módulo RFID
#include <MFRC522.h>

// Definição dos Sensores Infravermelho
#define sensorDIREITO A2
#define sensorCENTRAL A1
#define sensorESQUERDO A0

// Variáveis para armazenar dados dos Sensores
int Dados_SensorDIREITO;
int Dados_SensorESQUERDO;
int Dados_SensorCENTRAL;

// Definição da linha que divide preto e branco
// Se maior que o limiar, será branco
// Se menor que o limiar, será preto
#define limiar 400

// Definição dos Pinos de Potência dos Motores
#define pinPotMotorESQ 6
#define pinPotMotorDIR 5

// Definição dos Pinos de Direção de Rotação
// Motor Direito
#define MotorDireito_Frente 2
#define MotorDireito_Atras 18
// Motor Esquerdo
#define MotorEsquerdo_Frente 4
#define MotorEsquerdo_Atras 8

// Definição de Velocidades
#define veloESQ1 190
#define veloDIR1 130
#define veloZero 0
#define veloCurvE 220
#define veloCurvD 170
#define veloESQ2 0
#define veloDIR2 0

// Função que faz o carrinho ficar parado
void PARADO()
{
  digitalWrite(MotorDireito_Frente, LOW);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, LOW);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloZero);
  analogWrite(pinPotMotorDIR, veloZero);
  Serial.println("parado");
}

// Função que faz o carrinho andar para frente com velociodade reduzida 
void FRENTE2()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, 150);
  analogWrite(pinPotMotorDIR, 110);
  Serial.println("frente");
}


// Função que faz o carrinho andar para frente
void FRENTE()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloESQ1);
  analogWrite(pinPotMotorDIR, veloDIR1);
  Serial.println("frente");
}

// Função que faz o carrinho andar para a esquerda
// Para virar à esquerda, o motor esquerdo é desativado e o da direita é ativado
void ESQUERDA()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloESQ2);
  analogWrite(pinPotMotorDIR, veloCurvD);
  Serial.println("esquerda");
}

// Função que faz o carrinho andar para a direita
// Para virar à direita, o motor direito é desativado e o da esquerda é ativado
void DIREITA()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloCurvE);
  analogWrite(pinPotMotorDIR, veloDIR2);
}

// Função que faz o carrinho andar para trás
void TRAS()
{
  digitalWrite(MotorDireito_Frente, LOW);
  digitalWrite(MotorDireito_Atras, HIGH);
  digitalWrite(MotorEsquerdo_Frente, LOW);
  digitalWrite(MotorEsquerdo_Atras, HIGH);

  analogWrite(pinPotMotorESQ, veloESQ1);
  analogWrite(pinPotMotorDIR, veloDIR1);
}

// Definição dos Pinos do Ultrassônico
#define echo 7
#define trig 3

// Variáveis para armazenar dados do sensor ultrassônico
unsigned long tempo;
float distancia;

bool carrinhoParado = false;  // Variável de controle para verificar se o carrinho deve permanecer parado

//definição dos pinos para o módulo RFID e configuração do display LCD
#define SS_PIN 10
#define RST_PIN 9
#define col 16 //número de colunas do display LCD
#define lin 2  //número de linhas do display LCD.
#define ende 0x27 //endereço I2C do display LCD.

//inicialização do LCD e do módulo RFID
//LiquidCrystal_I2C lcd(ende, col, lin); //passando parâmetros para controlar o display
hd44780_I2Cexp lcd; //controlaar display
MFRC522 rfid(SS_PIN, RST_PIN);          //passando parâmetros para controlar o módulo RFID
MFRC522::MIFARE_Key key;                //chave de autenticação padrão

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

unsigned long tempoBloqueio = 0;                       //variável para controlar o tempo de bloqueio após desempilhar (em milissegundos)
const unsigned long tempoBloqueioDuracao = 1000;       //5 segundos de bloqueio após desempilhar (em milissegundos)
unsigned long empilhandoStartTime = 0;                 //tempo inicial durante o processo de empilhamento
const unsigned long empilhandoDuration = 10000;        //duração do processo de empilhamento (em milissegundos)

String mensagemPadrao = "Buscando..."; //mensagem padrão exibida no LCD

void setup()
{
  // Configuração dos Modos de Pino
  // INPUT = Entrada de Dados
  // OUTPUT = Saída de Dados

  // Configuração dos Pinos dos Motores como SAÍDA
  pinMode(MotorDireito_Frente, OUTPUT);
  pinMode(MotorDireito_Atras, OUTPUT);
  pinMode(MotorEsquerdo_Frente, OUTPUT);
  pinMode(MotorEsquerdo_Atras, OUTPUT);

  // Configuração das Potências dos Motores como SAÍDA
  pinMode(pinPotMotorESQ, OUTPUT);
  pinMode(pinPotMotorDIR, OUTPUT);

  // Configuração dos Sensores como ENTRADA
  pinMode(sensorDIREITO, INPUT);
  pinMode(sensorESQUERDO, INPUT);
  pinMode(sensorCENTRAL, INPUT);

  // Configuração dos pinos do Ultrassônico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //inicialização do LCD, Serial, SPI e do módulo RFID
  lcd.init();
  lcd.lineWrap(); //pular para a linha debaixo 
  lcd.backlight();
  lcd.clear();
  SPI.begin();
  rfid.PCD_Init();

  //exibição da mensagem inicial no LCD e no Serial Monitor
  Serial.println("Buscando...");
  lcd.setCursor(0, 0);
  lcd.print(mensagemPadrao);

  //inicialização da pilha e do tempo inicial
  inicializarPilha();
  startTime = millis();

  Serial.begin(9600); // Inicialização da comunicação serial para debug
  delay(3000);        // Aguarda 3 segundos antes de iniciar
}

void loop()
{
  //obtém o tempo atual
  unsigned long tempoAtual = millis();

  // Leitura dos dados dos sensores
  Dados_SensorCENTRAL = analogRead(sensorCENTRAL);
  Dados_SensorDIREITO = analogRead(sensorDIREITO);
  Dados_SensorESQUERDO = analogRead(sensorESQUERDO);

  /* Serial.print("Sensor Central: ");
  Serial.println(Dados_SensorCENTRAL);
  Serial.print("Sensor Direito: ");
  Serial.println(Dados_SensorDIREITO);
  Serial.print("Sensor Esquerdo: ");
  Serial.println(Dados_SensorESQUERDO);
 */

  // Lógica para o movimento do carrinho baseado nos sensores
  // Caso o sensor central esteja na linha branca e os outros sensores na linha preta, siga em frente
  if (Dados_SensorCENTRAL <= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO >= limiar)
  {
    FRENTE();
    Serial.println("frente");
  }

  // Caso o sensor direito esteja na linha branca e os outros sensores na linha preta, vire à direita
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO <= limiar && Dados_SensorDIREITO >= limiar)
  {
    DIREITA();
    Serial.println("direita");
  }

  // Caso o sensor esquerdo esteja na linha branca e os outros sensores na linha preta, vire à esquerda
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO <= limiar)
  {
    ESQUERDA();
    Serial.println("esquerda");
  }

   // Lógica para o ultrassônico - se a distância for menor ou igual a 17 cm, o carrinho para
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  tempo = pulseIn(echo, HIGH);
  distancia = tempo / 58.0;

 /*  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm"); */

  // Se a distância for menor ou igual a 20 cm, pare o carrinho
  if (distancia <= 20)
  {
    delay(100);
    PARADO();
    Serial.println("Carrinho Parado!");
    carrinhoParado = true;  // Carrinho está parado

    //RFID
    //verifica se o tempo de bloqueio após desempilhar foi atingido
  if (tempoAtual - tempoBloqueio >= tempoBloqueioDuracao)
  {
    //permite a leitura das tags novamente
    //verifica se um novo cartão RFID está presente ou se o cartão atual foi lido com sucesso
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
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

  //exibe o estado atual da pilha no LCD
  lcd.setCursor(0, 0);
  lcd.print("Empilhar:");

  //exibe os dados empilhados no LCD
  if (pilha.topo >= 0)
  {
    for (int i = 0; i <= pilha.topo; i++)
    {
      lcd.print(pilha.dados[i]);
      lcd.print(" ");
      delay(1000);
      FRENTE();
      delay(100);
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

      while (pilha.topo >= 0)
      {
        lcd.print(desempilhar());
        lcd.print(" ");
        delay(500); //aguarda 500 milissegundos (0,5 segundos) entre cada número para tornar o desempilhamento mais dinâmico
        //lcd.print(mensagemPadrao);
      }

      // auarda 10 segundos antes de voltar a empilhar novas tags
      delay(10000);
      FRENTE2();
      delay(10);
    }
  }
  else
  {
    //quando a pilha estiver vazia, exibe mensagem de "Buscando a tag" no LCD
    lcd.print(mensagemPadrao);
  }

  rfid.PICC_HaltA();

  // Quando a pilha estiver vazia, exibe "Empilhando:" no LCD
  if (pilha.topo == -1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(mensagemPadrao);
  }

  }
  else
  {
    // Se o carrinho estava parado e a distância é maior que 20 cm, retome o movimento
    if (carrinhoParado)
    {
      carrinhoParado = false;
      // Adicione a lógica de movimento desejada (por exemplo, retomar a frente)
      FRENTE2();
      //Serial.println("frente");
    }
  }
}