#INCLUDE    <16F887.H>
#include    <TV_PICKIT2_SHIFT_LCD.c> 
#FUSES      WDT, PUT, HS, NOPROTECT, NOLVP
#USE        DELAY(CLOCK = 20000000)
#USE        I2C(MASTER,SLOW,SDA=PIN_D0,SCL=PIN_D1)
#include    <TV_DS1307.c>

#define LCD_ENABLE_PIN  PIN_c0                                                                       
#define LCD_RS_PIN      PIN_c2                                   
#define LCD_RW_PIN      PIN_c1                                    
#define LCD_DATA4       PIN_e2                                   
#define LCD_DATA5       PIN_e1                                   
#define LCD_DATA6       PIN_e0                                    
#define LCD_DATA7       PIN_a5                                    
#include <lcd.c>

#define btn_mode pin_b2       
#define btn_up   pin_b1
#define btn_dw   pin_b3
#define on_off   pin_b4
#define btn_set  pin_b0  
                                                                   
#define relay        pin_b7
#define bat_relay    output_low(relay);
#define tat_relay    output_high(relay); 

#define den_nen_lcd  pin_a4                    
 
unsigned int16 gio_ht = 0, phut_ht = 0, giay_ht = 0, giay_temp = 0;
unsigned int16 gio_tat = 0, phut_tat = 0;
unsigned int16 gio_bat = 0, phut_bat = 0;
int1 chedo_hd;
unsigned int8 dem_btnup = 0, dem_btndw = 0;                                                                                  
unsigned int8 mode = 0, set = 0;
int1 sln_btnup = 0, sln_btndw = 0;               

void ht_soz_toa_do_xy(signed int8 so, x1, y1)
{                     
   lcd_gotoxy(x1, y1);
   for(int i=0; i<6; i++)
   {          
      if(i==3) lcd_gotoxy(x1, y1+1);
      if(lcd_so_x[so][i] != 7)   
         lcd_putc(lcd_so_x[so][i]);
      else lcd_putc(0xff);                                                                                
   }
} 

// DOC THOI GIAN TU DS130707
void doc_thoi_gian_tu_ds1307()
{                  
   doc_thoi_gian_ds1307();      
   gio_ht  = (gio_ds1307  >> 4)*10 + (gio_ds1307&0x0f);
   phut_ht = (phut_ds1307 >> 4)*10 + (phut_ds1307&0x0f);
   giay_ht = (giay_ds1307 >> 4)*10 + (giay_ds1307&0x0f);
}            

// DAT LAI THOI GIAN HIEN TAI    
void set_time_ds1307(unsigned int8 gio, unsigned int8 phut)
{
   gio_ds1307 =  ((gio / 10) << 4)  | (gio % 10);
   phut_ds1307 = ((phut / 10) << 4) | (phut % 10);
   giay_ds1307 = 0;                   
   dat_lai_thoi_gian_ds1307();   
}                                  

// HIEN THI LCD  
void hienthi_lcd()
{
   switch (mode)
   {                                                                                               
/**/  case 0:    
         ht_soz_toa_do_xy(gio_ht/10,1,1);
         ht_soz_toa_do_xy(gio_ht%10,4,1); 
             
         lcd_gotoxy(7,1);   lcd_putc(3);
         lcd_gotoxy(7,2);   lcd_putc(1);               
                                   
         ht_soz_toa_do_xy(phut_ht/10,8,1);
         ht_soz_toa_do_xy(phut_ht%10,11,1);  
                                        
         lcd_gotoxy(14,1);        
         printf(lcd_putc,":%02lu", giay_ht); 
         
         lcd_gotoxy(14,2);    lcd_putc(" ");
         if (chedo_hd == 0)   {lcd_gotoxy(15,2);    lcd_putC("  ");}
         else                 {lcd_gotoxy(15,2);    lcd_putC("**"); }
         break;
      
/**/  case 1:  switch (set)   //thoi gian hien tai
         {   
         case 0:
            lcd_gotoxy(1, 1) ;
            lcd_putc("1. Cai dat thoi ") ;
            lcd_gotoxy(1, 2) ;
            lcd_putc("  gian hien tai.");
            break;
         case 1:
            lcd_gotoxy(1, 1) ;
            printf (lcd_putc, "* Gio  : %02lu       ", gio_ht);
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  Phut : %02lu       ", phut_ht); 
            break;         
         case 2:
            lcd_gotoxy(1, 1) ;  
            printf (lcd_putc, "  Gio  : %02lu       ", gio_ht); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "* Phut : %02lu        ", phut_ht); 
            break;
         }
         break;
      
/**/  case 2:  switch (set)   //gio, phut tat relay
         {                                                                                                    
         case 0:
            lcd_gotoxy(1, 1) ;
            lcd_putc("2. Cai dat thoi ");
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  gian TAT den. ");
            break;
         case 1:              
            lcd_gotoxy(1, 1) ; 
            printf (lcd_putc, "* Gio  : %02lu       ", gio_tat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  Phut : %02lu       ", phut_tat); 
            break;
         case 2:      
            lcd_gotoxy(1, 1) ;  
            printf (lcd_putc, "  Gio  : %02lu       ", gio_tat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "* Phut : %02lu        ", phut_tat); 
            break;
         }
         break;
      
/**/  case 3: switch (set)    //che do hoat dong
         {
         case 0:
            lcd_gotoxy(1, 1) ;
            lcd_putc("3. Cai dat thoi ");
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  gian BAT den. ");
            break;
         case 1:              
            lcd_gotoxy(1, 1) ; 
            printf (lcd_putc, "* Gio  : %02lu       ", gio_bat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  Phut : %02lu       ", phut_bat); 
            break;
         case 2:      
            lcd_gotoxy(1, 1) ;  
            printf (lcd_putc, "  Gio  : %02lu       ", gio_bat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "* Phut : %02lu        ", phut_bat); 
            break;
         }
         break;
   }
}

// NUT NHAN SET
void button_set()
{
   if (input(btn_set) == 0)
   {                
      delay_ms (40) ;
      if (input (btn_set) == 0)
      {
         set++ ;
         switch (mode)
         {
            case 1:
            case 2: 
            case 3:  if (set > 2) set = 1;    break;
         }
         hienthi_lcd ();
      }
      while (input (btn_set) == 0) ;
   }
}

// NUT NHAN MODE 
void button_mode()
{
   if (input (btn_mode) == 0)
   {
      delay_ms (40) ;
      if (input (btn_mode) == 0)
      {              
         if (set != 0)  set = 0;
         else  mode++;
         if (mode > 3)  mode = 0;
         hienthi_lcd ();
      }
      while (input (btn_mode) == 0);
   }
}

// NUT NHAN TANG
int1 chong_doi_btn_up()
{             
   unsigned int8 n = 20;
   if (input(btn_up) == 0) 
   {              
      dem_btnup++;      delay_ms(1);   
      if (sln_btnup == 1)   n = 200;
      if(dem_btnup >= n)    
      {     
         dem_btnup = 0;   sln_btnup= 1;   
         return 1;       
      }
      else return 0;
   }
   else 
   {
      sln_btnup = 0;   dem_btnup = 0;
      return 0;
   }
}  

void button_up()
{                              
   if(chong_doi_btn_up())
   {                                                                                                  
      switch(mode)
      {
         case 0:    break;                                                                                                      
         case 1:
            if(set == 1)
            {           
               if(gio_ht == 23) gio_ht = 0;
               else  gio_ht++;
            }
            else  if(set == 2)
            {
               if(phut_ht == 59)   phut_ht = 0;
               else  phut_ht++;
            }
            set_time_ds1307(gio_ht, phut_ht);
            break;
         
         case 2:
            if(set == 1)
            {
               if(gio_tat >= 23)   gio_tat = 0;                                                                
               else  gio_tat++;
               write_eeprom(0,gio_tat);
            }
            else if(set == 2)
            {
               if(phut_tat >= 59)  phut_tat = 0;
               else  phut_tat++; 
               write_eeprom(1,phut_tat);
            }
            break;
         
         case 3:  
            if(set == 1)
            {
               if(gio_bat >= 23)   gio_bat = 0;                                                                
               else  gio_bat++;
               write_eeprom(2,gio_bat);
            }
            else if(set == 2)
            {
               if(phut_bat >= 59)  phut_bat = 0;
               else  phut_bat++; 
               write_eeprom(3,phut_bat);
            }
            break;  
      }                   
      hienthi_lcd ();
   }                                          
}

// NUT NHAN GIAM 
int1 chong_doi_btn_dw()
{
   unsigned int8 n = 20;        
   if (input(btn_dw) == 0)
   {                             
      dem_btndw++;          delay_ms(1);  
      if (sln_btndw == 1)   n = 200; 
      if(dem_btndw >= n)            
      {                          
         dem_btndw = 0;   sln_btndw = 1;                                                                     
         return 1;
      }                        
      else return 0;
   }
   else 
   {                  
      sln_btndw = 0;   dem_btndw = 0;
      return 0;
   }
}

void button_dw()     
{
   if(chong_doi_btn_dw())
   {      
      switch(mode)
      {         
         case 0:       break; 
         case 1:        
            if(set == 1)                                                                                    
            {
               if (gio_ht == 0)    gio_ht = 23;
               else  gio_ht--;
            }                  
            else  if(set == 2)
            {                                                                                                   
               if (phut_ht == 0)   phut_ht = 59;
               else  phut_ht--;
            }
            set_time_ds1307(gio_ht, phut_ht);
            break;
         
         case 2:
            if (set == 1)
            {
               if (gio_tat == 0)    gio_tat = 23;
               else  gio_tat--;     
               write_eeprom(0, gio_tat);
            }          
            else if(set == 2)
            {         
               if (phut_tat == 0)  phut_tat = 59;
               else  phut_tat--;   
               write_eeprom(1, phut_tat);
            }
            break;
         
         case 3:  
            if (set == 1)
            {
               if (gio_bat == 0)    gio_bat = 23;
               else  gio_bat--;     
               write_eeprom(2, gio_bat);
            }          
            else if(set == 2)
            {         
               if (phut_bat == 0)  phut_bat = 59;
               else  phut_bat--;   
               write_eeprom(3, phut_bat);
            }
            break;
      }
      hienthi_lcd ();
   }
} 

/* BUTTON ON-OFF*/
void button_onOff()
{
   if (input(on_off)==0)
   {
      chedo_hd = 1;
   }
   
   if (chedo_hd == 1)
   {
      if ((gio_ht >= gio_tat) || (gio_ht < 5)) 
         bat_relay;
   }
   
/**/ //error   
   if (chedo_hd == 0)
   {
      if(input_state(relay))
      {
         if((gio_ht*60+phut_ht) < (gio_tat*60+phut_tat))
         {
            if((gio_ht*60+phut_ht) > (gio_bat*60+phut_bat))
            {
               bat_relay;
            }
         }
      }
   }
/*****************/ 
   
   if (gio_ht == 6 && phut_ht < 3)  chedo_hd = 0;
}
/*bat tat den */
void on_off_den()
{
   if ((phut_ht == phut_bat) && (gio_ht == gio_bat))  bat_relay;
   if (chedo_hd == 0)
   {
      if ((phut_ht == phut_tat) && (gio_ht == gio_tat))  tat_relay;
   }
   else
   {
      if ((phut_ht == 30) && (gio_ht == 5))  tat_relay;
   }
}
// CHUONG TRINH CHINH
VOID MAIN()
{    
   set_tris_a (0xcf);      set_tris_b (0x7f) ;     tat_relay; 
   set_tris_c (0x00) ;     set_tris_e (0x00) ;
   port_b_pullups(0x7f);   output_high(den_nen_lcd);
                   
   lcd_init ();   
   
   //watch dog timer
   setup_wdt(WDT_2304MS);
                                                                                                                    
   mode = 0;         set = 0;          chedo_hd = 0;    
                           
   gio_tat = read_eeprom (0);  phut_tat = read_eeprom (1);    
   
   gio_bat = read_eeprom (2);  phut_bat = read_eeprom (3); 
   
   lcd_send_byte(0, 0x40);                                                                                     
   for(int8 j=0; j<64; j++)
   {                               
      lcd_send_byte(1,LCD_MA_8DOAN[j]);
   }                                                  
   unsigned char temp = 0; // dua ve man hinh chinh.                                                 
   while (true)     
   {                           
      doc_thoi_gian_tu_ds1307(); 
      if(giay_temp != giay_ht)
      {   
         if(0 != mode)
         {
            temp++;
            if(120 == temp)
            {
               mode = 0;
               set = 0;
            }
         }
         else 
            temp = 0;
         hienthi_lcd ();         
         giay_temp = giay_ht;   
      } 
       
      for (unsigned int16 k=0; k<500; k++) 
      {                                                                                
         button_up ();              button_dw ();
         button_mode ();            button_set ();  
         on_off_den();              button_onOff();
      }    
      restart_wdt();
   }
}
                                                                                                                

