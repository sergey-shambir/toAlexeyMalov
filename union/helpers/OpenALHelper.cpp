#include "OpenALHelper.h"

#include <AL/al.h>

void OpenALHelper::setListenerCoordinates(const vec3 &position,
                                          const vec3 &front,
                                          const vec3 &up)
{
    ALfloat listenerOri[6] = {(ALfloat)front.x, (ALfloat)front.y, (ALfloat)front.z,
                              (ALfloat)up.x, (ALfloat)up.y, (ALfloat)up.z};
    alListenerfv(AL_ORIENTATION, listenerOri);
    alListenerfv(AL_POSITION, position.convert<ALfloat>());
}

void OpenALHelper::setListenerCoordinates(const vec3d &position,
                                          const vec3d &front,
                                          const vec3d &up)
{
    ALfloat listenerOri[6] = {(ALfloat)front.x, (ALfloat)front.y, (ALfloat)front.z,
                              (ALfloat)up.x, (ALfloat)up.y, (ALfloat)up.z};
    alListenerfv(AL_ORIENTATION, listenerOri);
    alListenerfv(AL_POSITION, position.convert<ALfloat>());
}

void OpenALHelper::setListenerSpeed(const vec3 &velocity)
{
    alListenerfv(AL_VELOCITY, velocity.convert<ALfloat>());
}

void OpenALHelper::setListenerSpeed(const vec3d &velocity)
{
    alListenerfv(AL_VELOCITY, velocity.convert<ALfloat>());
}
