//next_irr_flag = true
uint8_t fDay(struct Timer *timers)
{
  return (*timers).futureDay;
}
uint8_t fMth(struct Timer *timers)
{
  return (*timers).futureMonth;
}
uint8_t fYr(struct Timer *timers)
{
  return (*timers).futureYr;
}
int starthour(struct Timer *timers)
{
  return (*timers).StartHr;
}
int startmin(struct Timer *timers)
{
  return (*timers).StartMin;
}
int runtime(struct Timer *timers)
{
  return (*timers).Runtime;
}
int frequency(struct Timer *timers)
{
  return (*timers).Interval;
}
int valves(struct Timer *timers)
{
  return (*timers).Valve;
}
uint8_t next_irrigation()
{
  const uint8_t CLASH = 13;
  int fYr_store[All_Str.prog_count];
  int fMth_store[All_Str.prog_count];
  int fDay_store[All_Str.prog_count];
  int all_prog_pos[All_Str.prog_count];
  int corr_yr_progs[All_Str.prog_count];
  int corr_mth_progs[All_Str.prog_count];
  int corr_day_progs[All_Str.prog_count];
  int corr_starthr_progs[All_Str.prog_count];
  int corr_startmin_progs[All_Str.prog_count];
  // int corr_starthr_addr[All_Str.prog_count];
  // int corr_startmin_addr[All_Str.prog_count];
  int starthr_store[All_Str.prog_count]; //
  int startmin_store[All_Str.prog_count];
  //unsigned long nearest_epoch = 0UL;
  int nearest_hour = 0, nearest_min = 0;
  uint8_t nearest_yr = 0, nearest_mth = 0, nearest_day = 0, yrcount = 0, hrcount = 0, mthcount = 0, daycount = 0, progpos = 0, mincount = 0, occured_at = 0;

  // struct Timer Programs[PROG_SIZE];
  // EEPROM.get(5 * sizeof(All_Str.prog_count), Programs);
  if (All_Str.prog_count > 1)
  {
    /****get all future years****/
    for (int i = 0; i < PROG_SIZE; i++)
    { //go through all possible Programs
      if (fYr(&Programs[i]) >= 22)
      { // pick Programs set
        fYr_store[progpos] = fYr(&Programs[i]); // store the program years
        all_prog_pos[progpos] = i;              //store the program positions in array
        progpos++;
      }
    }
    /*****get nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (i == 0)
      {
        nearest_yr = fYr_store[i];
        // corr_prog = i + 1;
      }
      else
      {
        if (fYr_store[i] < nearest_yr)
        {
          nearest_yr = fYr_store[i];
          // corr_prog = i + 1;
        }
      }
    }
    // if(nearest_yr == 0)//test
    //   irrState = 1;
    /*****get number of occurences of nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (nearest_yr == fYr_store[i])
      {
        occured_at = all_prog_pos[i] + 1;
        corr_yr_progs[yrcount] = all_prog_pos[i]; // from all program pos get all corresponding Programs where nearest year occured
        yrcount++;                                //print for test
      }
    }
    // return yrcount; //test print
    /*****return corresponding program (not array position) if it occured only once*****/
    if (yrcount == 1)
    {
      // corr_prog = occured_at;
      return occured_at;
    }
    else
    {
      /*****get all months within the nearest year*****/
      for (int i = 0; i < yrcount; i++)
      {
        int pos = corr_yr_progs[i];
        fMth_store[i] = fMth(&Programs[pos]);
      }
      /*****get nearest month*****/
      for (int i = 0; i < yrcount; i++)
      {
        if (i == 0)
        {
          nearest_mth = fMth_store[i];
        }
        else
        {
          if (fMth_store[i] < nearest_mth)
          {
            nearest_mth = fMth_store[i];
          }
        }
      }
      /*****get number of occurences of nearest month*****/
      for (int i = 0; i < yrcount; i++)
      {
        if (nearest_mth == fMth_store[i])
        {
          occured_at = corr_yr_progs[i] + 1;
          corr_mth_progs[mthcount] = corr_yr_progs[i]; // from all program pos get all corresponding Programs where nearest year occured
          mthcount++;                                  //print for test
        }
      }
      // return mthcount; //test print
      /*****return corresponding program (not array position) if it occured only once*****/
      if (mthcount == 1)
      {
        // corr_prog = occured_at;
        return occured_at;
      }
      else
      {
        /*****get all days within the nearest month*****/
        for (int i = 0; i < mthcount; i++)
        {
          int pos = corr_mth_progs[i];
          fDay_store[i] = fDay(&Programs[pos]);
        }
        /*****get nearest day*****/
        for (int i = 0; i < mthcount; i++)
        {
          if (i == 0)
          {
            nearest_day = fDay_store[i];
          }
          else
          {
            if (fDay_store[i] < nearest_day)
            {
              nearest_day = fDay_store[i];
            }
          }
        }
        /*****get number of occurences of nearest day*****/
        for (int i = 0; i < mthcount; i++)
        {
          if (nearest_day == fDay_store[i])
          {
            occured_at = corr_mth_progs[i] + 1;
            corr_day_progs[daycount] = corr_mth_progs[i]; // from all program pos get all corresponding Programs where nearest day occured
            daycount++;                                   //print for test
          }
        }
        // return daycount; //test print
        /*****return corresponding program (not array position) if it occured only once*****/
        if (daycount == 1)
        {
          // corr_prog = occured_at;
          return occured_at;
        }
        else
        {
          /*****get all start hrs within the nearest day*****/
          for (int i = 0; i < daycount; i++)
          {
            int pos = corr_day_progs[i];
            starthr_store[i] = starthour(&Programs[pos]);
          }
          /*****get nearest hour*****/
          for (int i = 0; i < daycount; i++)
          {
            if (i == 0)
            {
              nearest_hour = starthr_store[i];
            }
            else
            {
              if (starthr_store[i] < nearest_hour)
              {
                nearest_hour = starthr_store[i];
              }
            }
          }
          /*****get number of occurences of nearest hour*****/
          for (int i = 0; i < daycount; i++)
          {
            if (nearest_hour == starthr_store[i])
            {
              occured_at = corr_day_progs[i] + 1;
              corr_starthr_progs[hrcount] = corr_day_progs[i]; // from all program pos get all corresponding Programs where nearest hour occured
              hrcount++;                                       //print for test
            }
          }
          // return hrcount; //test print
          /*****return corresponding program (not array position) if it occured only once*****/
          if (hrcount == 1)
          {
            // corr_prog = occured_at;
            return occured_at;
          }
          else
          {
            /*****get all start mins within the nearest hr*****/
            for (int i = 0; i < hrcount; i++)
            {
              int pos = corr_starthr_progs[i];
              startmin_store[i] = startmin(&Programs[pos]);
            }
            /*****get nearest min*****/
            for (int i = 0; i < hrcount; i++)
            {
              if (i == 0)
              {
                nearest_min = startmin_store[i];
              }
              else
              {
                if (startmin_store[i] < nearest_min)
                {
                  nearest_min = startmin_store[i];
                }
              }
            }
            /*****get number of occurences of nearest min*****/
            for (int i = 0; i < hrcount; i++)
            {
              if (nearest_min == startmin_store[i])
              {
                occured_at = corr_starthr_progs[i] + 1;
                corr_startmin_progs[mincount] = corr_starthr_progs[i]; // from all program pos get all corresponding Programs where nearest hour occured
                mincount++;                                            //print for test
              }
            }
            // return mincount; //test print
            /*****return corresponding program (not array position) if it occured only once*****/
            if (mincount == 1)
            {
              // corr_prog = occured_at;
              return occured_at;
            }
            else
            {
              /*****return a constant to let us know there is a clash*****/
              //irrState = 1;
              clashCount = mincount; // collect number of clashes
              for (int i = 0; i < mincount; i++)
              {
                clashProgs[i] = corr_startmin_progs[i]; //collect corresponding clashing programs
              }
              return CLASH;
            }
          }
        }
      }
    }
  }
  else
  {
    for (int i = 0; i < PROG_SIZE; i++)
    { //go through all possible Programs
      if (fYr(&Programs[i]) >= 22)
      { // pick Programs that are set
        fYr_store[progpos] = fYr(&Programs[i]); // store the program years
        all_prog_pos[progpos] = i;              //store the program positions in array
        progpos++;
      }
    }
    /*****get nearest year*****/
    for (int i = 0; i < All_Str.prog_count; i++)
    {
      if (i == 0)
      {
        nearest_yr = fYr_store[i];
        occured_at = all_prog_pos[i] + 1;
      }
      else
      {
        if (fYr_store[i] < nearest_yr)
        {
          nearest_yr = fYr_store[i];
          occured_at = all_prog_pos[i] + 1;
        }
      }
    }
    return occured_at;
  }
}
// /*****get all epochs*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   epoch_store[i] = epochs(&Programs[i]);
// }
// /*****get nearest epoch*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   if (i == 0) {
//     nearest_epoch = epoch_store[i];
//     // corr_prog = i + 1;
//   }
//   else {
//     if (epoch_store[i] < nearest_epoch) {
//       nearest_epoch = epoch_store[i];
//       // corr_prog = i + 1;
//     }
//   }
// }
// /*****get number of occurences of nearest epoch*****/
// for (int i = 0; i < All_Str.prog_count; i++) {
//   if (nearest_epoch == epoch_store[i]) {
//     occured_at = i + 1;
//     corr_epoc_progs[count] = i;// get all corresponding Programs where it occured
//     count++;//print for test
//   }
// }
// //return count;//test value
// // return All_Str.prog_count;//test value
// /*****return corresponding program if it occured only once*****/
// if (count == 1) {
//   // corr_prog = occured_at;
//   return occured_at;
// }
// /*****if shortest epoch occured more than once*****/
// else {
//   /*****get all start hrs within the shortest epoch*****/
//   for (int i = 0; i < count; i++) {
//     int pos = corr_epoc_progs[i];
//     starthr_store[i] = starthour(&Programs[pos]);//print for test
//   }
//   /*****get nearest start hours*****/
//   for (int i = 0; i < count; i++) {
//     if (i == 0) {
//       nearest_hour = starthr_store[i];
//       occured_at = corr_epoc_progs[i] + 1;// corresponding program where the nearest hour occured
//     }
//     else {
//       if (starthr_store[i] < nearest_hour) {
//         nearest_hour = starthr_store[i];
//         occured_at = corr_epoc_progs[i] + 1;// corresponding program where the nearest hour occured
//       }
//     }
//   }
//   /*****get number of occurences of nearest start hour*****/
//   for (int i = 0; i < count; i++) {
//     if (nearest_hour == starthr_store[i]) {
//       // occured_at = i + 1;
//       corr_starthr_addr[hrcount] = i;// get all corresponding position where nearest hour was found in the store
//       hrcount++;
//     }
//   }
//   /*****return corresponding program if it occured only once*****/
//   if (hrcount == 1) {
//     return occured_at;
//   }
//   /*****if shortest start hour occured more than once*****/
//   else {
//     /*****loop through corresponding epoch Programs and get the Programs corresponding to start hour addresses*****/
//     uint8_t hrprogcount = 0;
//     for (int i = 0; i < hrcount; i++) {
//       for (int j = 0; j < count; j++) {
//         if (corr_starthr_addr[i] == j) {
//           corr_starthr_progs[hrprogcount] = corr_epoc_progs[j];
//           hrprogcount++;
//         }
//       }

//     }
//     /*****get all start mins within the shortest start hrs*****/
//     for (int i = 0; i < hrcount; i++) {
//       int pos = corr_starthr_progs[i];
//       startmin_store[i] = startmin(&Programs[pos]);
//     }
//     /*****get nearest start mins*****/
//     for (int i = 0; i < hrcount; i++) {
//       if (i == 0) {
//         nearest_min = startmin_store[i];
//         occured_at = corr_starthr_progs[i] + 1;
//       }
//       else {
//         if (startmin_store[i] < nearest_min) {
//           nearest_min = startmin_store[i];
//           occured_at = corr_starthr_progs[i] + 1;
//         }
//       }
//     }
//     /*****get number of occurences of nearest start mins*****/
//     for (int i = 0; i < hrcount; i++) {
//       if (nearest_min == startmin_store[i]) {
//         // occured_at = i + 1;
//         // return occured_at;
//         corr_startmin_addr[mincount] = i;// get all corresponding Programs where it occured
//         mincount++;
//       }
//     }
//     /*****return corresponding program if it occured only once*****/
//     if (mincount == 1) {
//       return occured_at;
//     }
//     else {
//       for (int i = 0; i < mincount; i++) {
//         for (int j = 0; j < hrcount; j++) {
//           if (corr_startmin_addr[i] == j) {
//             corr_startmin_progs[minprogcount] = corr_starthr_progs[j];//get the program addresses of Programs with the same time settings
//             minprogcount++;
//           }
//         }
//       }
//       return 13;
//     }
//   }
// }
//    count = 0;
//    for (int i = 0; i < All_Str.prog_count; i++) {
//      starthr_store[i] = 0;
//    }
