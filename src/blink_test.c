/*
 * FT Dongle Test - blink BL backlight przez PWM
 *
 * Test fizycznego połączenia D5 (P0.24) -> BL pin GC9A01.
 * Backlight pulsuje:
 *   - 50% przez 1 sek (przyciemniony)
 *   - 100% przez 1 sek (pełna jasność)
 *   - 0% przez 1 sek (wyłączony)
 *   - 100% przez 1 sek (pełna jasność)
 *   ... (pętla)
 *
 * Jeśli widzisz pulsowanie - hardware działa, idziemy dalej do GC9A01 init.
 * Jeśli zawsze włączony - PWM nie działa, ale BL ma zasilanie (sprawdź pin).
 * Jeśli zawsze wyłączony - sprawdź lutowanie BL, VCC, GND.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ft_dongle_test, LOG_LEVEL_INF);

#define PWM_LEDS_NODE DT_NODELABEL(pwmleds)
#define BL_LED_IDX 0  /* pwm_led_0 = pierwsze dziecko pwmleds */

static int ft_dongle_blink_test(void)
{
    const struct device *led_dev = DEVICE_DT_GET(PWM_LEDS_NODE);

    if (!device_is_ready(led_dev)) {
        LOG_ERR("PWM LED device nie jest gotowe");
        return -ENODEV;
    }

    LOG_INF("FT Dongle Test: blink BL backlight, pulsuje 4-fazowo");

    while (1) {
        /* Faza 1: 50% jasności */
        led_set_brightness(led_dev, BL_LED_IDX, 50);
        k_msleep(1000);

        /* Faza 2: 100% jasności */
        led_set_brightness(led_dev, BL_LED_IDX, 100);
        k_msleep(1000);

        /* Faza 3: 0% (wyłączony) */
        led_set_brightness(led_dev, BL_LED_IDX, 0);
        k_msleep(1000);

        /* Faza 4: 100% */
        led_set_brightness(led_dev, BL_LED_IDX, 100);
        k_msleep(1000);
    }

    return 0;
}

/* Uruchamiamy test w wątku systemowym, niski priorytet */
SYS_INIT(ft_dongle_blink_test, APPLICATION, 90);
