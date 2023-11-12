//definição dos Sensores Infravermelho
#define sensorDIREITO A3
#define sensorESQUERDO A2
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
#define MotorDireito_Atras A3
//motor Esquerdo
#define MotorEsquerdo_Frente 4
#define MotorEsquerdo_Atras 8

// definição de Velocidades
#define veloESQ1 170
#define veloDIR1 170
#define veloZero 0
#define veloCurvE 160
#define veloCurvD 160

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

// Função que faz o carrinho andar para trás
void TRAS() {
  digitalWrite(MotorDireito_Frente, LOW);
  digitalWrite(MotorDireito_Atras, HIGH);
  digitalWrite(MotorEsquerdo_Frente, LOW);
  digitalWrite(MotorEsquerdo_Atras, HIGH);

  analogWrite(pinPotMotorESQ, veloESQ1); // Ajuste a velocidade conforme necessário
  analogWrite(pinPotMotorDIR, veloDIR1); // Ajuste a velocidade conforme necessário
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

   pinMode(A3, INPUT);  // Configuração do pino A3 como analógico
  pinMode(MotorDireito_Atras, INPUT);  // Configuração do pino A3 como digital

  delay(3000); //aguarda 3 segundos antes de iniciar
}

void loop() {
  // leitura dos dados dos sensores
  Dados_SensorCENTRAL = analogRead(sensorCENTRAL);
  Dados_SensorDIREITO = analogRead(sensorDIREITO);
  Dados_SensorESQUERDO = analogRead(sensorESQUERDO);

  // lógica para o movimento do carrinho baseado nos sensores
  // caso o sensor central esteja na linha preta e os outros sensores na linha branca, siga em frente
  if (Dados_SensorCENTRAL >= limiar && Dados_SensorESQUERDO <= limiar && Dados_SensorDIREITO <= limiar) {
    FRENTE();
  }

  // caso o sensor direito esteja na linha preta e os outros sensores na linha branca, vire à direita
  else if (Dados_SensorCENTRAL <= limiar && Dados_SensorESQUERDO >= limiar && Dados_SensorDIREITO <= limiar) {
    DIREITA();
  }

  // caso o sensor esquerdo esteja na linha preta e os outros sensores na linha branca, vire à esquerda
  else if (Dados_SensorCENTRAL <= limiar && Dados_SensorESQUERDO <= limiar && Dados_SensorDIREITO >= limiar) {
    ESQUERDA(); 
  }
}


