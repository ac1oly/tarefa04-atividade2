#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define LEDG   11  
#define LEDB   12  
#define LEDR   13  
#define BUTTON  5   
#define TEMPO_MS 3000  // Tempo entre as mudanças de estado (1 segundo)

bool led_active = false;
int cont = 0;

// Callback para alternar os LEDs
int64_t led_control_callback(alarm_id_t id, void *user_data) {
    cont++;

    switch (cont % 4) {
        case 0:
            gpio_put(LEDG, true);
            gpio_put(LEDB, true);
            gpio_put(LEDR, true);
            add_alarm_in_ms(TEMPO_MS, led_control_callback, NULL, false);
            break;
        case 1:
            gpio_put(LEDG, false);
            gpio_put(LEDB, true);
            gpio_put(LEDR, true);
            add_alarm_in_ms(TEMPO_MS, led_control_callback, NULL, false);
            break;
        case 2:
            gpio_put(LEDG, false);
            gpio_put(LEDB, false);
            gpio_put(LEDR, true);
            add_alarm_in_ms(TEMPO_MS, led_control_callback, NULL, false);
            break;
        case 3:
            gpio_put(LEDG, false);
            gpio_put(LEDB, false);
            gpio_put(LEDR, false);
            led_active = false; // Permite nova ativação pelo botão
            break;
    }

    return 0; // Indica que o alarme não deve se repetir automaticamente
}

int main() {
    stdio_init_all();

    gpio_init(LEDB);
    gpio_init(LEDR);
    gpio_init(LEDG);
    gpio_set_dir(LEDB, GPIO_OUT);
    gpio_set_dir(LEDR, GPIO_OUT);
    gpio_set_dir(LEDG, GPIO_OUT);    

    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    while (true) {
        if (gpio_get(BUTTON) == 0 && !led_active) {
            sleep_ms(50);
            if (gpio_get(BUTTON) == 0) {
                gpio_put(LEDG, true);
                gpio_put(LEDB, true);
                gpio_put(LEDR, true);

                led_active = true;
                cont = 0; // Reinicia a contagem
                add_alarm_in_ms(TEMPO_MS, led_control_callback, NULL, false); // Inicia o ciclo de LEDs
            }
        }
        sleep_ms(10);
    }
    return 0;
}
