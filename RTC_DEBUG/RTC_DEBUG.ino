//=======================================================================================================//|
//                                                                                                       //|
//                                        Program Description Begin                                      //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  This example executes a print out example that accesses individual data elements.                    //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                        Program Description End                                        //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                 This is the Revision Section Begin                                    //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//    Date                  Programmer                  Revisions Made                                   //|
//                                                                                                       //|
//    XX June 2012          Krodal                      Original                                         //|
//    XX March 2013         Krodal                      Added bcd2bin, bin2bcd_h, and bin2bcd_l functions//|
//    07 January 2014       Joseph Datillo              Converted file into library format.              //|
//                          T. Daniel Murphy            Added functions to allow for user implentation   //|
//                                                        as a library, vice a stand alone program.      //|
//                                                        This also adds the functions required to access//|
//                                                        individual data elements, and updating time.   //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                        Revision Section End                                           //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                        Acknowledgements Begin                                         //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  The original program was provided by Arduino.cc user Kodal on URL:                                   //|
//  http://playground.arduino.cc/Main/DS1302                                                             //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                          Acknowledgements End                                         //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                            Reference Links Begin                                      //|
//                                                                                                       //|
//=======================================================================================================//|
                                                                                                         //|
//  Datasheet:  http://www.hobbytronics.co.uk/datasheets/DS1302.pdf                                      //|
//  GET UPDATES: https://www.virtuabotix.com/resources.php?product=versalino                             //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                            Reference Links End                                        //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                          Include files required for Program Function Begin                            //|
//                                                                                                       //|
//=======================================================================================================//|
// SD init
// pin MOSI 11
// pin MISO 12
// pin CLK 13 SCK
// pin CS 10
#include <SPI.h>
#include <SD.h>
File myFile;
const int chipSelect = 10;                                                                                                         //|                                                                            //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                          Include files required for Program Function End                              //|
//                                                                                                       //|
//=======================================================================================================//|

// Creation of the Real Time Clock Object

#include <ThreeWire.h> //INCLUS??O DA BIBLIOTECA
#include <RtcDS1302.h> //INCLUS??O DA BIBLIOTECA
 
ThreeWire myWire(6, 7, 8); //OBJETO DO TIPO ThreeWire
RtcDS1302<ThreeWire> Rtc(myWire); //OBJETO DO TIPO RtcDS1302

void setup()  {      
    do {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  } while(!SD.begin());

// Set the current date, and time in the following format:
// seconds, minutes, hours, day of the week, day of the month, month, year
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled); 

    if(Rtc.GetIsWriteProtected()){ //SE O RTC ESTIVER PROTEGIDO CONTRA GRAVA????O, FAZ
        Rtc.SetIsWriteProtected(false); //HABILITA GRAVA????O NO RTC
    }
 
    if(!Rtc.GetIsRunning()){ //SE RTC N??O ESTIVER SENDO EXECUTADO, FAZ
        Rtc.SetIsRunning(true); //INICIALIZA O RTC
    }
 
    RtcDateTime now = Rtc.GetDateTime(); //VARI??VEL RECEBE INFORMA????ES
    if (now < compiled) { //SE A INFORMA????O REGISTRADA FOR MENOR QUE A INFORMA????O COMPILADA, FAZ
        Rtc.SetDateTime(compiled); //INFORMA????ES COMPILADAS SUBSTITUEM AS INFORMA????ES ANTERIORES
    }
}

//=======================================================================================================//|    
//                                                                                                       //|   
//                         Printout by accessing Single Element objects BEGIN                            //|   
//                                                                                                       //|   
//=======================================================================================================//|   
//                                                                                                       //|   
//  This example utilizes the Serial.print function to access individual data elements, this allows for  //|
//  user defined output format.                                                                          //|
//                                                                                                       //|   
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//|  
void loop()  {                                                                                           //| 
// This allows for the update of variables for time or accessing the individual elements.                //|
  RtcDateTime now = Rtc.GetDateTime(); //VARI??VEL RECEBE INFORMA????ES   
  myFile = SD.open("data.csv", FILE_WRITE);                                                              //| 
  if(myFile){                                                                                                       //| 
// Start printing elements as individuals                                                                //|                                                                                   //| 
  myFile.println(printDateTime(now));                                                                         //| 
  }                                                                                                         //| 
// Delay so the program doesn't print non-stop                                                           //| 
  delay( 5000);                                                                                          //| 
}                                                                                                        //| 
 
#define countof(a) (sizeof(a) / sizeof(a[0]))
 
void printDateTime(const RtcDateTime& dt){
    char datestring[20]; //VARI??VEL ARMAZENA AS INFORMA????ES DE DATA E HORA
 
    snprintf_P(datestring,
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"), //FORMATO DE EXIBI????O DAS INFORMA????ES
            dt.Day(), //DIA
            dt.Month(), //M??S
            dt.Year(), //ANO
            dt.Hour(), //HORA
            dt.Minute(), //MINUTOS
            dt.Second() ); //SEGUNDOS
    return(datestring); //IMPRIME NO MONITOR SERIAL AS INFORMA????ES
}
                                                                                                         
                                                                                                         //|    
//=======================================================================================================//|    
//                                                                                                       //|    
//                                   Printout using BUFFER objects BEGIN                                 //|    
//                                                                                                       //|    
//=======================================================================================================//|    
