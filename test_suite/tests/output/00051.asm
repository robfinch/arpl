 	.bss
	.align 4
_x:
	.type	_x,@object
	.size	_x,8
	.8byte	0
 
	.text

;{++ _main00051

	.sdreg	29
_main00051:
; switch(x)
  lda t0,_x[gp]
; case 0:
  bne t0,0,.00041
.00041:
; switch(x)
  lda t0,_x[gp]
; case 0:
  bne t0,0,.00044
; switch(x) {
  lda t0,_x[gp]
; case 0:
  bne t0,0,.00004
; goto next;
  bra .00002
.00004:
.00044:
; return 1;
  ldi a0,1
.00040:
  rts 
.00002:
; switch(x)
  lda t0,_x[gp]
; case 1:
  bne t0,1,.00050
; return 1;
  bra .00040
.00050:
; return 1;
  bra .00040
	.type	_main00051,@function
	.size	_main00051,$-_main00051


;--}

	.global	_main00051
	.extern	_start_data
	.extern	_start_bss
	.global	_x
	.extern	_start_rodata
