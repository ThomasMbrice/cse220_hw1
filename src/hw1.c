#include "hw1.h"

unsigned int bit_finder(unsigned char packet[], int section);
void bubble_sort(unsigned int fragoffset_grabber[], unsigned int len);

void print_packet_sf(unsigned char packet[])
{   //done
    unsigned int length = bit_finder(packet, 5);

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

   for(unsigned int i = 16; i < length; i+=4){                   //prints payload
        if(i+4 < length)
        printf("%d ", ((packet[i] << 24) | packet[i+1] << 16 | packet[i+2] << 8 | packet[i+3]));
        else  
        printf("%d\n", ((packet[i] << 24) | packet[i+1] << 16 | packet[i+2] << 8 | packet[i+3])); // same print just without a space
    }
}

unsigned int bit_finder(unsigned char packet[], int section){
    unsigned char transfer_one = 0,transfer_two = 0, transfer_three = 0, transfer_four = 0;

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
{   //done
    unsigned int length = bit_finder(packet,5), sum = 0;//, checklength = bit_finder(packet,7);

    sum += (unsigned int)bit_finder(packet, 0);
    sum += (unsigned int)bit_finder(packet, 1);
    sum += (unsigned int)bit_finder(packet, 2);
    sum += (unsigned int)bit_finder(packet, 3);
    sum += (unsigned int)bit_finder(packet, 4);          
    sum += (unsigned int)bit_finder(packet, 5);
    sum += (unsigned int)bit_finder(packet, 6);
    //skip checksum
    sum += (unsigned int)bit_finder(packet, 8);
    sum += (unsigned int)bit_finder(packet, 9);

    for(unsigned int i = 16; i < length; i+=4){
        sum += (unsigned int)abs((packet[i] << 24) | (packet[i+1] << 16) 
        | (packet[i+2] << 8) | packet[i+3]);
    }


     
    unsigned int checksum = sum % ((1 << 23) - 1);
    return checksum;
}


unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) 
{   //done
    unsigned int intcount = 0;                                      //counts ints

    for(unsigned int e = 0; e < packets_len; e++){   

        if(bit_finder(packets[e],7) == compute_checksum_sf(packets[e])){ //checks corrupted packets
            
            unsigned int rewrite_index = (bit_finder(packets[e], 4)/4);        //uses offset to calculate where in array should write

            for(unsigned int i = 16; i < bit_finder(packets[e],5); i+=4){    //creates values for  payload
            if(rewrite_index >= array_len){                                 //stops overflow
                break;
            } 
            
            array[rewrite_index++] = ((packets[e][i] << 24) | packets[e][i+1] << 16 //writes to array
            | packets[e][i+2] << 8 | packets[e][i+3]); 
        
            intcount++;
            }   
        }
    }
    return intcount;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class){
   /*
    for(int i = 0; i < packets_len; i++)            //allocates memory for packets
        packets[i] = malloc(15+max_payload);
    
    */
   unsigned int packetsadded = 0, array_indexer = 0, payloads_added = 0;

    for(unsigned int i = 0; i < packets_len; i++){           //need to generate Fragment Offset, Packet Length and Checksum
    payloads_added = 0;
    packets[i] = malloc(16+max_payload);
        packets[i][0] = (src_addr >> 20) & 0xFF;
        packets[i][1] = (src_addr >> 12) & 0xFF;
        packets[i][2] = (src_addr >> 4) & 0xFF;
        packets[i][3] = ((src_addr & 0xF) << 4) | ((dest_addr >> 24) & 0xF);                  // end src_addr
        packets[i][4] = (dest_addr >> 16) & 0xFF;                 
        packets[i][5] = (dest_addr >> 8) & 0xFF;
        packets[i][6] = dest_addr & 0xFF;
        packets[i][7] = (src_port << 4) | (dest_port & 0xF);                    // end src_port | end dest_port
        packets[i][12] =  ((maximum_hop_count & 0x1) << 7);
        //length checkd at end
        //skip checksum for end
        packets[i][15] = ((compression_scheme & 0x3) << 6 ) | (traffic_class & 0x3F);    //end compression_scheme traffic_class
          
        for(unsigned int e = 16; e < 16+max_payload; e+=4){          //enter payload
                if(array_indexer >= array_len){
                    break;
                }
                packets[i][e] = array[array_indexer] >> 24 & 0xFF;
                packets[i][e+1] = array[array_indexer] >> 16 & 0xFF;
                packets[i][e+2] = array[array_indexer] >> 8 & 0xFF;
                packets[i][e+3] = array[array_indexer++] & 0xFF;
                payloads_added++;
            }
        
        //printf("payloads %d \n", payloads_added);
        packets[i][8] = (((i*max_payload)) >> 6) & 0xFF ;                         //start offset
        packets[i][9] = ((i*max_payload) & 0x3F) <<2 | (((16+(payloads_added*4)) >> 12) & 0x3);      //end offset
        packets[i][10] = ((16+(payloads_added*4)) >> 4);                   // adds length
        packets[i][11] = ((16+(payloads_added*4)) << 4) | ((maximum_hop_count >> 1) & 0xF); //end length



        unsigned int checksum = compute_checksum_sf(packets[i]);        //checksum time
        packets[i][12] = packets[i][12] | ((checksum >> 16) & 0x7F);     //end hop count
        packets[i][13] = (checksum >> 8) & 0xFF;
        packets[i][14] = checksum & 0xFF;
        

    if(payloads_added > 0){
    packetsadded++;        
    }                                
     //iterate numpackets added
    //print_packet_sf(packets[i]);   
    //printf("\n \n \n") ;                 
    }
    return packetsadded;
    
    
}

/*
unsigned int j = 16;
    while(j < (16+max_payload)){
        if(array_indexer > array_len){
            break;
        }
        for (int k = 24; k >= 0; k -= 8) {
            packets[i][j++] = (array[array_indexer] >> k) & 0xFF;
        }
    // printf("added value %d  and index of array %d \n", array[array_indexer], array_indexer);
    array_indexer++;
    payloads_added++;
    }
*/
