unsigned char gio_ds1307,phut_ds1307,giay_ds1307;                              
void dat_lai_thoi_gian_ds1307()
{
   i2c_start ();
   i2c_write (0xD0);
   i2c_write (0x00) ;
   i2c_write (giay_ds1307);
   i2c_write (phut_ds1307) ;
   i2c_write (gio_ds1307);
   i2c_stop () ;
}                                  
void doc_thoi_gian_ds1307()
{
   i2c_start () ;
   i2c_write (0xD0) ;
   i2c_write (0x00);
   i2c_start ();      
   i2c_write (0xD1) ;
   giay_ds1307 = i2c_read ();
   phut_ds1307 = i2c_read ();
   gio_ds1307 = i2c_read (0);
   i2c_stop () ;
}
