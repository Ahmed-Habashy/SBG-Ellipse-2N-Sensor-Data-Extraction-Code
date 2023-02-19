
#include <string.h>
#include <ctype.h>


unsigned int  msg_c, i;
byte byte_c[2];
char y, rxstream[1024],  msg_tx[65] ;
bool f_euler,f_nav, f_ekf_nav ,  rmc_rd;
int indc0, indc1, indc2, indc3, indc4, indc5, indc6, indc7, indc8, indc9;
long  tr_angle= 0;
long ac_speed= 0;
String stringGPS = "";

unsigned long previousMillis = 0; // last time update
long interval = 400; // interval at which to do something (milliseconds)




void setup() 
{
  Serial2.begin(19200); // ts4000
  Serial2.println("start ver2 mod RMC_test100");
  delay (90000);
  Serial1.begin(19200 );
  delay(200);
  Serial3.begin(19200 ); // stmf407
  delay(400);
   

   msg_c = i = 0;
   f_nav =f_euler= f_ekf_nav =  rmc_rd = false;
}


void loop() 
{
  unsigned long currentMillis = millis();
  if (Serial1.available()) 
 { 

      rxstream[i] = Serial1.read();
      
      if (rxstream[i] == 0x06 && rxstream[i-1] == 0x5A && rxstream[i-2] == 0xFF) //--------------------------- Euler Msg Sync (ID=06)
     {
       i=2;
       f_euler = true;
       rxstream[i] = 0x06;
       rxstream[i-1] = 0x5A;
       rxstream[i-2] = 0xFF;
          byte_c[0] = msg_c&0xff;
          byte_c[1]= (msg_c>>8);
       msg_tx[0]= 0xff;
       msg_tx[1]= 0x5a;
       msg_tx[2]= byte_c[0];
       msg_tx[3]= byte_c[1];
       msg_tx[4]= 0x06;
       msg_tx[18]= 0x2d;       // *************************************************************    POD ID
       msg_tx[63]= 0x33;
       msg_tx[64]= 0x0A;
     }     
     if (i==22 && f_euler == true)
     {
        for (int x=10; x<= 21; x++) // 8,9 >> msg_length ---- 10:21 >> euler
        {
        msg_tx[x-5]= rxstream[x];
        }

        f_euler = false;
      }
      
      if (rxstream[i] == 0x08 && rxstream[i-1] == 0x5A && rxstream[i-2] == 0xFF)   //------------------------------ EKF-Navigation Msg (ID=08)
     {
       i=2;
       f_ekf_nav = true;
       msg_tx[17]= 0x08;
       rxstream[i]   = 0x08;
       rxstream[i-1] = 0x5A;
       rxstream[i-2] = 0xFF; 
     } 
     if (i==58 && f_ekf_nav == true)
     {
        for (int x=34; x<= 57; x++) // ----  >> Lat.-Long.-Alt. 
        {
        msg_tx[x-4]= rxstream[x];
        }
        f_ekf_nav = false;
     }
     
     if ( rxstream[i] == 0x50 && rxstream[i-1] == 0x47 && rxstream[i-2] == 0x24)    //----------------------------------- $GPRMC_msg
     {
       i=2;
       rmc_rd = true;
       rxstream[i] = 0x50;
       rxstream[i-1] = 0x47;
       rxstream[i-2] = 0x24;
     }
     if (rxstream[i-1] != 0x0D && rxstream[i] != 0x0A && rmc_rd == true)
        {
         stringGPS += rxstream[i];
        }        
     else if (stringGPS  != "" ) //if (stringGPS.substring(0, 4) == "PRMC"  )                             
      {
        rmc_rd = false;
        indc0 = stringGPS.indexOf(',');
        indc1 = stringGPS.indexOf(',', indc0+1);
        indc2 = stringGPS.indexOf(',', indc1+1);
        indc3 = stringGPS.indexOf(',', indc2+1);
        indc4 = stringGPS.indexOf(',', indc3+1);
        indc5 = stringGPS.indexOf(',', indc4+1);
        indc6 = stringGPS.indexOf(',', indc5+1);
        indc7 = stringGPS.indexOf(',', indc6+1);
        indc8 = stringGPS.indexOf(',', indc7+1);
        indc9 = stringGPS.indexOf(',', indc8+1);
        ac_speed = stringGPS.substring(indc6+1, indc7).toInt();
        tr_angle = stringGPS.substring(indc7+1, indc8).toInt();       
         msg_tx[19] = ac_speed & 0xff ;
         msg_tx[20] = ac_speed>>8 ;     
         msg_tx[21] = tr_angle & 0xff ;
         msg_tx[22]= (tr_angle>>8);
           
           for (int x=23; x<= 29; x++) //------------------------------ ----  >> RESERVED ******************
            {
            msg_tx[x]= 0x00; 
            }
                   stringGPS = "";                         
      }
      
     if (rxstream[i] == 0x0E && rxstream[i-1] == 0x5A && rxstream[i-2] == 0xFF )      //------------------------ POSITION Msg Sync (ID=0E)
     {
       i=2;
       f_nav = true;
       rxstream[i] = 0x0E;
       rxstream[i-1] = 0x5A;
       rxstream[i-2] = 0xFF;
     }    
     if (i==59 && f_nav == true)
     {
      for (int x= 3; x<=10; x++)  // ----GPS_POS_STATUS / GPS_TOW
        {
        msg_tx[x+51]= rxstream[x+7]; 
        }
        msg_tx[62]=  rxstream[58];  // Num of space vehicles
        f_nav = false;
      }

      if(currentMillis - previousMillis > interval)
        {
           previousMillis = currentMillis; 
           Serial2.write(msg_tx,65);
           //Serial2.println("");
           Serial3.write(msg_tx,65);
           msg_c++;
           if (msg_c > 65000)     msg_c =0;    
        }
     
     if (i < 1024) i++;
      else i = 0;
      
  } 
}
