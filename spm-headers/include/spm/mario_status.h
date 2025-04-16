#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::mario_status)

USING(spm::effdrv::EffEntry)
USING(spm::mario::MarioStatus)
USING(wii::mtx::Vec3)

// Should be converted to an enum called MarioStatusType eventually
#define STATUS_POISION 0x1
#define STATUS_SLOW 0x2
#define STATUS_NO_SKILLS 0x4
#define STATUS_NO_JUMP 0x8
#define STATUS_FLIPPED_CONTROLS 0x10
#define STATUS_HALF_DAMAGE 0x20
#define STATUS_DOUBLE_ATTACK 0x40
#define STATUS_ELECTRIFIED 0x80
#define STATUS_HP_REGEN 0x100
#define STATUS_BARRIER 0x200
#define STATUS_FAST_FLOWER 0x400
#define STATUS_SLOW_FLOWER 0x800
#define STATUS_COIN_FLOWER 0x1000
#define STATUS_PAL_PILLS 0x4000
#define STATUS_GHOST_SHROOM 0x8000
#define STATUS_DANGEROUS_DELIGHT 0x10000

void marioStatusApplyStatuses(s32 status, s32 lv);

void marioStatusEntry(s32 type, uint flags, void * userWork, void * mainFunc, void * deleteFunc, void * mapChangeFunc);

void statusIconEffectSetPos(double x, double y, double z, EffEntry * effEntry);

EffEntry * statusIconEffect(double x, double y, double z, u32 unk);

s32 statusFlippedControlsMain(MarioStatus * status, Vec3 * position, int type);

s32 statusPoisonMain(MarioStatus * status, Vec3 * position, int type);

s32 statusNoSkillsMain(MarioStatus * status, Vec3 * position, int type);

void marioCheckStatusPauseMot();

// more

CPP_WRAPPER_END()