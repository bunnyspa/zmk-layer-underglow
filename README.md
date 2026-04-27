# zmk-layer-underglow

A [ZMK](https://zmk.dev) module that changes RGB underglow color and effect based on which keymap layer is active.

**On split keyboards, runs on the central half only.**

## Getting Started

### `config/west.yml`

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

### `<keyboard>.conf`

```ini
CONFIG_ZMK_RGB_UNDERGLOW=y
CONFIG_ZMK_LAYER_UNDERGLOW=y

# Default state when no layer-entries match
CONFIG_ZMK_RGB_UNDERGLOW_EFF_START=0
CONFIG_ZMK_RGB_UNDERGLOW_HUE_START=0
CONFIG_ZMK_RGB_UNDERGLOW_SAT_START=0
CONFIG_ZMK_RGB_UNDERGLOW_BRT_START=50
```

### `<keyboard>.overlay` or `<keyboard>.dtsi`

> [!NOTE]
> **Split keyboards:** Place this in the central half's overlay only (e.g. `<keyboard>_left.overlay`), not in a shared `.overlay` or `.dtsi`.

```c
#include <zmk/layer_underglow.dtsi>

&layer_underglow {
    layer-entries = <
        // layer  eff    H    S   B
           1      0    240  100  50
           2      1      0  100  50
    >;
};
```

## Parameters

**`layer-entries`** — flat array of `(layer effect H S B)` tuples.

- Entries are checked from last to first; the first match wins — list layers in ascending priority order.
- When no entry matches, applies `CONFIG_ZMK_RGB_UNDERGLOW_*_START`.
- For effect indices see [ZMK lighting config](https://zmk.dev/docs/config/lighting). For spectrum and swirl effects, H/S/B are ignored.

