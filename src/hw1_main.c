#include <stdio.h>
#include "hw1.h"
/*
int  main() {
    #include "packetize_array_sf_args03.h"
	printf("\n %d \n ", packetize_array_sf(array, sizeof(array) / sizeof(array[0]), actual_packets,
			sizeof(actual_packets) / sizeof(actual_packets[0]), max_payload, src_addr, dest_addr, src_port, dest_port, 
			maximum_hop_count, compression_scheme, traffic_class));
	for (unsigned int i = 0; i < num_expected_packets; i++)
        free(actual_packets[i]);
    return 0;
  
}
*/

int main() {
    unsigned char packet[] = {
        0x0e,0x4e,0x1c,0x00,0xd5,0x9f,0x80,0x87,0x54,0xe0,0x02,0x49,0x81,0x2a,0x25,0x1a,0x00,0x01,0x62,0xbc,0x00,0x0c,0xa2,0x4e,0xff,0xff,0xdc,0x5d,0x00,0x00,0x1d,0xd2,0x00,0x06,0xc8,0x1c,
		};
    print_packet_sf(packet);
	printf(" %d \n", compute_checksum_sf(packet)); 
    return 0;
}

