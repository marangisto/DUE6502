#include "board.h"
#include <delay.h>
#include <main.h>
#include <UART.h>
#include <watchdog.h>
#include <stdlib.h>

typedef uart_t uart;

void setup()
{
    uart::setup<115200>();

    LED0::setup();
    LED1::setup();
    LED2::setup();

    // configure pins
    D0::setup();
    D1::setup();
    D2::setup();
    D3::setup();
    D4::setup();
    D5::setup();
    D6::setup();
    D7::setup();
    A0::setup();
    A1::setup();
    A2::setup();
    A3::setup();
    A4::setup();
    A5::setup();
    A6::setup();
    A7::setup();
    A8::setup();
    A9::setup();
    A10::setup();
    A11::setup();
    A12::setup();
    A13::setup();
    A14::setup();
    A15::setup();
    BE::setup();
    MLB::setup();
    RWB::setup();
    RDY::setup<pull_up>();    // using pull-up as this pin is potenially an output on 6502
    SYNC::setup();
    SOB::setup();
    PHI2::setup();
    PHI1O::setup();
    PHI2O::setup();
    RESB::setup();
    IRQB::setup();
    NMIB::setup();
    VBP::setup();

    // initial values

    SOB::set();             // set overflow
    IRQB::set();            // interrupt request
    NMIB::set();            // non-maskable interrupt
    RESB::set();            // don't hold reset
    PHI2::clear();          // start with low clock
    BE::set();              // bus enable
}

template<class T7, class T6, class T5, class T4, class T3, class T2, class T1, class T0>
struct byte_t
{
    static uint8_t read()
    {
        return (static_cast<uint8_t>(T7::get()) << 7)
             | (static_cast<uint8_t>(T6::get()) << 6)
             | (static_cast<uint8_t>(T5::get()) << 5)
             | (static_cast<uint8_t>(T4::get()) << 4)
             | (static_cast<uint8_t>(T3::get()) << 3)
             | (static_cast<uint8_t>(T2::get()) << 2)
             | (static_cast<uint8_t>(T1::get()) << 1)
             | (static_cast<uint8_t>(T0::get()) << 0)
             ;
    }

    static void write(uint8_t x)
    {
        T7::set(x & (1 << 7));
        T6::set(x & (1 << 6));
        T5::set(x & (1 << 5));
        T4::set(x & (1 << 4));
        T3::set(x & (1 << 3));
        T2::set(x & (1 << 2));
        T1::set(x & (1 << 1));
        T0::set(x & (1 << 0));
    }

    static void enable_output()
    {
        T7::enable_output();
        T6::enable_output();
        T5::enable_output();
        T4::enable_output();
        T3::enable_output();
        T2::enable_output();
        T1::enable_output();
        T0::enable_output();
    }

    static void disable_output()
    {
        T7::disable_output();
        T6::disable_output();
        T5::disable_output();
        T4::disable_output();
        T3::disable_output();
        T2::disable_output();
        T1::disable_output();
        T0::disable_output();
    }
};

typedef byte_t<D7, D6, D5, D4, D3, D2, D1, D0> data;
typedef byte_t<A7, A6, A5, A4, A3, A2, A1, A0> lo_addr;
typedef byte_t<A15, A14, A13, A12, A11, A10, A9, A8> hi_addr;

static const char *status_str()
{
    static char buf[8];

    buf[0] = PHI2::get() ? 'C' : '-';
    buf[1] = PHI1O::get() ? '1' : '-';
    buf[2] = PHI2O::get() ? '2' : '-';
    buf[3] = SYNC::get() ? 'S' : '-';
    buf[4] = RWB::get() ? 'R' : 'W';
    buf[5] = VBP::get() ? '-' : 'V';
    buf[6] = MLB::get() ? '-' : 'M';
    buf[7] = '\0';

    return buf;
}

struct toggler_t
{
    toggler_t(int n): m_n(n), m_b(false) {}

    bool toggle(int i)
    {
        if (i % m_n == 0)
            m_b = !m_b;
        return m_b;
    }

    int m_n;
    bool m_b;
};

static inline int strlen(const char *p)
{
    int l = 0;

    while (*p++)
        l++;
    return l;
}

static inline void print(const char *s)
{
    for (const char *p = s; *p; ++p)
        uart::put_char(*p);
}

static inline void println(const char *s = "")
{
    print(s);
    uart::put_char('\n');
}

static inline void print(int i, int base = 10, int w = 0)
{
    char buf[32];

    itoa(i, buf, base);

    int l = strlen(buf);

    while (l++ < w)
        uart::put_char('0');

    print(buf);
}

static inline void print_state(int i)
{
    print(i);
    print(" ");
    print(status_str());
    print("  0x");
    print(hi_addr::read(), 16);
    print(lo_addr::read(), 16);
    print(" ");
    print(data::read(), 16);
    println();
}

static const uint16_t ram_size = 1024;
static const uint16_t ram_start = 0x0;
static const uint16_t rom_size = 256;
static const uint16_t rom_start = 0xff00;

static uint8_t ram[ram_size], rom[rom_size];

static const uint8_t NOP = 0xea;
static const uint8_t BRA = 0x80;
static const uint8_t JMP = 0x6c;

static const uint8_t prog0[] = { NOP, NOP, NOP, NOP, NOP, BRA, static_cast<uint8_t>(-7) };

static void initialize_memory(const uint8_t *pgm = 0, size_t pgm_len = 0)
{
    for (size_t i = 0; i < ram_size; ++i)
        ram[i] = 0;

    for (size_t i = 0; i < pgm_len; ++i)
        ram[i] = pgm[i];

    for (size_t i = 0; i < rom_size; ++i)
        rom[i] = 0;
}

static inline uint8_t read_memory(uint16_t addr)
{
    if (addr >= ram_start && addr < ram_start + ram_size)
        return ram[addr - ram_start];
    if (addr >= rom_start && addr < rom_start + rom_size)
        return rom[addr - rom_start];
    print("invalid write address: 0x");
    print(addr, 16, 4);
    println();
    return 0;
}

static inline void write_memory(uint16_t addr, uint8_t x)
{
    if (addr >= ram_start && addr < ram_start + ram_size)
        ram[addr - ram_start] = x;
    else
    {
        print("invalid write address: 0x");
        print(addr, 16, 4);
        println();
    }
}

void loop()
{
    watchdogReset();
 
    static int i = 0;
    static toggler_t output0(3);
    static toggler_t output1(5);
    static toggler_t output2(7);


    if (i == 0)     // make sure we have a fresh reset
    {
        RESB::clear();
        delay(1);
        RESB::set();
        initialize_memory(prog0, sizeof(prog0));
    }

    LED0::set(output0.toggle(i));
    LED1::set(output1.toggle(i));
    LED2::set(output2.toggle(i));

    int c;
    bool step = false;
 
    while ((c = uart::get_char()) > 0)
        step = true;

    if (step)
    {
        PHI2::toggle();
        delay(1);
 
        if (PHI2::get())            // rising edge
        {
            uint16_t addr = (hi_addr::read() << 8) | lo_addr::read();

            print("a = 0x");
            print(addr, 16, 4);
            print(" ");

            if (RWB::get())         // reading data
            {
                uint8_t x = read_memory(addr);
                data::enable_output();
                data::write(x);
                print("r 0x");
                print(x, 16, 2);
            }
            else                    // writing data
            {
                uint8_t x = data::read();
                write_memory(addr, x);
                print("w 0x");
                print(x, 16, 2);
            }
            print(" ");
            print(status_str());
            println();
        }
        else                      // falling edge
        {
            data::disable_output();
        }
       // print_state(i);
        ++i;
    }
}

