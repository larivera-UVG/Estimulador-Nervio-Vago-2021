/* PWM_Module_With_RF
 * 
 * Codigo para el modulo de estimulacion VNS. Este codigo esta disenado para que un microcontrolador SAMD21E18 genere pulsos
 * PWM deacuerdo con la definicion de parametros del usuario.
 * Ademas de eso tambien adapta el modulo estimulador para funcionar con cualquier modulo tipo NRF24L01 y de esa manera recibir los parametros de programacion
 *Autores:
 * Rodrigo Jose Figueroa Hernandez - 161678  Fase III
 * Gustavo Ordoñes - 16357 Fase II
 * Ingeniería Mecatrónica/Electronica - UVG

  */
// ............................................. Librerias usadas para el modulo NRF24L01 .............................
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ............................................. Variables de comunicacion por RF usando NRF24L01 ......................
#define CE_PIN   PORT_PA18 //
#define CSN_PIN  PORT_PA17//
RF24 radio(CE_PIN, CSN_PIN); // Instancia del módulo RF. CE = pin PA18, CSN = pin PA17
const byte addresses [][6] = {"00001", "00002"}; // Direcciones de escritura y lectura 
int contSec = 0;
uint8_t dataBus[6]; // Bus de parámetros
uint8_t dataConfOut[2]; // Bus de confirmación
uint8_t dataControl = 0; // Variable para salir del modo de lectura

// ............................................. PWM and Timer Formulas ................................................

// PWM frequency:
// freq = GCLKx_freq/(TCCx_Prescaler*(1+TOP_Value))
// => Top_Value = (GCLKx_freq/TCCx_Prescaler*freq)-1

// Timer value:
// Time = (Timer_Presc*dfactor/GCLKx_freq)*(CCx+1)
// => CCx = time*(GCLKx_freq/Timer_Presc*dfactor)-1

// Note: dfactor is a scaling factor selected in clock divider to GCLKx configuration


// ....................................... PWM and Timer Values Data Base...............................................


// Frequency configuration of the Stimulation Signal (PWM) for 30, 25, 20, 15, 10, 5, 2 and 1Hz  
const unsigned int PWM_freq[3][8] = {{1562, 1874, 2343, 3124, 4687, 9374, 23437, 46874},    // 48MHz
                                     {1041, 1249, 1562, 2082, 3124, 6249, 15624, 31249},    // 8MHz
                                     {1066, 1279, 1599, 2132, 3199, 6399, 15999, 31999}};   // 32KHz

// CCx values for 1ms, 750us, 500us, 250us and 130us respectively, for pulse 
// width or hight state in the square signal (PWM) during the Stimulation Time
const unsigned char PWM_pw[3][5] = {{47, 35, 23, 12, 6},   // 48MHz
                                    {31, 23, 16, 8, 4},    // 8MHz
                                    {32, 24, 16, 8, 4}};   // 32KHz

// Array for CCx values for 600, 300, 180, 108, 66, 60 48, 30, 21, 18, 14, 7, 2, 1s respectively, for
// On/Off time for Stimulation Signal. Clock at 32KHz
const unsigned int ON_OFF_TIME[14] = {18749, 9374, 5624, 3374, 2062, 1874, 1499, 937, 655, 562, 437, 218, 62, 30};


// ........................................ PWM and Timer Values Selection .............................................


volatile unsigned int period;         // = PWM_freq[0][3];
volatile float pulse_width;           // = period*0.25; //PWM_pw[2][0];     
volatile int ON_Time;                 // = ON_OFF_TIME[13];
volatile int OFF_Time;                // = ON_OFF_TIME[13];


// ............................... Variables for PWM, Timer and Digital Potentiometer ..................................

bool flag = 0; //Declaracion de la banderas (no es necesario cambiarlas)
byte mode = 0;
byte pulsos = 0;
bool Ramp_mode = false; 
bool go_to_sleep = false;
bool update_mode = false;
bool Hall_Sensor_Armed = false;

uint8_t amplitude;
float output_voltage = 1.75;                        // Voltage (V)
uint8_t frequency = 15;                              // Hertz (Hz)
uint16_t Stimulation_time = 1;                      // Seconds (s)
uint16_t Repose_time = 2;                          // Seconds (s)

  float magnet_output_voltage = 0.5;
uint16_t magnet_Stimulation_time = 2;

float ramp_interval;
int timer_interval;
int amplitude_max;                                  // Maximum Stimulation Amplitude (V)
int amplitude_min = 0;                              // Minimum Stimulation Amplitude (V)
 
uint16_t pot_command;
uint16_t pot_value;


// .....................................................................................................................


void setup()
{

  PM->CPUSEL.reg = PM_CPUSEL_CPUDIV(2);

//...................................... Set Input/Output Pins Mode Configuration ......................................
  
  pinMode(0, OUTPUT);                                                     // VNS Stimulation Signal Output 
  pinMode(1, INPUT_PULLUP);                                               // Input for Hall Sensor (open drain)
  PORT->Group[PORTA].DIRSET.reg = PORT_PA03;                              // Set PA03 Pin as Output (CS SPI Communication)
  PORT->Group[PORTA].OUTSET.reg = PORT_PA03;                              // Set PA03 Pin to High
  //attachInterrupt(digitalPinToInterrupt(1), Hall_Sensor, FALLING);        // Configure Falling Interrupt for Hall Sensor
  attachInterrupt(digitalPinToInterrupt(1), Hall_Sensor, RISING);          // Configure Rising Interrupt for Hall Sensor
  RF_Config(); //Funcion que configura la velocidad de tranmision y el nivel de poder para envio y recpcion de datos
  SPI.begin();                                                            // Enable SPI Communication
  
// .....................................................................................................................

//........................................Set SPI Comunication with nRF4L01............................................
  PORT->Group[PORTA].DIRSET.reg= PORT_PA14; //Inicializamos el Pin 14 como CSN (CHip Select NOT), En salida
  PORT->Group[PORTA].OUTSET.reg = PORT_PA14;                        // Set CSN Pin to HIGH
  PORT->Group[PORTA].DIRSET.reg= PORT_PA15;// Inicializamos el Pin 15 como CE(Control data transmission), En salida
  
//....................................................................................................................

  amplitude_max = output_voltage*(255/3.3)+4;
  
  switch (frequency) 
  { 
    case 1:
      period = PWM_freq[1][7];    
      break;
    case 2:
      period = PWM_freq[1][6];
      break;       
    case 5:
      period = PWM_freq[1][5];
      break;       
    case 10:
      period = PWM_freq[1][4];
      break;
    case 15:
      period = PWM_freq[1][3];
      break;
    case 20:
      period = PWM_freq[1][2];
      break;
    case 25:
      period = PWM_freq[1][1];
      break; 
    case 30:
      period = PWM_freq[1][0];
      break;                                           
    default:
      period = PWM_freq[1][7];
      break;
  }

  //pulse_width = PWM_pw[1][4];
  pulse_width = period*0.5;

  switch (Stimulation_time) 
  { 
    case 1:
      ON_Time = ON_OFF_TIME[13];    
      break;
    case 2:
      ON_Time = ON_OFF_TIME[12]; 
      break;       
    case 7:
      ON_Time = ON_OFF_TIME[11]; 
      break;       
    case 14:
      ON_Time = ON_OFF_TIME[10]; 
      break;
    case 18:
      ON_Time = ON_OFF_TIME[9]; 
      break;
    case 21:
      ON_Time = ON_OFF_TIME[8]; 
      break;
    case 30:
      ON_Time = ON_OFF_TIME[7]; 
      break; 
    case 48:
      ON_Time = ON_OFF_TIME[6]; 
      break;    
    case 60:
      ON_Time = ON_OFF_TIME[5]; 
      break;                 
    case 66:
      ON_Time = ON_OFF_TIME[4]; 
      break; 
    case 108:
      ON_Time = ON_OFF_TIME[3]; 
      break;     
    case 180:
      ON_Time = ON_OFF_TIME[2]; 
      break;
    case 300:
      ON_Time = ON_OFF_TIME[1]; 
      break;          
    case 600:
      ON_Time = ON_OFF_TIME[0]; 
      break;     
    default:
      ON_Time = ON_OFF_TIME[7];
      break;
  }


  switch (Repose_time) 
  { 
    case 1:
      OFF_Time = ON_OFF_TIME[13];    
      break;
    case 2:
      OFF_Time = ON_OFF_TIME[12]; 
      break;       
    case 7:
      OFF_Time = ON_OFF_TIME[11]; 
      break;       
    case 14:
      OFF_Time = ON_OFF_TIME[10]; 
      break;
    case 18:
      OFF_Time = ON_OFF_TIME[9]; 
      break;
    case 21:
      OFF_Time = ON_OFF_TIME[8]; 
      break;
    case 30:
      OFF_Time = ON_OFF_TIME[7]; 
      break; 
    case 48:
      OFF_Time = ON_OFF_TIME[6]; 
      break;    
    case 60:
      OFF_Time = ON_OFF_TIME[5]; 
      break;                 
    case 66:
      OFF_Time = ON_OFF_TIME[4]; 
      break; 
    case 108:
      OFF_Time = ON_OFF_TIME[3]; 
      break;     
    case 180:
      OFF_Time = ON_OFF_TIME[2]; 
      break;
    case 300:
      OFF_Time = ON_OFF_TIME[1]; 
      break;          
    case 600:
      OFF_Time = ON_OFF_TIME[0]; 
      break;     
    default:
      OFF_Time = ON_OFF_TIME[0];
      break;
  }

  if (frequency >= 10) Ramp_mode = true; 
  else Ramp_mode = false;  

  
//...................................... Configurate PWM (Stimulation signal) ..........................................

  PWM_Config();
  Enable_PWM_Interrupts();  
  
// ......................... CONFIGURAR TIEMPO DE ACTUALIZACIÓN DE LA RAMPA ............................................

if (Ramp_mode == true)
{
  // CCx = (time*(8MHz/1024))-1
  // 1/time = (amplitude_max-1)/2000ms   
   
  amplitude = amplitude_min;                      // Ramp-up Starts in Amplitude = 1
  ramp_interval = (amplitude_max-1)/2;            // (98-1)/2s = 48.5Hz => 20.62ms
  timer_interval = (7812.5/ramp_interval)-1;      // (7812.5/ramp_interval)-1;     // (46,875.0/48.5)-1 = 965.49 => 965 

  Timer2_Config();                                // Configurate Timer for Ramp Amplitude Refresh
  Timer2_Interrupts();                            // Enable Ramp Amplitude Refresh
  Timer2_Start();                                 // Start Timer for Ramp Amplitude Refresh
}
else
{
  amplitude = amplitude_max;
}

//...................................... Configurar Timer (ON/OFF Time) ................................................

  Timer_Config();           
  Timer_Start();    

// .....................................................................................................................
  
  flag=1;
// .....................................................................................................................

//SPIClass SPI1 (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

}


// .....................................................................................................................


void loop() 
{
//........................................Funcion de recepcion de parametros............................................
  RF_Begin();
//.....................................................................................................................
  if(update_mode)
  {
    next_mode();
    update_mode = false;  
  }

  if(flag)
  {
    setResistance(amplitude);     
    flag = false;
  }

  if(go_to_sleep)
  {
    go_to_sleep = false;
    SAMD21_Sleep();    
  }
  
}


// ............................. HALL Sensor Activation for On Demand Mode .............................................


void Hall_Sensor()
{

  mode = 0;
  flag = 0;
  pulsos = 0;
  go_to_sleep = false;
  
  if(Ramp_mode) amplitude = amplitude_min;
  else amplitude = amplitude_max;

  flag = true;
  update_mode = true;
  
}

// ............................. TIMER DE ACTUALIZACIÓN DE AMPLITUD DE LA RAMPA ........................................


void TCC0_Handler()
{

  if(mode==2)                                 // ¿Intevalo de Estimulación?
  {
    pulsos++;                                 // Contar pulsos realizados
    
    if(pulsos==frequency)                     // ¿# de pulsos coinciden con la frecuencia configurada?
    {
      pulsos=0;                               // Reiniciar contador de pulsos
      if(Ramp_mode == true) amplitude--;      // Disminuir amplitud para entrar en rampa de bajada
      flag=1;                                 // Actualizar amplitud del poteciometro
    } 
     
  }

  TCC0->INTFLAG.bit.MC0 = 1;                  // Clear interrupt flag
  
}


// ............................. TIMER PARA GENERAR SEÑAL PWM DE ESTIMULACIÓN ..........................................


void TCC1_Handler()
{

  if(mode==1) amplitude++;              // ¿Rampa de subida? -> Incrementar Amplitud 
  else if(mode==3) amplitude--;         // ¿Rampa de bajada? -> Decrementar Amplitud

  if(amplitude > amplitude_max) amplitude = amplitude_max;  
  if(amplitude < amplitude_min) amplitude = amplitude_min;
  
  flag = 1;
  TCC1->INTFLAG.bit.MC0 = 1;            
  
}


// .....................................................................................................................


void setResistance(int percent)
{
   
  pot_command = 0x0011;                                             // Write Command to Potentiometer
  pot_value = (pot_command << 8) | percent;                         // Add Potentiometer Value to Write Command

  PORT->Group[PORTA].OUTCLR.reg = PORT_PA03;                        // Set Potentiometer CS Pin to Low
  delayMicroseconds(50);                                            // Delay
  
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));  // Begin SPI Transaction  
  SPI.transfer16(pot_value);                                        // Write New Value to Potentiometer
  SPI.endTransaction();                                             // End SPI Transaction
  
  delayMicroseconds(50);                                            // Delay
  PORT->Group[PORTA].OUTSET.reg = PORT_PA03;                        // Set Potentiometer CS Pin to High

}


// .....................................................................................................................


void TC5_Handler (void) 
{

  update_mode = true;
  TC5->COUNT16.INTFLAG.bit.MC0 = 1;                     // Clear interrupt flag  
  
}


// .....................................................................................................................


void next_mode(void)
{
  
  if(Ramp_mode)                                         // Ramp Mode On?
  {                                                     // Ramp Mode On (Freq. >= 10):
    mode++;                                             // Use Sequence: Ramp Up -> On Time -> Ramp Down -> Off Time
    if(mode>=5) mode=1;                                 // Reset Stimultation Sequence    
  }
  else                                                  
  {                                                     // Ramp Mode Off (Freq. < 10):
    if(mode==2) mode = 4;                               // Use Sequence: On Time -> Off Time
    else mode = 2;                                      // Reset Stimultation Sequence 
  } 

  
  if(mode == 1)                                         // Ramp Up Interval Active?
  {    
    Timer_Disable();                                    // Stop Timer to change to Ramp Up Time (2s)
    TC5->COUNT16.CC[0].reg = (uint16_t) 62;             // Set TC5 value with Ramp Up Time (2s)   
    PWM_Start();                                        // Start PWM  (Stimulation Signal)
    Timer_Start();                                      // Enable Timer again    
    Timer2_Start();                                     // Update Up Ramp Amplitude Interval     
  }
  else if(mode == 2)                                    // On Time Interval Active?
  {
    Timer2_Stop();                                      // Stop Update of Ramp Amplitude 
    pulsos=0;                                           // Reset Stimulation Pulses Count
    Timer_Disable();                                    // Stop Timer to change to ON time value       
    TC5->COUNT16.CC[0].reg = (uint16_t) ON_Time;        // Set TC5 value with ON Time (Stimulation)    
    if(Ramp_mode == false) PWM_Start();                 // Enable PWM 
    Timer_Start();                                      // Enable Timer again    
  } 
  else if(mode == 3)                                    // Down Up Interval Active?
  {    
    Timer_Disable();                                    // Stop Timer to change to Ramp Down Time
    TC5->COUNT16.CC[0].reg = (uint16_t) 62;             // Set TC5 value with Ramp Down Time    
    Timer_Start();                                      // Enable Timer again     
    Timer2_Start();                                     // Update Down Ramp Amplitude                  
  }
  else if(mode == 4)                                    // Off Time Interval Active?
  {
    Timer2_Stop();                                      // Stop Update of Ramp Amplitude   
    PWM_Stop();                                         // Disable PWM     
    Timer_Disable();                                    // Stop Timer to change to OFF Time Value
    TC5->COUNT16.CC[0].reg = (uint16_t) OFF_Time;       // Set TC5 value with OFF Time (Repose)    
    Timer_Start();                                      // Enable Timer again        
    //go_to_sleep = true;                                 // Enter in Standy Mode to Save Energy
  }
    
}


void PWM_Config()
{

  PM->APBCMASK.reg |= PM_APBCMASK_TCC0;
  //GCLK_GENCTRL_RUNSTDBY |             // Set GCLK4 to run in standby mode 


  // Enable and configure the Generic CLK Generator (GCLK)
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |                  // Improve duty cycle 
                      GCLK_GENCTRL_GENEN |                // Enable GCLK
                      GCLK_GENCTRL_SRC_OSC8M |            // OSC32K | OSC8M | DFLL48M |   // Set the 8MHz as Clock Source
                      GCLK_GENCTRL_ID(4);                 // Select GCLK4 as ID 
  while (GCLK->STATUS.bit.SYNCBUSY);                      // Wait for synchronization


  // Select clock divider to GCLK4
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |                 // Divide 8MHz by 1 
                     GCLK_GENDIV_ID(4);                   // Apply it to GCLK4    
  while (GCLK->STATUS.bit.SYNCBUSY);                      // Wait for synchronization

  
  // Enable GCLK4 and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |                // Enable Generic Clock 
                      GCLK_CLKCTRL_GEN_GCLK4 |            // Select GCLK4
                      GCLK_CLKCTRL_ID_TCC0_TCC1;          // Feed CLK4 to TCC0 and TCC1
  while (GCLK->STATUS.bit.SYNCBUSY);                      // Wait for synchronization


  TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV256;          // Divide counter by 1 (This is N)
  TCC0->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;               // Set Timer counter Mode to 16 bits
  TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;                 // Select NPWM (Single-slope): count up to PER, match on CC[n]
  while (TCC0->SYNCBUSY.bit.WAVE);                        // Wait for synchronization

  TCC0->PER.reg = period;                                 // Set the period (TOP) for rate or frequency of PWM signal
  while (TCC0->SYNCBUSY.bit.PER);                         // Wait for synchronization

  // Set duty cycle where n for CC[n] is n = x % 4 and x is form WO[x]
  TCC0->CC[0].reg = pulse_width;                          // Set PWM signal to 50% of duty cicle 
  while (TCC0->SYNCBUSY.bit.CC0);                         // Wait for synchronization

  // Configure PA08 (D0 in Trinket) to be output
  PORT->Group[PORTA].DIRSET.reg = PORT_PA08;              // Set pin as output
  PORT->Group[PORTA].OUTCLR.reg = PORT_PA08;              // Set pin to low
  PORT->Group[PORTA].PINCFG[8].reg |= PORT_PINCFG_PMUXEN; // Enable the port multiplexer for PA08

  // Connet TCC1 timer to PA08. Function F is TCC1/WO[2] for PA08. Function E is TCC0/WO[0]
  // Odd pin num (2*n+1): use PMUXO
  // Even pin num (2*n): use PMUXE
  PORT->Group[PORTA].PMUX[4].reg = PORT_PMUX_PMUXE_E;
  
}


// .....................................................................................................................


void Enable_PWM_Interrupts(void)
{
  // Configure interrupt request
  NVIC_DisableIRQ(TCC0_IRQn);                           // Deshabilitar interrupciones
  NVIC_ClearPendingIRQ(TCC0_IRQn);                      // Limpiar banderas de interrupción
  NVIC_SetPriority(TCC0_IRQn, 0);                       // Nivel de prioridad de la interrupción
  NVIC_EnableIRQ(TCC0_IRQn);                            // Habilitar interrupciones
  
  TCC0->INTENSET.bit.MC0 = 1;                           // Enable TC5 Overflow Interrupt Request (OVF)
  while(TCC0->STATUS.reg & TC_STATUS_SYNCBUSY); 
}


// .....................................................................................................................


void PWM_Start()
{ 
  TCC0->CTRLA.reg |= (TCC_CTRLA_ENABLE);                // Enable TCC1 (Start PWM)
  while (TCC0->SYNCBUSY.bit.ENABLE);                    // Wait for synchronization     
}


// .....................................................................................................................


void PWM_Stop()
{
  TCC0->CTRLA.reg &= (~TCC_CTRLA_ENABLE);               // Disable TCC1 (Stop PWM)
  while (TCC0->SYNCBUSY.bit.ENABLE);                    // Wait for synchronization
}


// .....................................................................................................................


void Timer_Config()
{

  // Enable and configure the Generic CLK Generator (GCLK)
  GCLK->GENCTRL.reg = GCLK_GENCTRL_RUNSTDBY |           // Set GCLK5 to run in standby mode
                      GCLK_GENCTRL_IDC |                // Improve duty cycle 
                      GCLK_GENCTRL_GENEN |              // Enable GCLK
                      GCLK_GENCTRL_SRC_OSC32K |         // Set the 32KHz as Clock Source
                      GCLK_GENCTRL_ID(5);               // Select GCLK5 as ID 
  while (GCLK->STATUS.bit.SYNCBUSY);                    // Wait for synchronization


  // Select clock divider to GCLK5
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |               // Divide 32KHz by 1
                     GCLK_GENDIV_ID(5);                 // Apply it to GCLK5 
  while (GCLK->STATUS.bit.SYNCBUSY);                    // Wait for synchronization


  // Enable GCLK0 and connect it to TC4 and TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |              // Enable Generic Clock 
                      GCLK_CLKCTRL_GEN_GCLK5 |          // Select GCLK5
                      GCLK_CLKCTRL_ID_TC4_TC5;          // Feed CLK5 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY);                    // Wait for synchronization

 
  Timer_Reset(); //reset TC5

  // Set TC5 Mode, WaveForm, Prescaler and Enable it 
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_RUNSTDBY |           // Set the timer to run in standby mode
                           TC_CTRLA_MODE_COUNT16 |       // Set Timer counter mode to 16 bits
                           TC_CTRLA_WAVEGEN_MFRQ |       // Set TC5 mode as match frequency
                           TC_CTRLA_PRESCALER_DIV1024 |  // Set prescaler to 1024
                           TC_CTRLA_ENABLE;              // Enable TC5

  TC5->COUNT16.CC[0].reg = (uint16_t) OFF_Time;          // Set TC5 value with ON Time (Stimulation)
  while (tcIsSyncing());
  
  NVIC_DisableIRQ(TC5_IRQn);                             // Configure Interrupt Request
  NVIC_ClearPendingIRQ(TC5_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);
  NVIC_EnableIRQ(TC5_IRQn);
  
  TC5->COUNT16.INTENSET.bit.MC0 = 1;                     // Enable TC5 Overflow Interrupt Request (OVF)
  while (tcIsSyncing());                                 // Wait until TC5 is done syncing 
} 


// .....................................................................................................................


bool tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}


// .....................................................................................................................


void Timer_Start()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;            // set the CTRLA register
  while (tcIsSyncing());                                // wait until snyc'd
}


// .....................................................................................................................


void Timer_Reset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}


// .....................................................................................................................


void Timer_Disable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}


// .....................................................................................................................


void DAC_Init(void)
{  
  PORT->Group[PORTA].DIRSET.reg = PORT_PA02;                  // Configure PA02 (D1 in Trinket M0) to be output
  PORT->Group[PORTA].PINCFG[2].reg |= PORT_PINCFG_PMUXEN;     // Enable the port multiplexer for PA02

  // Odd pin num (2*n+1): use PMUXO
  // Even pin num (2*n): use PMUXE
  PORT->Group[PORTA].PMUX[1].reg = PORT_PMUX_PMUXE_B;         // Connet DAC to PA02. Function B is DAC OUTPUT for PA02      
  
  PM->APBCMASK.reg |= PM_APBCMASK_DAC;                        // Enable Peripheral Clock for DAC
  
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID (DAC_GCLK_ID) |         // DAC share same GCLK
                      GCLK_CLKCTRL_CLKEN |                    // Enable Generic Clock
                      GCLK_CLKCTRL_GEN(0);                    // Select Generic Clock #0
  
  DAC->CTRLA.reg = DAC_CTRLA_SWRST;                           // Apply Software Reset
  while (DAC->CTRLA.reg & DAC_CTRLA_SWRST);                   // Wait Until Reset is Complete
 
  DAC->CTRLA.reg &= ~(DAC_CTRLA_ENABLE);                      // Disable DAC to permit change CTRLA B register.
  DAC->CTRLB.reg |= DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL_AVCC;   // Enable the DAC output on Vout pin. Enable VDDANAL REF. 
  DAC->CTRLA.reg |= DAC_CTRLA_ENABLE;                         // Enable DAC
  while (DAC->STATUS.reg & DAC_STATUS_SYNCBUSY);              // Wait Until Clock is Ok
}


// .....................................................................................................................


void Timer2_Config()
{

  TCC1->WAVE.reg = TCC_WAVE_WAVEGEN_MFRQ;                 // Select NPWM (Single-slope): count up to PER, match on CC[n]
  TCC1->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV1024;         // Divide counter by 1 (This is N)
  while (TCC1->SYNCBUSY.bit.WAVE);                        // Wait for synchronization

  TCC1->CC[0].reg = timer_interval;                       // Set PWM signal to 50% of duty cicle 
  while (TCC1->SYNCBUSY.bit.CC0);                         // Wait for synchronization

}


// .....................................................................................................................


void Timer2_Interrupts()
{
  
  NVIC_DisableIRQ(TCC1_IRQn);
  NVIC_ClearPendingIRQ(TCC1_IRQn);
  NVIC_SetPriority(TCC1_IRQn, 1);
  NVIC_EnableIRQ(TCC1_IRQn);

  TCC1->INTENSET.bit.MC0 = 1;                           // Enable TC5 Overflow Interrupt Request (OVF)
  while(TCC1->STATUS.reg & TC_STATUS_SYNCBUSY); 
  
} 


// .....................................................................................................................

void Timer2_Start()
{ 
  TCC1->CTRLA.reg |= (TCC_CTRLA_ENABLE);              // Enable TCC1 (Start PWM)
  while (TCC1->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization     
}


// .....................................................................................................................


void Timer2_Stop()
{
  TCC1->CTRLA.reg &= (~TCC_CTRLA_ENABLE);             // Disable TCC1 (Stop PWM)
  while (TCC1->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
}


// .....................................................................................................................


void Timer2_Reset()
{
  TCC1->CTRLA.reg = TCC_CTRLA_SWRST;
  while (TCC1->CTRLA.bit.SWRST);
}


// .....................................................................................................................


void SAMD21_Sleep()
{
  // Due to a hardware bug on the SAMD21, the SysTick interrupts become active before the flash has powered up from sleep, causing a hard fault
  // To prevent this the SysTick interrupts are disabled before entering sleep mode
  
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;               // Disable SysTick interrupts
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;                        // Put the SAMD21 in deep sleep upon executing the __WFI() function
  //SYSCTRL->VREG.bit.RUNSTDBY = 1;                         // Keep the voltage regulator in normal configuration during run stanby
  SYSCTRL->OSC32K.bit.RUNSTDBY = 1;                         // Set the Internal 32KHz crystal to run standby mode
  //SYSCTRL->OSC8M.bit.RUNSTDBY = 1;                        // Set the Internal 8MHz crystal to run standby mode 
  NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_SLEEPPRM_DISABLED;    // Disable Auto Power Reduction During Sleep - SAMD21 Errata 1.14.2
  __WFI();                                                  // Put the SAMD21 into Deep Sleep, Zzzzzzzz...   
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;                // Enable SysTick interrupts 
  
}
// .....................................................................................................................
void RF_Config(){
  Serial.begin(115200); // Se inicializa la comunicación serial a una tasa de 115200
  radio.begin(); // Se inicializa el módulo RF
  radio.setChannel(110); // Canal 100, 2510 MHz
  radio.openWritingPipe(addresses[0]); // Dirección 0 para escribir
  radio.openReadingPipe(1, addresses[1]); // Dirección 1 para leer
  radio.setDataRate(RF24_250KBPS); // Tasa de datos de 250 kbps
  radio.setPALevel(RF24_PA_MIN); // PA power al mínimo, -18 dBm
}
// .....................................................................................................................
void RF_Begin() //Funcion encargada de habilitar la recepcion de datos mediante el encendido y apagado de los pines CSN y CE
{
  delay(5); 
  while(dataControl == 0){ // Mientras todavía se quiere leer 
  radio.startListening(); // Módulo RF como receptor
  if(radio.available()) // Si hay datos en el pipe
  {
    Serial.println("Si hay datos");
    radio.read(&dataBus, sizeof(dataBus)); // Leer y almacenar en dataBus
    Serial.println(dataBus[1]);
    Serial.println(dataBus[2]);
    Serial.println(dataBus[3]);
    Serial.println(dataBus[4]);
    Serial.println(dataBus[5]);
    if(dataBus[5] == 1){ // Si el bit 5 de dataBus es 1 (recepción confirmada) 
      dataControl = 1; // Ya no se quiere leer 
      contSec++;
    }
  }
  delay(5); 
  }
  radio.stopListening(); // Módulo RF como transmisor
  dataConfOut[0] = 1; // Se prepara la confirmación 
  radio.write(dataConfOut, sizeof(dataConfOut)); // Se envía la confirmación 
  // Se configuran los parámetros de estimulación dependiendo de lo recibido
  int modo = dataBus[0]; 
  int ancho = dataBus[1]; 
  int tiempo = dataBus[2]; 
  int freq = dataBus[3];
  int sleep = dataBus[4]; 
  // Se muestran en el monitor serial las opciones seleccionadas de los parámetros dependiendo de lo que venia en el array de la varilla programadora
  Serial.print("Modo de Operación: ");
  if(modo == 1)
  {
    Serial.println("Estimulación"); 
  }
  else if(modo == 2)
  {
    Serial.println("Programación");
  }
  else if(modo == 3)
  {
    Serial.println("Reposo");
  }

  Serial.print("Ancho de Pulso: "); 
  if(ancho == 1)
  {
    Serial.println("250 us");
  }
  else if(ancho == 2)
  {
    Serial.println("500 us");
  }
  else if(ancho == 3)
  {
    Serial.println("25%");
  }
  else if(ancho == 4)
  {
    Serial.println("50%");
  }
  else if(ancho == 5)
  {
    Serial.println("75%");
  }

  Serial.print("Tiempo de Estimulación: ");
  if(tiempo == 1)
  {
    Serial.println("30 s");
  }
  else if(tiempo == 2)
  {
    Serial.println("60 s");
  }
  else if(tiempo == 3)
  {
    Serial.println("Tiempo 3");
  }
  else if(tiempo == 4)
  {
    Serial.println("Tiempo 4");
  }
  else if(tiempo == 5)
  {
    Serial.println("Tiempo 5");
  }

  Serial.print("Frequencia: ");
  if(freq == 1)
  {
    Serial.println("490.20 Hz (Default)");
  }
  else if(freq == 2)
  {
    Serial.println("30.64");
  }
  else if(freq == 3)
  {
    Serial.println("122.50 Hz");
  }
  else if(freq == 4)
  {
    Serial.println("245.10 Hz");
  }
  else if(freq == 5)
  {
    Serial.println("980.39 Hz");
  }
  else if(freq == 6)
  {
    Serial.println("3921.16 Hz");
  }
  else if(freq == 7)
  {
    Serial.println("31372.55 Hz");
  }
  else if(freq == 8)
  {
    Serial.println("Frecuencia 8");
  }
  Serial.print("Tiempo de apagado: ");
  if(sleep == 1)
  {
    Serial.println("5 min");
  }
  else if(sleep == 2)
  {
    Serial.println("NO");
  }
  Serial.println("");
  dataConfOut[0] = 0; // Se reinicia la confirmación 
  dataControl = 0; // Se pone en lectura otra vez el módulo
}

// .....................................................................................................................
  
