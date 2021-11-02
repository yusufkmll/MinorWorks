#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_task_wdt.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define CONFIG_WIFI_SSID "kolay_wifi"
#define CONFIG_WIFI_PASSWORD "kolay_sifre"

#define CONFIG_BROKER_URL "mqtt://broker.mqttdashboard.com"

#define PUBTOP "vehicle_to_interface"
#define SUBTOP "interface_to_vehicle"

#define QoS0 0
#define QoS1 1
#define QoS2 2
#define RETAIN_TRUE 1
#define RETAIN_FALSE 0

#define RX_WATCHDOG_DURATION_SECONDS 2

#define BAUD_RATE 115200

#define TXD_PIN_1 10 //s
#define RXD_PIN_1 9 //sd3

static const char *TAG = "Esp32 ";
static const int RX_BUF_SIZE = 255;
static const char *Mesaj = "denemeeeee XXX Large";

static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;

esp_mqtt_client_handle_t client;
TaskHandle_t rxTaskHandler;
bool oneShot = true;

char* uart_tx_buffer;

void uart_init_1(void) {
    const uart_config_t uart_config = 
    {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN_1, RXD_PIN_1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {

        if(oneShot == true)
        {
            esp_task_wdt_add(rxTaskHandler);
            oneShot = false;
        }
        if(oneShot == false)
        {
            esp_task_wdt_reset();
        }
        int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 10 / portTICK_RATE_MS);
        //ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
        if (rxBytes > 0) {

            //data[rxBytes] = 0;
            esp_mqtt_client_publish(client, PUBTOP, (char*)data, rxBytes, QoS0, RETAIN_FALSE);

            for(int i = 0; i < rxBytes; i++)
            {
                //printf("%d\t", data[i]);
            }
            printf("\n\n");
            //ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}


//msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
//MQTT events
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    client = event->client;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            esp_mqtt_client_subscribe(client, SUBTOP, QoS0);
            printf("\n buraya girdi");
            esp_mqtt_client_publish(client, PUBTOP,Mesaj, 0, QoS0, RETAIN_FALSE);
            break;
        case MQTT_EVENT_DISCONNECTED:
            break;
        case MQTT_EVENT_SUBSCRIBED:
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            break;
        case MQTT_EVENT_PUBLISHED:
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            uart_tx_buffer = event->data;
            uart_write_bytes(UART_NUM_1, event->data,event->data_len);
            break;
        case MQTT_EVENT_ERROR:
            break;
        default:
            break;
    }
    return ESP_OK;
}

//Initializes MQTT
//If broker reques authorization, decomment username and password
static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .event_handle = mqtt_event_handler,
        .disable_clean_session = false,
        //,.username = AUTH_USERNAME,
        //.password = AUTH_PASSWORD
        // .user_context = (void *)your_context
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

static void wifi_init(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    //ESP_LOGI(TAG, "start the WIFI SSID:[%s]", CONFIG_WIFI_SSID);
    ESP_ERROR_CHECK(esp_wifi_start());
    //ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void app_main()
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
    nvs_flash_init();
    esp_task_wdt_init(RX_WATCHDOG_DURATION_SECONDS, false);


    wifi_init();
    mqtt_app_start();
    uart_init_1();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, &rxTaskHandler);
   
}
