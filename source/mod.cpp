#include "mod.h"
#include "patch.h"

#include <common.h>
#include <util.h>
#include <cutscene_helpers.h>
#include <evtpatch.h>
#include <evt_cmd.h>
#include <spm/rel/aa1_01.h>
#include <spm/rel/mi4.h>
#include <spm/rel/relocatable_module.h>
#include <spm/temp_unk.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/eff_small_star.h>
#include <spm/eff_spm_confetti.h>
#include <spm/eff_zunbaba.h>
#include <spm/eff_spm_recovery.h>
#include <spm/eff_spm_spindash.h>
#include <spm/evt_cam.h>
#include <spm/evt_case.h>
#include <spm/evt_dimen.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_frame.h>
#include <spm/evt_guide.h>
#include <spm/evt_hit.h>
#include <spm/evt_img.h>
#include <spm/evt_item.h>
#include <spm/evt_map.h>
#include <spm/evt_mario.h>
#include <spm/evt_msg.h>
#include <spm/evt_mobj.h>
#include <spm/evt_npc.h>
#include <spm/evt_offscreen.h>
#include <spm/evt_paper.h>
#include <spm/evt_pouch.h>
#include <spm/evt_shop.h>
#include <spm/evt_snd.h>
#include <spm/evt_seq.h>
#include <spm/evt_sub.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/fontmgr.h>
#include <spm/hitdrv.h>
#include <spm/hud.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/map_data.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/mario_pouch.h>
#include <spm/mario_status.h>
#include <spm/mobjdrv.h>
#include <spm/mot_damage.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/npc_tile.h>
#include <spm/npcdrv.h>
#include <spm/parse.h>
#include <spm/seq_mapchange.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/winmgr.h>
#include <spm/rel/dan.h>
#include <spm/rel/machi.h>
#include <wii/os/OSError.h>
#include <wii/cx.h>
#include <wii/gx.h>
#include <wii/tpl.h>
#include <wii/wpad.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <cstdio>

namespace mod
{
    bool youSuck = false;

    const char musicOption[] =
        "<system>"
        "Would you like to play\n"
        "custom 8-bit Pit music?\n"
        "(Made by Tater-Tot Tunes)\n"
        "<o>";

    const char yesNoSelect_NoByDefault[] =
        "<select 1 -1 160 40>\n"
        "Yes\n"
        "No";

    const char yesNoSelect_YesByDefault[] =
        "<select 0 -1 160 40>\n"
        "Yes\n"
        "No";

    const char quickstartText[] =
        "<system>"
        "Do you want to quickstart\n"
        "the Pit Randomizer or\n"
        "create a new save file?\n"
        "<o>";

    const char houraiFirstUse[] =
        "<system>"
        "Hourai Doll has taken effect!\n"
        "HP has been fully restored,\n"
        "but the Blessing has cleared.\n"
        "<k>\n<p>\n"
        "You now have an Affliction:\n"
        "Phoenix's Tail. Attack is\n"
        "halved; HP increased by 10.\n"
        "<k>\n<p>\n"
        "Hourai Doll's effects are no\n"
        "longer active, so be careful!\n"
        "<k>\n<p>\n"
        "These effects will remain\n"
        "until you exit the Pit or\n"
        "the Affliction has cleared.\n"
        "<k>";

    const char houraiWearOff[] =
        "<system>"
        "The temporary HP boost and\n"
        "Attack cut from Phoenix's\n"
        "Tail have now worn off.\n"
        "<k>";

    const char merlunaIntro[] =
        "<shake>Wah-hah-hah!</shake><wait 500> You've found\n"
        "me, the oh-so-wonderful...<wait 250> the\n"
        "dearly accursed... <wait 250><wave>Merluna!</wave>\n"
        "<k>";

    const char merlunaIntro2[] =
        "What am I doing here, you ask\n"
        "me?<wait 250> Well, now, aren't you an\n"
        "<shake>inquisitive</shake> one, indeed...\n"
        "<k>\n<p>\n"
        "If you must know, I am in\n"
        "the business of <wave>distributing\n"
        "the Ancients' unyielding will!\n"
        "</wave>\n<k>\n<p>\n"
        "\"What does she mean, truly?\"<wait 500> \n"
        "Well, if you'd like, I <wave>cooould\n"
        "</wave>show you my great powers...\n"
        "<dkey></dkey><k>\n<p>\n"
        "<wait 1500>...So I will!<wait 500> For a nominal\n"
        "fee, I shall render Divine\n"
        "Judgement upon your person!\n"
        "<k>\n<p>\n"
        "Worry not;<wait 250> quite often, fortune\n"
        "may very well be in your\n"
        "favor...<wait 750> it's all up to chance!\n"
        "<k>\n<p>\n"
        "You have a 70 percent chance\n"
        "to be <wave>blessed,</wave><wait 500> but a 30 percent\n"
        "chance to be <shake>cursed...\n"
        "</shake><k>\n<p>\n"
        "If you do get cursed,<wait 250> you can\n"
        "always come back to me<wait 100> and\n"
        "I'll make it <wave>allll better...\n"
        "</wave><k>\n<p>\n"
        "Now, allow me to divine the\n"
        "latest in the Tribe's bidding\n"
        "for you,<wait 500> <wave>o great Hero...\n"
        "</wave><k>";

    const char merlunaIntro3[] =
        "Hah-hah,<wait 300> so you've come back\n"
        "for me, <wave>haven't you, Hero?\n"
        "<wait 700></wave><shake>Well, then...\n"
        "</shake><k>\n<p>\n"
        "You know how this goes,<wait 300> so I\n"
        "shall spare you the lecture\n"
        "and hear the Ancients' will...\n"
        "<k>";

    const char merlunaJudgement[] =
        "The Blessing is %s;\n"
        "<wait 300>the Curse is %s.\n"
        "<wait 700><wave>Just 10 coins...<wait 500> wanna risk it?\n"
        "</wave><o>";

    const char merlunaOptions[] =
        "<select 3 -1 360 55>\n"
        "Let's do it!\n"
        "More about Blessing...\n"
        "More about Curse...\n"
        "No, thanks.";

    const char merlunaDecline[] =
        "<wave>Okay, okay, okayyyy... </wave><wait 600><shake>Well,\n"
        "</shake><wait 300>if you change your mind,\n"
        "<wait 300><wave>I'll always be heeeere!\n"
        "</wave><k>";

    const char merlunaBless[] =
        "<wave>Yay!</wave><wait 300> You've just earned the\n"
        "blessing of the Tribe!\n"
        "<wait 500><wave>Enjoy it while you can!\n"
        "</wave><k>";

    const char merlunaCurse[] =
        "<wave>Oh no!</wave><wait 300> Looks like you weren't\n"
        "so lucky...<wait 300> What a shame...\n"
        "<wait 700><shake>Well, good luck now, Hero!\n"
        "</shake><k>";

    const char merlunaScam[] =
        "<shake><dynamic 3>Ugh!!!</shake><wait 500> Have you no respect\n"
        "for the powers of my tribe!?\n"
        "<wait 500>You can't afford me!\n"
        "</dynamic></shake><k>\n<p>\n"
        "<shake>Honestly, </shake><wait 300> some people just\n"
        "have NO respect for us...\n"
        "<wait 700><dynamic 3><shake>DON'T WASTE MY TIME!\n"
        "</dynamic></shake><k>";

    const char merlunaRelieveCurse[] =
        "<shake>Wah-hah-hah,</shake><wait 300> hello, Hero...\n"
        "<wave><wait 800>So, have you been enjoying\n"
        "yourself? </wave><wait 700><shake>Hehehe...\n"
        "</shake><k>\n<p>\n"
        "Well, <wait 300>that's just how it goes!\n"
        "<wait 700>Sometimes,<wait 300> you get lucky,\n"
        "<wait 300>and other times...<wait 800> <wave>You don't!\n"
        "</wave><k>\n<p>\n"
        "I'm just <shake>sure</shake> you've had enough\n"
        "by now, <wait 300>haven't you? <wait 700><wave>But worry\n"
        "not,<wait 200> I can take care of you...\n"
        "</wave><k>\n<p>\n"
        "<wait 1000>For the measly price of... <wait 500><wave>100\n"
        "coins!</wave><wait 500> Surely you can afford\n"
        "that...<wait 700> <wave>Wanna clear the curse?\n"
        "</wave><o>";

    const char merlunaRelieveOptions[] =
        "<select 0 -1 400 40>\n"
        "Yes, please, ma'am!\n"
        "I rather like this curse.";

    const char merlunaRelieveScam[] =
        "<shake><dynamic 3>Unbelievable!</shake><wait 500> You incurred the\n"
        "wrath of my tribe and didn't\n"
        "even prepare an offering!?\n"
        "</dynamic></shake><k>\n<p>\n"
        "<shake>Honestly, </shake><wait 300> some people just\n"
        "have NO respect for us...\n"
        "<wait 700><dynamic 3><shake>DON'T WASTE MY TIME!\n"
        "</dynamic></shake><k>";

    const char merlunaRelieveDecline[] =
        "<wait 1000><wave>...<wait 1400>Well, aren't you strange?\n"
        "</wave><wait 500>Are you what they call,<wait 200> mm,\n"
        "<wait 200>a <shake>\"masochist?\"\n"
        "</shake><k>\n<p>\n"
        "I must say,<wait 200> I don't <wait 100><shake>quite\n"
        "<wait 100></shake>understand your tastes,<wait 200> <wave>but\n"
        "I hope you have fun!\n"
        "</wave><k>";

    const char merlunaRelieveReal[] =
        "<wave>All done!</wave><wait 500> Your curse has been\n"
        "cleared!<wait 200> <shake>Now...</shake><wait 200> <wave>would you like\n"
        "to retry Divine Judgement?\n"
        "</wave><o>";

    const char merlunaRetryOptions[] =
        "<select 0 -1 400 30>\n"
        "I won't lose twice!\n"
        "Maybe not...";

    const char merlunaRetryAccept[] =
        "Hehehe,<wait 200> well,<wait 200> <wave>aren't you a\n"
        "brave little Hero?</wave><wait 800> So be it...\n"
        "<wait 200><shake>Here we go!\n"
        "</shake><k>";

    const char merlunaRetryDecline[] =
        "<wave>Hehehe, I understand... </wave><wait 600><shake>Well,\n"
        "</shake><wait 300>if you change your mind,\n"
        "<wait 300><wave>I'll always be heeeere!\n"
        "</wave><k>";

    const char merlunaSpectre[] =
        "The Blessing known as Spectre\n"
        "<wait 100>is<wait 100> <wave>oh-so very convenient!\n"
        "</wave><wait 200>You'll <shake>certainly</shake> love it...\n"
        "<k>\n<p>\n"
        "Each enemy you defeat will\n"
        "have a 30 percent chance to\n"
        "heal you a certain amount!\n"
        "<k>\n<p>\n"
        "The HP obtained depends on\n"
        "the enemy's HP as well as\n"
        "your progress in the Pit.\n"
        "<k>\n<p>\n"
        "<wave>Doesn't that sound great?\n"
        "<wait 700>It<wait 100> could<wait 100> be<wait 100> yoooours...\n"
        "<wait 100></wave><k>";

    const char merlunaAster[] =
        "The Curse,<wait 200> Tatarian Aster,\n"
        "<wait 200>is simply <shake>terrible...</shake><wait 700> You\n"
        "certainly don't want it!\n"
        "<k>\n<p>\n"
        "Known to some in my tribe as\n"
        "\"Shion,\"<wait 300> it will ensure you have\n"
        "terrible luck...<wait 300> <shake>FOR LIFE!\n"
        "</shake><k>\n<p>\n"
        "In the Pit,<wait 200> you'll have a 50\n"
        "percent chance to be forced\n"
        "to defeat <dynamic 2>every enemy.\n"
        "</dynamic><k>\n<p>\n"
        "Although tedious early on,<wait 200> it\n"
        "can become dangerous in later\n"
        "floors,<wait 200> <wave>so be careful!\n"
        "</wave><k>\n<p>\n"
        "Of course,<wait 200> if you get cursed,\n"
        "<wait 200>I can always make it better\n"
        "for you...<wait 600> <shake>Hehehe...\n"
        "</shake><k>";

    const char merlunaHex[] =
        "take more damage and also\n"
        "deal less damage. isn't\n"
        "that crazy lol\n"
        "<k>";

    const char merlunaHourai[] =
        "Placeholder\n"
        "<k>";

    const char merlunaParamita[] =
        "Placeholder\n"
        "<k>";

    const char merlunaMigraine[] =
        "Placeholder\n"
        "<k>";

    const char moverIntro[] =
        "Hey.<wait 250> Wassup?<wait 250> I'm a Mover.\n"
        "I know a few things about\n"
        "getting through this dungeon.\n"
        "<k>\n<p>\n"
        "For a few coins, I can move\n"
        "you where you wanna go.\n"
        "<wait 250>Where do you wanna go?\n"
        "<o>";

    const char moverChoicesLv1[] =
        "<select 2 -1 400 40>\n"
        "20 Coins: Down 2 Floors\n"
        "60 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv2[] =
        "<select 2 -1 400 40>\n"
        "25 Coins: Down 2 Floors\n"
        "75 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv3[] =
        "<select 2 -1 400 40>\n"
        "30 Coins: Down 2 Floors\n"
        "90 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverChoicesLv4[] =
        "<select 2 -1 400 40>\n"
        "40 Coins: Down 2 Floors\n"
        "120 Coins: Down 5 Floors\n"
        "Nah, I'm fine.";

    const char moverNo[] =
        "Whatever, man. Have it your\n"
        "way. It's not like I do this\n"
        "FOR A LIVING or anything!\n"
        "<k>";

    const char moverScam[] =
        "<wave>Maaan,</wave> you don't have the cash\n"
        "money! How about you talk\n"
        "to me when you can PAY UP?\n"
        "<k>";

    const char moverYes[] =
        "<wave>Alriiiight!</wave> Appreciate the\n"
        "business, man. Let's getcha\n"
        "outta here. <wave>Close your eyes!</wave>\n"
        "<k>";

    const char quickstartOptions[] =
        "<select 0 -1 360 40>\n"
        "New Save\n"
        "Start from 2-1\n"
        "Start from 6-1\n"
        "Start from Postgame";

    const char jimboIntro[] =
        "Sup. I'm Jimbo. Nice to meet\n"
        "ya. I sorta guard the entrance\n"
        "to this here Pit.\n"
        "<k>\n<p>\n"
        "Yeah, I'm kind of a big deal.\n"
        "Warnin' the heroes of the\n"
        "perils that lie in wait...\n"
        "<k>\n<p>\n"
        "Yep, that's just what I do.\n"
        "No one even asks me to do\n"
        "this. I'm just <wave>that cool.\n"
        "</wave><k>";

    const char jimboIntro2[] =
        "Mario\n"
        "<k>";

    const char boodinIntro[] =
        "Yo!<wait 400> Name's Boodin,<wait 200> owner an'\n"
        "operator of Fondest Hopes,\n"
        "<wait 200>your #1 Flopside card shop.\n"
        "<k>\n<p>\n"
        "You got luck!<wait 300> You caught\n"
        "me right in the middle of\n"
        "restockin' my shop.\n"
        "<k>\n<p>\n"
        "Go ahead and take a look\n"
        "at what I got on hand.\n"
        "<wait 200>I'll hook you up.\n"
        "<k>";

    const char boodinItemSelected[] =
        "The %s card?\n"
        "<wait 200> That basically goes for\n"
        "%d coins,<wait 200>  buuut...\n"
        "<k>\n<p>\n"
        "I'll cut you a deal.\n"
        "<wait 300><dynamic 3>%d coins.</dynamic><wait 300> That's half price!\n"
        "<wait 200>How 'bout it?\n"
        "<o>";

    const char boodinSelect[] =
        "<select 0 -1 160 40>\n"
        "Sure\n"
        "Nah";

    const char boodinDecline[] =
        "Change your mind?<wait 300> That's\n"
        "chill.<wait 300> Thanks for stoppin'\n"
        "by!<wait 300> Stay safe out there.\n"
        "<k>";

    const char boodinSatisfied[] =
        "Thanks for the buyin'!\n"
        "<wait 300>Stay safe out there.\n"
        "<k>";

    const char boodinClassism[] =
        "Yo, tough luck on that.\n"
        "<wait 200>You can't afford that card.\n"
        "<k>\n<p>\n"
        "Maybe you'll catch me at\n"
        "another time. Thanks for\n"
        "stoppin' by!\n"
        "<k>";

    const char *mac_kanban_004 =
        "<kanban>\n"
        "<center>Flopside's #1 Useless Pipe</center>\n"
        "<col c00000ff><center>IT'S USELESS! Try it out!</center>\n"
        "</col><center>-Flopside City Hall-</center>\n"
        "<k>";

    const char *D100_entrance_03 =
        "<system>\n"
        "I can't promise this will\n"
        "work correctly, but hey,\n"
        "whatever floats your boat.\n"
        "<o>";

    enum BlessId
    {
        /* 0x0 */ MERLUNA_NULL_BLESS,
        /* 0x1 */ MERLUNA_SPECTRE,
        /* 0x2 */ MERLUNA_HOURAI,
        /* 0x3 */ MERLUNA_PARAMITA
    };

    enum CurseId
    {
        /* 0x0 */ MERLUNA_NULL_CURSE,
        /* 0x1 */ MERLUNA_SHION,
        /* 0x2 */ MERLUNA_HEX,
        /* 0x3 */ MERLUNA_MIGRAINE
    };

    // Patches Dimentio to have a dynamic movement zone rather than being hardcoded for one room.
    s32 dimen_determine_move_pos_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        spm::mario::MarioWork *marioWork = spm::mario::marioGetPtr();
        spm::npcdrv::NPCEntry *npc = entry->ownerNPC;
        double destYPos = 0;
        f32 marioZ = ((marioWork->position).z);
        f32 destXPos = 0;
        u32 dimenMoveRand = 0;
        wii::mtx::Vec3 min;
        wii::mtx::Vec3 max;
        spm::hitdrv::hitGetMapEntryBbox(0, &min, &max);
        s32 i = 0;
        do
        {
            while (true)
            {
                do
                {
                    i = i + 1;
                    dimenMoveRand = spm::system::irand(400);
                    destXPos = ((marioWork->position).x + (f32)dimenMoveRand - 200);
                    if (i > 50)
                    {
                        destXPos = npc->position.x;
                        goto outOfBounds;
                    }
                } while ((destXPos <= (min.x + 25)) || ((max.x - 25) <= destXPos));
            outOfBounds:
                u32 yMoveBehavior = spm::system::irand(100);
                if (yMoveBehavior < 67)
                {
                    dimenMoveRand = spm::system::irand(4);
                    destYPos = (10.0 * (f32)dimenMoveRand + 20.0);
                }
                else
                {
                    dimenMoveRand = spm::system::irand(3);
                    destYPos = (32.0 * (f32)dimenMoveRand + 40.0);
                }
                if (npc->flippedTo3d != 0)
                    break;
                if ((100.0 < __builtin_abs((destXPos - (marioWork->position).x))) || (80.0 < destYPos))
                    goto setFloats;
            }
            destYPos = spm::system::distABf(destXPos, marioZ, ((marioWork->position).x), marioZ);
        } while ((destYPos <= 120.0) && (destYPos <= 80.0));
    setFloats:
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)entry->pCurData;
        spm::evtmgr_cmd::evtSetFloat(entry, args[0], destXPos);
        spm::evtmgr_cmd::evtSetFloat(entry, args[1], destYPos);
        spm::evtmgr_cmd::evtSetFloat(entry, args[2], marioZ);
        return 2;
    }

    static double boobies = 0;

    const char *destMap;
    const char *loadMap;

    // Create arrays for enemy generation, separated by difficulty class

    // Goomba, Flip Goomba, Paragoomba, Green Koopa, Glassesless Koopa, Red Koopa, Flip Red Koopa, Green Paratroopa, Glassesless Paratroopa, Buzzy Beetle, Stone Buzzy, Boo, Green Cheep Cheep,
    // Red Cheep Cheep, Flip Cheep Cheep, Bald Cleft, Squiglet, Squig, Sproing-Oing, Boing-Oing, Boomboxer, Growmeba, Jellien, Slow Cursya, Cherbil, Poison Cherbil, Frackle, Spinia, Flip Buzzy, Dark Puff, Jawbus
    int lv1Names[] = {1, 2, 8, 12, 13, 15, 16, 21, 22, 26, 33, 85, 89, 90, 91, 100, 126, 127, 135, 137, 143, 160, 179, 246, 441, 443, 451, 496, 471, 112, 169};
    int lv1Odds[] = {70, 65, 60, 35, 35, 45, 35, 35, 30, 20, 45, 73, 50, 40, 20, 40, 60, 61, 55, 55, 60, 45, 40, 60, 65, 70, 60, 40, 25, 70, 50};
    int lv1SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    int lv1Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1};
    int lv1Max[] = {7, 6, 5, 5, 5, 5, 5, 4, 4, 5, 6, 6, 4, 4, 3, 6, 6, 6, 5, 4, 5, 3, 5, 5, 4, 5, 4, 6, 5, 6, 3};

    // Gloomba, Spiked Goomba, Spiked Gloomba, Red Paratroopa, Flip Red Paratroopa, Spike Top, Parabuzzy, Spiny, Flip Spiny, Hammer Bro, Magikoopa, Fuzzy, Pokey, 13
    // Spania, Chain Chomp, Squog, Beepboxer, Mister I, Shlurp, Tileoid G, Tileoid B, Rawbus, PatrolMeow, Longator, Pigarithm, 3x Floro Sapiens, Ninjoe,
    // Tech Cursya, Heavy Cursya, Reversya Cursya, Ice Cherbil, Flip Spike Top, Moon Cleft, Kilo Muth, Bleepboxer
    int lv2Names[] = {4, 5, 6, 23, 24, 28, 31, 37, 38, 46, 64, 97, 103, 114, 123, 129, 145, 151, 157, 164, 165, 170, 174, 189, 201, 206, 208, 210, 212, 248, 249, 250, 445, 472, 101, 507, 505};
    int lv2Odds[] = {85, 60, 50, 40, 40, 40, 25, 45, 35, 80, 70, 80, 80, 85, 70, 90, 75, 40, 10, 40, 40, 20, 55, 80, 64, 30, 30, 30, 40, 30, 25, 35, 75, 30, 75, 16, 69};
    int lv2SpR[] = {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv2Min[] = {4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 2, 3, 1, 3, 3, 1, 2, 2, 2, 1, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 2};
    int lv2Max[] = {6, 6, 5, 5, 4, 5, 4, 6, 6, 4, 5, 6, 4, 6, 3, 6, 5, 3, 4, 4, 4, 3, 5, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 5, 5, 2, 5};

    // Headbonk Goomba, Koopatrol, Spiky Parabuzzy, Dry Bones, Flip Hammer Bro, Boomerang Bro, Fire Bro, Broom Magikoopa, Koopa Striker, Bill Blaster, Dark Boo, Clubba, Pink Fuzzy,
    // Hyper Cleft, Ruff Puff, Crazee Dayzee, Zoing-Oing, Blomeba, Tileoid R, Tileoid Y, BombMeow, Foton, Hooligon, Copta, Hogarithm, Ninjohn, Flip Skellobit, Flip Skellobomber,
    // Muth, Spunia, Hyper Goomba, Hyper Spiked Goomba, Hyper Paragoomba, Dark Koopa, Dark Paratroopa, Flip Boomerang Bro, Flip Fire Bro, Soopa Striker, Green Fuzzy, Ice Bro, Red Magikoopa,
    // Red Broom Magikoopa, White Magikoopa, White Broom Magikoopa, Green Magikoopa, Green Broom Magikoopa
    int lv3Names[] = {10, 19, 32, 40, 48, 52, 58, 65, 70, 84, 86, 95, 98, 102, 110, 116, 139, 161, 166, 167, 178, 180, 185, 193, 202, 215, 225, 231, 258, 497, 3, 7, 9, 17, 25, 54, 60, 74, 99, 62, 480, 481, 477, 478, 474, 475};
    int lv3Odds[] = {80, 35, 30, 30, 45, 50, 50, 50, 65, 30, 70, 75, 65, 35, 75, 69, 55, 40, 40, 40, 10, 30, 50, 60, 40, 40, 50, 30, 10, 45, 30, 30, 30, 20, 25, 30, 30, 50, 35, 50, 20, 20, 20, 20, 20, 20};
    int lv3SpR[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv3Min[] = {4, 2, 2, 3, 1, 3, 3, 2, 2, 2, 3, 3, 4, 3, 3, 3, 3, 1, 2, 2, 2, 2, 2, 3, 2, 2, 3, 3, 1, 2, 3, 3, 3, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 2, 2};
    int lv3Max[] = {6, 5, 4, 5, 3, 4, 4, 4, 5, 4, 6, 5, 6, 5, 6, 5, 5, 3, 4, 4, 5, 4, 3, 5, 4, 4, 6, 5, 1, 5, 6, 6, 6, 5, 5, 5, 5, 4, 8, 5, 4, 4, 4, 4, 4, 4};

    // Lakitu, Flip Lakitu, Dull Bones, Poison Pokey, Amazee Dayzee, Red Chomp, Squoinker, Blastboxer, Red I, Shlorp, Chromeba, Gawbus, Longadile, Ninjerry,
    // Flip Spiky Skellobit, Flip Skellobomber w/ Skellobait, 3x Magiblots, Back Cursya, Gigabyte, Mega Muth, Shady Koopa, Flip Shady Koopa, Ash Cherbil, Sky-Blue Spiny,
    // Red Spike Top, Dark Bones, White Clubba, Shady Magikoopa, Shady Broom Magikoopa, Tileoid PU, Ninjeremiah, Dark Koopatrol, Kamikaze Goomba,
    // Skellobyte, Spiky Skellobyte, Shady Hammer Bro, Shady Boomerang Bro, Shady Striker
    int lv4Names[] = {35, 36, 42, 105, 118, 124, 131, 147, 153, 158, 162, 171, 190, 218, 228, 234, 238, 240, 242, 247, 256, 259, 466, 467, 447, 39, 30, 44, 96, 67, 68, 168, 221, 20, 11, 226, 229, 50, 56, 76};
    int lv4Odds[] = {60, 50, 65, 80, 7, 30, 80, 80, 20, 20, 70, 30, 50, 40, 50, 30, 40, 40, 40, 4, 10, 10, 60, 60, 73, 75, 70, 60, 70, 66, 44, 74, 40, 30, 70, 40, 40, 60, 60, 60};
    int lv4SpR[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int lv4Min[] = {1, 1, 2, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 1, 4, 2, 2, 1, 1, 2};
    int lv4Max[] = {2, 2, 5, 3, 2, 3, 6, 5, 2, 2, 3, 2, 5, 4, 4, 4, 5, 5, 5, 4, 2, 2, 5, 5, 5, 6, 2, 3, 3, 3, 3, 4, 4, 4, 6, 5, 4, 2, 2, 5};

    //   int SpR2Doors[] = {3, 6, 12, 13, 19, 22};
    //   int SpR1Doors[] = {27, 28, 29, 30};

    int enemyConfigArray[2001] = {169};

    // Pit Flimm item pool to iterate from
    int rotenShopItemPool[] = {67, 68, 69, 70, 75, 81, 82, 84, 89, 90, 99, 106, 118, 123, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
                               141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 161, 162, 163, 164, 166, 168, 169, 170,
                               171, 173, 174, 177, 178, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 198, 199, 200,
                               201, 202, 203, 204, 205, 207, 208, 209, 211, 212, 214, 215};

    int boodinShopItemPool[] = {288, 292, 298, 299, 301, 302, 305, 307, 308, 310, 312, 313, 321, 323, 328, 329, 332, 335, 341, 343, 346, 355, 358, 360, 363,
                                366, 372, 377, 383, 389, 393, 396, 398, 399, 400, 401, 409, 412, 415, 418, 424, 426, 428, 433, 432, 436, 437, 440, 444, 446,
                                447, 448, 0x11c, 0x11e, 0x121, 0x122, 0x125, 0x126, 0x129, 0x12c, 0x132, 0x135, 0x13a, 0x13b, 0x13c,
                                0x13e, 0x142, 0x144, 0x14d, 0x156, 0x167, 0x16c, 0x184, 0x192, 0x1b2, 0x1b6, 0x1b7};

    struct
    {
        s32 cardCount = 13;
        s32 cardArray[15] = {0x58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    } boodinBalls;

    // Set the Pit Flimm inventory
    s32 newRotenShopItems[] = {
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        -1};

    int rfcCommon[] = {
        spm::item_data::ItemType::ITEM_ID_COOK_HUNNY_KINOKO, 1,
        spm::item_data::ItemType::ITEM_ID_USE_HONOO_SAKURETU, 1,
        spm::item_data::ItemType::ITEM_ID_USE_STAR_MEDAL, 2,
        spm::item_data::ItemType::ITEM_ID_USE_BIG_EGG, 1,
        spm::item_data::ItemType::ITEM_ID_USE_KOURA_DE_PON, 2,
        spm::item_data::ItemType::ITEM_ID_USE_KOORI_NO_IBUKI, 2,
        spm::item_data::ItemType::ITEM_ID_USE_BARIA_FRAME, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_BOMB_EGG, 3,
        spm::item_data::ItemType::ITEM_ID_USE_POW_BLOCK, 2,
        spm::item_data::ItemType::ITEM_ID_USE_TOROPICO_MANGO, 1,
        spm::item_data::ItemType::ITEM_ID_USE_MILD_CACAO, 1,
        spm::item_data::ItemType::ITEM_ID_USE_ROW_PASTA, 1,
        spm::item_data::ItemType::ITEM_ID_USE_INGREDIENTS_OF_SWEET, 1,
        spm::item_data::ItemType::ITEM_ID_USE_TUKUSHINBO, 1,
        spm::item_data::ItemType::ITEM_ID_USE_PRIMITIVENUT, 1,
        spm::item_data::ItemType::ITEM_ID_USE_HERB, 1,
        spm::item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_FAIL_COOKING1, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_FAIL_COOKING2, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_HOT_COCOA, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_KAME_TEA, 3,
        spm::item_data::ItemType::ITEM_ID_COOK_HERB_TEA, 3};

    int rfcUncommon[] = {
        spm::item_data::ItemType::ITEM_ID_COOK_KINOKO_FRY, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_HUNNY_KINOKO_S, 2,
        spm::item_data::ItemType::ITEM_ID_USE_NANIGA_OKORUKANA, 2,
        spm::item_data::ItemType::ITEM_ID_USE_KAMINARI_DOKKAN, 1,
        spm::item_data::ItemType::ITEM_ID_USE_TUYOTUYO_DRINK, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_HOTDOG, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_HANAJIRU_SYRUP, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_MOUSSE_CAKE, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_CHOCOLA_CAKE, 2,
        spm::item_data::ItemType::ITEM_ID_USE_KINKYU_KINOKO, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_PEACH_TART, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_MIX_SHAKE, 2,
        spm::item_data::ItemType::ITEM_ID_USE_POWERFUL_MEET, 2,
        spm::item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_TRIAL_PAN, 3};

    int rfcRare[] = {
        spm::item_data::ItemType::ITEM_ID_COOK_KINOKO_HOILE_FRY, 1,
        spm::item_data::ItemType::ITEM_ID_USE_KIRAKIRA_OTOSHI, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_DINNER, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_HANA_DANGO, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_MANGO_PUDDING, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_GOLD_CHOKO, 2,
        spm::item_data::ItemType::ITEM_ID_USE_GOLD_MEDAL, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_ICHIGO_RABBIT, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_SNOW_RABBIT, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_LOVE_NOODLE, 2,
        spm::item_data::ItemType::ITEM_ID_USE_ULTRA_DRINK, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_DOROCY_DELUX, 3,
        spm::item_data::ItemType::ITEM_ID_COOK_EMERGENCY_MEAL, 2,
        spm::item_data::ItemType::ITEM_ID_COOK_FRUITS_HUMBURG, 1,
        spm::item_data::ItemType::ITEM_ID_USE_KINKYU_KINOKO, 2,
        spm::item_data::ItemType::ITEM_ID_USE_SHINABITA_KINOKO, 1,
        spm::item_data::ItemType::ITEM_ID_COOK_TRIAL_PAN, 3};

    int rfcItems[] = {0, 0, 0, -1};

    s32 no = 0;
    s32 i = 0;
    int currentFloor = 0;
    int nextFloor = 0;
    int enemyArrayOffset = 0;
    int nextEnemyCount = 0;
    int nextName1 = 0;
    int nextNum1 = 0;
    int nextPos1 = 0;
    int nextName2 = 0;
    int nextNum2 = 0;
    int nextPos2 = 0;
    int nextName3 = 0;
    int nextNum3 = 0;
    int nextPos3 = 0;

    s32 marioMaxHp = 0;
    s32 marioAtk = 0;
    s32 bowserAtk = 0;

    int moverRNG = 0;
    int houraiStoredAtk = 0;
    bool hpMaxed = false;
    bool roomOnHud = false;
    bool houraiActivation = 0;
    int blessingRoomCounter = 0;
    int blessingNum = 0;
    int curseNum = 0;
    int rand100Num = 0;
    int blessStorage = 0;
    int curseStorage = 0;
    int moverDown2Price = 0;
    int moverDown5Price = 0;
    int motId = 0;

    // Shy Guy anim defs
    spm::npcdrv::NPCTribeAnimDef heihoAnims[] = { // Copied from Goombas, then R_2 thrown in lol
        {0, "heiho_S_1"},                         // Idle
        {1, "heiho_W_1"},                         // Walking
        {2, "heiho_R_2"},                         // Running (Replace with R_1 for normal running)
        {3, "heiho_T_1"},                         // Talking
        {4, "heiho_D_1"},                         // Damaged
        {6, "heiho_D_1"},                         // Damaged
        {7, "heiho_D_1"},                         // Damaged
        {8, "heiho_D_1"},                         // Damaged
        {9, "heiho_K_1"},                         // Dizzy
        {10, "heiho_Z_1"},                        // Idle
        {11, "heiho_Z_1"},                        // Idle
        {12, "heiho_K_1"},                        // Dizzy
        {13, "heiho_K_1"},                        // Dizzy
        {14, "heiho_N_1"},                        // Eepy
        {15, "heiho_Z_1"},                        // Idle
        {26, "heiho_R_2"},                        // Ludicrous speed!!! (Use only for non-enemy stuff)
        {-1, "heiho_Z_1"}};

    EVT_BEGIN(heiho_g_panic_real)
    DO(0)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(4), LW(5), LW(6))
    USER_FUNC(spm::evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(7))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(14), 0, LW(15))
    USER_FUNC(spm::evt_mario::evt_mario_check_3d, LW(10))
    IF_EQUAL(LW(10), 0) // Check if in 3D
                        // Check if Shy Guy is facing Mario and is within 200 units on X (Z only if in 3D)
    IF_EQUAL(LW(13), 0) // Not already in Panicked state
    SUB(LW(14), LW(4))
    MUL(LW(14), LW(7))
    IF_SMALL(LW(14), 0) // If NPC is facing Mario & under 200 units away. Trust me, this works
    IF_LARGE(LW(14), -100)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(1), LW(2), LW(3))
    SUB(LW(5), LW(2))
    IF_EQUAL(LW(5), 0) // Same Y val. Execute code now
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 1)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(14), LW(2), LW(15))
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(14), LW(15), 0, 0, 532, 0, 0)
    MUL(LW(1), -1)
    MUL(LW(7), -1)
    WAIT_MSEC(450)
    //        USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), LW(7))
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 13, 1)
    WAIT_MSEC(350)
    MUL(LW(7), 8192)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(7), LW(3), 0, 127, 532, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 0)
    DO_BREAK()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    WAIT_FRM(5)
    WHILE()
    RETURN()
    EVT_END()

    EVT_BEGIN(heiho_g_panic)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(12))
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    IF_EQUAL(LW(13), 0)
    IF_EQUAL(LW(12), 530)
    RUN_EVT(heiho_g_panic_real)
    END_IF()
    END_IF()
    IF_EQUAL(LW(11), 0)
    IF_EQUAL(LW(12), 530) // Is Green Shy Guy
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 13, LW(13))
    IF_EQUAL(LW(13), 1) // In Panicked state
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 8, LW(8))
    IF_EQUAL(LW(8), 0)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 255, 532, 0, 0)
    END_IF()
    ELSE() // Not in Panicked state
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 40, 532, 0, 0)
    END_IF()
    ELSE() // Not Green Shy Guy
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 30, 532, 0, 0)
    END_IF()
    ELSE() // Not Green Shy Guy OR Goomba
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 30, 540, 0, 0)
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 8, 0)
    RETURN_FROM_CALL()

    // Define enemy defenses
    spm::npcdrv::NPCDefense createDef(int type, int defense, int flags)
    {
        spm::npcdrv::NPCDefense def;
        def.type = type;
        def.defense = defense;
        def.flags = flags;
        return def;
    }
    spm::npcdrv::NPCDefense def0_1 = createDef(0x0, 0x1, 0x0);
    spm::npcdrv::NPCDefense def1_1 = createDef(0x1, 0x1, 0x0);
    spm::npcdrv::NPCDefense def2_1 = createDef(0x2, 0x1, 0x0);
    spm::npcdrv::NPCDefense def3_1 = createDef(0x3, 0x1, 0x0);
    spm::npcdrv::NPCDefense def4_1 = createDef(0x4, 0x1, 0x0);
    spm::npcdrv::NPCDefense def7_1 = createDef(0x7, 0x1, 0x0);
    spm::npcdrv::NPCDefense def10_1 = createDef(0xa, 0x1, 0x0);
    spm::npcdrv::NPCDefense def11_1 = createDef(0xb, 0x1, 0x0);
    spm::npcdrv::NPCDefense def12_1 = createDef(0xc, 0x1, 0x0);
    spm::npcdrv::NPCDefense def13_1 = createDef(0xd, 0x1, 0x0);
    spm::npcdrv::NPCDefense def15_1 = createDef(0xf, 0x1, 0x0);
    spm::npcdrv::NPCDefense def17_1 = createDef(0x11, 0x1, 0x0);
    spm::npcdrv::NPCDefense def29_1 = createDef(0x1d, 0x1, 0x0);
    spm::npcdrv::NPCDefense def31_1 = createDef(0x1f, 0x1, 0x0);

    // Initiators / Terminators (41 for sure, 48 unsure?)
    // Use def0 for enemies with no default defense
    spm::npcdrv::NPCDefense def0 = createDef(0x0, 0x0, 0x0);
    spm::npcdrv::NPCDefense def48 = createDef(0x30, 0x32, 0x0);
    spm::npcdrv::NPCDefense defterm = createDef(41, 0x64, 0x0);
    static spm::npcdrv::NPCDefense defaultDefenses[] = {def0, def48, defterm};

    // Add/reduce damage to certain enemies; later, maybe set DEFs for enemies that could actually use it and don't override all defenses with Fucking damage reduction.
    // There are certainly a few in this array that can use DEFs, DR is just easier for me right now.
    int (*marioCalcDamageToEnemy)(s32 damageType, s32 tribeId);
    void (*marioTakeDamage)(wii::mtx::Vec3 *position, u32 flags, s32 damage);
    s32 (*npcDamageMario)(spm::npcdrv::NPCEntry *npcEntry, spm::npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 status, s32 damage, u32 flags);
    s32 (*npcHandleHitXp)(spm::mario::MarioWork *marioWork, spm::npcdrv::NPCEntry *npcEntry, s32 killXp, s32 unk_variant);
    void patchMarioDamage()
    {
        marioCalcDamageToEnemy = patch::hookFunction(spm::mario::marioCalcDamageToEnemy,
                                                     [](s32 damageType, s32 tribeId)
                                                     {
                                                         int dmg = marioCalcDamageToEnemy(damageType, tribeId);
                                                         int paramitaTimer = spm::swdrv::swByteGet(1610);
                                                         int floor = spm::swdrv::swByteGet(1);
                                                         if (paramitaTimer > 20)
                                                         {
                                                             if (floor < 150)
                                                             {
                                                                 dmg = (dmg * 2);
                                                             }
                                                             else if (floor < 175)
                                                             {
                                                                 dmg = (dmg * 3);
                                                             }
                                                             else
                                                             {
                                                                 dmg = (dmg * 4);
                                                             }
                                                         }
                                                         // Kilo Muth, DEF 3 -> 1
                                                         // Ice Bro, DEF 3 --> 1
                                                         // Red Spike Top, DEF 6 --> 4
                                                         // Sky-Blue Spiny, DEF 6 --> 4
                                                         if (tribeId == 506 || tribeId == 61 || tribeId == 29 || tribeId == 38)
                                                         {
                                                             dmg += 2;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         // Spunia, DEF 0 --> 1
                                                         // Green Shy Guy, DEF 0 --> 1
                                                         else if (tribeId == 496 || tribeId == 530)
                                                         {
                                                             dmg -= 1;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         // Shady Bros, DEF 3 --> 2
                                                         if (tribeId == 61 || tribeId == 55 || tribeId == 49)
                                                         {
                                                             dmg += 1;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         // Green (Broom) Magikoopa, DEF 0 --> 3
                                                         // (Flip) Shady Koopa, DEF 0 --> 3
                                                         else if (tribeId == 473 || tribeId == 474 || tribeId == 465 || tribeId == 466)
                                                         {
                                                             dmg -= 3;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         // Blue Shy Guy, DEF 0 --> 2
                                                         else if (tribeId == 531)
                                                         {
                                                             dmg -= 2;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         // Dark Koopatrol, DEF 8 --> 5
                                                         else if (tribeId == 19)
                                                         {
                                                             dmg += 3;
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }

                                                         // Vulnerability Hex
                                                         int curse = spm::swdrv::swByteGet(1601);
                                                         if (curse == CurseId::MERLUNA_HEX)
                                                         {
                                                             int dmg = marioCalcDamageToEnemy(damageType, tribeId);
                                                             if (floor < 149)
                                                             {
                                                                 dmg -= 1;
                                                             }
                                                             else if (floor < 174)
                                                             {
                                                                 dmg -= 2;
                                                             }
                                                             else
                                                             {
                                                                 dmg -= 3;
                                                             }
                                                             if (dmg < 0)
                                                             {
                                                                 dmg = 0;
                                                             }
                                                         }
                                                         return dmg;
                                                     });

        marioTakeDamage = patch::hookFunction(spm::mario::marioTakeDamage,
                                              [](wii::mtx::Vec3 *position, u32 flags, s32 damage)
                                              {
                                                  // Vulnerability Hex
                                                  int curse = spm::swdrv::swByteGet(1601);
                                                  if (curse == CurseId::MERLUNA_HEX)
                                                  {
                                                      int floor = spm::swdrv::swByteGet(1);
                                                      if (floor < 49)
                                                      {
                                                          damage += 1;
                                                      }
                                                      else if (floor < 74)
                                                      {
                                                          damage += 2;
                                                      }
                                                      else
                                                      {
                                                          damage += 3;
                                                      }
                                                      if (damage < 0)
                                                      {
                                                          damage = 0;
                                                      }
                                                  }
                                                  // Migraine
                                                  if (curse == CurseId::MERLUNA_MIGRAINE)
                                                  {
                                                      int procMigraine = spm::system::rand() % 100;
                                                      if (procMigraine > 50)
                                                      {
                                                          spm::mario_status::marioStatusApplyStatuses(STATUS_FLIPPED_CONTROLS, 2);
                                                          spm::swdrv::swSet(1670);
                                                      }
                                                      else if (procMigraine > 30)
                                                      {
                                                          spm::mario_status::marioStatusApplyStatuses(STATUS_NO_SKILLS, 2);
                                                          spm::swdrv::swSet(1670);
                                                      }
                                                  }
                                                  marioTakeDamage(position, flags, damage);
                                              });

        npcDamageMario = patch::hookFunction(spm::npcdrv::npcDamageMario,
                                             [](spm::npcdrv::NPCEntry *npcEntry, spm::npcdrv::NPCPart *part, wii::mtx::Vec3 *position, u32 status, s32 damage, u32 flags)
                                             {
                                                 if (npcEntry->tribeId == 62) // Ice Bro projectiles will freeze you
                                                 {
                                                     status = status + 0x2000;
                                                 }
                                                 if (npcEntry->unkShellSfx == "holo") // Holographic enemies in the Pit will have 1.5x direct ATK
                                                 {
                                                     if (damage % 2 == 1) // Variable is odd, so round up.
                                                     {
                                                         damage = ((f32)damage * 1.5) + 0.5;
                                                     }
                                                     else
                                                     {
                                                         damage = damage * 1.5;
                                                     }
                                                 }
                                                 return npcDamageMario(npcEntry, part, position, status, damage, flags);
                                             });

        npcHandleHitXp = patch::hookFunction(spm::temp_unk::npcHandleHitXp,
                                             [](spm::mario::MarioWork *marioWork, spm::npcdrv::NPCEntry *npcEntry, s32 killXp, s32 unk_variant)
                                             {
                                                 if (npcEntry->unkShellSfx == "holo") // Holographic enemies in the Pit will give 2x score
                                                 {
                                                     killXp = killXp * 2;
                                                 }
                                                 return npcHandleHitXp(marioWork, npcEntry, killXp, unk_variant);
                                             });
    }

    // Toggle below to "true" to enable on-screen displays for current enemies in a Pit room / enemies in the next room / GSW1. Will display in pit rooms 2-98.
    // This feature is not complete or stable, nor do I need it to be. Leaving the pit early may cause the game to crash.
    bool displayDebugValues = false;

    // Called to remove default segments and their shadows from the map (Top 7 are the hitobjs, followed by mapobjs for visible segment and dropshadow)
    s32 danClearMapParts()
    {
        // Hitobjs for default segs
        spm::hitdrv::hitGrpFlagOn(0, "pCube11_q00009", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube12_q00010", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube15_q00011", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube16_q00012", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube17_q00013", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube18_q00014", 1);
        spm::hitdrv::hitGrpFlagOn(0, "pCube19_q00015", 1);

        // dan_01 mapobjs for default segs and shadows
        spm::mapdrv::mapGrpFlagOn(0, "pCube11_q00028", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube11_q00029", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube12_q00030", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube12_q00031", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube15_q00032", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube15_q00033", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube16_q00034", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube16_q00035", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube17_q00036", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube17_q00037", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube18_q00038", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube18_q00039", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube19_q00040", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube19_q00041", 1);

        // dan_02 mapobjs for default segs and shadows (1 shared with dan_01)
        spm::mapdrv::mapGrpFlagOn(0, "pCube11_q00036", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube11_q00037", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube12_q00036", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube12_q00037", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube15_q00036", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube15_q00037", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube16_q00038", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube16_q00039", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube17_q00039", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube17_q00040", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube18_q00040", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube18_q00041", 1);

        // dan_43 mapobjs (tails are offset by 1 from dan_01)
        spm::mapdrv::mapGrpFlagOn(0, "pCube11_q00027", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube12_q00029", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube15_q00031", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube16_q00033", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube17_q00035", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube18_q00037", 1);
        spm::mapdrv::mapGrpFlagOn(0, "pCube19_q00039", 1);
        return 2;
    }

    s32 generateMoverRng()
    {
        moverRNG = spm::system::rand() % 1000;
        int floor = spm::swdrv::swByteGet(1);
        if (floor >= 43 && floor <= 148)
        {
            moverRNG = 999;
        }
        else if (floor > 194)
        {
            moverRNG = 999;
        }
        //    moverRNG = 2; // Debug; comment out normally
        wii::os::OSReport("moverRNG: %d.\n", moverRNG);
        return 2;
    }

    static void dan_01BossRoomTex()
    {
        loadMap = spm::seq_mapchange::seq_mapchange_wp->mapName;
        spm::mapdrv::MapWorkGroup *mapWork = spm::mapdrv::mapGetWork(); // Gets MapWorkGroup, which should contain the MapEntry for the current map
        spm::mapdrv::MapEntry *mapEntry = &mapWork->entries[0];         // The MapEntry for current map

        char *supermariogaming = msl::string::strstr(loadMap, "dan");
        if (supermariogaming != nullptr)
        {
            generateMoverRng();
            /*    if (moverRNG >= 25 && moverRNG <= 49)
                {
                    const char *dvdRoot = spm::system::getSpmarioDVDRoot();
                    spm::filemgr::FileEntry *bossRoomFile = spm::filemgr::fileAllocf(0, "%s/map/boss_dan_01.tpl", dvdRoot);
                    s32 tplSize = bossRoomFile->length;
                    wii::tpl::TPLHeader *bossRoomTpl = (wii::tpl::TPLHeader *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, tplSize);
                    msl::string::memcpy(bossRoomTpl, bossRoomFile->sp->data, tplSize);
                    spm::filemgr::fileFree(bossRoomFile);
                    wii::os::OSReport("tplSize: %d\n", tplSize);

                    wii::tpl::TPLBind(bossRoomTpl); // Initializes custom TPL in memory
                    mapEntry->tpl = bossRoomTpl;    // Replaces normal tpl for map with the boss room TPL
                }
                else
                { */
            wii::tpl::TPLBind(mapEntry->tpl);
        }
        //    }
        else
        {
            wii::tpl::TPLBind(mapEntry->tpl);
        }
    }

    static void handleHouraiDoll()
    {
        int blessing = spm::swdrv::swByteGet(1600);
        houraiActivation = spm::swdrv::swGet(1671);
        if (blessing == BlessId::MERLUNA_HOURAI && !houraiActivation)
        {
            spm::mario_motion::marioChgMot(MOT_DAMAGE);
            spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
            spm::mario::marioKeyOff();
            spm::swdrv::swSet(1671);
            houraiStoredAtk = pouch->attack;
            s32 halvedAtk = (f32)(msl::math::floor((f32)(pouch->attack / 2)));
            if ((halvedAtk * 2) < pouch->attack) // Workaround since ceil just doesn't seem to exist in SPM
            {
                pouch->attack = (halvedAtk + 1); // i.e. if original ATK is 5, desired result is 3. Floors to 2. 4 < 5, so 2 + 1 = 3.
            }
            else
            {
                pouch->attack = halvedAtk; // i.e. if original ATK is 4, desired result is 2. Floors to 2. 4 !< 4, so remains 2.
            }
            if (pouch->maxHp != 999)
            {
                pouch->maxHp = (pouch->maxHp + 10);
            }
            else
            {
                hpMaxed = true;
            }
            pouch->hp = pouch->maxHp;
            spm::swdrv::swSet(1672);
            return;
        }
        else
        {
            spm::mario_motion::marioChgMot(24); // MOT_DEATH
        }
    }
    s32 handleBlessingWearOff(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        houraiActivation = spm::swdrv::swGet(1671);
        if (houraiActivation)
        {
            spm::swdrv::swClear(1671);
            spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
            pouch->attack = houraiStoredAtk;
            int currentHp = pouch->hp;
            if (!hpMaxed)
            {
                pouch->maxHp = (pouch->maxHp - 10);
            }
            if (currentHp > pouch->maxHp)
            {
                pouch->hp = pouch->maxHp;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(handleBlessingWearOff, 0)

    s32 evt_dan_read_data_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)entry;

        // Allocate work on first run
        // (check is a relD leftover, this only runs once on retail anyway)
        if (isFirstCall)
        {
            spm::dan::dan_wp = (spm::dan::DanWork *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanWork));
            msl::string::memset(spm::dan::dan_wp, 0, sizeof(*spm::dan::dan_wp));
            spm::dan::dan_wp->dungeons = (spm::dan::DanDungeon *)spm::memory::__memAlloc(spm::memory::Heap::HEAP_MAP, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
            msl::string::memset(spm::dan::dan_wp->dungeons, 0, sizeof(spm::dan::DanDungeon[DUNGEON_MAX]));
        }

        // Reset Pit chests
        spm::swdrv::swClear(433);
        spm::swdrv::swClear(434);
        spm::swdrv::swClear(435);
        spm::swdrv::swClear(436);
        spm::swdrv::swClear(437);
        spm::swdrv::swClear(438);
        spm::swdrv::swClear(439);
        spm::swdrv::swClear(440);
        spm::swdrv::swClear(441);
        spm::swdrv::swClear(442);
        spm::swdrv::swClear(443);
        spm::swdrv::swClear(444);
        spm::swdrv::swClear(445);
        spm::swdrv::swClear(446);
        spm::swdrv::swClear(447);
        spm::swdrv::swClear(448);
        spm::swdrv::swClear(449);
        spm::swdrv::swClear(450);
        bool nipples = spm::swdrv::swGet(409);

        // Clear Merluna judgement flag
        spm::swdrv::swClear(1669);

        // Prepare pit text to be read
        /*    u32 size = wii::cx::CXGetCompressionHeader(spm::lz_embedded::pitText).decompSize;
            char *decompPitText = (char *)spm::memory::__memAlloc(0, size);
            wii::cx::CXUncompressLZ(spm::lz_embedded::pitText, decompPitText);
            spm::parse::parseInit(decompPitText, size); */
        i = 0;
        currentFloor = spm::swdrv::swByteGet(1);
        int shadooEntries = spm::swdrv::swByteGet(24);

        // Global room generation RNG value (1-100, 5 decimal places)
        double thighs = spm::system::rand() % 99 + 1;
        double divisor = 10;
        double ass = (spm::system::rand() % 10) / divisor;
        divisor = 100;
        double booty = (spm::system::rand() % 10) / divisor;
        divisor = 1000;
        double butt = (spm::system::rand() % 10) / divisor;
        divisor = 10000;
        double bottom = (spm::system::rand() % 10) / divisor;
        divisor = 100000;
        double buttocks = (spm::system::rand() % 10) / divisor;
        boobies = (thighs + ass + booty + butt + bottom + buttocks);

        // Check if room 48, then shove you into Flopside Pit
        if (currentFloor == 48)
        {
            spm::swdrv::swByteSet(1, 148);
        }

        // Resets no if pit is started
        if (currentFloor == 0)
        {
            no = -1;
        }
        no = no + 1;

        // Ensure Shadoo is fought on first room entry, but never again
        if (shadooEntries == 0)
        {
            spm::swdrv::swByteSet(24, 1);
        }

        // Start pit room randomization process
        // Establish default doors + other vars
        int activeDoorCount = 19; // Actually 20, but let's not complicate things
        int doorOn[31] = {2, 3, 6, 7, 9, 12, 13, 16, 18, 19, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32};
        int roomDecCode = 0;
        int segmentCount = 0;
        bool structureGenerated = true;
        bool segment400 = false;
        bool segment1 = false;
        bool segment800 = false;
        bool segment80 = false;
        bool segment40 = false;
        bool segment2 = false;
        bool segment2000 = false;
        bool segment1000 = false;
        bool segment8 = false;
        bool segment4 = false;
        bool segment4000 = false;
        bool segment200 = false;
        bool segment100 = false;
        bool segment10 = false;
        bool segment8000 = false;
        bool segment20 = false;
        bool segment10000 = false;
        bool segment20000 = false;
        bool segment40000 = false;
        bool segment80000 = false;

        if (currentFloor != 198)
        {
            // Determine # of segments to generate for current room
            int segmentRNG = spm::system::rand() % 152;
            if (segmentRNG >= 0 && segmentRNG < 42)
            {
                segmentCount = 3;
            }
            else if (segmentRNG >= 42 && segmentRNG < 45)
            {
                segmentCount = 1;
            }
            else if (segmentRNG >= 45 && segmentRNG < 109)
            {
                segmentCount = 4;
            }
            else if (segmentRNG >= 109 && segmentRNG < 122)
            {
                segmentCount = 2;
            }
            else if (segmentRNG >= 122 && segmentRNG < 124)
            {
                segmentCount = 0;
            }
            else
            {
                segmentCount = 5;
            }
            wii::os::OSReport("Queueing %d segment generations for this room.\n", segmentCount);
            i = 0;

            // Enclosed structure generation. If more than 2 segments are generated at any time, the remainder is subtracted from segmentCount
            int structureRNG = spm::system::rand() % 180;
            int structureMiscRNG = spm::system::rand() % 100;
            // Split
            if (structureRNG >= 0 && structureRNG < 8)
            {
                wii::os::OSReport("!!! Split Structure is generating !!!\n");
                segment2000 = true;
                segment8 = true;
                if (structureMiscRNG < 70)
                {
                    segment20000 = true;
                }
                else if (structureMiscRNG < 92)
                {
                    segment80000 = true;
                }
                else
                {
                    segment1 = true;
                    segment20 = true;
                    segment400 = true;
                    segment8000 = true;
                    segment10000 = true;
                    segment20000 = true;
                    segment40000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 4;
                }
            }
            // Quarter
            else if (structureRNG >= 8 && structureRNG < 19)
            {
                wii::os::OSReport("!!! Quarter Structure is generating !!!\n");
                if (structureMiscRNG < 31)
                {
                    segment100 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                        {
                            segment80000 = true;
                        }
                    }
                    else
                    {
                        segment80000 = true;
                    }
                }
                else if (structureMiscRNG < 54)
                {
                    segment40 = true;
                    segment8 = true;
                    if (segment40000)
                    {
                        if (boobies < 50)
                        {
                            segment80000 = true;
                        }
                    }
                    else
                    {
                        segment80000 = true;
                    }
                }
                else if (structureMiscRNG < 77)
                {
                    segment100 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                        {
                            segment20000 = true;
                        }
                    }
                    else
                    {
                        segment20000 = true;
                    }
                }
                else
                {
                    segment40 = true;
                    segment2000 = true;
                    if (segment10000)
                    {
                        if (boobies < 50)
                        {
                            segment20000 = true;
                        }
                    }
                    else
                    {
                        segment20000 = true;
                    }
                }
            }
            // Cube
            else if (structureRNG >= 19 && structureRNG < 21)
            {
                wii::os::OSReport("!!! Cube Structure is generating !!!\n");
                if (structureMiscRNG < 50)
                {
                    segment400 = true;
                    segment80 = true;
                    segment1 = true;
                    segment20 = true;
                    segmentCount = segmentCount - 2;
                    segment20000 = true;
                    segment40000 = true;
                }
                else
                {
                    segment8000 = true;
                    segment200 = true;
                    segment20 = true;
                    segment1 = true;
                    segmentCount = segmentCount - 2;
                    segment20000 = true;
                    segment40000 = true;
                }
            }
            // Pillar
            else if (structureRNG >= 21 && structureRNG < 23)
            {
                wii::os::OSReport("!!! Pillar Structure is generating !!!\n");
                if (structureMiscRNG < 60)
                {
                    segment800 = true;
                    segment80 = true;
                    segment2 = true;
                }
                else
                {
                    segment4000 = true;
                    segment200 = true;
                    segment10 = true;
                }
                if (boobies < 50)
                {
                    segment20000 = true;
                }
                else
                {
                    segment80000 = true;
                }
            }
            // Small
            else if (structureRNG >= 23 && structureRNG < 26)
            {
                wii::os::OSReport("!!! Small Structure is generating !!!\n");
                if (structureMiscRNG < 33)
                {
                    segment800 = true;
                    segment400 = true;
                    segment10 = true;
                    segment20 = true;
                    segment8000 = true;
                    segment10000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 3;
                }
                else if (structureMiscRNG < 67)
                {
                    segment8000 = true;
                    segment4000 = true;
                    segment1 = true;
                    segment2 = true;
                    segment400 = true;
                    segment10000 = true;
                    segment80000 = true;
                    segmentCount = segmentCount - 3;
                }
                else
                {
                    if (boobies < 25)
                    {
                        segment8000 = true;
                        segment4000 = true;
                        segment400 = true;
                        segment10000 = true;
                        segmentCount = segmentCount - 1;
                    }
                    else if (boobies < 50)
                    {
                        segment800 = true;
                        segment400 = true;
                        segment8000 = true;
                        segment10000 = true;
                        segmentCount = segmentCount - 1;
                    }
                    else if (boobies < 75)
                    {
                        segment10 = true;
                        segment20 = true;
                        segment80000 = true;
                    }
                    else
                    {
                        segment1 = true;
                        segment2 = true;
                        segment80000 = true;
                    }
                }
            }
            // I'm sorry Tartt but I am leaving the stream during Lobates. They bore the hell out of me and frankly I can think of a whole host of things I'd rather be doing, including playing with a rubber band. Please
            else if (structureRNG >= 26 && structureRNG < 32)
            {
                wii::os::OSReport("!!! Lobate Structure is generating !!!\n");
                if (structureMiscRNG <= 66)
                {
                    if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment8000 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 50)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment80 = true;
                        segment400 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 75)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment20 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment1 = true;
                        segment20000 = true;
                        segmentCount = segmentCount - 2;
                    }
                }
                else
                {
                    if (structureMiscRNG >= 80)
                    {
                        segment20000 = true;
                    }
                    else
                    {
                        segment80000 = true;
                    }
                    if (boobies < 12.5)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment80 = true;
                        segment800 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 25)
                    {
                        segment4 = true;
                        segment8 = true;
                        segment200 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 37.5)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment80 = true;
                        segment2 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 50)
                    {
                        segment1000 = true;
                        segment2000 = true;
                        segment200 = true;
                        segment10 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 62.5)
                    {
                        segment2 = true;
                        segment4 = true;
                        segment200 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 75)
                    {
                        segment10 = true;
                        segment200 = true;
                        segment800 = true;
                        segment1000 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else if (boobies < 87.5)
                    {
                        segment4 = true;
                        segment10 = true;
                        segment80 = true;
                        segment800 = true;
                        segmentCount = segmentCount - 2;
                    }
                    else
                    {
                        segment2 = true;
                        segment80 = true;
                        segment1000 = true;
                        segment4000 = true;
                        segmentCount = segmentCount - 2;
                    }
                }
            }
            // Structure does not generate
            else
            {
                structureGenerated = false;
            }
            wii::os::OSReport("Structure check passed with RNG val %d, %d segments to generate remaining.\n", structureRNG, segmentCount);

            // Random segment generation + add'l doors
            while (segmentCount > 0 && i < 20)
            {
                int segmentID = spm::system::rand() % 16;
                switch (segmentID)
                {
                case 0:
                    // Segment 400
                    if ((segment400) ||
                        (segment800) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment4000) ||
                        (segment1000 && segment8000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment2 && segment80) ||
                        (segment1 && segment4 && segment200 && segment1000) ||
                        (segment1 && segment4 && segment8 && segment2000) ||
                        (segment1 && segment80) ||
                        (segment40 && segment80) ||
                        (segment4 && segment8 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment4 && segment20 && segment80) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment20 && segment200 && segment1000) ||
                        (segment80 && segment4 && segment200 && segment4000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment100 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment400 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 1;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg400 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 1:
                    // Segment 1
                    if ((segment1) ||
                        (segment2) ||
                        (segment4 && segment8) ||
                        (segment4 && segment10) ||
                        (segment4 && segment20) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment80 && segment800) ||
                        (segment4 && segment200 && segment400 && segment1000) ||
                        (segment8 && segment400 && segment1000 && segment2000) ||
                        (segment80 && segment400) ||
                        (segment40 && segment80) ||
                        (segment80 && segment1000 && segment2000) ||
                        (segment80 && segment1000 && segment4000) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment4 && segment200 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment10) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment4 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment1 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 17;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg1 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 2:
                    // Segment 40
                    if ((segment80 && segment800) ||
                        (segment40) ||
                        (segment2 && segment80) ||
                        (segment80 && segment400) ||
                        (segment1 && segment80) ||
                        (segment2000) ||
                        (segment8) ||
                        (segment100) ||
                        (segment4 && segment20 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment10) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment80 && segment4 && segment200 && segment4000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment80 && segment1000 && segment4000))
                    {
                        break;
                    }
                    else
                    {
                        segment40 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 10;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 11;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg40 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 3:
                    // Segment 1000
                    if ((segment400 && segment8000) ||
                        (segment400 && segment4000) ||
                        (segment800 && segment8000) ||
                        (segment1000) ||
                        (segment800 && segment4000) ||
                        (segment800 && segment2000) ||
                        (segment2000 && segment4000) ||
                        (segment2000 && segment8000) ||
                        (segment400 && segment2000) ||
                        (segment2 && segment10 && segment80 && segment200) ||
                        (segment4 && segment80 && segment200) ||
                        (segment40 && segment80 && segment2000) ||
                        (segment4 && segment8 && segment80 && segment2000) ||
                        (segment4 && segment8 && segment200 && segment2000) ||
                        (segment2 && segment8 && segment80 && segment2000) ||
                        (segment8 && segment10 && segment200 && segment2000) ||
                        (segment2 && segment80 && segment8000) ||
                        (segment2 && segment80 && segment4000) ||
                        (segment2 && segment80 && segment2000) ||
                        (segment1 && segment80 && segment8000) ||
                        (segment1 && segment80 && segment4000) ||
                        (segment1 && segment80 && segment2000) ||
                        (segment40 && segment80 && segment8000) ||
                        (segment40 && segment80 && segment4000) ||
                        (segment40 && segment80 && segment2000) ||
                        (segment10 && segment200 && segment800) ||
                        (segment10 && segment200 && segment400) ||
                        (segment10 && segment200 && segment2000) ||
                        (segment20 && segment200 && segment800) ||
                        (segment20 && segment200 && segment400) ||
                        (segment20 && segment200 && segment2000) ||
                        (segment100 && segment200 && segment800) ||
                        (segment100 && segment200 && segment2000) ||
                        (segment1 && segment4 && segment200 && segment400) ||
                        (segment4 && segment20 && segment80 && segment8000) ||
                        (segment2 && segment80 && segment200 && segment20) ||
                        (segment2 && segment80 && segment200 && segment100) ||
                        (segment1 && segment80 && segment200 && segment10) ||
                        (segment1 && segment80 && segment200 && segment20) ||
                        (segment1 && segment80 && segment200 && segment100) ||
                        (segment40 && segment80 && segment200 && segment10) ||
                        (segment40 && segment80 && segment200 && segment20) ||
                        (segment40 && segment80 && segment200 && segment100) ||
                        (segment400 && segment200 && segment4 && segment2) ||
                        (segment400 && segment200 && segment4 && segment8) ||
                        (segment800 && segment200 && segment4 && segment1) ||
                        (segment800 && segment200 && segment4 && segment2) ||
                        (segment800 && segment200 && segment4 && segment8) ||
                        (segment2000 && segment200 && segment4 && segment1) ||
                        (segment2000 && segment200 && segment4 && segment2) ||
                        (segment2000 && segment200 && segment4 && segment8) ||
                        (segment8000 && segment80 && segment4 && segment10) ||
                        (segment8000 && segment80 && segment4 && segment8) ||
                        (segment4000 && segment80 && segment4 && segment20) ||
                        (segment4000 && segment80 && segment4 && segment10) ||
                        (segment4000 && segment80 && segment4 && segment8) ||
                        (segment2000 && segment80 && segment4 && segment20) ||
                        (segment2000 && segment80 && segment4 && segment10) ||
                        (segment2000 && segment80 && segment4 && segment8) ||
                        (segment100 && segment200 && segment400))
                    {
                        break;
                    }
                    else
                    {
                        segment1000 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 4;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 5;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg1000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 4:
                    // Segment 4
                    if ((segment1 && segment20) ||
                        (segment4) ||
                        (segment2 && segment10) ||
                        (segment1 && segment10) ||
                        (segment2 && segment20) ||
                        (segment2 && segment8) ||
                        (segment8 && segment10) ||
                        (segment8 && segment20) ||
                        (segment1 && segment8) ||
                        (segment80 && segment200 && segment800 && segment4000) ||
                        (segment80 && segment200 && segment1000) ||
                        (segment8 && segment40 && segment80) ||
                        (segment8 && segment100 && segment200) ||
                        (segment8 && segment80 && segment1000 && segment2000) ||
                        (segment8 && segment200 && segment1000 && segment2000) ||
                        (segment8 && segment80 && segment800 && segment2000) ||
                        (segment8 && segment200 && segment2000 && segment4000) ||
                        (segment20 && segment80 && segment800) ||
                        (segment10 && segment80 && segment800) ||
                        (segment8 && segment80 && segment800) ||
                        (segment20 && segment80 && segment400) ||
                        (segment10 && segment80 && segment400) ||
                        (segment8 && segment80 && segment400) ||
                        (segment20 && segment40 && segment80) ||
                        (segment10 && segment40 && segment80) ||
                        (segment1 && segment200 && segment4000) ||
                        (segment2 && segment200 && segment4000) ||
                        (segment8 && segment200 && segment4000) ||
                        (segment1 && segment200 && segment8000) ||
                        (segment2 && segment200 && segment8000) ||
                        (segment8 && segment200 && segment8000) ||
                        (segment1 && segment100 && segment200) ||
                        (segment1 && segment200 && segment400 && segment1000) ||
                        (segment20 && segment80 && segment1000 && segment8000) ||
                        (segment800 && segment80 && segment200 && segment8000) ||
                        (segment800 && segment80 && segment200 && segment100) ||
                        (segment400 && segment80 && segment200 && segment4000) ||
                        (segment400 && segment80 && segment200 && segment8000) ||
                        (segment400 && segment80 && segment200 && segment100) ||
                        (segment40 && segment80 && segment200 && segment4000) ||
                        (segment40 && segment80 && segment200 && segment8000) ||
                        (segment40 && segment80 && segment200 && segment100) ||
                        (segment400 && segment1000 && segment200 && segment2) ||
                        (segment400 && segment1000 && segment200 && segment8) ||
                        (segment800 && segment1000 && segment200 && segment1) ||
                        (segment800 && segment1000 && segment200 && segment2) ||
                        (segment800 && segment1000 && segment200 && segment8) ||
                        (segment2000 && segment1000 && segment200 && segment1) ||
                        (segment2000 && segment1000 && segment200 && segment2) ||
                        (segment2000 && segment1000 && segment200 && segment8) ||
                        (segment8000 && segment1000 && segment80 && segment10) ||
                        (segment8000 && segment1000 && segment80 && segment8) ||
                        (segment4000 && segment1000 && segment80 && segment20) ||
                        (segment4000 && segment1000 && segment80 && segment10) ||
                        (segment4000 && segment1000 && segment80 && segment8) ||
                        (segment2000 && segment1000 && segment80 && segment20) ||
                        (segment2000 && segment1000 && segment80 && segment10) ||
                        (segment2000 && segment1000 && segment80 && segment8) ||
                        (segment2 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment4 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 20;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 21;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg4 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 5:
                    // Segment 100
                    if ((segment200 && segment4000) ||
                        (segment100) ||
                        (segment10 && segment200) ||
                        (segment100 && segment200) ||
                        (segment20 && segment200) ||
                        (segment200 && segment8000) ||
                        (segment2000) ||
                        (segment8) ||
                        (segment40) ||
                        (segment1 && segment4 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment2 && segment80 && segment1000 && segment200) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment800 && segment80 && segment4 && segment200) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment200 && segment800 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment100 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 14;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 15;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg100 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 6:
                    // Segment 8000
                    if ((segment4000) ||
                        (segment8000) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment800) ||
                        (segment1000 && segment400) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment10 && segment200) ||
                        (segment4 && segment20 && segment80 && segment1000) ||
                        (segment4 && segment8 && segment20 && segment2000) ||
                        (segment20 && segment200) ||
                        (segment100 && segment200) ||
                        (segment4 && segment8 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment1 && segment4 && segment200) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment800 && segment80 && segment4 && segment200) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment40 && segment80 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment8000 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 8;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg8000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 7:
                    // Segment 20
                    if ((segment10) ||
                        (segment20) ||
                        (segment4 && segment8) ||
                        (segment2 && segment4) ||
                        (segment1 && segment4) ||
                        (segment4 && segment80 && segment800) ||
                        (segment200 && segment4000) ||
                        (segment4 && segment80 && segment1000 && segment8000) ||
                        (segment8 && segment1000 && segment2000 && segment8000) ||
                        (segment200 && segment8000) ||
                        (segment100 && segment200) ||
                        (segment200 && segment1000 && segment2000) ||
                        (segment200 && segment800 && segment1000) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment2 && segment80 && segment1000 && segment200) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment40 && segment80))
                    {
                        break;
                    }
                    else
                    {
                        segment20 = true;
                        activeDoorCount = activeDoorCount + 1;
                        doorOn[activeDoorCount] = 24;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg20 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 8:
                    // Segment 800
                    if ((segment400) ||
                        (segment800) ||
                        (segment80 && segment2) ||
                        (segment80 && segment40) ||
                        (segment80 && segment1) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment4000) ||
                        (segment1000 && segment8000) ||
                        (segment4 && segment8 && segment80) ||
                        (segment4 && segment10 && segment80) ||
                        (segment4 && segment20 && segment80) ||
                        (segment4 && segment80 && segment200 && segment4000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment20 && segment200 && segment1000) ||
                        (segment80 && segment4 && segment200 && segment8000) ||
                        (segment80 && segment4 && segment200 && segment100) ||
                        (segment1000 && segment200 && segment4 && segment1) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment100 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment800 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg800 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 9:
                    // Segment 80
                    if ((segment2 && segment800) ||
                        (segment80) ||
                        (segment1 && segment40) ||
                        (segment40 && segment400) ||
                        (segment2 && segment400) ||
                        (segment1 && segment800) ||
                        (segment1 && segment400) ||
                        (segment40 && segment1000 && segment2000) ||
                        (segment40 && segment800) ||
                        (segment4 && segment8 && segment40) ||
                        (segment2 && segment40) ||
                        (segment4 && segment200 && segment1000) ||
                        (segment4 && segment20 && segment1000 && segment8000) ||
                        (segment2 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment800 && segment4000) ||
                        (segment2 && segment10 && segment200 && segment1000) ||
                        (segment2 && segment1000 && segment8000) ||
                        (segment2 && segment1000 && segment4000) ||
                        (segment2 && segment1000 && segment2000) ||
                        (segment1 && segment1000 && segment8000) ||
                        (segment1 && segment1000 && segment4000) ||
                        (segment1 && segment1000 && segment2000) ||
                        (segment40 && segment1000 && segment8000) ||
                        (segment40 && segment1000 && segment4000) ||
                        (segment4 && segment20 && segment800) ||
                        (segment4 && segment20 && segment400) ||
                        (segment4 && segment20 && segment40) ||
                        (segment4 && segment10 && segment800) ||
                        (segment4 && segment10 && segment400) ||
                        (segment4 && segment10 && segment40) ||
                        (segment4 && segment8 && segment800) ||
                        (segment2 && segment1000 && segment200 && segment20) ||
                        (segment2 && segment1000 && segment200 && segment100) ||
                        (segment1 && segment1000 && segment200 && segment10) ||
                        (segment1 && segment1000 && segment200 && segment20) ||
                        (segment1 && segment1000 && segment200 && segment100) ||
                        (segment40 && segment1000 && segment200 && segment10) ||
                        (segment40 && segment1000 && segment200 && segment20) ||
                        (segment40 && segment1000 && segment200 && segment100) ||
                        (segment800 && segment4 && segment200 && segment8000) ||
                        (segment800 && segment4 && segment200 && segment100) ||
                        (segment400 && segment4 && segment200 && segment4000) ||
                        (segment400 && segment4 && segment200 && segment8000) ||
                        (segment400 && segment4 && segment200 && segment100) ||
                        (segment40 && segment4 && segment200 && segment4000) ||
                        (segment40 && segment4 && segment200 && segment8000) ||
                        (segment40 && segment4 && segment200 && segment100) ||
                        (segment8000 && segment1000 && segment4 && segment10) ||
                        (segment8000 && segment1000 && segment4 && segment8) ||
                        (segment4000 && segment1000 && segment4 && segment20) ||
                        (segment4000 && segment1000 && segment4 && segment10) ||
                        (segment4000 && segment1000 && segment4 && segment8) ||
                        (segment2000 && segment1000 && segment4 && segment20) ||
                        (segment2000 && segment1000 && segment4 && segment10) ||
                        (segment2000 && segment1000 && segment4 && segment8) ||
                        (segment4 && segment8 && segment400))
                    {
                        break;
                    }
                    else
                    {
                        segment80 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg80 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 10:
                    // Segment 2
                    if ((segment1) ||
                        (segment2) ||
                        (segment80 && segment800) ||
                        (segment40 && segment80) ||
                        (segment80 && segment400) ||
                        (segment4 && segment8) ||
                        (segment4 && segment10) ||
                        (segment4 && segment20) ||
                        (segment80 && segment1000 && segment2000) ||
                        (segment80 && segment1000 && segment4000) ||
                        (segment80 && segment1000 && segment8000) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment10 && segment80 && segment200 && segment1000) ||
                        (segment4 && segment200 && segment8000) ||
                        (segment80 && segment1000 && segment200 && segment20) ||
                        (segment80 && segment1000 && segment200 && segment100) ||
                        (segment400 && segment1000 && segment200 && segment4) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment4 && segment100 && segment200))
                    {
                        break;
                    }
                    else
                    {
                        segment2 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg2 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 11:
                    // Segment 2000
                    if ((segment8) ||
                        (segment2000) ||
                        (segment800 && segment1000) ||
                        (segment1000 && segment4000) ||
                        (segment400 && segment1000) ||
                        (segment1000 && segment8000) ||
                        (segment40) ||
                        (segment100) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment10 && segment200 && segment1000) ||
                        (segment1000 && segment200 && segment4 && segment1) ||
                        (segment1000 && segment200 && segment4 && segment2) ||
                        (segment1000 && segment200 && segment4 && segment8) ||
                        (segment1000 && segment80 && segment4 && segment20) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment20 && segment200 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment2000 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg2000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 12:
                    // Segment 8
                    if ((segment2000) ||
                        (segment8) ||
                        (segment2 && segment4) ||
                        (segment4 && segment10) ||
                        (segment1 && segment4) ||
                        (segment4 && segment20) ||
                        (segment40) ||
                        (segment100) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment4 && segment200 && segment4000) ||
                        (segment400 && segment1000 && segment200 && segment4) ||
                        (segment800 && segment1000 && segment200 && segment4) ||
                        (segment2000 && segment1000 && segment200 && segment4) ||
                        (segment8000 && segment1000 && segment80 && segment4) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment200 && segment8000))
                    {
                        break;
                    }
                    else
                    {
                        segment8 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg8 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 13:
                    // Segment 4000
                    if ((segment8000) ||
                        (segment4000) ||
                        (segment10 && segment200) ||
                        (segment100 && segment200) ||
                        (segment20 && segment200) ||
                        (segment1000 && segment2000) ||
                        (segment1000 && segment800) ||
                        (segment1000 && segment400) ||
                        (segment4 && segment8 && segment200) ||
                        (segment2 && segment4 && segment200) ||
                        (segment1 && segment4 && segment200) ||
                        (segment4 && segment80 && segment200 && segment800) ||
                        (segment2 && segment80 && segment1000) ||
                        (segment1 && segment80 && segment1000) ||
                        (segment400 && segment80 && segment4 && segment200) ||
                        (segment40 && segment80 && segment4 && segment200) ||
                        (segment1000 && segment80 && segment4 && segment20) ||
                        (segment1000 && segment80 && segment4 && segment10) ||
                        (segment1000 && segment80 && segment4 && segment8) ||
                        (segment40 && segment80 && segment1000))
                    {
                        break;
                    }
                    else
                    {
                        segment4000 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg4000 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 14:
                    // Segment 200
                    if ((segment10 && segment4000) ||
                        (segment200) ||
                        (segment20 && segment100) ||
                        (segment100 && segment8000) ||
                        (segment10 && segment8000) ||
                        (segment20 && segment4000) ||
                        (segment20 && segment8000) ||
                        (segment100 && segment1000 && segment2000) ||
                        (segment100 && segment200 && segment2000) ||
                        (segment4 && segment8 && segment100) ||
                        (segment10 && segment100) ||
                        (segment100 && segment4000) ||
                        (segment4 && segment80 && segment1000) ||
                        (segment1 && segment4 && segment400 && segment1000) ||
                        (segment10 && segment400 && segment1000) ||
                        (segment4 && segment80 && segment800 && segment4000) ||
                        (segment2 && segment10 && segment80 && segment1000) ||
                        (segment10 && segment400 && segment1000) ||
                        (segment10 && segment800 && segment1000) ||
                        (segment10 && segment1000 && segment2000) ||
                        (segment20 && segment400 && segment1000) ||
                        (segment20 && segment800 && segment1000) ||
                        (segment20 && segment1000 && segment2000) ||
                        (segment100 && segment400 && segment1000) ||
                        (segment100 && segment800 && segment1000) ||
                        (segment1 && segment4 && segment4000) ||
                        (segment1 && segment4 && segment8000) ||
                        (segment1 && segment4 && segment100) ||
                        (segment2 && segment4 && segment4000) ||
                        (segment2 && segment4 && segment8000) ||
                        (segment2 && segment4 && segment100) ||
                        (segment4 && segment8 && segment4000) ||
                        (segment2 && segment80 && segment1000 && segment20) ||
                        (segment2 && segment80 && segment1000 && segment100) ||
                        (segment1 && segment80 && segment1000 && segment10) ||
                        (segment1 && segment80 && segment1000 && segment20) ||
                        (segment1 && segment80 && segment1000 && segment100) ||
                        (segment40 && segment80 && segment1000 && segment10) ||
                        (segment40 && segment80 && segment1000 && segment20) ||
                        (segment40 && segment80 && segment1000 && segment100) ||
                        (segment800 && segment80 && segment4 && segment8000) ||
                        (segment800 && segment80 && segment4 && segment100) ||
                        (segment400 && segment80 && segment4 && segment4000) ||
                        (segment400 && segment80 && segment4 && segment8000) ||
                        (segment400 && segment80 && segment4 && segment100) ||
                        (segment40 && segment80 && segment4 && segment4000) ||
                        (segment40 && segment80 && segment4 && segment8000) ||
                        (segment40 && segment80 && segment4 && segment100) ||
                        (segment400 && segment1000 && segment4 && segment2) ||
                        (segment400 && segment1000 && segment4 && segment8) ||
                        (segment800 && segment1000 && segment4 && segment1) ||
                        (segment800 && segment1000 && segment4 && segment2) ||
                        (segment800 && segment1000 && segment4 && segment8) ||
                        (segment2000 && segment1000 && segment4 && segment1) ||
                        (segment2000 && segment1000 && segment4 && segment2) ||
                        (segment2000 && segment1000 && segment4 && segment8) ||
                        (segment4 && segment8 && segment8000))
                    {
                        break;
                    }
                    else
                    {
                        segment200 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg200 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;

                case 15:
                    // Segment 10
                    if ((segment20) ||
                        (segment10) ||
                        (segment200 && segment4000) ||
                        (segment100 && segment200) ||
                        (segment200 && segment8000) ||
                        (segment4 && segment8) ||
                        (segment2 && segment4) ||
                        (segment1 && segment4) ||
                        (segment200 && segment1000 && segment2000) ||
                        (segment200 && segment800 && segment1000) ||
                        (segment200 && segment400 && segment1000) ||
                        (segment2 && segment80 && segment200 && segment1000) ||
                        (segment4 && segment80 && segment800) ||
                        (segment4 && segment80 && segment400) ||
                        (segment1 && segment80 && segment1000 && segment200) ||
                        (segment40 && segment80 && segment1000 && segment200) ||
                        (segment8000 && segment1000 && segment80 && segment4) ||
                        (segment4000 && segment1000 && segment80 && segment4) ||
                        (segment2000 && segment1000 && segment80 && segment4) ||
                        (segment4 && segment40 && segment80))
                    {
                        break;
                    }
                    else
                    {
                        segment10 = true;
                        segmentCount = segmentCount - 1;
                        wii::os::OSReport("#%d: Seg10 successful. %d remaining.\n", i, segmentCount);
                    }
                    break;
                }
                ++i;
            }

            // Room layout cleanup: choose from possible doors and add pipes
            // Pipes
            if (!structureGenerated)
            {
                int pipeRNG = spm::system::rand() % 133;
                if (pipeRNG >= 0 && pipeRNG < 39)
                {
                    segment20000 = true;
                }
                else if (pipeRNG >= 39 && pipeRNG < 76)
                {
                    segment80000 = true;
                }
                else if (pipeRNG >= 76 && pipeRNG < 80 && segment1 && segment20)
                {
                    segment40000 = true;
                }
                else if (pipeRNG >= 80 && pipeRNG < 82 && segment400 && segment8000)
                {
                    segment10000 = true;
                }
                else if (pipeRNG >= 82 && pipeRNG < 83)
                {
                    segment20000 = true;
                    segment80000 = true;
                }
                else if (pipeRNG >= 83 && pipeRNG < 86 && segment1 && segment20 && segment400 && segment8000)
                {
                    segment10000 = true;
                    segment20000 = true;
                    segment40000 = true;
                    segment80000 = true;
                }
            }

            // Add all segment values
            if (segment400)
            {
                roomDecCode = roomDecCode + 1024;
            }
            if (segment800)
            {
                roomDecCode = roomDecCode + 2048;
            }
            if (segment1000)
            {
                roomDecCode = roomDecCode + 4096;
            }
            if (segment2000)
            {
                roomDecCode = roomDecCode + 8192;
            }
            if (segment4000)
            {
                roomDecCode = roomDecCode + 16384;
            }
            if (segment8000)
            {
                roomDecCode = roomDecCode + 32768;
            }
            if (segment40)
            {
                roomDecCode = roomDecCode + 64;
            }
            if (segment80)
            {
                roomDecCode = roomDecCode + 128;
            }
            if (segment100)
            {
                roomDecCode = roomDecCode + 256;
            }
            if (segment200)
            {
                roomDecCode = roomDecCode + 512;
            }
            if (segment1)
            {
                roomDecCode = roomDecCode + 1;
            }
            if (segment2)
            {
                roomDecCode = roomDecCode + 2;
            }
            if (segment4)
            {
                roomDecCode = roomDecCode + 4;
            }
            if (segment8)
            {
                roomDecCode = roomDecCode + 8;
            }
            if (segment10)
            {
                roomDecCode = roomDecCode + 16;
            }
            if (segment20)
            {
                roomDecCode = roomDecCode + 32;
            }

            wii::os::OSReport("Finished generating. Roomcode w/o pipes is %d.\n", roomDecCode);

            if (segment10000)
            {
                roomDecCode = roomDecCode + 65536;
            }
            if (segment20000)
            {
                roomDecCode = roomDecCode + 131072;
            }
            if (segment40000)
            {
                roomDecCode = roomDecCode + 262144;
            }
            if (segment80000)
            {
                roomDecCode = roomDecCode + 524288;
            }

            if (currentFloor == 198)
            {
                roomDecCode = 0;
                activeDoorCount = 19;
            }
        }

        // Doors
        int successfulDoors = 0;
        int doorExit = 0;
        int doorEntrance = 0;
        int doorId = 0;
        bool currentDoorSuccess = false;
        bool doorOnToggleableSegment = false;
        while (successfulDoors != 2)
        {
            currentDoorSuccess = false;
            doorOnToggleableSegment = false;
            doorId = spm::system::rand() % 32 + 1;
            while (doorEntrance == doorId)
            {
                doorId = spm::system::rand() % 32 + 1;
            }
            wii::os::OSReport("Generated doorId %d.\n", doorId);
            if (doorId == 1)
            {
                doorOnToggleableSegment = true;
                if (segment400)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 4 || doorId == 5)
            {
                doorOnToggleableSegment = true;
                if (segment1000)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 8)
            {
                doorOnToggleableSegment = true;
                if (segment8000)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 10 || doorId == 11)
            {
                doorOnToggleableSegment = true;
                if (segment40)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 14 || doorId == 15)
            {
                doorOnToggleableSegment = true;
                if (segment100)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 17)
            {
                doorOnToggleableSegment = true;
                if (segment1)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 20 || doorId == 21)
            {
                doorOnToggleableSegment = true;
                if (segment4)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            if (doorId == 24)
            {
                doorOnToggleableSegment = true;
                if (segment20)
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId %d passed segment check.\n", doorId);
                }
            }
            wii::os::OSReport("All toggleable segment checks finished; doorOnToggleableSegment is %d, successfulDoors is %d, currentDoorSuccess is %d.\n", doorOnToggleableSegment, successfulDoors, currentDoorSuccess);
            if (!doorOnToggleableSegment)
            {
                if (doorId >= 25)
                {
                    if (boobies > 35)
                    {
                        successfulDoors = successfulDoors + 1;
                        currentDoorSuccess = true;
                        wii::os::OSReport("doorId was not on a toggleable segment; successfulDoors is %d.\n", successfulDoors);
                    }
                }
                else
                {
                    successfulDoors = successfulDoors + 1;
                    currentDoorSuccess = true;
                    wii::os::OSReport("doorId was not on a toggleable segment; successfulDoors is %d.\n", successfulDoors);
                }
            }
            if (currentDoorSuccess)
            {
                if (successfulDoors == 1)
                {
                    doorEntrance = doorId;
                }
                else if (successfulDoors == 2)
                {
                    doorExit = doorId;
                }
            }
            wii::os::OSReport("doorEntrance: %d. doorExit: %d.\n", doorEntrance, doorExit);
        }

        // NEW Super Item Substitution Bros. Wii
        i = 0;
        currentFloor = spm::swdrv::swByteGet(1);
        int itemRarity = 0; // Common by default. 1 is Uncommon, 2 is Rare
        int n = 0;
        int itemArraySize = 0;
        int itemSubrarity = 0;
        int itemId = 0;
        rfcItems[0] = 0;
        rfcItems[1] = 0;
        rfcItems[2] = 0;
        bool itemAssigned = false;
        if (currentFloor > 100)
        {
            currentFloor = currentFloor - 100;
        }
        // Get 3 items.
        for (i = 0; i < 3; ++i)
        {
            itemRarity = 0;
            n = 0;
            // Determine item rarity. One success makes it Uncommon, two successes makes it Rare, none makes it Common.
            do
            {
                n = n + 1;
                rand100Num = spm::system::rand() % 100;
                if (rand100Num < currentFloor)
                {
                    itemRarity = itemRarity + 1;
                }
            } while (n < 2);

            // Determine the item via subrarity. If subrarity check fails, loop again until it doesn't.
            do
            {
                itemAssigned = false;
                switch (itemRarity)
                {
                case 0:
                    itemArraySize = (sizeof(rfcCommon) / 8);              // Divide by 4 to get array item length, further divide by 2 to get item ID count
                    itemId = ((spm::system::rand() % itemArraySize) * 2); // Even numbered rand will always be an item ID
                    itemSubrarity = rfcCommon[itemId + 1];                // Subrarity for an item is indexed 1 position after its resp. item ID every time
                    itemId = rfcCommon[itemId];                           // Reassign itemId to true item ID
                    wii::os::OSReport("Item %d is Common. itemId is %d.\n", i, itemId);
                    break;
                case 1:
                    itemArraySize = (sizeof(rfcUncommon) / 8);
                    itemId = ((spm::system::rand() % itemArraySize) * 2);
                    itemSubrarity = rfcUncommon[itemId + 1];
                    itemId = rfcUncommon[itemId];
                    wii::os::OSReport("Item %d is Uncommon. itemId is %d.\n", i, itemId);
                    break;
                default:
                    itemArraySize = (sizeof(rfcRare) / 8);
                    itemId = ((spm::system::rand() % itemArraySize) * 2);
                    itemSubrarity = rfcRare[itemId + 1];
                    itemId = rfcRare[itemId];
                    wii::os::OSReport("Item %d is Rare. itemId is %d.\n", i, itemId);
                    break;
                }
                rand100Num = spm::system::rand() % 4;
                if (itemId != rfcItems[0] && itemId != rfcItems[1] && itemId != rfcItems[2] && itemSubrarity > rand100Num)
                {
                    rfcItems[i] = itemId; // Assigns the item to the rfcItems array
                    wii::os::OSReport("Item %d was assigned to rfcItems. itemId is %d.\n", i, itemId);
                    itemAssigned = true;
                }
            } while (!itemAssigned); // Loops until the rand is below the subrarity value AND item isn't identical to previously assigned items
        }

        i = 0;
        for (i = 0; i < 200; ++i)
        {
            // Placeholder item, true items are determined by above protocol and chosen upon opening chest
            spm::dan::dan_wp->dungeons[i].item = spm::itemdrv::itemTypeNameToId("ITEM_ID_COOK_TRIAL_PAN");

            // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
            spm::dan::dan_wp->dungeons[i].map = roomDecCode;

            // Door substitution
            spm::dan::dan_wp->dungeons[i].doors[0].enter = doorEntrance;
            spm::dan::dan_wp->dungeons[i].doors[0].exit = doorExit;
            spm::dan::dan_wp->dungeons[i].doorCount = 1;
        }

        i = 0;
        if (currentFloor == 0)
        {
            int enemyArrayVal = 1;
            int funnyVal = 0;
            for (int roomGens = 0; roomGens <= 199; roomGens = roomGens + 1)
            {
                // Enemy generation setup
                i = 0;
                int enemyTypes = 0;
                int arrayRNG = 0;
                int enemyAmt = 0;
                int enemyDifference = 0;
                int enemyMin = 0;
                int enemyMax = 0;
                //  int spDoorRNG = 0;
                int vsOdds = 0;
                int enemyGenRNG = spm::system::rand() % 100;
                if (enemyGenRNG <= 60)
                {
                    enemyTypes = 2;
                }
                else if (enemyGenRNG <= 85)
                {
                    enemyTypes = 1;
                }
                else
                {
                    enemyTypes = 3;
                }
                enemyConfigArray[enemyArrayVal] = enemyTypes;
                enemyArrayVal = enemyArrayVal + 1;
                // Enemy Substition Protocol
                while (i < enemyTypes)
                {
                    // Lv1
                    if (roomGens <= 24)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 75)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 95)
                        {
                            arrayRNG = spm::system::rand() % 37;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                if (lv2Odds[arrayRNG] == 195 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 37;
                                    if (lv2Odds[arrayRNG] == 195)
                                    {
                                        arrayRNG = spm::system::rand() % 37;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv2
                    else if (roomGens <= 149)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 20)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //    spDoorRNG = spm::system::rand() % 6;
                                    //   spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 85)
                        {
                            arrayRNG = spm::system::rand() % 37;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                if (lv2Odds[arrayRNG] == 195 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 37;
                                    if (lv2Odds[arrayRNG] == 195)
                                    {
                                        arrayRNG = spm::system::rand() % 37;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    //    spDoorRNG = spm::system::rand() % 4;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    //   spDoorRNG = spm::system::rand() % 6;
                                    //    spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes == 1)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 4;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv3
                    else if (roomGens <= 174)
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 15)
                        {
                            arrayRNG = spm::system::rand() % 37;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                if (lv2Odds[arrayRNG] == 195 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 37;
                                    if (lv2Odds[arrayRNG] == 195)
                                    {
                                        arrayRNG = spm::system::rand() % 37;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 80)
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 40;
                            vsOdds = spm::system::rand() % 100;
                            if (lv4Odds[arrayRNG] > vsOdds)
                            {
                                if (lv4Odds[arrayRNG] == 197 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 40;
                                    if (lv4Odds[arrayRNG] == 197)
                                    {
                                        arrayRNG = spm::system::rand() % 40;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv4Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv4Max[arrayRNG] - lv4Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv4Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv4Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv4Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv4Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                    if (lv4Names[arrayRNG] == 195 || lv4Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv4SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv4SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                    // Lv4
                    else
                    {
                        enemyGenRNG = spm::system::rand() % 100;
                        if (enemyGenRNG < 5)
                        {
                            arrayRNG = spm::system::rand() % 31;
                            vsOdds = spm::system::rand() % 100;
                            if (lv1Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv1Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv1Max[arrayRNG] - lv1Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv1Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv1Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv1Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv1Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv1Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv1SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv1SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 16)
                        {
                            arrayRNG = spm::system::rand() % 37;
                            vsOdds = spm::system::rand() % 100;
                            if (lv2Odds[arrayRNG] > vsOdds)
                            {
                                if (lv2Odds[arrayRNG] == 195 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 37;
                                    if (lv2Odds[arrayRNG] == 195)
                                    {
                                        arrayRNG = spm::system::rand() % 37;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv2Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv2Max[arrayRNG] - lv2Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv2Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv2Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv2Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv2Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv2Min[arrayRNG];
                                    if (lv2Names[arrayRNG] == 195 || lv2Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv2SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv2SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else if (enemyGenRNG < 50)
                        {
                            arrayRNG = spm::system::rand() % 46;
                            vsOdds = spm::system::rand() % 100;
                            if (lv3Odds[arrayRNG] > vsOdds)
                            {
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv3Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv3Max[arrayRNG] - lv3Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv3Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv3Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv3Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv3Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv3Min[arrayRNG];
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv3SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv3SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                        else
                        {
                            arrayRNG = spm::system::rand() % 40;
                            vsOdds = spm::system::rand() % 100;
                            if (lv4Odds[arrayRNG] > vsOdds)
                            {
                                if (lv4Odds[arrayRNG] == 197 && i > 0)
                                {
                                    arrayRNG = spm::system::rand() % 40;
                                    if (lv4Odds[arrayRNG] == 197)
                                    {
                                        arrayRNG = spm::system::rand() % 40;
                                    }
                                }
                                ++i;
                                enemyConfigArray[enemyArrayVal] = lv4Names[arrayRNG];
                                enemyArrayVal = enemyArrayVal + 1;
                                if (i == 0)
                                {
                                    enemyDifference = lv4Max[arrayRNG] - lv4Min[arrayRNG] + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                }
                                else
                                {
                                    if (lv4Min[arrayRNG] <= 3)
                                    {
                                        enemyMin = 1;
                                    }
                                    else if (lv4Min[arrayRNG] <= 5)
                                    {
                                        enemyMin = 2;
                                    }
                                    if (lv4Max[arrayRNG] <= 3)
                                    {
                                        enemyMax = 1;
                                    }
                                    else if (lv4Max[arrayRNG] <= 5)
                                    {
                                        enemyMax = 2;
                                    }
                                    else
                                    {
                                        enemyMax = 3;
                                    }
                                    enemyDifference = enemyMax - enemyMin + 1;
                                    enemyAmt = spm::system::rand() % enemyDifference + lv4Min[arrayRNG];
                                    if (lv4Names[arrayRNG] == 195 || lv4Names[arrayRNG] == 197)
                                    {
                                        enemyAmt = 1;
                                    }
                                }
                                if (enemyTypes < 3)
                                {
                                    if (boobies <= 25)
                                    {
                                        enemyAmt = enemyAmt + 1;
                                    }
                                    else if (boobies <= 75)
                                    {
                                        enemyAmt = enemyAmt + 2;
                                    }
                                    else
                                    {
                                        enemyAmt = enemyAmt + 3;
                                    }
                                }
                                enemyConfigArray[enemyArrayVal] = enemyAmt;
                                enemyArrayVal = enemyArrayVal + 1;
                                if (lv4SpR[arrayRNG] == 0)
                                {
                                    enemyConfigArray[enemyArrayVal] = 0;
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else if (lv4SpR[arrayRNG] == 1)
                                {
                                    // spDoorRNG = spm::system::rand() % 4;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR1Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                else
                                {
                                    // spDoorRNG = spm::system::rand() % 6;
                                    // spm::dan::dan_wp->dungeons[roomGens].enemies[i].pos = SpR2Doors[spDoorRNG];
                                    enemyConfigArray[enemyArrayVal] = 0; // Temp until I can fix SpR spawns
                                    enemyArrayVal = enemyArrayVal + 1;
                                }
                                funnyVal = (enemyArrayVal - 3);
                                wii::os::OSReport("Room #%d: Enemy name %d generated %d times. i = %d.\n", roomGens, enemyConfigArray[funnyVal], enemyAmt, i);
                            }
                        }
                    }
                }
                // Fill empty slots if enemy types < 3
                if (enemyTypes == 1)
                {
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                }
                else if (enemyTypes == 2)
                {
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                    enemyConfigArray[enemyArrayVal] = 0;
                    enemyArrayVal = enemyArrayVal + 1;
                }
            }
        }

        // Determine enemy data for current room
        if (moverRNG > 24)
        {
            for (i = 0; i < 200; ++i)
            {
                enemyArrayOffset = (currentFloor * 10 + 1);
                spm::dan::dan_wp->dungeons[i].enemyCount = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[i].enemies[1].name = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[i].enemies[1].num = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                spm::dan::dan_wp->dungeons[i].enemies[1].pos = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                if (enemyConfigArray[enemyArrayOffset] != 0)
                {
                    spm::dan::dan_wp->dungeons[i].enemies[2].name = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    spm::dan::dan_wp->dungeons[i].enemies[2].num = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    spm::dan::dan_wp->dungeons[i].enemies[2].pos = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    if (enemyConfigArray[enemyArrayOffset] != 0)
                    {
                        spm::dan::dan_wp->dungeons[i].enemies[3].name = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        spm::dan::dan_wp->dungeons[i].enemies[3].num = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        spm::dan::dan_wp->dungeons[i].enemies[3].pos = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                    }
                }
            }

            nextFloor = 0;
            nextEnemyCount = 0;
            nextName1 = 0;
            nextNum1 = 0;
            nextPos1 = 0;
            nextName2 = 0;
            nextNum2 = 0;
            nextPos2 = 0;
            nextName3 = 0;
            nextNum3 = 0;
            nextPos3 = 0;
            // Set up values for debug displays
            if (currentFloor != 199)
            {
                nextFloor = currentFloor + 1;
                enemyArrayOffset = ((nextFloor * 10) + 1);
                nextEnemyCount = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                nextName1 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                nextNum1 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                nextPos1 = enemyConfigArray[enemyArrayOffset];
                enemyArrayOffset = enemyArrayOffset + 1;
                if (enemyConfigArray[enemyArrayOffset] != 0)
                {
                    nextName2 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    nextNum2 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    nextPos2 = enemyConfigArray[enemyArrayOffset];
                    enemyArrayOffset = enemyArrayOffset + 1;
                    if (enemyConfigArray[enemyArrayOffset] != 0)
                    {
                        nextName3 = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        nextNum3 = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                        nextPos3 = enemyConfigArray[enemyArrayOffset];
                        enemyArrayOffset = enemyArrayOffset + 1;
                    }
                }
            }
        }
        //  Uncomment this and replace with any enemy name to add enemy to first 3 Floors. May break stuff sometimes
        spm::dan::dan_wp->dungeons[0].enemies[1].name = 114;
        spm::dan::dan_wp->dungeons[0].enemies[1].num = 2;
        spm::dan::dan_wp->dungeons[1].enemies[2].name = 496;
        spm::dan::dan_wp->dungeons[1].enemies[2].num = 2;
        //    spm::dan::dan_wp->dungeons[0].enemyCount = 4;

        // Replace Flimm inventory; this sets a number of random items from the custom rotenShopItemPool to be selectable across the entire Pit session.
        if (currentFloor == 0)
        {
            int poolItem = 0;
            for (i = 0; i < 67; i = i + 3)
            {
                poolItem = spm::system::rand() % 87;
                newRotenShopItems[i] = rotenShopItemPool[poolItem];
            }
            for (i = 0; i < 70; i++)
            {
                spm::dan::dan_rotenShopItems[i] = newRotenShopItems[i];
            }
        }

        // Mover logic
        if (moverRNG <= 24)
        {
            for (i = 0; i < 200; ++i)
            {
                spm::dan::dan_wp->dungeons[i].enemies[1].name = 99;
                spm::dan::dan_wp->dungeons[i].enemies[1].num = 1;
                spm::dan::dan_wp->dungeons[i].enemies[1].pos = 26;
                spm::dan::dan_wp->dungeons[i].enemyCount = 1;
                spm::dan::dan_wp->dungeons[i].doors[0].enter = 28;
                spm::dan::dan_wp->dungeons[i].doors[0].exit = 29;
                spm::dan::dan_wp->dungeons[i].map = 0;
            }
            danClearMapParts();
        }

        blessingRoomCounter = spm::swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            blessingRoomCounter = blessingRoomCounter - 1;
            spm::swdrv::swByteSet(1602, blessingRoomCounter);
            if (blessingRoomCounter == 0)
            {
                spm::swdrv::swByteSet(1600, 0);
            }
        }

        int paramitaTimer = spm::swdrv::swByteGet(1610);
        if (paramitaTimer > 0)
        {
            paramitaTimer = paramitaTimer + 2;
            spm::swdrv::swByteSet(1610, paramitaTimer);
        }

        // Free pit text
        spm::parse::parsePop();

        return EVT_RET_CONTINUE;
    }

    static const char *(*msgSearchReal)(const char *msgName);
    static void messagePatch()
    {
        msgSearchReal = patch::hookFunction(spm::msgdrv::msgSearch,
                                            [](const char *msgName)
                                            {
                                                if (msl::string::strcmp(msgName, "D100_entrance_03") == 0)
                                                    // flopsite pit 1st pipe interaction (should never be seen)
                                                    return D100_entrance_03;
                                                else if (msl::string::strcmp(msgName, "mac_kanban_004") == 0)
                                                    // flopside pit entrance sign
                                                    return mac_kanban_004;
                                                else
                                                    return msgSearchReal(msgName);
                                            });
    }

    // I literally had to port this entire thing just to override a condition that makes certain enemies force the Pit to fucking crash upon entry. Thank you Intelligent Systems, very cool
    s32 evt_dan_get_enemy_info_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;

        // Get dungeon and enemy index
        spm::evtmgr::EvtScriptCode *args = entry->pCurData;
        //    s32 no = spm::evtmgr_cmd::evtGetValue(entry, args[0]);
        s32 enemyIdx = spm::evtmgr_cmd::evtGetValue(entry, args[1]);
        spm::dan::DanDungeon *dungeon = spm::dan::dan_wp->dungeons + no;

        if ((enemyIdx < 0) || (enemyIdx >= 16))
        {
            // Return 0 for invalid enemies
            spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
            spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);

            return EVT_RET_CONTINUE;
        }
        else
        {
            // Get enemy
            spm::dan::DanEnemy *enemy = dungeon->enemies + enemyIdx;

            if (enemy->num > 0)
            {
                // Find template with correct tribe id
                s32 tribeId = enemy->name - 1;
                s32 i;
                spm::npcdrv::NPCEnemyTemplate *curTemplate = spm::npcdrv::npcEnemyTemplates;
                for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
                {
                    spm::npcdrv::npcEnemyTemplates[10].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[13].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[74].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[76].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[55].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[101].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[321].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[322].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[323].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[324].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[82].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[316].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[317].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[318].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[319].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[320].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[276].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[277].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[279].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[23].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[125].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[30].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[113].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[200].unknown_0x8 = 0;
                    spm::npcdrv::npcEnemyTemplates[332].unknown_0x8 = 0;

                    if (((curTemplate->unknown_0x8 & 1) == 0) && (curTemplate->tribeId == tribeId))
                        break;
                }
                assertf(i < NPCTEMPLATE_MAX, "みつかりませんでした[%d]", tribeId);

                // Return template id and num
                spm::evtmgr_cmd::evtSetValue(entry, args[2], i);
                spm::evtmgr_cmd::evtSetValue(entry, args[3], enemy->num);
            }
            else
            {
                // Return 0 for empty enemy slots
                spm::evtmgr_cmd::evtSetValue(entry, args[2], 0);
                spm::evtmgr_cmd::evtSetValue(entry, args[3], 0);
            }

            return EVT_RET_CONTINUE;
        }
    }

    static spm::seqdef::SeqFunc *seq_gameMainReal;

    void youSuckDisplay(spm::seqdrv::SeqWork *wp)
    {
        if (youSuck)
        {
            wii::gx::GXColor funnyColor = {255, 255, 255, 255};
            f32 scale = 2.8f;
            char buffer[50];
            const char *youSuckText = "YOU SUCK";
            msl::stdio::sprintf(buffer, "%s", youSuckText);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColor();
            f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
            spm::fontmgr::FontDrawString(x, 200.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void new_dan_gameover()
    {
        youSuck = true;
    }

    void merlunaBlessingDisplay(spm::seqdrv::SeqWork *wp)
    {
        blessingRoomCounter = spm::swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            roomOnHud = spm::swdrv::swGet(422);
            wii::gx::GXColor funnyColor = {18, 227, 178, 225};
            if (roomOnHud)
            {
                funnyColor = {18, 227, 178, 80};
            }
            f32 scale = 0.64f;
            char buffer[100];
            blessingNum = spm::swdrv::swByteGet(1600);
            houraiActivation = spm::swdrv::swGet(1671);
            // Iterate through exceptional cases that modify message text
            if (houraiActivation)
            {
                funnyColor = {227, 178, 18, 225};
                msl::stdio::sprintf(buffer, "Affliction: Phoenix's Tail");
                if (roomOnHud)
                {
                    funnyColor = {227, 178, 18, 80};
                }
            }
            else if (blessingNum == BlessId::MERLUNA_PARAMITA)
            {
                int paramitaTimer = spm::swdrv::swByteGet(1610);
                if (paramitaTimer > 20)
                {
                    paramitaTimer = paramitaTimer - 20;
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Active, %d)", paramitaTimer);
                }
                else if (paramitaTimer > 0)
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Cooldown, %d)", paramitaTimer);
                }
                else
                {
                    msl::stdio::sprintf(buffer, "Blessing: Paramita (Press B to use)", paramitaTimer);
                }
            }
            else
            {
                const char *blessingNames[] = {"DUMMY", "Spectre", "Hourai Doll", "Paramita"};
                const char *blessingDisp = blessingNames[blessingNum];
                msl::stdio::sprintf(buffer, "Blessing: %s", blessingDisp);
            }
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            spm::fontmgr::FontDrawString(-360, -170.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaBlessingNumDisplay(spm::seqdrv::SeqWork *wp)
    {
        blessingRoomCounter = spm::swdrv::swByteGet(1602);
        if (blessingRoomCounter > 0)
        {
            roomOnHud = spm::swdrv::swGet(422);
            wii::gx::GXColor funnyColor = {8, 110, 102, 225};
            if (roomOnHud)
            {
                funnyColor = {8, 110, 102, 80};
            }
            f32 scale = 0.64f;
            char buffer[50];
            houraiActivation = spm::swdrv::swGet(1671);
            if (houraiActivation)
            {
                funnyColor = {140, 110, 18, 225};
                if (roomOnHud)
                {
                    funnyColor = {140, 110, 18, 80};
                }
            }
            msl::stdio::sprintf(buffer, "Neutralizes in %d rooms", blessingRoomCounter);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            spm::fontmgr::FontDrawString(-360, -190.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaCurseDisplay(spm::seqdrv::SeqWork *wp)
    {
        curseNum = spm::swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            roomOnHud = spm::swdrv::swGet(422);
            wii::gx::GXColor funnyColor = {138, 0, 207, 225};
            if (roomOnHud)
            {
                funnyColor = {138, 0, 207, 80};
            }
            f32 scale = 0.64f;
            char buffer[100];
            const char *curseNames[] = {"DUMMY", "Tatarian Aster", "Vulnerability Hex", "Migraine"};
            const char *curseDisp = curseNames[curseNum];
            msl::stdio::sprintf(buffer, "Curse: %s", curseDisp);
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            spm::fontmgr::FontDrawString(-360, -170.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void merlunaCurseNumDisplay(spm::seqdrv::SeqWork *wp)
    {
        curseNum = spm::swdrv::swByteGet(1601);
        if (curseNum > 0)
        {
            roomOnHud = spm::swdrv::swGet(422);
            wii::gx::GXColor funnyColor = {71, 4, 99, 225};
            if (roomOnHud)
            {
                funnyColor = {71, 4, 99, 80};
            }
            f32 scale = 0.64f;
            char buffer[50];
            msl::stdio::sprintf(buffer, "Pay Merluna to clear");
            const char *msg = buffer;
            spm::fontmgr::FontDrawStart();
            spm::fontmgr::FontDrawEdge();
            spm::fontmgr::FontDrawColor(&funnyColor);
            spm::fontmgr::FontDrawScale(scale);
            spm::fontmgr::FontDrawNoiseOff();
            spm::fontmgr::FontDrawRainbowColorOff();
            spm::fontmgr::FontDrawString(-360, -190.0f, msg);
        }
        seq_gameMainReal(wp);
    }

    void textDisplay(spm::seqdrv::SeqWork *wp)
    {
        merlunaBlessingDisplay(wp);
        merlunaBlessingNumDisplay(wp);
        merlunaCurseDisplay(wp);
        merlunaCurseNumDisplay(wp);
        youSuckDisplay(wp);
    }

    static spm::seqdef::SeqFunc *seq_titleMainReal;
    static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
    {
        wii::gx::GXColor notgreen = {230, 116, 216, 255};
        f32 scale = 0.8f;
        const char *msg = "SPM Flipside Pit Randomizer beta v2.0";
        spm::fontmgr::FontDrawStart();
        spm::fontmgr::FontDrawEdge();
        spm::fontmgr::FontDrawColor(&notgreen);
        spm::fontmgr::FontDrawScale(scale);
        spm::fontmgr::FontDrawNoiseOff();
        spm::fontmgr::FontDrawRainbowColorOff();
        f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
        spm::fontmgr::FontDrawString(x, 200.0f, msg);
        seq_titleMainReal(wp);
    }
    static void titleScreenCustomTextPatch()
    {
        seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
        spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
        seq_gameMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main;
        spm::seqdef::seq_data[spm::seqdrv::SEQ_GAME].main = &textDisplay;
    }

    static void danOverwrite()
    {
        patch::hookFunction(spm::dan::evt_dan_read_data, evt_dan_read_data_new);
        writeBranchLink(spm::mapdrv::mapLoad, 0x3B8, dan_01BossRoomTex);
        writeBranchLink(spm::mot_damage::onHpEquals0, 0x68, handleHouraiDoll);
    }

    static void danYouSuck()
    {
        patch::hookFunction(spm::dan::danCountdownDone, new_dan_gameover);
    }

    static void danDontFuckingCrash()
    {
        patch::hookFunction(spm::dan::evt_dan_get_enemy_info, evt_dan_get_enemy_info_new);
    }

    static void dimenPatch()
    {
        patch::hookFunction(spm::temp_unk::dimen_determine_move_pos, dimen_determine_move_pos_new);
    }

    static void evtNpcPatchAllEnemies()
    {
        patch::hookFunction(spm::evt_npc::evt_npc_set_rgba, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });

        patch::hookFunction(spm::evt_npc::evt_npc_set_rgbacopy, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
        patch::hookFunction(spm::evt_npc::evt_npc_set_rgbacopytwo, [](spm::evtmgr::EvtEntry *entry, bool isFirstCall)
                            {
            (void)entry;
            (void)isFirstCall;
            return 2; });
    }

    static void danPatchPitEnemies()
    {
        // Define default animposes for new enemies
        spm::npcdrv::npcTribes[5].animPoseName = "e_kuribo_y";     // Unused Spiked Goomba --> Spiked Gloomba
        spm::npcdrv::npcTribes[2].animPoseName = "e_kuribo_h";     // Dark Goomba --> Hyper Goomba
        spm::npcdrv::npcTribes[6].animPoseName = "e_kuribo_h";     // Dark Spiked Goomba --> Hyper Spiked Goomba
        spm::npcdrv::npcTribes[8].animPoseName = "e_kuribo_h";     // Dark Paragoomba --> Hyper Paragoomba
        spm::npcdrv::npcTribes[16].animPoseName = "e_nokoteki_d";  // Dark Koopa --> Dark Koopa
        spm::npcdrv::npcTribes[24].animPoseName = "e_nokoteki_d";  // Dark Paratroopa --> Dark Paratroopa
        spm::npcdrv::npcTribes[446].animPoseName = "e_cheririn_a"; // Dark Cherbil --> Ash Cherbil
        spm::npcdrv::npcTribes[38].animPoseName = "e_togezb";      // Dark Spiny --> Sky-Blue Spiny
        spm::npcdrv::npcTribes[29].animPoseName = "e_mer";         // Dark Spike Top --> Red Spike Top
        spm::npcdrv::npcTribes[98].animPoseName = "e_chorobon_g";  // Dark Fuzzy --> Green Fuzzy
        spm::npcdrv::npcTribes[43].animPoseName = "e_karon_d";     // Dark Dull Bones --> Dark Bones
        spm::npcdrv::npcTribes[44].animPoseName = "e_karon_d";     // Dark Bones projectile
        spm::npcdrv::npcTribes[111].animPoseName = "e_kmond";      // Dark Ruff Puff --> Dark Puff
        spm::npcdrv::npcTribes[95].animPoseName = "e_gabow";       // Dark Clubba --> White Clubba
        spm::npcdrv::npcTribes[66].animPoseName = "e_kames";       // Dark Magikoopa --> Shady Magikoopa
        spm::npcdrv::npcTribes[67].animPoseName = "e_kames";       // Dark Broom Magikoopa --> Shady Broom Magikoopa
                                                                   //    spm::npcdrv::npcTribes[124].animPoseName = "e_wanwan_g";   // Dark Chomp --> Gold Chomp
                                                                   //    spm::npcdrv::npcTribes[124].partsList[0].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[1].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[2].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[3].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[4].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[5].animPoseName = "e_wanwan_g";
                                                                   //    spm::npcdrv::npcTribes[124].partsList[6].animPoseName = "e_wanwan_g";
        spm::npcdrv::npcTribes[167].animPoseName = "e_ntl_p";      // Dark Tileoid --> Tileoid PU
        spm::npcdrv::npcTribes[220].animPoseName = "e_nin_d";      // Dark Ninjoe --> Ninjeremiah
        spm::npcdrv::npcTribes[19].animPoseName = "e_togenokd";    // Dark Koopatrol --> Dark Koopatrol
        spm::npcdrv::npcTribes[171].animPoseName = "e_jyama_b";    // Dark Jawbus --> Bawbus
        spm::npcdrv::npcTribes[10].animPoseName = "e_k_kuribo";    // Dark Headbonk Goomba --> Kamikaze Goomba
        spm::npcdrv::npcTribes[225].animPoseName = "e_tesita_bt";  // Dark Skellobit --> Skellobyte
        spm::npcdrv::npcTribes[228].animPoseName = "e_tesita_sb";  // Dark Spiky Skellobit --> Spiky Skellobyte
        spm::npcdrv::npcTribes[49].animPoseName = "e_buross_h";    // Dark Hammer Bro --> Shady Hammer Bro
        spm::npcdrv::npcTribes[55].animPoseName = "e_buross_b";    // Dark Boomerang Bro --> Shady Boomerang Bro
        spm::npcdrv::npcTribes[56].animPoseName = "e_buross_b";    // Shady Boomerang Bro projectile
        spm::npcdrv::npcTribes[61].animPoseName = "e_burosu_i";    // Dark Fire Bro --> Ice Bro
        spm::npcdrv::npcTribes[62].animPoseName = "e_burosu_i";    // Ice Bro projectile
        spm::npcdrv::npcTribes[101].animPoseName = "e_sinemoh";    // Dark Cleft --> Hyper Cleft
        spm::npcdrv::npcTribes[75].animPoseName = "e_k_shoote4";   // Dark Striker --> Shady Striker

        // Adjust new NPC stats
        // Spinia
        spm::npcdrv::npcTribes[495].catchCardItemId = 344;
        spm::npcdrv::npcTribes[495].catchCardDefense = 5;
        spm::npcdrv::npcTribes[495].maxHp = 4;
        spm::npcdrv::npcTribes[495].killXp = 200;
        spm::npcdrv::npcTribes[495].coinDropChance = 100;
        spm::npcdrv::npcTribes[495].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[495].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[495].coinDropExtraMax = 1;
        spm::npcdrv::npcTribes[495].attackStrength = 1;
        spm::npcdrv::npcTribes[495].bounceEjection = 2;
        spm::npcdrv::npcTribes[495].jumpEjection = 0;

        // Spunia
        spm::npcdrv::npcTribes[496].catchCardItemId = 350;
        spm::npcdrv::npcTribes[496].catchCardDefense = 20;
        spm::npcdrv::npcTribes[496].maxHp = 9;
        spm::npcdrv::npcTribes[496].killXp = 600;
        spm::npcdrv::npcTribes[496].coinDropChance = 100;
        spm::npcdrv::npcTribes[496].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[496].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[496].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[496].attackStrength = 3;
        spm::npcdrv::npcTribes[496].bounceEjection = 2;
        spm::npcdrv::npcTribes[496].jumpEjection = 0;

        // Shady Koopa, ATK 5 DEF 3
        spm::npcdrv::npcTribes[465].catchCardItemId = 304;
        spm::npcdrv::npcTribes[465].catchCardDefense = 30;
        spm::npcdrv::npcTribes[465].maxHp = 15;
        spm::npcdrv::npcTribes[465].killXp = 300;
        spm::npcdrv::npcTribes[465].coinDropChance = 100;
        spm::npcdrv::npcTribes[465].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[465].coinDropExtraChance = 75;
        spm::npcdrv::npcTribes[465].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[465].attackStrength = 4;
        spm::npcdrv::npcTribes[465].bounceEjection = 1;
        spm::npcdrv::npcTribes[465].jumpEjection = 0;

        // Flip Shady Koopa, ATK 5 DEF 3
        spm::npcdrv::npcTribes[466].catchCardItemId = 304;
        spm::npcdrv::npcTribes[466].catchCardDefense = 30;
        spm::npcdrv::npcTribes[466].maxHp = 15;
        spm::npcdrv::npcTribes[466].killXp = 300;
        spm::npcdrv::npcTribes[466].coinDropChance = 100;
        spm::npcdrv::npcTribes[466].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[466].coinDropExtraChance = 75;
        spm::npcdrv::npcTribes[466].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[466].attackStrength = 4;
        spm::npcdrv::npcTribes[466].bounceEjection = 1;
        spm::npcdrv::npcTribes[466].jumpEjection = 0;

        // Spiked Gloomba, ATK 3
        spm::npcdrv::npcTribes[5].catchCardItemId = 336;
        spm::npcdrv::npcTribes[5].catchCardDefense = 6;
        spm::npcdrv::npcTribes[5].maxHp = 2;
        spm::npcdrv::npcTribes[5].killXp = 200;
        spm::npcdrv::npcTribes[5].coinDropChance = 100;
        spm::npcdrv::npcTribes[5].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[5].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[5].coinDropExtraMax = 1;
        spm::npcdrv::npcTribes[5].attackStrength = 3;

        // Green Magikoopa, DEF 2
        spm::npcdrv::npcTribes[473].catchCardItemId = 353;
        spm::npcdrv::npcTribes[473].catchCardDefense = 20;
        spm::npcdrv::npcTribes[473].maxHp = 6;
        spm::npcdrv::npcTribes[473].killXp = 600;
        spm::npcdrv::npcTribes[473].coinDropChance = 100;
        spm::npcdrv::npcTribes[473].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[473].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[473].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[473].attackStrength = 3;
        spm::npcdrv::npcTribes[473].bounceEjection = 2;
        spm::npcdrv::npcTribes[473].jumpEjection = 0;

        // Green Broom Magikoopa, DEF 2
        spm::npcdrv::npcTribes[474].catchCardItemId = 353;
        spm::npcdrv::npcTribes[474].catchCardDefense = 20;
        spm::npcdrv::npcTribes[474].maxHp = 6;
        spm::npcdrv::npcTribes[474].killXp = 600;
        spm::npcdrv::npcTribes[474].coinDropChance = 100;
        spm::npcdrv::npcTribes[474].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[474].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[474].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[474].attackStrength = 3;
        spm::npcdrv::npcTribes[474].bounceEjection = 2;
        spm::npcdrv::npcTribes[474].jumpEjection = 0;

        // Green Magi Projectile
        spm::npcdrv::npcTribes[475].catchCardItemId = 353;

        // White Magikoopa
        spm::npcdrv::npcTribes[476].catchCardItemId = 352;
        spm::npcdrv::npcTribes[476].catchCardDefense = 20;
        spm::npcdrv::npcTribes[476].maxHp = 18;
        spm::npcdrv::npcTribes[476].killXp = 600;
        spm::npcdrv::npcTribes[476].coinDropChance = 100;
        spm::npcdrv::npcTribes[476].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[476].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[476].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[476].attackStrength = 3;
        spm::npcdrv::npcTribes[476].bounceEjection = 2;
        spm::npcdrv::npcTribes[476].jumpEjection = 0;

        // White Broom Magikoopa
        spm::npcdrv::npcTribes[477].catchCardItemId = 352;
        spm::npcdrv::npcTribes[477].catchCardDefense = 20;
        spm::npcdrv::npcTribes[477].maxHp = 18;
        spm::npcdrv::npcTribes[477].killXp = 600;
        spm::npcdrv::npcTribes[477].coinDropChance = 100;
        spm::npcdrv::npcTribes[477].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[477].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[477].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[477].attackStrength = 3;
        spm::npcdrv::npcTribes[477].bounceEjection = 2;
        spm::npcdrv::npcTribes[477].jumpEjection = 0;

        // White Magi Projectile
        spm::npcdrv::npcTribes[478].catchCardItemId = 352;

        // Red Magikoopa
        spm::npcdrv::npcTribes[479].catchCardItemId = 351;
        spm::npcdrv::npcTribes[479].catchCardDefense = 20;
        spm::npcdrv::npcTribes[479].maxHp = 6;
        spm::npcdrv::npcTribes[479].killXp = 600;
        spm::npcdrv::npcTribes[479].coinDropChance = 100;
        spm::npcdrv::npcTribes[479].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[479].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[479].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[479].attackStrength = 5;
        spm::npcdrv::npcTribes[479].bounceEjection = 2;
        spm::npcdrv::npcTribes[479].jumpEjection = 0;

        // Red Broom Magikoopa
        spm::npcdrv::npcTribes[480].catchCardItemId = 351;
        spm::npcdrv::npcTribes[480].catchCardDefense = 20;
        spm::npcdrv::npcTribes[480].maxHp = 5;
        spm::npcdrv::npcTribes[480].killXp = 600;
        spm::npcdrv::npcTribes[480].coinDropChance = 100;
        spm::npcdrv::npcTribes[480].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[480].coinDropExtraChance = 60;
        spm::npcdrv::npcTribes[480].coinDropExtraMax = 4;
        spm::npcdrv::npcTribes[480].attackStrength = 5;
        spm::npcdrv::npcTribes[480].bounceEjection = 2;
        spm::npcdrv::npcTribes[480].jumpEjection = 0;

        // Red Magi Projectile, ATK 6
        spm::npcdrv::npcTribes[481].catchCardItemId = 351;

        // Bleepboxer, ATK 3
        spm::npcdrv::npcTribes[504].catchCardItemId = 384;
        spm::npcdrv::npcTribes[504].catchCardDefense = 15;
        spm::npcdrv::npcTribes[504].maxHp = 12;
        spm::npcdrv::npcTribes[504].killXp = 500;
        spm::npcdrv::npcTribes[504].coinDropChance = 100;
        spm::npcdrv::npcTribes[504].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[504].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[504].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[504].attackStrength = 4;
        spm::npcdrv::npcTribes[504].bounceEjection = 2;
        spm::npcdrv::npcTribes[504].jumpEjection = 0;

        // Kilo Muth, ATK 2
        spm::npcdrv::npcTribes[506].catchCardItemId = 427;
        spm::npcdrv::npcTribes[506].catchCardDefense = 15;
        spm::npcdrv::npcTribes[506].maxHp = 25;
        spm::npcdrv::npcTribes[506].killXp = 700;
        spm::npcdrv::npcTribes[506].attackStrength = 2;
        spm::npcdrv::npcTribes[506].bounceEjection = 2;
        spm::npcdrv::npcTribes[506].jumpEjection = 0;

        // Mega Muth and Muth HP drop
        spm::npcdrv::npcTribes[257].maxHp = 50;
        spm::npcdrv::npcTribes[258].maxHp = 75;

        // Ice Bro nerf, ATK 3 DEF 2
        spm::npcdrv::npcTribes[61].catchCardDefense = 16;
        spm::npcdrv::npcTribes[61].maxHp = 10;
        spm::npcdrv::npcTribes[61].killXp = 800;
        spm::npcdrv::npcTribes[61].coinDropChance = 100;
        spm::npcdrv::npcTribes[61].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[61].coinDropExtraChance = 50;
        spm::npcdrv::npcTribes[61].coinDropExtraMax = 5;
        spm::npcdrv::npcTribes[61].attackStrength = 3;

        // Dark Puff nerf
        spm::npcdrv::npcTribes[111].catchCardDefense = 8;
        spm::npcdrv::npcTribes[111].maxHp = 6;
        spm::npcdrv::npcTribes[111].killXp = 300;
        spm::npcdrv::npcTribes[111].coinDropChance = 100;
        spm::npcdrv::npcTribes[111].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[111].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[111].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[111].attackStrength = 2;

        // Flip Buzzy Beetle/Flip Spike Top patch
        spm::npcdrv::npcTribes[470].catchCardItemId = 298;
        spm::npcdrv::npcTribes[470].catchCardDefense = 2;
        spm::npcdrv::npcTribes[470].maxHp = 1;
        spm::npcdrv::npcTribes[470].killXp = 200;
        spm::npcdrv::npcTribes[470].coinDropChance = 100;
        spm::npcdrv::npcTribes[470].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[470].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[470].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[470].attackStrength = 1;
        spm::npcdrv::npcTribes[470].bounceEjection = 2;
        spm::npcdrv::npcTribes[470].jumpEjection = 0;

        spm::npcdrv::npcTribes[471].catchCardItemId = 299;
        spm::npcdrv::npcTribes[471].catchCardDefense = 10;
        spm::npcdrv::npcTribes[471].maxHp = 1;
        spm::npcdrv::npcTribes[471].killXp = 300;
        spm::npcdrv::npcTribes[471].coinDropChance = 100;
        spm::npcdrv::npcTribes[471].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[471].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[471].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[471].attackStrength = 2;
        spm::npcdrv::npcTribes[471].bounceEjection = 2;
        spm::npcdrv::npcTribes[471].jumpEjection = 0;

        // Green Fuzzy nerf
        spm::npcdrv::npcTribes[98].catchCardDefense = 16;
        spm::npcdrv::npcTribes[98].maxHp = 10;
        spm::npcdrv::npcTribes[98].killXp = 600;
        spm::npcdrv::npcTribes[98].coinDropChance = 100;
        spm::npcdrv::npcTribes[98].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[98].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[98].coinDropExtraMax = 2;
        spm::npcdrv::npcTribes[98].attackStrength = 2;

        // Tileoid PU stats
        spm::npcdrv::npcTribes[167].catchCardDefense = 12;
        spm::npcdrv::npcTribes[167].maxHp = 1;
        spm::npcdrv::npcTribes[167].killXp = 600;
        spm::npcdrv::npcTribes[167].coinDropChance = 100;
        spm::npcdrv::npcTribes[167].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[167].coinDropExtraChance = 40;
        spm::npcdrv::npcTribes[167].coinDropExtraMax = 6;
        spm::npcdrv::npcTribes[167].attackStrength = 5;
        spm::npcdrv::npcTribes[167].bounceEjection = 10;
        spm::npcdrv::npcTribes[167].jumpEjection = 10;
        // We're actually gonna nerf the other Tileoids' ejection strengths, too!
        spm::npcdrv::npcTribes[163].bounceEjection = 0.5;
        spm::npcdrv::npcTribes[164].bounceEjection = 0.5;
        spm::npcdrv::npcTribes[165].bounceEjection = 0.5;
        spm::npcdrv::npcTribes[166].bounceEjection = 0.5;
        spm::npcdrv::npcTribes[163].jumpEjection = 0.5;
        spm::npcdrv::npcTribes[164].jumpEjection = 0.5;
        spm::npcdrv::npcTribes[165].jumpEjection = 0.5;
        spm::npcdrv::npcTribes[166].jumpEjection = 0.5;

        // White Clubba nerf
        spm::npcdrv::npcTribes[95].maxHp = 30;
        spm::npcdrv::npcTribes[95].attackStrength = 4;

        // Frackle point buff
        spm::npcdrv::npcTribes[450].killXp = 10;

        // (Spiky) Skellobyte nerfs
        spm::npcdrv::npcTribes[225].maxHp = 20;
        spm::npcdrv::npcTribes[228].maxHp = 20;

        // Create Dark Lakitu (Template 30, Tribe 534)
        spm::npcdrv::npcEnemyTemplates[30].tribeId = 534;
        spm::npcdrv::npcEnemyTemplates[30].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[30].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[30].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[30].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[30].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[30].flags = 0x210;
        spm::npcdrv::npcEnemyTemplates[30].onSpawnScript = spm::npcdrv::npcEnemyTemplates[61].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[30].initScript = 0;
        spm::npcdrv::npcEnemyTemplates[30].moveScript = spm::npcdrv::npcEnemyTemplates[61].moveScript;
        spm::npcdrv::npcEnemyTemplates[30].onHitScript = spm::npcdrv::npcEnemyTemplates[61].onHitScript;
        spm::npcdrv::npcEnemyTemplates[30].pickupScript = spm::npcdrv::npcEnemyTemplates[61].pickupScript;
        spm::npcdrv::npcEnemyTemplates[30].throwScript = spm::npcdrv::npcEnemyTemplates[61].throwScript;
        spm::npcdrv::npcEnemyTemplates[30].deathScript = spm::npcdrv::npcEnemyTemplates[61].deathScript;
        spm::npcdrv::npcEnemyTemplates[30].atkScript = spm::npcdrv::npcEnemyTemplates[61].atkScript;
        spm::npcdrv::npcEnemyTemplates[30].miscScript = spm::npcdrv::npcEnemyTemplates[61].miscScript;
        spm::npcdrv::npcEnemyTemplates[30].kouraKickScript = spm::npcdrv::npcEnemyTemplates[61].kouraKickScript;
        spm::npcdrv::npcEnemyTemplates[30].unkDefinitionTable = spm::npcdrv::npcEnemyTemplates[61].unkDefinitionTable;
        spm::npcdrv::npcTribes[534].catchCardItemId = 307;
        spm::npcdrv::npcTribes[534].catchCardDefense = 20;
        spm::npcdrv::npcTribes[534].maxHp = 13;
        spm::npcdrv::npcTribes[534].killXp = 900;
        spm::npcdrv::npcTribes[534].coinDropChance = 100;
        spm::npcdrv::npcTribes[534].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[534].coinDropExtraChance = 68;
        spm::npcdrv::npcTribes[534].coinDropExtraMax = 6;
        spm::npcdrv::npcTribes[534].attackStrength = 3;
        spm::npcdrv::npcTribes[534].animPoseName = "e_jugemu_d";
        spm::npcdrv::npcTribes[534].animDefs = spm::npcdrv::npcTribes[34].animDefs;
        spm::npcdrv::npcTribes[534].hitXp = 0;

        // Create KP Koopa (Template 7, Tribe 533)
        spm::npcdrv::npcEnemyTemplates[7].tribeId = 533;
        spm::npcdrv::npcEnemyTemplates[7].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[7].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[7].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[7].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[7].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[7].flags = 0x18;
        spm::npcdrv::npcEnemyTemplates[7].onSpawnScript = spm::npcdrv::npcEnemyTemplates[9].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[7].initScript = spm::npcdrv::npcEnemyTemplates[9].initScript;
        spm::npcdrv::npcEnemyTemplates[7].moveScript = spm::npcdrv::npcEnemyTemplates[9].moveScript;
        spm::npcdrv::npcEnemyTemplates[7].onHitScript = spm::npcdrv::npcEnemyTemplates[9].onHitScript;
        spm::npcdrv::npcEnemyTemplates[7].pickupScript = spm::npcdrv::npcEnemyTemplates[9].pickupScript;
        spm::npcdrv::npcEnemyTemplates[7].throwScript = spm::npcdrv::npcEnemyTemplates[9].throwScript;
        spm::npcdrv::npcEnemyTemplates[7].deathScript = spm::npcdrv::npcEnemyTemplates[9].deathScript;
        spm::npcdrv::npcEnemyTemplates[7].atkScript = 0;
        spm::npcdrv::npcEnemyTemplates[7].miscScript = 0;
        spm::npcdrv::npcEnemyTemplates[7].kouraKickScript = spm::npcdrv::npcEnemyTemplates[9].kouraKickScript;
        spm::npcdrv::npcEnemyTemplates[7].unkDefinitionTable = spm::npcdrv::npcEnemyTemplates[9].unkDefinitionTable;
        spm::npcdrv::npcTribes[533].catchCardItemId = 291;
        spm::npcdrv::npcTribes[533].catchCardDefense = 8;
        spm::npcdrv::npcTribes[533].maxHp = 5;
        spm::npcdrv::npcTribes[533].killXp = 200;
        spm::npcdrv::npcTribes[533].coinDropChance = 100;
        spm::npcdrv::npcTribes[533].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[533].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[533].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[533].attackStrength = 2;
        spm::npcdrv::npcTribes[533].partsCount = 2;
        spm::npcdrv::npcTribes[533].partsList[0] = spm::npcdrv::npcTribes[14].partsList[0];
        spm::npcdrv::npcTribes[533].partsList[1] = spm::npcdrv::npcTribes[14].partsList[1];
        spm::npcdrv::npcTribes[533].animPoseName = "e_touginoko";
        spm::npcdrv::npcTribes[533].animDefs = spm::npcdrv::npcTribes[14].animDefs;
        spm::npcdrv::npcTribes[533].hitXp = 0;

        // Create Red Shy Guy (Template 32, Tribe 532)
        spm::npcdrv::npcEnemyTemplates[32].tribeId = 532;
        spm::npcdrv::npcEnemyTemplates[32].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[32].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[32].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[32].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[32].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[32].flags = 0x18;
        spm::npcdrv::npcEnemyTemplates[32].onSpawnScript = spm::npcdrv::npcEnemyTemplates[2].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[32].initScript = spm::npcdrv::npcEnemyTemplates[2].initScript;
        spm::npcdrv::npcEnemyTemplates[32].moveScript = spm::npcdrv::npcEnemyTemplates[2].moveScript;
        spm::npcdrv::npcEnemyTemplates[32].onHitScript = spm::npcdrv::npcEnemyTemplates[2].onHitScript;
        spm::npcdrv::npcEnemyTemplates[32].pickupScript = spm::npcdrv::npcEnemyTemplates[2].pickupScript;
        spm::npcdrv::npcEnemyTemplates[32].throwScript = spm::npcdrv::npcEnemyTemplates[2].throwScript;
        spm::npcdrv::npcEnemyTemplates[32].deathScript = spm::npcdrv::npcEnemyTemplates[2].deathScript;
        spm::npcdrv::npcEnemyTemplates[32].atkScript = 0;
        spm::npcdrv::npcEnemyTemplates[32].miscScript = 0;
        spm::npcdrv::npcEnemyTemplates[32].kouraKickScript = 0;
        spm::npcdrv::npcEnemyTemplates[32].unkDefinitionTable = spm::npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        spm::npcdrv::npcTribes[532].catchCardItemId = 299;
        spm::npcdrv::npcTribes[532].catchCardDefense = 3;
        spm::npcdrv::npcTribes[532].maxHp = 5;
        spm::npcdrv::npcTribes[532].killXp = 200;
        spm::npcdrv::npcTribes[532].coinDropChance = 100;
        spm::npcdrv::npcTribes[532].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[532].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[532].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[532].attackStrength = 2;
        spm::npcdrv::npcTribes[532].animPoseName = "e_heiho";
        spm::npcdrv::npcTribes[532].animDefs = heihoAnims;
        spm::npcdrv::npcTribes[532].hitXp = 0;

        // Create Blue Shy Guy (Template 33, Tribe 531)
        spm::npcdrv::npcEnemyTemplates[33].tribeId = 531;
        spm::npcdrv::npcEnemyTemplates[33].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[33].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[33].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[33].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[33].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[33].flags = 0x18;
        spm::npcdrv::npcEnemyTemplates[33].onSpawnScript = spm::npcdrv::npcEnemyTemplates[2].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[33].initScript = spm::npcdrv::npcEnemyTemplates[2].initScript;
        spm::npcdrv::npcEnemyTemplates[33].moveScript = spm::npcdrv::npcEnemyTemplates[2].moveScript;
        spm::npcdrv::npcEnemyTemplates[33].onHitScript = spm::npcdrv::npcEnemyTemplates[2].onHitScript;
        spm::npcdrv::npcEnemyTemplates[33].pickupScript = spm::npcdrv::npcEnemyTemplates[2].pickupScript;
        spm::npcdrv::npcEnemyTemplates[33].throwScript = spm::npcdrv::npcEnemyTemplates[2].throwScript;
        spm::npcdrv::npcEnemyTemplates[33].deathScript = spm::npcdrv::npcEnemyTemplates[2].deathScript;
        spm::npcdrv::npcEnemyTemplates[33].atkScript = 0;
        spm::npcdrv::npcEnemyTemplates[33].miscScript = 0;
        spm::npcdrv::npcEnemyTemplates[33].kouraKickScript = 0;
        spm::npcdrv::npcEnemyTemplates[33].unkDefinitionTable = spm::npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        spm::npcdrv::npcTribes[531].catchCardItemId = 299;
        spm::npcdrv::npcTribes[531].catchCardDefense = 5;
        spm::npcdrv::npcTribes[531].maxHp = 5;
        spm::npcdrv::npcTribes[531].killXp = 300;
        spm::npcdrv::npcTribes[531].coinDropChance = 100;
        spm::npcdrv::npcTribes[531].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[531].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[531].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[531].attackStrength = 2;
        spm::npcdrv::npcTribes[531].animPoseName = "e_beiho";
        spm::npcdrv::npcTribes[531].animDefs = heihoAnims;
        spm::npcdrv::npcTribes[531].hitXp = 0;
        spm::npcdrv::npcTribes[531].bounceEjection = 1;
        spm::npcdrv::npcTribes[531].jumpEjection = 0;
        spm::npcdrv::npcTribes[531].powBlockDeathSfx = "SFX_BS_DDN_CRASH_LANDING1";

        // Create Green Shy Guy (Template 34, Tribe 530)
        spm::npcdrv::npcEnemyTemplates[34].tribeId = 530;
        spm::npcdrv::npcEnemyTemplates[34].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[34].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[34].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[34].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[34].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[34].flags = 0x18;
        spm::npcdrv::npcEnemyTemplates[34].onSpawnScript = spm::npcdrv::npcEnemyTemplates[2].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[34].initScript = spm::npcdrv::npcEnemyTemplates[2].initScript;
        spm::npcdrv::npcEnemyTemplates[34].moveScript = spm::npcdrv::npcEnemyTemplates[2].moveScript;
        spm::npcdrv::npcEnemyTemplates[34].onHitScript = spm::npcdrv::npcEnemyTemplates[2].onHitScript;
        spm::npcdrv::npcEnemyTemplates[34].pickupScript = spm::npcdrv::npcEnemyTemplates[2].pickupScript;
        spm::npcdrv::npcEnemyTemplates[34].throwScript = spm::npcdrv::npcEnemyTemplates[2].throwScript;
        spm::npcdrv::npcEnemyTemplates[34].deathScript = spm::npcdrv::npcEnemyTemplates[2].deathScript;
        spm::npcdrv::npcEnemyTemplates[34].atkScript = 0;
        spm::npcdrv::npcEnemyTemplates[34].miscScript = 0;
        spm::npcdrv::npcEnemyTemplates[34].kouraKickScript = 0;
        spm::npcdrv::npcEnemyTemplates[34].unkDefinitionTable = spm::npcdrv::npcEnemyTemplates[2].unkDefinitionTable;
        spm::npcdrv::npcTribes[530].catchCardItemId = 299;
        spm::npcdrv::npcTribes[530].catchCardDefense = 5;
        spm::npcdrv::npcTribes[530].maxHp = 5;
        spm::npcdrv::npcTribes[530].killXp = 300;
        spm::npcdrv::npcTribes[530].coinDropChance = 100;
        spm::npcdrv::npcTribes[530].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[530].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[530].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[530].attackStrength = 2;
        spm::npcdrv::npcTribes[530].animPoseName = "e_geiho";
        spm::npcdrv::npcTribes[530].animDefs = heihoAnims;
        spm::npcdrv::npcTribes[530].hitXp = 0;
        spm::npcdrv::npcTribes[530].bounceEjection = 1;
        spm::npcdrv::npcTribes[530].jumpEjection = 0;

        // Create Yellow Shy Guy (Template 35, Tribe 529)
        spm::npcdrv::npcEnemyTemplates[35].tribeId = 529;
        spm::npcdrv::npcEnemyTemplates[35].unknown_0x0 = 1;
        spm::npcdrv::npcEnemyTemplates[35].unknown_0x1 = 1;
        spm::npcdrv::npcEnemyTemplates[35].unknown_0x2 = 0;
        spm::npcdrv::npcEnemyTemplates[35].unknown_0x3 = 0;
        spm::npcdrv::npcEnemyTemplates[35].unknown_0x8 = 0;
        spm::npcdrv::npcEnemyTemplates[35].flags = 0x18;
        spm::npcdrv::npcEnemyTemplates[35].onSpawnScript = spm::npcdrv::npcEnemyTemplates[2].onSpawnScript;
        spm::npcdrv::npcEnemyTemplates[35].initScript = spm::npcdrv::npcEnemyTemplates[2].initScript;
        spm::npcdrv::npcEnemyTemplates[35].moveScript = spm::npcdrv::npcEnemyTemplates[2].moveScript;
        spm::npcdrv::npcEnemyTemplates[35].onHitScript = spm::npcdrv::npcEnemyTemplates[2].onHitScript;
        spm::npcdrv::npcEnemyTemplates[35].pickupScript = spm::npcdrv::npcEnemyTemplates[2].pickupScript;
        spm::npcdrv::npcEnemyTemplates[35].throwScript = spm::npcdrv::npcEnemyTemplates[2].throwScript;
        spm::npcdrv::npcEnemyTemplates[35].deathScript = spm::npcdrv::npcEnemyTemplates[2].deathScript;
        spm::npcdrv::npcEnemyTemplates[35].atkScript = 0;
        spm::npcdrv::npcEnemyTemplates[35].miscScript = 0;
        spm::npcdrv::npcEnemyTemplates[35].kouraKickScript = 0;
        spm::npcdrv::npcEnemyTemplates[35].unkDefinitionTable = 0;
        spm::npcdrv::npcTribes[529].catchCardItemId = 299;
        spm::npcdrv::npcTribes[529].catchCardDefense = 8;
        spm::npcdrv::npcTribes[529].maxHp = 12;
        spm::npcdrv::npcTribes[529].killXp = 300;
        spm::npcdrv::npcTribes[529].coinDropChance = 100;
        spm::npcdrv::npcTribes[529].coinDropBaseCount = 0;
        spm::npcdrv::npcTribes[529].coinDropExtraChance = 45;
        spm::npcdrv::npcTribes[529].coinDropExtraMax = 3;
        spm::npcdrv::npcTribes[529].attackStrength = 2;
        spm::npcdrv::npcTribes[529].animPoseName = "e_yeiho";
        spm::npcdrv::npcTribes[529].animDefs = heihoAnims;
        spm::npcdrv::npcTribes[529].hitXp = 0;
        spm::npcdrv::npcTribes[529].bounceEjection = 1;
        spm::npcdrv::npcTribes[529].jumpEjection = 0;
        spm::npcdrv::NPCDefense testDefs[5] = {def1_1, def7_1, def0_1, def48, defterm};
        spm::npcdrv::npcTribes[529].partsList[0].defenses = testDefs;
        wii::os::OSReport("def0_1: %d.\n", def0_1.defense);
        wii::os::OSReport("defterm: %d.\n", defterm.defense);

        // Let's replace the pit key texture while we're at it lol
        // Ty kiki!!! <3
        spm::item_data::itemDataTable[48].iconId = 60;
    }

    s32 patch_tileoid_pu(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::npcdrv::NPCEntry *npc = evtEntry->ownerNPC;
        if (npc->tribeId == 167)
        {
            spm::npc_tile::TileoidWork *tileWrk = (spm::npc_tile::TileoidWork *)npc->unitWork[0];
            tileWrk->rotationSpeedScale = 3.5;
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(patch_tileoid_pu, 0)

    s32 setHitFlags(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        s32 flagVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        spm::npcdrv::NPCEntry *ownerNpc = (spm::npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[0].hitFlags = flagVal;
        ownerNpc->parts[1].hitFlags = flagVal;
        ownerNpc->parts[2].hitFlags = flagVal;
        ownerNpc->parts[3].hitFlags = flagVal;
        ownerNpc->parts[4].hitFlags = flagVal;
        ownerNpc->parts[5].hitFlags = flagVal;
        ownerNpc->parts[6].hitFlags = flagVal;
        ownerNpc->parts[7].hitFlags = flagVal;
        ownerNpc->parts[8].hitFlags = flagVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setHitFlags, 1)

    s32 setFlag2c(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        s32 partId = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 flagVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        spm::npcdrv::NPCEntry *ownerNpc = (spm::npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[partId].flag2c = flagVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setFlag2c, 2)

    s32 setHitboxSize(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        s32 partId = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 xVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 yVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        f32 zVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[3]);
        spm::npcdrv::NPCEntry *ownerNpc = (spm::npcdrv::NPCEntry *)evtEntry->ownerNPC;
        ownerNpc->parts[partId].hitboxSize.x = xVal;
        ownerNpc->parts[partId].hitboxSize.y = yVal;
        ownerNpc->parts[partId].hitboxSize.z = zVal;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(setHitboxSize, 4)

    s32 declare_shadoo_stats(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
        marioMaxHp = pouch->maxHp;

        // Shadoo HP = Mario max HP
        spm::npcdrv::npcTribes[332].maxHp = marioMaxHp; // Dark Peach
        spm::npcdrv::npcTribes[333].maxHp = marioMaxHp; // Dark Bowser
        spm::npcdrv::npcTribes[330].maxHp = marioMaxHp; // Dark Mario
        spm::npcdrv::npcTribes[331].maxHp = marioMaxHp; // Dark Luigi
        return 2;
    }
    EVT_DECLARE_USER_FUNC(declare_shadoo_stats, 0)

    s32 get_mover_rng(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], moverRNG);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(get_mover_rng, 1)

    s32 osReportFloat(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // arg1 is like PTR("Variable name:"), arg2 is actual value to pass through
        // You can also just use one variable if you only want to pass a ptr string or float on its own
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        char *introStr = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 printFloat = spm::evtmgr_cmd::evtGetFloat(evtEntry, args[1]);
        if (introStr == nullptr || introStr == 0)
        {
            wii::os::OSReport("%f\n", printFloat);
        }
        else if (printFloat == 0)
        {
            wii::os::OSReport("%s\n", introStr);
        }
        else
        {
            wii::os::OSReport("%s %f\n", introStr, printFloat);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(osReportFloat, 2)

    s32 rand100(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        rand100Num = spm::system::rand() % 100;
        wii::os::OSReport("rand100Num: %d.\n", rand100Num);
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], rand100Num);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(rand100, 1)

    s32 evt_mario_motion_chg_mot(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        int motId = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        spm::mario_motion::marioChgMot(motId);
        spm::mario::MarioWork *mario = spm::mario::marioGetPtr();
        mario->flags = mario->flags &= ~(0x10000000);
        mario->invincibilityTimer = 60;
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_mario_motion_chg_mot, 1)

    s32 evt_mario_chk_key(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 marioKey = spm::mario::marioChkKey(); // "is mario allowed to move?" returns true if so
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], marioKey);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_mario_chk_key, 1)

    s32 evtCompareStrings(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        char *mapName = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        char *comparison = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        int compstrReturn = 1;
        char *result = msl::string::strstr(mapName, comparison);
        if (result != 0)
        {
            compstrReturn = 1;
        }
        else
        {
            compstrReturn = 0;
        }
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], compstrReturn);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evtCompareStrings, 3)

    s32 migraineShortenCurseTimers(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::mario::MarioWork *mario = spm::mario::marioGetPtr();
        for (spm::mario::MarioStatus *yes = mario->firstStatus; yes != 0; yes = yes->next)
        {
            if (yes->type == STATUS_FLIPPED_CONTROLS || yes->type == STATUS_NO_SKILLS)
            {
                // WAIT_MSEC(500) precedes this user func, so it starts with .5s
                // Thus, status conditions may last anywhere from 1-10s
                s32 yesRand = spm::system::rand() % 540 + 30;
                yes->timer = (f32)yesRand;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(migraineShortenCurseTimers, 0)

    s32 evt_eff_spm_spindash(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        f32 x = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        f32 y = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        f32 z = spm::evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        f32 scale = spm::evtmgr_cmd::evtGetValue(evtEntry, args[3]);
        f32 uwVal = spm::evtmgr_cmd::evtGetValue(evtEntry, args[4]);
        spm::eff_spm_spindash::effSpmSpindashEntry(x, y, z, scale, uwVal);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(evt_eff_spm_spindash, 5)

    s32 spectreFloor(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        double hp = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        int floor = spm::swdrv::swByteGet(1);
        if (floor <= 24) // Lv1
        {
            hp = (hp / 10);
        }
        else if (floor <= 48) // Lv2
        {
            hp = (hp / 6.667);
        }
        else if (floor <= 174) // Lv3
        {
            hp = (hp / 5);
        }
        else // Lv4
        {
            hp = (hp / 4);
        }
        s32 hpReturn = msl::math::floor(hp);
        if (hpReturn == 0)
        {
            hpReturn = 1;
        }
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], hpReturn);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(spectreFloor, 2)

    s32 paramitaFloor(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
        if (pouch->hp >= 1)
        {
            f32 hp = (f32)(pouch->hp * 0.3);
            s32 hpReturn = msl::math::floor(hp);
            spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], hpReturn);
        }
        else
        {
            spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], 0);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(paramitaFloor, 1)

    s32 generate_mover_prices(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Change Mover prices based on Pit Level
        int floor = spm::swdrv::swByteGet(1);
        if (floor <= 24) // Lv1
        {
            moverDown2Price = 20;
            moverDown5Price = 60;
        }
        else if (floor <= 48) // Lv2
        {
            moverDown2Price = 25;
            moverDown5Price = 75;
        }
        else if (floor <= 174) // Lv3
        {
            moverDown2Price = 30;
            moverDown5Price = 90;
        }
        else // Lv4
        {
            moverDown2Price = 40;
            moverDown5Price = 120;
        }
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], moverDown2Price);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], moverDown5Price);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(generate_mover_prices, 2)

    s32 mover_down_2(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        int floor = spm::swdrv::swByteGet(1);
        floor = floor + 1;
        spm::swdrv::swByteSet(1, floor);
        // Seeky, please forgive me for the copypasting I'm about to do.
        switch (floor)
        {
        // Flipside special
        case 9:
        case 19:
            destMap = "dan_21";
            break;
        case 29:
        case 39:
            destMap = "dan_22";
            break;
        case 49:
        case 59:
            destMap = "dan_23";
            break;
        case 69:
        case 79:
        case 89:
            destMap = "dan_24";
            break;
        case 99:
            destMap = "dan_30";
            break;

        // Flopside special
        case 109:
        case 119:
            destMap = "dan_61";
            break;
        case 129:
        case 139:
            destMap = "dan_62";
            break;
        case 149:
        case 159:
            destMap = "dan_63";
            break;
        case 169:
        case 179:
        case 189:
            destMap = "dan_64";
            break;
        case 199:
            destMap = "dan_70";
            break;

        // Enemy rooms
        default:
            // Flipside
            if ((floor >= 0) && (floor <= 24))
                destMap = "dan_01";
            else if ((floor >= 25) && (floor <= 49))
                destMap = "dan_02";
            else if ((floor >= 50) && (floor <= 74))
                destMap = "dan_03";
            else if ((floor >= 75) && (floor <= 99))
                destMap = "dan_04";

            // Flopside
            else if ((floor >= 100) && (floor <= 124))
                destMap = "dan_41";
            else if ((floor >= 125) && (floor <= 149))
                destMap = "dan_42";
            else if ((floor >= 150) && (floor <= 174))
                destMap = "dan_43";
            else if ((floor >= 175) && (floor <= 199))
                destMap = "dan_44";
        }
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], destMap);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(mover_down_2, 1)

    s32 mover_down_5(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        int floor = spm::swdrv::swByteGet(1);
        floor = floor + 4;
        spm::swdrv::swByteSet(1, floor);
        // Seeky, please forgive me for the copypasting I'm about to do.
        switch (floor)
        {
        // Flipside special
        case 9:
        case 19:
            destMap = "dan_21";
            break;
        case 29:
        case 39:
            destMap = "dan_22";
            break;
        case 49:
        case 59:
            destMap = "dan_23";
            break;
        case 69:
        case 79:
        case 89:
            destMap = "dan_24";
            break;
        case 99:
            destMap = "dan_30";
            break;

        // Flopside special
        case 109:
        case 119:
            destMap = "dan_61";
            break;
        case 129:
        case 139:
            destMap = "dan_62";
            break;
        case 149:
        case 159:
            destMap = "dan_63";
            break;
        case 169:
        case 179:
        case 189:
            destMap = "dan_64";
            break;
        case 199:
            destMap = "dan_70";
            break;

        // Enemy rooms
        default:
            // Flipside
            if ((floor >= 0) && (floor <= 24))
                destMap = "dan_01";
            else if ((floor >= 25) && (floor <= 49))
                destMap = "dan_02";
            else if ((floor >= 50) && (floor <= 74))
                destMap = "dan_03";
            else if ((floor >= 75) && (floor <= 99))
                destMap = "dan_04";

            // Flopside
            else if ((floor >= 100) && (floor <= 124))
                destMap = "dan_41";
            else if ((floor >= 125) && (floor <= 149))
                destMap = "dan_42";
            else if ((floor >= 150) && (floor <= 174))
                destMap = "dan_43";
            else if ((floor >= 175) && (floor <= 199))
                destMap = "dan_44";
        }
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], destMap);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(mover_down_5, 1)

    s32 start_from_21(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
        spm::mario_pouch::pouchSetMaxHp(20);
        spm::mario_pouch::pouchSetHp(20);
        spm::mario_pouch::pouchAddXp(60000);
        spm::mario_pouch::pouchSetLevel(5);
        spm::mario_pouch::pouchSetAttack(3);
        spm::mario_pouch::pouchSetCoin(40);
        spm::mario_pouch::pouchAddItem(50);
        spm::mario_pouch::pouchAddItem(217);
        spm::mario_pouch::pouchAddItem(221);
        spm::mario_pouch::pouchAddItem(222);
        spm::mario_pouch::pouchAddItem(80);
        spm::mario_pouch::pouchAddItem(80);
        spm::mario_pouch::pouchAddItem(70);
        spm::mario_pouch::pouchAddItem(118);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_21, 0)

    s32 start_from_61(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
        spm::mario_pouch::pouchSetMaxHp(30);
        spm::mario_pouch::pouchSetHp(30);
        spm::mario_pouch::pouchAddXp(240000);
        spm::mario_pouch::pouchSetLevel(9);
        spm::mario_pouch::pouchSetAttack(4);
        spm::mario_pouch::pouchSetCoin(120);
        spm::mario_pouch::pouchAddItem(50);
        spm::mario_pouch::pouchAddItem(217);
        spm::mario_pouch::pouchAddItem(218);
        spm::mario_pouch::pouchAddItem(221);
        spm::mario_pouch::pouchAddItem(222);
        spm::mario_pouch::pouchAddItem(223);
        spm::mario_pouch::pouchAddItem(224);
        spm::mario_pouch::pouchAddItem(225);
        spm::mario_pouch::pouchAddItem(226);
        spm::mario_pouch::pouchAddItem(227);
        spm::mario_pouch::pouchAddItem(228);
        spm::mario_pouch::pouchAddItem(80);
        spm::mario_pouch::pouchAddItem(118);
        spm::mario_pouch::pouchAddItem(102);
        spm::swdrv::swSet(515);
        spm::swdrv::swSet(511);
        spm::swdrv::swSet(514);
        spm::swdrv::swSet(513);
        spm::swdrv::swSet(510);
        spm::swdrv::swSet(508);
        spm::swdrv::swSet(534);
        spm::swdrv::swSet(1037);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_61, 0)

    s32 start_from_eg(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::mario_pouch::MarioPouchWork *pouch = spm::mario_pouch::pouchGetPtr();
        spm::mario_pouch::pouchSetMaxHp(40);
        spm::mario_pouch::pouchSetHp(40);
        spm::mario_pouch::pouchAddXp(360000);
        spm::mario_pouch::pouchSetLevel(12);
        spm::mario_pouch::pouchSetAttack(6);
        spm::mario_pouch::pouchSetCoin(200);
        spm::mario_pouch::pouchAddItem(50);
        spm::mario_pouch::pouchAddItem(217);
        spm::mario_pouch::pouchAddItem(218);
        spm::mario_pouch::pouchAddItem(219);
        spm::mario_pouch::pouchAddItem(221);
        spm::mario_pouch::pouchAddItem(222);
        spm::mario_pouch::pouchAddItem(223);
        spm::mario_pouch::pouchAddItem(224);
        spm::mario_pouch::pouchAddItem(225);
        spm::mario_pouch::pouchAddItem(226);
        spm::mario_pouch::pouchAddItem(227);
        spm::mario_pouch::pouchAddItem(228);
        spm::mario_pouch::pouchAddItem(229);
        spm::mario_pouch::pouchAddItem(230);
        spm::mario_pouch::pouchAddItem(81);
        spm::mario_pouch::pouchAddItem(118);
        spm::swdrv::swSet(515);
        spm::swdrv::swSet(511);
        spm::swdrv::swSet(514);
        spm::swdrv::swSet(513);
        spm::swdrv::swSet(510);
        spm::swdrv::swSet(508);
        spm::swdrv::swSet(418);
        spm::swdrv::swSet(533);
        spm::swdrv::swSet(534);
        spm::swdrv::swSet(535);
        spm::swdrv::swSet(536);
        spm::swdrv::swSet(1037);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(start_from_eg, 0)

    s32 create_holographic_enemy(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], 0);
        s32 sup = spm::system::rand() % 100;
        if (sup > 0) // NORMALLY 96, 0 IS DEBUG!!!
        {
            spm::npcdrv::NPCEntry *npc = spm::npcdrv::npcNameToPtr_NoAssert(spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]));
            if ((s32)npc != 0 && npc->templateUnkScript9 == 0 && npc->tribeId != 200 && npc->tribeId != 201 && npc->tribeId != 32 && npc->tribeId != 142 && npc->tribeId != 144 && npc->tribeId != 146 && npc->tribeId != 504 && npc->tribeId != 156 && npc->tribeId != 157 && npc->tribeId != 188 && npc->tribeId != 189)
            {
                npc->maxHp = (npc->maxHp * 2);
                npc->hp = (npc->hp * 2);
                npc->unkShellSfx = "holo"; // Used as an identifier for holographic enemies in the npcDamageMario and npcHandleHitXp patches
                s32 tribe = npc->tribeId;
                spm::npcdrv::NPCDropItem *dropItems = spm::npcdrv::npcTribes[tribe].dropItemList;
                if (npc->dropItemId == 0 && dropItems[0].itemId != 0) // If it doesn't already have an item, continue
                {
                    s32 chance = spm::npcdrv::npcTribes[tribe].dropItemChance * 8;
                    sup = spm::system::rand() % 100;
                    if (chance >= sup)
                    {
                        sup = 1;
                        for (i = 0; sup != 0; ++i)
                        {
                            sup = dropItems[i].itemId;
                        }
                        do
                        {
                            sup = spm::system::rand() % i;
                            npc->dropItemId = dropItems[sup].itemId;
                            if (npc->dropItemId == 0x57)
                            {
                                s32 sup2 = spm::system::rand() % 100;
                                if (sup2 > 25)
                                {
                                    npc->dropItemId = 0x53; // 75% chance to replace Catch Card drops with Dried Shrooms
                                }
                            }
                        } while (dropItems[sup].itemId == 0);
                    }
                }
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], 1);
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(create_holographic_enemy, 2)

    s32 dan_boodin_setup_cards(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        s32 cardNum = spm::system::rand() % 9 + 5; // 5-13
        boodinBalls.cardCount = cardNum;
        s32 poolCard = 0;
        for (i = 1; i <= cardNum; ++i)
        {
            poolCard = spm::system::rand() % 77;
            boodinBalls.cardArray[i] = boodinShopItemPool[poolCard];
            if (i == cardNum)
            {
                boodinBalls.cardArray[i] = -1;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(dan_boodin_setup_cards, 0)

    s32 dan_boodin_card_select(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        // Check if there are any cards remaining
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        // Format array to eliminate zeroes before opening the shop
        s32 finalCardArray[15] = {0x58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        s32 n = 1;
        for (i = 1; i <= 14; ++i)
        {
            if (boodinBalls.cardArray[i] != 0)
            {
                finalCardArray[n] = boodinBalls.cardArray[i];
                n = n + 1;
            }
        }
        // Open shop
        if (firstRun)
        {
            spm::spmario_snd::spsndSFXOn("SFX_SYS_MENU_OPEN1");
            spm::winmgr::WinmgrSelect *shop = spm::winmgr::winMgrSelectEntry(&finalCardArray, 0, 9);
            evtEntry->tempU[0] = (u32)shop;
        }
        s32 selectedCard = spm::winmgr::winMgrSelect((spm::winmgr::WinmgrSelect *)evtEntry->tempU[0]);
        s32 ret = 0;
        if (selectedCard == 0)
        {
            ret = 0;
        }
        else
        {
            if (selectedCard != 0x58) // Never overwrite Catch Card SP
            {
                spm::winmgr::WinmgrSelect *shop = (spm::winmgr::WinmgrSelect *)evtEntry->tempU[0];
                boodinBalls.cardArray[shop->selectedItemIdx] = 0;
            }
            spm::winmgr::winMgrSelectDelete((spm::winmgr::WinmgrSelect *)evtEntry->tempU[0]);
            if (selectedCard == -1)
            {
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], -1);
            }
            else
            {
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], selectedCard);
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], spm::msgdrv::msgSearch(spm::item_data::itemDataTable[selectedCard].nameMsg));
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], (u32)spm::item_data::itemDataTable[selectedCard].buyPrice);
                u32 halvedCardPrice = 0;
                if (((u32)spm::item_data::itemDataTable[selectedCard].buyPrice) % 2 == 1) // Variable is odd, so round up.
                {
                    halvedCardPrice = ((f32)spm::item_data::itemDataTable[selectedCard].buyPrice / 2) + 0.5;
                }
                else
                {
                    halvedCardPrice = (u32)spm::item_data::itemDataTable[selectedCard].buyPrice / 2;
                }
                spm::evtmgr_cmd::evtSetValue(evtEntry, args[3], halvedCardPrice);
            }
            ret = 2;
        }
        return ret;
    }
    EVT_DECLARE_USER_FUNC(dan_boodin_card_select, 4) // Return item ID, item name, and its original/new sell value

    s32 render_judgement(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        // Blessing
        s32 blessing = spm::system::rand() % 3 + 1;
        blessStorage = blessing;
        char *blessName;
        switch (blessing)
        {
        case BlessId::MERLUNA_SPECTRE:
            blessName = "Spectre";
            break;
        case BlessId::MERLUNA_HOURAI:
            blessName = "Hourai Doll";
            break;
        case BlessId::MERLUNA_PARAMITA:
            blessName = "Paramita";
            break;
        }
        // Curse
        s32 curse = spm::system::rand() % 3 + 1;
        curseStorage = curse;
        char *curseName;
        switch (curse)
        {
        case CurseId::MERLUNA_SHION:
            curseName = "Tatarian Aster";
            break;

        case CurseId::MERLUNA_HEX:
            curseName = "Vulnerability Hex";
            break;

        case CurseId::MERLUNA_MIGRAINE:
            curseName = "Migraine";
            break;
        }

        // Determine which one actually happens
        s32 judgement = spm::system::rand() % 100;
        if (judgement < 70)
        {
            judgement = 0; // Is Blessing
        }
        else
        {
            judgement = 1; // Is Curse
        }
        wii::os::OSReport("merluna bless name: %s.\n", blessName);
        wii::os::OSReport("merluna curse name: %s.\n", curseName);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], blessing);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], blessName);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], curse);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[3], curseName);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[4], judgement);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(render_judgement, 5)

    s32 rerender_judgement(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        // Blessing
        char *blessName;
        switch (blessStorage)
        {
        case BlessId::MERLUNA_SPECTRE:
            blessName = "Spectre";
            break;
        case BlessId::MERLUNA_HOURAI:
            blessName = "Hourai Doll";
            break;
        case BlessId::MERLUNA_PARAMITA:
            blessName = "Paramita";
            break;
        }
        // Curse
        char *curseName;
        switch (curseStorage)
        {
        case CurseId::MERLUNA_SHION:
            curseName = "Tatarian Aster";
            break;

        case CurseId::MERLUNA_HEX:
            curseName = "Vulnerability Hex";
            break;

        case CurseId::MERLUNA_MIGRAINE:
            curseName = "Migraine";
            break;
        }

        // Determine which one actually happens
        s32 judgement = spm::system::rand() % 100;
        if (judgement < 70)
        {
            judgement = 0; // Is Blessing
        }
        else
        {
            judgement = 1; // Is Curse
        }
        wii::os::OSReport("RERENDER merluna bless name: %s.\n", blessName);
        wii::os::OSReport("RERENDER merluna curse name: %s.\n", curseName);
        wii::os::OSReport("RERENDER merluna judgement: %d.\n", judgement);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], blessStorage);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[1], blessName);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[2], curseStorage);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[3], curseName);
        spm::evtmgr_cmd::evtSetValue(evtEntry, args[4], judgement);
        return 2;
    }
    EVT_DECLARE_USER_FUNC(rerender_judgement, 5)

    s32 render_judgement_2(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        s32 bless = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 curse = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 blessingOrCurse = spm::evtmgr_cmd::evtGetValue(evtEntry, args[2]);

        // Set target GSW to the judgement ID
        if (blessingOrCurse == 0)
        {
            spm::swdrv::swByteSet(1600, bless);
            spm::swdrv::swByteSet(1602, 20);
        }
        else
        {
            spm::swdrv::swByteSet(1601, curse);
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(render_judgement_2, 3)

    s32 explain_judgement(spm::evtmgr::EvtEntry *evtEntry, bool firstRun)
    {
        spm::evtmgr::EvtVar *args = (spm::evtmgr::EvtVar *)evtEntry->pCurData;
        s32 bless = spm::evtmgr_cmd::evtGetValue(evtEntry, args[0]);
        s32 curse = spm::evtmgr_cmd::evtGetValue(evtEntry, args[1]);
        s32 judgementType = spm::evtmgr_cmd::evtGetValue(evtEntry, args[2]);
        s32 speaker = spm::evtmgr_cmd::evtGetValue(evtEntry, args[3]);

        // Blessing
        if (judgementType == 0)
        {
            switch (bless)
            {
            case BlessId::MERLUNA_SPECTRE:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaSpectre, 0, speaker);
                break;
            case BlessId::MERLUNA_HOURAI:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaHourai, 0, speaker);
                break;
            case BlessId::MERLUNA_PARAMITA:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaParamita, 0, speaker);
                break;
            }
        }
        else // Curse
        {
            switch (curse)
            {
            case CurseId::MERLUNA_SHION:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaAster, 0, speaker);
                break;
            case CurseId::MERLUNA_HEX:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaHex, 0, speaker);
                break;
            case CurseId::MERLUNA_MIGRAINE:
                return spm::evt_msg::_evt_msg_print(evtEntry, firstRun, 1, merlunaMigraine, 0, speaker);
                break;
            }
        }
        return 2;
    }
    EVT_DECLARE_USER_FUNC(explain_judgement, 4)

    static spm::evt_door::DokanDesc new_dan_70_dokan_desc = {
        0, 0, 0, "dokan", "dan_70", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"};

    spm::npcdrv::NPCTribeAnimDef merlunaAnims[] = {
        {0, "n_stg2_mistS_1"},      // Idle
        {1, "n_stg2_mistW_1"},      // Walking
        {2, "n_stg2_mistR_1"},      // Running
        {3, "n_stg2_mistT_1"},      // Talking (Idle)
        {25, "n_stg2_mistA_1"},     // ):<
        {26, "n_stg2_mistA_2"},     // D:<
        {27, "n_stg2_mistO_1"},     // O_O
        {28, "n_stg2_mistTW_1_a"},  // KEK
        {29, "n_stg2_mistTW_1_b"},  // LMAOOOOOO
        {30, "n_stg2_mistTW_1_c"},  // lol
        {31, "n_stg2_mistTOB_1_a"}, // >_<
        {32, "n_stg2_mistTOB_1_b"}, // @_@
        {33, "n_stg2_mistTOB_1_c"}, // Back to normal
        {34, "n_stg2_mistHi_1"},    // :3
        {35, "n_stg2_mistS_2"},     // Idle (demure)
        {36, "n_stg2_mistJ_1_a"},   // Jump
        {37, "n_stg2_mistJ_1_b"},   // Land
        {-1, "n_stg2_mistZ_1"}};

    spm::npcdrv::NPCTribeAnimDef moverAnims[] = {
        {0, "stg2_syuuzin_b_S_1"}, // Idle
        {1, "stg2_syuuzin_b_W_1"}, // Walking
        {2, "stg2_syuuzin_b_R_1"}, // Running
        {3, "stg2_syuuzin_b_T_1"}, // Talking (Idle)
        {-1, "stg2_syuuzin_b_Z_1"}};

    spm::npcdrv::NPCTribeAnimDef boodinAnims[] = {
        {0, "S_1"},  // Idle
        {1, "W_1"},  // Walking
        {2, "R_1"},  // Running
        {3, "T_1"},  // Talking
        {24, "T_2"}, // One bow goodbye
        {25, "T_3"}, // Three small bows goodbye (unused)
        {26, "O_1"}, // WHAT
        {-1, "Z_1"}};

    /* static spm::evt_door::DokanDesc temp_mac_04_2_dokan_desc = {
         0, 0, 0, "dokan", "mac_04", "A2D_dokan_1", "A3D_dokan_1", "dan_70", "dokan_1"}; */

    EVT_BEGIN(handle_dj_misc_behavior)
    // Search for dialogue that needs to be run
    IF_EQUAL(GSWF(1671), 1)
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(houraiWearOff), 0, 0)
    END_IF()
    USER_FUNC(handleBlessingWearOff)
    RETURN()
    EVT_END()

    EVT_BEGIN(mover_speech)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
    USER_FUNC(generate_mover_prices, LW(3), LW(4))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverIntro), 0, PTR("me"))
    SWITCH(LW(3))
    CASE_EQUAL(20)
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(moverChoicesLv1))
    CASE_EQUAL(25)
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(moverChoicesLv2))
    CASE_EQUAL(30)
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(moverChoicesLv3))
    CASE_ETC()
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(moverChoicesLv4))
    END_SWITCH()
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 2) // "I'm good, thanks"
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverNo), 0, PTR("me"))
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    END_IF()
    USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(1))
    IF_EQUAL(LW(0), 1)     // Down 5 Floors
    IF_SMALL(LW(1), LW(4)) // Checks if Mario is poor...
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverScam), 0, PTR("me"))
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(4), -1)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_coins, LW(4))
    USER_FUNC(spm::evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 2)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverYes), 0, PTR("me"))
    USER_FUNC(mover_down_5, LW(2))
    USER_FUNC(spm::evt_pouch::evt_pouch_check_have_item, 48, LW(5)) // Removes Pit Key if Mario has one
    IF_EQUAL(LW(5), 1)
    USER_FUNC(spm::evt_pouch::evt_pouch_remove_item, 48)
    END_IF()
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, LW(2), PTR(0))
    END_IF()
    END_IF()
    IF_EQUAL(LW(0), 0)     // Down 2 Floors
    IF_SMALL(LW(1), LW(3)) // Checks if Mario is poor...
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverScam), 0, PTR("me"))
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(3), -1)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_coins, LW(3))
    USER_FUNC(spm::evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 2)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(moverYes), 0, PTR("me"))
    USER_FUNC(mover_down_2, LW(2))
    USER_FUNC(spm::evt_pouch::evt_pouch_check_have_item, 48, LW(5)) // Removes Pit Key if Mario has one
    IF_EQUAL(LW(5), 1)
    USER_FUNC(spm::evt_pouch::evt_pouch_remove_item, 48)
    END_IF()
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, LW(2), PTR(0))
    END_IF()
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_mover_speech)
    RUN_EVT(mover_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_walmart)
    WAIT_FRM(1)
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_speech_2)
    // Merluna prepares judgement. Expand this into something fuller
    IF_EQUAL(GSWF(1669), 0)
    SET(GSWF(1669), 1)
    USER_FUNC(render_judgement, LW(1), LW(2), LW(3), LW(4), LW(5))
    WAIT_MSEC(1000)
    END_IF()
    USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(merlunaJudgement), 0, PTR("me"), LW(2), LW(4))
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(merlunaOptions))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    SWITCH(LW(0))
    CASE_EQUAL(0) // Chooses to receive Judgement
    USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(10))
    IF_SMALL(LW(10), 10) // Checks if Mario is poor...
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaScam), 0, PTR("me"))
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_coins, -10)
    USER_FUNC(spm::evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 2)
    WAIT_MSEC(700)
    USER_FUNC(render_judgement_2, LW(1), LW(3), LW(5))
    IF_EQUAL(LW(5), 0) // Receives Blessing
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_QUIZ_7_SEIKAI1"), 360, 0, 0)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaBless), 0, PTR("me"))
    ELSE() // Receives Curse
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_EVT_QUIZ_FAULT1"), 360, 0, 0)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaCurse), 0, PTR("me"))
    END_IF()
    SWITCH_BREAK()
    CASE_EQUAL(3) // No thanks!
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaDecline), 0, PTR("me"))
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    RETURN()
    SWITCH_BREAK()
    CASE_SMALL_EQUAL(1) // About Blessing...
    SET(LW(6), PTR("me"))
    USER_FUNC(explain_judgement, LW(1), LW(3), 0, LW(6))
    WAIT_MSEC(250)
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    SWITCH_BREAK()      // lol
    CASE_SMALL_EQUAL(2) // About Curse...
    SET(LW(6), PTR("me"))
    USER_FUNC(explain_judgement, LW(1), LW(3), 1, LW(6))
    WAIT_MSEC(250)
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    SWITCH_BREAK() // lol
    END_SWITCH()
    // Merluna disappears after judgement is dispensed
    INLINE_EVT()
    SET(GSWF(1669), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("rebear"), 9, PTR(merluna_walmart))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    END_INLINE()
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_delete, PTR("me"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), 398, 0, 0)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, 398, 0, 0, FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_curse_speech)
    INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 30, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRelieveCurse), 0, PTR("me"))
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(merlunaRelieveOptions))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(1))
    IF_SMALL(LW(1), 100) // Checks if Mario is poor...
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRelieveScam), 0, PTR("me"))
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    ELSE() // ...but if he's not, Proceed tm
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_coins, -100)
    USER_FUNC(spm::evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 2)
    WAIT_MSEC(1000)
    SET(GSW(1601), 0)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRelieveReal), 0, PTR("me"))
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(merlunaRetryOptions))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRetryAccept), 0, PTR("me"))
    RUN_CHILD_EVT(merluna_speech_2)
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRetryDecline), 0, PTR("me"))
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    END_IF()
    END_IF()
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaRelieveDecline), 0, PTR("me"))
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 800, 11)
    WAIT_MSEC(800)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_speech)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, FLOAT(237), FLOAT(101), FLOAT(644), FLOAT(340), FLOAT(50), FLOAT(200), 1000, 11)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(4), LW(5), LW(6))
    USER_FUNC(spm::evt_mario::evt_mario_walk_to, 350, 0, 800)
    USER_FUNC(spm::evt_mario::evt_mario_face_npc, PTR("me"))
    // Merluna first cutscene
    IF_EQUAL(GSWF(1600), 0)
    SET(GSWF(1600), 1)
    INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 28, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 29, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 30, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaIntro), 0, PTR("me"))
    WAIT_MSEC(250)
    MARIO_SPAWN_QUESTION_MARK()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_10"), 0)
    WAIT_MSEC(1000)
    INLINE_EVT()
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 34, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    END_INLINE()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(600)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaIntro2), 0, PTR("me"))
    ELSE() // Not first cutscene
    WAIT_MSEC(1000)
    IF_LARGE(GSW(1601), 0)
    RUN_CHILD_EVT(merluna_curse_speech)
    RETURN()
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(merlunaIntro3), 0, PTR("me"))
    IF_EQUAL(GSWF(1669), 1)
    USER_FUNC(rerender_judgement, LW(1), LW(2), LW(3), LW(4), LW(5))
    END_IF()
    WAIT_MSEC(600)
    END_IF()
    END_IF()
    RUN_CHILD_EVT(merluna_speech_2)
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_merluna_speech)
    RUN_EVT(merluna_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(boodin_speech)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 0)
    SET(LW(6), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 0, 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(boodinIntro), 0, PTR("card"))
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    USER_FUNC(dan_boodin_card_select, LW(4), LW(5), LW(1), LW(2)) // Returns item ID, item name, buy price, and buy price div 2
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 2)
    IF_NOT_EQUAL(LW(4), -1)
    USER_FUNC(spm::evt_msg::evt_msg_print_insert, 1, PTR(boodinItemSelected), 0, PTR("card"), LW(5), LW(1), LW(2))
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(boodinSelect))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    IF_EQUAL(LW(0), 0)
    USER_FUNC(spm::evt_pouch::evt_pouch_get_coins, LW(3))
    IF_SMALL(LW(3), LW(2))
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(boodinClassism), 0, PTR("card"))
    ELSE()
    USER_FUNC(spm::evt_sub::evt_sub_hud_configure, 0)
    WAIT_MSEC(500)
    MUL(LW(2), -1)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_coins, LW(2))
    USER_FUNC(spm::evt_shop::evt_shop_wait_coin_sfx)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("card_item"), LW(4), 0, 0, -1000, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("card_item"), 8)
    USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("card_item"))
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(boodinSatisfied), 0, PTR("card"))
    SET(LW(6), 1)
    END_IF()
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(boodinDecline), 0, PTR("card"))
    END_IF()
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(boodinDecline), 0, PTR("card"))
    END_IF()
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    INLINE_EVT()
    IF_EQUAL(LW(6), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 25, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("card"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 0, 1)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 24, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("card"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 0, 1)
    END_IF()
    END_INLINE()
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_boodin_speech)
    RUN_EVT(boodin_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(new_dan_chest_open_evt)
    USER_FUNC(spm::evt_mobj::evt_mobj_wait_animation_end, PTR("box"), 0)
    USER_FUNC(spm::evt_sub::evt_sub_item_select_menu, 0, PTR(&rfcItems), LW(0), 0)
    IF_SMALL_EQUAL(LW(0), 0) // Select menu cancelled
    USER_FUNC(spm::evt_mobj::evt_mobj_set_anim, PTR("box"), PTR("A_3"))
    USER_FUNC(spm::evt_mobj::evt_mobj_wait_animation_end, PTR("box"), 0)
    USER_FUNC(spm::evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(spm::evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(new_dan_chest_open_evt), 0, 0)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    ELSE()
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(1), LW(2), LW(3))
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), LW(0), 0, LW(1), LW(2), LW(3), 0, 0, 0, 0)
    USER_FUNC(spm::evt_item::evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(spm::evt_item::evt_item_wait_collected, PTR("item"), 0)
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(merluna_setup)
    // Handle music
    IF_EQUAL(GSWF(585), 0)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    ELSE()
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_RELAXATION1"))
    END_IF()
    // Overwrite vanilla chests
    USER_FUNC(spm::evt_mobj::evt_mobj_delete, PTR("box"))
    USER_FUNC(spm::evt_mobj::evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(new_dan_chest_open_evt), 0, 0)
    // Handle blessing wear-off effects in rest floors
    IF_SMALL(GSW(1602), 1)
    USER_FUNC(handleBlessingWearOff)
    END_IF()
    // Initialize NPC draw logic with Merluna overrides.
    SET(LW(1), 0)          // 0 is no NPC, 1 is Flimm, 2 is Merluna, 3 is Boodin
    SET(LW(2), 0)          // 0 is Merluna-neutral, 1 is forcibly active, 2 is forcibly inactive
    IF_LARGE(GSW(1600), 0) // If ANY blessing is active, Merluna will never show up.
    SET(LW(1), 0)
    SET(LW(2), 2)
    END_IF()
    IF_LARGE(GSW(1601), 0) // If ANY curse is active, Merluna will always show up.
    SET(LW(1), 2)
    SET(LW(2), 1)
    END_IF()
    // Run normal NPC draws with Merluna overrides in consideration
    USER_FUNC(spm::evt_sub::evt_sub_random, 100, LW(0))
    IF_EQUAL(LW(2), 0) // Assuming neither a blessing or curse is active
    IF_SMALL(LW(0), 40)
    SET(LW(1), 1)
    ELSE()
    IF_SMALL(LW(0), 80)
    SET(LW(1), 2)
    ELSE()
    SET(LW(1), 3)
    END_IF()
    END_IF()
    END_IF()
    IF_EQUAL(LW(2), 2) // Assuming Merluna is forcibly inactive
    IF_SMALL(LW(0), 70)
    SET(LW(1), 1)
    ELSE()
    SET(LW(1), 3)
    END_IF()
    END_IF()
    // Handle NPC draws
    IF_EQUAL(LW(1), 1) // Flimm
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("roten"), PTR("n_machi_roten"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("roten"), -1)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("roten"), 14, PTR(&spm::dan::rotenTribeAnimDefs))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("roten"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("roten"), 1, 0x4400004)
    USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, PTR("roten"), 1, 32)
    USER_FUNC(spm::evt_npc::evt_npc_add_flip_part, PTR("roten"))
    USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("roten"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("roten"), 398, 0, 0)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("roten"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("roten"), 11, 40)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("roten"), 10, 60)
    USER_FUNC(spm::evt_npc::evt_npc_modify_part, PTR("roten"), -1, 11, 40)
    USER_FUNC(spm::evt_npc::evt_npc_modify_part, PTR("roten"), -1, 10, 60)
    USER_FUNC(spm::evt_shop::evt_shop_set_defs, PTR(&spm::dan::rotenShopDef), 1)
    END_IF()
    IF_EQUAL(LW(1), 2) // Merluna
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("rebear"), PTR("n_rebear"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("rebear"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(merlunaAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("rebear"), 0, true)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("rebear"), 398, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("rebear"), 9, PTR(fwd_merluna_speech))
    USER_FUNC(spm::evt_npc::evt_npc_add_flip_part, PTR("rebear"))
    END_IF()
    IF_EQUAL(LW(1), 3) // Boodin
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("card"), PTR("n_machi_c_card"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("card"), 14, PTR(boodinAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("card"), 0, 1)
    USER_FUNC(spm::evt_npc::evt_npc_flag8_onoff, PTR("card"), 1, 0x4400004)
    USER_FUNC(spm::evt_npc::evt_npc_animflag_onoff, PTR("card"), 1, 32)
    USER_FUNC(spm::evt_npc::evt_npc_add_flip_part, PTR("card"))
    USER_FUNC(spm::evt_npc::evt_npc_set_move_mode, PTR("card"), 1)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("card"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("card"), 398, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("card"), 9, PTR(fwd_boodin_speech))
    USER_FUNC(dan_boodin_setup_cards)
    END_IF()
    // Initialize Mover spawn logic, independent of all other NPCs
    USER_FUNC(get_mover_rng, LW(0))
    IF_SMALL_EQUAL(LW(0), 34)
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("mover"), PTR("n_stg2_syuuzin_b"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("mover"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(moverAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("mover"), 0, true)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("mover"), 200, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("mover"), 9, PTR(fwd_mover_speech))
    END_IF()
    RETURN_FROM_CALL()

    // Dialogue to determine whether to enable/disable custom Pit music
    EVT_BEGIN(determine_custom_music)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(musicOption), 0, 0)
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(yesNoSelect_NoByDefault))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    IF_NOT_EQUAL(LW(0), 0)
    SET(GSWF(585), 0)
    ELSE()
    SET(GSWF(585), 1)
    END_IF()
    RETURN_FROM_CALL()

    // Overwrite Pit music
    EVT_BEGIN(custom_pit_music)
    IF_EQUAL(GSWF(585), 0)
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    ELSE()
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_RELAXATION1"))
    END_IF()
    RETURN_FROM_CALL()

    EVT_BEGIN(spectre_logic)
    IF_EQUAL(GSW(1600), 1)
    USER_FUNC(rand100, LW(1))
    IF_SMALL(LW(1), 25)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_NOT_EQUAL(LW(1), 156)
    IF_NOT_EQUAL(LW(1), 157)
    USER_FUNC(spm::evt_npc::evt_npc_get_max_hp, PTR("me"), LW(1)) // Set to LW(1) after test
    USER_FUNC(spectreFloor, LW(1), LW(2))
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(7))
    ADD(LW(4), LW(7))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_recovery"), LW(3), LW(4), LW(5), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_pouch::evt_pouch_add_hp, LW(2))
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    s32 danPitLock[] = {48, -1};

    // Like so many other functions used in this mod, this was adapted heavily from decomp dan.c
    // Thank you Seeky! This mod and many others would not exist without your work.
    // You are greatly appreciated by all of us in the SPM Community.
    s32 evt_dan_handle_key_failsafe_new(spm::evtmgr::EvtEntry *entry, bool isFirstCall)
    {
        (void)isFirstCall;
        // Check whether the key exists anywhere
        if (
            !spm::dan::danCheckEnemyInMapBbox() && !spm::dan::danCheckKeyInMapBbox() &&
            !spm::mario_pouch::pouchCheckHaveItem(48) &&
            !spm::itemdrv::itemCheckForId(48))
        {
            // Spawn the key at Mario's position if not
            spm::mario::MarioWork *mario = spm::mario::marioGetPtr();
            spm::itemdrv::itemEntry(NULL, 48, 1, mario->position.x, mario->position.y, mario->position.z, NULL, 0);
            return 2;
        }
        else
        {
            return 0;
        }
    }
    EVT_DECLARE_USER_FUNC(evt_dan_handle_key_failsafe_new, 0)

    EVT_BEGIN(homogenize_lock_interact)
    WAIT_MSEC(0)
    RETURN()
    EVT_END()

    EVT_BEGIN(dan_enemy_room_init_evt_new)
    SET(LW(0), GSW(1))
    USER_FUNC(get_mover_rng, LW(1)) // Movers
    IF_SMALL_EQUAL(LW(1), 24)
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("mover"), PTR("n_stg2_syuuzin_b"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("mover"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(moverAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("mover"), 0, true)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("mover"), -40, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("mover"), 9, PTR(fwd_mover_speech))
    END_IF()
    USER_FUNC(spm::dan::evt_dan_read_data)
    USER_FUNC(spm::dan::evt_dan_handle_map_parts, LW(0))
    USER_FUNC(spm::dan::evt_dan_handle_dokans, LW(0))
    USER_FUNC(spm::evt_door::evt_door_set_dokan_descs, PTR(&spm::dan::danDokanDescs), 8)
    SET(LW(1), 0)
    USER_FUNC(spm::dan::evt_dan_handle_doors, LW(0), LW(1), LW(10), LW(11), LW(2), LW(3), LW(4))
    USER_FUNC(spm::evt_door::evt_door_set_map_door_descs, PTR(&spm::dan::danMapDoorDescs), 2)
    USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 0, LW(10))
    USER_FUNC(spm::evt_door::evt_door_enable_disable_map_door_desc, 0, LW(11))
    USER_FUNC(spm::evt_mobj::evt_mobj_zyo, PTR("lock_00"), 48, LW(2), LW(3), LW(4), 0, PTR(homogenize_lock_interact), PTR(spm::dan::dan_lock_open_evt), 0) // Only one lock type
    USER_FUNC(spm::dan::evt_dan_make_spawn_table, LW(0))
    SET(LW(10), 0)
    SET(LW(9), 0)
    DO(16)
    USER_FUNC(spm::dan::evt_dan_get_enemy_info, LW(0), LW(10), LW(11), LW(12))
    IF_LARGE(LW(12), 0)
    DO(LW(12))
    USER_FUNC(spm::dan::evt_dan_get_enemy_spawn_pos, LW(9), LW(0), LW(10), LW(13), LW(14), LW(15))
    ADD(LW(9), 1)
    USER_FUNC(spm::evt_npc::evt_npc_entry_from_template, 0, LW(11), LW(13), LW(14), LW(15), LW(5), EVT_NULLPTR)
    USER_FUNC(create_holographic_enemy, LW(5), LW(6))
    IF_EQUAL(LW(6), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_animpose_disp_callback, LW(5), PTR(spm::mi4::mi4MimiHolographicEffect), 0)
    END_IF()
    WHILE()
    END_IF()
    ADD(LW(10), 1)
    WHILE()
    IF_EQUAL(GSW(1601), 1) // Tatarian Aster
    USER_FUNC(rand100, LW(8))
    IF_SMALL(LW(8), 50)
    USER_FUNC(spm::dan::evt_dan_decide_key_enemy, 48)
    END_IF()
    ELSE()
    USER_FUNC(spm::dan::evt_dan_decide_key_enemy, 48)
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_freeze_all)
    USER_FUNC(spm::evt_hit::evt_hitobj_attr_onoff, 1, 1, PTR("A2"), 1073741824)
    USER_FUNC(spm::evt_hit::evt_hitobj_attr_onoff, 1, 1, PTR("A3"), 536870912)
    USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(spm::evt_map::evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    RUN_CHILD_EVT(spm::evt_door::door_init_evt)
    ADD(GSW(1), 1)
    IF_EQUAL(GSWF(585), 0) // Custom music
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    ELSE()
    USER_FUNC(spm::evt_snd::evt_snd_bgmon, 0, PTR("BGM_EVT_RELAXATION1"))
    END_IF()
    USER_FUNC(spm::evt_snd::evt_snd_set_sfx_reverb_mode, 0)
    IF_SMALL(GSW(1602), 1)
    USER_FUNC(handleBlessingWearOff)
    END_IF()
    INLINE_EVT()
    USER_FUNC(spm::evt_door::evt_door_wait_flag, 256)
    USER_FUNC(spm::evt_sub::evt_sub_display_room_name, 1, 6)
    END_INLINE()
    USER_FUNC(spm::dan::evt_dan_start_countdown)
    INLINE_EVT()
    USER_FUNC(spm::evt_door::evt_door_wait_flag, 256)
    USER_FUNC(spm::evt_npc::evt_npc_unfreeze_all)
    USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_init, 11, 255, 0, 1000)
    DO(0)
    USER_FUNC(spm::evt_sub::evt_sub_intpl_msec_get_value)
    USER_FUNC(spm::dan::evt_dan_get_door_names, LW(2), LW(3))
    USER_FUNC(spm::evt_map::evt_mapobj_color, 1, LW(2), 255, 255, 255, LW(0))
    WAIT_FRM(1)
    IF_EQUAL(LW(1), 0)
    DO_BREAK()
    END_IF()
    WHILE()
    USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, LW(2), 1)
    END_INLINE()
    INLINE_EVT()
    USER_FUNC(evt_dan_handle_key_failsafe_new) // Completely overhauls key despawn behavior
    END_INLINE()
    USER_FUNC(spm::evt_sub::func_800d4de4, 1, 0)
    RETURN()
    EVT_END()

    // Dialogue to determine quickstart or no
    EVT_BEGIN(determine_quickstart)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(quickstartText), 0, 0)
    USER_FUNC(spm::evt_msg::evt_msg_select, 1, PTR(quickstartOptions))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    SWITCH(LW(0))
    CASE_EQUAL(1) // 2-1
    SET(GSW(137), 1)
    SET(GSW(0), 78)
    USER_FUNC(start_from_21)
    CASE_EQUAL(2) // 6-1
    SET(GSW(137), 2)
    SET(GSW(0), 226)
    USER_FUNC(start_from_61)
    CASE_EQUAL(3) // Postgame
    SET(GSW(137), 3)
    SET(GSW(0), 424)
    USER_FUNC(start_from_eg)
    CASE_ETC() // Normal pills
    SET(GSW(137), 0)
    END_SWITCH()
    IF_NOT_EQUAL(GSW(137), 0)
    SET(GSWF(2), 1)
    SET(GSWF(386), 1)
    SET(GSWF(387), 1)
    SET(GSWF(431), 1)
    SET(GSWF(501), 1)
    SET(GSWF(512), 1)
    SET(GSWF(612), 1)
    SET(GSWF(614), 1)
    SET(GSWF(615), 1)
    SET(GSWF(616), 1)
    USER_FUNC(spm::evt_seq::evt_seq_set_seq, spm::seqdrv::SEQ_MAPCHANGE, PTR("mac_05"), PTR("elv1"))
    END_IF()
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_mario_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 523, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_peach_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 458, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_peach_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 521, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_bowser_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 476, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_bowser_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 536, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_luigi_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 512, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(overwrite_dark_luigi_card_chest)
    USER_FUNC(spm::evt_item::evt_item_entry, PTR("item"), 231, 0, LW(0), LW(1), LW(2), 0, 0, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(disable_flopside_pit_entrance)
    USER_FUNC(spm::evt_door::evt_door_enable_disable_dokan_desc, 0, PTR("dokan_1"))
    USER_FUNC(spm::evt_map::evt_mapobj_flag_onoff, 1, 1, PTR("dokan_1"), 1)
    USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A2d_dokan_1"), 1, 0)
    USER_FUNC(spm::evt_hit::evt_hitobj_onoff, PTR("A3d_dokan_1"), 1, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(patch_pit_exit)
    SET(GSW(1601), 0)
    SET(GSW(1600), 0)
    SET(GSW(1602), 0)
    USER_FUNC(declare_shadoo_stats)
    USER_FUNC(spm::evt_door::evt_door_set_dokan_descs, PTR(&new_dan_70_dokan_desc), 1)
    RETURN_FROM_CALL()

    EVT_BEGIN(jimbo_speech)
    USER_FUNC(spm::evt_mario::evt_mario_key_off, 1)
    USER_FUNC(spm::evt_cam::evt_cam_get_pos, 5, LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, LW(0), 60, LW(2), EVT_NULLPTR, 60, -1100, 1000, 11)
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(3), LW(4), LW(5))
    SUB(LW(3), 45)
    USER_FUNC(spm::evt_mario::evt_mario_walk_to, LW(3), LW(5), 500)
    USER_FUNC(spm::evt_mario::evt_mario_face_npc, PTR("me"))
    WAIT_MSEC(1000)
    // Jimbo first cutscene
    IF_EQUAL(GSWF(586), 0)
    SET(GSWF(586), 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(jimboIntro), 0, PTR("me"))
    ELSE()
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(jimboIntro2), 0, PTR("me"))
    END_IF()
    USER_FUNC(spm::evt_cam::evt_cam_zoom_to_coords, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    RETURN()
    EVT_END()

    EVT_BEGIN(fwd_jimbo_speech)
    RUN_EVT(jimbo_speech)
    RETURN()
    EVT_END()

    EVT_BEGIN(patch_mac_05)
    SET(GSW(1600), 0)
    SET(GSW(1601), 0)
    SET(GSW(1602), 0)
    SET(GSW(1610), 0)
    SET(GSW(1611), 0)
    RUN_CHILD_EVT(handle_dj_misc_behavior)
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR("jimbo"), PTR("e_antho"), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("jimbo"), mod::cutscene_helpers::NPCProperty::ANIMS, PTR(heihoAnims))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("jimbo"), 0, true)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("jimbo"), -50, 0, -1250)
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR("jimbo"), 9, PTR(fwd_jimbo_speech))
    USER_FUNC(spm::evt_npc::evt_npc_set_scale, PTR("jimbo"), FLOAT(1.1287), FLOAT(1.1287), FLOAT(1.1287))
    RETURN_FROM_CALL()

    EVT_BEGIN(global_operations)
    USER_FUNC(spm::evt_sub::evt_sub_get_mapname, 0, LW(14))
    USER_FUNC(evtCompareStrings, LW(14), PTR("dan"), LW(15))
    IF_NOT_EQUAL(LW(15), 1)
    RETURN()
    ELSE()
    DO(0)
    IF_EQUAL(GSWF(1670), 1) // If Migraine has just been activated
    SET(GSWF(1670), 0)
    INLINE_EVT()
    WAIT_MSEC(500)
    USER_FUNC(migraineShortenCurseTimers)
    END_INLINE()
    END_IF()                // End Migraine check
    IF_EQUAL(GSWF(1672), 1) // Hourai Doll cutscene
    SET(GSWF(1672), 0)
    INLINE_EVT()
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_mario::evt_mario_get_height, LW(7))
    ADD(LW(4), LW(7))
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    WAIT_MSEC(700)
    SUB(LW(4), 7)
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_recovery"), LW(3), LW(4), LW(5), -1, 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), 10)
    ADD(LW(4), 10)
    ADD(LW(5), 10)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), -20)
    ADD(LW(4), -20)
    ADD(LW(5), -20)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(4), 20)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRM(2)
    ADD(LW(3), 20)
    ADD(LW(4), -20)
    ADD(LW(5), 20)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_APPEAR1"), LW(3), LW(4), LW(5))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(3), LW(4), LW(5), FLOAT(3.5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MSEC(200)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("I_2"), 0)
    SET(GSW(1602), 19)
    WAIT_MSEC(1800)
    IF_EQUAL(GSWF(1673), 0)
    SET(GSWF(1673), 1)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(houraiFirstUse), 0, 0)
    ELSE()
    WAIT_MSEC(700)
    END_IF()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario_motion_chg_mot, 0)
    USER_FUNC(spm::evt_mario::evt_mario_key_on)
    END_INLINE()
    END_IF()               // End Hourai Doll activation check
    IF_EQUAL(GSW(1600), 3) // Begin Paramita active check
    // Handle cooldown timer first
    IF_LARGE(GSW(1610), 0)  // Seconds timer above 0
    IF_LARGE(GSW(1611), 0)  // Frame timer above 0
    SUB(GSW(1611), 1)       // Reduce frame timer by 1
    ELSE()                  // Frame timer hits 0, underflow
    SET(GSW(1611), 59)      // Reset frames to 60
    SUB(GSW(1610), 1)       // Subtract 1 second from the primary Paramita timer
    IF_EQUAL(GSW(1610), 20) // Play sfx when cooldown begins
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(4), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(0), LW(1), LW(2))
    END_IF()
    END_IF()
    ELSE()
    IF_LARGE(GSW(1611), 0) // If sec = 0, set frame timer to 0
    SET(GSW(1611), 0)
    END_IF()
    END_IF()
    // Handle Paramita activation detection
    IF_EQUAL(GSW(1610), 0)
    IF_EQUAL(GSW(1611), 0)              // Both timers must be zeroed
    USER_FUNC(evt_mario_chk_key, LW(0)) // Mario must be keyed on
    IF_EQUAL(LW(0), 1)
    USER_FUNC(spm::evt_sub::evt_key_get_button, 0, LW(0))
    IF_FLAG(LW(0), 0x400) // B must be a button held
    SET(GSW(1610), 40)
    SET(GSW(1611), 60) // Sets Paramita timers
    USER_FUNC(spm::evt_mario::evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(4), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(0), LW(1), LW(2))
    USER_FUNC(paramitaFloor, LW(0))
    USER_FUNC(spm::evt_mario::evt_mario_take_damage, 2, 0, 0, 0, 0, LW(0))
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    END_IF()
    WAIT_FRM(1)
    WHILE()
    END_IF()
    RETURN()
    EVT_END()

    EVT_BEGIN(run_global_operations)
    RUN_EVT(global_operations)
    RETURN_FROM_CALL()

    EVT_BEGIN(dan_enter_pipe_wait)
    WAIT_MSEC(0)
    RETURN_FROM_CALL()

    // TEMP
    /*  EVT_BEGIN(temp_pipe)
      USER_FUNC(spm::evt_door::evt_door_set_dokan_descs, PTR(&temp_mac_04_2_dokan_desc), 1)
      RETURN_FROM_CALL() */

    // Dark Puff contact ATK patch
    EVT_BEGIN(d_puff_dir_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 1)
    RETURN_FROM_CALL()

    // Dark Puff projectile ATK patch
    EVT_BEGIN(d_puff_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 2)
    RETURN_FROM_CALL()

    // Ice Bro projectile ATK patch
    EVT_BEGIN(i_bro_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Ice Bro contact ATK patch
    EVT_BEGIN(i_bro_dir_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Hyper Goomba contact ATK patch
    EVT_BEGIN(h_goomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Hyper Paragoomba contact ATK patch
    EVT_BEGIN(h_pgoomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    RETURN_FROM_CALL()

    // Red Magikoopa projectile ATK patch
    EVT_BEGIN(r_magi_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Tileoid PU ATK patch
    EVT_BEGIN(p_tile_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Tileoid PU speed patch
    EVT_BEGIN(p_tile_speed)
    USER_FUNC(patch_tileoid_pu)
    RETURN_FROM_CALL()

    // Kilo Muth ATK/DEF patch
    EVT_BEGIN(k_muth_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Spiked Gloomba ATK patch
    EVT_BEGIN(s_gloomba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // Shady Koopa/Flip Shady Koopa ATK patch
    EVT_BEGIN(uranoko_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 4)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 2, 4)
    RETURN_FROM_CALL()

    // Bleepboxer projectile ATK patch
    EVT_BEGIN(bleep_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 3)
    RETURN_FROM_CALL()

    // White Clubba ATK patch
    EVT_BEGIN(w_clubba_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 4)
    RETURN_FROM_CALL()

    // Shady Boomerang Bro contact ATK patch
    EVT_BEGIN(sbb_dir_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    RETURN_FROM_CALL()

    // Shady Boomerang Bro proj ATK patch
    EVT_BEGIN(sbb_proj_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 5)
    RETURN_FROM_CALL()

    // Dark Mario ATK patch
    EVT_BEGIN(d_mario_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Dark Mario ATK patch
    EVT_BEGIN(d_mario_atk_again)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 2, 10)
    RETURN_FROM_CALL()

    // Dark Peach ATK patch
    EVT_BEGIN(d_peach_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Dark Bowser ATK patch
    EVT_BEGIN(d_bowser_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Dark Bowser ATK patch 2
    EVT_BEGIN(d_bowser_atk_again)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 2, 5)
    RETURN_FROM_CALL()

    // Dark Luigi ATK patch
    EVT_BEGIN(d_luigi_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 5)
    RETURN_FROM_CALL()

    // Dark Luigi ATK patch 2
    EVT_BEGIN(d_luigi_atk_again)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(10))
    IF_EQUAL(LW(10), 331)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 5)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    END_IF()
    RETURN_FROM_CALL()

    // Shady Magikoopa projectile speedup
    EVT_BEGIN(s_magi_proj_speed)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(9))
    IF_EQUAL(LW(9), 68)
    USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(0), LW(1), LW(2), 5000, FLOAT(300.0), 0, 0, 0, 0)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_glide_to, PTR("me"), LW(0), LW(1), LW(2), 5000, FLOAT(120.0), 0, 0, 0, 0)
    END_IF()
    RETURN_FROM_CALL()

    // Shady Magikoopa projectile ATK
    EVT_BEGIN(s_magi_proj_atk)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), 1, 5)
    RETURN_FROM_CALL()

    // Shady Magikoopa animation delay
    EVT_BEGIN(s_magi_delay)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(9))
    SWITCH(LW(9))
    CASE_EQUAL(66)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    WAIT_MSEC(666)
    SWITCH_BREAK()
    CASE_EQUAL(67)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    WAIT_MSEC(666)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_KAMEKU_MAGIC_SING1"), PTR("me"))
    END_SWITCH()
    RETURN_FROM_CALL()

    // White Clubba movement speed
    EVT_BEGIN(w_clubba_mvmt)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(14))
    IF_EQUAL(LW(14), 95)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(3), LW(5), 0, 120, 4, 0, 0)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(3), LW(5), 0, 60, 4, 0, 0)
    END_IF()
    RETURN_FROM_CALL()

    // White Clubba triple attack
    EVT_BEGIN(w_clubba_triple)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(14))
    IF_EQUAL(LW(14), 95)
    IF_SMALL(LW(6), 128)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    END_IF()
    ELSE()
    IF_SMALL(LW(6), 60)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_E_GABON_HURU1"), PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 26, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 27, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("me"), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("me"), 1, 1)
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    // Shady Koopa AI rework
    EVT_BEGIN(uranoko_brain)
    USER_FUNC(spm::evt_npc::evt_npc_get_unitwork, PTR("me"), 9, LW(8))
    IF_EQUAL(LW(8), 1)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    USER_FUNC(spm::evt_npc::evt_npc_get_axis_movement_unit, PTR("me"), LW(9))
    MUL(LW(9), -1)
    USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), LW(9))
    END_IF()
    RETURN_FROM_CALL()

    // Shady Koopa movement speed
    EVT_BEGIN(uranoko_mvmt)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(4), LW(5), LW(6))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("target"), LW(7), LW(8), LW(9))
    SUB(LW(7), LW(4))
    IF_SMALL(LW(7), 0)
    SUB(LW(1), 250)
    ELSE()
    ADD(LW(1), 250)
    END_IF()
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 1)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, 300, 4, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    RETURN_FROM_CALL()

    EVT_BEGIN(uranoko_dendrite)
    USER_FUNC(spm::evt_npc::evt_npc_set_unitwork, PTR("me"), 9, 0)
    RETURN_FROM_CALL()

    // Kamikaze Goomba explosion visuals & second attack
    EVT_BEGIN(kami_no_dokkan)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(11))
    IF_EQUAL(LW(11), 10)
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    ADD(LW(8), 50)
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(8), LW(9), 800, 0, 45, 0, 16, 0)
    //  USER_FUNC(spm::evt_npc::func_801072a4, PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    USER_FUNC(spm::evt_eff::evt_eff, 0, PTR("spm_explosion"), 2, LW(11), LW(8), LW(9), FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_CAMEREBOM2_EXPLOSION1"), LW(11), LW(8), LW(9))
    INLINE_EVT()
    USER_FUNC(spm::evt_cam::evt_cam_shake, 5, FLOAT(0.8), FLOAT(0.8), FLOAT(0.0), 173, 0)
    END_INLINE()
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("target"), LW(11), LW(8), LW(9))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(13), LW(14), LW(15))
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(11), LW(14), LW(9), 500, 0, 30, 0, 16, 0)
    //  USER_FUNC(spm::evt_npc::func_801072a4, PTR("me"))
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(11), LW(8), LW(9))
    END_IF()
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_3d, PTR("SFX_E_KURIBOO_LANDING1"), LW(11), LW(8), LW(9))
    RETURN_FROM_CALL()

    // Skellobyte delay shorten
    EVT_BEGIN(skello_ai)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    WAIT_MSEC(0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    WAIT_MSEC(0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(spm::evt_npc::evt_npc_arc_to, PTR("me"), LW(0), LW(1), LW(2), 200, 0, FLOAT(10.0), 0, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    // Skellobyte faster attacks
    EVT_BEGIN(skello_ai2)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(255.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(255.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(0), LW(2), 0, FLOAT(100.0), 28, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    // Skellobyte faster movement in general
    EVT_BEGIN(skello_ai3)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(6))
    SWITCH(LW(6))
    CASE_EQUAL(225)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(60.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_EQUAL(228)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(60.0), 28, 0, 0)
    SWITCH_BREAK()
    CASE_ETC()
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("me"), LW(1), LW(3), 0, FLOAT(40.0), 28, 0, 0)
    SWITCH_BREAK()
    END_SWITCH()
    RETURN_FROM_CALL()

    /*
        EVT_BEGIN(uranoko_mvmtcheck)
        LBL(73)
        USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("target"), LW(4), LW(5), LW(6))
        USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("me"), LW(7), LW(8), LW(9))
        SUB(LW(8), LW(5))
        IF_LARGE(LW(8), 50)
        WAIT_FRM(1)
        GOTO(73)
        END_IF()
        IF_SMALL(LW(8), -50)
        WAIT_FRM(1)
        GOTO(73)
        END_IF()
        RETURN_FROM_CALL() */

    // IF_SMALL(LW(7), 0)
    // USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), 1)
    // ELSE()
    // USER_FUNC(spm::evt_npc::evt_npc_set_axis_movement_unit, PTR("me"), -1)
    // END_IF()

    // Dark Lakitu stats
    EVT_BEGIN(dlak_stats)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 3)
    USER_FUNC(setHitFlags, 0)
    USER_FUNC(setHitboxSize, 0, FLOAT(24), FLOAT(32), FLOAT(10))
    END_IF()
    RETURN_FROM_CALL()

    // Dark Lakitu proj limit patch
    EVT_BEGIN(dlak_limit)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(spm::temp_unk::lakitu_count_spinies, 36, LW(0))
    ELSE()
    USER_FUNC(spm::temp_unk::lakitu_count_spinies, 38, LW(0))
    END_IF()
    RETURN_FROM_CALL()

    // Dark Lakitu proj patch
    EVT_BEGIN(dlak_skyblue)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(spm::evt_npc::evt_npc_agb_async, PTR("e_togezo"), LW(0))
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_agb_async, PTR("e_togezo"), LW(0)) // Make this e_togezb somehow, eventually >_<
    END_IF()
    RETURN_FROM_CALL()

    // Dark Lakitu proj patch 2
    EVT_BEGIN(dlak_skyblue_2)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 34)
    USER_FUNC(spm::temp_unk::lakitu_spawn_spiny, 59)
    ELSE()
    USER_FUNC(spm::temp_unk::lakitu_spawn_spiny, 336)
    END_IF()
    RETURN_FROM_CALL()

    // KP Koopa stats
    EVT_BEGIN(kp_stats)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 14)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    //   USER_FUNC(setHitFlags, 1)
    //   USER_FUNC(setFlag2c, 0, 0x304000)
    //   USER_FUNC(setFlag2c, 1, 0x224000)
    //   USER_FUNC(setHitboxSize, 0, FLOAT(20), FLOAT(22), FLOAT(20))
    //   USER_FUNC(setHitboxSize, 1, FLOAT(14), FLOAT(10), FLOAT(20))
    END_IF()
    RETURN_FROM_CALL()

    // Shy Guy stats
    EVT_BEGIN(heiho_stats)
    USER_FUNC(spm::evt_npc::evt_npc_get_property, PTR("me"), 13, LW(1))
    IF_EQUAL(LW(1), 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 1)
    ELSE()
    USER_FUNC(spm::evt_npc::evt_npc_set_part_attack_power, PTR("me"), -1, 2)
    IF_LARGE(LW(1), 500)
    //  USER_FUNC(setHitboxSize, 0, FLOAT(27), FLOAT(27), FLOAT(27))
    USER_FUNC(spm::evt_npc::evt_npc_set_scale, PTR("me"), FLOAT(1.1287), FLOAT(1.1287), FLOAT(1.1287))
    END_IF()
    END_IF()
    RETURN_FROM_CALL()

    static void evtPatches()
    {
        // Initialize the patches to the EVT interpreter to add custom opcodes
        evtpatch::evtmgrExtensionInit();

        /* // Get Flipside Tower's init EVT script
         spm::evtmgr_cmd::EvtScriptCode *flipsideInitEvt = spm::map_data::mapDataPtr("mac_02")->initScript;

         // Add a hook at the beginning of Flipside's init EVT script that will play our custom script first
         evtpatch::hookEvt(flipsideInitEvt, 1, flipside_nastasia_funny); */

        // Render Flopside Pit pipe useless and patch Pit exit pipe
        spm::evtmgr_cmd::EvtScriptCode *disableFlopsidePitEntrance = spm::map_data::mapDataPtr("mac_15")->initScript;
        evtpatch::hookEvt(disableFlopsidePitEntrance, 11, disable_flopside_pit_entrance);
        evtpatch::hookEvt(spm::dan::dan_70_init_evt, 8, patch_pit_exit);

        // Enemy room init evt complete rewrite
        evtpatch::hookEvtReplaceBlock(spm::dan::dan_enemy_room_init_evt, 1, dan_enemy_room_init_evt_new, 71);

        // Get enemy onSpawnScripts from templates
        spm::evtmgr_cmd::EvtScriptCode *dPuffDirAtk = spm::npcdrv::npcEnemyTemplates[357].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dPuffAtk = spm::npcdrv::npcEnemyTemplates[358].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *iBroDirAtk = spm::npcdrv::npcEnemyTemplates[341].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *iBroAtk = spm::npcdrv::npcEnemyTemplates[344].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *hGoombaAtk = spm::npcdrv::npcEnemyTemplates[329].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *hpGoombaAtk = spm::npcdrv::npcEnemyTemplates[328].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *rMagiAtk = spm::npcdrv::npcEnemyTemplates[324].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *pTileAtk = spm::npcdrv::npcEnemyTemplates[379].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *pTileSpeed = spm::npcdrv::npcEnemyTemplates[379].moveScript;
        spm::evtmgr_cmd::EvtScriptCode *kMuthAtk = spm::npcdrv::npcEnemyTemplates[279].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *sGloombaAtk = spm::npcdrv::npcEnemyTemplates[6].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *uranokoAtk = spm::npcdrv::npcEnemyTemplates[10].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *fUranokoAtk = spm::npcdrv::npcEnemyTemplates[13].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *bleepAtk = spm::npcdrv::npcEnemyTemplates[277].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *wClubbaAtk = spm::npcdrv::npcEnemyTemplates[350].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *sbbDirAtk = spm::npcdrv::npcEnemyTemplates[340].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *sbbProjAtk = spm::npcdrv::npcEnemyTemplates[343].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dMarioAtk = spm::npcdrv::npcEnemyTemplates[287].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dPeachAtk = spm::npcdrv::npcEnemyTemplates[288].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dBowserAtk = spm::npcdrv::npcEnemyTemplates[285].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dBowserAtkAgain = spm::npcdrv::npcEnemyTemplates[285].atkScript;
        spm::evtmgr_cmd::EvtScriptCode *dBowserAtkAgainAgain = spm::npcdrv::npcEnemyTemplates[285].moveScript;
        spm::evtmgr_cmd::EvtScriptCode *dLuigiAtk = spm::npcdrv::npcEnemyTemplates[286].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *dLuigiAtkAgain = spm::npcdrv::npcEnemyTemplates[286].onHitScript;
        spm::evtmgr_cmd::EvtScriptCode *sMagiProjSpeed = spm::npcdrv::npcEnemyTemplates[347].moveScript;
        spm::evtmgr_cmd::EvtScriptCode *sMagiProjAtk = spm::npcdrv::npcEnemyTemplates[347].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *wClubbaTriple = spm::npcdrv::npcEnemyTemplates[350].atkScript;
        spm::evtmgr_cmd::EvtScriptCode *uranokoMvmt = spm::npcdrv::npcEnemyTemplates[10].atkScript;
        spm::evtmgr_cmd::EvtScriptCode *uranokoBrain = spm::npcdrv::npcEnemyTemplates[10].moveScript;
        spm::evtmgr_cmd::EvtScriptCode *uranokoOnDmgTaken = spm::npcdrv::npcEnemyTemplates[10].onHitScript;
        spm::evtmgr_cmd::EvtScriptCode *kamiKuriAtk = spm::npcdrv::npcEnemyTemplates[330].atkScript;
        spm::evtmgr_cmd::EvtScriptCode *dLakAtk = spm::npcdrv::npcEnemyTemplates[30].atkScript;
        spm::evtmgr_cmd::EvtScriptCode *dLakStats = spm::npcdrv::npcEnemyTemplates[30].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *kpStats = spm::npcdrv::npcEnemyTemplates[7].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *goombaStats = spm::npcdrv::npcEnemyTemplates[2].onSpawnScript;
        spm::evtmgr_cmd::EvtScriptCode *commonDeathScript = spm::npcdrv::npcEnemyTemplates[350].deathScript;

        // Enemy stat overwrite
        evtpatch::hookEvtReplace(dPuffDirAtk, 2, d_puff_dir_atk);
        evtpatch::hookEvtReplace(dPuffAtk, 11, d_puff_atk);
        evtpatch::hookEvtReplace(iBroDirAtk, 1, i_bro_dir_atk);
        evtpatch::hookEvtReplace(iBroAtk, 1, i_bro_atk);
        evtpatch::hookEvtReplace(hGoombaAtk, 2, h_goomba_atk);
        evtpatch::hookEvtReplace(hpGoombaAtk, 2, h_pgoomba_atk);
        evtpatch::hookEvtReplace(rMagiAtk, 1, r_magi_atk);
        evtpatch::hookEvtReplace(pTileAtk, 1, p_tile_atk);
        evtpatch::hookEvt(pTileSpeed, 16, p_tile_speed);
        evtpatch::hookEvt(spm::npc_tile::npc_tile_common_onspawn_evt, 6, p_tile_speed);
        evtpatch::hookEvtReplace(kMuthAtk, 1, k_muth_atk);
        evtpatch::hookEvtReplace(sGloombaAtk, 2, s_gloomba_atk);
        evtpatch::hookEvtReplace(uranokoAtk, 5, uranoko_atk);
        evtpatch::hookEvtReplace(fUranokoAtk, 2, uranoko_atk);
        evtpatch::hookEvtReplace(bleepAtk, 1, bleep_atk);
        evtpatch::hookEvtReplace(wClubbaAtk, 1, w_clubba_atk);
        evtpatch::hookEvtReplace(sbbDirAtk, 1, sbb_dir_atk);
        evtpatch::hookEvtReplace(sbbProjAtk, 1, sbb_proj_atk);
        evtpatch::hookEvtReplace(dMarioAtk, 3, d_mario_atk_again);
        evtpatch::hookEvtReplace(dMarioAtk, 4, d_mario_atk);
        evtpatch::hookEvtReplace(dPeachAtk, 1, d_peach_atk);
        evtpatch::hookEvtReplace(dBowserAtk, 35, d_bowser_atk);
        evtpatch::hookEvtReplace(dBowserAtk, 36, d_bowser_atk_again);
        evtpatch::hookEvtReplaceBlock(dBowserAtkAgain, 12, d_bowser_atk, 13);
        evtpatch::hookEvtReplaceBlock(dBowserAtkAgain, 219, d_bowser_atk, 220);
        evtpatch::hookEvtReplaceBlock(dBowserAtkAgain, 248, d_bowser_atk, 249);
        evtpatch::hookEvtReplaceBlock(dBowserAtkAgainAgain, 12, d_bowser_atk, 13);
        writeWord(&spm::temp_unk::bowser_spawn_fire, 0x488, 0x38E0000A); // Patch fire ATK
        evtpatch::hookEvtReplace(dLuigiAtk, 27, d_luigi_atk);
        evtpatch::hookEvt(dLuigiAtkAgain, 2, d_luigi_atk_again);
        writeWord(&spm::temp_unk::luigi_superjump_atk, 0x684, 0x3800000A); // Patch initial superjump ATK
        evtpatch::hookEvtReplace(sMagiProjSpeed, 24, s_magi_proj_speed);
        evtpatch::hookEvtReplace(sMagiProjAtk, 1, s_magi_proj_atk);
        evtpatch::hookEvtReplace(wClubbaTriple, 36, w_clubba_mvmt);
        evtpatch::hookEvtReplace(wClubbaTriple, 50, w_clubba_mvmt);
        evtpatch::hookEvtReplaceBlock(wClubbaTriple, 52, w_clubba_triple, 59);
        evtpatch::hookEvtReplace(uranokoMvmt, 16, uranoko_mvmt);
        //    evtpatch::hookEvt(uranokoMvmt, 1, uranoko_mvmtcheck);
        evtpatch::hookEvt(uranokoBrain, 1, uranoko_brain);
        evtpatch::hookEvt(uranokoOnDmgTaken, 1, uranoko_dendrite);
        evtpatch::hookEvtReplace(kamiKuriAtk, 51, kami_no_dokkan);
        evtpatch::hookEvtReplace(spm::temp_unk::skellobits_unk7, 7, skello_ai);
        evtpatch::hookEvtReplace(spm::temp_unk::skellobits_unk7, 25, skello_ai2);
        evtpatch::hookEvtReplace(spm::temp_unk::skellobits_unk2, 16, skello_ai3);
        evtpatch::hookEvtReplace(spm::temp_unk::skellobits_unk8, 16, skello_ai2);
        evtpatch::hookEvt(spm::temp_unk::kameks_unk7, 18, s_magi_delay);
        evtpatch::hookEvtReplace(spm::temp_unk::dark_broom_kamek_unk7, 17, s_magi_delay);
        evtpatch::hookEvtReplace(dLakAtk, 7, dlak_limit);
        evtpatch::hookEvtReplace(dLakAtk, 12, dlak_skyblue);
        evtpatch::hookEvtReplace(dLakAtk, 31, dlak_skyblue_2);
        evtpatch::hookEvtReplace(spm::temp_unk::lakitu_onspawn_child, 8, dlak_stats);
        evtpatch::hookEvtReplace(kpStats, 4, kp_stats);
        evtpatch::hookEvtReplace(goombaStats, 2, heiho_stats);
        evtpatch::hookEvtReplaceBlock(spm::temp_unk::goomba_unk2_child, 12, heiho_g_panic, 16);

        // Ninjoe patched to only bomb 10% of the time, thanks Lily!
        writeWord(&spm::temp_unk::ninjoe_bomb_thing, 0xDC, 0x2C03000A);

        // Post-Shadoo chest reward overwrites
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_mario_chest_open_evt, 3, overwrite_dark_mario_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_peach_chest_open_evt, 3, overwrite_peach_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_peach_chest_open_evt, 3, overwrite_dark_peach_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_bowser_chest_open_evt, 3, overwrite_bowser_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_bowser_chest_open_evt, 3, overwrite_dark_bowser_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_luigi_chest_open_evt, 3, overwrite_luigi_card_chest);
        evtpatch::hookEvtReplace(spm::dan::dan_70_dark_luigi_chest_open_evt, 3, overwrite_dark_luigi_card_chest);

        // Pit music replacement
        evtpatch::hookEvt(spm::evt_door::evt_door_dan_dokan, 79, determine_custom_music);

        // Pit room pipe speedup
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_one, 108, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_two, 92, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_three, 85, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_four, 71, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_one, 99, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_two, 83, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_three, 76, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_left_four, 62, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_one, 119, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_two, 92, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_three, 85, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_four, 71, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_one, 110, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_two, 83, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_three, 76, dan_enter_pipe_wait);
        evtpatch::hookEvtReplace(spm::evt_door::evt_door_dan_dokan_right_four, 62, dan_enter_pipe_wait);

        // Quickstart
        evtpatch::hookEvt(spm::aa1_01::aa1_01_mario_house_transition_evt, 10, determine_quickstart);

        // Custom NPCs
        evtpatch::hookEvtReplaceBlock(spm::dan::dan_chest_room_init_evt, 65, merluna_setup, 83);
        evtpatch::hookEvtReplace(spm::map_data::mapDataPtr("mac_05")->initScript, 79, patch_mac_05);
        writeWord(&spm::evt_shop::evt_shop_build_flimm_pit_item_tables, 0xB8, 0x2C000001); // Patch Pit Flimm's double pricing in the Flopside Pit

        // Blessings/Curses
        evtpatch::hookEvt(commonDeathScript, 4, spectre_logic);
        evtpatch::hookEvt(spm::evt_door::door_init_evt, 1, run_global_operations);
    }

    void main()
    {
        wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
        titleScreenCustomTextPatch();
        messagePatch();
        danOverwrite();
        danYouSuck();
        danDontFuckingCrash();
        evtNpcPatchAllEnemies();
        danPatchPitEnemies();
        evtPatches();
        patchMarioDamage();
        dimenPatch();
    }
}