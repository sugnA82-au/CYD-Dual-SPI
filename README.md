# CYD-Dual-SPI
Setup baselines for Kecheng/Dual-SPI CYD Dev Board

This repo is for my baseline config for a Kecheng/Dual-SPI varient CYD, which has differnt setup.ini & main.cpp to the standard online docs. 

Use of much bitbanging was used to map the spi bus to get reliable use of the display / touch. The touch is wired over the top of the SD card module, so holding it low while reading data from the touch XPT2046_Touchscreen.h library is needful. 


+++Layouts of Board for touch and display+++

Screen: Standard VSPI (CLK 14, MOSI 13, MISO 12, CS 15)

Touch: Separate HSPI (CLK 25, MOSI 32, MISO 39, CS 33)

Library: TFT_eSPI for display and XPT2046_Touchscreen with a custom SPIClass for touch
