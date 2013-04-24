/***********************
 physical_params.h
 T.Sugimoto
 マシン・モータの物理定数
***********************/
#ifndef PHYSICAL_PARAMS_H
#define PHYSICAL_PARAMS_H

#define PI 3.14159265
//machine and motor params
#define VATTERY_V  24.0f //V 基準電圧 割と適当。約2倍？
#define R_MOTOR  0.317			//d V/A 
//#define L_MOTOR  0.0000823		//d H original
#define L_MOTOR  0.0020023		//d H original
#define I0_MOTOR 0.137			//d A
//#define Kb       0.0302			//d Vsec/rad , Nm/A
#define Kb       0.0202			//d Vsec/rad , Nm/A
#define Kt Kb					//d Vsec/rad , Nm/A
//#define J_MOTOR  0.000000138	//d Kgm^2
#define J_MOTOR  0.0000022	//d Kgm^2
//#define B_MOTOR  0.0000478		//d Nmsec/rad 10^-3/3.33/2/pi = 4.77942772 × 10-5
#define B_MOTOR  0.0000178		//d Nmsec/rad 10^-3/3.33/2/pi = 4.77942772 × 10-5
//#define J_GEAR   0.000000000001	//d Kgm^2
#define J_GEAR   0.0000000004	//d Kgm^2
#define B_GEAR   0.00000000478	//c Nmsec/rad
#define GEAR_RATE 15.0			//d --
#define GEAR_EFFICIENCY 0.7		//d --
#define MACHINE_WEIGHT (2.5)		//d Kg
#define HALF_TIRE_INTERVAL (-0.20975)	//d m
#define HALF_TIRE_LENGTH 0.032	//d m
//#define J_MACHINE_OMEGA MACHINE_WEIGHT*HALF_TIRE_INTERVAL*HALF_TIRE_INTERVAL/2	//c Kgm^2 円柱モデル
//#define J_MACHINE_OMEGA MACHINE_WEIGHT*HALF_TIRE_INTERVAL*HALF_TIRE_INTERVAL/2	//c Kgm^2 円柱モデル
#define J_MACHINE_OMEGA MACHINE_WEIGHT*HALF_TIRE_INTERVAL*HALF_TIRE_INTERVAL/2.0	//c Kgm^2 円柱モデル

//#define J_MACHINE_V 0.009		//? Kgm^2
//#define B_MACHINE 0.009			//? Nmsec/rad
#define J_MACHINE_V 0.045		//? Kgm^2
#define B_MACHINE 0.001			//? Nmsec/rad

//internal? params 中間定数
#define IPA ((J_MACHINE_OMEGA + J_MOTOR*GEAR_RATE*GEAR_RATE*GEAR_EFFICIENCY + J_GEAR)/HALF_TIRE_LENGTH + HALF_TIRE_LENGTH*MACHINE_WEIGHT/2)
#define IPB ((B_MACHINE + B_MOTOR*GEAR_RATE*GEAR_RATE*GEAR_EFFICIENCY + B_GEAR)/HALF_TIRE_LENGTH)
#define IPC ((J_MACHINE_OMEGA + J_MOTOR*GEAR_RATE*GEAR_RATE*GEAR_EFFICIENCY + J_GEAR)*HALF_TIRE_INTERVAL/HALF_TIRE_LENGTH + HALF_TIRE_LENGTH*J_MACHINE_V/2/HALF_TIRE_INTERVAL)
#define IPD ((B_MACHINE + B_MOTOR*GEAR_RATE*GEAR_RATE*GEAR_EFFICIENCY + B_GEAR)*HALF_TIRE_INTERVAL/HALF_TIRE_LENGTH)



#define SIM_DT 0.0001
#define STEP_DT 0.001

#define STEP_LEFT 0.0748
#define STEP_RIGHT 0.0748

#define ENCODER_WIDTH 400

#endif //PHYSICAL_PARAMS_H
