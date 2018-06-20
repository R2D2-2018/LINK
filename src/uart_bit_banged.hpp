#ifndef UART_BIT_BANGED_HPP
#define UART_BIT_BANGED_HPP

#include "wrap-hwlib.hpp"

class UARTBitBanged : public hwlib::istream, public hwlib::ostream {
    hwlib::pin_out& tx;
    hwlib::pin_in& rx;
    const uint32_t baudrate;

    void putc(char c) override {
        const auto bit_cel = ( ( 1000L * 1000L ) / baudrate );

        tx.set( 1 );
        // use busy waiting, otherwise logging from within the RTOS
        // will cause serious problems
        hwlib::wait_us_busy( bit_cel );

        // start bit
        tx.set( 0 );
        hwlib::wait_us_busy( bit_cel );

        // 8 data bits
        for( uint_fast8_t i = 0; i < 8; ++i ){
            tx.set( ( c & 0x01 ) != 0x00 );
            c = c >> 1;
            hwlib::wait_us_busy( bit_cel );
        }   

        // 2 stop bits
        tx.set( 1 );
        hwlib::wait_us_busy( 2 * bit_cel );
   }       
   
   char getc() override {
      char c = 0;        
      
      const auto bit_cel = ( ( 1000L * 1000L ) / baudrate );
      
      // wait for start of startbit
      while( rx.get() ){ };//hwlib::wait_ns( 1 ); }
      
      // wait until halfway the first data bit
      auto t = hwlib::now_us();
      t += bit_cel + ( bit_cel / 2 );
      while( hwlib::now_us() < t ){
      };
      
      // 8 data bits
      for( uint_fast8_t i = 0; i < 8; ++i ){
         c = c >> 1;            
         if( rx.get() ){
            c = c | 0x80;                
         }
         
         t+= bit_cel;
         while( hwlib::now_us() < t ){};
      }   

      return c;
   }     

public:
    UARTBitBanged(hwlib::pin_out& tx, hwlib::pin_in& rx, uint32_t baudrate) : tx(tx), rx(rx), baudrate(baudrate) { }
};

#endif // UART_BIT_BANGED_HPP