/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This is a sample usage example that shows how to interface with the SGP30
 * and SHTC1 at the same time.
 *
 * This is simply a merged version of the usage_example.c from
 *     https://github.com/Sensirion/embedded-sht
 * and sgp30_example_usage.c from
 *     https://github.com/Sensirion/embedded-sgp
 */


#include "sgp30.h"
#include "sht.h"

#include <stdio.h> // printf
#include <unistd.h> // sleep


int main(void) {
    u16 i = 0;
    s16 err;
    u16 tvoc_ppb, co2_eq_ppm;
    u32 iaq_baseline;
    u16 ethanol_signal, h2_signal;

    s8 ret;
    s32 temperature, humidity;

    /* Busy loops for initialization. The main loop does not work without
     * the sensors. */
    while (sgp30_probe() != STATUS_OK) {
        printf("SGP sensor probing failed\n");
        sleep(1);
    }
    printf("SGP sensor probing successful\n");

    while (sht_probe() != STATUS_OK) {
        printf("SHT sensor probing failed\n");
    }
    printf("SHT sensor probing successful\n");


    /* Read gas signals */
    err = sgp30_measure_raw_blocking_read(&ethanol_signal,
                                            &h2_signal);
    if (err == STATUS_OK) {
        /* Print ethanol signal and h2 signal */
        printf("Ethanol signal: %u\n", ethanol_signal);
        printf("H2 signal: %u\n", h2_signal);
    } else {
        printf("error reading signals\n");
    }


    /* Consider the two cases (A) and (B):
     * (A) If no baseline is available or the most recent baseline is more than
     *     one week old, it must discarded. A new baseline is found with
     *     sgp_iaq_init() */
    err = sgp30_iaq_init();
    /* (B) If a recent baseline is available, set it after sgp_iaq_init() for
     * faster start-up */
    /* IMPLEMENT: retrieve iaq_baseline from presistent storage;
     * err = sgp30_set_iaq_baseline(iaq_baseline);
     */

    /* Run periodic IAQ measurements at defined intervals */
    while (1) {
        err = sgp30_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
        if (err == STATUS_OK) {
            printf("tVOC  Concentration: %dppb\n", tvoc_ppb);
            printf("CO2eq Concentration: %dppm\n", co2_eq_ppm);
        } else {
            printf("error reading IAQ values\n");
        }

        ret = sht_measure_blocking_read(&temperature, &humidity);
        if (ret == STATUS_OK) {
            printf("measured temperature: %0.2f degreeCelsius, "
                      "measured humidity: %0.2f percentRH\n",
                      temperature / 1000.0f,
                      humidity / 1000.0f);
        } else {
            printf("error reading measurement\n");
        }

        /*
        * IMPLEMENT: get absolute humidity to enable humidity compensation
        * u32 ah = get_absolute_humidity(); // absolute humidity in mg/m^3
        * sgp_set_absolute_humidity(ah);
        */

        /* Persist the current baseline every hour */
        if (++i % 3600 == 3599) {
            err = sgp30_get_iaq_baseline(&iaq_baseline);
            if (err == STATUS_OK) {
                /* IMPLEMENT: store baseline to presistent storage */
            }
        }

        /* The IAQ measurement must be triggered exactly once per second (SGP30)
         * to get accurate values.
         */
        sleep(1); // SGP30
    }
    return 0;
}
