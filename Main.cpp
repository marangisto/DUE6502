#include "board.h"
#include <delay.h>
#include <main.h>
#include <UART.h>
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
};

typedef byte_t<D7, D6, D5, D4, D3, D2, D1, D0> data;
typedef byte_t<A7, A6, A5, A4, A3, A2, A1, A0> lo_addr;
typedef byte_t<A15, A14, A13, A12, A11, A10, A9, A8> hi_addr;
typedef byte_t<MLB, MLB, MLB, RWB, SYNC, PHI1O, PHI2O, VBP> status;

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

static inline void print(int i, int base = 10)
{
    char buf[32];

    itoa(i, buf, base);
    print(buf);
}

static void print_state(int i)
{
    print(i);
    print("  0x");
    print(hi_addr::read(), 16);
    print(lo_addr::read(), 16);
    print(" ");
    print(data::read(), 16);
    print(" ");
    print(0x3f & status::read(), 2);
    println();
}

void loop()
{
    static int i = 0;
    static toggler_t output0(3);
    static toggler_t output1(5);
    static toggler_t output2(7);

    if (i == 0)     // make sure we have a fresh reset
    {
        RESB::clear();
        delay(1);
        RESB::set();
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
        delay(100);
        print_state(i);
        PHI2::toggle();
        ++i;
    }
}

