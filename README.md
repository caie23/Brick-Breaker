# Brick-Breaker
A simple brick breaker game mimicking the ”Break Out” game in x86 assembly.

DOSBox setting - in temrinal:
```
cd  ~/Library/Preferences/
cat DOSBox\ 0.74-3-3\ Preferences
```
```
mount c: ~
c:
cd Downlo~1
cd COMP308
cd TASM
cd BIN
```

run bricks.asm on DOSBox, with bricks.asm and UTIL_BR.OBJ in /bin:
```
tasm bricks
tlink bricks util_br
bricks
```

Compile using the command: $ gcc coverart.c -lglut -lGL -o coverart.

Execute ./coverart on open gpu.

To see the game scene written with OpenGL, compile and run coverart.c on a Trottier computer:
compile with command `g++ coverart.c -lglut -lGL -o coverart`, then run `./coverart`.
