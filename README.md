LostKingdom
===========

Jon Ripley's Brainfuck Lost Kingdom running in your browser.

I picked up [jquery.terminal](https://github.com/jcubic/jquery.terminal) a copy of the brainfuck program LostKingdom. Then using my nice little [BF->asm.js](https://github.com/rdebath/Brainfuck/blob/master/bf2any/bf2asmjs.c) converter I generated a 5MB javascript file and promptly crashed Firefox.

With a bit of bashing and turning off of the "use asm" flag it now seems to work nicely.

<http://rdebath.github.io/LostKingdom>

This is how bf.js was generated, it also runs with node.

    bf2asmjs -b LostKng.b > tmp.js
    yui-compressor --nomunge --line-break 80 tmp.js -o bf.js
    rm tmp.js
    node bf.js
