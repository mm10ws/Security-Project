#MayurM
#tested on thunder server
gatorcrypt: gatorcrypt.c gatordec.c
	gcc -o gatorcrypt gatorcrypt.c -L/usr/lib -lgcrypt -lgpg-error #link with libraries
	gcc -o gatordec gatordec.c -L/usr/lib -lgcrypt -lgpg-error

	
