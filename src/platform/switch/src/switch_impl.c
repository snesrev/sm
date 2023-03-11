#include "switch_impl.h"

#include <switch.h>

void SwitchImpl_Init() {
    appletInitializeGamePlayRecording();
    appletSetGamePlayRecordingState(true);
    appletSetFocusHandlingMode(AppletFocusHandlingMode_NoSuspend);
    socketInitializeDefault();
    nxlinkStdio();
}

void SwitchImpl_Exit() {
    appletSetGamePlayRecordingState(false);
    socketExit();
}