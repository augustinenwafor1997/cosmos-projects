uint8_t batterypercentage(const uint8_t pin)
{
  analogReadResolution(12);
  float batVolt;
  int reading = analogRead(pin);
  batVolt = ((reading * 3.3) / 4096) * 5.55;
  float percent = (58.37 * batVolt) - 627.16;
  return percent;
}

bool isBatteryCharging(const uint8_t pin)
{
  analogReadResolution(12);
  float batVolt;
  int reading = analogRead(pin);
  batVolt = ((reading * 3.3) / 4096) * 5.55;
  if (batVolt > 11)
    return true;
  return false;
}
