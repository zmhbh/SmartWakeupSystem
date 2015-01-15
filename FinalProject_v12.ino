
/*****************************************************************\
** Version 1
** Code takes raw serial data and parses it into something useful
** Towards the end of the file there are some nested case statements
** these are in the section of code for interpreting completed
** packets, any user code should go there. There may be some errors
** in the code, I have done some testing, but it is pretty difficult
** to verify things are working correctly while you are asleep...
** 
** I have intentionally left the case statements empty for the most
** part for individuals to write their own implementation. 
** 
** Arduino Configuration:
** this code was developed on an arduino mega, it MUST be modified
** to work on an UNO or similar.
**
** Serial (usb port) 115200 baud -- data going to SerialMonitor
**         fast baud to minimize time spent sending data
**
** Serial1 (pin 19) 38400 baud -- data coming from Zeo
**         code referencing this serial port must be changed if you
**         are using an arduino with only 1 serial port. I'm using
**         the mega because future itterations will be controlling
**         lights, audio playback, temp/humidity logging, etc...
**         thus I want to use a more powerful controller.
\*****************************************************************/



//-----------------------------------------------------------------------------
//                 Utility
//-----------------------------------------------------------------------------
#include<SD.h>
int led = 13;
int recordindex=1;
char datafile[]="data0.txt";
char datafile1[]="data1.txt";
char datafile2[]="data2.txt";
char datafile3[]="data3.txt";
char datafile4[]="data4.txt";
char datafile5[]="data5.txt";
char datafile6[]="data6.txt";
char datafile7[]="data7.txt";
char datafile8[]="data8.txt";
char datafile9[]="data9.txt";
char datafile10[]="data10.txt";
char datafile11[]="data11.txt";
char datafile12[]="data12.txt";
char datafile13[]="data13.txt";
char datafile14[]="data14.txt";
char datafile15[]="data15.txt";
char datafile16[]="data16.txt";
char datafile17[]="data17.txt";
char datafile18[]="data18.txt";
char datafile19[]="data19.txt";
char datafile20[]="data20.txt";
char datafile21[]="data21.txt";
char datafile22[]="data22.txt";
char datafile23[]="data23.txt";
char datafile24[]="data24.txt";
char datafile25[]="data25.txt";
char datafile26[]="data26.txt";
char datafile27[]="data27.txt";

//void Vibration();

unsigned long time;
int k=0;
File myFile;
int x=0, y=0, z=0;
int x_l, y_l, z_l;
float xx,yy,zz;
//unsigned long unix_offset=1413840060;   //  2014/10/20 21:21:00
enum event_t {
	NightStart = 1,
	SleepOnset = 2,
	HeadbandDocked = 4,
	HeadbandUnDocked = 8,
	AlarmOff = 16,
	AlarmSnooze = 32,
	AlarmPlay = 64,
	NightEnd = 128,
	NewHeadband = 256
};
enum data_t {
	Event = 1,
	SliceEnd = 2,
	Version = 4,
	Waveform = 8,
	FrequencyBins = 16,
	SQI = 32,
	ZeoTimestamp = 64,
	Impedance = 128,
	BadSignal = 256,
	SleepStage = 512
};
enum freqBin_t {
	Delta,
	Theta,
	Alpha,
	BetaL,
	BetaH,
	BetaSS,
	Gamma
};
enum stage_t {
	Undefined,
	Awake,
	REM,
	Light,
	Deep
};
union {
  unsigned long ul;
  byte data[sizeof(unsigned long)];  
} ulc;

union {
  long l;
  byte data[sizeof(long)];
} lc;

long getInt32(byte a, byte b, byte c, byte d){
	lc.data[0] = a;
    lc.data[1] = b;
    lc.data[2] = c;
    lc.data[3] = d;
	return lc.l;
}
unsigned long getUInt32(byte a, byte b, byte c, byte d){
	ulc.data[0] = a;
    ulc.data[1] = b;
    ulc.data[2] = c;
    ulc.data[3] = d;
	return ulc.ul;
}
int getInt16(byte a, byte b){
	return (int) word(b,a);
}
unsigned int getUInt16(byte a, byte b){
	return (unsigned int) word(b,a);
}


//-----------------------------------------------------------------------------
//                 Class Declarations
//-----------------------------------------------------------------------------


class Zeo {
	public:
		Zeo();
		void Update();
                void Vibration();
	protected:
		void ClearBuffer();
		void Printbuffer(int y,int x);
	private:
		unsigned int aOffset; //what byte are we currently reading?
		boolean aSearching;
		byte zData[512];
		unsigned long zTime;//in unix time
		byte dWirelessSignal;//current signal quality
		//current sleep state
		event_t dEventTracker;//event bitfield
		float zFrequencies[7];//currently frequency bin
		bool dHeadband; //true for headband connected and sending good data
		unsigned int dHeadband_badCount; //if we have a headband disconnected for a while, fire an event
		//bitfield array for suppressing outputs
};


//-----------------------------------------------------------------------------
//                 Global Variables
//-----------------------------------------------------------------------------


Zeo * zeo1 = new Zeo(); //constructor initiates the zeo's serial port

//---------------;-------------------------------------------------------------
//                 Main Program
//-----------------------------------------------------------------------------

void setup() {
	//remove this line and change all instances of Serial1 to Serial  if you are using a standard arduino Uno.
	// Serial.begin(115200);
 //Serial.print("Initializing SD card...");
 pinMode(53, OUTPUT);
 pinMode(led, OUTPUT);     

 if (!SD.begin(53)) {
   // Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
//  FlexiTimer2::set(50,Vibration);
//  FlexiTimer2::start();

}

void loop() {
      time=0;
      if(k==0)  
     { 
     myFile = SD.open(datafile, FILE_WRITE);
   }
      
      if(k==1)       
      {
      myFile = SD.open(datafile1, FILE_WRITE);
       }
      
      if(k==2)       
      {
        myFile = SD.open(datafile2, FILE_WRITE);
      }
      if(k==3)       
      {
        myFile = SD.open(datafile3, FILE_WRITE);
      }
      if(k==4)      
     {
       myFile = SD.open(datafile4, FILE_WRITE);
     }
      if(k==5)      
     { 
       myFile = SD.open(datafile5, FILE_WRITE);
     }
      if(k==6)      
     {
       myFile = SD.open(datafile6, FILE_WRITE);
     }
      if(k==7)       
    {
     myFile = SD.open(datafile7, FILE_WRITE);
    }
      if(k==8)       
      {
        myFile = SD.open(datafile8, FILE_WRITE);
      }
      if(k==9)       
      {
        myFile = SD.open(datafile9, FILE_WRITE);
      }
      if(k==10)       
      {
        myFile = SD.open(datafile10, FILE_WRITE);
      }
      if(k==11)       
      {
        myFile = SD.open(datafile11, FILE_WRITE);
      }
      if(k==12)       
      {
        myFile = SD.open(datafile12, FILE_WRITE);
      }
      if(k==13)       
      {
        myFile = SD.open(datafile13, FILE_WRITE);
      }
      if(k==14)       
      {
        myFile = SD.open(datafile14, FILE_WRITE);
      }
      if(k==15)       
      {
        myFile = SD.open(datafile15, FILE_WRITE);
      }
      if(k==16)       
      {
        myFile = SD.open(datafile16, FILE_WRITE);
      }
      if(k==17)       
      {
        myFile = SD.open(datafile17, FILE_WRITE);
      }
      if(k==18)       
      {
        myFile = SD.open(datafile18, FILE_WRITE);
      }
      if(k==19)       
      {
        myFile = SD.open(datafile19, FILE_WRITE);
      }
      if(k==20)       
      {
        myFile = SD.open(datafile20, FILE_WRITE);
      }
      if(k==21)       
      {
        myFile = SD.open(datafile21, FILE_WRITE);
      }
      if(k==22)       
      {
        myFile = SD.open(datafile22, FILE_WRITE);
      }
     if(k==23)       
      {
        myFile = SD.open(datafile23, FILE_WRITE);
      }
      if(k==24)       
      {
        myFile = SD.open(datafile24, FILE_WRITE);
      }
      if(k==25)       
      {
        myFile = SD.open(datafile25, FILE_WRITE);
      }
      if(k==26)       
      {
        myFile = SD.open(datafile26, FILE_WRITE);
      }
      if(k==27)       
      {
        myFile = SD.open(datafile27, FILE_WRITE);
      }
      myFile.println("update success");
       while(time<150000000)   //15000000=1200s=20min
        {
        
        zeo1 -> Update();
        time++;
      }
      
      myFile.close();
      k=k+1;
	//logic here to get state and determine if anything has changed and what should be done
	//delay(1); //I don't believe in delays
}





//-----------------------------------------------------------------------------
//                 Zeo Class
//-----------------------------------------------------------------------------


Zeo::Zeo(){
	//open the seial link, change to "Serial" if not using an arduino mega
	Serial1.begin(38400);

	//clear any pending data from the stream
	while ( Serial1.available() > 0 ) {
		byte discard = Serial1.read();
	};
	ClearBuffer();
}

void Zeo:: Vibration(){
  x_l=x;
  y_l=y;
  z_l=z;
 //float v_threshold=0.0049;
 long  vibra;
  x = analogRead(0);       // read analog input pin 0
  y = analogRead(1);       // read analog input pin 1
  z = analogRead(2);       // read analog input pin 2
  xx=abs((x-x_l));
  yy=abs((y-y_l));
  zz=abs(z-z_l);
  vibra=xx+yy+zz;
 
  myFile.print("Vibration:");
  myFile.println(vibra);

  
  /*
  if(xx>v_threshold)
  {
  //Serial.print(xx, DEC);    // print the acceleration in the X axis
  //Serial.print(" ");       // prints a space between the numbers
 // Serial.println("Event: Vibration!!!!@@@@@*******");
  myFile.println("Event: Vibration!!!!@@@@@*******");
//  Serial.println("Event: Vibration!!!!@@@@@*******");

  }
  else if(yy>v_threshold)
  {
 // Serial.print(yy, DEC);    // print the acceleration in the Y axis
 // Serial.print(" ");       // prints a space between the numbers
 // Serial.println("Event: Vibration!!!!@@@@@******");
  myFile.println("Event: Vibration!!!!@@@@@******");
//  Serial.println("Event: Vibration!!!!@@@@@*******");

  }
  else if(zz>v_threshold)
  {
  //Serial.println(zz, DEC);  // print the acceleration in the Z axis
 // Serial.println("Event: Vibration!!!!@@@@@*****");
   myFile.println("Event: Vibration!!!!@@@@@******");
 //  Serial.println("Event: Vibration!!!!@@@@@*****");
  }      
  
  */
  
  
}

//used to clear out the received data buffer
//and set the update function to searching mode
void Zeo::ClearBuffer(){
	int m;
	aOffset = 0;
	aSearching = true;
	for(m = 0; m < 512; m++) {
		zData[m] = 0;
	}
}

//debug function to print the buffer, x and y indicate start and stop offsets
void Zeo::Printbuffer(int y, int x){
	int m;
	for (m = y;m <= x;m=m+2){ 
		int rawtemp;
               // if ( zData[m] < 10 ) { Serial.print(" "); };
		// if ( zData[m] < 100 ) { Serial.print(" "); };
                //rawtemp=float(getInt16(zData[m],zData[m+1])*315)/0x8000;
		rawtemp=getInt16(zData[m],zData[m+1]);
       //         Serial.print(rawtemp);
	//	Serial.println(",");   
                myFile.print(rawtemp);
		myFile.println(",");
            //    Vibration();

	}
  
}

void Zeo::Update() {
	 
	while (Serial1.available() > 0) {
		//looped since some of the other update functions push data to the 12c bus,
		//we don't want to miss anything when we get our chance to update...
		unsigned int len = 500; //arbitrarily high
		
		//if we have read the first 6 bytes calculate the length of the packet
		if (aOffset > 6) {
			len = getUInt16( zData[3],zData[4] );
			len += 10; //zeo length does not take into account the beginning of the pacekt
		}
		
		//if we are almost at the end of the frame check to see if the last byte would be a 65
		//in my testing I occasionally get a packet that is 1 byte shorter than it should be
		//thus the next packet is dropped since the next start byte is skipped. This code
		//checks to see if the last byte in a message is a 65 and if so decrease the length by 1
		//so that normal packet processing operations happen. I assume that this may mess up eeg
		//data if my assumption is wrong. However, since I don't want to do eeg processing on
		//a microcontroller due to memory/processor constraints, I don't have any test data
		//on if it does mess it up.
		if(aOffset + 1 == len && Serial1.peek() == 65 ) {
			len--;
		}
		
		//read the next available byte from the serial connection
		byte tmp = Serial1.read();
		
		
		if (tmp == 65 && aSearching) { //if we are searching for the beginning of a new packet
			ClearBuffer();
			aSearching = false;
			zData[aOffset] = tmp;
			aOffset++;
		} else if (aSearching == false) { //otherwise append data
			
			//error flag is used instead of continue/break statements
			boolean err = false;
			
			//assign the most recent byte to the buffer
			zData[aOffset] = tmp;
			
			//datastream error checking
			switch (aOffset) {
				case 1:
					if ( zData[0] != 65 || zData[1] != 52 ) { //check to see if this is the beginning of a valid packet
						myFile.println("Length error");

						err = true;
					}
					break;
				case 6:
					if( zData[3] ^ 0xff != zData[5] && zData[4] ^ 0xff != zData[6]){//check to see if the length is correct
						err = true;
						myFile.println( "message length error" );
					}
					break;
				//case 9: //logic for timestamping
			};

			if (aOffset >= len) {
				//---------------------------------------
				//   Packet Complete, begin processing
				//---------------------------------------
				
				//debug code that prints the raw packet
				//Printbuffer(0,len);
				//Serial.println("");
				
				//determine datatype
				switch ( zData[11] ) {
					case 0x00: //Event
					//	Serial.print("Event: ");
						myFile.print("Event: ");

						switch( zData[12] ) {
							case 0x05://night start
                                          //                    Serial.println("night start");

								break;          
							case 0x07://Sleep onset
								break;
							case 0x0E://Headband Docked
                                                              myFile.println("Headband Docked");
                                                              myFile.print("@@@---The End----@@@");
                                                     
                                                              myFile.println(recordindex);
                                                              recordindex++;
                                                              myFile.close();

                                                          //    myFile.println("Headband Docked");
								break;
							case 0x0F://Headband Undocked
                                                               myFile.println("Headband Undocked");
                                                                if(k==0)  
     { 
     myFile = SD.open(datafile, FILE_WRITE);
   }
      
      if(k==1)       
      {
      myFile = SD.open(datafile1, FILE_WRITE);
       }
      
      if(k==2)       
      {
        myFile = SD.open(datafile2, FILE_WRITE);
      }
      if(k==3)       
      {
        myFile = SD.open(datafile3, FILE_WRITE);
      }
      if(k==4)      
     {
       myFile = SD.open(datafile4, FILE_WRITE);
     }
      if(k==5)      
     { 
       myFile = SD.open(datafile5, FILE_WRITE);
     }
      if(k==6)      
     {
       myFile = SD.open(datafile6, FILE_WRITE);
     }
      if(k==7)       
    {
     myFile = SD.open(datafile7, FILE_WRITE);
    }
      if(k==8)       
      {
        myFile = SD.open(datafile8, FILE_WRITE);
      }
      if(k==9)       
      {
        myFile = SD.open(datafile9, FILE_WRITE);
      }
      if(k==10)       
      {
        myFile = SD.open(datafile10, FILE_WRITE);
      }
      if(k==11)       
      {
        myFile = SD.open(datafile11, FILE_WRITE);
      }
      if(k==12)       
      {
        myFile = SD.open(datafile12, FILE_WRITE);
      }
      if(k==13)       
      {
        myFile = SD.open(datafile13, FILE_WRITE);
      }
      if(k==14)       
      {
        myFile = SD.open(datafile14, FILE_WRITE);
      }
      if(k==15)       
      {
        myFile = SD.open(datafile15, FILE_WRITE);
      }
      if(k==16)       
      {
        myFile = SD.open(datafile16, FILE_WRITE);
      }
      if(k==17)       
      {
        myFile = SD.open(datafile17, FILE_WRITE);
      }
      if(k==18)       
      {
        myFile = SD.open(datafile18, FILE_WRITE);
      }
      if(k==19)       
      {
        myFile = SD.open(datafile19, FILE_WRITE);
      }
      if(k==20)       
      {
        myFile = SD.open(datafile20, FILE_WRITE);
      }
      if(k==21)       
      {
        myFile = SD.open(datafile21, FILE_WRITE);
      }
      if(k==22)       
      {
        myFile = SD.open(datafile22, FILE_WRITE);
      }
     if(k==23)       
      {
        myFile = SD.open(datafile23, FILE_WRITE);
      }
      if(k==24)       
      {
        myFile = SD.open(datafile24, FILE_WRITE);
      }
      if(k==25)       
      {
        myFile = SD.open(datafile25, FILE_WRITE);
      }
      if(k==26)       
      {
        myFile = SD.open(datafile26, FILE_WRITE);
      }
      if(k==27)       
      {
        myFile = SD.open(datafile27, FILE_WRITE);
      }	

                                                      
								break;
							case 0x10://Alarm turned Off
								break;
							case 0x11://alarm snoozed
								break;
							case 0x13://alarm has started playing
								break;
							case 0x15://night end
								break;
							case 0x24://new headband detected
								break;
							default://undefined, error probably happened
								//Serial.println("[read error]");
								err = true;
								break;
						};
						break;
					case 0x02: //Slice End
						break;
					case 0x03: //Version
						break;
					case 0x80: //eeg waveform
                                                if (dWirelessSignal>8)
                                                {
                                                
						Printbuffer(12, len); //print the raw waveform data
                                                }
                                                Vibration();
						break;
					case 0x83: //FrequencyBins
						//the following code may be incorrect, needs more testing.
						int j,k;
						j=0;
						//Serial.print("frequency bins:");

                                               /* 
						for(k=12;k<=24;k+=2){
							zFrequencies[j] = float( getInt16( zData[k], zData[k + 1] ) ) / 0x8000;
						//	Serial.print(zFrequencies[j]);
						//	Serial.print("; ");
							j++;
						}
						//Serial.println("");
						//myFile.println("");
                                                */
						break;
					case 0x84: //Wireless Signal Quality
						dWirelessSignal = zData[12];
                                              myFile.print("Signal Quality: ");
                                              myFile.println(zData[12]);
                                              Vibration();
						break;
					case 0x8A: //ZeoTimestamp (uint32 in unix time format)
						zTime = getUInt32(zData[12],zData[13],zData[14],zData[15]);
						//Serial.print("Unix Timestamp (s):");
						//Serial.println(zTime);
                                                myFile.print("Unix Timestamp (s):");
						myFile.println(zTime);
                                              Vibration();
                                               // Vibration();
                                                /*
						if(zTime%900==0)
                                                {
                                                  myFile.close();
                                                  myFile = SD.open(datafile, FILE_WRITE);
                                                  if(myFile)
                                                  {
                                                  myFile.println("success");
                                                  }
                                                }
                                                */
                                                break;
					case 0x97: //Impedance of headband
						break;
					case 0x9C: //BadSignal, is sent every second when the headband is not detecting the user
						if(zData[12] == 1) {
							dHeadband = false;
							dHeadband_badCount++;
						} else {
							dHeadband = true;
							dHeadband_badCount = 0;
						}
						//logic hear for excessive headband erroring...
						break;
					case 0x9D: //SleepStage
						switch( zData[12] ) {
							case 0x00://undefined
								break;
							case 0x01://Awake
                                                        myFile.println("Awake");

								break;
							case 0x02://REM
                                                        myFile.println("REM");
								break;
							case 0x03://Light
                                                        myFile.println("Light");
								break;
							case 0x04://Deep
                                                        myFile.println("Deep");
								break;
							default://undefined
							//	Serial.println("Undefined Sleep Stage");
								err = true;
								break;
						};
						break;
					default: //read error of some kind
					//	Serial.println("Bad Datatype");
						err = true;
						break;
				};
				
				
				ClearBuffer();
				return;
			}//end completed packet processing
			
			//prepare for next itteration
			if (err) {
				ClearBuffer();
			} else {
				aOffset++;
			}
		}
		
	}
	return;
}
