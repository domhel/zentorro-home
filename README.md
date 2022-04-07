# Zentorro Home
Software to connect 433MHz-controllable Smart Home devices to Home Assistant

- [See on Hackster.io](https://www.hackster.io/448758/433mhz-smart-home-zentorro-home-b05f44)
- [Home Assistant Config (GitHub)](https://github.com/dippa-1/zentorro-home-hass-config)

## Arduino requirements
- ArduinoJSON
- RCSwitch
- ESPMqttClient
- Effortless-SPIFFS

## Folder structure
- `ZentorroHome`: Arduino project. This is what you need to upload to your NodeMCU or other ESP
- `home-assistant-config`: Config files for HA. Some file contents need to be appended/merged, other files can be copied in whole. Read Hackster entry for more detailed instructions.
