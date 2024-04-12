#include <Wire.h>
#include <SparkFun_KX13X.h>

SparkFun_KX134 kxAccel;

outputData myData; // Struct for the accelerometer's data
float initialPitch = 0.0;
float initialRoll = 0.0;
float degDisplacement = 0.0;
float currentPitch = 0.0;
float currentRoll = 0.0;
int numRotations = 0;

void setup()
{

  Wire.begin();
  Serial.begin(115200);

  // Wait for the Serial monitor to be opened.
  while (!Serial)
    delay(50);

  if (!kxAccel.begin())
  {
    Serial.println("Could not communicate with the the KX13X");
  }
  Serial.println("Ready.");

  if (kxAccel.softwareReset())
    Serial.println("Reset.");
  delay(5);

  kxAccel.enableAccel(false);

  kxAccel.setRange(SFE_KX134_RANGE16G);     

  kxAccel.enableDataEngine(); // Enables the bit that indicates data is ready.
  kxAccel.enableAccel();
  
}

void loop()
{
  // Check if data is ready
  if (kxAccel.dataReady())
  {
    kxAccel.getAccelData(&myData);
    float X = myData.xData;
    float Y = myData.yData;
    float Z = myData.zData;
    float pitch, roll;
    //Calculate pitch and roll
    pitch = atan2(Y, sqrt(X * X + Z * Z)) * (180.0 / PI);
    roll = atan2(-X, Z) * (180.0 / PI);
    //calculate differences in pitch and roll from last update
    currentPitch = abs(pitch-initialPitch);
    initialPitch = pitch;
    currentRoll = abs(roll-initialRoll);
    initialRoll = roll;
    //calculate degree displacement
    degDisplacement = degDisplacement+currentPitch+currentRoll;
    if (degDisplacement>360){ //if degree>=360, reset angles
      degDisplacement -= 360;
      numRotations++;
      currentPitch = 0;
      currentRoll = 0;
      initialPitch = 0;
      initialRoll = 0;
      Serial.print("Num Rotations: ");
      Serial.println(numRotations);
    }
  }
  delay(500);
}