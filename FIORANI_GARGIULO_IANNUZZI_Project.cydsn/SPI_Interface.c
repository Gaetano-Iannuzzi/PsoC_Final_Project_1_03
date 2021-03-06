/**
 * @file SPI_Interface.c
 * @brief Functions required to interface with the SPI MASTER (SPIM).
 * 
 * This source code file contains macros and functions to interface
 * with the SPIM_1 and SPIM_2 Masters (SPIM) of the PSoC.
 * 
 *@authors Simone Fiorani, Alessandra Gargiulo, Gaetano Iannuzzi
 *@date 2020
*/


#include "SPI_Interface.h"

#ifndef SLAVE_CS_Write
    #define SLAVE_CS_Write      CS_1_Write
    #define SLAVE_CS_Write_2    CS_2_Write
#endif

/*
* @brief Full-Duplex, Single-Operation 1-Byte Trade
*/
uint8_t SPI_Interface_tradeByte(uint8_t byte) {
	
	/* Enable the Slave */
	SLAVE_CS_Write(0);
	
	/* Load the TX Buffer */
	SPIM_1_WriteTxData(byte);
	
	/* Wait for TX */
    while( !(SPIM_1_ReadTxStatus() & SPIM_1_STS_BYTE_COMPLETE) );
	
	/* Read the RX Buffer */
	uint8_t data = SPIM_1_ReadRxData();
	
	/* Disable the Slave */
	SLAVE_CS_Write(1);
	
	return data;
    
}


/*
* @brief RX-only, Dual-Operation 1-Byte READ
*/
uint8_t SPI_Interface_ReadByte(uint8_t byteTX) {
	
	/* Enable the Slave */
	SLAVE_CS_Write(0);
	
	/* Load the TX Buffer */
	SPIM_1_WriteTxData(byteTX);
	
	/* SPI Dummy Byte */
	SPIM_1_WriteTxData(SPI_DUMMY_BYTE);
	
	/* Wait for TX/RX */
    while(!(SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE));
	
	/* Read the RX Buffer */
	SPIM_1_ReadRxData();
	uint8_t byteRX = SPIM_1_ReadRxData();
	
	/* Disable the Slave */
	SLAVE_CS_Write(1);
	
	return byteRX;	
	
}


///*
//* @brief Full-Duplex, Multi-Byte Trade (RX/TX)
//*/
//void SPI_Interface_Multi_Trade(uint8_t* dataTX, uint8_t* dataRX, uint8_t nBytes) {
//	
//    
//	
//}

/*
* @brief RX-only, Dual-Operation Multi-Byte READ/WRITE
*/
void SPI_Interface_Multi_RW(uint8_t* dataTX, uint8_t bytesTX, uint8_t* dataRX, uint8_t bytesRX) {
	
	/* Enable the Slave */
    SLAVE_CS_Write(0);
        
    int8_t count = bytesTX, index = 0;
    	
    /* Transmit Data */
    while ( count > 0 ) {
            
    	/* Load the TX buffer with Data*/
        SPIM_1_PutArray(&dataTX[index*SPI_TxBufferSize], (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
        /* Wait for TX */
        while( !(SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE) );
        
        /* Update count */
        count -= SPI_TxBufferSize;
        index++;
           
    }
        
    /* Clear the RX Buffer */
    SPIM_1_ClearFIFO();
    SPIM_1_ClearRxBuffer();
        
    /* Init the Dummy TX Buffer */
    uint8_t dummyTX[SPI_RxBufferSize];
    memset(dummyTX, SPI_DUMMY_BYTE, SPI_RxBufferSize);
        
    /* Update count for RX */
    count = bytesRX;
    index = 0;
        
    /* Get the RX Data */
    while ( count > 0 ) {
         
        /* Load the TX buffer with Dummy Bytes*/
        SPIM_1_PutArray(dummyTX, (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
        /* Wait for TX */
        while( !(SPIM_1_ReadTxStatus() & SPIM_1_STS_SPI_DONE) );
        /* Read the RX Buffer */
        for( uint8_t j = 0; j < (count > SPI_TxBufferSize ? SPI_TxBufferSize : count); j++ ) {
            dataRX[j + index*SPI_RxBufferSize] = SPIM_1_ReadRxData();   
        }
        
        count -= SPI_RxBufferSize;
        index++;
        
    }
        
    /* Disable the Slave */
    SLAVE_CS_Write(1);
    	
    /* Clear */
    SPIM_1_ClearFIFO();
    SPIM_1_ClearRxBuffer();
    SPIM_1_ClearTxBuffer();

}

/********** ACCELEROMETER SPI FUNCTION **********/

uint8_t ACC_SPI_Interface_tradeByte(uint8_t byte) {
	
	/* Enable the Slave */
	SLAVE_CS_Write_2(0);
	
	/* Load the TX Buffer */
	SPIM_2_WriteTxData(byte);
	
	/* Wait for TX */
    while( !(SPIM_2_ReadTxStatus() & SPIM_1_STS_BYTE_COMPLETE) );
	
	/* Read the RX Buffer */
	uint8_t data = SPIM_2_ReadRxData();
	
	/* Disable the Slave */
	SLAVE_CS_Write_2(1);
	
	return data;
    
}

uint8_t ACC_SPI_Interface_ReadByte(uint8_t byteTX) {
	
	/* Enable the Slave */
	SLAVE_CS_Write_2(0);
	
	/* Load the TX Buffer */
	SPIM_2_WriteTxData(byteTX);
	
	/* SPI Dummy Byte */
	SPIM_2_WriteTxData(SPI_DUMMY_BYTE);
	
	/* Wait for TX/RX */
    while(!(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE));
	
	/* Read the RX Buffer */
	SPIM_2_ReadRxData();
	uint8_t byteRX = SPIM_2_ReadRxData();
	
	/* Disable the Slave */
	SLAVE_CS_Write_2(1);
	
	return byteRX;	
	
}

void ACC_SPI_Interface_Multi_RW(uint8_t* dataTX, uint8_t bytesTX, uint8_t* dataRX, uint8_t bytesRX) {
	
	/* Enable the Slave */
    SLAVE_CS_Write_2(0);
        
    int8_t count = bytesTX, index = 0;
    	
    /* Transmit Data */
    while ( count > 0 ) {
            
    	/* Load the TX buffer with Data*/
        SPIM_2_PutArray(&dataTX[index*SPI_TxBufferSize], (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
        /* Wait for TX */
        while( !(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE) );
        
        /* Update count */
        count -= SPI_TxBufferSize;
        index++;
           
    }
        
    /* Clear the RX Buffer */
    SPIM_2_ClearFIFO();
    SPIM_2_ClearRxBuffer();
        
    /* Init the Dummy TX Buffer */
    uint8_t dummyTX[SPI_RxBufferSize];
    memset(dummyTX, SPI_DUMMY_BYTE, SPI_RxBufferSize);
        
    /* Update count for RX */
    count = bytesRX;
    index = 0;
        
    /* Get the RX Data */
    while ( count > 0 ) {
         
        /* Load the TX buffer with Dummy Bytes*/
        SPIM_2_PutArray(dummyTX, (count > SPI_TxBufferSize ? SPI_TxBufferSize : count));
        /* Wait for TX */
        while( !(SPIM_2_ReadTxStatus() & SPIM_2_STS_SPI_DONE) );
        /* Read the RX Buffer */
        for( uint8_t j = 0; j < (count > SPI_TxBufferSize ? SPI_TxBufferSize : count); j++ ) {
            dataRX[j + index*SPI_RxBufferSize] = SPIM_2_ReadRxData();   
        }
        
        count -= SPI_RxBufferSize;
        index++;
        
    }
        
    /* Disable the Slave */
    SLAVE_CS_Write_2(1);
    	
    /* Clear */
    SPIM_2_ClearFIFO();
    SPIM_2_ClearRxBuffer();
    SPIM_2_ClearTxBuffer();

}

/* [] END OF FILE */


