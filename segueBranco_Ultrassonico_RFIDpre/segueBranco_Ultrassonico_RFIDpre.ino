//inclusão das bibliotecas necessárias para o programa
//controlar o display LCD usando I2C
#include <LiquidCrystal_I2C.h>

//comunicação SPI
#include <SPI.h>

//interação com o módulo RFID
#include <MFRC522.h>

// Definição dos Sensores Infravermelho
#define sensorDIREITO A2
#define sensorESQUERDO A0
#define sensorCENTRAL A1

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
#define veloESQ1 250
#define veloDIR1 170
#define veloZero 0
#define veloCurvE 220
#define veloCurvD 170

// Função que faz o carrinho ficar parado
void PARADO()
{
  digitalWrite(MotorDireito_Frente, LOW);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, LOW);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloZero);
  analogWrite(pinPotMotorDIR, veloZero);
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
}

// Função que faz o carrinho andar para a direita
// Para virar à direita, o motor direito é desativado e o da esquerda é ativado
void DIREITA()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloZero);
  analogWrite(pinPotMotorDIR, veloCurvD);
}

// Função que faz o carrinho andar para a esquerda
// Para virar à esquerda, o motor esquerdo é desativado e o da direita é ativado
void ESQUERDA()
{
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);

  analogWrite(pinPotMotorESQ, veloCurvE);
  analogWrite(pinPotMotorDIR, veloZero);
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
LiquidCrystal_I2C lcd(ende, col, lin); //passando parâmetros para controlar o display
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

unsigned long desempilhandoStartTime = 0;             //tempo inicial durante o processo de desempilhamento
const unsigned long desempilhandoDuration = 30000;    //duração do processo de desempilhamento (em milissegundos)
unsigned long tempoBloqueio = 0;                       //variável para controlar o tempo de bloqueio após desempilhar (em milissegundos)
const unsigned long tempoBloqueioDuracao = 1000;       //5 segundos de bloqueio após desempilhar (em milissegundos)
unsigned long empilhandoStartTime = 0;                 //tempo inicial durante o processo de empilhamento
const unsigned long empilhandoDuration = 10000;        //duração do processo de empilhamento (em milissegundos)

String mensagemPadrao = "Aproxime a tag..."; //mensagem padrão exibida no LCD

bool modoMovimento = true; // Variável de estado para alternar entre movimento e RFID

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
  lcd.backlight();
  lcd.clear();
  SPI.begin();
  rfid.PCD_Init();

  //exibição da mensagem inicial no LCD e no Serial Monitor
  Serial.println("Aproxime a tag...");
  lcd.setCursor(1, 0);
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
  }

  // Caso o sensor direito esteja na linha branca e os outros sensores na linha preta, vire à direita
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO <= limiar && Dados_SensorDIREITO >= limiar)
  {
    DIREITA();
  }

  // Caso o sensor esquerdo esteja na linha branca e os outros sensores na linha preta, vire à esquerda
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO <= limiar)
  {
    ESQUERDA();
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

  // Se a distância for menor ou igual a 17 cm, pare o carrinho
  if (distancia <= 17)
  {
    PARADO();
    Serial.println("Carrinho Parado!");
    carrinhoParado = true;  // Carrinho está parado
  }
  else
  {
    // Se o carrinho estava parado e a distância é maior que 17 cm, retome o movimento
    if (carrinhoParado)
    {
      carrinhoParado = false;
      // Adicione a lógica de movimento desejada (por exemplo, retomar a frente)
      FRENTE();
    }
  }

  delay(500); // Aguarda 0.5 segundos antes da próxima leitura
}