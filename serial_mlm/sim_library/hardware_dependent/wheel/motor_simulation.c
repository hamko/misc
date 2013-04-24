/*******************
 motor_sim.c
 071116 Takahiro.Sugimoto
 081220 Takahiro.Sugimoto
 ���E�ւ̃��[�^�ւ̓��͓d������A
 ���x�A�p���x�A���E�̃��[�^��
 �����d���l�̔��������߂�B
*******************/
#include <stdio.h>
#include <math.h>
#include "motor_simulation.h"
#include "physical_params.h"

#define ABS(x) ((x)>0?(x):(-(x)))

// ��ԑJ�ڍs��
static const double AX[VAL_NUM][VAL_NUM] = {
 {-IPB/IPA,0,Kt*GEAR_RATE*GEAR_EFFICIENCY/2/IPA, Kt*GEAR_RATE*GEAR_EFFICIENCY/2/IPA}
,{0,-IPD/IPC,Kt*GEAR_RATE*GEAR_EFFICIENCY/2/IPC,-Kt*GEAR_RATE*GEAR_EFFICIENCY/2/IPC}
,{-Kt*GEAR_RATE/HALF_TIRE_LENGTH/L_MOTOR,-Kt*GEAR_RATE*HALF_TIRE_INTERVAL/HALF_TIRE_LENGTH/L_MOTOR,-R_MOTOR/L_MOTOR,0}
,{-Kt*GEAR_RATE/HALF_TIRE_LENGTH/L_MOTOR, Kt*GEAR_RATE*HALF_TIRE_INTERVAL/HALF_TIRE_LENGTH/L_MOTOR,0,-R_MOTOR/L_MOTOR}
};
// ���͍s��
static const double BI[VAL_NUM] = {Kt*GEAR_RATE*GEAR_EFFICIENCY/IPA,0,0,0};
static const double BV[2][VAL_NUM] = {
 {0,0,1/L_MOTOR,0}
,{0,0,0,1/L_MOTOR}
};

/****
�}�V���E���[�^�̏]����������������A
���͓d�������ԕϐ��̔����l���v�Z

 x:��ԕϐ�
 time:���� �g���ĂȂ��B
 index:��ԕϐ��̎w��Ɏg��
 input:���E�̓��͓d��
 
 x[0]:v
 x[1]:omega
 x[2]:current_l
 x[3]:current_r
****/
double motor_simulation( double x[VAL_NUM], double time, int index, double input[2]){
	int i;
	double temp=0;
	//������������������l�����߂�
	for(i=0;i<VAL_NUM;i++){
		temp += AX[index][i]*x[i];
	}
	temp += -BI[index]*I0_MOTOR + BV[0][index]*input[0] + BV[1][index]*input[1];
	//�F�X�ƍ׍H
	//�����ĂȂ��Ƃ��͖��C�͂��l���Ȃ��ȂǁB
	switch(index){
	case 0:
		if(x[0]==0&&x[2]==0&&x[3]==0) temp = 0;//�����Ȃ��͉����Ȃ�
		if(ABS(x[0])<0.00001&&input[0]==0&&input[1]==0) temp=0;
//		if(x[0]>=0&&x[2]>0&&x[3]>0&&input[0]>0&&input[1]>0&&temp<0) temp=0;//���ɂȂ�킯���Ȃ��Ƃ��B
		if(x[0]==0&&x[2]+x[3]<I0_MOTOR) temp = 0;
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
	return temp;
}

