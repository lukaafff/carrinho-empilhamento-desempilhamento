
# 👋 RFID E DISPLAY LCD


## ✈️ O código

## Visão Geral:
Este código implementa um sistema RFID (Radio-Frequency Identification) com um módulo RFID e um display LCD. O sistema permite empilhar IDs de tags RFID permitidas e, quando a pilha estiver cheia, desempilhar os IDs armazenados. O código gerencia a leitura das tags RFID, verifica se elas são permitidas, evita leituras repetidas e exibe informações no LCD.

## Componentes Utilizados:
### Módulo RFID: 
Utilizado para ler os IDs das tags RFID.
### Display LCD:
Usado para exibir mensagens e informações.
### Bibliotecas:
### LiquidCrystal_I2C.h: 
Para controlar o display LCD usando I2C.
### SPI.h:
Para comunicação SPI com o módulo RFID.
### MFRC522.h: 
Para interagir com o módulo RFID.

## Funcionalidades:
### Empilhamento de Tags:

- Quando uma tag RFID permitida é lida, seu ID é empilhado.
- O sistema mantém uma pilha de IDs das tags RFID permitidas.
#### Prevenção de Leituras Repetidas:

 O código evita a leitura repetida da mesma tag RFID permitida.

#### Desempilhamento:

Quando a pilha atinge seu limite (4 tags empilhadas), o sistema desempilha e exibe os IDs das tags no LCD.

#### Tempo de Bloqueio:

Após o desempilhamento, o sistema entra em um estado de bloqueio por um período de tempo (5 segundos), durante o qual novas leituras são ignoradas.

## Funcionamento:
### Inicialização:

- O sistema é inicializado, e a mensagem "Aproxime a tag..." é exibida no LCD.
- A pilha é inicializada vazia.
#### Leitura da Tag:

- Quando uma tag é aproximada, seu ID é lido e comparado com a lista de IDs permitidos.
- Se a tag é permitida e não foi lida anteriormente, ela é empilhada.

#### Empilhamento:

- Os IDs das tags empilhadas são exibidos no LCD com a mensagem "Empilhando:".
- Quando a pilha atinge 4 tags, o sistema aguarda 2 segundos e então desempilha as tags, exibindo "Desempilhando:" no LCD.

#### Desempilhamento:

Os IDs desempilhados são exibidos no LCD com a mensagem "Desempilhando:" por 5 segundos.

#### Bloqueio Temporário:

Após o desempilhamento, o sistema entra em um estado de bloqueio por 5 segundos antes de permitir novas leituras.

## Limitações:
- Número Máximo de Tags Empilhadas: O sistema permite empilhar no máximo 4 tags. Após atingir esse limite, as tags são desempilhadas.

- Bloqueio Temporário: Após o desempilhamento, o sistema entra em um estado de bloqueio por 5 segundos, durante o qual novas leituras são ignoradas.
