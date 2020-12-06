#ifndef COMPONENTS_PAGE_MANAGER_INCLUDE_PAGE_MANAGER_H_
#define COMPONENTS_PAGE_MANAGER_INCLUDE_PAGE_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

    const int epd_page_count = 4;
    enum EpdPage {                                     
    	PAGE_SPLASH,
        PAGE_ENROLLMENT,
    	PAGE_CLOCK,
        PAGE_WEATHER
	};

    void page_manager_init();

#ifdef __cplusplus
}
#endif

#endif