#define DT_DRV_COMPAT zmk_layer_underglow

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/rgb_underglow.h>

LOG_MODULE_REGISTER(layer_underglow, CONFIG_ZMK_LOG_LEVEL);

#if DT_INST_NODE_HAS_PROP(0, layer_entries)
static const uint32_t layer_entries[] = DT_INST_PROP(0, layer_entries);
#define LAYER_ENTRIES_LEN ARRAY_SIZE(layer_entries)
#else
static const uint32_t layer_entries[] = {};
#define LAYER_ENTRIES_LEN 0
#endif

static void apply_state(uint32_t effect, uint32_t h, uint32_t s, uint32_t b) {
    zmk_rgb_underglow_set_hsb((struct zmk_led_hsb){.h = h, .s = s, .b = b});
    zmk_rgb_underglow_select_effect(effect);
}

static void apply_default(void) {
    apply_state(CONFIG_ZMK_RGB_UNDERGLOW_EFF_START,
                CONFIG_ZMK_RGB_UNDERGLOW_HUE_START,
                CONFIG_ZMK_RGB_UNDERGLOW_SAT_START,
                CONFIG_ZMK_RGB_UNDERGLOW_BRT_START);
}

static int layer_underglow_listener(const zmk_event_t *eh) {
    for (int i = LAYER_ENTRIES_LEN - 5; i >= 0; i -= 5) {
        if (zmk_keymap_layer_active(layer_entries[i])) {
            apply_state(layer_entries[i + 1], layer_entries[i + 2],
                        layer_entries[i + 3], layer_entries[i + 4]);
            return ZMK_EV_EVENT_BUBBLE;
        }
    }

    apply_default();
    return ZMK_EV_EVENT_BUBBLE;
}

static int layer_underglow_init(void) {
    apply_default();
    return 0;
}
SYS_INIT(layer_underglow_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

ZMK_LISTENER(layer_underglow, layer_underglow_listener);
ZMK_SUBSCRIPTION(layer_underglow, zmk_layer_state_changed);
