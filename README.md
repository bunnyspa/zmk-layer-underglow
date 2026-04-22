# zmk-layer-underglow

A [ZMK](https://zmk.dev) module that changes RGB underglow color and effect based on which keymap layer is active.

**On split keyboards, runs on the central half only.**

## `config/west.yml`

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    # --- copy from here ---
    - name: bunnyspa
      url-base: https://github.com/bunnyspa
    # --- to here ---
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    # --- copy from here ---
    - name: zmk-layer-underglow
      remote: bunnyspa
      revision: main
    # --- to here ---
  self:
    path: config
```

## `<keyboard>.conf`

```ini
CONFIG_ZMK_RGB_UNDERGLOW=y
CONFIG_ZMK_LAYER_UNDERGLOW=y

# Default state when no layer-entries match
CONFIG_ZMK_RGB_UNDERGLOW_EFF_START=0
CONFIG_ZMK_RGB_UNDERGLOW_HUE_START=0
CONFIG_ZMK_RGB_UNDERGLOW_SAT_START=0
CONFIG_ZMK_RGB_UNDERGLOW_BRT_START=50
```

## Usage

Add the following to your `<keyboard>.overlay` or `<keyboard>.dtsi` (central half only if split).

A state is `(effect H S B)`. For effect indices see [ZMK lighting config](https://zmk.dev/docs/config/lighting). For spectrum and swirl, H/S/B are ignored.

```c
/ {
    layer_underglow: layer_underglow {
        compatible = "zmk,layer-underglow";
        layer-entries = <
            // layer  eff    H    S   B
               1      0    240  100  50  // solid blue  (lower priority)
               2      1      0  100  50  // breathe red (higher priority)
        >;
    };
};
```

**`layer-entries`** — flat array of `(layer effect H S B)` tuples. Entries are checked from last to first; the first match wins, so list layers in ascending priority order. When no entry matches, applies `CONFIG_ZMK_RGB_UNDERGLOW_*_START`.

## How it works

The module subscribes to `zmk_layer_state_changed` events. On each event it checks `layer-entries` for an active layer and applies the matching state. If no entry matches, it applies `CONFIG_ZMK_RGB_UNDERGLOW_*_START`. On split keyboards the listener only compiles for the central half (`ZMK_SPLIT_ROLE_CENTRAL`), so the peripheral half is unaffected.
