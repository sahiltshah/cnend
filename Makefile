all: editor r1 r2 r3 document n1 n2

editor: editor.c 
	gcc editor.c -o editor

r1: r1.c 
	gcc r1.c -o r1
	
r2: r2.c 
	gcc r2.c -o r2

r3: r3.c 
	gcc r3.c -o r3

document: document.c
	gcc document.c -o document

n1: n1.c
	gcc n1.c -o n1

n2: n2.c
	gcc n2.c -o n2

