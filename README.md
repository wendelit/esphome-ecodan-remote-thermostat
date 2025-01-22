# ESPHome Ecodan Remote Thermostat
Mimics a remote thermostat over CNRF. You can use any thermostat/temp sensor as a remote thermostat. This projects supports up to 8 remote thermostats. No need to buy the PARxx units. The required hardware is the same as used by https://github.com/gekkekoe/esphome-ecodan-hp

# available languages
English (default), Dutch, Italian, French, Spanish. Select the language in `ecodan-remote-thermostat-esphome.yaml` file. 
If you want to contribute with a translation: copy the file `ecodan-labels-en.yaml` to `ecodan-labels-xx.yaml`, fill in all the labels and submit a pull request.

# recommended hardware
If you don't want to solder, use one of the tested boards. [More boards that should be working](https://github.com/SwiCago/HeatPump/issues/13#issuecomment-457897457). It also should work for airco units with cn105 connectors. 

Cable
* Get one of the grove female cable and a ST PAP-05V-S connector. Remove one end of the grove connector and replace it with a ST PAP-05V-S connector. Here's an example:
![image](https://github.com/gekkekoe/esphome-ecodan-hp/blob/main/img/m5stack_cn105.jpg?raw=true)

Pin mapping (from left to right)
| grove | cn105 |
|:---|:---|
|pin 1 - black (gnd) | pin 4 - black (gnd) |
|pin 2 - red (5v) | pin 3 - red (5v) |
|pin 3 - white (GPIO 2) | pin 2 - white (Tx) |
|pin 4 - yellow (GPIO 1) | pin 1 - yellow (Rx) |

*Note: pin 5 (12v) on the cn105 is not used.*

# where to buy
atom s3 lite
https://www.digikey.nl/en/products/detail/m5stack-technology-co-ltd/C124/18070571

grove cable (multiple options/lengths available)
https://www.digikey.nl/en/products/detail/seeed-technology-co-ltd/110990036/5482563

JST PAP-05V-S connector
https://www.digikey.nl/en/products/detail/jst-sales-america-inc/PAP-05V-S/759977


# build esphome-ecodan-remote-thermostat firmware
### Build via cmd line:
* Install ESPHome https://esphome.io/guides/getting_started_command_line.html
    ```console
    python3 -m venv venv
    source venv/bin/activate
    pip3 install wheel
    pip3 install esphome
    ```
* Fill in `secrets.yaml` and copy the `ecodan-remote-thermostat-esphome.yaml` to your esphome folder and edit the values (*check GPO pins (uart: section), you might need to swap the pins in the config*)
The secrets.yaml should at least contain the following entries:
```
wifi_ssid: "wifi network id"
wifi_password: "wifi password"
```
* Edit the following section in `ecodan-remote-thermostat-esphome.yaml` to match your configuration (esp board, zone1/zone2, language, server control, enable debug). Default is an esp32-s3 board, 1 zone and english language.

```
packages:
# up to 8 thermostats can be defined. 0-7.
  thermostat-room-0: !include { file: confs/thermostat-room.yaml, vars: { room_identifier: 0, room_name: "Room 1" } }
#  thermostat-room-1: !include { file: confs/thermostat-room.yaml, vars: { room_identifier: 1, room_name: "Room 2" } }
  remote_package:
    url: https://github.com/gekkekoe/esphome-ecodan-remote-thermostat
    ref: main
    refresh: always
    files: [ 
            confs/esp32s3.yaml,
            ## enable label language file
            confs/ecodan-labels-en.yaml,
            #confs/ecodan-labels-nl.yaml,
            #confs/ecodan-labels-it.yaml,
            #confs/ecodan-labels-fr.yaml,
            confs/server-control.yaml,
            #confs/debug.yaml,
            confs/wifi.yaml,
           ]

```

* Build
```console
esphome ecodan-remote-thermostat-esphome.yaml
```
* To find the tty* where the esp32 is connected at, use `sudo dmesg | grep tty`. On my machine it was `ttyACM0` for usb-c, and `ttyUSB0` for usb-a. On a Mac its `tty.usbmodemxxx`
* Connect your esp32 via usb and flash
```console 
esphome upload --device=/dev/ttyACM0 ecodan-remote-thermostat-esphome.yaml
```
* You can update the firmware via the web interface of the esp after the initial flash, or use the following command to flash over the network
```console 
esphome upload --device ip_address ecodan-remote-thermostat-esphome.yaml
```
The esphome component will be auto detected in Home Assistant. In Home Assistant you need to enable actions. See https://esphome.io/components/api.html#actions
Perform the following steps:

```
  1. Find your device in the device list
  2. Click the “configure” button next to it
  3. Check the “Allow the device to perform Home Assistant actions” box
  4. Then click “submit”.
```

Depending on how many remote thermostats you have configured the following actions will become available: `set_climate_temperature_room_0`, `set_climate_temperature_room_1`, .. `set_climate_temperature_room_7` 

Use these actions to link your thermostat to a temp sensor or any other thermostat. Update this value when the source changes. 
You can manually test (if everything is correct) by performing the following steps in Home Assistant:
```
  1. Click Developer tools and select the actions tab
  2. Select action set_climate_temperature_room_0
  3. Fill the temperature field and click perform action
```

Your thermostat is then updated with the new temperature.

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/gekkekoe)
