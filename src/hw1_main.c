#include <stdio.h>
#include "hw1.h"
    #include "reconstruct_array_sf_args02.h"


int  main() {
	
     #include "reconstruct_array_sf_args05.h"
    unsigned int elements_written = reconstruct_array_sf((unsigned char **)packets, sizeof(packets)/sizeof(packets[0]), 
        reconstructed_array, sizeof(reconstructed_array) / sizeof(reconstructed_array[0]));
   

	

	

    (void)elements_written;
    (void)expected_num_elements;
	(void)expected_array;
	
    return 0;
}

/*
int main() {
    unsigned char packet[] = {
        0x0e,0x4e,0x1c,0x00,0xd5,0x9f,0x80,0x87,0x54,0xe0,0x02,0x49,0x81,0x2a,0x25,0x1a,0x00,0x01,0x62,0xbc,0x00,0x0c,0xa2,0x4e,0xff,0xff,0xdc,0x5d,0x00,0x00,0x1d,0xd2,0x00,0x06,0xc8,0x1c,
		};
    print_packet_sf(packet);
	printf(" %d \n", compute_checksum_sf(packet)); 
    return 0;
}
*/
