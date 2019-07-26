				if(ksym == XK_s && 
						ControlMask == 
						(e->state & ~(Mod2Mask|(1<<13))))//~ignoremod))
					printf("hello\n");

lack of function:
- copy/cut/paste
- move input cursor