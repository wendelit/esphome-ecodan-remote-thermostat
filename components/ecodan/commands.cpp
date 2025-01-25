#include "ecodan.h"

#include "esphome.h"

#include <functional>
#include <queue>

namespace esphome {
namespace ecodan 
{    
    void EcodanHeatpump::set_room_thermostat_target_temp(float temp, ClimateRoomIdentifier room) {
        if (temp != NAN) {
            auto room_index = static_cast<uint8_t>(room);
            status.TargetRoomTemperatures[room_index] = temp;

            Message cmd{MsgType::THERMOSTAT_SET, SetType::THERMOSTAT_TEMPERATURE_SETTINGS};
            for (auto i = 0; i < MAX_REMOTE_THERMOSTATS; i++) {
                if (i + i == room_index) {
                    auto roundedTemp = round_nearest_half(status.TargetRoomTemperatures[i]);
                    cmd.set_float8_v3(roundedTemp, 1+i);
                }
                else {
                    cmd[1+i] = 0xff;
                }
            }
            schedule_cmd(cmd);
        }
    }

    bool EcodanHeatpump::schedule_cmd(Message& cmd)
    {   
        cmdQueue.emplace(std::move(cmd));
        return dispatch_next_cmd();
    }

    #define MAX_STATUS_CMD_SIZE 2
    Message statusCmdQueue[MAX_STATUS_CMD_SIZE] = {
        // initial request
        //Message{MsgType::THERMOSTAT_INITIAL_GET, GetType::THERMOSTAT_STATE_A},
        Message{MsgType::THERMOSTAT_INITIAL_GET, std::array<char, PAYLOAD_SIZE> { static_cast<char>(GetType::THERMOSTAT_STATE_A), 0x03, 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
        Message{MsgType::THERMOSTAT_GET, GetType::THERMOSTAT_STATE_B} 
    };

    bool EcodanHeatpump::dispatch_next_status_cmd()
    {
        if (proxy_available())
            return true;
        
        auto static cmdIndex = 0;
        Message& cmd = statusCmdQueue[cmdIndex];
        cmdIndex = (cmdIndex + 1) % MAX_STATUS_CMD_SIZE;

        if (cmd.type() == MsgType::THERMOSTAT_GET) {
            // fill all current thermostat values
            for (auto i = 0; i < MAX_REMOTE_THERMOSTATS; i++) {
                if (status.CurrentRoomTemperatures[i] != 0xff) {
                    auto roundedTemp = round_nearest_half(status.CurrentRoomTemperatures[i]);
                    cmd.set_float8_v3(roundedTemp, 1+i);
                }
                else {
                    cmd[1+i] = 0xff;
                }
            }
            cmd[9] = 3;
            cmd[10] = 0xff;
        }

        if (!serial_tx(uart_, cmd))
        {
            ESP_LOGI(TAG, "Unable to dispatch status update request, flushing queued requests...");
            cmdIndex = 0;
            connected = false;
            return false;
        }

        return true;
    }

    bool EcodanHeatpump::dispatch_next_cmd()
    {
        if (cmdQueue.empty())
        {
            return true;
        }
        
        //ESP_LOGI(TAG, msg.debug_dump_packet().c_str());

        if (!serial_tx(uart_, cmdQueue.front()))
        {
            ESP_LOGI(TAG, "Unable to dispatch status update request, flushing queued requests...");
            connected = false;
            return false;
        }

        return true;
    }

    bool EcodanHeatpump::begin_connect()
    {
        Message cmd{MsgType::CONNECT_CMD};
        uint8_t payload[2] = {0xCA, 0x01 };
        cmd.write_payload(payload, sizeof(payload));

        ESP_LOGI(TAG, "Attempt to tx CONNECT_CMD!");
        if (!serial_tx(uart_, cmd))
        {
            ESP_LOGI(TAG, "Failed to tx CONNECT_CMD!");
            return false;
        }

        return true;
    }

}}
