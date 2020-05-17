# Memory Read/Write Manager  

My aim was to concieve a way to write/read a rom memory (AT28C64B) with an arduino uno/nano. The program is divided in two parts, one is the client and the other part is the reader/writer.


## Client Part  

The cient is coded in Python and uses the Serial library. It is not a big deal, it just write any command and add ```$``` at the beginning and ```;``` at the end. It is a light protocol, allowing the arduino to be sure that a command has been send only if the message begin by a ```$```.  
The user should only use two commands:  
* ```r:xxxxx``` with  ```xxxxx``` between 0 and 8191 representing the address in decimal form.  
* ```w:xxxxx:yyyyy``` with  ```xxxxx``` between 0 and 8191 representing the address in decimal form and  ```yyyyy``` between 0 and 255 representing the value we want to store in decimal form.

  
  
## Arduino Part  

On the arduino, an handler is always waiting for a command and parse it.  
It then process to read or write the value from the rom memory.


## Register 8 bits ?  

In order to reduce the numbers of bits used and be able to do the operations with an arduino nano, I chose to store the address in two shift registers of 8 bits each. The address is send in a serial way and then presented at each output of the shift registers.
Is it possible to bypass them and directly output the address from the arduino. 

## Example

The final interface looks similar to this:  
```shell
Enter a command: 
w:2:4
b'Write at address: 2 value : 4\r\n'
Enter a command: 
r:2
b'READ address: 2 => value : 4\r\n'
Enter a command: 
r:2
b'READ address: 2 => value : 4\r\n'
Enter a command: 
r:1
b'READ address: 1 => value : 9\r\n'
Enter a command: 
r:1
b'READ address: 1 => value : 9\r\n'
Enter a command: 
stop
b'Error: unrecognized command\r\n'
bye.....
```