# zmk-layer-underglow

A [ZMK](https://zmk.dev) module that changes RGB underglow color and effect based on which keymap layer is active. Configure a default state and any number of layer mappings in your device tree.

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
```

The following are overridden at boot and on each layer change by this module:

- `CONFIG_ZMK_RGB_UNDERGLOW_HUE_START`
- `CONFIG_ZMK_RGB_UNDERGLOW_SAT_START`
- `CONFIG_ZMK_RGB_UNDERGLOW_BRT_START`
- `CONFIG_ZMK_RGB_UNDERGLOW_EFF_START`

## Usage

Add the following to your `<keyboard>.overlay` or `<keyboard>.dtsi` (central half only if split).

A state is `(effect H S B)`. For effect indices see [ZMK lighting config](https://zmk.dev/docs/config/lighting). For spectrum and swirl, H/S/B are ignored.

```c
/ {
    layer_underglow: layer_underglow {
        compatible = "zmk,layer-underglow";
        default-state = <0 0 0 40>;     // effect H S B
        layer-entries = <
            // layer  eff    H    S   B
               1      0    240  100  40  // solid blue  (lower priority)
               2      1      0  100  40  // breathe red (higher priority)
        >;
    };
};
```

**`default-state`** — state shown when no `layer-entries` match.

**`layer-entries`** — flat array of `(layer effect H S B)` tuples. Entries are checked from last to first; the first match wins, so list layers in ascending priority order.

## How it works

The module subscribes to `zmk_layer_state_changed` events. On each event it checks `layer-entries` for an active layer and applies the matching state. If no entry matches, it falls back to `default-state`. On split keyboards the listener only compiles for the central half (`ZMK_SPLIT_ROLE_CENTRAL`), so the peripheral half is unaffected.
