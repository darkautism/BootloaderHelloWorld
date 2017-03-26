
enum KYBRD_CTRL_STATS_MASK {

        KYBRD_CTRL_STATS_MASK_OUT_BUF   =       1,              //00000001
        KYBRD_CTRL_STATS_MASK_IN_BUF    =       2,              //00000010
        KYBRD_CTRL_STATS_MASK_SYSTEM    =       4,              //00000100
        KYBRD_CTRL_STATS_MASK_CMD_DATA  =       8,              //00001000
        KYBRD_CTRL_STATS_MASK_LOCKED    =       0x10,           //00010000
        KYBRD_CTRL_STATS_MASK_AUX_BUF   =       0x20,           //00100000
        KYBRD_CTRL_STATS_MASK_TIMEOUT   =       0x40,           //01000000
        KYBRD_CTRL_STATS_MASK_PARITY    =       0x80            //10000000
};

char keycode[256]={
        0,0,'1','2','3','4','5','6','7','8',
	'9','0',0,0,0,0,'q','w','e','r',
	't','y','u','i','o','p','[',']','\n',0,
	'a','s','d','f','g','h','j','k','l',';',
	''',
	
};


static inline uint8_t inb(uint16_t port) {
        uint8_t ret;
        asm volatile ( "inb %1, %0"
                : "=a"(ret)
                : "Nd"(port) );
        return ret;
}


