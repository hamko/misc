#include"device_manager.h"
#include"util.h"
#include"world.h"
#include<math.h>

static void presync0(hal_device *device);
static void presync1(hal_device *device);

static hal_linesensor ls0 = NULL;
static hal_linesensor ls1 = NULL;

hal_linesensor link_as_linesensor0(void)
{
  hal_device *new_device;

  dprintf("\tSetting linesensor0...");

  new_device = DM_add_device(presync0, NULL);
  clear_linesensor(&(new_device->linesensor));

  dprintf("Done.\n");

  ls0 = &(new_device->linesensor);
  return ls0;
}

hal_linesensor link_as_linesensor1(void)
{
  hal_device *new_device;

  dprintf("\tSetting linesensor1...");

  new_device = DM_add_device(presync1, NULL);
  clear_linesensor(&(new_device->linesensor));

  dprintf("Done.\n");

  ls1 = &(new_device->linesensor);
  return ls1;
}

#define ODTOLS0_X 200
#define ODTOLS0_Y -40
static void presync0(hal_device *device)
{
    UNUSED_VARIABLE(device);

    world_pos_t *wpos;
    wpos = get_world_pos();
    
    int i = 1;
    int bit;
    ls0->sensor = 0;
    for (bit = 0; bit < 8; i <<= 1, bit++) {
        float psx = wpos->x + ODTOLS0_X * cos(wpos->theta) - ((-35 + bit * 10) + ODTOLS0_Y) * sin(wpos->theta);
        float psy = wpos->y + ODTOLS0_X * sin(wpos->theta) + ((-35 + bit * 10) + ODTOLS0_Y) * cos(wpos->theta);

        psx += (float)1000000;
        psy += (float)1000000;
        int rpsx = (int)psx % 500;
        int rpsy = (int)psy % 500;
        if (!(rpsx > 15 && rpsx < 485) || !(rpsy > 15 && rpsy < 485)) {
            ls0->sensor += (unsigned char)i;
        }
    }
}

#define ODTOLS1_X 200
#define ODTOLS1_Y 40
static void presync1(hal_device *device)
{
    UNUSED_VARIABLE(device);

    world_pos_t *wpos;
    wpos = get_world_pos();
    
    int i = 1;
    int bit;
    ls1->sensor = 0;
    for (bit = 0; bit < 8; i <<= 1, bit++) {
        float psx = wpos->x + ODTOLS1_X * cos(wpos->theta) - ((-35 + bit * 10) + ODTOLS1_Y) * sin(wpos->theta);
        float psy = wpos->y + ODTOLS1_X * sin(wpos->theta) + ((-35 + bit * 10) + ODTOLS1_Y) * cos(wpos->theta);
        //printf("%d %f %f ", bit, psx, psy);
        psx += 1000000;
        psy += 1000000;
        int rpsx = (int)psx % 500;
        int rpsy = (int)psy % 500;
        //printf("%f %f %d %d\n", psx, psy, rpsx, rpsy);
        if (!(rpsx > 15 && rpsx < 485) || !(rpsy > 15 && rpsy < 485))
            ls1->sensor += i;
    }
    //printf("%d\n", ls1->sensor);
}
