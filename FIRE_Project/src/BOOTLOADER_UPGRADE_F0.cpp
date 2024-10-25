// #include "BOOTLOADER_UPGRADE_F0.h"
//  #include "M5Unified.h"
// void BOOTLOADER_UPGRADE::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
//                               uint8_t length) {
//     _wire->beginTransmission(addr);
//     _wire->write(reg);
//     for (int i = 0; i < length; i++) {
//         _wire->write(*(buffer + i));
//     }
//     _wire->endTransmission();
// }

// void BOOTLOADER_UPGRADE::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
//                              uint8_t length) {
//     uint8_t index = 0;
//     _wire->beginTransmission(addr);
//     _wire->write(reg);
//     _wire->endTransmission(false);
//     _wire->requestFrom(addr, length);
//     for (int i = 0; i < length; i++) {
//         buffer[index++] = _wire->read();
//     }
// }

// bool BOOTLOADER_UPGRADE::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl,
//                          uint32_t speed) {
//     _wire  = wire;
//     _addr  = addr;
//     _sda   = sda;
//     _scl   = scl;
//     _speed = speed;
//     _wire->begin(_sda, _scl);
//     _wire->setClock(speed);
//     _wire->setBufferSize(3000);
//     delay(10);
//     _wire->beginTransmission(_addr);
//     uint8_t error = _wire->endTransmission();
//     if (error == 0) {
//         return true;
//     } else {
//         return false;
//     }     
// }

// void BOOTLOADER_UPGRADE::func_iap_upgrade(const unsigned char *app_buffer)
// {
//     int8_t error = 1;
//     uint8_t sendbuffer[1032];
//     uint16_t loop=0;
//     uint16_t buffer_index = 0;//*endp; 

//     uint32_t APPLICATION_ADDRESS = 0x08001000;
//     uint32_t sendbuffer_Size = 0;   

//     sendbuffer_Size = FW_SIZE;  

//     printf("begin\r\n"); 
//     M5.Display.printf("begin\r\n");
//     // jumpBootloader();

//     _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR);    
//     error = _wire->endTransmission(); 
//     while (error != 0) {
//         _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR);    
//         error = _wire->endTransmission();   
//         printf("waitting bootloader");
//         M5.Display.printf("waitting bootloader\n");
//     }    
    
//     do {
//         /*Fille data in to buffuer*/
//         sendbuffer[0]=OPC_WREN;      /*Command Code*/
        
//         /*Address*/
//         sendbuffer[1]=(uint8_t)(APPLICATION_ADDRESS>>24);
//         sendbuffer[2]=(uint8_t)(APPLICATION_ADDRESS>>16);
//         sendbuffer[3]=(uint8_t)(APPLICATION_ADDRESS>>8);
//         sendbuffer[4]=(uint8_t)(APPLICATION_ADDRESS>>0);
    
//         if (sendbuffer_Size>=1024) {
//             sendbuffer[5]=0x04;
//             sendbuffer[6]=0x0;
//             do {
//                 sendbuffer[8+loop]=app_buffer[buffer_index++];
//                 printf("%02X,",sendbuffer[8+loop]);
//                 loop++;
//             } while(loop < 1024);
        
//         } else {
//             sendbuffer[5] = (uint8_t)(sendbuffer_Size >> 8);
//             sendbuffer[6] = (uint8_t)(sendbuffer_Size >> 0); 
//             do {
//                 sendbuffer[8+loop]=app_buffer[buffer_index++];
//                 printf("%02X,",sendbuffer[8+loop]);
//                 loop++;
//             } while(loop < sendbuffer_Size);
//         }
//         printf("\n");
    
//         sendbuffer[7]=0xFF;      
//         loop=0;
//         if(sendbuffer_Size >= 1024) {
//             _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//             error = _wire->endTransmission(); 
//             while (error) {
//                 _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//                 error = _wire->endTransmission();
//                 printf("fuck\r\n");
//             }            
//             _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR);
//             _wire->write(sendbuffer, 1032);
//             _wire->endTransmission(); 
//             sendbuffer_Size -= 1024;
//             APPLICATION_ADDRESS += 1024;
//         //*temp +=128;
//         } else {
//             _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//             error = _wire->endTransmission(); 
//             while (error) {
//                 _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//                 error = _wire->endTransmission();        
//             }             
//             _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR);
//             _wire->write(sendbuffer, sendbuffer_Size+8);
//             _wire->endTransmission();
//             sendbuffer_Size=0;
//         }
//         delay(200);
//         M5.Display.printf("addr 0x%x data send success\n",APPLICATION_ADDRESS);
//     } while(sendbuffer_Size>0);
//     _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//     error = _wire->endTransmission(); 
//     while (error) {
//         _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR); 
//         error = _wire->endTransmission();        
//     } 
//     _wire->beginTransmission(BOOTLOADER_UPGRADE_ADDR);
//     _wire->write(OPC_USRCD);
//     _wire->endTransmission();  
//     _wire->setBufferSize(128); 

//     // checkUpgrade();
// }

// bool BOOTLOADER_UPGRADE::checkUpgrade(void)
// {
//     _wire->beginTransmission(_addr);    
//     int8_t error = _wire->endTransmission(); 
//     while (error != 0) {
//         printf("not good\r\n");
//         _wire->beginTransmission(_addr);    
//         error = _wire->endTransmission(); 
//         delay(100);
//     }    
//     if (error == 0) {
//         printf("success\r\n");
//     }     
// }


// uint8_t BOOTLOADER_UPGRADE::getFirmwareVersion(void) {
//     _wire->beginTransmission(_addr);
//     _wire->write(FIRMWARE_VERSION_REG);
//     _wire->endTransmission(false);  

//     uint8_t RegValue;

//     _wire->requestFrom(_addr, 1); 
//     RegValue = _wire->read();
//     return RegValue;
// }

// uint8_t BOOTLOADER_UPGRADE::setI2CAddress(uint8_t addr) {
//   uint8_t temp[2] = {0};

//   temp[0] = I2C_ADDRESS_REG;

//   _wire->beginTransmission(_addr);
//   _wire->write(temp[0]);
//   _wire->write(addr);
//   _wire->endTransmission();
//   _addr = addr;
//   return _addr;
// }

// uint8_t BOOTLOADER_UPGRADE::getI2CAddress(void) {
//   uint8_t temp[2] = {0};

//   temp[0] = I2C_ADDRESS_REG;

//   _wire->beginTransmission(_addr);
//   _wire->write(temp[0]);
//   _wire->endTransmission(false);

//   uint8_t RegValue;

//   _wire->requestFrom(_addr, 1);
//   RegValue = _wire->read();
//   return RegValue;
// }

// void BOOTLOADER_UPGRADE::jumpBootloader(void) {
//     uint8_t value = 1;

//     writeBytes(_addr, JUMP_TO_BOOTLOADER_REG, (uint8_t *)&value, 1);
// }
