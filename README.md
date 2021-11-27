# Estimulador-Nervio-Vago-2021
Desarrollo de un módulo de estimulación para el nervio vago, y su varilla de programación.
## Modulo Estimulador
### Funcionamiento
### Codigos
Los codigos utilizados para el modulo estimulador se dividen en 2 partes. Una serie de codigos para pruebas directamente con los breakout boards (Trinket M0) y los modulos de comunicacion RF (NRF24L01). Los demas codigos son los codigos encargados de generar los pulsos PWM con una rampa de inicio y una rampa de bajada desarollados por Gustavo Ordoñez, en conjunto con la seccion de codigo encargada de generar la comunicacion por medio de RF por medio de la funcion __RFBegin__.
#### Codigos Para Pruebas de Comunicacion
De los codigos para pruebas se tienen los siguientes archivos:
* Codigos_de_Diagnostico_para_los_modulos: Este codigo imprime en la consola (COM) los parametros de transmision del modulo NRF24L01 e indica si este tiene problemas de transmision:
* Pruebas basicas de diagnostico:
  * Trinket_reciver: Esta es un prueba donde el ESP8266/ESP Wroom32 mandan una serie de caracteres y en este codigo el trinket recibe los caracteres y los muestra en la interfaz serial
  * SAMD_a_ESP8266_nRF24L01: Este es una version con mas modos de prueba que el codigo anterior. Por medio de 5 _cases_ se puede probar mandar, recibir, mandar y rebir datos de el trinket a otro microcontrolador con el modulo nEF24L01 y los _addresses_ indicados
  * Slave_NRF_tester: Un codigo que prueba mandar una serie de numeros que incrementan de manera lineal. Aqui el receptor no reporta nada, solo el emisor reporta si el envio fue exitoso o no
  * Pruebas_con_el_ESP32: Codigo de pruebas para ver si se podian recibir los datos desde el ESP Wroom 32
#### Codigo del modulo estimulador
Aqui solo se tiene un archivo de codigo comentado que es la combinacion del codigo de Gustavo y mi codigo. Este es el codigo que se le debe de cargar al modulo estimulador para poder lograr la comunicacion RF y la generacion de Pulsos
* PWM_Module_With_RF
### Modelos CAD
*De momento todavia no hay archivos cad para la carcaza del estimulador*
#### Archivos de Inventor 2021
Para poder acesar a los archivos de inventor en conjunto con las librerias de componentes, dadas las limitaciones de tamano de 100 MB para un repositorio de Github, estas se van a colocar como un archivo de texto (.txt) que contiene el link al Google Drive donde estan subidos y distribuidos por carpetas
Tambien adjunto el link aqui: 
#### STLs
Los archivos que se obtienen apartir del programa CAD, en este caso Inventor 2021, para ser introducidos al _slicer_ (software encargado de traducir el STL a codigo que puede ser interpretado por la impresora) que en torno genera G-code. Se recomendaria utilizar estos archivos para impresion de la carcasa, ya que al utilizar directamtente el G-code, puede que este no este optimizado para impresora donde se quiere imprimir.
#### G-Code
El G-Code fue generado utilizando el _slicer_ de Ultimaker, Ultimaker Cura Version 4.7. Se optimizo para poderse imprimir con una Ender 3 V1 y utilizando filamento PLA+. Es importante notar que la temperatura de impresion del PLA+ es superior a la del PLA normal. Tambien se optimizaron los comandos para reducir el _stringing_ y otros fenomenos del filamento. Se hiceron impresiones con PLA+ azul oscuro de eSUN y PLA+ cobre _silky_ tambien de eSUN. Los resultados de impresion con otros filamentos y en otras impresoras pueden ser diferentes. Sin embargo, las impresiones estan siempre puestas en el centro de la cama de impresion. La ender 3 V2 tiene un volumen de impresion de 25x25x30 cm.
### Pruebas de Neuro Estimulacion
#### Archivo de LTspice
Este archivo contiene los circuitos con los parametros de simulacion adecuados para validar los resultados teoricos
* Simulacion Nervio Vago.asc: Archivo para ser abierto en LTspice con los circuitos y parametros
* Simulacion Nervio Vago.raw: Archivo que contiene los parametros para replicar las graficas
* Simulacion Nervio Vago.plt
* Simulacion Nervio Vago.log

## Modulo Programador
### Modelos CAD 
Misma descripcion que para el modulo estimulador, pero se van a dar dos links diferentes a la misma carpeta de Google Drive para facilitar el orden.
#### Archivos de Inventor 2021
Para poder acesar a los archivos de inventor en conjunto con las librerias de componentes, dadas las limitaciones de tamano de 100 MB para un repositorio de Github, estas se van a colocar como un archivo de texto (.txt) que contiene el link al Google Drive donde estan subidos y distribuidos por carpetas
Tambien adjunto el link aqui: 
#### STLs
Los archivos que se obtienen apartir del programa CAD, en este caso Inventor 2021, para ser introducidos al _slicer_ (software encargado de traducir el STL a codigo que puede ser interpretado por la impresora) que en torno genera G-code. Se recomendaria utilizar estos archivos para impresion de la carcasa, ya que al utilizar directamtente el G-code, puede que este no este optimizado para impresora donde se quiere imprimir.
#### G-Code
El G-Code fue generado utilizando el _slicer_ de Ultimaker, Ultimaker Cura Version 4.7. Se optimizo para poderse imprimir con una Ender 3 V1 y utilizando filamento PLA+. Es importante notar que la temperatura de impresion del PLA+ es superior a la del PLA normal. Tambien se optimizaron los comandos para reducir el _stringing_ y otros fenomenos del filamento. Se hiceron impresiones con PLA+ azul oscuro de eSUN y PLA+ cobre _silky_ tambien de eSUN. Los resultados de impresion con otros filamentos y en otras impresoras pueden ser diferentes. Sin embargo, las impresiones estan siempre puestas en el centro de la cama de impresion. La ender 3 V2 tiene un volumen de impresion de 25x25x30 cm.
## Open OCD
En esta carpeta estan tanto los archivos de Altium asi como los Gerbers necesarios para la fabricacion de la placa que se uso para cargar el _bootloader_ al SAMD21E18-AF. 
Se utilizaron los siguientes links para poder cargar OpenOCD en una Rapberry Pi 3B+ y generar el archivo .hex que contiene el bootloader del arduino zero y el uf2 para el trinket M0:
* [Link para adaptar un Trinket M0 a un Nintendo Switch](http://www.google.fr/ "Named link title")
* [Video de como instalar OpenOCD en la RaspberryPi y cargar el archivo .hex](http://www.google.fr/ "Named link title")
* [Tutorial de Adafruit de como usar OpenOcd](http://www.google.fr/ "Named link title")
## Estudio de Mercado para La bio-compatibilidad
Este estudio esta presentado en un archivo de Microsoft Excel .xlxs y tiene diversas partes. Una parte del estudio encuadra el listado de los 8 materiales y por medio de tres fuentes de lugares diferentes (locales de ser posible) determina el precio/g del material. Otra parte esta donde se indican las caracteristicas a ser comparadas a mayor detalle. Tambien hay un recuadro donde se catalogan las propiedades mecanicas relevantes a la ergonomia y fabricacion de los materiales. Otra seccion explica los criterios de biocompatibilidad y explica porque se determino que el material tenia que ser bio-inerte. Por ultimo se tiene el recuadro donde se ejemplifica el estudio de mercado con los resultados para cada material
## Documentacion
### Excel con el resumen de todas las corridas de las pruebas de simulacion de nervio vago
Este archivo contiene la compilacion de las diferentes corridas con las medidas en los nodos mas importantes. Se tienen 4 paginas, una para la corrida de un solo nodo (extension de 1cm de tejido) y la otra con 5 nodos (extension de 6 cm de tejido) ademas de una pagina donde se realizaron pruebas transitorias y otra donde se deriva el despeje matematico del circuito
