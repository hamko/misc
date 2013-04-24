//#include "3052.h"
#include "device_manager.h"
//#include "util.h"

hal_encoder link_null_as_encoder(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_encoder(&(new_device->encoder));
  
  return &(new_device -> encoder);
}

hal_led link_null_as_led(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_led(&(new_device -> led));

  return &(new_device -> led);
}

hal_linesensor link_null_as_linesensor(void)
{ 
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_linesensor(&(new_device -> linesensor));

  return &(new_device -> linesensor);
}

hal_port link_null_as_port(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_port(&(new_device -> port));

  return &(new_device -> port);
}

hal_pscontroller link_null_as_pscontroller(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_pscontroller(&(new_device -> pscontroller));

  return &(new_device -> pscontroller);
}

hal_switch link_false_as_switch(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_switch(&(new_device -> _switch));
  new_device -> _switch.status = TRUE;

  return &(new_device -> _switch);
}

hal_switch link_true_as_switch(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_switch(&(new_device -> _switch));
  new_device -> _switch.status = FALSE;

  return &(new_device -> _switch);
}


hal_2008ir link_null_as_2008ir(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_2008ir(&(new_device -> _2008ir));

  return &(new_device -> _2008ir);
}

hal_adc link_null_as_adc(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_adc(&(new_device->adc));

  return &(new_device->adc);
}

hal_servo link_null_as_servo(void)
{ 
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  release_servo(&(new_device -> servo));

  return &(new_device -> servo);
}

hal_motor link_null_as_motor(void)
{
  hal_device *new_device;

  new_device = DM_add_device(NULL, NULL);
  clear_motor(&(new_device -> motor));

  return &(new_device -> motor);
}

