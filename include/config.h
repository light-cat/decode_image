#ifndef __CONFIG__
#define __CONFIG__ 

#define FBDEVICE "/dev/fb0"

// 触摸屏设备
#define TOUCHDEVICE "/dev/input/event1"


#ifdef DEBUG
#define debug(...)                                                      \
        {                                                               \
            fprintf(stderr, "[debug][%s:%s:%d] ",                     \
                    __FILE__, __FUNCTION__, __LINE__);                  \
            fprintf(stderr, __VA_ARGS__);                               \
        }
#else
#define debug(...)  
#endif

#endif // !


