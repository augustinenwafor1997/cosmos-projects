//#pragma once
class Button
{
public:
    Button(uint8_t pin, bool pullup = false, uint16_t debounceDelay = 50)
        : _pin(pin), _state(LOW), _lastState(LOW),
          _lastMillis(0), _debounceDelay(debounceDelay),
          _lastDebounceTime(0)
    {
        if (pullup == true)
        {
            pinMode(_pin, INPUT_PULLUP);
        }
        else
        {
            pinMode(_pin, INPUT_PULLDOWN);
        }
    }

    /**
    @brief Debounces the button and returns the state if it was just changed.
    @param None
    @retval None
    */
    bool checkClick(bool triggerState = LOW)
    {
        bool reading = digitalRead(_pin);
        // Checks if the buttons has changed state
        if (reading != _lastState)
        {
            _lastDebounceTime = millis();
        }
        uint16_t tim = millis() - _lastDebounceTime;
        // Checks if the buttons hasn't changed state for '_debounceDelay' milliseconds.
        if (tim >= _debounceDelay && tim < 500)
        {
            // Checks if the buttons has changed state
            if (reading != _state)
            {
                _state = reading;
                return _state;
            }
        }
        _lastState = reading;
        // If this code is reached, it returns the normal state of the button.
        if (triggerState == HIGH)
        {
            return LOW;
        }
        else
        {
            return HIGH;
        }
    }

    /**
    @brief Debounces the button and returns the state if it was just changed on rising edge.
    @param None
    @retval None
    */
    bool checkClickOnRelease(bool triggerState = LOW)
    {
        bool reading = digitalRead(_pin);
        // Checks if the buttons is rising
        if ((reading == HIGH) && (_lastState == LOW))
        {
          uint16_t tim = millis() - _lastDebounceTime;
            // Checks if the buttons hasn't changed state for '_debounceDelay' milliseconds.
            if (tim >= _debounceDelay && tim < 1000)
            {
                _lastState = reading;
                return LOW;
            }
            _lastDebounceTime = millis();
        }
        _lastState = reading;

        // If this code is reached, it returns the normal state of the button.
        if (triggerState == HIGH)
        {
            return LOW;
        }
        else
        {
            return HIGH;
        }
    }

    /**
    @brief Debounces the button and returns the state if it was just pressed on hold for one second.
    @param None
    @retval None
    */
    bool checkHold(bool triggerState = LOW)
    {
        bool reading = digitalRead(_pin);
        // Checks if the buttons has changed state(LOW)
        if ((reading == HIGH) && (_lastState == LOW))
        {
            // Checks if the buttons hasn't changed state for '_debounceDelay' milliseconds.
            if (millis() - _lastDebounceTime >= 4000)
            {
                _lastState = reading;
                _lastDebounceTime = millis();
                return LOW;
            }
        }
        else
        {
            _lastDebounceTime = millis();
            _lastState = reading;
            // If this code is reached, it returns the normal state of the button.
            if (triggerState == HIGH)
            {
                return LOW;
            }
            else
            {
                return HIGH;
            }
        }
    }

    /**
    @brief Debounces the button and returns the state if it was just pressed cycle the process.
    @param None
    @retval None
    */
    bool checkpress(bool triggerState = LOW)
    {
        bool reading = digitalRead(_pin);
        // Checks if the buttons has changed state
        if (reading == HIGH)
        {
            // Checks if the buttons hasn't changed state for '_debounceDelay' milliseconds.
            if (millis() - _lastDebounceTime > 2000)//150
            {
                _lastDebounceTime = millis();
                return LOW;
            }
            else
            {
                // If this code is reached, it returns the normal state of the button.
                if (triggerState == HIGH)
                {
                    return LOW;
                }
                else
                {
                    return HIGH;
                }
            }
        }
        else
        {

            _lastDebounceTime = millis();
            // If this code is reached, it returns the normal state of the button.
            if (triggerState == HIGH)
            {
                return LOW;
            }
            else
            {
                return HIGH;
            }
        }
    }

private:
    const uint8_t _pin;
    bool _state;
    bool _lastState;
    uint32_t _lastMillis;
    uint16_t _debounceDelay;
    uint32_t _lastDebounceTime;
};
