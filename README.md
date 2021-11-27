# Estimulador-Nervio-Vago-2021
Desarrollo de un módulo de estimulación para el nervio vago, y su varilla de programación. En este repositorio se encuentran la combinacion de todos los avances logrados en la Fase III del desarollo de un prototipo de estimulador de nervio vago de la Universidad del Valle de Guatemala. Los aportes estan basados en el trabajo de las fases pasadas del proyecto. En esta fase se logro establecer la comunicacion inalambrica de los dos modulos que conforman el estimulador VNS. Ademas se hicieron estudios para determinar el material de desarollo para la carcasa del modulo estimulador, asi como un estudio para validar los parametros de descarga del mismo. Asi mismo se hicieron mejoras a ambos modulos por medio de diseno 3D y diseno de circuitos impresos. 
## Modulo Estimulador
### Funcionamiento
### Codigos
Los codigos utilizados para el modulo estimulador se dividen en 2 partes. Una serie de codigos para pruebas directamente con los breakout boards (Trinket M0) y los modulos de comunicacion RF (NRF24L01). Los demas codigos son los codigos encargados de generar los pulsos PWM con una rampa de inicio y una rampa de bajada desarollados por Gustavo Ordoñez, en conjunto con la seccion de codigo encargada de generar la comunicacion por medio de RF por medio de la funcion __RFBegin__ y la funcion __RFSetup__.
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
### Codigos
Los codigos utilizados para la varilla programadora se dividen en 3 partes. Al igual que el modulo estimulador se tienen codigos para pruebas de comunicacion por medio del modulo NRF24L01. Luego se tiene el codigo principal que le da funcionalidad WiFi y RF a la varilla programadora. Por ultimo esta el codigo de la interfaz grafica que le deja al usuario programar los parametros por medio de una consola corriendo una aplicacion de Python.
### Codigo de la varilla programadora
* Compu_a_Esp_a_SAMD21: Este codigo
### Codigo para la interfaz grafica
* Interfaz_VNS.py
### Codigo para pruebas de comunicacion
* Codigos de Diagnostico
 * I2C_Scanner: Da la direccion de cualquier dispositivo conectado por medio de I2C a el ESP8266
 * lcd_i2c-test: Pruebas para verificar que los datos mandados por I2C aparezcan en la pantalla LCD 1602
 * Master_NRF_Tester: Codigo hermano del Slave_NRF_tester que muestra en la pantalla serial si se estan recibiendo exitosamente una serie de numeros incrementales, si no es el caso avisa que hubo un error de comunicacion de la forma "TX Failed"
*  ESP8266_a_SAMD21_nRF4L01: Codigo hermano de SAMD_a_ESP8266_nRF24L01 que se utiliza para hacer pruebas de emision, recepcion, emision y recepcion e inclusive pruebas de la pantalla LCD por medio de I2C para el ESP8266 y el ESP Wroom32
*  Trinket_Como_Receptor_de_los_Parametros: Codigo de Pruebas basicas para recibir datos en forma de un array desde el Arduino o desde el ESP8266
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
