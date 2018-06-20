#include "link_module.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    hwlib::wait_ms(1000);

    LinkModule::waitForAddress();

    hwlib::cout << "Src-slave hello world!" << hwlib::endl; 
}