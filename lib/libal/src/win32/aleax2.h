#include "alMain.h"
#include "al.h"
#include "alc.h"

ALenum eax2BufferGet(ALuint property, ALuint source, ALvoid *value, ALuint size, ALint iSWMixer);
ALenum eax2ListenerGet(ALuint property, ALsource *pALSource, ALvoid *value, ALuint size, ALint iSWMixer);

ALenum eax2BufferSet(ALuint property, ALuint source, ALvoid *value, ALuint size, ALint iSWMixer);
ALenum eax2ListenerSet(ALuint property, ALsource *pALSource, ALvoid *value, ALuint size, ALint iSWMixer);
