# curly_eureka
ESP Home ProtoType Testing - BME280, D1Mini, Motor Controller (Custom Code)

## Approach
This is using ESP Home (https://esphome.io/index.html) as the foundation coding and integration to Home Assistant (https://www.home-assistant.io/).

What is in this prototype:
1. BME280 using https://esphome.io/components/sensor/bmp280.html
2. 16x2 Line I2C LCD Display using https://esphome.io/components/display/lcd_display.html
3. A simple LED as a switch component called "Office Room Airconditioner" using https://esphome.io/components/switch/gpio.html
4. A slow PWM driven LED called "Office Strobe Slow" using https://esphome.io/components/output/slow_pwm.html
5. Two Motor Controller custom code items using the WEMOS Motor Driver which is just a H-Bridge controller in the end.  This was a tes for driving Christmas LED strings, those with alternating polarity LEDs.  I have several sitting around after their controllers died after one season.  The idea is to control them as garden lighting (someday).

## Compiling etc.
You will need to install ESP Home and Home Assistant if you want to use this.

ESP Home is best initially set up using the wizard as it will set up directories and code structures.  Once you get the hang of it, it is much like any other Arduino ESP8266 coding practice.

I used VSCode as the IDE and linked this back to my GITHUB environment.

ESP Home needs to be compiled in the CLI in a terminal (well for me at least) using:

`esphome curly_eureka_1.yaml compile`

To deploy it to a connects D1 Mini use:
`esphome curly_eureka_1.yaml run`
