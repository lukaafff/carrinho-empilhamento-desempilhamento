//definição dos Pinos do Ultrassônico
#define echo 7
#define trig 3

//variáveis para armazenar dados do sensor ultrassônico
unsigned long tempo;
float distancia;

void setup() {
  //inicialização dos pinos
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600); //inicialização da comunicação serial para debug
}

void loop() {
  //ciclo de emissão do sinal ultrassônico
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  //medição do tempo de retorno do sinal ultrassônico (em microssegundos)
  tempo = pulseIn(echo, HIGH);
  
  //cálculo da distância em centímetros (o som percorre 1 cm a cada 58 microssegundos)
  distancia = tempo / 58.0;
  
  //saída da distância no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  delay(500); //aguarda 0.5 segundos antes da próxima leitura
}
