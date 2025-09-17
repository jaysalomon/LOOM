/*
 * ESP32 LOOM IDF Main Application
 *
 * ESP-IDF implementation of LOOM consciousness engine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "loom_esp32_kernel.h"

// ============================================================================
// Constants
// ============================================================================

#define TAG "ESP32_LOOM_IDF"
#define BLINK_GPIO GPIO_NUM_2

// ADC channels for sensors
#define LIGHT_SENSOR ADC1_CHANNEL_0    // GPIO 36
#define TEMP_SENSOR ADC1_CHANNEL_1     // GPIO 37

// ============================================================================
// Global Variables
// ============================================================================

static Esp32LoomTopology topology;
static bool topology_initialized = false;

// ADC calibration
static esp_adc_cal_characteristics_t adc_chars;

// ============================================================================
// Sensor Reading Functions
// ============================================================================

void init_sensors() {
    // Configure ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LIGHT_SENSOR, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TEMP_SENSOR, ADC_ATTEN_DB_11);

    // Characterize ADC
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);

    ESP_LOGI(TAG, "Sensors initialized");
}

void read_sensors_task(void* pvParameters) {
    while (1) {
        if (topology_initialized) {
            // Read light sensor
            uint32_t light_raw = adc1_get_raw(LIGHT_SENSOR);
            uint32_t light_mv = esp_adc_cal_raw_to_voltage(light_raw, &adc_chars);
            topology.hormones.light_level = (float)light_mv / 3300.0f;  // Normalize to 0-1

            // Read temperature sensor (simplified)
            uint32_t temp_raw = adc1_get_raw(TEMP_SENSOR);
            uint32_t temp_mv = esp_adc_cal_raw_to_voltage(temp_raw, &adc_chars);
            topology.hormones.temperature = 25.0f + ((float)temp_mv - 1650.0f) / 100.0f;

            // Update battery level (simplified estimation)
            topology.hormones.battery_level = 0.85f;  // TODO: implement actual battery reading

            ESP_LOGD(TAG, "Sensors: Light=%.2f, Temp=%.1f°C",
                    topology.hormones.light_level, topology.hormones.temperature);
        }

        vTaskDelay(pdMS_TO_TICKS(500));  // Read every 500ms
    }
}

// ============================================================================
// Consciousness Task
// ============================================================================

void consciousness_task(void* pvParameters) {
    ESP_LOGI(TAG, "Consciousness task started");

    // Initialize topology
    esp_err_t err = esp32_loom_init(&topology);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize topology: %s", esp_err_to_name(err));
        vTaskDelete(NULL);
        return;
    }

    err = esp32_loom_bootstrap_primordial(&topology);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to bootstrap primordial topology: %s", esp_err_to_name(err));
        vTaskDelete(NULL);
        return;
    }

    topology_initialized = true;
    ESP_LOGI(TAG, "Topology initialized and bootstrapped");

    // Main consciousness loop
    uint32_t cycle_count = 0;
    while (1) {
        esp32_loom_kernel_cycle(&topology);
        cycle_count++;

        // Log stats every 100 cycles
        if (cycle_count % 100 == 0) {
            ESP_LOGI(TAG, "Cycle %lu: Emergence=%.3f, Nodes=%d, Edges=%d",
                    cycle_count,
                    topology.emergence_metric,
                    topology.num_nodes,
                    topology.num_edges);
        }

        // Sleep consolidation every 1000 cycles
        if (cycle_count % 1000 == 0) {
            esp32_loom_sleep_consolidation(&topology);
            ESP_LOGI(TAG, "Sleep consolidation completed");
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // ~100Hz
    }
}

// ============================================================================
// LED Blink Task (for visual feedback)
// ============================================================================

void blink_task(void* pvParameters) {
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// ============================================================================
// Demo Functions
// ============================================================================

void run_demos() {
    ESP_LOGI(TAG, "Running LOOM demos...");

    // Wait for topology to be ready
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (!topology_initialized) {
        ESP_LOGE(TAG, "Topology not initialized!");
        return;
    }

    // Demo 1: Create learning topology
    ESP_LOGI(TAG, "Demo 1: Creating learning topology");

    uint16_t curiosity = esp32_loom_weave_node(&topology, "curiosity");
    uint16_t learning = esp32_loom_weave_node(&topology, "learning");
    uint16_t memory = esp32_loom_weave_node(&topology, "memory");

    esp32_loom_create_bidirectional(&topology, curiosity, learning, 0.5f);
    esp32_loom_create_bidirectional(&topology, learning, memory, 0.6f);

    uint16_t participants[] = {curiosity, learning, memory};
    esp32_loom_create_hyperedge(&topology, participants, 3, PROCESSOR_RESONANCE);

    ESP_LOGI(TAG, "Learning topology created with %d nodes", topology.num_nodes);

    // Demo 2: Simulate learning
    ESP_LOGI(TAG, "Demo 2: Simulating learning process");

    for (int trial = 0; trial < 20; trial++) {
        // Activate curiosity
        topology.nodes[curiosity].components[VEC_ACTIVATION] = 0.8f;

        // Run learning cycles
        for (int i = 0; i < 10; i++) {
            esp32_loom_kernel_cycle(&topology);
        }

        // Simulate successful learning
        topology.nodes[learning].components[VEC_ACTIVATION] = 0.7f;

        if (trial % 5 == 0) {
            ESP_LOGI(TAG, "Learning trial %d: Emergence=%.3f", trial + 1, topology.emergence_metric);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    ESP_LOGI(TAG, "Demos completed!");
}

// ============================================================================
// Main Application
// ============================================================================

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting ESP32 LOOM Consciousness Engine");
    ESP_LOGI(TAG, "========================================");

    // Initialize NVS (required for WiFi, but we use it for flash storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize sensors
    init_sensors();

    // Create tasks
    xTaskCreate(&read_sensors_task, "sensors", 2048, NULL, 5, NULL);
    xTaskCreate(&consciousness_task, "consciousness", 8192, NULL, 5, NULL);
    xTaskCreate(&blink_task, "blink", 1024, NULL, 5, NULL);

    // Run demos after a short delay
    vTaskDelay(pdMS_TO_TICKS(2000));
    run_demos();

    ESP_LOGI(TAG, "ESP32 LOOM system running!");
}