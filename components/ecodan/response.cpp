#include "ecodan.h"
#include <string>
#include <algorithm>

namespace esphome {
namespace ecodan 
{
    void EcodanHeatpump::handle_get_response(Message& res)
    {
        {
            switch (res.payload_type<GetType>())
            {
            case GetType::THERMOSTAT_STATE_RES:
                if (res.type() == MsgType::THERMOSTAT_INITIAL_GET_RES) {
                    status.Zone1SetTemperature = res.get_float8_v3(3);
                    status.Zone2SetTemperature = res.get_float8_v3(13);
                }
                else {
                    for (auto i = 0; i < MAX_REMOTE_THERMOSTATS; i++) {
                        status.TargetRoomTemperatures[i] = res[1+i] != 0xff ?
                            res.get_float8_v3(3) : 0xff;
                    }
                }
                break;
            default:
                if (static_cast<uint8_t>(res.payload_type<GetType>()) != 0)
                    ESP_LOGI(TAG, "Unknown response type received on serial port: %u", static_cast<uint8_t>(res.payload_type<GetType>()));
                break;
            }
        }
    }

    void EcodanHeatpump::handle_connect_response(Message& res)
    {
        ESP_LOGI(TAG, "connection reply received from heat pump");

        connected = true;
    }

    void EcodanHeatpump::handle_response(Message& res) {
        switch (res.type())
        {
        case MsgType::SET_RES:
        case MsgType::THERMOSTAT_SET_RES:
            handle_set_response(res);
            break;
        case MsgType::GET_RES:
        case MsgType::THERMOSTAT_INITIAL_GET_RES:
        case MsgType::THERMOSTAT_GET_RES:
        case MsgType::CONFIGURATION_RES:
            handle_get_response(res);
            break;
        case MsgType::CONNECT_RES:
            handle_connect_response(res);
            break;
        default:
            ESP_LOGI(TAG, "Unknown serial message type received: %#x", static_cast<uint8_t>(res.type()));
            break;
        }
    }

    void EcodanHeatpump::handle_set_response(Message& res)
    {
        //ESP_LOGW(TAG, res.debug_dump_packet().c_str());
        if (!cmdQueue.empty())
            cmdQueue.pop();

        if (res.type() != MsgType::SET_RES && res.type() != MsgType::THERMOSTAT_SET_RES)
        {
            ESP_LOGI(TAG, "Unexpected set response type: %#x", static_cast<uint8_t>(res.type()));
        }          
    }
}
}
