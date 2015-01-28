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
float bi = 1.00;    // [rad/s]
float sxx = 0.5;    // [rad^2], initial variance of theta (about x3)
float sxy = 0;      // [rad rad/s]
float syy = 0.1;    // [(rad/s)^2], initial variance of bias
// tuning parameter
#define VAR_ACCEL 0.5 // [m/ss], std of acceleration
#define VAR_OMEGA 0.1 // [rad/s], variance of omega at 1 rad/s 
float q = VAR_ACCEL*VAR_ACCEL;      // [(m/s)^2]
float alpha = VAR_OMEGA*VAR_OMEGA;  // [(rad/s)^2]
// model parameter
#ifdef PRECISE_MODE
#define K_XDD_XD -7.33
#define K_XDD_TH -16.05
#define K_XDD_TD 0.464
#define K_XDD_FC 0.406
#define K_TDD_XD 9.88
#define K_TDD_TH 34.88
#define K_TDD_TD -1.01
#define K_TDD_FC -0.547
#define MOMENT_ARM 0.05 // [m]
#else
#define FORCE_NOISE 0.2 // [(m/ss)/N], 1N出すことによる加速度センサに乗るだろう標準偏差
#endif

void kalman(float o, float a, float f)
{
    // gyro
    float p_th, p_bi, p_sxx, p_syy, p_sxy;
    p_th=th, p_bi=bi, p_sxx=sxx, p_syy=syy, p_sxy=sxy;
    th = p_th + (o-p_bi)*dt;
    sxy = p_sxy - p_syy*dt;
    sxx = p_sxx + (alpha*o*o+p_syy)*dt*dt-2*p_sxy*dt;
    printf("%f #th\n", th);

    // acce
#ifdef PRECISE_MODE
    float xdd = K_XDD_XD*xd+K_XDD_TH*th+K_XDD_TD*o+K_XDD_FC*f;
    float tdd = K_TDD_XD*xd+K_TDD_TH*th+K_TDD_TD*o+K_TDD_FC*f;
    a -= (xdd + MOMENT_ARM * tdd);
#else
    q += FORCE_NOISE*f*f; // 動いている時は加速度センサの信用をなくす
#endif

    p_th=th, p_bi=bi, p_sxx=sxx, p_syy=syy, p_sxy=sxy;
    float k1 = g/(g*g*p_sxx+q), k2 = k1*g;
    //    printf("%f %f %f #z\n", a-g*p_th, a, g*p_th);
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

    if (t < 400) {bi += o;}
    else if (t == 400) {bi /= 400; Serial.println(bi,0);}
    else {
        //    th += (o - bi)*dt;
        //    if (t % 100 == 0){Serial.print(o*10000); Serial.print(bi*10000); Serial.print("\n");}
        //    if (t % 100 == 0){Serial.print(th); Serial.print("\n");}
        //    if (t % 100 == 0){Serial.print(a); Serial.print("\n");}
        kalman(o, a, f);
        if (t % 200 == 0){Serial.print(th); Serial.print("\n");}
        if {t % 200 == 0}{Serial.print(bi); Serial.print("\n");}
    }
    t++;
}

void setup(){
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write((uint8_t)0x6B);  // PWR_MGMT_1 register
    Wire.write((uint8_t)0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission();
    Serial.begin(9600);
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
