#include "rpi-pwm.h"

#include <stdio.h>

int main() {
	rpi_pwm_ctl_t test;
	rpi_pwm_ctl_t *ptr;
	rpi_pwm_ctl_reg_t testreg;
	uint32_t raw_val = 0;
	ptr = (rpi_pwm_ctl_t*)&raw_val;
	test.MODE1 = 1;
	test.PWEN1 = 1;
	//printf("Hello %x \n",(unsigned int)test);
	printf("size %d \n",(int)sizeof(rpi_pwm_ctl_t));
	printf("size reg %d \n",(int)sizeof(rpi_pwm_ctl_reg_t));
	printf("MODE1 %x \n",test.MODE1);
	printf("PWEN1 %x \n",test.PWEN1);
	ptr->MODE1 = 1;
	ptr->SBIT1 = 1;
	ptr->Reserved1 = 45;
	printf("raw_val %x \n",raw_val);
	testreg.val = 0;
	testreg.fields.MODE1 = 1;
	testreg.fields.SBIT1 = 1;
	testreg.fields.POLA1 = 1;
	printf("reg val %x \n",	testreg.val);
}
