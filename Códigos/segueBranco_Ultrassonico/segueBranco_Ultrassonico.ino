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
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  digitalWrite(MotorEsquerdo_Frente, HIGH);
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
  Serial.begin(9600); // Inicialização da comunicação serial para debug
  delay(3000);        // Aguarda 3 segundos antes de iniciar
}

void loop()
{
  // Leitura dos dados dos sensores
  Dados_SensorCENTRAL = analogRead(sensorCENTRAL);
  Dados_SensorDIREITO = analogRead(sensorDIREITO);
  Dados_SensorESQUERDO = analogRead(sensorESQUERDO);

Serial.print("Sensor Central: ");
Serial.println(Dados_SensorCENTRAL);
Serial.print("Sensor Direito: ");
Serial.println(Dados_SensorDIREITO);
Serial.print("Sensor Esquerdo: ");
Serial.println(Dados_SensorESQUERDO);


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

   // Lógica para o ultrassônico - se a distância for menor ou igual a 17 cm, pare o carrinho
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