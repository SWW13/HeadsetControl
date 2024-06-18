#include "../device.h"
#include "../utility.h"
#include "logitech.h"

#include <math.h>
#include <string.h>

#define MSG_SIZE 64

static struct device device_q810;

static const uint16_t PRODUCT_ID = 0x2069;

static int q810_lights(hid_device* device_handle, uint8_t on);

#define SET_LIGHTS   0x4b // 0 = off, 1 = on
#define SET_ANC      0x46 // 0 = off, 1 = on, 2 = tt (talk through)
#define SET_SIDETONE 0x5d // 0 = off, 1 = low, 2 = mid, 3 = high

#define IRQ_ANC     0x02 // 0 = off, 1 = on, 2 = tt (talk through)
#define IRQ_03      0x03 // 0x03 ???
#define IRQ_MIC     0x06 // 0 = off, 1 = on
#define IRQ_LIGHTS  0x07 // 0 = off, 1 = on
#define IRQ_BATTERY 0x08 // 0 .. 100 battery charge in percent (?)
#define IRQ_SOUND   0x10 // 0 = chat ... 0x10 = game: sound mix

void q810_init(struct device** device)
{
    device_q810.idVendor            = VENDOR_JBL;
    device_q810.idProductsSupported = &PRODUCT_ID;
    device_q810.numIdProducts       = 1;

    strncpy(device_q810.device_name, "JBL Quantum 810", sizeof(device_q810.device_name));

    device_q810.capabilities = B(CAP_LIGHTS); //  | B(CAP_BATTERY_STATUS) | B(CAP_INACTIVE_TIME);
    // device_q810.capability_details[CAP_SIDETONE]       = (struct capability_detail) { .usagepage = 0xff00, .usageid = 0x1, .interface = 3 };
    // device_q810.capability_details[CAP_BATTERY_STATUS] = (struct capability_detail) { .usagepage = 0xff43, .usageid = 0x202, .interface = 3 };
    // device_q810.capability_details[CAP_INACTIVE_TIME]  = (struct capability_detail) { .usagepage = 0xff43, .usageid = 0x0202 };
    device_q810.capability_details[CAP_LIGHTS] = (struct capability_detail) { .usagepage = 0xff13, .usageid = 0x1, .interface = 5 };
    // device_q810.request_battery                        = &q810_request_battery;
    // device_q810.send_sidetone                          = &q810_send_sidetone;
    // device_q810.send_inactive_time                     = &q810_send_inactive_time;
    device_q810.switch_lights = &q810_lights;

    *device = &device_q810;
}

static int q810_lights(hid_device* device_handle, uint8_t on)
{
    unsigned char data[2] = { SET_LIGHTS, on };
    return hid_write(device_handle, data, 2);
}
