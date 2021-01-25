// #include <Wire.h>
// #include "WEMOS_Motor.h"
#include "esphome.h"

// namespace wimos_motor {


#define _MOTOR_A 0
#define _MOTOR_B 1

#define _SHORT_BRAKE 0
#define _CCW 1
#define _CW 2
#define _STOP 3
#define _STANDBY 4

static const char *TAG = "custom.WemosMotorComponent";

class WMotorCustomComponent : public Component, public Switch {
  
  public:

    WMotorCustomComponent(){
      ESP_LOGD(TAG, "Instantiating default instance");
      ESP_LOGCONFIG(TAG, "Motor:A");
      ESP_LOGCONFIG(TAG, "Adx: 0x30");
      ESP_LOGCONFIG(TAG, "Freq:1000");
      _address = 0x30;
      _motor = _MOTOR_A;
      setfreq(1000);

    }

    WMotorCustomComponent(uint8_t address, uint8_t motor, uint32_t freq){
      ESP_LOGD(TAG, "Instantiating default instance");
            
      if(motor==_MOTOR_A){
        _motor=_MOTOR_A;
        ESP_LOGCONFIG(TAG, "Motor:A");
      }
      else{
        _motor=_MOTOR_B;
        ESP_LOGCONFIG(TAG, "Motor:B");
      }
      setfreq(1000);
      _address = address;
      ESP_LOGCONFIG(TAG, "Adx: %#x", address);
      ESP_LOGCONFIG(TAG, "Freq: %d", freq);
    }
    void setup() override {
      // This will be called once to set up the component
      // think of it as the setup() call in Arduino

      setmotor(_STOP);
      _state = false;
      publish_state(_state);

    }

    void write_state(bool state) override {
      _state = state;

      if (_state == false){setmotor(_STOP);}

      // Acknowledge new state by publishing it
      publish_state(_state);
      // You can also log messages
      ESP_LOGD(TAG, "Starting-stopping to Motor");  
    }

    void loop() override {
      unsigned long now = millis();
      // This will be called very often after setup time.
      // think of it as the loop() call in Arduino
      if (_state){
        if (_toggle){
          setmotor(_CW, pattern1_[index_++]);
          if (index_ >= sizeof(pattern1_) ){
            _toggle = false;
            index_ = 0;
          }
        }else{
          setmotor(_CCW, pattern1_[index_++]);
          if (index_ >= sizeof(pattern1_) ){
            _toggle = true;
            index_ = 0;
          }          
        }

        
      }

      // You can also log messages
      //ESP_LOGD("custom", "Writing to Motor");
      
    }

  protected:

    int pwm_{0}; 
    bool _state;

    /* setmotor() -- set motor

    motor:
      _MOTOR_A	0	Motor A
      _MOTOR_B	1	Motor B

    dir:
      _SHORT_BRAKE	0
      _CCW			1
      _CCW			2
      _STOP			3
      _STANDBY		4

    pwm_val:
      0.00 - 100.00  (%)

    */
    void setmotor(uint8_t dir, float pwm_val)
    {
      uint16_t _pwm_val;
        
      Wire.beginTransmission(_address);
      Wire.write(_motor | (byte)0x10);
      Wire.write(dir);

      _pwm_val=uint16_t(pwm_val*100);
      if(_pwm_val>10000)
        _pwm_val=10000;

      Wire.write((byte)(_pwm_val >> 8));
      Wire.write((byte)_pwm_val);
      Wire.endTransmission();     // stop transmitting

      delay(100);
    }

    void setmotor(uint8_t dir)
    {
      setmotor(dir,100);
    }

    /* setfreq() -- set PWM's frequency

    freq:
      PWM's frequency	

    */
    void setfreq(uint32_t freq)
    {
      Wire.beginTransmission(_address);
      Wire.write(((byte)(freq >> 16)) & (byte)0x0f);
      Wire.write((byte)(freq >> 16));
      Wire.write((byte)(freq >> 8));
      Wire.write((byte)freq);
      Wire.endTransmission();     // stop transmitting
      delay(100);
    }

  private:
    uint8_t _address;
    uint8_t _motor;
    //bool _use_STBY_IO=false;
    uint8_t _STBY_IO;
    bool _toggle{true};

    bool is_on_{false};
    // These are used for cycling the LED brightness
    unsigned long period_start_time_{0};
    unsigned long period_{20};
    uint8_t index_{0};
    uint8_t pattern1_[40] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,95,90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0};

};

