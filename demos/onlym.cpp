#include <miniaudio.h>
#include <iostream>


int mian()
{
    ma_engine engine;
    if(ma_engine_init(NULL,&engine) != MA_SUCCESS)
    {
        std::cout<<"false to init"<<std::endl;
        return 0;
    }
    ma_result result = ma_engine_play_sound(
        &engine,
        "resources/textures/a.wav",
        NULL
    )
    
    return 0;
}