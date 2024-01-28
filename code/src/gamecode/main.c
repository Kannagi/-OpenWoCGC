#include "../nu.h"
#include "gamecode/main.h"
/*
  8004f584 0000bc 8004f584  4 InitTexAnimScripts 	Global
  8004f640 000168 8004f640  4 SetTexAnimSignals 	Global
  8004f7a8 000054 8004f7a8  4 LoadFont3D 	Global
  8004f7fc 00005c 8004f7fc  4 InitCreatureModels 	Global
  8004f858 000050 8004f858  4 InitCreatures 	Global
  8004f8a8 000010 8004f8a8  4 IsTitleScreen 	Global
  8004f8b8 00005c 8004f8b8  4 CreateFadeMtl 	Global
  8004f914 00009c 8004f914  4 UpdateFade 	Global
  8004f9b0 00005c 8004f9b0  4 DrawFade 	Global
  8004fa0c 0000e4 8004fa0c  4 InitParticleSystem 	Global
  8004faf0 0006a4 8004faf0  4 InitWorld 	Global
  80050194 0000d0 80050194  4 ResetSuperBuffer 	Global
  80050264 000128 80050264  4 ResetSuperBuffer2 	Global
  8005038c 000124 8005038c  4 MAHLoadingMessage 	Global
  800504b0 000240 800504b0  4 LoadLevel 	Global
  800506f0 0000a0 800506f0  4 DrawWorld 	Global
  80050790 000038 80050790  4 PauseRumble 	Global
  800507c8 000064 800507c8  4 PauseGame 	Global
  8005082c 000020 8005082c  4 ResumeGame 	Global
  8005084c 00042c 8005084c  4 DoInput 	Global
  80050c78 00006c 80050c78  4 TBCODESTART 	Global
  80050ce4 000040 80050ce4  4 TBCODEEND 	Global
  80050d24 000060 80050d24  4 TBDRAWSTART 	Global
  80050d84 000034 80050d84  4 TBDRAWEND 	Global
  80050db8 0000d8 80050db8  4 InitPauseRender 	Global
  80050e90 000048 80050e90  4 ClosePauseRender 	Global
  80050ed8 000158 80050ed8  4 HandlePauseRender 	Global
  80051030 0000ac 80051030  4 SetLevel 	Global
  800510dc 0000d8 800510dc  4 firstscreenfade 	Global
  800511b4 000084 800511b4  4 CopyFilesThreadProc 	Global
  80051238 000104 80051238  4 LoadGBABG 	Global
  8005133c 000068 8005133c  4 UnLoadGBABG 	Global
  800513a4 0000fc 800513a4  4 Reseter 	Global
  800514a0 000058 800514a0  4 Managememcard 	Global
  800514f8 0004d0 800514f8  4 firstscreens 	Global
  800519c8 001b04 800519c8  4 main 	Global
*/

int main()
{
}

union variptr_u superbuffer_base;
union variptr_u superbuffer_end;
s32 superbuffersize;
union variptr_u superbuffer_reset_base;
struct nugscn_s* font3d_scene;
struct nuscene_s* font3d_scene2;
static struct txanmlist texanimscripts[24];
char texanimbuff[32768];
u32 texanimbits;
static struct numtl_s* fade_mtl;
int fade_rate;
int fadeval;
int fadehack;
static int fadecol;
s32 SHEIGHT;
s32 SWIDTH;
s32 IsLoadingScreen;
s32 FRAME;
char tbtxt[16][16];
s32 i_tb_code;
s32 app_tbset;
s32 i_tb_draw;
struct numtl_s* pause_src_mtl;
struct numtl_s* pause_rndr_mtl;
s32 PHYSICAL_SCREEN_X;
s32 PHYSICAL_SCREEN_Y;
float WeatherBossSkeletonTimer; //vehicle.c
s32 WeatherBossDead; //vehicle.c
s32 GAMEOBJECTCOUNT; //game_obj.c
char* tLOADING[6]; //text.c



//NGC MATCH
void ResetSuperBuffer(void)
{
  if (superbuffersize != (double)0x4156eb851eb851ec) {
    if (superbuffer_base.voidptr != NULL) {
      NuMemFree(superbuffer_base.voidptr);
    }
    superbuffer_base.voidptr = NuMemAlloc(0x5bae14);

    superbuffersize = 0x5bae14;
    superbuffer_reset_base = superbuffer_base;
    superbuffer_end.voidptr = (superbuffer_base.voidptr + superbuffersize);
    if (superbuffer_base.voidptr == NULL) {
      NuErrorProlog("C:/source/crashwoc/code/gamecode/main.c",0x5c5)
                                      ("unable to allocate super buffer");
    }
  }
  superbuffer_ptr = superbuffer_reset_base;
  return;
}

/*
//78%
void ResetSuperBuffer2(void) {
    double dVar1;

    if (superbuffersize != (double)6008340.48) {
        if (superbuffer_base.voidptr != NULL) {
            NuMemFree(superbuffer_base.voidptr);
        }
        superbuffer_base.voidptr = NuMemAlloc(0x5bae14);
        superbuffersize = 0x5bae14;
        if ((double)0x4156eb851eb851ec <= superbuffersize + 6008340) {
            superbuffer_end.intaddr = (int)(0x4156eb851eb851ec);
        }
        else {
            superbuffer_end.intaddr = (uint)dVar1;
        }
        superbuffer_reset_base.voidptr = superbuffer_base.voidptr;
        if (superbuffer_base.voidptr == NULL) {
            NuErrorProlog("C:/source/crashwoc/code/gamecode/main.c",0x5e2)
            ("unable to allocate super buffer");
        }
    }
    superbuffer_ptr = superbuffer_reset_base;
    return;
}

//debris.c (gamelib)
void DebrisMalloc(void) {
  if (debbuffer == NULL) {
    debbuffer = (char *)malloc_x(0x93400);
  }
  return;
}

*/

/*


int main(int argc,char **argv)

{
  bool bVar1;
  bool bVar2;
  float fVar3;
  short sVar4;
  uint uVar5;
  float fVar6;
  nucamera_s *pnVar7;
  void *paused;
  mask *mask;
  int iVar8;
  int iVar9;
  char **in_r5;
  nucamera_s *pnVar10;
  nucamera_s *pnVar11;
  char *pcVar12;
  CamMtx *pCVar13;
  creature_s *plr;
  void *pause;
  CamMtx *GameCam_;
  double dVar14;
  double dVar15;
  double dVar16;
  float LODWORD;
  nuvec_s pos;
  int v155;
  int local_9c [4];

  __main(argc,argv,in_r5);
  v155 = 0;
  DEMOInit((_GXRenderModeObj *)0x0);
  GS_Init();
  SS_Init();
  MC_Init();
  NuFileInitEx(0,0);
  Version();
  PadData.dwPacketNumber = (uint)GS_FrameBufferCopydataptr;
  DefaultGame();
  ResetGame();
  NuPs2Init();
  DebrisMalloc();
  _LODWORD = (double)CONCAT44(0x43300000,SWIDTH ^ 0x80000000);
  Level = -1;
  Hub = -1;
  MAXVPSIZEX = SWIDTH;
  MAXVPSIZEY = SHEIGHT;
  MINVPSIZEX = (int)(((float)(_LODWORD - (double)0x4330000080000000) * 390.0) / 640.0);
  _LODWORD = (double)CONCAT44(0x43300000,SHEIGHT ^ 0x80000000);
  MINVPSIZEY = (int)(((float)(_LODWORD - (double)0x4330000080000000) * 244.0) / 480.0);
  InitCutScenes();
  NuTrigInit();
  NuVpInit();
  NuTexInit();
  NuGobjInit();
  NuMtlInit();
  NuRndrInitEx(0x200000);
  NuLightInit();
  pNuCam = NuCameraCreate();
  firstscreens();
  Pad[0] = NuPs2OpenPad(0,0);
  Pad[1] = NuPs2OpenPad(1,0);
  app_tbset = tbsetCreate((int *)0x0);
  if (Level == -1) {
    Level = 0x23;
  }
  else {
    NewGame();
    CalculateGamePercentage(&Game);
    Hub = HubFromLevel(Level);
  }
  last_level = Level;
LAB_80051ba4:
  srand(0);
  qseed = 0x3039;
  NuRandSeed(0);
  if (Level == 0x23) {
    NewGame();
    Hub = -1;
    NewMenu(&Cursor,-1,-1,-1);
    cutmovie = 0;
    Level = 0x25;
    Demo = 0;
  }
  SetLevel();
  highallocaddr = 0;
  NuTexInit();
  NuMtlInit();
  NuGobjInit();
  InstInit();
  NuSoundKillAllAudio();
  while (cutmovie != -1) {
    pad_play = 0;
    InitPadPlayRecord(PadRecordPath,0,0x13ec,(void *)PadData.dwPacketNumber);
    PlayCutMovie(cutmovie);
    NewMenu(&Cursor,-1,-1,-1);
    cutmovie = -1;
    logos_played = 1;
    NuSoundKillAllAudio();
  }
  SetLevel();
  InitCameraTargetMaterial();
  DebrisSetup();
  VehicleSetup();
  DebrisRegisterCutoffCameraVec((nuvec_s *)&global_camera.mtx._30);
  CreateFadeMtl();
  ResetSuperBuffer();
  CrateMat = (numtl_s *)0x0;
  CrateMat2 = (numtl_s *)0x0;
  LoadLevel();
  InitPauseRender();
  NuLgtSetArcMat(DebMat[5],0.5019531,0.7480469,0.5605469,0.9980469);
  Reset3DFontObjects();
  InitPlayerEvents();
  if (Level == 0x28) {
    GameMode = 1;
  }
  else if (Level == 0x26) {
    ResetTempCharacter(2,0x22);
    tempanim_nextaction = 0x73;
    gamesfx_channel = 4;
    GameSfx(0xb2,(nuvec_s *)0x0);
    tempanim_waitaudio = 1;
    cortex_gameover_i = -1;
  }
  else {
    if (((Level == 0x25) && ((Game.cutbits & 1) == 0)) && (force_menu == -1)) {
      iVar9 = 0;
    }
    else {
      if (just_continued == 0) {
LAB_80051e34:
        GameMode = 0;
        goto LAB_80051e38;
      }
      if (((Level == 0x25) && ((Game.level[21].flags & 0x800) != 0)) && ((Game.cutbits & 0x40) ==  0)
         ) {
        iVar9 = 6;
      }
      else {
        if (just_continued == 0) goto LAB_80051e34;
        if (((Level == 0x25) && ((Game.level[23].flags & 0x800) != 0)) &&
           ((Game.cutbits & 0x800) == 0)) {
          iVar9 = 0xb;
        }
        else {
          if ((((just_continued == 0) || (Level != 0x25)) || ((Game.level[22].flags & 0x800) == 0) )
             || ((Game.cutbits & 0x10000) != 0)) goto LAB_80051e34;
          iVar9 = 0x10;
        }
      }
    }
    NewCut(iVar9);
    GameMode = 1;
  }
LAB_80051e38:
  if (GameMode == 1) {
    if (Level == 0x25) {
      Hub = -1;
    }
    ResetTimer(&CutTimer);
  }
  just_continued = 0;
  if ((ForcePlayRecord == 0) && (pad_play = 0, Demo != 0)) {
    pad_play = (int)(pad_record == 0);
  }
  if (pad_record == 0) {
    if (pad_play == 0) {
      InitPadPlayRecord(PadRecordPath,0,0x13ec,(void *)PadData.dwPacketNumber);
      goto LAB_80051ef4;
    }
    iVar9 = 2;
  }
  else {
    iVar9 = 1;
  }
  InitPadPlayRecord(PadRecordPath,iVar9,0x13ec,(void *)PadData.dwPacketNumber);
LAB_80051ef4:
  ResetBonus();
  ResetDeath();
  ResetGemPath();
  game_music = -1;
  LostLife = 0;
  if ((Cursor.menu != '\x13') && (GameMode != 1)) {
    GameMusic((int)LDATA->music[0],0);
  }
  USELIGHTS = 1;
  musicvol_mul = 0.0;
  srand(0);
  qseed = 0x3039;
  NuRandSeed(0);
  do {
    NuSoundSetLevelAmbience();
    NuSoundUpdate();
    if (((Cursor.menu != '\x13') && (game_music != LDATA->music[0])) && (GameMode != 1)) {
      GameMusic((int)LDATA->music[0],0);
    }
    InitCrateExplosions();
    if (Bonus != 4) {
      ResetBonus();
    }
    ResetDeath();
    ResetTimeTrial();
    ResetCrates();
    ResetGemPath();
    ResetWumpa();
    NuSoundUpdate();
    ResetChases();
    ResetPlayerEvents();
    ResetGates();
    ResetRings();
    iVar9 = ResetMaskFeathers();
    ResetAI();
    NuSoundUpdate();
    InitGameMode(iVar9);
    ResetBug();
    ResetLevel();
    ResetVehicleControl((int)(player->obj).RPos.iRAIL,(int)(player->obj).RPos.iALONG,
                        (player->obj).RPos.fALONG);
    InitDeb3();
    NuSoundUpdate();
    ResetProjectiles();
    pause_dir = 0;
    Paused = 0;
    ResetPanelDebris();
    if (Pad[0] != (nupad_s *)0x0) {
      NuPs2PadSetMotors(Pad[0],0,0);
    }
    ResetGameSfx();
    local_9c[1] = 0;
    local_9c[2] = 0;
    vtog_time = 0.0;
    vtog_duration = 0.0;
    ResetGameCameras(&GameCam,1);
    in_finish_range = 0;
    ResetAwards();
    NuSoundUpdate();
    fade_rate = -8;
    frameout_count = nuvideo_global_vbcnt;
    frameout = local_9c[1];
    NuInitFrameAdvance();
    dVar16 = 1.0;
    dVar14 = 0.5;
    while (((new_mode == -1 && (new_level == -1)) || ((fadeval < 0xff || (fadehack != 0))))) {
      DBTimerStart(1);
      tbslotBegin(app_tbset,0);
      if ((LDATA->fognear == LDATA->fogfar) || (pause_rndr_on != 0)) {
        level_clearcolour = 0;
        level_fogcolour = 0;
      }
      else {
        level_clearcolour =
             (uint)LDATA->foga << 0x18 | (uint)LDATA->fogr << 0x10 | (uint)LDATA->fogg << 8 |
             (uint)LDATA->fogb;
        level_fogcolour =
             (uint)LDATA->foga << 0x18 | (uint)LDATA->fogb << 0x10 | (uint)LDATA->fogg << 8 |
             (uint)LDATA->fogr;
      }
      bVar2 = 0x1e < v155;
      v155 = v155 + 1;
      if (bVar2) {
        v155 = 0;
        NuSoundSetLevelAmbience();
      }
      NuGetFrameAdvance();
      plr = player;
      FRAMES = 1;
      _LODWORD = (double)CONCAT44(0x43300000,(int)cufps ^ 0x80000000);
      fVar3 = (float)(_LODWORD - (double)0x4330000080000000);
      if (fVar3 < 59.0) {
        FRAMES = 2;
      }
      if (fVar3 < 29.0) {
        FRAMES = FRAMES + 1;
      }
      if (fVar3 < 19.0) {
        FRAMES = FRAMES + 1;
      }
      if (fVar3 < 14.0) {
        FRAMES = FRAMES + 1;
      }
      if (FRAMES == 0) {
        FRAMES = 1;
      }
      FRAME = 0;
      bVar2 = local_9c[2] != 0;
      if (FRAMES != 0) {
        dVar15 = 5.0;
        do {
          if (FRAME == 0) {
            tbslotBegin(app_tbset,1);
          }
          ProcessGlass(Paused);
          if ((((((GameMode != 1) && (0xb3 < GameTimer.frame)) && (GlobalTimer.frame % 0x3c == 0x1 e)
                ) && ((fadeval == 0 && (iVar9 = NuSoundKeyStatus(0), iVar9 != 1)))) && (Bonus != 1) )
             && (Bonus != 3)) {
            GameMusic(game_music,0);
          }
          if (mg_wumpatot != 0) {
            mg_wumpatot = mg_wumpatot + -1;
            plr_wumpas.count = plr_wumpas.count + 1;
          }
          if (((FRAME == 0) || (FRAMES == 1)) || ((Demo != 0 || (Level == 0x26)))) {
            DoInput();
          }
          if ((Demo != 0) &&
             (((pad_play != 0 && (iVar9 = NuPs2PadDemoEnd(), iVar9 != 0)) ||
              (20.0 <= GameTimer.ftime)))) {
            new_level = 0x23;
          }
          UpdateGameSfx();
          NuXboxSoundUpdate();
          DBTimerStart(2);
          if (Paused == 0) {
            crate_wumpa = Paused;
            mask_crates = Paused;
            RotateDirectionalLight(ldir,-0x2000,((GameTimer.frame % 0x1e0) * 0x10000) / 0x1e0);
            iVar9 = 9;
            pcVar12 = &Character[8].anim_processed;
            do {
              *pcVar12 = '\0';
              pcVar12 = pcVar12 + -0xce4;
              iVar9 = iVar9 + -1;
            } while (iVar9 != 0);
            SetLevelLights();
            SetTexAnimSignals();
            TBCODESTART(0,"Plats");
            if ((LDATA->flags & 8) != 0) {
              TerrainPlatformOldUpdate();
            }
            if (world_scene[0] != (nugscn_s *)0x0) {
              NuGScnUpdate(1.0,world_scene[0]);
            }
            edobjUpdateObjects(1.0);
            edanimUpdateObjects(1.0);
            NuRndrWaterRippleUpdate(1);
            NuBridgeUpdate(&Character[0].obj.pos);
            NuWindUpdate(&Character[0].obj.pos);
            if ((LDATA->flags & 8) != 0) {
              TerrainPlatformNewUpdate();
            }
            TBCODEEND(0);
            TBCODESTART(1,"Chars");
            if ((LDATA->flags & 1) != 0) {
              if (FRAME == 0) {
                tbslotBegin(app_tbset,5);
              }
              ManageCreatures();
              ProcessCreatures();
              if (FRAME == 0) {
                tbslotEnd(app_tbset,5);
              }
            }
            TBCODEEND(1);
            TBCODESTART(2,"Update");
            if (Cursor.menu == '\x13') {
              UpdateCutMovie();
            }
            if (level_part_2 == 0) {
              UpdateLevel();
              UpdateKabooms();
              UpdateWumpa();
              UpdateCrates();
              UpdateMaskFeathers();
              UpdateCrateExplosions();
              UpdateChases();
              UpdateProjectiles();
              UpdateBugLight(plr);
              UpdateGameCut();
            }
            ProcessVehicleLevel(Pad[0]);
            TBCODESTART(3,"Deb");
            ProcDeb3();
            if ((NODEBRIS == 0) &&
               ((Debris(0), GLASSPLAYER == 0 || (dVar15 <= (double)plr_invisibility_time)))) {
              pos.x = (player->obj).pos.x;
              pos.y = (float)((double)(((player->obj).bot + (player->obj).top) * (player->obj).SCA LE
                                      ) * dVar14 + (double)(player->obj).pos.y);
              pos.z = (player->obj).pos.z;
              iVar9 = DebrisCollisionCheck(&pos,(player->obj).RADIUS);
              if (iVar9 != -1) {
                KillPlayer(&player->obj,0x14);
              }
            }
            NuMtlAnimate(0.01666667);
            TBCODEEND(3);
            GameCam_ = (CamMtx *)UpdateTempCharacter();
            UpdateAwards();
            MoveGameCamera(GameCam_,&plr->obj);
            UpdateScreenWumpas();
            if (PLAYERCOUNT != 0) {
              UpdatePlayerStats(plr);
            }
            UpdatePanelDebris();
          }
          else {
            PauseRumble();
            iVar9 = 0x30;
            pnVar11 = pNuCam;
            GameCam_ = &GameCam;
            do {
              pCVar13 = GameCam_;
              pnVar10 = pnVar11;
              iVar9 = iVar9 + -0x18;
              (pnVar10->mtx)._00 = (pCVar13->m)._00;
              (pnVar10->mtx)._01 = (pCVar13->m)._01;
              (pnVar10->mtx)._02 = (pCVar13->m)._02;
              (pnVar10->mtx)._03 = (pCVar13->m)._03;
              (pnVar10->mtx)._10 = (pCVar13->m)._10;
              GameCam_ = (CamMtx *)&(pCVar13->m)._12;
              (pnVar10->mtx)._11 = (pCVar13->m)._11;
              pnVar7 = pNuCam;
              pnVar11 = (nucamera_s *)&(pnVar10->mtx)._12;
            } while (iVar9 != 0);
            *(float *)pnVar11 = *(float *)GameCam_;
            (pnVar10->mtx)._13 = (pCVar13->m)._13;
            (pnVar10->mtx)._20 = (pCVar13->m)._20;
            (pnVar10->mtx)._21 = (pCVar13->m)._21;
            NuCameraSet(pnVar7);
            if ((Pad[0] != (nupad_s *)0x0) && ((Pad[0]->old_paddata & 0xa0) != 0)) {
              pausestats_frame = (pausestats_frame / 0xb4) * 0xb4 + 0xb4;
              GameSfx(0x36,(nuvec_s *)0x0);
            }
          }
          if ((FRAME == FRAMES + -1) && (pause_rndr_on == 0)) {
            AddBugLight();
          }
          GameTiming();
          if (FRAME == 0) {
            ProcMenu(&Cursor,Pad[0]);
          }
          UpdateFade();
          if ((Level == 2) && (VEHICLECONTROL != 1)) {
            NuLightFog(0.0,0.0,0x40000000,BLUR,0);
          }
          else {
            NuLightFog(LDATA->fognear,LDATA->fogfar,level_fogcolour,BLUR,
                       (uint)LDATA->hazea << 0x18 | (uint)LDATA->hazeb << 0x10 |
                       (uint)LDATA->hazeg << 8 | (uint)LDATA->hazer);
          }
          if (FRAME == 0) {
            tbslotEnd(app_tbset,1);
          }
          DBTimerEnd(2);
          DBTimerStart(3);
          if ((((Level == 2) || (Level == 6)) || (Level == 0x18)) || (Level == 0x22)) {
            GS_SetAlphaCompareForce(1);
          }
          else {
            GS_SetAlphaCompareForce(0);
          }
          if (pause_rndr_on == 0) {
            if (FRAME == FRAMES + -1) {
              tbslotBegin(app_tbset,9);
              GS_Parallax = 1;
              pCam = &GameCam;
              iVar9 = NuRndrBeginScene(1);
              if (iVar9 != 0) {
                GameVP();
                NuRndrClear(0xf,level_clearcolour,1.0);
                if (level_part_2 == 0) {
                  DrawParallax();
                }
              }
              NuRndrEndScene();
              GS_Parallax = 0;
              NuRndrClear(10,0,1.0);
              local_9c[0] = NuRndrBeginScene(1);
              pnVar11 = pNuCam;
              if (Level == 0x18) {
                (pNuCam->mtx)._32 = (float)PadData.field1_0x4;
                NuCameraSet(pnVar11);
                DrawWorld();
                NuRndrEndScene();
                iVar9 = 0x30;
                GameCam_ = pCam;
                pnVar11 = pNuCam;
                do {
                  pnVar10 = pnVar11;
                  pCVar13 = GameCam_;
                  iVar9 = iVar9 + -0x18;
                  (pnVar10->mtx)._00 = (pCVar13->m)._00;
                  (pnVar10->mtx)._01 = (pCVar13->m)._01;
                  (pnVar10->mtx)._02 = (pCVar13->m)._02;
                  (pnVar10->mtx)._03 = (pCVar13->m)._03;
                  (pnVar10->mtx)._10 = (pCVar13->m)._10;
                  GameCam_ = (CamMtx *)&(pCVar13->m)._12;
                  (pnVar10->mtx)._11 = (pCVar13->m)._11;
                  pnVar7 = pNuCam;
                  pnVar11 = (nucamera_s *)&(pnVar10->mtx)._12;
                } while (iVar9 != 0);
                *(float *)pnVar11 = *(float *)GameCam_;
                (pnVar10->mtx)._13 = (pCVar13->m)._13;
                (pnVar10->mtx)._20 = (pCVar13->m)._20;
                (pnVar10->mtx)._21 = (pCVar13->m)._21;
                NuCameraSet(pnVar7);
                local_9c[0] = NuRndrBeginScene(1);
              }
              if (FRAME == FRAMES + -1) {
                TBDRAWSTART(0,"Chars");
              }
            }
            bVar1 = FRAME == FRAMES + -1;
            local_9c[1] = (int)((plr->obj).dead == '\x02');
            uVar5 = ((uint)(byte)(bVar1 << 1) << 0x1c) >> 0x1d;
            if (bVar1) {
              tbslotBegin(app_tbset,0xc);
            }
            if ((((LDATA->flags & 1) == 0) || (DRAWCREATUREHACK == 0)) || (cut_on != 0)) {
LAB_80052b3c:
              if (FRAME == FRAMES + -1) {
                tbslotEnd(app_tbset,0xc);
              }
            }
            else {
              if (FRAME == FRAMES + -1) {
                TBDRAWSTART(1,"Crash");
              }
              if ((local_9c[1] == 0) &&
                 (((GLASSPLAYER == 0 || (dVar15 <= (double)plr_invisibility_time)) ||
                  (Level == 0x17)))) {
                pCam = &GameCam;
                DrawCreatures(Character,1,uVar5,1);
              }
              if (FRAME == FRAMES + -1) {
                TBDRAWEND(1);
              }
              if ((level_part_2 == 0) && (DrawCreatures(Character + 1,8,uVar5,1), Level == 0x1c))  {
                DrawCreatures(&OppTubCreature,1,uVar5,0);
              }
              DrawNextVehicle(uVar5);
              if (FRAME == FRAMES + -1) {
                DrawExtraCreatures();
                goto LAB_80052b3c;
              }
            }
            if (cut_on == 0) {
              DrawChases(uVar5);
              DrawTempCharacter(uVar5);
              DrawTempCharacter2(uVar5);
            }
          }
          FRAME = FRAME + 1;
        } while (FRAME < FRAMES);
      }
      pause = (void *)Paused;
      if (screendump != 0) {
        pause = PadData.field2_0x8;
      }
      if ((pause_rndr_on == 0) && (local_9c[0] != 0)) {
        if (pause == (void *)0x0) {
          fVar3 = 1.0;
        }
        else {
          fVar3 = 0.0;
        }
        NuWaterSpeed(fVar3);
        CurrentCausticTexture = (CurrentCausticTexture + 1) % 0x1f;
        TBDRAWSTART(2,"World");
        if ((LDATA->flags & 4) != 0) {
          tbslotBegin(app_tbset,10);
          if (world_vd == (visidata_s *)0x0) {
            ApplyVisiTable(world_scene[0],&pCam->pos);
          }
          else if (pVIS != (nugspline_s *)0x0) {
            visiSetSplineKnot(world_vd,((int)pVIS - (int)world_scene[0]->splines) * -0x55555555 >>  2
                              ,iVIS);
          }
          if ((DRAWWORLDHACK != 0) && (Level != 0x18)) {
            DrawWorld();
          }
          tbslotEnd(app_tbset,10);
        }
        TBDRAWEND(2);
        TBDRAWSTART(3,"Deb");
        if ((cut_on == 0) && (DrawVehicleTrail(), cut_on == 0)) {
          DrawDeb3();
          DrawProjectiles();
        }
        TBDRAWEND(3);
        if ((level_part_2 == 0) && (cut_on == 0)) {
          TBDRAWSTART(4,"Crates");
          DrawCrates();
          TBDRAWEND(4);
          TBDRAWSTART(5,"Wumpa");
          DrawWumpa();
          TBDRAWEND(5);
          TBDRAWSTART(6,"Misc");
          if ((LDATA->flags & 1) != 0) {
            if (((((plr->used != '\0') && (mask = (plr->obj).mask, mask != (mask *)0x0)) &&
                 (mask->active != 0)) && ((LDATA->flags & 0xe00) == 0)) &&
               (((VEHICLECONTROL != 1 || ((LBIT._0_4_ & 0x10 | LBIT._4_4_ & 0x5042000) == 0)) &&
                ((Cursor.menu != '$' && (advice_wait == 0)))))) {
              DrawMask(mask);
            }
            DrawMaskFeathers();
          }
          DrawKabooms();
          DrawTransporters();
          DrawAwards();
          DrawBug();
          DrawRings();
          DrawTarget();
          DrawLevel();
          DrawGameCut();
          if (Level == 0x1d) {
            DrawCarpet();
          }
          NuRndrShadPolys(ShadowMat);
          NuRndrWaterRip(DebMat[4]);
        }
        TBDRAWEND(6);
        if (Cursor.menu == '\x13') {
          DrawCutMovie();
        }
        if (cut_on == 0) {
          NuRndrFootPrints(DebMat[4],foot_u,foot_v);
        }
        tbslotEnd(app_tbset,9);
        NuWaterRender();
        NuRndrEndScene();
        paused = (void *)0x0;
        if (Cursor.menu != '\f') {
          paused = pause;
        }
        NuRndrFx((int)paused,&Character[0].obj.pos);
        tbslotBegin(app_tbset,0xe);
        NuRndrBeginScene(1);
        DrawCrateExplosions();
        NuGlassRenderStatic();
        NuRndrEndScene();
        force_glass_screencopy_enable = 1;
        tbslotEnd(app_tbset,0xe);
        tbslotBegin(app_tbset,2);
        NuRndrBeginScene(1);
        if ((((LBIT._4_4_ & 0x200000a1) != 0) && (cut_on == 0)) &&
           ((Level != 7 || ((sVar4 = (player->obj).RPos.iALONG, sVar4 < 0x67 || (0x91 < sVar4))))) )
        {
          if ((Level == 5) && ((sVar4 = (player->obj).RPos.iALONG, 0x6d < sVar4 && (sVar4 < 0x79)) ))
          {
            if (sVar4 == 0x6e) {
              PadData.field3_0xc = (void *)(float)(dVar16 - (double)(player->obj).RPos.fALONG);
              DoClouds(Paused);
            }
            if ((player->obj).RPos.iALONG == 0x78) {
              PadData.field3_0xc = (void *)(player->obj).RPos.fALONG;
              DoClouds(Paused);
            }
          }
          else {
            PadData.field3_0xc = (void *)(float)dVar16;
            DoClouds(Paused);
          }
        }
        if ((((LDATA->flags & 0x10) != 0) && (NODEBRIS == 0)) && (DRAWDEBRISHACK != 0)) {
          DebrisDraw((int)pause);
        }
        NuRndrEndScene();
        tbslotEnd(app_tbset,2);
      }
      GameVP();
      HandlePauseRender((int)pause);
      TBDRAWSTART(7,"Panel");
      iVar9 = NuRndrBeginScene(1);
      if (iVar9 != 0) {
        bVar1 = local_9c[1] == 0;
        if ((bVar1) || (PLAYERCOUNT == 0)) {
          if (Cursor.menu == '\"') {
            _LODWORD = (double)CONCAT44(0x43300000,SHEIGHT << 3 ^ 0x80000000);
            iVar9 = (int)((float)(dVar16 - (double)(float)((double)(float)((double)(POWERTEXTY + 0 .7
                                                                                   ) + dVar16) *
                                                          dVar14)) *
                         (float)(_LODWORD - (double)0x4330000080000000));
            NuRndrRect2di(0,iVar9,SWIDTH << 4,
                          (int)((float)(dVar16 - (double)(float)((double)(float)((double)(POWERTEX TY
                                                                                         - 0.75) +
                                                                                dVar16) * dVar14))  *
                               (float)(_LODWORD - (double)0x4330000080000000)) - iVar9,0x18777777,
                          fade_mtl);
          }
        }
        else {
          fVar3 = (plr->obj).die_duration;
          fVar6 = (plr->obj).die_time * 3.0;
          if (fVar3 < fVar6) {
            fVar6 = fVar3;
          }
          uVar5 = (uint)((fVar6 / fVar3) * 255.0);
          NuRndrRect2di(0,0,SWIDTH << 4,SHEIGHT << 3,uVar5 | uVar5 << 8 | uVar5 << 0x10 | 0x800000 00
                        ,fade_mtl);
        }
        NuRndrClear(10,0,1.0);
        if (!bVar1) {
          DrawCreatures(Character,1,1,0);
        }
        if ((((((LDATA->flags & 1) != 0) && (plr->used != '\0')) &&
             ((mask = (plr->obj).mask, mask != (mask *)0x0 &&
              ((mask->active != 0 && ((LDATA->flags & 0xe00) == 0)))))) &&
            ((VEHICLECONTROL != 1 || ((LBIT._0_4_ & 0x10 | LBIT._4_4_ & 0x5042000) == 0)))) &&
           ((Cursor.menu == '$' || (advice_wait != 0)))) {
          DrawMask(mask);
        }
        DrawPanel();
        if ((new_mode != -1) || (new_level != -1)) {
          fade_rate = 8;
        }
        NuRndrEndScene();
      }
      iVar9 = NuRndrBeginScene(1);
      if (iVar9 != 0) {
        DrawFade();
        NuRndrEndScene();
      }
      TBDRAWEND(7);
      tbslotEnd(app_tbset,0);
      iVar9 = nuvideo_global_vbcnt - frameout_count;
      frameout_count = nuvideo_global_vbcnt;
      frameout = iVar9 + -1;
      if (frameout < 0) {
        frameout = 0;
      }
      if (bVar2) {
        frameout = 0;
        local_9c[2] = local_9c[2] + -1;
      }
      if (pause_rndr_on != 0) {
        local_9c[2] = 2;
      }
      DBTimerEnd(3);
      DBTimerEnd(1);
      NuRndrSwapScreen(1);
      NuDynamicWaterUpdate(0);
      Reseter();
      GC_DiskErrorPoll();
    }
    pause_dir = 0;
    Paused = 0;
    if (((plr_died != 0) && (best_cRPos != (RPos_s *)0x0)) && (Rail[best_cRPos->iRAIL].type == '\0 ')
       ) {
      LostLife = 1;
    }
    plr_died = 0;
    iVar9 = 2;
    do {
      iVar8 = NuRndrBeginScene(1);
      if (iVar8 != 0) {
        NuRndrClear(0xb,0,1.0);
        NuRndrEndScene();
      }
      NuRndrSwapScreen(1);
      iVar8 = new_mode;
      iVar9 = iVar9 + -1;
    } while (0 < iVar9);
    if (new_mode == -1) break;
    PauseGameAudio(0);
    NuSoundStopStream(4);
    NuSoundUpdate();
    GameMode = new_mode;
  } while( true );
  NuSoundKillAllAudio();
  NuSoundUpdate();
  ClosePauseRender();
  if (Pad[0] != (nupad_s *)0x0) {
    NuPs2PadSetMotors(Pad[0],0,0);
  }
  if (Pad[1] != (nupad_s *)0x0) {
    NuPs2PadSetMotors(Pad[1],0,0);
  }
  ReleaseVisiTable();
  CloseCutMovie(0);
  CloseCreatures();
  CloseCrates();
  InstClose();
  CloseClouds();
  ResetBug();
  NuSceneDestroy(font3d_scene2);
  font3d_scene = (nugscn_s *)0x0;
  font3d_scene2 = (nuscene_s *)0x0;
  font3d_initialised = 0;
  CloseXboxEffectSystem();
  if (PLAYERCOUNT == 0) {
    last_character = iVar8;
  }
  else {
    last_character = (int)(plr->obj).character;
  }
  if (last_character != -1) {
    last_used_character = last_character;
  }
  last_level = Level;
  last_hub = HubFromLevel(Level);
  GetSpaceCut(last_level,new_level);
  Level = new_level;
  number_of_times_played = number_of_times_played + 1;
  goto LAB_80051ba4;
}
*/


void InitTexAnimScripts(void)	//PS2
{
    struct nutexanimprog_s *prog;
    struct nudathdr_s* dfanim;
    struct txanmlist *list;
    union variptr_u ptr;

    list = texanimscripts;
    NuTexAnimProgSysInit();
    //dfanim = NuDatOpen("ats.dat", 0, 0);
    NuDatSet(0);  //NuDatSet(dfanim);
    ptr.voidptr = texanimbuff;
    memset(texanimbuff, 0, sizeof(texanimbuff));

    while( list->path != NULL ) {
        if ((list->levbits & LBIT) != 0){
            prog = NuTexAnimProgReadScript(&ptr, list->path);
            if (prog == NULL) {
                NuDebugMsgProlog(".\\main.c", 0x4ed, "", list->path);
            }
        }
        list = list + 1;
    }

    NuDatSet(0);
    /*if (dfanim != 0) {
        NuDatClose(dfanim);
    }*/
    return;
}


//NGC MATCH
void SetTexAnimSignals(void) {

    if ((player->used != '\0') && (player->obj.dead == '\x03' || (player->obj.dead == '\b') || (player->freeze != '\0'))) {
        texanimbits = texanimbits | 2;
    }
    else {
        texanimbits = texanimbits & 0xfffffffd;
    }
    if (((player->obj).mask != NULL) && ((player->obj).mask->active > 1)) {
        texanimbits = texanimbits | 4;
    }
    else {
        texanimbits = texanimbits & 0xfffffffb;
    }
    if (((player->used != '\0') && ((player->obj).dead != '\0')) && ((player->obj).dead != '\x12'))  {
        texanimbits = texanimbits | 0x20;
    }
    else {
        texanimbits = texanimbits & 0xffffffdf;
    }
    if (Level == 0x17) {
        texanimbits = texanimbits | 0x40;
    }
    else {
        texanimbits = texanimbits & 0xffffffbf;
    }
    if (Level == 0x18) {
        if (WeatherBossSkeletonTimer != 0.0f) {
            texanimbits = texanimbits | 0x100;
        }
        else {
            texanimbits = texanimbits & 0xfffffeff;
        }
        if (WeatherBossDead != 0) {
            texanimbits = texanimbits | 0x200;
        }
        else {
            texanimbits = texanimbits & 0xfffffdff;
        }
    }
    NuTexAnimSetSignals(texanimbits);
    NuTexAnimProcess();
    return;
}

//NGC MATCH
void LoadFont3D() {

  font3d_initialised = 0;
  font3d_scene2 = NuSceneReader2(&superbuffer_ptr,&superbuffer_end,"stuff\\font.nus");
  if (font3d_scene2 != NULL) {
    font3d_scene = font3d_scene2->gscene;
    InitFont3D(font3d_scene);
  }
  return;
}

//NGC MATCH
void InitCreatureModels(void) {
  s32 i;

  for (i = 0; i < 9; i++) {
    Character[i].used = '\0';
    Character[i].on = '\0';
    Character[i].off_wait = '\0';
  }
  GAMEOBJECTCOUNT = 0;
  InitSkinning(0);
  LoadCharacterModels();
  CloseSkinning();
  return;
}

//NGC MATCH
void InitCreatures(void) {
  s32 i;

  InitAI();
  InitChases();
  ResetChases();
  i = LDATA->flags & 1;
  if ((LDATA->flags & 1) != 0) {
    AddCreature((s32)LDATA->character,0,-1);
    i = 1;
  }
  PLAYERCOUNT = i;
  return;
}

s32 cutmovie; //cut.c

//NGC MATCH
s32 IsTitleScreen(void) {
  return (cutmovie == 0);
}

//NGC MATCH
void CreateFadeMtl() {
  struct numtl_s *mtl;

  mtl = NuMtlCreate(1);
  fade_mtl = mtl;
  fade_mtl->attrib.zmode = 3;
  fade_mtl->attrib.filter = 1;
  fade_mtl->attrib.lighting = 2;
  fade_mtl->attrib.utc = 1;
  fade_mtl->attrib.vtc = 1;
  fade_mtl->attrib.colour = 1;
  fade_mtl->attrib.alpha = 3;
  NuMtlUpdate(mtl);
  return;
}

//NGC MATCH
void UpdateFade(void) {
  s32 old;

  old = fadeval;
  fadeval = fadeval + fade_rate;
  if (fadeval > 0xff) {
    fadeval = 0xff;
  }
  else if (fadeval < 0) {
      fadeval = 0;
  }
  if ((old < 0xff) && (fadeval == 0xff)) {
    fadehack = 1;
  }
  else if (fadehack != 0) {
    fadehack = fadehack + -1;
  }
  if ((fadeval == 0) || (fadeval == 0xff)) {
    fade_rate = 0;
  }
  fadecol = fadeval | fadeval << 8 | fadeval << 0x10 | 0xff000000;
  return;
}

//NGC MATCH
void DrawFade(void) {
  if ((Cursor.menu != '\x02') && (fadeval != 0)) {
    NuRndrRect2di(0,0,SWIDTH << 4,SHEIGHT << 3,fadecol,fade_mtl);
  }
  return;
}

//NGC MATCH
void InitParticleSystem(void) {
  edppDestroyAllParticles();
  edppDestroyAllEffects();
  if ((LDATA->flags & 0x10) != 0) {
    sprintf(tbuf,"%s.ptl",LevelFileName);
    if (NuFileExists(tbuf) != 0) {
      edppLoadEffects(tbuf,'\x01');
    }
    sprintf(tbuf,"stuff\\general.ptl");
    if (NuFileExists(tbuf) != 0) {
      edppMergeEffects(tbuf,'\0');
    }
    sprintf(tbuf,"%s.cpt",LevelFileName);
    if (NuFileExists(tbuf) != 0) {
      edppMergeEffects(tbuf,'\x05');
    }
  }
  InitGameDebris();
  edppRestartAllEffectsInLevel();
  return;
}

/*
void InitWorld(void)

{
  nugscn_s *gsc;
  char *filename;
  nugscn_s **wrld;
  int size;

  LevelFileName._0_4_ = 0x6c657665;
  LevelFileName._4_4_ = 0x6c735c00;
  strcat(LevelFileName,LDATA->filepath);
  strcpy(PadRecordPath,LevelFileName);
  strcat(PadRecordPath,".pad");
  size = 0x20;
  wrld = world_scene + 0x1f;
  do {
    *wrld = (nugscn_s *)0x0;
    wrld = wrld + -1;
    size = size + -1;
  } while (size != 0);
  NuRndrInitWorld();
  if ((LDATA->flags & 4) != 0) {
    strcpy(tbuf,LevelFileName);
    strcat(tbuf,".nux");
    strcpy(tbuf,LevelFileName);
    strcat(tbuf,".nus");
    if ((Level != 0x27) && (Level != 0x29)) {
      size = NuFileSize(tbuf);
      if (size == -1) {
        world_scene[0] = (nugscn_s *)0x0;
        world_scene2[0] = (nuscene_s *)0x0;
      }
      else {
        world_scene2[0] = NuSceneReader(&superbuffer_ptr,&superbuffer_end,tbuf);
        if (world_scene2[0] != (nuscene_s *)0x0) {
          world_scene[0] = world_scene2[0]->gscene;
        }
      }
      strcpy(tbuf,LevelFileName);
      strcat(tbuf,".vis");
      world_vd = (visidata_s *)visiLoadData(tbuf,world_scene[0],&superbuffer_ptr);
      if ((world_scene[0] != (nugscn_s *)0x0) && (world_vd == (visidata_s *)0x0)) {
        BuildVisiTable(world_scene[0]);
      }
    }
    if (Level == 0xe) {
      world_scene2[1] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\a\\tsunami\\wave.nus");
      if (world_scene2[1] != (nuscene_s *)0x0) {
        world_scene[1] = world_scene2[1]->gscene;
      }
    }
    else if ((Level == 0x27) || (Level == 0x29)) {
      world_scene2[0] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\intro\\intro1.nus");
      if (world_scene2[0] != (nuscene_s *)0x0) {
        world_scene[0] = world_scene2[0]->gscene;
      }
      world_scene2[1] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\intro\\intro2.nus");
      if (world_scene2[1] != (nuscene_s *)0x0) {
        world_scene[1] = world_scene2[1]->gscene;
      }
      world_scene2[2] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\intro\\corridor.nus");
      if (world_scene2[2] != (nuscene_s *)0x0) {
        world_scene[2] = world_scene2[2]->gscene;
      }
      world_scene2[3] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\intro\\mystic.nus");
      if (world_scene2[3] != (nuscene_s *)0x0) {
        world_scene[3] = world_scene2[3]->gscene;
      }
      world_scene2[4] =
           NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\intro\\space.nus");
      if (world_scene2[4] != (nuscene_s *)0x0) {
        world_scene[4] = world_scene2[4]->gscene;
      }
    }
    if ((LBIT._4_4_ & 0x400000) == 0) {
      if ((LBIT._0_4_ & 5 | LBIT._4_4_ & 0xa031894d) == 0) goto LAB_8004fe3c;
      filename = "stuff\\jeepbits.nus";
    }
    else {
      filename = "stuff\\redbits.nus";
    }
    world_scene2[2] = NuSceneReader(&superbuffer_ptr,&superbuffer_end,filename);
    if (world_scene2[2] != (nuscene_s *)0x0) {
      world_scene[2] = world_scene2[2]->gscene;
    }
  }
LAB_8004fe3c:
  edobjRegisterBaseScene(world_scene[0]);
  edbitsRegisterBaseScene(world_scene[0]);
  NuBridgeRegisterBaseScene(world_scene[0]);
  edanimRegisterBaseScene(world_scene[0]);
  edbitsRegisterSfx(SfxTabGLOBAL,LDATA->pSFX,0xb1,SFXCOUNT_ALL);
  NuBridgeInit();
  NuWindInit();
  noterraininit();
  if ((LDATA->flags & 8) != 0) {
    TerrainSetCur(superbuffer_ptr.voidptr);
    terraininit(Level,&superbuffer_ptr.s16,superbuffer_end.s16,0,LevelFileName,world_scene[0],0);
    superbuffer_ptr.intaddr = (uint)((int)&(superbuffer_ptr.vec4)->w + 3) & 0xfffffff0;
  }
  crate_scene = (nugscn_s *)0x0;
  WUMPACOUNT = 0;
  if (((LDATA->flags & 0x40) != 0) &&
     (crate_scene2 = NuSceneReader(&superbuffer_ptr,&superbuffer_end,"stuff\\crates.nus"),
     crate_scene2 != (nuscene_s *)0x0)) {
    crate_scene = crate_scene2->gscene;
  }
  pause_scene = (nugscn_s *)0x0;
  if ((Demo == 0) &&
     ((((LDATA->flags & 1) != 0 || (Level == 0x23)) &&
      (pause_scene2 = NuSceneReader(&superbuffer_ptr,&superbuffer_end,"levels\\b\\pause\\pause.nus ")
      , pause_scene2 != (nuscene_s *)0x0)))) {
    pause_scene = pause_scene2->gscene;
  }
  edobjObjectReset();
  sprintf(tbuf,"%s.obj",LevelFileName);
  size = NuFileExists(tbuf);
  if (size != 0) {
    edobjFileLoadObjects(tbuf);
  }
  edanimParamReset();
  sprintf(tbuf,"%s.anm",LevelFileName);
  size = NuFileExists(tbuf);
  if (size != 0) {
    edanimFileLoad(tbuf);
  }
  edgraClumpsReset();
  sprintf(tbuf,"%s.gra",LevelFileName);
  size = NuFileExists(tbuf);
  if (size != 0) {
    edgraFileLoad(tbuf);
  }
  edgraInitAllClumps();
  NuDatSet((nudathdr_s *)0x0);
  LoadVehicleStuff();
  if (Level == 0x27) {
    LoadCutMovie(1);
    StartCutMovie();
  }
  else if (Level == 0x29) {
    LoadCutMovie(3);
    StartCutMovie();
  }
  TerrainPlatformOldUpdate();
  if (world_scene[0] != (nugscn_s *)0x0) {
    NuGScnUpdate(0.0,world_scene[0]);
  }
  edobjUpdateObjects(0.0);
  edanimUpdateObjects(0.0);
  NuBridgeUpdate(&Character[0].obj.pos);
  NuWindUpdate(&Character[0].obj.pos);
  TerrainPlatformNewUpdate();
  if (((LDATA->flags & 1) != 0) && (Level != 0x25)) {
    ShadowMat = CreateAlphaBlendTexture64("stuff\\gradient.raw",0,1,100);
    ShadowMatBodge = ShadowMat->tid;
  }
  ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
  InitSplineTable();
  InitObjectTable();
  InitRails();
  InitWumpa();
  ResetMaskFeathers();
  InitCrates();
  InitVehicleToggles();
  InitLevel();
  NuDatSet((nudathdr_s *)0x0);
  wrld = world_scene;
  do {
    gsc = *wrld;
    wrld = wrld + 1;
    if (gsc != (nugscn_s *)0x0) {
      InitXboxGSceneEffects(gsc,&superbuffer_ptr,&superbuffer_end);
    }
  } while ((int)wrld < -0x7fec337b);
  NuLightMatInit();
  return;
}
*/


//GCN MATCH
void MAHLoadingMessage(void)
{
  IsLoadingScreen = 1;
  NuRndrClear(0xb,0,1.0);
  if (NuRndrBeginScene(1) != 0) {
    memcpy(&pNuCam->mtx, &numtx_identity, sizeof (struct numtx_s));
    NuCameraSet(pNuCam);
    if ((font3d_scene != NULL) && (font3d_initialised != 0)) {
      NuShaderSetBypassShaders(1);
      DrawGameMessage(tLOADING[Game.language],0,0.0);
      NuShaderSetBypassShaders(0);
    }
    NuRndrEndScene();
  }
  NuRndrSwapScreen(1);
  IsLoadingScreen = 0;
  return;
}


/*
void LoadLevel(void)

{
  int Cdat;
  struct nucolour4_s local_18;

  loadcount = loadcount + 1;
  load_anim_data = NULL;
  hLoadScreenThread = NULL;
  InitXboxEffectSystem(Level);
  NuSoundKillAllAudio();
  XbWaitForAllBuffersToStop();
  NuStartLoadScreen(LDATA->totalsize);
  if (Level != 0x28) {
    LoadFont3D();
  }
  PurgeCharacterModels();
  crash_loaded = 0;
  if (Level != 0x28) {
    MAHLoadingMessage();
  }
  InitTexAnimScripts();
  InitCreatureModels();
  if ((LBIT._4_4_ & 0x200000a1) != 0) {
    InitClouds(&superbuffer_ptr,&superbuffer_end);
  }
  if ((LDATA->flags & 0x40) != 0) {
    if (CrateMat == NULL) {
      if (CrateMat2 == NULL) {
        CrateMat = CreateAlphaBlendTexture256128_32("stuff\\box.raw",0,1,0x100,0);
        CrateMat2 = CreateAlphaBlendTexture256128_32("stuff\\boxyellow.raw",0,1,0x100,0);
      }
    }
  }
  InitLocalSfx(LDATA->pSFX,(int)LDATA->nSFX);
  InitSpecular();
  wumpa_scene = NULL;
  if ((LDATA->flags & 0x100) != 0) {
    wumpa_scene2 = NuSceneReader(&superbuffer_ptr,&superbuffer_end,"stuff\\wumpa.nus");
    if (wumpa_scene2 != NULL) {
      wumpa_scene = wumpa_scene2->gscene;
    }
  }
  local_18.b = 0.25;
  local_18.a = 0.5;
  local_18.r = 0.625;
  local_18.g = 0.75;
  NuLigthSetPolyHazeMat(DebMat[4],&local_18.r,&local_18.b);
  ClearGameObjects();
  ResetCheckpoint(-1,-1,0.0,NULL);
  InitWorld();
  i_cratetypedata = 0;
  if (((LDATA->flags & 0x40) != 0) && (Cdat = ReadCrateData(), Cdat != 0)) {
    ReadInCrateData();
  }
  LoadLights();
  InitCreatures();
  ParticleReset();
  InitParticleSystem();
  NuStopLoadScreen(0);
  if (Level == 7) {
    YTOL = 0.01166667;
  }
  else {
    YTOL = 0.01;
  }
  return;
}
*/

//NGC MATCH
void DrawWorld(void) {
  if (level_part_2 != 0) {
    if (world_scene[1] != NULL) {
        NuGScnRndr3(world_scene[1]);
        NuBridgeDraw(world_scene[1],DebMat[6]);
        NuWindDraw(world_scene[1]);
        edobjRenderObjects(world_scene[1]);
      }
  }
  else if (world_scene[0] != NULL) {
      NuGScnRndr3(world_scene[0]);
      NuBridgeDraw(world_scene[0],DebMat[6]);
      NuWindDraw(world_scene[0]);
      edobjRenderObjects(world_scene[0]);
    }
  return;
}

//NGC MATCH
void PauseRumble(void) {
  if (Pad[0] != NULL) {
    NuPs2PadSetMotors(Pad[0],0,0);
  }
  return;
}

//NGC MATCH
void PauseGame(void) {
  pause_dir = 1;
  NewMenu(&Cursor,5,0,-1);
  ResetTimer(&PauseTimer);
  pausestats_frame = 0;
  PauseGameAudio(1);
  GameSfx(0x36,NULL);
  return;
}

void ResumeGame(void) {
  ResumeGameAudio();
  return;
}

/*

void DoInput(void)

{
  float fVar1;
  int iVar2;
  nupad_s *pad;
  void *local_10;
  void *pvStack_c;

  XbPollPeripherals();
  XbPollAllControllers(Cursor.menu != -1);
  pad = Pad[0];
  iVar2 = NuPs2ReadPad((nupad_s *)&pad);
  if (iVar2 == 0) {
    if ((((Paused == 0) && (Pad[0] != (nupad_s *)0x0)) && ((Pad[0]->paddata_db & 0x20000000) == 0) )
       && (((Demo == 0 && (Cursor.menu != '\0')) &&
           ((Cursor.menu != '#' && ((GameMode != 1 && (cutmovie == -1)))))))) {
      PauseGame();
    }
    Pad[0] = XbGetWorkingController();
  }
  else {
    lost_controller = 0;
    if ((pad->padhandle == (void *)0x0) &&
       ((PadRecInfo == (PadRecInfo *)0x0 || (PadRecInfo->padmode != 2)))) {
      if ((((((Paused == 0) || (pause_dir == 2)) &&
            ((Pad[0] != (nupad_s *)0x0 &&
             (((((Pad[0]->paddata_db & 0x20000000) == 0 && (Demo == 0)) && (Cursor.menu != '\0'))  &&
              ((Cursor.menu != '#' && (GameMode != 1)))))))) && (cutmovie == -1)) &&
          (((new_mode == -1 && (new_level == -1)) && ((fadeval == 0 && (Cursor.menu == -1)))))) &&
         ((PLAYERCOUNT == 0 ||
          (((player->obj).finished == '\0' &&
           (((Level != 0x25 || (tumble_duration <= tumble_time)) || (last_hub == -1)))))))) {
        XbPollAllControllers(1);
        Pad[0] = (nupad_s *)0x0;
        PauseGame();
      }
    }
    else {
      if ((pad_record != 0) && (((pad->old_paddata & 0x800) != 0 || ((player->obj).dead != '\0'))) )
      {
        pad_record = 0;
      }
      stick_bits = 0;
      fVar1 = (float)((double)CONCAT44(0x43300000,(uint)pad->buttons_hi) -
                     (double)0x4330000000000000) - 127.5;
      if (-85.0 <= fVar1) {
        if (85.0 < fVar1) {
          stick_bits = 0x2000;
        }
      }
      else {
        stick_bits = 0x8000;
      }
      pvStack_c = (void *)(uint)pad->buttons_lo;
      local_10 = (void *)0x43300000;
      fVar1 = (float)((double)CONCAT44(0x43300000,pvStack_c) - (double)0x4330000000000000) - 127.5 ;
      if (-85.0 <= fVar1) {
        if (85.0 < fVar1) {
          stick_bits = stick_bits | 0x4000;
        }
      }
      else {
        stick_bits = stick_bits | 0x1000;
      }
      stick_bits_db = stick_bits & ~stick_oldbits;
      stick_oldbits = stick_bits;
      if (((((pad->old_paddata & 0x800) != 0) && ((LDATA->flags & 1) != 0)) && (new_mode == -1)) & &
         (((new_level == -1 && (fadeval == 0)) && ((GameMode != 1 && (cutmovie == -1)))))) {
        if ((((Paused != 0) || (Cursor.menu != -1)) ||
            ((PLAYERCOUNT != 0 &&
             (((player->obj).finished != '\0' ||
              (((Level == 0x25 && (tumble_time < tumble_duration)) && (last_hub != -1)))))))) ||
           (((LBIT._4_4_ & 0x3e00000) != 0 && (boss_dead == 2)))) {
          if (Paused != 0x1e) {
            lost_controller = 0;
            stick_oldbits = stick_bits;
            return;
          }
          iVar2 = 0x3c;
          pause_dir = 2;
        }
        else {
          PauseGame();
          iVar2 = 0x36;
        }
        GameSfx(iVar2,(nuvec_s *)0x0);
      }
    }
  }
  return;
}
*/

//NGC MATCH
void TBCODESTART(s32 i,char *txt) {
  if (((FRAME == 0) && (i_tb_code == i)) && (strlen(txt) < 0x10)) {
    strcpy(tbtxt[8],txt);
    tbslotBegin(app_tbset,8);
  }
  return;
}

//NGC MATCH
void TBCODEEND(s32 i) {
  if ((FRAME == 0) && (i_tb_code == i)) {
    tbslotEnd(app_tbset,8);
  }
  return;
}

//NGC MATCH
void TBDRAWSTART(s32 i,char *txt) {
  if ((i_tb_draw == i) && (strlen(txt) < 0x10)) {
    strcpy(tbtxt[0xb],txt);
    tbslotBegin(app_tbset,0xb);
  }
  return;
}

//NGC MATCH
void TBDRAWEND(s32 i) {
  if (i_tb_draw == i) {
    tbslotEnd(app_tbset,0xb);
  }
  return;
}

/*
void InitPauseRender(void)

{
  numtl_s *mtl;
  double dVar1;

  mtl = NuMtlCreate(1);
  dVar1 = 1.0;
  pause_rndr_mtl = mtl;
  mtl->alpha_sort = 0x7fff;
  mtl->alpha = 1.0;
  mtl->tid = 0x270e;
  mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xc3cffff | 0x114e8000);
  NuMtlUpdate(mtl);
  mtl = NuMtlCreate(1);
  pause_src_mtl = mtl;
  mtl->alpha = (float)dVar1;
  mtl->tid = 0x270e;
  mtl->alpha_sort = 0x7ffe;
  mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xc3cffff | 0x114e8000);
  NuMtlUpdate(mtl);
  pause_rndr_on = 0;
  return;
}
*/

//NGC MATCH
void ClosePauseRender(void) {
  if (pause_src_mtl != NULL) {
    NuMtlDestroy(pause_src_mtl);
  }
  if (pause_rndr_mtl != NULL) {
    NuMtlDestroy(pause_rndr_mtl);
  }
  pause_src_mtl = NULL;
  pause_rndr_mtl = NULL;
  return;
}

//NGC MATCH
void HandlePauseRender(int pause_count) {
  static s32 old_pause_state;
  s32 vp_x;
  s32 vp_y;
  struct nuviewport_s *vp;

  vp_x = (s32)((float)pause_count * 27.0f / 30.0f);
  vp_y = (s32)((float)pause_count * 19.0f / 30.0f);
  if ((pause_rndr_on != 0) && (NuRndrBeginScene(1) != 0)) {
    vp = NuVpGetCurrentViewport();
    NuRndrClear(0xb,0,1.0f);
    NuRndrRectUV2di(vp_x,vp_y,vp->width - vp_x,vp->height - vp_y,0.0f,0.0f,1.0f,1.0f,-1,pause_rndr_mtl);
    NuRndrEndScene();
  }
  if (Paused != 0) {
    if (old_pause_state == 0) {
      NudxFw_MakeBackBufferCopy(1);
      GS_CopyFBToPause();
      pause_rndr_on = 1;
    }
    if (Paused != 0) {
        goto block_end;
    }
  }
  if (old_pause_state != 0) {
    pause_rndr_on = 0;
  }
block_end:
  old_pause_state = Paused;
  return;
}

//PS2 MATCH
void SetLevel(void) {
    LBIT = 1L << (u64)Level; //NGC //LBIT = __ashldi3(0,1);
  LDATA = &LData[Level];
  if (pNuCam != NULL) {
    if (LDATA->farclip > 9) {
      pNuCam->farclip = (float) LDATA->farclip;
    }
    else {
     pNuCam->farclip = 1000.0f;
    }
  }
  AIVISRANGE = 25.0f;
  return;
}

//NGC MATCH
void firstscreenfade(struct numtl_s *mat,s32 dir) {
    s32 s;
    s32 t;
    u32 colour;
    s32 col;

    if (dir > 0) {
        colour = 0;
        s = 0x10;
    }
    else {
        colour = 0xff;
        s = -0x10;
    }
    for (t = 0; t < 0xe; t++) {
        col = (0xFF << 24) | (colour << 16) | (colour << 8) | colour;
        colour = colour + s;
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,col,mat);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
    }
    return;
}

//NGC MATCH
s32 CopyFilesThreadProc() {
    s32 iVar1;
    char texBuf [128];
    u32 iStack_c;

    iVar1 = GetTickCount();
    InitLevelSfxTables();
    InitGlobalSfx();
    iStack_c = GetTickCount() - iVar1;
    sprintf(texBuf,"Filecopy took %.2f seconds", iStack_c / 1000.0f);
    return 0;
}

/*

void LoadGBABG(void)

{
  int iVar1;
  numtl_s *mtl;
  numtlattrib_s attr;
  nutex_s tex;

  GBABG_Ptr = malloc_x(0x2000c);
  NuFileLoadBuffer("gfx\\crash2gb.s3",GBABG_Ptr,0x2000c);
  tex.height = 0x200;
  tex.decal = 0;
  tex.bits = GBABG_Ptr;
  iss3cmp = 0x20000;
  tex.width = 0x200;
  tex.mmcnt = 1;
  tex.pal = (int *)0x0;
  tex.type = NUTEX_RGB24;
  GBABG_tid = NuTexCreate(&tex);
  iss3cmp = 0;
  mtl = NuMtlCreate(1);
  iVar1 = GBABG_tid;
  attr = mtl->attrib;
  GBABG_mtl = mtl;
  (mtl->diffuse).b = 1.0;
  mtl->alpha = 0.999;
  mtl->tid = iVar1;
  (mtl->diffuse).r = 1.0;
  (mtl->diffuse).g = 1.0;
  mtl->attrib = (numtlattrib_s)((uint)attr & 0xcc0cffff | 0x16e8000);
  NuMtlUpdate(mtl);
  return;
}

//NGC MATCH
void UnLoadGBABG(void) {
  if (GBABG_Ptr != NULL) {
    free_x(GBABG_Ptr);
  }
  GBABG_Ptr = NULL;
  if (GBABG_mtl != NULL) {
    if (GBABG_mtl->tid != 0) {
      NuTexDestroy(GBABG_mtl->tid);
    }
    NuMtlDestroy(GBABG_mtl);
    GBABG_mtl = NULL;
  }
  return;
}
*/

/*

//NGC MATCH
void Reseter(void) {
  if (OSGetResetButtonState() != 0) {
    reset_256 = 1;
  }
  else {
    if (reset_256 != 0) {
      OSReport("SS_StopAllSFX\n");
      SS_StopAllSFX();
      OSReport("SS_TrackStop\n");
      SS_TrackStop(-1);
      OSReport("GXAbortFrame\n");
      OSReport("GXDrawDone\n");
      GXDrawDone();
      OSReport("VISetBlack\n");
      VISetBlack(1);
      OSReport("VIFlush\n");
      VIFlush();
      OSReport("VIWaitForRetrace\n");
      VIWaitForRetrace();
      OSReport("OSResetSystem\n");
      OSResetSystem(0,1,0);
    }
  }
  return;
}

//NGC MATCH
void Managememcard(void) {
  SS_StopAllSFX();
  SS_TrackStop(-1);
  GXDrawDone();
  VISetBlack(1);
  VIFlush();
  VIWaitForRetrace();
  OSResetSystem(1,1,1);
  return;
}


*/

//95% NGC
void firstscreens(void) {
    char *gerbils;
    s32 firstscreen1_tid;
    struct numtl_s *firstscreen1_mtl;
    struct nutex_s tex;
    
    CopyFilesThreadProc(0);
    gerbils = malloc_x(0x4000c);
    NuFileLoadBuffer("gfx\\licnin.s3",gerbils,0x2000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.pal = NULL;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGB24;

    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    //firstscreen1_mtl->attrib = (struct numtlattrib_s *)((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 4;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\copyr1.s3",gerbils,0x4000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x40000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\euro.s3",gerbils,0x2000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    (firstscreen1_mtl->diffuse).r = 1.0f;
    (firstscreen1_mtl->diffuse).g = 1.0f;
    (firstscreen1_mtl->diffuse).b = 1.0f;
    //firstscreen1_mtl->attrib = (numtlattrib_s *)((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    nuvideo_global_vbcnt = 0;
    do {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } while (nuvideo_global_vbcnt < 0x78);
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    free_x(gerbils);
    if (firstscreen1_mtl != NULL) {
        if (firstscreen1_mtl->tid != 0) {
            NuTexDestroy(firstscreen1_mtl->tid);
        }
        NuMtlDestroy(firstscreen1_mtl);
    }
    return;
}
