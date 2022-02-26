/*
-----------------------------------------------------------------
19-05-2019 calibration.c 
Caclulate the offset of the sensor values at the initial state 
The MPU-6065 working at the DMP modes
FIFO is updated at a fixed rate of 100Hz
Take 256 samples, shift 8 bits,  
caculate the offset of sp, sq, sr; sax, say, saz; phi, theta, psi
sp, sq, sr angular rate gyro sensor output
sax, say, saz accelerometer sensor output 
phi, theta, psi true drone state variable 
uint8_t sensor_fifo_count 
scale sp sq sr : 16.4 
      sax, say, saz : 16384
      euler angle: 32767 ～ 180

Author: Ji Hang      
-----------------------------------------------------------------
*/

#include "calibration.h"
#include "in4073.h"

void static_calibration(void)
{    
    int32_t sp_offset_calc = 0;
    int32_t sq_offset_calc = 0;
    int32_t sr_offset_calc = 0;
    int32_t sax_offset_calc = 0;
    int32_t say_offset_calc = 0;
    int32_t saz_offset_calc = 0;
    int32_t sphi_offset_calc = 0;
    int32_t stheta_offset_calc = 0;
    int32_t spsi_offset_calc = 0;

    data_cnt = 0;

    sp_offset = 0;
    sq_offset = 0;
    sr_offset = 0;

    sax_offset = 0;
    say_offset = 0;
    saz_offset = 0;

    sphi_offset = 0;
    stheta_offset = 0;
    spsi_offset= 0;

    while (data_cnt < SAMPLE)
    {
        if(check_sensor_int_flag())
        {        
            if(dmp_used)
            {
                get_dmp_data();

                sphi_offset_calc += sphi;
                stheta_offset_calc += stheta; 
                spsi_offset_calc += spsi;
            }
		    else
            {
                get_raw_sensor_data();                
            }             

            sp_offset_calc += sp;
            sq_offset_calc += sq;
            sr_offset_calc += sr;

            sax_offset_calc += sax;
            say_offset_calc += say;
            saz_offset_calc += saz;

            data_cnt++;
        }

    }

    //calculate the offset 
    sp_offset = (int16_t)(sp_offset_calc>>DIV);
    sq_offset = (int16_t)(sq_offset_calc>>DIV);
    sr_offset = (int16_t)(sr_offset_calc>>DIV);
            
    sax_offset = (int16_t)(sax_offset_calc>>DIV);
    say_offset = (int16_t)(say_offset_calc>>DIV);
    saz_offset = (int16_t)(saz_offset_calc>>DIV); 
    
    if(dmp_used)
    {
        sphi_offset = (int16_t)(sphi_offset_calc>>DIV);
        stheta_offset = (int16_t)(stheta_offset_calc>>DIV);
        spsi_offset = (int16_t)(spsi_offset_calc>>DIV);
    }

    // Test Calib
    // printf("sp: %d\n",sp-sp_offset);
    // printf("sq: %d\n",sq-sq_offset);
    // printf("sr: %d\n",sr-sr_offset);
    // printf("sax: %d\n",sax-sax_offset);
    // printf("say: %d\n",say-say_offset);
    // printf("saz: %d\n",saz-saz_offset);
    // printf("phi: %d\n",sphi-sphi_offset);
    // printf("theta: %d\n",stheta-stheta_offset);
    // printf("psi: %d\n",spsi-spsi_offset);
    printf("sp: %d\n",sp);
    printf("sq: %d\n",sq);
    printf("sr: %d\n",sr);
    printf("sax: %d\n",sax);
    printf("say: %d\n",say);
    printf("saz: %d\n",saz);
    printf("phi: %d\n",sphi);
    printf("theta: %d\n",stheta);
    printf("psi: %d\n",spsi);   

}
