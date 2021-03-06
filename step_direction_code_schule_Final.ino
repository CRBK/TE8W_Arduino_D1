/*

Task: Excecuting simple movements with a Stepper Motor.
In addition: read the curruent position.
Author: Niklas Schröder and Marco Thielen
*/
 


// User setup
bool refdirection = true;	//Reference direction, true = right, false = left
int lifting = 10;		//Lifting in mm
const float SPR = 200;		//Motor steps per rotation (360°)
const int feed = 10;		//Feed of axis (mm/rotation)
const int gear = 1;		//Gear, 1 = 1:1, 5 = 5:1
const int acceleration = 1;	//Acceleration (mm/s²)
const float speed = 10;		//Speed (mm/s)
const float refspeed = 1;	//Reference speed (mm/s)


// Outputs and inputs  
const int step = 2;  		//Output direction
const int direction = 3; 	//Output step
const int enable = 4;		//Output enable

const int LS = 5; 		//Input reference switch
const int start = 6;		//Input for starting the movement 
const int stop = 7;		//Input for stoping the movement


// Systemvariables
  int statestart;		//Variable to read state of input start
  int statestop;		//Variable to read state of input stop
  int stateLS;			//Variable to read state of input LS
  long SPlift;			//Steps per lift
  bool run;			//Starts steps to run
  long stepscounted;		//Counted lift steps
  float calcspeed;		//Calculated pausetime for normal speed
  float calcrefspeed;		//Calculated reference speed
  float steptime;		//Time per step/2
  bool Refdone;			//Is true if reference run is done
  bool Ref;			//Runs Reverence


void setup() {
  
    Serial.begin(9600);
  
  
// Setup the pins as Outputs
  pinMode(step, OUTPUT); 
  pinMode(direction, OUTPUT);
  pinMode(enable, OUTPUT);
  
  
// Setup the pins as Inputs, Pullup the Pins so the whole HIGH/LOW logic in the code is inversed 
  pinMode(LS, INPUT_PULLUP); 
  pinMode(start, INPUT);
  pinMode(stop, INPUT);
  
  
// Set reference direction
  if(refdirection == true)digitalWrite(direction,HIGH);
    else digitalWrite(direction,LOW);
  
  
// Calculation for steps per lift  
   SPlift = ((lifting/feed)*SPR*gear);
  
// Calculation for speed time (calcspeed = (1000000/((speed/feed)*SPR*gear))/2;)
  calcspeed=float(500000/(float(speed/feed*SPR*gear)));
  
// Calculation for speed time (calcrefspeed = (1000000/((refspeed/feed)*SPR*gear))/2;)
  calcrefspeed=float(500000/(float(refspeed/feed*SPR*gear)));
  
  
}


void loop() {

 
  statestart = digitalRead(start);
  statestop = digitalRead(stop);
  stateLS = digitalRead(LS);
  
  
  if(statestart == HIGH && Refdone == false)Ref = true;

	
  
  if(statestart == HIGH && Refdone == true)
  {
        digitalWrite(enable,HIGH);
    	steptime = calcspeed;
    	run = true;
    	delay(10);

  }
  
  
  if(statestop == HIGH)
  {
    	digitalWrite(enable,LOW);
    	run = false;
  }
  
  
// Direction turn to the left
  if(stepscounted >= SPlift && digitalRead(direction) == HIGH)
  {
	run = false;
    stepscounted = 0;
    digitalWrite(direction,LOW);
    delay(3000);
    run = true;
  }
 

// Direction turn to the right
  if(stepscounted >= SPlift && digitalRead(direction) == LOW)
  {
    run = false;
    stepscounted = 0;
    digitalWrite(direction, HIGH);
    delay(3000);
    run = true;
  }
  
  if(Ref == true)Reference();
  
  if(run == true)steps();
  else digitalWrite(step,LOW);
  
  
 Serial.println(stepscounted);
 Serial.println(SPlift);
 // Serial.println(calcspeed);
}


void Reference()
{
  calcrefspeed = steptime;
  digitalWrite(enable,HIGH);
  run = true;
  stepscounted = 0;
    if(stateLS == LOW)
    {
      	if(refdirection == true)digitalWrite(direction,LOW);
        else(digitalWrite(direction,HIGH));
        run = false;
      	digitalWrite(enable,LOW);
     	stepscounted = 0;
      	Refdone = true;
      	Ref = false;
    }
}


void steps()
{
  	digitalWrite(step,HIGH);
    	delayMicroseconds(steptime);
    	digitalWrite(step,LOW);
    	delayMicroseconds(steptime);
        stepscounted++;
}
