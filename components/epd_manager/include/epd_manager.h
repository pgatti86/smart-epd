/*
 * epd_manager.h
 *
 *  Created on: Sep 22, 2019
 *      Author: paolo
 */

#ifndef COMPONENTS_EPD_MANAGER_INCLUDE_EPD_MANAGER_H_
#define COMPONENTS_EPD_MANAGER_INCLUDE_EPD_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif
	void epd_manager_init();

	void epd_manager_write_on_lcd(char *str);

#ifdef __cplusplus
}
#endif


#endif /* COMPONENTS_EPD_MANAGER_INCLUDE_EPD_MANAGER_H_ */
