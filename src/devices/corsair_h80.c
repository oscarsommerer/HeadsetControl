#include "../device.h"
#include "../utility.h"

#include <hidapi.h>
#include <string.h>

static struct device device_h80;

enum void_wireless_battery_flags {
    VOID_BATTERY_MICUP = 128
};

#define ID_CORSAIR_H80_MAX                   0x0a97

static const uint16_t PRODUCT_IDS[] = {
    ID_CORSAIR_H80_MAX
};
static int h80_lights(hid_device* device_handle, uint8_t on);

void h80_init(struct device** device)
{
    device_h80.idVendor            = VENDOR_CORSAIR;
    device_h80.idProductsSupported = PRODUCT_IDS;
    device_h80.numIdProducts       = sizeof(PRODUCT_IDS) / sizeof(PRODUCT_IDS[0]);

    strncpy(device_h80.device_name, "Corsair Headset Device", sizeof(device_h80.device_name));

    device_h80.capabilities                               = B(CAP_LIGHTS);
    device_h80.capability_details[CAP_LIGHTS]             = (struct capability_detail) { .usagepage = 0xff42, .usageid = 0x01, .interface = 3 };

    device_h80.switch_lights     = &h80_lights;

    *device = &device_h80;
}

static int h80_lights(hid_device* device_handle, uint8_t on)
{
#define MSG_SIZE 64
    unsigned char data[3] = { 0x01, on ? 0x00 : 0x01, 0x01 };
    return hid_write(device_handle, data, 3);
}
