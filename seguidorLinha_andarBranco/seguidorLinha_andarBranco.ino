//definição dos Sensores Infravermelho
#define sensorDIREITO A3
#define sensorESQUERDO A0 
#define sensorCENTRAL A1 

//variáveis para armazenar dados dos Sensores
int Dados_SensorDIREITO; 
int Dados_SensorESQUERDO; 
int Dados_SensorCENTRAL; 

//definição da linha que divide preto e branco
//se maior que o limiar, será branco
//se menor que o limiar, será preto
#define limiar 300

//definição dos Pinos de Potência dos Motores
#define pinPotMotorESQ  6
#define pinPotMotorDIR  5

//definição dos Pinos de Direção de Rotação
//motor Direito
#define MotorDireito_Frente 2
#define MotorDireito_Atras 13
//motor Esquerdo
#define MotorEsquerdo_Frente 4
#define MotorEsquerdo_Atras 8

// definição de Velocidades
#define veloESQ1 200
#define veloDIR1 185
#define veloZero 0
#define veloCurvE 185
#define veloCurvD 170

//função que faz o carrinho ficar parado
void PARADO(){
  digitalWrite(MotorDireito_Frente, HIGH );
  digitalWrite(MotorDireito_Atras, LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras, LOW );

  analogWrite(pinPotMotorESQ , veloZero);
  analogWrite(pinPotMotorDIR , veloZero);
}

//função que faz o carrinho andar para frente
void FRENTE(){
  digitalWrite(MotorDireito_Frente, HIGH );
  digitalWrite(MotorDireito_Atras, LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras, LOW );

  analogWrite(pinPotMotorESQ , veloESQ1);
  analogWrite(pinPotMotorDIR , veloDIR1);
}

//função que faz o carrinho andar para a direita
//para virar à direita, o motor direito é desativado e o da esquerda é ativado
void DIREITA(){
  digitalWrite(MotorDireito_Frente, HIGH );
  digitalWrite(MotorDireito_Atras, LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras, LOW );

  analogWrite(pinPotMotorESQ , veloZero);
  analogWrite(pinPotMotorDIR , veloCurvD);
}

//função que faz o carrinho andar para a esquerda
//para virar à esquerda, o motor esquerdo é desativado e o da direita é ativado
void ESQUERDA(){
  digitalWrite(MotorDireito_Frente, HIGH );
  digitalWrite(MotorDireito_Atras, LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras, LOW );

  analogWrite(pinPotMotorESQ , veloCurvE);
  analogWrite(pinPotMotorDIR , veloZero);
}

void setup() {
  //configuração dos Modos de Pino
  // INPUT = Entrada de Dados
  // OUTPUT = Saída de Dados

  //configuração dos Pinos dos Motores como SAÍDA
  pinMode(MotorDireito_Frente, OUTPUT);
  pinMode(MotorDireito_Atras, OUTPUT);
  pinMode(MotorEsquerdo_Frente, OUTPUT);
  pinMode(MotorEsquerdo_Atras, OUTPUT);
  
  //configuração das Potências dos Motores como SAÍDA
  pinMode(pinPotMotorESQ, OUTPUT);
  pinMode(pinPotMotorDIR, OUTPUT);

  //configuração dos Sensores como ENTRADA
  pinMode(sensorDIREITO , INPUT);
  pinMode(sensorESQUERDO , INPUT);
  pinMode(sensorCENTRAL , INPUT);

  delay(3000); //aguarda 3 segundos antes de iniciar
}

void loop() {
  //leitura dos dados dos sensores
  Dados_SensorCENTRAL = analogRead(sensorCENTRAL);
  Dados_SensorDIREITO = analogRead(sensorDIREITO);
  Dados_SensorESQUERDO = analogRead(sensorESQUERDO);

  //lógica para o movimento do carrinho baseado nos sensores
  //caso o sensor central esteja na linha branca e os outros sensores na linha preta, siga em frente
  if (Dados_SensorCENTRAL <= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO >= limiar) {
    FRENTE();
  }

  //caso o sensor direito esteja na linha branca e os outros sensores na linha preta, vire à direita
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO <= limiar && Dados_SensorDIREITO >= limiar) {
    DIREITA();
  }

  //caso o sensor esquerdo esteja na linha branca e os outros sensores na linha preta, vire à esquerda
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO <= limiar) {
    ESQUERDA(); 
  }
}