#INCLUDE    <16F887.H>
#device     adc = 10
#FUSES      NOWDT, PUT, HS, NOPROTECT, NOLVP
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
#define  step  0.0048875855327;                    
float temp_gtadc, gtadc = 0;  
unsigned int8 gio_ht = 0, phut_ht = 0, giay_ht = 0, giay_temp = 0;
unsigned int8 gio_tat = 0, phut_tat = 0;
int1 tt_relay,  cp_doc_cbq, chedo_hd;
unsigned int8 dem_btnup = 0, dem_btndw = 0, dem_tg_tcd = 0, dem_tat_dn_lcd = 0;                                                                                              
unsigned int8 mode = 0, set = 0, sl_doc_adc = 0;
unsigned int16 dem = 0;
int1 sln_btnup = 0, sln_btndw = 0;              
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
/**/  case 0:  switch(set)       //hien thi thoi hien tai, trang thai relay, che do hoat dong
         {
         case 0:
            lcd_gotoxy(1, 1) ;
            printf (lcd_putc, "Thgian: %02u:%02u:%02u  ", gio_ht, phut_ht, giay_ht);
            lcd_gotoxy(1, 2) ;
            if (tt_relay == 1)   lcd_putc("Den:Bat ");                                                     
            else                 lcd_putc("Den:Tat ");
            if (chedo_hd == 0)   lcd_putC(" Tu dong"); 
            else                 lcd_putC("Thu cong");
            break;
         case 1:                                                                                    
            lcd_gotoxy(1, 1) ;                                                                         
            lcd_putc("Thoigian cai dat");
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "tat den: %02u:%02u       ", gio_tat, phut_tat);
            break;                                                                                     
         }
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
            printf (lcd_putc, "* Gio  : %02u       ", gio_ht);
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  Phut : %02u       ", phut_ht); 
            break;         
         case 2:
            lcd_gotoxy(1, 1) ;  
            printf (lcd_putc, "  Gio  : %02u       ", gio_ht); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "* Phut : %02u        ", phut_ht); 
            break;
         }
         break;
      
/**/  case 2:  switch (set)   //gio, phut tat relay
         {                                                                                                    
         case 0:
            lcd_gotoxy(1, 1) ;
            lcd_putc("2. Cai dat thoi ");
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  gian tat den. ");
            break;
         case 1:              
            lcd_gotoxy(1, 1) ; 
            printf (lcd_putc, "* Gio  : %02u       ", gio_tat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "  Phut : %02u       ", phut_tat); 
            break;
         case 2:      
            lcd_gotoxy(1, 1) ;  
            printf (lcd_putc, "  Gio  : %02u       ", gio_tat); 
            lcd_gotoxy(1, 2) ;
            printf (lcd_putc, "* Phut : %02u        ", phut_tat); 
            break;
         }
         break;
  /**/  case 3: switch (set)    //che do hoat dong
      {
         case 0:
            lcd_gotoxy(1, 1) ;
            lcd_putC("3.Cai dat che do");
            lcd_gotoxy(1, 2) ;
            lcd_putC("  hoat dong.    ");
            break;          
         case 1:  switch (chedo_hd)
         {
            case  0:
               lcd_gotoxy(1, 1) ; 
               lcd_putC("* Tu dong.      ");
               lcd_gotoxy(1, 2) ;
               lcd_putC("  Thu cong.     ");
               break;
            case  1:
               lcd_gotoxy(1, 1) ;
               lcd_putC("  Tu dong.      ");
               lcd_gotoxy(1, 2) ;
               lcd_putC("* Thu cong.     ");
               break;
         }
         break;
      }
   }
}
// NUT NHAN SET
void button_set()
{
   if (input(btn_set) == 0)
   {                
      delay_ms (20) ;
      if (input (btn_set) == 0)
      {
         set++ ;
         switch (mode)
         {
            case 1:
            case 2:  if (set > 2) set = 1;    break;
            case 0:
            case 3:  if (set > 1) set = 1;    break;
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
      delay_ms (20) ;
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
// NUT NHAN TAT MO DEN BANG TAY 
void button_on_off_lamp()
{        
   if (input (on_off) == 0)
   {              
      delay_ms (20) ;       
      if (input (on_off) == 0)
      {             
                           
         if (tt_relay == 0)    
         {       
            bat_relay; 
            tt_relay = 1;
         }
         else
         {     
            tat_relay; 
            tt_relay = 0;       
         }
         hienthi_lcd ();
         delay_ms(300); 
      }
      while (input (on_off) == 0);
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
         case 0:                                                                                                     
            if(set == 0)    
            {                      
               output_toggle(den_nen_lcd); 
               while (input(btn_up) == 0); 
            }                  
            break;   
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
               chedo_hd =  ~ chedo_hd;
               while (input(btn_up) == 0); 
            }   
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
         case 0:  
            if(set == 0)    
            {        
               output_toggle(den_nen_lcd); 
               while (input(btn_dw) == 0); 
            }
            break; 
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
            if(set == 1)  
            {  
               chedo_hd =  ~chedo_hd;
               while (input(btn_dw) == 0); 
            }
      }
      hienthi_lcd ();
   }
}
// DOC DIEN AP TREN QUANG TRO
void doc_dien_ap_tren_quang_tro()
{                                                                                                                          
   dem++;     delay_us(100);
   if(dem >= 500)                                                                                          
   {                     
      temp_gtadc = read_adc() + temp_gtadc;
      sl_doc_adc++;          
      dem = 0;                                                                                             
      if (sl_doc_adc == 40)                                                                                                 
      {                                           
         temp_gtadc = temp_gtadc/40.0;         
         gtadc = temp_gtadc * step;    
         temp_gtadc = 0;        
         sl_doc_adc = 0; 
      }  
   } 
}                          
// BAT DEN KHI HOAT DONG O CHE DO TU DONG 
void bat_den_o_che_do_tu_dong()
{                          
   if (cp_doc_cbq != 0 && gio_ht> 17) 
   {                                         
      doc_dien_ap_tren_quang_tro();
      if (gtadc >= 4.9)      
      {                       
         bat_relay;        tt_relay = 1;     
         cp_doc_cbq = 0;   write_eeprom(2,cp_doc_cbq);  
         gtadc = 0;             
         hienthi_lcd ();                     
      }                       
   }          
}                                                                           
// TAT RELAY - XOA TT RELAY
void tat_den_o_che_do_tu_dong()
{
   if ((phut_tat <= phut_ht) && (gio_tat == gio_ht))
   {
      tat_relay;    tt_relay = 0;      
   }                        
   if ((gio_ht == 16) && (cp_doc_cbq == 0))  
   {                               
      cp_doc_cbq = 1;      
      write_eeprom(2,cp_doc_cbq);   
   }                 
}                                      
// THOAT KHOI CHE DO CAI DAT 
void thoat_che_do_cai_dat()
{
   if (mode != 0  ||  set != 0) 
   {           
      dem_tg_tcd ++;         
      if (dem_tg_tcd >= 90) 
      {           
         mode = 0;    set = 0;     
         dem_tg_tcd = 0;  
      }               
   }                  
   else  dem_tg_tcd = 0; 
}                                   
// TU DONG TAT DEN NEN LCD
void tu_dong_tat_den_nen_lcd()
{       
   if((mode ==0) && (set == 0) && (input_state(den_nen_lcd) == 1))     
   {               
       dem_tat_dn_lcd++;
       if(dem_tat_dn_lcd >= 90)
       {                   
          output_low(den_nen_lcd);    
          dem_tat_dn_lcd = 0;                                                                                 
       }                                                                                                  
   }
   else dem_tat_dn_lcd = 0;
}                                                                                                                                                                          
// CHUONG TRINH CHINH
VOID MAIN()
{                                                                    
   set_tris_a (0xcf);      set_tris_b (0x7f) ;      tt_relay = 0;    tat_relay; 
   set_tris_c (0x00) ;     set_tris_e (0x00) ;
   port_b_pullups(0x7f);   output_low(den_nen_lcd);
                   
   lcd_init ();                      
   
   setup_adc(adc_clock_div_8);  
   setup_adc_ports(sAN1 | VREF_VREF); 
   set_adc_channel(1);     
                                                                                                                    
   mode = 0;         set = 0;         temp_gtadc=0;    
   gtadc = 0;        chedo_hd = 0;    
                           
   gio_tat = read_eeprom (0);  phut_tat = read_eeprom (1);  
   cp_doc_cbq = read_eeprom (2);                                              
   while (true)     
   {                           
      doc_thoi_gian_tu_ds1307(); 
      if(giay_temp != giay_ht)
      {                           
          hienthi_lcd ();         
          giay_temp = giay_ht;                                                                            
          thoat_che_do_cai_dat();   
          tu_dong_tat_den_nen_lcd();   
      } 
       
      for (unsigned int16 i=0; i<500; i++) 
      {                                        
         if (chedo_hd == 0)                                                                          
         {                 
            bat_den_o_che_do_tu_dong ();        
            tat_den_o_che_do_tu_dong ();                
         }                                         
         else   button_on_off_lamp ();
         button_up ();              button_dw ();
         button_mode ();            button_set () ;  
      }                                       
   }
}
