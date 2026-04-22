if(CONFIG_ZMK_LAYER_UNDERGLOW AND ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
    target_sources(app PRIVATE src/layer_underglow.c)
endif()
