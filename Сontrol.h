//=============================================== Control library v.1.0 =====================================================================================

#include "Arduino.h"

//===========================================================================================================================================================

class Button
{
    private:
      boolean p_state;
      uint8_t att;
      uint64_t t;
    public:
      Button(uint8_t pin) 
      : att(pin), p_state(true)
      {
        pinMode(att, INPUT_PULLUP);
        t = millis();
      }
      
      boolean ifclick()
      {
        boolean c_state = digitalRead(att), res = false;
        if(p_state && !c_state)
        {
          if(millis() - t > 5)
          {
            t = millis();
            c_state = digitalRead(att);
            res = !c_state;
          }
        }
        p_state = c_state;
        return res;
      }
};

//===========================================================================================================================================================

class Pot
{
  private:
    uint8_t att;
  public:
    Pot(uint8_t pin) 
    : att(pin)
    {
      pinMode(att, INPUT);
    }

    uint16_t getval()
    {
      return analogRead(att);
    }
};

//===========================================================================================================================================================

struct JData
{
  int16_t xvalue;
  int16_t yvalue;
};

class Joystick
{
  private:
    uint8_t px, py, b, lshx, rshx, lshy, rshy, mx, my;
    int16_t lbx, ubx, lby, uby;
    boolean p_state;
    uint64_t t;

    static int16_t trim_data(uint16_t a, uint8_t lsh, uint8_t rsh, uint8_t m, int16_t lb, int16_t ub)
    {
      if(lsh + rsh != 0 && lsh*rsh == 0)
      {
        if(lsh == 0)
        {
          if(a >= 512 - rsh - m && a <= 512 + rsh + m) 
            return 0;
          else
          {
            int16_t res;
            if(a > 512)
              res = (int16_t)a - rsh - m;
            else 
              res = (int16_t)a + rsh + m;
            res = map(res - 512, rsh + m - 512, 512 - rsh - m, lb, ub + 1);
            return res;
          }
        }
        else
        {
          if(a >= 512 - lsh - m && a <= 512 + lsh + m) 
            return 0;
          else
          {
            int16_t res;
            if(a > 512)
              res = (int16_t)a - lsh - m;
            else 
              res = (int16_t)a + lsh + m;
            res = map(res - 512, lsh + m - 512, 512 - lsh - m, lb, ub + 1);
            return res;
          }
        }
      } else return map((int16_t)a, 0, 1024, lb, ub + 1);
    }
  public:
    Joystick(uint8_t x_pin, uint8_t y_pin, uint8_t button_pin, uint8_t margin_x = 0, uint8_t margin_y = 0)
    : px(x_pin), py(y_pin), b(button_pin), mx(margin_x), my(margin_y), lshx(0), rshx(0), lshy(0), rshy(0), lbx(-255), ubx(255), lby(-255), uby(255), p_state(true)
    {
      pinMode(px, INPUT);
      pinMode(py, INPUT);
      pinMode(b, INPUT_PULLUP);
      t = millis();
    }

    void autotrim(int16_t lower_bound_x, int16_t upper_bound_x, int16_t lower_bound_y, int16_t upper_bound_y)
    {
      lbx = lower_bound_x;
      lby = lower_bound_y;
      ubx = upper_bound_x;
      uby = upper_bound_y;
      uint16_t valx = analogRead(px);
      uint16_t valy = analogRead(py);
      int16_t delta = valx - 512;
      if(delta > 0)
        rshx = abs(delta);
      else
        lshx = abs(delta);
      delta = valy - 512;
      if(delta > 0)
        rshy = abs(delta);
      else
        lshy = abs(delta);
    }

    JData getval()
    {
      JData data;
      data.xvalue = map(trim_data(analogRead(px), lshx, rshx, mx, -255, 255), -255, 255, lbx, ubx);
      data.yvalue = map(trim_data(analogRead(py), lshy, rshy, my, -255, 255), -255, 255, lby, uby);
      return data;
    }

    boolean ifclick()
    {
      boolean c_state = digitalRead(b), res = false;
      if(p_state && !c_state)
      {
        if(millis() - t > 5)
        {
          t = millis();
          c_state = digitalRead(b);
          res = !c_state;
        }
      }
      p_state = c_state;
      return res;
    }
};
