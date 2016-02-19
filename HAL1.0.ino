


//Libraries
#include <SPI.h> // SPI Library




// Global Variables
int pins[4] = {2, 3, 4, 5}; //Input pins from WSN module
int mux[3] = {6, 7, 8}; //disp line select
int spi_sel = A0; //spi select data
int sw = 9; //switch line
int i, j, k; //Temporary Data
String WSN_data;


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
  Enable = digitalRead(pins[3]);
  if ((Enable != data))
  {
    data = Enable;

    Serial.println(WSN_data);
    WSN_data="";
    clocks = 0;
  }

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

}//End of Initialize





/*
 * Author: Gowrish
 * Purpose: Interrupt Routiene for Clock from WSN Module
 * Language:  C
 */
void WSN_Clock()
{
  if (digitalRead(pins[2]) == 0)
  {
    state = ~state;
    clocks++;
    //char just = char (digitalRead(3));
    WSN_data += digitalRead(pins[0]);
  }

  else
  {}

}//end of WSN Interrupt Clock Data
