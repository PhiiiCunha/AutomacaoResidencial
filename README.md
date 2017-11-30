INTEGRANTES:
- Felipe Barbosa      RA: 20748825
- Felipe Negri        RA: 20970332
- Leonardo Sanches    RA: 20982131
- Philipe Cunha       RA: 20931241
- Thales Lima         RA: 20777107




INTRODUÇÃO   	   
            Com o avanço tecnológico as pessoas estão cada vez mais buscando praticidade, comodidade e segurança para seus lares. A automação residencial vem ganhando espaço no mercado não só pelo status e modernidade que proporciona, mas também por permitir uma melhor utilização de recursos naturais, diminuindo assim as despesas para se manter uma residência.   
            Este trabalho consiste em um sistema de controle redirecionado à automação residencial. O software desenvolvido irá trabalhar juntamente com um microcontrolador (Arduino), que estará trabalhando no gerenciamento e controle dos dispositivos ligados a ele (Leds, Relés, Sensores, etc), oferecendo ao usuário, um ambiente simples e prático para o controle de sua residência.   
   
	   
MICROCONTROLADOR ARDUINO   
            O microcontrolador Arduino, é uma plataforma de prototipagem eletrônica de hardware livre e de placa única, projetada com um microprocessador Atmel com suporte de I/O’s embutido, uma linguagem de programação padrão que é essencialmente C/C++ ( Existe alguns softwares que possibilita o desenvolvimento com linguagens Python, Ladder fazendo a conversão para linguagem padrão do microcontrolador). O objetivo do projeto foi e ainda é criar ferramentas, projetos ou qualquer tipo de produto que seja acessível, com baixo custo, flexível e fácil de ser manuseado por pessoas consideradas amadores na área de tecnologia e/ou da eletrônica. Principalmente também, para aqueles que não teriam alcance aos controladores mais sofisticados e de ferramentas mais complexas.   
            Pode ser usado para o desenvolvimento de objetos interativos independentes, ou ainda para ser conectado a um “computador hospedeiro”. Uma típica placa Arduino é composta por um controlador, algumas linhas de I/O digital e analógica, além de uma interface serial ou USB, para interligar-se ao hospedeiro, que é usado para programá-la e interagi-la em tempo real. Ela em si não possui qualquer recurso de rede, porém é comum combinar um ou mais sensores, módulos e Shields apropriados para que haja tal comunicação se necessário.   
   
Documentário Arduino:   
http://www.openculture.com/2011/01/arduino_documentary_open_source_hardware_is_here.  
html   
   
SENSORES   
   
DISPLAY LCD 16x2 I2C   
Módulos de display LCD de caracteres alfanuméricos são interfaces de comunicação visual muito úteis e atraentes. Eles se encontram em quase todos os aparelhos domésticos, eletroeletrônicos, automóveis, instrumentos de medição etc. São dispositivos que possuem interfaces elétricas padronizadas e graças a esses recursos internos gráficos e de software que permitem facilmente a troca de um display por outros de inúmeros fabricantes, sem que necessariamente, seja preciso alterar o programa de aplicação. Por ser altamente padronizado seu custo é baixo. É um recurso antigo, mas continua atual, com suas inúmeras formas, cores, tamanhos e preços. A tecnologia predominante continua sendo o LCD (Liquid Crystal Display – Display de Cristal Liquido), porém já se pode encontrar alguns baseados em LEDs orgânicos (OLED).   
            I2C - O protocolo I2C (Inter-Integrated Circuit – Circuito Inter-Integrado) descreve o funcionamento de um barramento de comunicação serial que utiliza  apenas dois fios o SDA (Serial Data – Dados Seriais) e o SCL (Serial Clock – Clock Serial). Inventado pela Philips no início da década de 90, este protocolo é muito utilizado para conectar periféricos de baixa velocidade como por exemplo, os microcontroladores.    
           O SDA é o pino que efetivamente transfere os dados, e o SCL serve para temporização  entre os dispositivos (Clock para leitura), de modo que a comunicação pela SDA possa ter  confiabilidade sem que nenhum dado seja perdido ou transferido de forma errada. Tanto o  envio, quanto a recepção de dados é realizada utilizando a linha serial de dados, ou seja, é uma  linha bi-direcional de comunicação, que em um ciclo de tempo estará enviando dados por este  pino e outro estará recebendo dados.   
     
DHT11   
O Sensor de Umidade e Temperatura DHT11 é um sensor de temperatura e umidade que permite fazer leituras de temperaturas entre 0 a 50 Celsius e umidade entre 20 a 90%, muito usado em projetos que tem um microcontrolador Arduino.    
O elemento sensor de temperatura do DHT11 é um termistor do tipo NTC (Negative Temperature Coeficient – Coeficiente de Temperatura Negativa) e o sensor de Umidade é do tipo HR202 ( resistor sensível feito de materiais de macromoléculas orgânicos que através da umidade, gera resistencia elétrica variada igualada a porcentagem de umidade), o circuito interno faz a leitura dos sensores e se comunica com o microcontrolador através de um sinal serial de uma via uma porta analógica. A única desvantagem real desse sensor é que para poder obter novos dados dele terá que ser feito uma leitura a cada 2 segundos.   
   
   
RTC DS3231   
O Real Time Clock (RTC) DS3231 é um relógio de tempo real de alta precisão e baixo consumo de energia. Em sua placa vem embutido um sensor de temperatura e um cristal 
oscilador para melhorar sua exatidão tanto em contagem quanto na “captura” de temperatura.   
Este módulo DS3231 é capaz de fornecer informações como segundo, minutos, dia, data, mês e ano (de 2000 até 2099). Correções como meses com menos de 31 dias e anos bissextos são corrigidos automaticamente e pode operar tanto no formato 12 horas como 24 horas. Em caso de falha de energia o DS3231 automaticamente aciona a bateria que acompanha o módulo para evitar perda de dados. Endereço e informações são transferidas via protocolo I2C.   
   
RFID- RC 552   
RFID (Radio Frequency Identification - Sistema de Identificação por Radiofrequência) é uma tecnologia capaz para identificar, rastrear e gerenciar diversos produtos, documentos, animais ou até mesmo pessoas. Geralmente, um RFID é composto por um Leitor com Antena, um Transponde ( TAG, RF TAG, Etiqueta eletrônica) e um processador dedicado com funções especializadas para esse tipo de comunicação, que possui uma memória onde são armazenados os dados, os quais são enviados ao microcontrolador Arduino.    
As três frequências mais utilizadas para o sistema RFID passivo são: baixa frequência (LF), de 125kHz, alta frequência (HF), 13,56 MHz, e ultra alta frequência (UHF), operando na faixa de 860 a 960MHz. Um dos aspectos mais importantes do sistema RFID é a frequência de operação e o tipo de transponder. Em geral quando se trata de RFID, a frequência define a taxa de transferência de dados entre a etiqueta e o leitor ( O mais comum das TAG utilizadas quando 
o assunto é RFID e Arduino, são TAG’s de baixa e alta frequência).   
Existem transponders passivos, semipassivos e ativos. Neste trabalho, foram utilizados apenas transponders passivos que quando aproximamos o mesmo do leitor, o campo de radiofrequência do leitor é ativado e alimenta o transponder através das ondas, que só então, pode transmitir dados da sua memória para o leitor e vice-versa.   
BARREIRA ÓPTICA   
O sensor óptico de barreira funciona da seguinte forma, o sensor (neste caso o LED) que possui função emissora ira emitir uma luz infravermelha da qual irá passar pelo objeto que está em sua área de atuação, essa luz infravermelha chegará no sensor receptor (Outro LED) com a informação de que existe ou não um objeto na área sensorial do aparelho, após essa passagem o sensor ira transformar a energia obtida em alguma outra forma de energia desejada (de acordo com a função que lhe foi designada em seu circuito elétrico) enviando assim para o arduino que houve uma detecção de passagem.   
É importando ressaltar que, objetos de cores transparentes não são detectados devido a luz infravermelha ultrapassá-lo sem modificações nenhuma ou com modificações mínimas dos raios, sendo assim a luz infravermelha chega no sensor receptivo de forma muito semelhante ao que foi emitido.   
     	   
PROJETO  

FUNCIONAMENTO   
O Sistema de controle Residencial tem como funcionalidade tratar sinais analógicos do ambiente em que o seu usuário convive e informar de forma digital, todos esses parâmetros encontrados. Esses valores são adquiridos através de sensores ( Temperatura, Umidade, Alarme, etc).   
 Em “forma” uma de IHM ( Interface Homem-Máquina) o sistema interage com o usuário por um display e o inverso acontece através de botões ou um controle remoto. Sendo assim, o usuário pode fazer operações de controle em sua residência, desde, deixando um processo totalmente automático ou o mesmo ser apenas manual.   
   
POSSÍVEIS FALHAS   
No sistema de controle residencial, um problema que existe hoje é a leitura da variável de leitura do tempo desde que o arduino foi ligado, cálculos realizados mostra que a variável iria “estourar” o seu tamanho em cerca de 30 - 40 dias ligado sem parar. Para corrigir tal problema, o processo de leitura dessa variável deve ser feita através do Timer OU Interrupção que existe no arduino.   
   
COMPONENTES   
- 1 Arduino Mega 2560   
 	    	- 1 Sensor DHT11   
- 1 Módulo RFID-RC552   
- 1 Módulo RTC DS3231   
- 1 Display LCD 16x2 I2C   
- 1 Módulo Relé   
- 1 Receptor IR   
- 1 Sensor de Barreira Óptica   
- LED’s diversos   
- Fios diversos(Jumpers M/M e M/F)   
- Resistores diversos   
- Botões   
- Buzzer 

CIRCUITO ELÉTRICO

![ScreenShot](https://raw.github.com/PhiiiCunha/AutomacaoResidencial/branch/path/circuito_eletrico.png)

CONCLUSÃO   
Este artigo buscou apresentar o desenvolvimento de um sistema de baixo custo, que automatize os processos residenciais, com utilização do microcontrolador Arduino Mega e seus sensores e módulos ( RFID, RTC, DHT11, etc).   
Fica claro a evidencia que é possível automatizar uma residência sem ter que investir muito, pois, além da variedade de preços dos equipamentos, cada projeto em si tem sua particularidade e complexidade própria. Vale ressaltar também que, todo o equipamento eletrônico está exposto e sujeito a falhas e contra tempos decorrentes a defeitos de fábrica, interferências, falhas na comunicação e curto circuito. Na parte lógica, podem ocorrer falhas de programação ou entendimento lógico errado.   
   
REFERÊNCIAS BIBLIOGRÁFICAS 
   
HTTP://WWW.OPENCULTURE.COM/2011/01/ARDUINO_DOCUMENTARY_OPEN_SOURCE_HARDWARE_IS_HERE.HTML   
   
HTTPS://PT.WIKIPEDIA.ORG/WIKI/ARDUINO   
   
HTTP://WWW.EBAH.COM.BR/CONTENT/ABAAAFIF4AE/TCC-AUTOMACAO-RESIDENCIAL   
   
HTTPS://WWW.EMBARCADOS.COM.BR/MODULO-DE-DISPLAY-LCD/   
   
HTTP://WWW.ARDUINOBR.COM/ARDUINO/I2C-PROTOCOLO-DE-COMUNICACAO/   
   
HTTP://ROBOCRAFT.RU/FILES/DATASHEET/DHT11.PDF   
   
HTTP://WWW.IF.UFRGS.BR/MPEF/MEF004/20061/CESAR/SENSORES-TERMISTOR.HTML   
   
HTTPS://WWW.ELECROW.COM/DOWNLOAD/HR202%20HUMIDITY%20SENSOR.PDF   
   
HTTP://BRASIL.RFIDJOURNAL.COM/ARTIGOS/VISION?9591   

