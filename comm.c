#include "comm.h"

#include <stdint.h>
#include <string.h>
#include "slip.h"
#include "crc8.h"
#include "global_vars.h"

void send_ancwer(uint8_t ancwer)
{
	package[0] = ancwer;
	package[1] = crc8(package, 1);
	send_package(2);
}

void communication(void)
{
		int received = recv_package();
		if(received)
		{
			if (crc8(package, received))
			{
				send_ancwer(A_CRC_ERR);
				return;
			}
			
			switch(package[0])
			{
			case CR_REFERENCE_VALS:
			
					memcpy(package + 1, &reference_val, sizeof(reference_val));
					package[sizeof(reference_val) + 1] = crc8(package, sizeof(reference_val) + 1);
					send_package(sizeof(reference_val) + 2);
					break;
					
			case CW_REFERENCE_VALS:
					if (received < sizeof(reference_val) + 2)
					{
						send_ancwer(A_BAD_LEN);
						return;
					}
					
					memcpy(&reference_val, package + 1, sizeof(reference_val));
					send_ancwer(A_OK);
					break;
					
			case CR_INSTANT_VALS:
			
					memcpy(package + 1, &instant_val, sizeof(instant_val));
					package[sizeof(instant_val) + 1] = crc8(package, sizeof(instant_val) + 1);
					send_package(sizeof(instant_val) + 2);
					break;
					
			//case CW_INSTANT_VALS:
			//		if (received < sizeof(instant_val) + 2)
			//		{
			//			send_ancwer(A_BAD_LEN);
			//			return;
			//		}
			//		
			//		memcpy(&instant_val, &package + 1, sizeof(instant_val));					
			//		break;
					
			default:
				send_ancwer(A_NOT_IMPL);
			}
		}
}