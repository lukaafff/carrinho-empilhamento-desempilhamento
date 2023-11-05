//Definições dos motores
// Definição dos Pinos de Potência dos Motores
#define pinPotMotorESQ  6
#define pinPotMotorDIR  5

// Definição dos Pinos de Direção de Rotação
// Motor Direito
#define MotorDireito_Frente 2
#define MotorDireito_Atras 0

// Motor Esquerdo
#define MotorEsquerdo_Frente 4
#define MotorEsquerdo_Atras 1

//Definição de Velocidades

//Diferença de 25 no valor de um motor para o outro
#define veloESQ1 150
#define veloDIR1 105
#define veloZero 0
#define veloCurvE 75
#define veloCurvD 80

//Definição dos Sensores InfraVermelho
#define sensorDIREITO A5 //AZUL
#define sensorESQUERDO A3 //AMARELO
#define sensorCENTRAL A4 //VERMELHO

//Váriavel para armazenar dados dos Sensores
int Dados_SensorDIREITO; //AZUL
int Dados_SensorESQUERDO; //AMARELO
int Dados_SensorCENTRAL; //CENTRAL

//Definição da linha que divide preto e branco
//se maior que o linear, será preto
//se menor que o linear, será branco
#define linear 200
//#define limiar2 90


//Definiçoes do Ultrasonico
#define echo 7
#define trig 3
#define LED 8 
unsigned long tempo;
float distancia;
float distancia1;
float distancia2;

//_______________________________________Definiçoes GERAIS de Funçoes______________________________
//_______________________________________MOTOR______________________________

//Função que faz o carrinho ficar parado
//FUNÇÃO PARADO DESATIVADA
void PARADO(){
  digitalWrite(MotorDireito_Frente,  LOW );
  digitalWrite(MotorDireito_Atras,    LOW );
  digitalWrite(MotorEsquerdo_Frente, LOW );
  digitalWrite(MotorEsquerdo_Atras,   LOW );

  analogWrite(pinPotMotorESQ , veloZero);
  analogWrite(pinPotMotorDIR , veloZero);

  //Serial.println("parado");
}

//Função que faz o carrinho andar para frente
void FRENTE() {
  digitalWrite(MotorDireito_Frente, HIGH);
  digitalWrite(MotorDireito_Atras, LOW);
  analogWrite(pinPotMotorDIR, veloDIR1);  

  digitalWrite(MotorEsquerdo_Frente, HIGH);
  digitalWrite(MotorEsquerdo_Atras, LOW);
  analogWrite(pinPotMotorESQ, veloESQ1);  
}

//Função que faz o carrinho andar para a direita
//Para virar a direita o motor o direito é desativado e o da esquerdo ativado
void DIREITA(){
  digitalWrite(MotorDireito_Frente,  HIGH );
  digitalWrite(MotorDireito_Atras,    LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras,   LOW );

  analogWrite(pinPotMotorESQ , veloZero);
  analogWrite(pinPotMotorDIR , veloCurvD);
}

//Função que faz o carrinho andar para a esquerda
//Para virar a esquerda o motor o esquerdo é desativado e o da direito ativado
void ESQUERDA(){
  digitalWrite(MotorDireito_Frente,  HIGH );
  digitalWrite(MotorDireito_Atras,    LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras,   LOW );

  analogWrite(pinPotMotorDIR , veloZero);
  analogWrite(pinPotMotorESQ , veloCurvE);
  
}

//_______________________________________ULTRASSONICO______________________________

float calcular(){
  //ciclo de emissão do sinal do ultrasonico
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  tempo = pulseIn(echo, HIGH);
  float distanciacal = tempo/58;
  if(distanciacal > 100)
  {
    distanciacal = 101;
  }
  return distanciacal;
}

//Função para o carrinho dar a arrancada após parar com o ultrassônico
void ARRANCADA(){
  digitalWrite(MotorDireito_Frente,  HIGH );
  digitalWrite(MotorDireito_Atras,    LOW );
  digitalWrite(MotorEsquerdo_Frente, HIGH );
  digitalWrite(MotorEsquerdo_Atras,   LOW );

  analogWrite(pinPotMotorESQ , 95);
  analogWrite(pinPotMotorDIR , 95);
}

void setup() {
 //Definição dos Modos de pino
//INPUT = Entrada de Dados
//OUTPUT = Saída de Dados

//Definição pinos dos motores como SAIDA
  pinMode(MotorDireito_Frente,	OUTPUT);
	pinMode(MotorDireito_Atras,	  OUTPUT);
	pinMode(MotorEsquerdo_Frente, OUTPUT);
	pinMode(MotorEsquerdo_Atras,   OUTPUT);
  
//Definição de Potencias dos motores como SAIDA
  pinMode(pinPotMotorESQ,   OUTPUT);
  pinMode(pinPotMotorDIR,   OUTPUT);

  //Definição dos Sensores como ENTRADA
  pinMode(sensorDIREITO , INPUT);
  pinMode(sensorESQUERDO , INPUT);
  pinMode(sensorCENTRAL , INPUT);

//Definição dos pinos do Ultrasonico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED, OUTPUT);

//Tempo para andar
  delay(3000);

//________________________________iNICIALIZAÇÃO DO LOG_______________________________ 

  Serial.begin(9600);
  Serial.println("-------- iniciando LOG ---------");

}

void loop(){
  delay(500);
distancia = calcular();
Dados_SensorCENTRAL = analogRead(sensorCENTRAL);
Dados_SensorDIREITO = analogRead(sensorDIREITO);
Dados_SensorESQUERDO = analogRead(sensorESQUERDO);


   /* Serial.println(Dados_SensorCENTRAL );
    Serial.println("Dados_SensorCENTRAL" );
    
    Serial.println(Dados_SensorDIREITO );
    Serial.println("Dados_SensorDIREITO");
    
    Serial.println(Dados_SensorESQUERDO);
    Serial.println("Dados_SensorESQUERDO");
  */
    
   
    //Serial.println(distancia);
    if(distancia < 17)
      {
      PARADO();
      //digitalWrite(LED, HIGH);
      delay (500);
      //Serial.println("Objeto perto");
      }
    if(distancia >= 17)
      {
        ARRANCADA();
        //digitalWrite(LED, LOW);
        //Serial.println("arrancada");
      
      }
  
    if (distancia >= 17){
      if(Dados_SensorCENTRAL <= linear && Dados_SensorESQUERDO >= linear && Dados_SensorDIREITO >= linear )
      {
        FRENTE();
        Serial.println("Frente");
      }
      //caso o sensor vermelho esteja na linha branca, virar para a direita para o carrinho ficar reto
      if(Dados_SensorCENTRAL >= linear && Dados_SensorESQUERDO >= linear && Dados_SensorDIREITO <= linear )
      {
        DIREITA();
        Serial.println("Direita");
      }
      //caso o sensor amarelo esteja na linha branca, virar para a esquerda para o carrinho ficar reto
      if(Dados_SensorCENTRAL >= linear && Dados_SensorESQUERDO <= linear && Dados_SensorDIREITO >= linear )
      {
        ESQUERDA(); 
        Serial.println("Esquerda");
      }
      //cruzamento de Linhas
      if(Dados_SensorCENTRAL <= linear && Dados_SensorESQUERDO <= linear && Dados_SensorDIREITO <= linear )
      {
        FRENTE();
        //Serial.println("FRENTE");
      }
      //Caso saia da linha
      /*
      if(Dados_SensorCENTRAL > limiar2 && Dados_SensorESQUERDO > limiar2 && Dados_SensorDIREITO > limiar2 )
      {
        TRAS();
        Serial.println("Trás");
      }
      //direita 2
      
      if(Dados_SensorCENTRAL <= linear && Dados_SensorESQUERDO >= linear && Dados_SensorDIREITO <= linear )
      {
        DIREITA();
        Serial.println("Direita 2");
      }
      //esquerda 2
      if(Dados_SensorCENTRAL <= linear && Dados_SensorESQUERDO <= linear && Dados_SensorDIREITO >= linear )
      {
        ESQUERDA(); 
        Serial.println("Esquerda 2");
      }
      */
    }
  

}