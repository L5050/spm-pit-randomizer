#pragma once

#include <common.h>
#include <spm/npcdrv.h>
#include <spm/mario.h>

CPP_WRAPPER(spm::temp_unk)

UNKNOWN_FUNCTION(luigi_boss_set_stats);
UNKNOWN_FUNCTION(bowser_spawn_fire);
UNKNOWN_FUNCTION(luigi_superjump_atk);
EVT_DECLARE_USER_FUNC(uranoko_idk, 4)

EVT_DECLARE(kameks_unk7)
EVT_DECLARE(dark_broom_kamek_unk7)
EVT_DECLARE(skellobits_unk7)
EVT_DECLARE(skellobits_unk2)
EVT_DECLARE(skellobits_unk8)
EVT_DECLARE(lakitu_onspawn_child)
EVT_DECLARE(goomba_unk2_child)
UNKNOWN_FUNCTION(shellPart1UpdateFunc);
UNKNOWN_FUNCTION(shellPart2UpdateFunc);
EVT_DECLARE(dimen_unk_fight_script_1)
EVT_DECLARE(dimen_unk_fight_script_2)
EVT_DECLARE(dimen_unk_fight_script_3)
EVT_DECLARE_USER_FUNC(dimen_determine_move_pos, 3)

// template id
EVT_DECLARE_USER_FUNC(lakitu_spawn_spiny, 1)

// tribe id, num
EVT_DECLARE_USER_FUNC(lakitu_count_spinies, 2)

s32 ninjoe_bomb_thing(s32 param_1, spm::npcdrv::NPCPart * param_2, s32 param_3, int defenseType, s32 power, u32 param_6);

// should go in npcdrv.h, but compiler doesn't like recursive includes
s32 npcHandleHitXp(spm::mario::MarioWork * marioWork, spm::npcdrv::NPCEntry * npcEntry, s32 killXp, s32 unk_variant);

CPP_WRAPPER_END()
