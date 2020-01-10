;------------------------------------------------------------------------- 
.data
	permmask2 dword 0, 4, 0, 0,      0, 0, 0, 0
.code

DllMain PROC		;punkt wejœcia
	mov rax, 1
	ret
DllMain ENDP

;------------------------------------------------------------------------- 
; INPUT DATA
;------------------------------------------------------------------------- 
;rgb,		-zawiera wartoœci rgb wczytanego obrazu
;rgb2,		-zawiera wartoœci rgb obrazu wynikowego
;matrix,	-dane z macierzy filtruj¹cej
;bih->width,bih->height		-informacje na temat wysokoœci i szerokoœci obrazu 
;SumMatrix	-suma wartoœci macierzy filtruj¹cej
;-------------------------------------------------------------------------
;Filtration (RGB*,RGB*,matrix*,int,int,int)
;rgb*=rcx
;b=[RCX]		RDX		RDI R8		R9	r11
;g=[RCX+1]
;r=[RCX+2]
;rcx,rdx,rdi,r8,r9,r11,rip,rsp,rbp,efl
;-------------------------------------------------------------------------
MyProc1 PROC

pxor mm6,mm6 ;wyzerowanie
;moovq mm0,[rcx]
;movq mm1,[rdx]
;movq mm1,byte ptr [rcx]




;movq mm0, [rcx] ;za³adowanie tablicy
;punpcklbw mm0,mm6 ;rozpakowanie z zerami do 16 bitów
;pmullw mm0,rdi ;mno¿enie przez macierz
;packuswb mm0,mm0;spakowanie do bitu


ret
MyProc1 endp

END 
;-------------------------------------------------------------------------