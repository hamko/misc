#include <Wire.h>

#define CYC_INT 5
#define dt (0.001*CYC_INT)
const float g = 9.8;

const uint8_t MPU=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

#define UTHETA 6.75038043e-7
#define UTHETAD (UTHETA/dt)
#define UGRAVITY 0.00055056179
//#define UGRAVITY 0.00086438201

// initial parameter
float th = 0.0;     // [rad]
float bi = 0.0;    // [rad/s]
float sxx = 1;    // [rad^2], initial variance of theta (about x3)
float sxy = 0;      // [rad rad/s]
float syy = 0.1;    // [(rad/s)^2], initial variance of bias
// tuning parameter
#define VAR_ACCEL 0.5 // [m/ss], std of acceleration
#define VAR_OMEGA 0.1 // [rad/s], variance of omega at 1 rad/s 
float q = VAR_ACCEL*VAR_ACCEL;      // [(m/s)^2]
float alpha = VAR_OMEGA*VAR_OMEGA;  // [(rad/s)^2]
#define FORCE_NOISE 0.2 // [(m/ss)/N], 1N出すことによる加速度センサに乗るだろう標準偏差

void kalman(float o, float a, float f)
{
    // gyro
    float p_th, p_bi, p_sxx, p_syy, p_sxy;
    p_th=th, p_bi=bi, p_sxx=sxx, p_syy=syy, p_sxy=sxy;
    th = p_th + (o-p_bi)*dt;
    sxy = p_sxy - p_syy*dt;
    sxx = p_sxx + (alpha*o*o+p_syy)*dt*dt-2*p_sxy*dt;

    p_th=th, p_bi=bi, p_sxx=sxx, p_syy=syy, p_sxy=sxy;
    float k1 = g/(g*g*p_sxx+(q+FORCE_NOISE*f*f)), k2 = k1*g;
    th = p_th + k1*(a-g*sin(p_th))*p_sxx;
    bi = p_bi + k1*(a-g*sin(p_th))*p_sxy;
    sxx = p_sxx - k2*p_sxx*p_sxx;
    sxy = p_sxy - k2*p_sxx*p_sxy;
    syy = p_syy - k2*p_sxy*p_sxy;
}

// you can send only 4 character in control.
long t = 0;
void control() {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission();

    Wire.requestFrom((uint8_t)MPU,(uint8_t)14);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    float o = GyX * UTHETAD;
    float a = AcZ * UGRAVITY;
    float f = 0;

    //if (t % 100 == 0){Serial.print(o*10000); Serial.print("\n");Serial.print(bi*10000); Serial.print("\n");}
    //if (t % 100 == 0){Serial.print(a); Serial.print("\n"); Serial.print(o*10000); Serial.print("\n");}
    if (t % 200 == 0){Serial.print(th); Serial.print("\n");Serial.print(bi*10000); Serial.print("\n");}

    //th += (o - bi)*dt;
    kalman(o, a, f);

    t++;
}

void setup(){
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write((uint8_t)0x6B);  // PWR_MGMT_1 register
    Wire.write((uint8_t)0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission();
    Serial.begin(9600);
    Serial.print("Start.\n");
}

void loop(){
    static word time_last = 0;

    if ((int)((word)millis()-time_last)>=CYC_INT*2) {
        Serial.print("!");
    } 
    if ((int)((word)millis()-time_last)>=CYC_INT) {
        control();
        time_last+=CYC_INT;
    }
}
