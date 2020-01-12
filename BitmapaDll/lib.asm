.data
	;matrix, wartosci rgb oraz kanał alfa mnozony przez 0
	;P1 P2 P3
	;P4 P5 P6
	;P7 P8 P9
	ALIGN 16 
	P1 WORD -1, -1, -1,  0
	ALIGN 16 
	P2 WORD  0,  0,  0,  0
	ALIGN 16 
	P3 WORD  1,  1,  1,  0

	ALIGN 16 
	P4 WORD -1, -1, -1,  0
	ALIGN 16 
	P5 WORD  1,  1,  1,  0
	ALIGN 16 
	P6 WORD  1,  1,  1,  0
	
	ALIGN 16 
	P7 WORD -1, -1, -1,  0
	ALIGN 16 
	P8 WORD  0,  0,  0,  0
	ALIGN 16 
	P9 WORD  1,  1,  1,  0

.code

;------------------------------------------------------------------------- 

DllMain PROC  ;punkt wejścia
	mov rax, 1
	ret
DllMain ENDP


; fun(*RGB1, *RGB2, width, height
; RCX - *RGB1
; RDX - *RGB2
; R8 - width
; R9 - height
MyProc1 PROC

	lea RDX, [RDX + R8*4 + 4]		;adres pixela w RGB2, krawedzie obrazow pomijane

	mov R10, 2 ;x counter
	mov R11, 2 ;y counter
	;zamiast sprawdzac do width-2, zaczynamy od 2 az do width


	RowY:
	
		RowX:

			pmovzxbw XMM1, QWORD PTR [RCX]				;przenies r, g, b, a do xmm1 rozszerzajac na liczby 16-bitowe
			pmullw XMM1, P1								;pomnoz przez czesc P1 maceirzy
			
			pmovzxbw XMM2, QWORD PTR [RCX + 4]			;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P2								;pomnoz przez czesc P2 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			pmovzxbw XMM2, QWORD PTR [RCX + 8]			;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P3								;pomnoz przez czesc P3 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1
			;------------------------------------
			pmovzxbw XMM2, QWORD PTR [RCX + R8*4]		;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P4								;pomnoz przez czesc P4 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			pmovzxbw XMM2, QWORD PTR [RCX + R8*4 + 4]	;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P5								;pomnoz przez czesc P5 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			pmovzxbw XMM2, QWORD PTR [RCX + R8*4 + 8]	;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P6								;pomnoz przez czesc P6 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1
			;------------------------------------
			pmovzxbw XMM2, QWORD PTR [RCX + R8*8]		;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P7								;pomnoz przez czesc P7 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			pmovzxbw XMM2, QWORD PTR [RCX + R8*8 + 4]	;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P8								;pomnoz przez czesc P8 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			pmovzxbw XMM2, QWORD PTR [RCX + R8*8 + 8]	;przenies r, g, b, a do xmm2 rozszerzajac na liczby 16-bitowe
			pmullw XMM2, P9								;pomnoz przez czesc P9 maceirzy
			paddw XMM1, XMM2							;dodaj wynik do XMM1

			packuswb XMM1,XMM1							;zmiana nasycenia				
			movd DWORD PTR [RDX], XMM1					;zapisz otrzymana wartosc do piksela RGB2


			inc R10		;x++
			add RCX, 4 ;przejście do następnego pixela rgb
			add RDX, 4 ;przejście do następnego pixela rgb2

			cmp R10, R8	;if x > width => break
		
		jne RowX

		mov R10, 2	;x = 0

		inc R11		;y++
		add RCX, 8 ;przejście do następnego pixela rgb pomijając "krawędzie"
		add RDX, 8 ;przejście do następnego pixela rgb pomijając "krawędzie"
		
		cmp R11, R9 ;if y > height => break

	jne RowY


	ret
MyProc1 endp

END 
;-------------------------------------------------------------------------
