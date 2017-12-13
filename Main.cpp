#include <due.h>
#include <UART.h>

typedef uart_t uart;
typedef output_t<D13> BOARD;
typedef output_t<A0> LED0;
typedef output_t<A1> LED1;
typedef output_t<A2> LED2;
typedef output_t<A3> LED3;

void setup()
{
    uart::setup<115200>();
    BOARD::setup();
    LED0::setup();
    LED1::setup();
    LED2::setup();
    LED3::setup();
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

void loop()
{
    static int i = 0;
    static toggler_t board(1);
    static toggler_t output0(3);
    static toggler_t output1(5);
    static toggler_t output2(7);
    static toggler_t output3(11);

    BOARD::set(board.toggle(i));
    LED0::set(output0.toggle(i));
    LED1::set(output1.toggle(i));
    LED2::set(output2.toggle(i));
    LED3::set(output3.toggle(i));

    delay(50);
    ++i;
    int c;
 
    while ((c = uart::get_char()) > 0)
    {
        uart::put_char('>');
        uart::put_char(c);
        uart::put_char('\n');
    }
}

