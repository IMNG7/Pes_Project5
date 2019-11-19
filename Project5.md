PES Project 5
Readme File:
*************************************************************************************
PES PROJECT 5
Developed By: Nitik Satish Gupta and Rakesh Kumar
PES Project 5 is aimed to work upon the Custom UART module to communicate with the terminal. In addition, use circular buffer to implement the problem statement.
*************************************************************************************
Repo Contents
*************************************************************************************
PES_Project5(folder) -> The project files contain all the files and folder that are being used in the project for implementation.
Pes_Project5.pdf-> Contains all the .c files for plagiarism test.
Pes_Project5_Oscilloscope.pdf-> Contains the screenshots of the logic analyzer output for the I2C.
Pes_Project5.md-> The Readme file for the instructions.
Pes_Project5_FMEA.pdf -> FMEA analysis of the project
*************************************************************************************
Interesting Observations
*************************************************************************************
1. You cant do any testing or Print inside the UART interrupt or UART Polling mode, because it will be stuck inside a Recursive loop and will result in Stack-Overflow.
2. If there was a slight difference in the baudrate, then transmit might work fine, but receiving the character will be erroneous.
*************************************************************************************
Notes:
*************************************************************************************
* The program is compiled and executed using the MCUXpresso v11.0.1 IDE.
* There are 3 modes of operations: 1. Test Mode
				   2. Debug Mode
				   3. Normal Mode
* These can be executed using by selcting the different mode in the build option of the IDE.
* Each mode are operating according to the problem statement given in the project file.
* For display, Quickstart Panel->Miscellaneous->Quick Settings->SDK Debug Console-> UART Console. 
* For Application and the Echo mode, change the # define to Application mode or echo mode To check require functionality.
* By default polling mode is selcted, in the starting, we are giving the user to choose between the Interrupt mode and the Polling mode.
* FOr each mode, process might be same, but execution might be different, which can be seen in the timing of the process.
* For references different resources were used. Some of them include:

For UC Unit : https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/
	      
For UART: https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_8/Serial-Demo/src/UART.c

Special thanks to: Bruce Montgomery, Sorabh Gandhi, Khalid AlAwadhi, Shreya Chakraborty, Abhijeet Shrivastava, Sharan Arumugam.
