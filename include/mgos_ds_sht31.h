//
// Created by namoaton on 02.10.19.
//
#pragma once
#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
#include "DS28E17Rmt.h"
#include "SHT31.h"
#else
typedef struct DS28E17RmtTag DS28E17Rmt;
typedef struct SHT31Tag SHT31;
#endif

#ifdef __cplusplus
extern "C" {
#endif


    //SHT31 *mgos_ds_sht31_create(DS28E17Rmt *ds, char *ds_addr, uint8_t addr);
    void mgos_ds_sht31_create(SHT31 *_SHT31 ,DS28E17Rmt * ds, char* ds_addr,  uint8_t addr)  {
       // if (ds == nullptr) return nullptr;
        _SHT31->begin(ds,ds_addr,addr);
       //SHT31(ds,ds_addr,addr);
//        return new SHT31(ds,ds_addr,addr);
    };

    void mgos_ds_sht31_close(SHT31 *sht);

    bool mgos_ds_sht31_begin(SHT31 *sht);

    bool mgos_ds_sht31_read_TH(SHT31 *sht, float *data);

    float mgos_ds_sht31_read_temperature(SHT31 *sht);

    float mgos_ds_sht31_read_humidity(SHT31 *sht);

    void mgos_ds_sht31_reset(SHT31 *sht);

    void mgos_ds_sht31_heater(SHT31 *sht, bool h);

    uint8_t mgos_ds_sht31_crc8(const uint8_t *data, int len);


#ifdef __cplusplus
}

#endif