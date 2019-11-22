oss: Project5Main.c
	gcc Project5Main.c -o oss

userP: Project5UserP.c
	gcc Project5UserP.c -o userP

clean:
	rm oss
	rm userP
