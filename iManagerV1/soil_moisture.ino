uint8_t readSoilHumidity()
{
  int readingsBucket[20];
  int acc = 0;
  analogReadResolution(12);
  for (int i = 0; i < 20; i++)
  {
    readingsBucket[i] = analogRead(rSensor);
  }
  for (int i = 0; i < 20; i++)
  {
    acc += readingsBucket[i];
  }
  int value = acc / 20;
  uint8_t mappedValue = map(value, 3200, 1730, 0, 100);
  mappedValue = constrain(mappedValue, 0, 100);

  return mappedValue;
}