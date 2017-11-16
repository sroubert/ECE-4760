/**
 * The following code is draft code for Lab 1. The code is modified from the example timer capture signal generator
 * === OUTPUT COMPARE and INPUT CAPTURE ===
 * The system uses hardware to generate precisely timed
 * pulses, then uses input capture to compare the capture period
 * to the gereration period for acccuracy

 *This system will use hardware to charge a capacitor to an internal reference voltage and capture the time
 *when it reaches said voltage.




 *
 * There is a capture time print-summary thread
 * There is a one second timer tick thread
 * 
 * -- Pin 14 and 18 are output compare outputs
 * -- Pin 24 is input capture input -- connect this to one of the output compares
 *
 * -- TFT LCD connections explained elsewhere
 * Modified by Bruce Land 
 * Jan 2015
 */

////////////////////////////////////
// clock AND protoThreads configure!
// You MUST check this file!
#include "config.h"
// threading library
#include "pt_cornell_1_2.h"

////////////////////////////////////
// graphics libraries
#include "tft_master.h"
#include "tft_gfx.h"
// need for rand function
#include <stdlib.h>
////////////////////////////////////
 #include <math.h>

// === thread structures ============================================
// thread control structs

// note that UART input and output are threads
//static struct pt pt_print, pt_time ; //, pt_input, pt_output, pt_DMA_output ;

static struct pt pt_dischargeAndCharge, pt_print ; // 
/*
// system 1 second interval tick
int sys_time_seconds ;

//The measured period of the wave
short capture1, last_capture1=0, capture_period=99 ;
//The actual period of the wave
int generate_period=10000 ;*/

short capture1, capacitance, needToDischarge = 0 ;
short resistance = 100000 ;
short vref = 1.2 ;
short vdd = 3.3 ;

// == Capture 1 ISR ====================================================
// check every cpature for consistency

//following interrupt is to capture when the comparator reaches internal reference voltage
//the paramaters are which input capture and the interrupt priority
void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl3) C1Handler(void)
{

    /*capture1 = mIC1ReadCapture();
    capture_period = capture1 - last_capture1 ;
    last_capture1 = capture1 ;
    // clear the timer interrupt flag
    mIC1ClearIntFlag();*/

    capture1 = mIC1ReadCapture();

    capacitance = -capture1/(resistance*log(1-(Vc/Vdd));

    mIC1ClearIntFlag();

}

// === Period print Thread ======================================================
// prints the captured period of the generated wave
static PT_THREAD (protothread_print(struct pt *pt))
{
    PT_BEGIN(pt);
      // string buffer
      char buffer[128];
      tft_setCursor(0, 0);
      tft_setTextColor(ILI9340_WHITE);  tft_setTextSize(1);
      tft_writeString("Connect pin18 OR pin14 to pin24\n");

      while(1) {
            // print every 200 mSec
            PT_YIELD_TIME_msec(200) ;
            // erase
            tft_fillRoundRect(0,50, 200, 20, 1, ILI9340_BLACK);// x,y,w,h,radius,color
            // print the periods
            tft_setCursor(0, 50);
             sprintf(buffer,"cap=%d",
                capacitance);
            tft_writeString(buffer);

            //wait 100ms


      } // END WHILE(1)
  PT_END(pt);
} // thread 4

// === One second Thread ======================================================
// this thread will discharge capacitor fulfilling #3 of the lab
static PT_THREAD (protothread_dischargeAndCharge(struct pt *pt))
{
    PT_BEGIN(pt);

    //

      while(1) {
            // yield time 1 second
            //PT_YIELD_TIME_msec(200) ;
            PT_WAIT_THREAD(pt, protothread_print(&pt_print));
            //yielding while printing
            PT_YIELD_TIME_msec(100);
            //sys_time_seconds++ ;
            // NEVER exit while

            //setting to output
            mPORTBSetPinsDigitalOut( BIT_3) ;
            //clearning bit
            PORTClearBits(IOPORT_B, BIT_3);

            PT_YIELD_TIME_msec(200) ;

            //setting to input so now bit capacitor will charge
            mPORTBSetPinsDigitalIn( BIT_3) ;

            writeTimer1(0x0000) ;

            //here is where we wait 
      } // END WHILE(1)

  PT_END(pt);
} // 

// === Main  ======================================================

int main(void)
{
  
  // === Config timer and output compares to make pulses ========
  // set up timer1 to generate the wave period
  OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xffff);
  //ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
  //mT2ClearIntFlag(); // and clear the interrupt flag

  /*
  // set up compare3 for double compare mode
  // first number is the time to clear, second is the time to set the pin
  // in this case, the end of the timer period and 50% of the timer period
  OpenOC3(OC_ON | OC_TIMER2_SRC | OC_CONTINUE_PULSE , generate_period, generate_period>>1); //
  // OC3 is PPS group 4, map to RPB9 (pin 18)
  PPSOutput(4, RPB9, OC3);
 // mPORTASetPinsDigitalOut(BIT_3);    //Set port as output -- not needed

  // set pulse to go high at 1/4 of the timer period and drop again at 1/2 the timer period
  OpenOC2(OC_ON | OC_TIMER2_SRC | OC_CONTINUE_PULSE, generate_period>>1, generate_period>>2);
  // OC2 is PPS group 2, map to RPB5 (pin 14)
  PPSOutput(2, RPB5, OC2);
 // mPORTBSetPinsDigitalOut(BIT_5); //Set port as output -- not needed

  // === Config timer3 free running ==========================
  // set up timer3 as a souce for input capture
  // and let it overflow for contunuous readings
  OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_1, 0xffff);
  */

  // === set up input capture ================================
  OpenCapture1(  IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_ON );
  // turn on the interrupt so that every capture can be recorded
  ConfigIntCapture1(IC_INT_ON | IC_INT_PRIOR_3 | IC_INT_SUB_PRIOR_3 );
  INTClearFlag(INT_IC1);
  // connect PIN 24 to IC1 capture unit
  PPSInput(3, RPB13, IC1);

  // Set port as input (pin 7 is RB3)
  mPOORTBSetPinDigialIn( BIT_3 );

  //set up compare 1
  CMP1Open( CMP_ENABLE | CMP_OUTPUT_ENABLE | CMP1_NEG_INPUT_IVREF );

  // connect pin18 and C1OUT
  PPSOutput( 4, RPB9, C1OUT );

  // init the display
  tft_init_hw();
  tft_begin();
  tft_fillScreen(ILI9340_BLACK);
  //240x320 vertical display
  tft_setRotation(0); // Use tft_setRotation(1) for 320x240
  tft_setCursor(0, 0);
  
  // === config the uart, DMA, vref, timer5 ISR ===========
  PT_setup();

  // === setup system wide interrupts  ====================
  INTEnableSystemMultiVectoredInt();
  
  // === now the threads ===================================
  // init the threads
  PT_INIT(&pt_print);
  PT_INIT(&pt_dischargeAndCharge);

  // schedule the threads
  while(1) {
    PT_SCHEDULE(protothread_print(&pt_print));
    PT_SCHEDULE(protothread_discharge(&pt_dischargeAndCharge));
  }
} // main