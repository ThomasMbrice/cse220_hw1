#include "hw1.h"

unsigned int payload_finder(unsigned char *packet, int length);
unsigned int bit_finder(unsigned char packet[], int section);

void print_packet_sf(unsigned char packet[])
{
    int length = bit_finder(packet, 5);

    printf("Source Address: %d\n", bit_finder(packet, 0));
    printf("Destination Address: %d\n",  bit_finder(packet, 1));
    printf("Source Port: %d\n",  bit_finder(packet, 2));
    printf("Destination Port: %d\n" , bit_finder(packet, 3));
    printf("Fragment Offset: %d\n" , bit_finder(packet, 4));
    printf("Packet Length: %d\n", length);
    printf("Maximum Hop Count: %d\n" , bit_finder(packet, 6));
    printf("Checksum: %d\n" , bit_finder(packet, 7));
    printf("Compression Scheme: %d\n" , bit_finder(packet, 8));
    printf("Traffic Class: %d\n" , bit_finder(packet, 9));
    printf("Payload: ");

   for(int i = 16; i < length; i+=4){                   //prints payload
        if(i+4 < length)
        printf("%d ", ((packet[i] << 24) | packet[i+1] << 16 | packet[i+2] << 8 | packet[i+3]));
        else  
        printf("%d", ((packet[i] << 24) | packet[i+1] << 16 | packet[i+2] << 8 | packet[i+3])); // same print just without a space
    }
}

unsigned int bit_finder(unsigned char packet[], int section){
    unsigned char transfer_one = 0,transfer_two = 0, transfer_three = 0, transfer_four = 0, sum = 0;

    switch(section){
        case 0:                         //finds Source Address (correct)
            transfer_one = packet[0];
            transfer_two = packet[1];
            transfer_three = packet[2];
            transfer_four = packet[3];
            transfer_four &= 0xF0;
            transfer_four >>= 4;
            return ((transfer_one <<20) | (transfer_two <<12) | (transfer_three <<4) | (transfer_four));
        break;
        case 1:                       //finds Destination Address (correct)
            transfer_one = packet[3];
            transfer_two = packet[4];
            transfer_three = packet[5];
            transfer_four = packet[6];
            transfer_one &= 0x0F;
            return  ((transfer_one <<24) | (transfer_two <<16) | (transfer_three <<8) | (transfer_four));;
        break;
        case 2:                         //finds Source Port (correct)
            transfer_one = packet[7];
            transfer_one &= 0xF0; 
            transfer_one >>=4;
            return transfer_one;
        break;
        case 3:                         //finds Destination Port (correct)
            transfer_one = packet[7];
            transfer_one &= 0x0F; 
            return transfer_one;
        break;
        case 4:                        // frag offeseet (correct)
            transfer_one = packet[8];
            transfer_two = packet[9];
            transfer_two >>= 2;
            return (transfer_one<<6) | transfer_two;
        break;
        case 5:                     //finds Packet Length (correct)
            transfer_one = packet[9];
            transfer_two = packet[10];
            transfer_three = packet[11];
            transfer_one  &= 0x3;
            transfer_three  &= 0xF0;
            transfer_three >>= 4;
            return (transfer_one<<12) | (transfer_two<<4) | transfer_three;
        break;
        case 6:                     //finds Maximum Hop Count (correct)
            transfer_one = packet[11];
            transfer_two = packet[12];
            transfer_one &= 0xF;
            transfer_two = (transfer_two >> 7) & 0x1;

            return (transfer_one << 1) | transfer_two;

        break;
        case 7:                     //finds Checksum (correct)
            transfer_one = packet[12];
            transfer_two = packet[13];
            transfer_three = packet[14];
            transfer_one &= 0x7F;
            return (transfer_one << 16) | (transfer_two << 8) | transfer_three;
        break;
        case 8:                     //finds Compression Scheme (correct)
            transfer_one = packet[15];
            return transfer_one >> 6;
        break;
        case 9:                     //finds Traffic Class (correct)
            transfer_one = packet[15];
            return (transfer_one & 0x3F);
        break;                     
        default:                    //default
            return 0;   
        break;
    }
}

unsigned int compute_checksum_sf(unsigned char packet[])        //corrected
{
    int length = bit_finder(packet,5), sum = 0, payload = 0;//, checklength = bit_finder(packet,7);

    sum = bit_finder(packet, 0) + bit_finder(packet, 1) + bit_finder(packet, 2) + bit_finder(packet, 3)+
    bit_finder(packet, 4) + bit_finder(packet, 5) + bit_finder(packet, 6) + // deleted 7 
    bit_finder(packet, 8) + bit_finder(packet, 9);

    for(int i = 16; i < length; i+=4){
        sum += abs((packet[i] << 24) | packet[i+1] << 16 | packet[i+2] << 8 | packet[i+3]);
    }

    return sum % ((1<<23)-1);
}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) 
{
    int check1 = 0, offsetcheck = 0, offseter = 0, array_indexer = 0, intcount = 0;     //triggers loop not to overrite due to corrupted package

    for(int i = 0; i< packets_len; i++){
        
        for(int i = 0; i < packets_len; i++){               //finds correct packet in order 0-n
            if(bit_finder(packets[i],4) == offsetcheck)
                offseter = i;
        }
        int payloadleng = bit_finder(packets[offseter],5)-16, payloadindex= 0;
        unsigned int payload[bit_finder(packets[offseter],5)-16]; //creates payload array

        if(bit_finder(packets[i],7) != compute_checksum_sf(packets[i])){ //checks corrupted packets


        for(int i = 16; i < bit_finder(packets[offseter],5); i+=4){                   //creates values for  payload
        if(payloadindex == payloadleng)
        break;

        payload[payloadindex++] = ((packets[offseter][i] << 24) | packets[offseter][i+1] << 16 
        | packets[offseter][i+2] << 8 | packets[offseter][i+3]); 
        }

        for(int i = 0 ; i < sizeof(payload); i++){
            if(array_indexer == array_len){                                      //ends program
                return intcount;
            }
            else{
            array[array_indexer++] = payload[i];            ///???????????????? // copies correct payload to array
            intcount++;
            }
        }
        
        }
        else{
            array_indexer += (bit_finder(packets[offseter],5)-15);          // skips over values bec they are corrupted
        }
        offsetcheck++;
    }
    return intcount;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class){
   
    for(int i = 0; i < packets_len; i++)            //allocates memory for packets
        packets[i] = malloc(16+max_payload);
    
    for(int i = 0; i < packets_len; i++){           //need to generate Fragment Offset, Packet Length and Checksum
        packets[i][0] = (src_addr >> 28) & 0xFF;
        packets[i][1] = (src_addr >> 20) & 0xFF;
        packets[i][2] = (src_addr >> 12) & 0xFF;
        packets[i][3] = (src_addr >> 4) & 0xF0;                  // end src_addr
        packets[i][3] = (dest_addr >> 28) & 0x0F;                  // will this work out???
        packets[i][4] = (dest_addr >> 24) & 0xFF;
        packets[i][5] = (dest_addr >> 16) & 0xFF;
        packets[i][6] = (dest_addr >> 8) & 0xFF;                   // end dest_addr
        packets[i][7] = (src_port >> 4) & 0xF0;                    // end src_port
        packets[i][7] = (dest_port >> 4) & 0x0F;                    // end dest_port
        packets[i][8] = (i >> 14) & 0xFF;
        packets[i][9] = (i >> 6) & 0xFC;                        
        packets[i][9] = (i >> 2) & 0xFC;

                                                                                
    }
    



    
    return packets_len;
}
