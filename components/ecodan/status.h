#pragma once
#include <string>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

namespace esphome {
namespace ecodan 
{    

    struct Status
    {
        enum class RCMASK : uint8_t
        {
            RC0 = 0x01,
            RC1 = 0x02,
            RC2 = 0x04,
            RC3 = 0x08,
            RC4 = 0x10,
            RC5 = 0x20,
            RC6 = 0x40,
            RC7 = 0x80
        };

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
        
        uint8_t RcMasterZone1 = 0x0;
        uint8_t RcMasterZone2 = 0x0;
        
        // default RC0
        RCMASK RcMask = RCMASK::RC0;

        float Zone1SetTemperature{NAN};
        float Zone2SetTemperature{NAN};

        float CurrentRoomTemperatures[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        float TargetRoomTemperatures[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

        void set_operation_mode(uint8_t mode)
        {
            Operation = static_cast<OperationMode>(mode);
        }

        void set_heating_cooling_mode(uint8_t mode)
        {
            HeatingCoolingMode = static_cast<HpMode>(mode);
        }

        void update_target_temperatures() {
            if (RcMasterZone1 >= 1 && RcMasterZone1 <= 8) {
                TargetRoomTemperatures[RcMasterZone1 - 1] = Zone1SetTemperature;
            }
            if (RcMasterZone2 >= 1 && RcMasterZone2 <= 8) {
                TargetRoomTemperatures[RcMasterZone2 - 1] = Zone2SetTemperature;
            }
        }
    };

} // namespace ecodan
} // namespace esphome