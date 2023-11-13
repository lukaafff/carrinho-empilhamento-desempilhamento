
# 👋 Carrinho Autônomo


## ✈️ O projeto

## Visão Geral:
O projeto consiste em um carrinho autônomo projetado para seguir uma linha branca em um plano preto. O carrinho incorpora sensores e funcionalidades que o tornam capaz de navegar autonomamente por um caminho pré-definido, interagir com objetos detectados por um sensor ultrassônico e realizar operações controladas por tags RFID.

## Componentes Utilizados:
### Sensores Infravermelhos (3):

Três sensores infravermelhos analógicos são utilizados para permitir que o carrinho siga uma linha branca em um plano preto. Esses sensores são responsáveis por fornecer informações sobre a posição do carrinho em relação à linha.

### Motores DC (4) e Rodas (4):

Quatro motores DC e quatro rodas são empregados para possibilitar o movimento do carrinho. Eles são controlados pela ponte H L298, permitindo o ajuste da direção e velocidade do carrinho.
Ponte H L298:

A **ponte H L298** é utilizada para controlar os motores DC. Ela permite a inversão de polaridade nos motores, possibilitando que o carrinho siga diferentes direções.

### Arduino Uno R3:

O Arduino Uno R3 atua como o cérebro do projeto, processando os dados dos sensores e controlando os motores. Ele executa o código que define o comportamento autônomo do carrinho.

### Sensor Ultrassônico:

Um sensor ultrassônico é implementado para detectar obstáculos em um raio de 20 centímetros

### Sensor RFID:

O sensor RFID é responsável pela leitura das tags RFID. Ele permite a interação do carrinho com as tags, desencadeando ações específicas durante seu movimento autônomo.

### Tags RFID (5):

Cinco tags RFID são utilizadas como marcadores ao longo do caminho do carrinho. Cada tag contém informações que o carrinho lê durante sua operação autônoma.

### Display 16x2 com I2C:

Um display LCD 16x2 com interface I2C é empregado para fornecer uma interface visual, exibindo informações sobre as ações do carrinho, o status da leitura das tags e o empilhamento/desempilhamento.

### Protoboard:

A protoboard é usada para realizar as conexões elétricas entre os componentes, facilitando a montagem e desmontagem do circuito.

### Baterias 9V e Interruptor:

Baterias 9V alimentam o sistema, fornecendo energia para os motores e demais componentes. Um interruptor é utilizado para ligar ou desligar o sistema de forma conveniente.

### Jumpers:

Jumpers são utilizados para estabelecer as conexões elétricas entre os diferentes componentes, garantindo a comunicação adequada e a transmissão de dados e energia.

## Funcionalidades do Código

### Bibliotecas:
**Wire.h:**
Para comunicação I2C.

**LiquidCrystal_I2C.h:**
Para controlar o display LCD usando I2C.

**hd44780.h e hd44780_I2Cexp.h:**
Extensão do LiquidCrystal_I2C, ele contém funções a mais.

**SPI.h:**
Para comunicação SPI com o módulo RFID.

**MFRC522.h:**
Para interagir com o módulo RFID.

### Definição de Componentes e Variáveis:
**Sensores Infravermelhos:**
Define os pinos analógicos para os sensores direito, central e esquerdo, e cria variáveis para armazenar os dados lidos por esses sensores.

**Limiar:**
Estabelece um limiar para diferenciar entre o branco e o preto na linha.

**Motores e Direções:**
Define pinos e velocidades para os motores do carrinho, assim como funções para movimentá-lo em diferentes direções.

**Ultrassônico:**
Configura os pinos para o sensor ultrassônico e variáveis para armazenar dados sobre a distância.

**RFID:**
Define os pinos para o módulo RFID e configuração do display LCD.

**Temporizadores:**
Variáveis para controle de tempo durante empilhamento, desempilhamento e bloqueio temporário.

**Pilha:**
Estrutura da pilha para armazenar IDs das tags RFID.

### Funções de Movimentação do Carrinho:
**PARADO():**
Para todos os motores, mantendo o carrinho parado.

**FRENTE() e FRENTE2():**
Movimenta o carrinho para frente com diferentes velocidades.

**ESQUERDA(), DIREITA(), e TRAS():**
Movimenta o carrinho para esquerda, direita e ré, respectivamente.

### Lógica de Navegação Baseada em Sensores:
Utiliza a leitura dos sensores infravermelhos para tomar decisões sobre o movimento do carrinho. Por exemplo, se o sensor central está na linha branca e os sensores direito e esquerdo estão na linha preta, o carrinho segue em frente.

### Controle de Obstáculos com Ultrassônico:
Utiliza o sensor ultrassônico para detectar obstáculos a uma distância de até 20 cm. Se a distância for menor ou igual a 20 cm, o carrinho para.

### Controle RFID:
Integra a leitura de tags RFID ao sistema. O código verifica se a tag lida é permitida, evita leituras repetidas e empilha os IDs das tags permitidas. Quando a pilha atinge 5 tags, o carrinho desempilha e exibe os IDs no LCD.

### Exibição no Display LCD:
Controla a exibição de mensagens e informações no display LCD, como "Buscando...", "Empilhando:", e "Desempilhando:". Mostra também os IDs empilhados durante o processo.

## Empilhar e Desempilhar

- Estrutura da Pilha **(struct Pilha)**:

A estrutura da pilha é definida como uma estrutura **(struct)** chamada **Pilha**, que possui dois membros:

**dados:** Um array de inteiros com tamanho 4. Este array é utilizado para armazenar os dados da pilha, que, no contexto do código, representam os IDs das tags RFID.

**topo**: Um inteiro que indica o índice do topo da pilha. Quando a pilha está vazia, o **topo** é inicializado com -1.

- Inicialização da Pilha **(void inicializarPilha())**:

A função **inicializarPilha** é responsável por configurar a pilha no início do programa. Nela, o **topo** da pilha é definido como -1, indicando que a pilha está vazia.

- Empilhamento **(void empilhar(int dado))**:

A função **empilhar** é utilizada para adicionar um novo ID à pilha. Antes de realizar o empilhamento, ela verifica se a pilha não está cheia, ou seja, se o **topo** é menor que 4 (o limite é 5 IDs). Se a condição for atendida, o **topo** é incrementado, e o novo ID é armazenado no array **dados** no índice correspondente ao **topo**.

- Desempilhamento **(int desempilhar())**:

A função **desempilhar** é responsável por remover o ID do topo da pilha e retorná-lo. Antes de desempilhar, ela verifica se a pilha não está vazia, ou seja, se o **topo** é maior ou igual a 0. Se a condição for atendida, o ID do topo é armazenado em uma variável **dado**, o **topo** é decrementado, e o valor de **dad**o é retornado. Se a pilha estiver vazia, a função retorna -1, indicando que a pilha está vazia.

**Uso de Arrays em Vez de Ponteiros:**
A escolha de utilizar um array **(int dados[5])** em vez de ponteiros na estrutura da pilha pode ser atribuída à simplicidade e à praticidade do contexto em que a pilha é utilizada no código. Em geral, o uso de arrays em estruturas é preferido quando se tem um número fixo de elementos e quando a manipulação direta desses elementos é mais conveniente.

No caso específico desta aplicação, a pilha tem um tamanho fixo de 5 elementos (IDs de tags RFID). O acesso direto aos elementos da pilha é simplificado por meio do uso de um array, e não há necessidade de gerenciar a alocação dinâmica de memória, que seria mais comum com ponteiros.

## Controle de Tempo para Desempilhamento e Empilhamento:
No código várias variáveis são utilizadas para controlar o tempo durante os processos de desempilhamento e empilhamento, garantindo uma execução controlada e temporizada.

### Tempo de Bloqueio:

- **tempoBloqueio** mantém o controle do tempo decorrido desde o último desempilhamento. Durante este período, o sistema permanece bloqueado, ignorando novas leituras de tags RFID.

- **tempoBloqueioDuracao** define por quanto tempo o sistema deve permanecer bloqueado após o desempilhamento. Neste caso, é definido como 1 segundo (1000 milissegundos).

### Empilhamento:

- **empilhandoStartTime** é utilizado para registrar o momento inicial quando o processo de empilhamento é iniciado.

- **empilhandoDuration** define por quanto tempo o empilhamento deve ocorrer. Este valor controla o tempo durante o qual o carrinho continua empilhando tags RFID antes de desempilhar e reiniciar o processo.

## Display

A função **lcd.lineWrap();** encontrada na biblioteca **hd44780** é responsável por controlar o comportamento de quebra de linha no display LCD. Quando ativada, ela permite que o texto automaticamente "pule" para a linha abaixo quando atinge o limite máximo de colunas no display, que neste caso é de 16 colunas. Essa funcionalidade é especialmente útil para exibir textos mais longos ou para facilitar a exibição de informações de forma mais legível no display LCD, garantindo que o texto não ultrapasse os limites horizontais do display.

## Leitura dos Sensores Infravermelhos

Os sensores são utilizados para detectar a posição da linha branca em relação ao carrinho, permitindo que ele ajuste sua trajetória conforme necessário.

### Variáveis Utilizadas
- **Dados_SensorCENTRAL:** Armazena o valor analógico lido pelo sensor infravermelho central.

- **Dados_SensorDIREITO:** Armazena o valor analógico lido pelo sensor infravermelho direito.

- **Dados_SensorESQUERDO:** Armazena o valor analógico lido pelo sensor infravermelho esquerdo.

- **limiar:** Valor de limiar para diferenciar a linha branca da linha preta.

### Lógica de Movimento

- Se o sensor central estiver na linha branca e os sensores direito e esquerdo estiverem na linha preta, o carrinho é orientado para **frente.**
- Se o sensor direito estiver na linha branca e os sensores central e esquerdo estiverem na linha preta, o carrinho **vira à direita.**
- Se o sensor esquerdo estiver na linha branca e os sensores central e direito estiverem na linha preta, o carrinho **vira à esquerda.**

Essa lógica permite que o carrinho ajuste sua trajetória de acordo com a posição da linha branca em relação aos sensores infravermelhos.

## Sensor Ultrassônico

 O sensor é utilizado para medir a distância entre o carrinho autônomo e um objeto à sua frente. Se a distância for menor ou igual a 20 cm, o carrinho será parado.

O código aciona o sensor ultrassônico e calcula a distância em centímetros até um objeto. O tempo de viagem do pulso ultrassônico é medido e convertido em distância.

Se a distância medida for menor ou igual a 20 cm, o carrinho é parado pela função **PARADO()**. A variável **carrinhoParado** é definida como verdadeira para indicar que o carrinho está parado.

## RFID

A declaração **const String tagsPermitidas[]** representa uma lista de IDs de tags RFID que são consideradas permitidas pelo sistema. Cada elemento dentro dessa lista é uma string que representa o identificador único (ID) de uma tag RFID. Neste caso, as tags RFID permitidas são:

- "ED:75:38:04"
- "CC:07:39:04"
- "D6:60:38:04"
- "76:26:38:04"
- 

Essa lista é utilizada para verificar se a tag RFID lida pelo sistema pertence a uma das tags que são consideradas válidas. Quando uma nova leitura de tag ocorre, o sistema compara o ID da tag com os IDs presentes nesta lista para determinar se a tag é permitida. Essa verificação é feita na parte do código onde há a análise do UID (identificador único) lido da tag RFID.

Caso o UID da tag lida esteja na lista **tagsPermitidas**, o sistema considera a tag como permitida e realiza as ações correspondentes. Se a tag não estiver na lista, o sistema pode exibir uma mensagem indicando que a tag não é permitida e agir de acordo com a lógica definida para essa situação.

O identificador único (UID) de uma tag RFID geralmente é uma sequência de bytes representada em formato hexadecimal. Esses bytes são uma representação única e exclusiva da tag. Existem várias maneiras de obter o UID de uma tag RFID

- A parte específica do código que obtém o UID da tag é a seguinte:

```
String strID = "";
for (byte i = 0; i < rfid.uid.size; i++)
{
  strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) + (i != rfid.uid.size - 1 ? ":" : "");
}

```

- **rfid.uid.size:** Indica o tamanho do UID da tag.
- **rfid.uid.uidByte[i]:** Obtém o byte individual do UID na posição i.
- **String(rfid.uid.uidByte[i], HEX):** Converte o byte para uma representação hexadecimal em formato de string.
- **(rfid.uid.uidByte[i] < 0x10 ? "0" : ""):** Adiciona um zero à esquerda se o byte for menor que 0x10 para garantir que a representação hexadecimal tenha dois dígitos.

Assim, a concatenação desses valores individuais de byte em formato hexadecimal com ":" entre eles resulta no UID completo da tag em formato de string, que é então armazenado na variável strID.

### Lógica do RFID e Controle da Pilha

O código lida com a funcionalidade do RFID (Radio-Frequency IDentification) e o controle da pilha de IDs das tags RFID permitidas.

### Operação Geral
**Verificação do Tempo de Bloqueio:**

- Antes de realizar qualquer leitura RFID, verifica se o tempo de bloqueio após desempilhar foi atingido.
- Se o tempo de bloqueio não foi atingido, o carrinho não realiza leituras RFID para evitar problemas de sobreposição.

**Leitura da Tag RFID:**

- Utiliza a biblioteca RFID para verificar se um novo cartão RFID está presente e lê o seu identificador (UID).

**Validação da Tag:**

- Verifica se a tag lida é permitida ou se já foi lida anteriormente.
- Exibe mensagens no LCD conforme a situação: "Tag já lida", "Tag não permitida" ou realiza o empilhamento da tag.

**Controle da Pilha:**

- Atualiza a pilha com os IDs das tags permitidas.
- Exibe os dados empilhados no LCD.
- Quando a pilha está cheia, exibe os números empilhados por 10 segundos antes de desempilhar.

**Retomada do Movimento:**

- Se o carrinho estava parado e a distância é maior que 20 cm, ou a tag foi lida com sucesso retoma o movimento.