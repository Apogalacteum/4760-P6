oss: Project6Main.c
	gcc Project6Main.c -o oss

userP: Project6UserP.c
	gcc Project6UserP.c -o userP

clean:
	rm oss
	rm userP
