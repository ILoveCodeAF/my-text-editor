				if(ksym == XK_s && 
						ControlMask == 
						(e->state & ~(Mod2Mask|(1<<13))))//~ignoremod))
					printf("hello\n");

lack of functions:
	- copy/cut/paste
	- move input cursor

how to use

./te <filename> : to open file
press esc: to save and close file