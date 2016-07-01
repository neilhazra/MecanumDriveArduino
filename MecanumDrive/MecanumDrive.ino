/*
 *Neil Hazra
 *Send Data via Processing: 5 floats, semicolon delimiter, add semicolons till reading newe line
 */
#include <Servo.h>
const int dataSizeConst = 6;
const int maxSize = 30;
float processedData[dataSizeConst];
int dataSize;
char incommingData[maxSize];
char _incommingData[maxSize];
unsigned long startingTime = 0;
float v1,v2,v3,v4;
float theta;
double prevTime = 0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
void setup() {
  Serial.begin(9600);
  pinMode(19, INPUT_PULLUP); //resister for Bluetooth 
  servo1.attach(A0);
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3); 
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
}
void serialEvent()  {
   Serial.readStringUntil('\n').toCharArray(incommingData,maxSize);
   saveData();
   convertData();
   setMotors();
   prevTime = millis();
}
void loop() {
  servo2.writeMicroseconds(mapf(v1,1,-1,670,2330)); //frontleft
  servo3.writeMicroseconds(mapf(v2,1,-1,670,2330)); //frontright
  servo1.writeMicroseconds(mapf(v3,1,-1,670,2330)); //backleft
  servo4.writeMicroseconds(mapf(v4,1,-1,670,2330)); //backright
  
  while(millis()-prevTime > 300 && !Serial.available())  {
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  }
}  
void setMotors()  {
  float resultant = sqrt(pow(processedData[0],2) + pow(processedData[1],2));
  float theta = atan2(processedData[0],processedData[1]);
  v1 = resultant*sin(theta + PI/4)+ processedData[2];
  v2 = resultant*cos(theta + PI/4)- processedData[2];
  v3 = resultant*cos(theta + PI/4)+ processedData[2];
  v4 = resultant*sin(theta + PI/4)- processedData[2];

}
void convertData()  {
  for(int i = 0; i<dataSizeConst; i++)  {
    processedData[i] = processedData[i];
    Serial.print(processedData[i]);
    Serial.print(" ");
  }
  Serial.println();
}
void saveData()  {
    for(int i = 0; i<50; i++)  {
      _incommingData[i] = incommingData[i];
    }
   char *p = _incommingData;
   char *str;
   char *data[dataSizeConst];
   int i = 0;
   while ((str = strtok_r(p, ";", &p)) != NULL)   {// delimiter is the semicolon
     data[i] = str;
     i++;
   }
   dataSize = i;
   for(int _i = 0; _i<i; _i++)  {
     processedData[_i] = atof(data[_i]);
   }
}
/*
double atof(const char *s)
{
	double a = 0.0;
	int e = 0;
	int c;
	while ((c = *s++) != '\0' && isdigit(c)) {
		a = a*10.0 + (c - '0');
	}
	if (c == '.') {
		while ((c = *s++) != '\0' && isdigit(c)) {
			a = a*10.0 + (c - '0');
			e = e-1;
		}
	}
	if (c == 'e' || c == 'E') {
		int sign = 1;
		int i = 0;
		c = *s++;
		if (c == '+')
			c = *s++;
		else if (c == '-') {
			c = *s++;
			sign = -1;
		}
		while (isdigit(c)) {
			i = i*10 + (c - '0');
			c = *s++;
		}
		e += i*sign;
	}
	while (e > 0) {
		a *= 10.0;
		e--;
	}
	while (e < 0) {
		a *= 0.1;
		e++;
	}
	return a;
}
*/
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  //return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

