


Talk with serial
================

Using a dumb terminal (minicom) 115200 board No parity 8 bits 1 stop
bit, no flow control (might have to exit minicom and start again for
the flow control setting to take).  What you type on the dumb terminal
comes back.

`chmod 666 /dev/ttyUSB0`

`picocom  /dev/ttyUSB0 --baud 115200 --parity n --databits 8 --stopbits 1 --flow n`


Note
====

* UART code based on
https://github.com/dwelch67/raspberrypi/tree/master/uart02

* Line editing based on
https://github.com/antirez/linenoise

Plan
====

### raspberry side: ###

* static implementation
* numbers
* arithmetics

### client side: ###

* <termios.h> musl
```
BRKINT
ICRNL
INPCK
ISTRIP
IXON
OPOST
CS8
ECHO
ICANON
IEXTEN
ISIG
VMIN
VTIME
TCSAFLUSH
tcsetattr
```
* <unistd.h> musl
```
write
read
STDIN_FILENO
STDOUT_FILENO
```
* <stdio.h> implement/delete!?
```
printf
snprintf
```
* <string.h> musl
```
strlen
strcat
strncpy
strcmp
```
