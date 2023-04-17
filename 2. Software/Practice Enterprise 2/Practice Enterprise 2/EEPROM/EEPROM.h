/*
 * EEPROM.h
 *
 * Created: 17/04/2023 11:12:51
 *  Author: William VR
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void EEPROM_write(uint8_t ucAddress, uint8_t ucData);
uint8_t EEPROM_read(uint8_t ucAddress);

#endif /* EEPROM_H_ */