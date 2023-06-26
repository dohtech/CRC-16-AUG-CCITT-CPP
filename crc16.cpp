
#include <cstdint>
#include <iostream>
#include <iomanip>

uint16_t calculateCRC16(const uint16_t *data, size_t length)
{
    const uint16_t polynomial = 0x1021;
    uint16_t crc = 0x1d0f;

    for (size_t i = 0; i < length; ++i)
    {
        crc ^= (data[i] << 8);

        for (int j = 0; j < 8; ++j)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ polynomial;
            else
                crc = (crc << 1);
        }
    }

    return crc;
}

uint16_t calculateCRC16(const uint16_t opcode, const uint16_t *data, size_t length)
{
    // Create a new array that consists of the opcode and the data
    uint16_t *newData = new uint16_t[length + 1];
    newData[0] = opcode;
    for (size_t i = 0; i < length; ++i)
    {
        newData[i + 1] = data[i];
    }
    uint16_t crc = calculateCRC16(newData, length + 1);

    delete[] newData;
    return crc;
}

void printArray(const uint16_t *array, size_t size)
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

void testWithOpCode()
{
    uint16_t opcode = 0x8305;
    uint16_t inputData[] = {0x1234};//, 0x34 };
    size_t size = sizeof(inputData) / sizeof(inputData[0]);

    printArray(inputData, size);

    uint16_t result = calculateCRC16(opcode, inputData, size);

    //std::cout << "CRC-16/AUG-CCITT: 0x" << std::setw(2) << std::setfill('0') << std::hex << result << std::endl;
    printCRC16(result);
}


// CRC-16-CCITT function
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

void testFromChatGpt()
{
    uint8_t data[] = { 0x83, 0x05, 0x12, 0x34 }; // Input data
    size_t length = sizeof(data) / sizeof(data[0]);

    uint16_t checksum = calculateCRC16CCITT(data, length);
    printCRC16(checksum);
}

int main()
{
    uint16_t data = 0x1234;

    // Create a uint8_t array that consists of OPCODE_ECHO and data
    uint8_t *sendData = new uint8_t[4];
    sendData[0] = OPCODE_ECHO >> 8;
    sendData[1] = OPCODE_ECHO & 0xFF;
    sendData[2] = data >> 8;
    sendData[3] = data & 0xFF;

    uint16_t crcNew = calculateCRC16CCITT(sendData, 4);


    testFromChatGpt();
    // uint16_t inputData[] = { 1 };

    uint16_t inputData[] = {0x83, 0x05, 0x12, 0x34};
    size_t size = sizeof(inputData) / sizeof(inputData[0]);




    printArray(inputData, size);

    // uint16_t opcode[] = { 0x83, 0x05 };
    // uint16_t inputData[] = {0x12, 0x34 };

    size_t length = sizeof(inputData) / sizeof(inputData[0]);
    uint16_t result = calculateCRC16(inputData, length);

    printCRC16(result);

    //std::cout << "CRC-16/AUG-CCITT: 0x" << std::hex << result << std::endl;

    //testWithOpCode();


    return 0;
}
