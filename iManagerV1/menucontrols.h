
#ifndef _MENUCONTROLS_H_
#define _MENUCONTROLS_H_

int Upbtn;
int Dwnbtn;
int Entbtn;
int Rhtbtn;
int Leftbtn;
struct menucontrol
{
  //    int Upbtn;
  //    int Dwnbtn;
  //    int Entbtn;
  //    int Rhtbtn;
  //    int Leftbtn;
  uint8_t cusor;
  uint8_t frame;
  uint8_t cp;
};

struct menucontrol menu;

//////////////////////////// BUTTON  CONTROLS SETTUP//////////////////////////////////////////////////
void pinmode(int pin)
{
  if (pin != 0)
  {
    pinMode(pin, INPUT_PULLUP);
  }
}

void menubutton_Init(int upbtn, int dwnbtn, int entbtn, int rhtbtn, int leftbtn)
{
  pinmode(upbtn);
  pinmode(dwnbtn);
  pinmode(entbtn);
  pinmode(rhtbtn);
  pinmode(leftbtn);
  Upbtn = upbtn;
  Dwnbtn = dwnbtn;
  Entbtn = entbtn;
  Rhtbtn = rhtbtn;
  Leftbtn = leftbtn;
  //  menu.Upbtn = upbtn;
  //  menu.Dwnbtn = dwnbtn;
  //  menu.Entbtn = entbtn;
  //  menu.Rhtbtn = rhtbtn;
  //  menu.Leftbtn = leftbtn;
}

//////////////////////////// xml menu hieght control ////////////////////////////////////////////////
uint8_t height(uint8_t frame, uint8_t offset, uint8_t position)
{
  uint8_t set = 12 + (offset * (position - frame));
  if (set > 60 || set < 24)
  {
    return 0;
  }
  else
    return set;
}

/*
   push button initialization using structs and array of structs
*/
const unsigned long DEBOUNCE_DELAY = 50UL;

struct button_t
{
  const uint8_t pin;
  uint8_t state;
  uint8_t stateLast;
  uint8_t count;
  unsigned long lastDebounceTime;
};

struct button_t buttons[] = // array of button_t structs
{
  {PB13, LOW, LOW, 0, 0UL}, {PB14, LOW, LOW, 0, 0UL}, {PB12, LOW, LOW, 0, 0UL}, {PA8, LOW, LOW, 0, 0UL}, {PB15, LOW, LOW, 0, 0UL}
};

bool Debounce(struct button_t *button)
{
  const unsigned long millisRef = millis();

  uint8_t state = digitalRead((*button).pin);
  if (state != (*button).stateLast) //if state change
  {
    (*button).lastDebounceTime = millisRef; //record current time
  }

  if ((millisRef - (*button).lastDebounceTime) > DEBOUNCE_DELAY) // check if debounce is over
  {
    // digitalWrite (PC13, LOW);
    if (state != (*button).state)
    {
      (*button).state = state; // record current state

      if ((*button).state == LOW) // confirm button press
      {
        (*button).stateLast = state; //change last state to current state

        return HIGH;
      }
    }
  }

  (*button).stateLast = state;
  // digitalWrite (PC13, HIGH);
  return LOW;
}

/////////////////////////////////////////////////// menu funtion /////////////////////////////////////////////
int menufunc(uint8_t menusize, uint8_t screensize, void (*f)(uint8_t, uint8_t, uint8_t, struct All), uint8_t control, uint8_t cusor, struct All all)
{
  /*
     a struct is added as a parameter
     try converting to pointers
  */
  uint16_t selected;

  if (cusor == 1)
  {
    menu.cp = 1;
    menu.cusor = cusor;
    menu.frame = 0;
  }
  lcdChrono.restart();
  while (selected == 0)
  {
    lcdChrono.restart();
    ///////////////// RUN XML /////////////////////
    (*f)(menu.cp, menu.frame, control, all);
    ////////////////////// up button ///////////////////

    if (Debounce(&buttons[0]) == HIGH) // modified to record states accurately
    {
      lcdChrono.restart();
      if (control == 0) // just scroll up
      {
        menu.cusor--;
        if (menu.cusor < 1)
        {
          menu.cusor = 1;
          menu.cp = menu.cusor;
        }

        if (menu.cp >= 1)
        {
          menu.cp -= 1;
          if (menu.cp < 1)
          {
            menu.cp = 1;
            if (menu.cusor >= 1)
            {
              menu.frame = menu.cusor - 1;
            }
          }
        }
      }
      else // return cusor position and value
      {
        selected = menu.cusor * 100;
      }
    }

    ////////////////// down button //////////////////////
    else if (Debounce(&buttons[1]) == HIGH)
    {
      lcdChrono.restart();
      if (control == 0) // scroll down
      {
        //      digitalWrite (PC13, LOW);
        menu.cusor++;
        if (menu.cp <= screensize)
        {
          menu.cp += 1;
          if (menu.cp > screensize)
          {
            menu.cp = screensize;
            if (menu.cusor > screensize)
            {
              menu.frame = menu.cusor - screensize;
            }
          }
        }

        if (menu.cusor >= menusize)
        {
          menu.cusor = menusize;
          if (menu.cusor < screensize)
          {
            menu.cp = menusize;
          }
          else
          {
            menu.cp = screensize;
            menu.frame = menu.cusor - screensize;
          }
        }
      }
      else //reutrn position and value
      {
        // digitalWrite (PC13, LOW);

        selected = menu.cusor * 10;
      }
    }

    ////////////////////////////////////// Enter button /////////////////////////////////////////
    else if (Debounce(&buttons[2]) == HIGH)
    {
      lcdChrono.restart();
      selected = menu.cusor;
    }
    //////////////////////////////////////// right button ///////////////////////////////////////
    else if (Debounce(&buttons[3]) == HIGH)
    {
      lcdChrono.restart();
      if (control == 0) // return position and value
      {
        selected = menu.cusor * 10;
      }
      else //scroll
      {
        // digitalWrite (PC13, LOW);

        menu.cusor++;
        if (menu.cp <= screensize)
        {
          menu.cp += 1;
          if (menu.cp > screensize)
          {
            menu.cp = screensize;
            if (menu.cusor > screensize)
            {
              menu.frame = menu.cusor - screensize;
            }
          }
        }

        if (menu.cusor >= menusize)
        {
          menu.cusor = menusize;
          if (menu.cusor < screensize)
          {
            menu.cp = menusize;
          }
          else
          {
            menu.cp = screensize;
            menu.frame = menu.cusor - screensize;
          }
        }
      }
    }
    ////////////////////////////////////////// left button /////////////////////////////////////
    else if (Debounce(&buttons[4]) == HIGH)
    {
      lcdChrono.restart();
      if (control == 0) // return position and value
      {
        selected = menu.cusor * 100;
      }
      else // scroll down
      {

        menu.cusor--;
        if (menu.cusor < 1)
        {
          menu.cusor = 1;
          menu.cp = menu.cusor;
        }

        if (menu.cp >= 1)
        {
          menu.cp -= 1;
          if (menu.cp < 1)
          {
            menu.cp = 1;
            if (menu.cusor >= 1)
            {
              menu.frame = menu.cusor - 1;
            }
          }
        }
      }
    }
  }
  //     digitalWrite (PC13, HIGH);
  lcdChrono.restart();
  return selected;
}

#endif // _MENUCONTROLS_H_
