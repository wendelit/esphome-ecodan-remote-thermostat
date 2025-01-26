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

        enum class OperationMode : uint8_t
        {
            UNAVAILABLE = 255,
            OFF = 0,
            DHW_ON = 1,
            HEAT_ON = 2, // Heating
            COOL_ON = 3, // Cooling
            FROST_PROTECT = 5,
            LEGIONELLA_PREVENTION = 6
        };

        bool Initialized = false;

        HpMode HeatingCoolingMode = HpMode::OFF;
        OperationMode Operation = OperationMode::UNAVAILABLE;
        
        float Zone1SetTemperature{NAN};
        float Zone2SetTemperature{NAN};

        // default first temp to 0c to prevent initial p1 errors on slow units
        float CurrentRoomTemperatures[8] = { 0.0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        float TargetRoomTemperatures[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

        void set_operation_mode(uint8_t mode)
        {
            Operation = static_cast<OperationMode>(mode);
        }

        void set_heating_cooling_mode(uint8_t mode)
        {
            HeatingCoolingMode = static_cast<HpMode>(mode);
        }        
    };

} // namespace ecodan
} // namespace esphome