#include "project.h"

/* Defines for DMA_1 */
#define DMA_1_BYTES_PER_BURST 4
#define DMA_1_REQUEST_PER_BURST 1
#define DMA_1_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

// Declare buffers
uint32 buffer1[16];
uint32 buffer2[16];

void DMA_Config(void); // Place the DMA function prototype here

int main(void)
{
    CyGlobalIntEnable;
    UART_1_Start();
    CyDelay(100);
    for (int i = 0; i < 16; i++) // Fill source buffer
    {
        buffer1[i] = 0x12345678;
    }
    for (int i = 0; i < 16; i++) // Clear destination buffer
    {
        buffer2[i] = 0x00000000;
    }
    DMA_Config(); // Call the DMA configuration function
    //
    // Send contents of buffer2 to the console once every second
    //
    for (;;)
    {
        UART_1_WriteTxData(0x0d);
        CyDelay(5);
        UART_1_PutString("Contents of buffer2   ");
        CyDelay(5);
        for (int i = 0; i < 16; i++)
        {
            UART_1_WriteTxData(buffer2[i]);
            CyDelay(10);
        }
        CyDelay(1000);
    }
}

// Place our DMA initialisation code in this fumction
void DMA_Config()
{
    /* Variable declarations for DMA_1 */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[1];
    /* DMA Configuration for DMA_1 */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST,
                                     HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    DMA_1_TD[0] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_1_TD[0], 64, CY_DMA_DISABLE_TD, CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)buffer1), LO16((uint32)buffer2));
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    CyDmaChEnable(DMA_1_Chan, 1);
}
