#pragma once

#include <common.h>
#include <spm/mario.h>

CPP_WRAPPER(spm::mario_sbr)

void marioAdjustMoveDir();
bool marioCheck1HeldFor3();
f32 revise360(f32);
void toMovedir2(f32, f32);

CPP_WRAPPER_END()