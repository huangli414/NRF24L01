#ifndef __RNF24L01_H__
#define __RNF24L01_H__
typedef unsigned char uchar;
typedef unsigned char uint;
void inerDelay_us(unsigned char n);
void init_NRF24L01(void);
uint NRF24L01SpiWriteByte(uint uchar);
uchar NRF24L01SpiReadByte(uchar reg);
void RNF24L01SetRxMode(void);
void SetTX_Mode(void);
uint NRF24L01SpiRwReg(uchar reg, uchar value);
uint NRF24L01SpiReadBuf(uchar reg, uchar *pBuf, uchar uchars);
uint NRF24L01SpiWriteBuf(uchar reg, uchar *pBuf, uchar uchars);
unsigned char NRF24L01RxPacket(unsigned char* rx_buf);
void NRF24L01TxPacket(unsigned char * tx_buf);
#endif
