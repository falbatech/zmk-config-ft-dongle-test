# FT Dongle Test - włączamy blink_test.c gdy shield wybrany
if(CONFIG_SHIELD_FT_DONGLE_TEST)
    zephyr_library_named(ft_dongle_test)
    zephyr_library_sources(src/blink_test.c)
endif()
