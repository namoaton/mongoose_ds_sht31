#include "SHT31.h"
#include "mgos.h"
extern "C" {
#include <mgos_system.h>
#include <mgos_time.h>
}

SHT31::SHT31(){
    
}
SHT31::SHT31(DS28E17Rmt *ds) {
    setBridge(ds);
}
SHT31::SHT31(DS28E17Rmt *ds, char* ds_addr,  uint8_t addr =0x44):_ds(ds),
    _deviceAddress {ds_addr[0], ds_addr[1], ds_addr[2], ds_addr[3], ds_addr[4],
                    ds_addr[5], ds_addr[6], ds_addr[7]}, _i2caddr = addr
{
    setBridge(ds)
}
SHT31::~SHT31() {
    if (_ownBridge) {
        delete _ds;
        _ownBridge = false;
    }
}
void SHT31::init(DS28E17Rmt *ds, char* ds_addr,  uint8_t addr){
    _i2caddr = i2caddr;
    setBridge(ds);
    memcpy(_deviceAddress,ds_addr,8);
}

void SHT31::setBridge(DS28E17Rmt *ds) {
    if (_ownBridge) {
        delete _ds;
       _ds = nullptr;
    }
    _ownBridge = true;
   _ds = ds;
}


bool SHT31::begin(DS28E17Rmt * ds, char* ds_addr, uint8_t i2caddr) {;
     _i2caddr = i2caddr;
    setBridge(ds);
    memcpy(_deviceAddress,ds_addr,8);
    reset();
  return true;
}
//bool SHT31::begin(uint8_t i2caddr) {
//    _i2caddr = i2caddr;
//    reset();
//    return true;
//}
uint16_t SHT31::readStatus(void) {
  writeCommand(SHT31_READSTATUS);
  uint8_t data[3] = {0,0,0};
  _ds->ReadDataStop((uint8_t*) _deviceAddress, _i2caddr, 3, data);
//   Wire.requestFrom(_i2caddr, (uint8_t)3);
  uint16_t stat = data[0]<<8;
  stat |= data[1];
  //Serial.println(stat, HEX);
  return stat;
}

void SHT31::reset(void) {
  writeCommand(SHT31_SOFTRESET);
  mgos_msleep(10);
}

void SHT31::heater(bool h) {
  if (h)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}


float SHT31::readTemperature(void) {
  readTempHum();
  return temp;
}
  

float SHT31::readHumidity(void) {
  readTempHum();
  return humidity;
}
bool SHT31::readTH(float* data){
  bool res = true;
  res = readTempHum();
  if(res){
    data[0] = humidity;
    data[1] = temp;
  }
  return res;
}


bool SHT31::readTempHum(void) {
  uint8_t readbuffer[6];

  writeCommand(SHT31_MEAS_HIGHREP);  
  mgos_msleep(500);
  _ds->ReadDataStop((uint8_t*) _deviceAddress, (_i2caddr<<1)|1, 6, readbuffer);

  uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];
  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;
  // LOG(LL_WARN, ("ST = %X",ST));
  double stemp = ST;
  stemp *= 175;
  stemp /= 0xffff;
  stemp = -45 + stemp;
  temp = stemp;
  // LOG(LL_WARN, ("SRH = %X",SRH));
  double shum = SRH;
  shum *= 100;
  shum /= 0xFFFF;
  
  humidity = shum;
  
  return true;
}

void SHT31::writeCommand(uint16_t cmd) {
  uint8_t data[2];
  data[0] = (uint8_t)(cmd>>8);
  data[1] = (uint8_t)(cmd &0xff);
  _ds->WriteDataStop( (uint8_t*)_deviceAddress, _i2caddr<<1, 2, data);
//   Wire.beginTransmission(_i2caddr);
//   Wire.write(cmd >> 8);
//   Wire.write(cmd & 0xFF);
//   Wire.endTransmission();  
}

uint8_t SHT31::crc8(const uint8_t *data, int len)
{
/*
*
 * CRC-8 formula from page 14 of SHT spec pdf
 *
 * Test data 0xBE, 0xEF should yield 0x92
 *
 * Initialization data 0xFF
 * Polynomial 0x31 (x8 + x5 +x4 +1)
 * Final XOR 0x00
 */
unsigned char crc = 0xFF;
 unsigned int i;

    while (len--)
    {
        crc ^= *data++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}


