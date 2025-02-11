/*MIT License

Copyright (c) 2020 Nyameaama Gambrah

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include"temp_access_mem.h" 
/*
    PTAM implementation is a series of statically allocated 1D arrays
    which correspond to their respective data type
    Each data pair has two values
*/

//Arrays declared
//!!! OVERFLOW UNLIKELY AS THERE IS A SINGLE SENSOR REQUEST EVERY CYCLE !!!

static char* floatingP[length];
static char* eight_bit[length];
static char* thirtytwo_bit[length];
static char* character[length];

uint8_t floatingP_LEN = 0;
uint8_t eight_bit_LEN = 0;
uint8_t thirtytwo_bit_LEN = 0;
uint8_t character_LEN = 0;

//Method to add data to program temp access memory
//Returns the size of data sent
//+3 Overload
uint8_t PTAM::PTAM_ADD_BASE_DOUBLE(char* baseID,double data){
    //Convert data to char*
    char* converted;
    itoa(data,converted,10);
    //Add to floating point array
    floatingP[floatingP_LEN - 2] = baseID;
    floatingP[floatingP_LEN - 1] = converted;
    floatingP_LEN += 2;
    return sizeof(data);
}

uint8_t PTAM::PTAM_ADD_BASE_8(char* baseID,uint8_t data){
    //Convert data to char*
    char* converted;
    itoa(data,converted,10);
    //Add to eight_bit array
    eight_bit[eight_bit_LEN - 2] = baseID;
    eight_bit[eight_bit_LEN - 1] = converted;
    eight_bit_LEN += 2;
    return sizeof(data);
}

uint8_t PTAM::PTAM_ADD_BASE_32(char* baseID,uint32_t data){
    //Convert data to char*
    char* converted;
    itoa(data,converted,10);
    //Add to thirtytwo_bit array
    thirtytwo_bit[thirtytwo_bit_LEN - 2] = baseID;
    thirtytwo_bit[thirtytwo_bit_LEN - 1] = converted;
    thirtytwo_bit_LEN += 2;
    return sizeof(data);
}

uint8_t PTAM::PTAM_ADD_BASE_CHAR(char* baseID,char* data){
    //Add to character array
    character[character_LEN - 2] = baseID;
    character[character_LEN - 1] = data;
    character_LEN += 2;
    return sizeof(data);
}

//Method to retrieve data from program temp access memory
//Returns queried data
//!!!Disclaimer!!!
//Unable to use templates so used messy workaround
//!!!!!!!!!!!!!!!!!!
char* PTAM::PTAM_RETRIEVE_BASE_CHAR(char* baseID){
    char* temp_data;
    for(uint8_t i = 0; i < length;i+=2){
        if(character[i] == baseID){
            //ID found, get data in neighbouring index
            temp_data = character[i + 1];
            break;
        }else{continue;}
    }
    //Convert to appropriate format
    return temp_data;
}

uint8_t PTAM::PTAM_RETRIEVE_BASE_8(char* baseID){
    char* temp_data;
    for(uint8_t i = 0; i < length;i+=2){
        if(eight_bit[i] == baseID){
            //ID found, get data in neighbouring index
            temp_data = eight_bit[i + 1];
            break;
        }else{continue;}
    }
    //Convert to appropriate format
    uint8_t AP_format = atoi(temp_data);
    return AP_format;
}

uint32_t PTAM::PTAM_RETRIEVE_BASE_32(char* baseID){
    char* temp_data;
    for(uint8_t i = 0; i < length;i+=2){
        if(thirtytwo_bit[i] == baseID){
            //ID found, get data in neighbouring index
            temp_data = thirtytwo_bit[i + 1];
            break;
        }else{continue;}
    }
    //Convert to appropriate format
    uint32_t AP_format = atoi(temp_data);
    return AP_format;
}

double PTAM::PTAM_RETRIEVE_BASE_DOUBLE(char* baseID){
    char* temp_data;
    for(uint8_t i = 0; i < length;i+=2){
        if(floatingP[i] == baseID){
            //ID found, get data in neighbouring index
            temp_data = floatingP[i + 1];
            break;
        }else{continue;}
    }
    //Convert to appropriate format
    double AP_format = atoi(temp_data);
    return AP_format;
}

//Method to clear entire PTAM data for next program cycle
/*void PTAM::PTAM_CLEAR_ALL(){
    //Set all values to 0

}*/
