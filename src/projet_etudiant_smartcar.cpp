////////////////////////////////////////////////////////
//              Inclusion des librairies              //
////////////////////////////////////////////////////////
#include <Wire.h> // Librairie pour l'utilisation de l'I2C
#define SoftwareSerial_h

///////////////////////////////////////////////////////////
//  Déclarations des ALIAS des broches analogiques (pin) //
///////////////////////////////////////////////////////////
#define CAP_LUM_PIN A0 // le terme CAP_LUM_PIN sera remplacé par le compilateur* par le terme A0 (*par le préprocesseur)
#define SENS_AR_PIN A1
#define DISTANCE_AV_PIN A2
#define DISTANCE_AR_PIN A3
#define NIV_BATT_PIN A4

///////////////////////////////////////////////
//  Déclarations des ALIAS des broches (pin) //
///////////////////////////////////////////////
const uint32_t PHARES_AV_PIN = D11;
const uint32_t PHARES_AR_PIN = D2;

const uint32_t CMD_GENE_AUDIO_PIN = D9;
const uint32_t TONE_PIN = D10;
const uint32_t PRES_PIN = D7;
const uint32_t LOIN_PIN = D8;

const uint32_t PWM_AVANT_PIN = D4;
const uint32_t PWM_ARRIERE_PIN = D5;
const uint32_t PWM_DROITE_PIN = D3;
const uint32_t PWM_GAUCHE_PIN = D6;

// const uint16_t RX_PIN = D12; // mettre à jour les #define des codes générés par RemoteXY
// const uint16_t TX_PIN = D13; // mettre à jour les #define des codes générés par RemoteXY

const uint32_t SDA_PIN = D14;
const uint32_t SCL_PIN = D15;
///////////////////////////////////////////////
//           Déclarations des cannaux        //
///////////////////////////////////////////////

const uint32_t CANAL_DROITE = 2;
const uint32_t CANAL_GAUCHE = 3;
const uint32_t CANAL_AVANT = 2;
const uint32_t CANAL_ARRIERE = 1;
const uint32_t CANAL_PHARE = 3;

//////////////////////////////////////////////
//         RemoteXY include library         //
//////////////////////////////////////////////
// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 12
#define REMOTEXY_SERIAL_TX 13
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = // 69 bytes V19
    {255, 3, 0, 8, 0, 62, 0, 19, 0, 0, 0, 0, 209, 1, 200, 84, 1, 1, 4, 0,
     5, 130, 9, 61, 61, 32, 23, 26, 31, 73, 7, 4, 49, 15, 5, 128, 19, 23, 26, 0,
     0, 0, 0, 0, 0, 200, 66, 0, 0, 0, 0, 67, 16, 67, 40, 10, 78, 23, 26, 2,
     1, 23, 38, 24, 24, 0, 23, 31, 0};

// this structure defines all the variables and events of your control interface
struct
{

  // input variables
  int8_t joystick_x; // from -100 to 100
  int8_t joystick_y; // from -100 to 100
  uint8_t Klaxon;    // =1 if button pressed, else =0, from 0 to 1

  // output variables
  float Bargraphe; // from 0 to 100
  float value_01;

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)
// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 12
#define REMOTEXY_SERIAL_TX 13
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

/////////////////////////////////////////////
//          Déclaration des timers         //
/////////////////////////////////////////////

// HardwareTimer timerDirection(TIM2);
HardwareTimer timerPropulsion(TIM3);
// HardwareTimer timerPharesAR(TIM1);

////////////////////////////////////////////////////////////////////////////////////////////
//                                 DEFINITIONS DE FONCTIONS                               //
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////
//  Gestion de la direction   //
////////////////////////////////

// void pwm_direction()
// {
//   if (RemoteXY.joystick_x > 10)
//   {
//     timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
//     timerDirection.setCaptureCompare(CANAL_DROITE, RemoteXY.joystick_x);
//   }
//   else if (RemoteXY.joystick_x < -10)
//   {
//     timerDirection.setCaptureCompare(CANAL_GAUCHE, -RemoteXY.joystick_x);
//     timerDirection.setCaptureCompare(CANAL_DROITE, 0);
//   }
//   else
//   {
//     timerDirection.setCaptureCompare(CANAL_DROITE, 0);
//     timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
//   }
// }

/////////////////////////////////
//  Gestion de la propulsion   //
/////////////////////////////////

bool pwm_propulsion(bool p, bool l, bool s_ar)
{
  bool freiner = 0;

  if (RemoteXY.connect_flag == 1)
  {
    if (RemoteXY.joystick_y > 10)
    {
      timerPropulsion.setCaptureCompare(CANAL_AVANT, RemoteXY.joystick_y);
      timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
    }

    else if (RemoteXY.joystick_y < -10)
    {
      if (l == 1)
      {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE, (RemoteXY.joystick_y * (-1)));
      }
      else if (p == 0)
      {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE, (RemoteXY.joystick_y * (-1)) / 2);
      }
      else
      {
        if (s_ar == false)
        { 
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 100);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
          freiner = true;
        }
        else
        {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE,0);
        }

      }
    }
    else //si le joystick est au milieu
      { 
        if(p==1)
        {
          if (s_ar == false)
          { 
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 100);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
          freiner = true;
          }
          else
          {
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE,0);
          }
        }
        else{
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE,0);
        }


      }
  }
return freiner;
}

////////////////////////////////////////////////////////////////////////////////////////////
//  Gestion du bargraph du téléphone de niveau batterie ou de proximité du capteur avant  //
////////////////////////////////////////////////////////////////////////////////////////////

void gestion_bargraphe(uint32_t nivB)
{
  uint32_t a = 3722;
  uint32_t b = 3102;
  if (nivB < b)
  {
    RemoteXY.Bargraphe = 0;
  }
  else if (nivB > a)
  {
    RemoteXY.Bargraphe = 100;
  }
  else
  {
    RemoteXY.Bargraphe = 0.161 * nivB - 499;
  }
  RemoteXY.value_01 = nivB;
}

////////////////////////////
//    Gestion phares      //
////////////////////////////

// void gestion_phares(bool freinage, uint32_t c_lum)
// {
//   if (freinage = 1)
//   {
//     digitalWrite(PHARES_AR_PIN, 100);
//   }
//   else
//   {
//     if (c_lum > 1861)
//     {
//       digitalWrite(PHARES_AV_PIN, 100);
//       digitalWrite(PHARES_AR_PIN, 30);
//     }
//     else if (c_lum <= 1861)
//       digitalWrite(PHARES_AV_PIN, 0);
//       digitalWrite(PHARES_AR_PIN, 0);
//   }
// }

//////////////////////////////
//    Gestion du buzzer     //
//////////////////////////////

void gestion_buzzer(bool p, bool l)
{
  static uint32_t tempsPrec = millis();
  static bool bip = LOW;
  static uint32_t demi_periode = 250;
  uint32_t tempsActu = millis();

  if (RemoteXY.Klaxon == 1)
  {
    bip = HIGH;
  }
 
  else
  {
    if (p == true)
    {
      demi_periode = 125;
    }
    else
    {
      demi_periode = 250;
    }

    if (tempsActu - tempsPrec >= demi_periode)
    {
      tempsPrec = tempsActu;
      if (l == false)
      {
        bip = !bip;
      }
      else
      {
        bip = LOW;
      }
    }
  }
  digitalWrite(CMD_GENE_AUDIO_PIN, bip);
}

////////////////////////////////////////////
//    Gestion approche obstacle avant     //
////////////////////////////////////////////

/////////////////////////////////////////
//    Moyennage du capteur avant       //
/////////////////////////////////////////

/////////////////////////////////////
//    Gestion de la température    //
/////////////////////////////////////

//////////////////////////////////////////////////////
//              Initialisations                     //
//////////////////////////////////////////////////////
void setup()
{
  RemoteXY_Init(); // initialization by macros

  // timerDirection.setPrescaleFactor(44);
  // timerDirection.setOverflow(100);
  // timerDirection.setMode(CANAL_DROITE, TIMER_OUTPUT_COMPARE_PWM1, PWM_DROITE_PIN);
  // timerDirection.setMode(CANAL_GAUCHE, TIMER_OUTPUT_COMPARE_PWM1, PWM_GAUCHE_PIN);
  // timerDirection.setCaptureCompare(CANAL_DROITE, 0);
  // timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
  // timerDirection.resume();

  pinMode(PHARES_AR_PIN, OUTPUT);      // PHARES_AR_PIN est configurée en sortie
  pinMode(CMD_GENE_AUDIO_PIN, OUTPUT); // CMD_GENE_AUDIO_PIN est configurée en sortie
  pinMode(PRES_PIN, INPUT);            // PRES_PIN est configurée en entrée
  pinMode(LOIN_PIN, INPUT);            // LOIN_PIN est configurée en entrée
  pinMode(SENS_AR_PIN, INPUT);         // SENS_AR_PIN est configurée en entrée (lue par digitalRead)

  /////////////////////////////////////////////////////////////////////
  // Initialisation du timer propulsion (PWM avant / arrière)        //
  /////////////////////////////////////////////////////////////////////
  timerPropulsion.setPrescaleFactor(44);
  timerPropulsion.setOverflow(100);
  timerPropulsion.setMode(CANAL_AVANT,   TIMER_OUTPUT_COMPARE_PWM1, PWM_AVANT_PIN);
  timerPropulsion.setMode(CANAL_ARRIERE, TIMER_OUTPUT_COMPARE_PWM1, PWM_ARRIERE_PIN);
  timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
  timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
  timerPropulsion.resume();

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication liaison série                      //
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication avec l'interface graphique         //
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication I2C avec capteur de temperataure   //
  /////////////////////////////////////////////////////////////////////
  Wire.setSCL(SCL_PIN);
  Wire.setSDA(SDA_PIN);
  Wire.begin();
}

//////////////////////////////////////////////////////
//                 Boucle infinie                   //
//////////////////////////////////////////////////////
void loop()
{

  uint32_t nivB;

  /////////////////////////////////////////////////
  //  Communication avec l'interface graphique   //
  /////////////////////////////////////////////////
  RemoteXYEngine.handler();

  // déclaration des variables
  bool pres;    // Variable indiquant la position pres du véhicule avec un obstacle arriere
  bool loin;
  bool sens_ar;
  uint32_t c_lum; // Variable indiquant l'intensité lumineuse (jour ou nuit)
  // int capt_av;    // Variable contenant la moyenne de valeur de la conversion du capteur avant
  // int niv_batt;   // Variable contenant une information sur la tension batterie (8.7V batterie pleine -> 3.3V sur convertisseur; 6,5V batterie vide -> 2.5V sur convertisseur)

  analogReadResolution(12);
  gestion_bargraphe(analogRead(NIV_BATT_PIN));

  ///////////////////////////////////////
  //  Informations de l'environnement  //
  ///////////////////////////////////////

  pres = digitalRead(PRES_PIN);       // Présence d'un obstacle à moins de 40cm (PRES = '1')
  loin = digitalRead(LOIN_PIN);       // Présence d'un obstacle à moins de 40cm (LOIN = '10')
  c_lum = analogRead(CAP_LUM_PIN);
  sens_ar = digitalRead(SENS_AR_PIN);

  gestion_buzzer(pres, loin);

  pwm_propulsion(pres, loin, sens_ar);

  ///////////////////////////////
  //  Pilotage de la voiture   //
  ///////////////////////////////
  // pwm_direction();

}
