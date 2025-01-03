#pragma once
#include <string>

namespace esphome {
namespace ecodan 
{    

    struct Status
    {
        enum class HpMode : uint8_t
        {
            OFF = 255,
            HEAT_ROOM_TEMP = 0,
            HEAT_FLOW_TEMP = 1,
            HEAT_COMPENSATION_CURVE = 2,
            COOL_ROOM_TEMP = 3,
            COOL_FLOW_TEMP = 4
        };

        bool Initialized = false;

        HpMode HeatingCoolingMode = HpMode::HEAT_ROOM_TEMP;
        
        float Zone1SetTemperature{NAN};
        float Zone2SetTemperature{NAN};

        float CurrentRoomTemperatures[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        float TargetRoomTemperatures[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    };

} // namespace ecodan
} // namespace esphome