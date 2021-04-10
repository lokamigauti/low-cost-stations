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

#include <ThreeWire.h> //INCLUSÃO DA BIBLIOTECA
#include <RtcDS1302.h> //INCLUSÃO DA BIBLIOTECA
 
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

    if(Rtc.GetIsWriteProtected()){ //SE O RTC ESTIVER PROTEGIDO CONTRA GRAVAÇÃO, FAZ
        Rtc.SetIsWriteProtected(false); //HABILITA GRAVAÇÃO NO RTC
    }
 
    if(!Rtc.GetIsRunning()){ //SE RTC NÃO ESTIVER SENDO EXECUTADO, FAZ
        Rtc.SetIsRunning(true); //INICIALIZA O RTC
    }
 
    RtcDateTime now = Rtc.GetDateTime(); //VARIÁVEL RECEBE INFORMAÇÕES
    if (now < compiled) { //SE A INFORMAÇÃO REGISTRADA FOR MENOR QUE A INFORMAÇÃO COMPILADA, FAZ
        Rtc.SetDateTime(compiled); //INFORMAÇÕES COMPILADAS SUBSTITUEM AS INFORMAÇÕES ANTERIORES
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
  RtcDateTime now = Rtc.GetDateTime(); //VARIÁVEL RECEBE INFORMAÇÕES   
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
    char datestring[20]; //VARIÁVEL ARMAZENA AS INFORMAÇÕES DE DATA E HORA
 
    snprintf_P(datestring,
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"), //FORMATO DE EXIBIÇÃO DAS INFORMAÇÕES
            dt.Day(), //DIA
            dt.Month(), //MÊS
            dt.Year(), //ANO
            dt.Hour(), //HORA
            dt.Minute(), //MINUTOS
            dt.Second() ); //SEGUNDOS
    return(datestring); //IMPRIME NO MONITOR SERIAL AS INFORMAÇÕES
}
                                                                                                         
                                                                                                         //|    
//=======================================================================================================//|    
//                                                                                                       //|    
//                                   Printout using BUFFER objects BEGIN                                 //|    
//                                                                                                       //|    
//=======================================================================================================//|    
