#define enca 2
#define encb 3  //Choose digital pins with interrupts for taking in encoder output

#define dira1 22
#define dira2 23
#define dirb1 24
#define dirb2 25  //direction pins for L293N motor driver

#define pwma 4
#define pwmb 5  //Enabler pins for L293N motor driver which takes in pwm signals to control speed

volatile long enacount = 0;
volatile long enbcount = 0;
long prevTime = 0;
float ePrev = 0;
float eIntegral = 0;
int value = 50;  //takes the required motor speed from 0 to 100, code can be changed to get input serially as well


void setup() {
  
  pinMode(enca, INPUT);
  pinMode(encb, INPUT);  //Setting encoder pins as input pins for arduino

  pinMode(dira1, OUTPUT);
  pinMode(dira2, OUTPUT);
  pinMode(dirb1, OUTPUT);
  pinMode(dirb2, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);  //Setting direction pins and enabler pins (pwm pins) as output pins

  digitalWrite(dira1, HIGH);
  digitalWrite(dira2, LOW);
  digitalWrite(dirb1, HIGH);
  digitalWrite(dirb2, LOW); 
  /* Setting the direction pins of L293N motor driver so that 
     motors move in the same direction
     Can also be done physically by connecting the pins to 
     either +5V or ground instead of setting them as high or low through code
  */

  attachInterrupt(digitalPinToInterrupt(enca), handleenca, RISING);
  attachInterrupt(digitalPinToInterrupt(encb), handleencb, RISING);  //attaching interrupts to the encoder pins to increment counts

  Serial.begin(2000000);
}

void handleenca() {  //ISR for Encoder a
  enacount++;
}

void handleencb() {  //ISR for Encoder b
  enbcount++;
}

void loop() {
  int valadj = map(value, 0, 100, 0, 255);
  analogWrite(pwma, valadj);  //Giving 1st motor a speed to run at

  int target = enacount;  //Setting a target that encoder b has to reach

  float kp = 1.10;
  float kd = 0.06;
  float ki = 0.001;  //PID constant values, obtained through trial and error and varies with setup

  float u = pidController(target, kp, kd, ki);
  moveMotor(dirb1, dirb2, pwmb, u);

  Serial.print(enacount);
  Serial.print("    ");
  Serial.print(enbcount); /* Printing encoder counts so that it can be observed in serial plotter
                             Useful for finding fine tuning the PID constant values */
  Serial.println();
}
void moveMotor(int dir1, int dir2, int pwm, float u) {  //function to move the 2nd motor and match its motion wrt first motor
  float speed = fabs(u);  //Speed of the 2nd motor is taken as the absolute value of the calculated error for the encoder counts
  if (speed > 240)                                      
    speed = 240;
    /* 
    The limit 240 is set for the current motor(N20,600 rpm) as it tends to overshoot ny a large 
    value, resulting in oscillations, if given with speed = 255; This value is motor specific 
    */
  if (enbcount > enacount)  //Taking into consideration possibility of overshooting
    speed = 0;
  analogWrite(pwmb, speed);  //Sending the required speed to 2nd motor
}
float pidController(int target, float kp, float kd, float ki) {  //function to calculate error between the encoder counts by PID logic
  long currentTime = micros();
  float delta = ((float)(currentTime - prevTime)) / 1.0e6;
  int e = enbcount - enacount;                    //Calculating linear error
  float eDer = (e - ePrev) / delta;               //Calculating derivative error
  eIntegral += e * delta;                         //Calculating integral error
  float u = kp * e + kd * eDer + ki * eIntegral;  //Net error stored in variable 'u'
  prevTime = currentTime;
  ePrev = e;  //Updating time and error for the nextg iteration
  return u;
}
