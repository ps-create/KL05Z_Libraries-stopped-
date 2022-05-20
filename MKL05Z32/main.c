#include "MKL05Z4.h"
#include "uart.h"

int main(void)
{
	uart_init(115200);
	uart_sendStr("testestets\n");
	while(1)
	{	
	}
}