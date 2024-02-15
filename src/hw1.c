#include "hw1.h"

unsigned char* payload_finder(unsigned char *packet, int length);
unsigned char bit_finder(unsigned char packet[], int section);

void print_packet_sf(unsigned char packet[])
{
    int length = bit_finder(packet, 5);
    unsigned char *payload = payload_finder(packet, length);

    printf("Source Address: %d /n", bit_finder(packet, 0));
    printf("Destination Address: %d /n",  bit_finder(packet, 1));
    printf("Source Port: %d /n" + bit_finder(packet, 2));
    printf("Destination Port: %d /n" , bit_finder(packet, 3));
    printf("Fragment Offset: %d /n" , bit_finder(packet, 4));
    printf("Packet Length: %d /n", length);
    printf("Maximum Hop Count: %d /n" , bit_finder(packet, 6));
    printf("Checksum: %d /n" , bit_finder(packet, 7));
    printf("Compression Scheme: %d /n" , bit_finder(packet, 8));
    printf("Traffic Class: %d /n" , bit_finder(packet, 9));
    printf("Payload: ");
    
    for(int *i = 0; *i < (length-15); i++){            // will this work?????
        printf("%d ", payload[*i++]);       // this is wrong???
    }

}

unsigned char bit_finder(unsigned char packet[], int section){
    unsigned char transfer_one = 0,transfer_two = 0, transfer_three = 0, transfer_four = 0;

    switch(section){
        case 0:                         //finds Source Address
            transfer_one = packet[0];
            transfer_two = packet[1];

            transfer_three = packet[2];
            transfer_four = packet[3];
            transfer_four &= 0xF0;
            return ((transfer_one + transfer_two+ transfer_three + transfer_four)>> 4);
        break;
        case 1:                       //finds Destination Address
            transfer_one = packet[3];
            transfer_two = packet[4];
            transfer_three = packet[5];
            transfer_four = packet[6];
            transfer_one &= 0x0F;
            return transfer_one + transfer_two+ transfer_three + transfer_four;
        break;
        case 2:                         //finds Source Port
            transfer_one = packet[7];
            transfer_one &= 0xF0; //is this needed ?????
            transfer_one >>=4;
            return transfer_one;
        break;
        case 3:                         //finds Destination Port
            transfer_one = packet[7];
            transfer_one &= 0x0F; //is this needed ?????
            return transfer_one;
        break;
        case 4:                         //finds Fragment Offset
            transfer_one = packet[8];
            transfer_two = packet[9];
            transfer_two >>= 2;
            return transfer_one+ transfer_two;
        break;
        case 5:                     //finds Packet Length
            transfer_one = packet[9];
            transfer_two = packet[10];
            transfer_three = packet[11];
            transfer_one  &= 0x03;
            return ((transfer_one + transfer_two+ transfer_three)>> 4);
        break;
        case 6:                     //finds Maximum Hop Count
            transfer_one = packet[11];
            transfer_two = packet[12];
            transfer_one &= 0x0F;
            return ((transfer_one + transfer_two) >> 7);

        break;
        case 7:                     //finds Checksum
            transfer_one = packet[12];
            transfer_two = packet[13];
            transfer_three = packet[14];
            transfer_one &= 0x7F;
            return transfer_one+transfer_two+transfer_three;
        break;
        case 8:                     //finds Compression Scheme
            transfer_one = packet[15];
            return (transfer_one >> 6);
        break;
        case 9:                     //finds Traffic Class
            transfer_one = packet[15];
            return (transfer_one & 0x3F);
        break;                     
        default:                    //default
            return 0;   
        break;
    }
}

unsigned char* payload_finder(unsigned char *packet, int length) {      //finds payload
    unsigned char *payload = malloc(length - 15); 
    int e = 16;
    for(int i = 0; i < (length - 15); i++){
        payload[i] = packet[e++];
    }
    return payload;
}


unsigned int compute_checksum_sf(unsigned char packet[])
{
    int length = bit_finder(packet,5), sum = 0, checklength = bit_finder(packet,7);
    for(int i = 0; i < 10; i++){
        if(i != 7)
        sum += bit_finder(packet, i);
    }
    unsigned char payload* = payload_finder(packet, length);
    
    for(int *i = 0; i<(length-15); i++){            // will this work?????
        sum += payload[i++];
    }

    return sum;
}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) 
{
    int check1 = 0, offsetcheck = 0;     //triggers loop not to overrite due to corrupted package

    for(int i = 0; i< packets_len; i++){
        if(bit_finder(packets[i],7) != compute_checksum_sf(packets[i])) //checks corrupted packets
        check1 = 1;


        int payleng = (bit_finder(packets[i],5)-15);

        for(int e = 0; e < array_len; e++){

        }

        check1 = 0;
        offsetcheck++;
    }
    
    return -1;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class)           
{
    


    (void)array;
    (void)array_len;
    (void)packets;
    (void)packets_len;
    (void)max_payload;
    (void)src_addr;
    (void)dest_addr;
    (void)src_port;
    (void)dest_port;
    (void)maximum_hop_count;
    (void)compression_scheme;
    (void)traffic_class;
    return -1;
}
