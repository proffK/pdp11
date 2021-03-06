#define VRAM 0x4000
#define VRAM2 0x6000
#define VRAM_CONTROL 0xF000

void notmain(void) {
        volatile short* vram = VRAM;
        demo();
        return;
}

//void hor_line();

short abs(short x) {
        if (x > 0) {
                return x;
        }
        return 0 - x;
}

void set_bit(short* vram, unsigned short x, unsigned short y, unsigned short v) {
        short ind = y * 16 + (x / 16);
        short old = *(vram + ind);
        short new;
        if (v == 1) {
                new = old | (1 << (x & 0x000F));
        } else {
                new = old & ~(1 << (x & 0x000F));
        }
        *(vram + ind)  = new;
}
/*
void clr_bit(short* vram, short x, short y) {
        short ind = y * 16 + (x / 16);
        short old = *(vram + ind);
        short new = old & ~(1 << (x & 0x000F));
        *(vram + ind)  = new;
}
*/
void draw_line(short x1, short y1, short x2, short y2, short v) {
    const short deltaX = abs(x2 - x1);
    const short deltaY = abs(y2 - y1);
    const short signX = x1 < x2 ? 1 : -1;
    const short signY = y1 < y2 ? 1 : -1;
    short* vram = VRAM;

    //
    short error = deltaX - deltaY;
    //
    set_bit(vram, x2, y2, v);
    while(x1 != x2 || y1 != y2)
   {
        set_bit(vram, x1, y1, v);
        const short error2 = error * 2;
        //
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }

}

volatile void delay(short time) {
        while (time != 0) {
                short tmp1 = 0xFF;
                while (tmp1 != 0) tmp1--;
                time--;
        }
}

void vert_line(short* vram, short X, short v) {
        short i = 0;
        for (i = 0; i < 256; i++) {
                vram[i*16 + X] = v;
        }
}

void flip(short v) {
        short* vram_control = VRAM_CONTROL;
        *vram_control = v;
}

void demo() {
        short i = 0;
        short* vram = VRAM;
        volatile short* rand = 0xff00;
        while (1) {
                unsigned short x1 = *rand & 0x00FF;
                unsigned short x2 = *rand & 0x00FF;
                unsigned short y1 = *rand & 0x00FF;
                unsigned short y2 = *rand & 0x00FF;
                unsigned short v = *rand & 0x1;
                flip(1);
                draw_line(x1, y1, x2, y2, 1);
                //set_bit(vram, 15, y1, 1);
                //set_bit(vram, 14, y1, 1);
                //set_bit(vram, 0, y1, 1);
                //set_bit(vram, 1, y1, 1);
                flip(0);
                delay(400);
                //draw_line(120, 240, 200, 100, 1);
                //set_bit(vram, x, y, v);
        }
        //drawLine(0, 0, 200, 100);
/*        for (i = 0; i < 256; i++) {
                set_bit(vram, i, 0);
        }
*/
}
