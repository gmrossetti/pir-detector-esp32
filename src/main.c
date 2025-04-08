/*
    Author: Gian Marco Rossetti
    Description: 
        Il progetto implementa un sistema di controllo di un LED tramite un sensore di movimento PIR utilizzando Zephyr RTOS. 
        Quando il sensore PIR rileva un movimento, un callback aggiorna un timer e accende il LED. Il LED rimane acceso 
        per 15 secondi e poi si spegne automaticamente, salvo nuovi movimenti rilevati. Il progetto gestisce la configurazione 
        del sensore PIR e del LED tramite GPIO e utilizza un interrupt per rilevare il movimento.
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define LIGHT_TIMEOUT_MS 15000

static struct gpio_callback pir_sensor_cb_data;

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(DT_NODELABEL(blinking_led), gpios);

static const struct gpio_dt_spec pir_sensor = 
    GPIO_DT_SPEC_GET(DT_NODELABEL(pir_sensor), gpios);

volatile int64_t timeout = 0;

/** 
 * Funzione di callback che viene chiamata quando viene rilevato un movimento.
 */
void movement_detected(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    timeout = k_uptime_get() + LIGHT_TIMEOUT_MS;
}

/** 
 * Funzione per configurare il LED. Restituisce 0 in caso di successo, un codice di errore altrimenti.
 */
static int configure_led(void) {
    if (!device_is_ready(led.port)) {
        return -1;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    return ret;
}

/** 
 * Funzione per configurare il sensore PIR. Restituisce 0 in caso di successo, un codice di errore altrimenti.
 */
static int configure_pir_sensor(void) {
    if (!device_is_ready(pir_sensor.port)) {
        return -1;
    }

    int ret = gpio_pin_configure_dt(&pir_sensor, GPIO_INPUT | GPIO_PULL_UP);
    if (ret != 0) {
        return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&pir_sensor, GPIO_INT_EDGE_TO_ACTIVE);
    return ret;
}

/** 
 * Funzione per configurare il callback per il sensore PIR. 
 */
static void configure_pir_callback(void) {
    gpio_init_callback(&pir_sensor_cb_data, movement_detected, BIT(pir_sensor.pin));
    gpio_add_callback(pir_sensor.port, &pir_sensor_cb_data);
}

/** 
 * Funzione principale del programma.
 */
int main(void) {
    int ret;

    // Configurazione LED
    ret = configure_led();
    if (ret != 0) {
        return ret;
    }

    // Configurazione sensore PIR
    ret = configure_pir_sensor();
    if (ret != 0) {
        return ret;
    }

    // Configurazione callback del sensore PIR
    configure_pir_callback();

    // Loop principale
    while (true) {
        // Accendi il LED se il tempo limite non è passato
        if (k_uptime_get() < timeout) {
            gpio_pin_set_dt(&led, 1);
        } else {
            gpio_pin_set_dt(&led, 0);
        }
    }

    return 0;
}
