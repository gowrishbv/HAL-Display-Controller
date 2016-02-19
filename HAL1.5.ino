#include "LedControl.h"




//Libraries
//#include <SPI.h> // SPI Library




// Global Variables
//to be varied as per required
int alarm_lim = 20;// Change the counternumber to increase the snooze time (count=(no_of_snooze_seconds)/3)


int pins[4] = {2, 3, 4, 5}; //Input pins from WSN module
int mux[3] = {6, 7, 8}; //disp line select
int spi_sel = A0; //spi select data
int sw = 9; //switch line
int i, j, k, temp1, temp2; //Temporary Data
String WSN_data, WSN_Full;
int clocks, String_len;
int sign[4], decimalp[4], postdec[4];//Temperature data variables
int wsn_count = 0;
int No_of_sensors;
int alarm_count = 0;
int alarm_state = 0;
int chann = 0;

int relay = 10;





LedControl lc = LedControl(11, 13, A0, 1);
//Sub Routines
void initialize();
void printerr();
void printNumber(int v);
/*
   Author: Gowrish
   Purpose: Setup Loop
   Language:  C
*/
void setup() {


  //Initialise all Elements
  initialize();


}
//End of Setup




/*
   Author: Gowrish
   Purpose: Main Loop for HAL Display System Firmware
   Language:  C
*/
void loop() {
  int alarm_read = 0;
  // Alarm Section

  Serial.println(alarm_count);
  if ( ((alarm_state == 1) && (alarm_count == 0)) | (alarm_count >= alarm_lim))
  {
    alarm_count = alarm_lim + 1;
    digitalWrite(relay, HIGH);
  }
  else
  {}


  if (alarm_state == 1)
  {
    alarm_count++;

    //Serial.print(alarm_count);
  }


  else
  {
    alarm_count = 0;
  }


  // Read Acknowledgement

  while (digitalRead(5) == 0)
  {

    alarm_read++;
    delay(100);
    if  (alarm_read > 25)// Number for Waiting 
    {
      Serial.println("Alarm ACk");
      alarm_count = 1;
      digitalWrite(relay, LOW);
      break;
    }
    else
    {
      Serial.println(alarm_read);
    }
  }


// if for About 8 Seconds, WSN Does not transfer any temperature data to Control Card Display Error
  if (wsn_count > 3)
  {
    printerr();
  }
  else
  {

    printNumber(chann);
    chann++;
    delay(3000);
    wsn_count++;

    //Reset Channel Count
    if (chann >= No_of_sensors)
      chann = 0;
    else
    {}
  }






}// End of Loop





/*
   Author: Gowrish
   Purpose: Initializing all IOS for
   Language:  C
*/
void initialize()
{
  //Initialize all the IO for the communication with WSN node
  for (i = 0; i < 4; i++)
  {
    pinMode(pins[i], INPUT);
  }

  //Initialize SPI
  //  SPI.begin();

  //Initialize Switch
  pinMode(sw, INPUT);

  //Initialize Serial Communication
  Serial.begin(9600);

  //Initialize MUX
  for (i = 0; i < 3; i++)
  {
    pinMode(mux[i], INPUT);
  }

  // Initialize Relay
  pinMode(relay, OUTPUT);

  // pinMode(A0, OUTPUT);



  lc.shutdown(0, true);
  delay(100);
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 5);
  /* and clear the display */
  lc.clearDisplay(0);

  //Initialize SPI select Line
  pinMode(spi_sel, OUTPUT);

  //Attach Interrupt
  attachInterrupt(digitalPinToInterrupt(2), WSN_Clock , RISING);
  attachInterrupt(digitalPinToInterrupt(3), WSN_Data , RISING);

}//End of Initialize





/*
   Author: Gowrish
   Purpose: Interrupt Routiene for Clock from WSN Module
   Language:  C
*/
void WSN_Clock()
{
  if (digitalRead(pins[1]) == 0)
  {
    //state = ~state;
    clocks++;
    //char just = char (digitalRead(3));
    WSN_data += digitalRead(pins[2]);
  }

  else
  {}

}//end of WSN Interrupt Clock Data



/*
   Author: Gowrish
   Purpose: Interrupt Routiene for Data from WSN Modulec Enable pin High Rise
   Language:  C
*/
void WSN_Data()
{
  Serial.print(clocks);
  Serial.print(':');
  //Serial.println(WSN_data);
  WSN_Full = WSN_data;
  WSN_data = "";

  if ((clocks == 15) || (clocks == 29) || (clocks == 43) || (clocks == 57)) //Check if Data is valid
  {
    WSN_Decode(WSN_Full);
  }
  else
  {}
  //WSN_data = "";
  clocks = 0;
  wsn_count = 0;

}//end of WSN Interrupt for Data




/*
   Author: Gowrish
   Purpose: Interrupt Routiene for Data from WSN Modulec Enable pin High Rise
   Language:  C
*/
void WSN_Decode(String data)
{

  Serial.print(':');
  String_len = data.length();
  Serial.print(String_len);
  No_of_sensors = (String_len - 1) / 14;
  Serial.print(':');
  Serial.println(No_of_sensors);

  if (data.charAt(String_len - 1) == '0')
    alarm_state = 1;
  else
    alarm_state = 0;




  //Extract Data from WSN Data
  for (i = 0; i < No_of_sensors; i++)
  {
    String Extracted = data.substring(i * 14, i * 14 + 14);

    WSN_split(Extracted, i);
    Serial.print(':');
    Serial.println(Extracted);
  }

}//End of WSN Data Decode




/*
   Author: Gowrish
   Purpose: Split of WSN 14 BIT data
   Language:  C
*/
void WSN_split(String data, int channel)
{




  //Get the Temperature data
  decimalp[channel] = get_temperature(data);
  Serial.print(decimalp[channel]);
  Serial.print('.');

  //Detect Sign
  if (data.charAt(0) == '1')
    sign[channel] = 0;
  else
    sign[channel] = 1;
  //decimalp[channel] = decimalp[channel] * -1;

  postdec[channel] = get_temperature_postdec(data);
  Serial.print(postdec[channel]);


}//End of WSN data Split



/*
   Author: Gowrish
   Purpose: Get Temperature data pre Decimal Point
   Language: C
*/
int get_temperature(String data1)
{
  String data2 = data1.substring(1, 8);
  temp1 = 0;

  for (k = 0; k < data2.length(); k++)
  {
    if (data2.charAt(k) == '1')
    {
      //  temp1 += int ( pow (2, (data2.length() - 1) - k));
      temp1 +=  power ( (data2.length() - 1) - k);
    }
    else
    {}
  }
  return (temp1);
}//endof Data Extraction of Pre decimal Point






/*
   Author: Gowrish
   Purpose: Get Temperature data post Decimal Point
   Language:  C
*/
int get_temperature_postdec(String data1)
{
  String data2 = data1.substring(8, 12);
  temp2 = 0;

  for (k = 0; k < data2.length(); k++)
  {
    if (data2.charAt(k) == '1')
    {
      temp2 += power ( (data2.length() - 1) - k);
    }
    else
    {}
  }
  return (temp2);
}//endof Data Extraction of Post decimal Point



/*
   Author: Gowrish
   Purpose:Calculate the power of 2
   Language:  C
*/
int power(int dataaa)
{
  int tempo_pow = 1;
  int seq;
  for (seq = 0; seq < dataaa; seq++)
  {
    tempo_pow = tempo_pow * 2;
  }
  return (tempo_pow);
}

/*
   Author: Gowrish
   Purpose:Printing the Number to the 7 Segment Display
   Language:  C
*/
void printNumber(int ch) {
  int ones;
  int tens;
  int hundreds;
  boolean negative;



  //sign[4], decimalp[4], postdec[4];
  int v = decimalp[ch];

  lc.setChar(0, 0, 'P', false);// First Character 'P'
  lc.setDigit(0, 1, ch, false);// Channel Character
  lc.setChar(0, 2, ' ', false);


  if (sign[ch] == 0)
  {
    lc.setChar(0, 3, '-', false);
  }

  if (sign[ch] == 1) {
    lc.setChar(0, 3, ' ', false);

  }
  ones = v % 10;
  v = v / 10;
  tens = v % 10;
  v = v / 10;
  hundreds = v;


  lc.setDigit(0, 4, (byte)tens, false);
  lc.setDigit(0, 5, (byte)ones, true);
  lc.setDigit(0, 6, postdec[ch], false);
  lc.setChar(0, 7, 'c', false);
}




/*
   Author: Gowrish
   Purpose:Print Error in the 7 Segment Display wheh WSN Node is not sending data
   Language:  C
*/
void printerr()
{
  lc.clearDisplay(0);
  lc.setChar(0, 0, 'e', false);
  lc.setRow(0, 1, 5);
  lc.setRow(0, 2, 5);
  lc.setChar(0, 3, 'o', false);
  lc.setRow(0, 4, 5);
  delay(3000);
}
