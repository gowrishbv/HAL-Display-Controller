


//Libraries
#include <SPI.h> // SPI Library




// Global Variables
int pins[4] = {2, 3, 4, 5}; //Input pins from WSN module
int mux[3] = {6, 7, 8}; //disp line select
int spi_sel = A0; //spi select data
int sw = 9; //switch line
int i, j, k, temp1, temp2; //Temporary Data
String WSN_data, WSN_Full;
int clocks, String_len;
int sign[4], decimalp[4], postdec[4];//Temperature data variables

//Sub Routines
void initialize();
/*
 * Author: Gowrish
 * Purpose: Setup Loop
 * Language:  C
 */
void setup() {


  //Initialise all Elements
  initialize();


}
//End of Setup




/*
 * Author: Gowrish
 * Purpose: Main Loop for HAL Display System Firmware
 * Language:  C
 */
void loop() {


}// End of Loop





/*
 * Author: Gowrish
 * Purpose: Initializing all IOS for
 * Language:  C
 */
void initialize()
{
  //Initialize all the IO for the communication with WSN node
  for (i = 0; i < 4; i++)
  {
    pinMode(pins[i], INPUT);
  }

  //Initialize SPI
  SPI.begin();

  //Initialize Switch
  pinMode(sw, INPUT);

  //Initialize Serial Communication
  Serial.begin(9600);

  //Initialize MUX
  for (i = 0; i < 3; i++)
  {
    pinMode(mux[i], INPUT);
  }

  //Initialize SPI select Line
  pinMode(spi_sel, INPUT);

  //Attach Interrupt
  attachInterrupt(digitalPinToInterrupt(2), WSN_Clock , RISING);
  attachInterrupt(digitalPinToInterrupt(3), WSN_Data , RISING);

}//End of Initialize





/*
 * Author: Gowrish
 * Purpose: Interrupt Routiene for Clock from WSN Module
 * Language:  C
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
 * Author: Gowrish
 * Purpose: Interrupt Routiene for Data from WSN Modulec Enable pin High Rise
 * Language:  C
 */
void WSN_Data()
{
  Serial.print(clocks);
  Serial.print(':');
  Serial.print(WSN_data);
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

}//end of WSN Interrupt for Data




/*
 * Author: Gowrish
 * Purpose: Interrupt Routiene for Data from WSN Modulec Enable pin High Rise
 * Language:  C
 */
void WSN_Decode(String data)
{
  int No_of_sensors;
  Serial.print(':');
  String_len = data.length();
  Serial.print(String_len);
  No_of_sensors = (String_len - 1) / 14;
  Serial.print(':');
  Serial.println(No_of_sensors);


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
 * Author: Gowrish
 * Purpose: Split of WSN 14 BIT data
 * Language:  C
 */

void WSN_split(String data, int channel)
{


  //Detect Sign
  if (data.charAt(0) == '0')
    sign[channel] = 0;
  else
    sign[channel] = 1;


  //Get the Temperature data
  decimalp[channel] = get_temperature(data);
  Serial.print(decimalp[channel]);
  Serial.print('.');

  postdec[channel] = get_temperature_postdec(data);
  Serial.print(postdec[channel]);


}//End of WSN data Split



/*
 * Author: Gowrish
 * Purpose: Get Temperature data pre Decimal Point
 * Language:  C
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
 * Author: Gowrish
 * Purpose: Get Temperature data post Decimal Point
 * Language:  C
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
 * Author: Gowrish
 * Purpose:Calculate the power of 2
 * Language:  C
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
