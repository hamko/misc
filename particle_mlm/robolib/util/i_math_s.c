

#include "install_option.h"
#ifdef __INSTALL_I_MATH_S__


#include "i_math.h"
#include "r_stdint.h"
#include "i_math_s.h"

int32_t i_power(int32_t x,int16_t n,int32_t unit){
    int32_t result=1*unit;
    int16_t i=0;
    if(n<0){return 0;}
    for (i=0;i<n;i++){ result=multiply_divide(x,result,unit); }
    return result;
}

int32_t i_abs(int32_t number){
    if(number<0){return (int32_t)(-1)*number;}
    else{return number;}
}


map_t get_relative_pos(p_map_t zero,map_t target){
    int32_t x,y,theta;
    position_t pos;
     
    pos=get_map_pos(zero); 
        
    theta = target.theta - pos.theta;
    x = multiply_divide(target.x - pos.x,i_cos(pos.theta),UNIT_MM) + multiply_divide(target.y - pos.y,i_sin(pos.theta),UNIT_MM);
    y = multiply_divide(-(target.x - pos.x),i_sin(pos.theta),UNIT_MM) + multiply_divide(target.y - pos.y,i_cos(pos.theta),UNIT_MM);
    
    return (map_t){x,y,theta};
}


int32_t get_relative_distance(map_t zero,map_t target){
    
    return (int32_t )i_sqrt(i_power(target.x-zero.x,2,UNIT_MM)+i_power(target.y-zero.y,2,UNIT_MM));
}
#endif
