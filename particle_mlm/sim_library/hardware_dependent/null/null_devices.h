#ifndef _NULL_DEVICES_H_
#define _NULL_DEVICES_H_

hal_encoder link_null_as_encoder(void);
hal_led link_null_as_led(void);
hal_linesensor link_null_as_linesensor(void);
hal_port link_null_as_port(void);
hal_pscontroller link_null_as_pscontroller(void);
hal_switch link_true_as_switch(void);
hal_switch link_false_as_switch(void);
hal_2008ir link_null_as_2008ir(void);
hal_adc link_null_as_adc(void);
hal_servo link_null_as_servo(void);
hal_motor link_null_as_motor(void);

#endif //_NULL_DEVICES_H_
