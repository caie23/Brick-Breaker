.286
.model small
.stack 100h
.data
	studentID db "260899684$" ; change the content of the string to your studentID (do not remove the $ at the end)
	ball_x dw 160	 ; Default value: 160
	ball_y dw 144	 ; Default value: 144
	ball_x_vel dw 0	 ; Default value: 0
	ball_y_vel dw -1 ; Default value: -1 
	paddle_x dw 144  ; Default value: 144
	paddle_length dw 32 ; Default value: 32
	prev_score dw 0 ; score from last powerup
	power dw 0 ; 500 iterations of paddle extension
	laser_x dw -1
	laser_y dw -1
	laser_collision dw 1 ; 0:no collision

.code

; get the functions from the util_br.obj file (needs to be linked)
EXTRN setupGame:PROC, drawBricks:PROC, checkBrickCollision:PROC, sleep:PROC, decreaseLives:PROC, getScore:PROC, clearPaddleZone:PROC

; draw a single pixel specific to Mode 13h (320x200 with 1 byte per color)
drawPixel:
	color EQU ss:[bp+4]
	x1 EQU ss:[bp+6]
	y1 EQU ss:[bp+8]

	push	bp
	mov	bp, sp

	push	bx
	push	cx
	push	dx
	push	es

	; set ES as segment of graphics frame buffer
	mov	ax, 0A000h
	mov	es, ax


	; BX = ( y1 * 320 ) + x1
	mov	bx, x1
	mov	cx, 320
	xor	dx, dx
	mov	ax, y1
	mul	cx
	add	bx, ax

	; DX = color
	mov	dx, color

	; plot the pixel in the graphics frame buffer
	mov	BYTE PTR es:[bx], dl

	pop	es
	pop	dx
	pop	cx
	pop	bx

	mov	sp, bp
	pop	bp

	ret	6

drawBall:
	push	bp
	mov	bp, sp

	; erase the ball at current position
	push ball_y
	push ball_x
	push 00h
	call drawPixel

	; update ball position based on velocity
	mov ax, ball_x
	add ax, ball_x_vel
	mov ball_x, ax

	mov ax, ball_y
	add ax, ball_y_vel
	mov ball_y, ax

	push ball_y
	push ball_x
	push 0Fh
	call drawPixel

	mov	sp, bp
	pop	bp

	ret


checkWallCollision:
	x EQU ss:[bp+4]
	y EQU ss:[bp+6]
	
	push	bp
	mov	bp, sp

	push	bx

	; x=16 || x=303
	mov bx, x
	cmp bx, 16
	je nextToWall
	mov bx, x
	cmp bx, 303
	je nextToWall

	; y=32
	mov bx, y
	cmp bx, 32
	je nextToTop

	jmp collisionCaseOther

nextToWall: ; x=16 || x=303
	; y≥33
	mov bx, y
	cmp bx, 32
	je collisionCase3
	mov bx, y
	cmp bx, 33
	jge collisionCase1
	jmp collisionCaseOther

nextToTop: ; y=32
	; x<17 || x>302
	mov bx, x
	cmp bx, 17
	jl collisionCaseOther
	mov bx, x
	cmp bx, 302
	jg collisionCaseOther
	; 17≤x≤302
	jmp collisionCase2

collisionCase1:
	mov ax, 1
	jmp checkWallCollisionEnd

collisionCase2:
	mov ax, 2
	jmp checkWallCollisionEnd

collisionCase3: ; (x=16 || x=303) && y=32
	mov ax, 3
	jmp checkWallCollisionEnd

collisionCaseOther:
	mov ax, 0
	jmp checkWallCollisionEnd

checkWallCollisionEnd:	
	pop	bx

	mov	sp, bp
	pop	bp

	ret	4


handleLaserCollisions:
	push	bp
	mov	bp, sp

	push bx

	push laser_y
	push laser_x
	call checkWallCollision

	cmp ax, 0
	jne laserCollision

	mov ax, -1
	push ax 
	mov ax, 0
	push ax
	push laser_y 
	push laser_x 
	call checkBrickCollision

	cmp ax, 0
	jne laserCollision

	; no collision
	jmp handleLaserCollisionsEnd

laserCollision:
	mov ax, 1
	mov laser_collision, ax

handleLaserCollisionsEnd:
	pop	bx

	mov	sp, bp
	pop	bp

	ret



handleCollisions:
	push	bp
	mov	bp, sp

	push bx

	; collision with walls
	; change the velocity based on the return value
	push ball_y
	push ball_x
	call checkWallCollision

	mov bx, ax
	cmp bx, 1
	je invertX
	mov bx, ax
	cmp bx, 2
	je invertY
	mov bx, ax
	cmp bx, 3
	je invertXandY

	; collision with paddle
	call checkPaddleCollision
	mov bx, ax
	cmp bx, 1
	je paddleLeftCollision

	mov bx, ax
	cmp bx, 2
	je paddleMiddleCollision

	mov bx, ax
	cmp bx, 3
	je paddleRightCollision

	; collision with bricks
	push ball_y_vel 
	push ball_x_vel 
	push ball_y 
	push ball_x 
	call checkBrickCollision
	mov bx, ax
	cmp bx, 1
	je invertX
	mov bx, ax
	cmp bx, 2
	je invertY
	mov bx, ax
	cmp bx, 3
	je invertXandY

	; no collision
	jmp handleCollisionsEnd

invertX:
	mov ax, ball_x_vel
	neg ax
	mov ball_x_vel, ax
	jmp handleCollisionsEnd

invertY:
	mov ax, ball_y_vel
	neg ax
	mov ball_y_vel, ax
	jmp handleCollisionsEnd

invertXandY:
	mov ax, ball_x_vel
	neg ax
	mov ball_x_vel, ax
	mov ax, ball_y_vel
	neg ax
	mov ball_y_vel, ax
	jmp handleCollisionsEnd

paddleLeftCollision:
	mov ax, -1
	mov ball_x_vel, ax
	mov ball_y_vel, ax
	jmp handleCollisionsEnd

paddleMiddleCollision:
	mov ax, 0
	mov ball_x_vel, ax
	mov ax, -1
	mov ball_y_vel, ax
	jmp handleCollisionsEnd

paddleRightCollision:
	mov ax, 1
	mov ball_x_vel, ax
	mov ax, -1
	mov ball_y_vel, ax
	jmp handleCollisionsEnd

handleCollisionsEnd:
	pop	bx

	mov	sp, bp
	pop	bp

	ret


resetAfterBallLoss:
	; reset the ball position
	mov ax, 160
	mov ball_x, ax

	mov ax, 144
	mov ball_y, ax

	mov ax, 0
	mov ball_x_vel, ax

	mov ax, -1
	mov ball_y_vel, ax

	push ball_y
	push ball_x
	push 0Fh
	call drawPixel

	; reset the paddle position
	mov ax, 144
	mov paddle_x, ax
	mov ax, 32
	mov paddle_length, ax
	call drawPaddle

	call decreaseLives

	; return the number of lives that decreaseLives returned
	ret 


drawLine_hStart:
	color EQU ss:[bp+4]
	x1 EQU ss:[bp+6]
	y EQU ss:[bp+8]
	x2 EQU ss:[bp+10]

	push	bp
	mov	bp, sp
    
	push    ax
	push	bx
	push	cx
	push	dx

	sub sp, 4
    i equ word ptr ss:[bp-4]
	mov i, 1

drawLine_h:
	mov ax, x1
	add ax, i
	cmp ax, x2
    jge drawLine_hEnd

	mov bx, y
	push bx
	mov cx, x1
	add cx, i
	push cx
	mov dx, color
	push dx
	call	drawPixel

	inc i
	jmp drawLine_h

drawLine_hEnd:
	pop dx
    pop cx
	pop bx
	pop ax

	mov sp, bp
    pop bp

	ret 8


drawLine_vStart:
	color EQU ss:[bp+4]
	x EQU ss:[bp+6]
	y1 EQU ss:[bp+8]
	y2 EQU ss:[bp+10]

	push	bp
	mov	bp, sp
    
	push    ax
	push	bx
	push	cx
	push	dx

	sub sp, 4
    i equ word ptr ss:[bp-4]
	mov i, 1

drawLine_v:
	mov ax, y1
	add ax, i
	cmp ax, y2
    jge drawLine_vEnd

	mov bx, y1
	add bx, i
	push bx
	mov cx, x
	push cx
	mov dx, color
	push dx
	call	drawPixel

	inc i
	jmp drawLine_v

drawLine_vEnd:
	pop dx
    pop cx
	pop bx
	pop ax

	mov sp, bp
    pop bp

	ret 8



powerUp:
	mov ax, 64
	mov paddle_length, ax
	mov ax, 500
	mov power, ax
	call getScore
	mov prev_score, ax ; prev_score = curr_score
	jmp checkPower

shootLaser:
	mov ax, paddle_length
	mov bl, 2
	div bl
	mov cx, paddle_x
	add ax, cx
	mov laser_x, ax ; laser_x=paddle_x + (paddle_length/2)
	mov ax, 183
	mov laser_y, ax
	call getScore
	mov prev_score, ax
	mov ax, 0
	mov laser_collision, ax
	jmp checkPower

checkScore:
	mov bx, prev_score
	call getScore
	sub ax, bx ; ax = currScore - prevScore
	cmp ax, 50
	jge powerUp
	jmp checkPower

checkScoreLaser:
	mov bx, prev_score
	call getScore
	sub ax, bx ; ax = currScore - prevScore
	cmp ax, 50
	jge shootLaser
	jmp checkPower

drawPaddle:
    push bp
    mov bp, sp

    push bx
	push cx

	; check for key press
	mov     ah, 01h
    int     16h
    jz      checkPower
	mov ah, 0
	int 16h     
	mov ch, ah
	cmp ch, 30 ; 'A'
	je moveLeft
	; mov ch, ah
	cmp ch, 75 ; '<'
	je moveLeft
	; mov ch, ah
	cmp ch, 32 ; 'D'
	je moveRight
	; mov ch, ah
	cmp ch, 77 ; '>'
	je moveRight
	
	; mov ch, ah
	cmp ch, 2 ; 1 for power up
	je checkScore
	; mov ch, ah
	cmp ch, 3 ; 2 for shooting laser
	je checkScoreLaser
	jmp checkPower


moveLeft:
	mov ax, paddle_x
	sub ax, 8
	mov bx, ax
	cmp bx, 0
	jl setZero
	mov paddle_x, ax
	jmp checkPower

setZero:
	mov ax, 0
	mov paddle_x, ax
	jmp checkPower

moveRight:
	mov ax, 320
	mov bx, paddle_length
	sub ax, bx
	mov bx, ax ; bx = 320 - paddle_length
	mov ax, paddle_x
	add ax, 8
	mov cx, ax ; cx = paddle_x + 8
	cmp ax, bx
	jg setMost
	mov paddle_x, cx
	jmp checkPower

setMost:
	mov paddle_x, bx
	jmp checkPower

checkPower:
	mov ax, power
	cmp ax, 0
	jle powerDown
	; decrease the power
	mov ax, power
	sub ax, 1
	mov power, ax
	jmp checkLaser
	
powerDown:
	mov ax, 32
	mov paddle_length, ax

checkLaser:
	mov ax, laser_collision
	cmp ax, 1 ; collision
	je laserDisappear
	jmp drawLaser ; laser_collision == 0

drawLaser:
	; draw laser (if any)
	push laser_y
	push laser_x
	push 00h
	call drawPixel
	; move up 1 pixel
	mov ax, laser_y
	sub ax, 1
	mov laser_y, ax
	push laser_y ; y
	push laser_x ; x
	push 27h ; color
	call drawPixel

	jmp drawPaddleStart

laserDisappear:
	push laser_y ; y
	push laser_x ; x
	push 00h ; color
	call drawPixel
	mov ax, -1
	mov laser_x, ax
	mov laser_y, ax

	jmp drawPaddleStart

	
drawPaddleStart:
	
	call clearPaddleZone

	sub sp, 8
	; x0 = paddle_x
    x1 equ word ptr ss:[bp-4] 
	x2 equ word ptr ss:[bp-6] 
	x3 equ word ptr ss:[bp-8] 

	mov ax, paddle_length
	sub ax, 4
	mov bl, 2
	div bl ; al = ax / bx
	mov cx, paddle_x
	add cx, ax
	mov x1, cx ; x1 = x0 + (paddle_length-4)/2
	mov cx, x1
	add cx, 4
	mov x2, cx ; x2 = x1 + 4
	add cx, ax
	mov x3, cx ; x3 = x2 + (paddle_length-4)/2

	; draw first section
	push x1 ; x2
	push 184 ; y
	push paddle_x ; x1
	push 2Ch ; color
	call drawLine_hStart
   
	push x1 ; x2
	push 187 ; y
	push paddle_x ; x1
	push 2Ch ; color
	call drawLine_hStart

	push 187 ; y2
	push 184 ; y1
	mov ax, paddle_x
	add ax, 1
	push ax
	push 2Ch ; color
	call drawLine_vStart

	; second section
	push x2 ; x2
	push 184 ; y
	mov ax, x1
	sub ax, 1
	push ax ; x1
	push 2Dh ; color
	call drawLine_hStart

	push x2 ; x2
	push 187 ; y
	mov ax, x1
	sub ax, 1
	push ax ; x1
	push 2Dh ; color
	call drawLine_hStart

	; third section
	push x3 ; x2
	push 184 ; y
	mov ax, x2
	sub ax, 1
	push ax ; x1
	push 2Eh ; color
	call drawLine_hStart

	push x3 ; x2
	push 187 ; y
	mov ax, x2
	sub ax, 1
	push ax ; x1
	push 2Eh ; color
	call drawLine_hStart

	push 187 ; y2
	push 184 ; y1
	mov ax, x3
	sub ax, 1
	push ax ; x
	push 2Eh ; color
	call drawLine_vStart

	pop cx
    pop bx

	mov	sp, bp
	pop	bp
    
    ret


; get color of pixel at coordinate, reutrn in AL
getPixel:
    x1 EQU ss:[bp+4]
    y1 EQU ss:[bp+6]

    push bp
    mov bp, sp

    push bx
    push cx
    push dx
    push es

    ; set ES as segment of graphics frame buffer
    mov ax, 0A000h
    mov es, ax
    
    ; BX = ( y1 * 320 ) + x1
    mov bx, x1
    mov cx, 320
    xor dx, dx
    mov ax, y1
    mul cx
    add bx, ax

    ; DX = color
    mov dx, color

    ; get the color
    mov al, BYTE PTR es:[bx]

    pop es
    pop dx
    pop cx
    pop bx

    pop bp
    
    ret 4


checkPaddleCollision:
    push bp
    mov bp, sp

	push bx

	mov ax, ball_y
	cmp ax, 183
	jne paddleCollisionCase0 ; ball_y≠183

	; ball_y = 183
	mov ax, ball_y
	add ax, 1
	push ax ; y
	push ball_x ; x
	call getPixel
	mov bl, al
	cmp bl, 0
	je paddleCollisionCase0 ; not above the paddle

	mov bl, al
	cmp bl, 2Ch
	je paddleCollisionCase1 ; above left section

	mov bl, al
	cmp bl, 2Dh
	je paddleCollisionCase2 ; above middle section

	mov bl, al
	cmp bl, 2Eh
	je paddleCollisionCase3 ; above right section

	jmp checkPaddleCollisionEnd

paddleCollisionCase0:
	mov ax, 0
	jmp checkPaddleCollisionEnd

paddleCollisionCase1:
	mov ax, 1
	jmp checkPaddleCollisionEnd

paddleCollisionCase2:
	mov ax, 2
	jmp checkPaddleCollisionEnd

paddleCollisionCase3:
	mov ax, 3
	jmp checkPaddleCollisionEnd

checkPaddleCollisionEnd:
	pop bx

	mov	sp, bp
	pop	bp
    
    ret


start:
        mov ax, @data
        mov ds, ax
	
	push OFFSET studentID ; do not change this, change the string in the data section only
	push ds
	call setupGame ; change video mode, draw walls & write score, studentID and lives
	
	call drawBricks

main_loop:
	call sleep
	call drawPaddle
	call drawBall
	call handleCollisions
	call handleLaserCollisions
	mov ax, ball_y
	cmp ax, 199
	jle keypressCheck
	call resetAfterBallLoss
	cmp ax, 0
	jg keyboardInput
	; else wait for a keypress and exit
    mov ah, 0
    int 16h
	jmp exit
	
keypressCheck:
	mov ah, 01h ; check if keyboard is being pressed
	int 16h ; zero flag (zf) is set to 1 if no key pressed
	jz main_loop ; if zero flag set to 1 (no key pressed), loop back
keyboardInput:
	; else get the keyboard input
	mov ah, 00h
	int 16h

	cmp al, 1bh
	je exit

	jmp main_loop

exit:
        mov ax, 4f02h	; change video mode back to text
        mov bx, 3
        int 10h

        mov ax, 4c00h	; exit
        int 21h

END start

