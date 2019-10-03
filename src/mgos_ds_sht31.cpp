//
// Created by namoaton on 02.10.19.
//
#include <mgos.h>
//#include "mgos_ds28e17_rmt.h"
#include "mgos_ds_sht31.h"

void mgos_ds_sht31_create(SHT31* sht, DS28E17Rmt * ds, char* ds_addr,  uint8_t addr)  {
    sht->begin(ds, ds_addr, addr);
}
void mgos_ds28e17_rmt_close(SHT31 *sht) {
    if (sht != nullptr) {
        delete sht;
        sht = nullptr;
    }
}

bool mgos_ds_sht31_begin(SHT31 *sht,uint8_t i2caddr){
    if (sht == nullptr) return false;
    return sht->begin(sht->_ds,sht->_deviceAddress,i2caddr);
}

bool mgos_ds_sht31_read_TH(SHT31 *sht,float* data){
    if (sht == nullptr) return false;
    return sht->readTH(data);
}

float mgos_ds_sht31_read_temperature(SHT31 *sht){
    return sht->readTemperature();
}

float mgos_ds_sht31_read_humidity(SHT31 *sht){
    return sht->readHumidity();
}

void mgos_ds_sht31_reset(SHT31 *sht){
    if (sht == nullptr) return;
    sht->reset();
}

void mgos_ds_sht31_heater(SHT31 *sht,bool h){
    if (sht == nullptr) return;
    sht->heater(h);
}

uint8_t mgos_ds_sht31_crc8(SHT31 *sht, const uint8_t *data, int len){
    return sht->crc8(data, len);
}