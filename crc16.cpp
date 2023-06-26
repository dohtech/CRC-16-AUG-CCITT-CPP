
#include <cstdint>
#include <iostream>
#include <iomanip>


void printArray(const uint8_t *array, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(array[i]) << " ";
    }
    std::cout << "(" << size << " bytes)" << std::endl;
}

void printCRC16(const uint16_t crc)
{
    std::cout << "CRC-16/AUG-CCITT: 0x" << std::setw(2) << std::setfill('0') << std::hex << crc << std::endl;
}


uint16_t calculateCRC16CCITT(const uint8_t* data, size_t length)
{
    uint16_t crc = 0x1D0F; // Initial value

    for (size_t i = 0; i < length; ++i)
    {
        crc ^= (uint16_t)data[i] << 8; // XOR with the current data byte

        for (uint8_t bit = 0; bit < 8; ++bit)
        {
            if (crc & 0x8000) // If the MSB is set
            {
                crc = (crc << 1) ^ 0x1021; // Perform the XOR and shift left
            }
            else
            {
                crc <<= 1; // Just shift left
            }
        }
    }

    return crc;
}

const uint16_t OPCODE_ECHO = 0x8305;
const uint16_t OPCODE_RXDM = 0x1234;
const uint16_t OPCODE_RESET = 0xE402;

uint16_t calcuateCRCwithOpCode(uint16_t opcode, const uint8_t* data, size_t lengthData)
{
    size_t lengthCrcArray = sizeof(opcode) + lengthData;
    uint8_t* dataForCrc = new uint8_t[lengthCrcArray];
    
    dataForCrc[0] = opcode >> 8;
    dataForCrc[1] = opcode & 0xFF;
    memcpy(&dataForCrc[2], data, lengthData);

    uint16_t crc = calculateCRC16CCITT(dataForCrc, lengthCrcArray);
    delete[] dataForCrc;

    return crc;
}

void testCrcWithOpcode()
{
    uint16_t opcode = OPCODE_ECHO;
    uint8_t inputData[] = { 0x12, 0x34 };
    
    size_t length = sizeof(inputData) / sizeof(inputData[0]);
    uint16_t crc = calcuateCRCwithOpCode(opcode, inputData, length);
    
    std::cout << "Opcode: 0x" << std::setw(2) << std::setfill('0') << std::hex << opcode << std::endl;
    std::cout << "Input data: "; 
    printArray(inputData, 2);
    printCRC16(crc);
}

int main()
{
    testCrcWithOpcode();
    
    return 0;
}
