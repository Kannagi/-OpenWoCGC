//#define ALIGN_ADDRESS(addr, al) (((u32)(addr) + ((u32)al - 1)) & ~((u32)al - 1))
#define ASSIGN_IF_SET(a, b) a = (a == NULL) ? NULL : b
#define ALIGN_ADDRESS(addr, al) (((u32)(addr) + (al - 1)) & ~(al - 1))

// this one is a common known-integer-divisor optimization, where instead
// of using DIV we multiply by some magic constant and then shift by 
// some amount, this particular combination should be equivalent to x/20,
// but the compiler doesn't feel like doing the optimization itself??
#define FAST_DIV_20(num) (((iVar5 - iVar4) * (s32)0xCCCCCCCD) >> 4)

#define PI 3.1415927f
#define TAU 6.2831855f
#define MAX_FIXED_POINT 65536
#define DEG_TO_FIXED_POINT (MAX_FIXED_POINT * (1 / (2 * PI)))
short RO_640460[8];

struct NUGCUTLOCFNDATA_s* locatorfns;
static struct NUGCUTLOCFNDATA_s cutscene_locatorfns[2];
static struct instNUGCUTSCENE_s* active_cutscene_instances;

//PS2
void NuGCutSceneSysInit(struct NUGCUTLOCFNDATA_s *locfndat)
{
  locatorfns = locfndat;
  active_cutscene_instances = NULL;
  return;
}

//PS2
void NuGCutSceneSysUpdate(s32 paused)
{
    struct instNUGCUTSCENE_s* p;
    
    p = active_cutscene_instances;
    while( p != NULL ) {
        if (!p->been_updated_this_frame) {
            instNuGCutSceneUpdate(p, paused);
            p->been_updated_this_frame = 1;
        }
        p = p->next;
    }
    return;
}

//PS2
void NuGCutSceneSysRender(void)
{
    struct instNUGCUTSCENE_s *icutscene;
    
    for(icutscene = active_cutscene_instances; icutscene != NULL; icutscene = icutscene->next) { 
        instNuGCutSceneRender(icutscene);
        icutscene->been_updated_this_frame = 0; //& 0xffbfffff
    }
    
    return;
}


//PS2
struct NUGCUTSCENE_s * NuGCutSceneLoad(char *fname,union variptr_u *buff,union variptr_u *endbuff)
{
    struct NUGCUTSCENE_s *cutscene;
    s32 fsize;
    
    buff->voidptr = (void*)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
    fsize = NuFileLoadBuffer(fname,buff->voidptr, (s32)endbuff->voidptr - (s32)buff->voidptr);
    if (fsize == 0) {
     NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x149)(
         "Could not read from cutscene <fname>, either file is not there or the buffer may be full");
    }
    
    cutscene = (struct NUGCUTSCENE_s *)buff->voidptr;
    buff->voidptr = (void *)((s32)&cutscene->version + fsize);
    if (cutscene->version != 1) {
        NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x14e)
            ("Out of date version of cutscene <%s>, you have version <%d>", fname, cutscene->version);
    }
    
    cutscene->address_offset = (s32)cutscene - cutscene->address_offset;
    
    ASSIGN_IF_SET(cutscene->name_table, cutscene->name_table + cutscene->address_offset);
    
    if (cutscene->cameras != NULL) {
        NuGCutCamsSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->locators != NULL) {
        NuGCutLocatorSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->rigids != NULL) {
        NuGCutRigidSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->chars != NULL) {
        NuGCutCharSysFixPtrs(cutscene, cutscene->address_offset);
    }
    
    if (cutscene->triggersys != NULL) {
        NuGCutTriggerSysFixPtrs(cutscene, cutscene->address_offset);
    }

    ASSIGN_IF_SET(cutscene->bbox, (struct NUGCUTBBOX_s*)((s32)cutscene->bbox + cutscene->address_offset));
    
    return cutscene;
}


//PS2
void NuGCutSceneFixUp(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *scene,struct NUTRIGGERSYS_s *triggersys)
{
  if (cutscene != NULL) {
    if (cutscene->rigids != NULL) {
      NuGCutRigidSysFixUp(cutscene,scene);
    }
    if (cutscene->chars != NULL) {
      NuGCutCharSysFixUp(cutscene);
    }
    if (cutscene->locators != NULL) {
      NuGCutLocatorSysFixUp(cutscene->locators);
    }
    if ((cutscene->triggersys != NULL) && (triggersys != NULL)
       ) {
      NuGCutTriggerSysFixUp(cutscene,triggersys);
    }
  }
  return;
}


//PS2
struct instNUGCUTSCENE_s *instNuGCutSceneCreate (struct NUGCUTSCENE_s *cutscene, struct nugscn_s *gscene,
                                                struct instNUTRIGGERSYS_s *itriggersys, char *name, union variptr_u *buff)
{
    struct instNUGCUTSCENE_s *icutscene;

    icutscene = 0;
    if (cutscene != NULL)
    {
        icutscene = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &icutscene[1];
        
        memset(icutscene, 0, sizeof(struct instNUGCUTSCENE_s));
        
        icutscene->next = active_cutscene_instances;
        if (active_cutscene_instances != NULL) {
            active_cutscene_instances->prev = icutscene;
        }
        
        active_cutscene_instances = icutscene;
        icutscene->cutscene = cutscene;
        if (name != NULL) {
            sprintf(icutscene->name, name);
        }
        
        if (cutscene->cameras != NULL) {
            icutscene->icameras = instNuCGutCamSysCreate(cutscene->cameras, buff);
        }
        
        if (cutscene->rigids != NULL) {
            icutscene->irigids = instNuCGutRigidSysCreate(cutscene, gscene, buff);
        }
        
        if (cutscene->chars != NULL) {
            icutscene->ichars = instNuCGutCharSysCreate(cutscene, buff);
        }
        
        if (cutscene->locators != NULL) {
            icutscene->ilocators = NuCGutLocatorSysCreateInst(cutscene->locators, buff);
        }
        
        if ((cutscene->triggersys != NULL) && (itriggersys != NULL)) {
            icutscene->itriggersys = instNuCGutTriggerSysCreate(cutscene, itriggersys, buff);
        }
        
        instNuGCutSceneCalculateCentre(icutscene, 0);
        icutscene->rate = 1.0f;
    }
    
    return icutscene;
}

void instNuGCutSceneDestroy(instNUGCUTSCENE_s *icutscene)

{
  if ((*(uint *)&icutscene->field_0x6c & 0x40000000) != 0) //param_1[0x1b] & 0x40000000U
  {
    instNuGCutSceneEnd(icutscene);
  }
  if (icutscene->next != (instNUGCUTSCENE_s *)0x0) {
    icutscene->next->prev = icutscene->prev;
  }
  if (icutscene->prev == (instNUGCUTSCENE_s *)0x0) {
    active_cutscene_instances = icutscene->next;
  }
  else {
    icutscene->prev->next = icutscene->next;
  }
  return;
}

//PS2
void instNuGCutSceneStart(struct instNUGCUTSCENE_s *icutscene)
{
    icutscene->cframe = 1.0f;
    
    icutscene->is_playing = 1;
    icutscene->played_first_frame = 0;
    
    if (icutscene->icameras != NULL) {
        instNuGCutCamSysStart(icutscene->icameras, icutscene->cutscene->cameras);
    }
    
    if (icutscene->irigids != NULL) {
        instNuGCutRigidSysStart(icutscene->irigids, icutscene->cutscene->rigids);
    }
    
    if (icutscene->ilocators != NULL) {
        instNuGCutLocatorSysStart(icutscene->ilocators, icutscene->cutscene->locators);
    }
    
    if (icutscene->itriggersys != NULL) {
        instNuGCutTriggerSysStart(icutscene);
    }
    return;
}

void instNuGCutSceneEnd(instNUGCUTSCENE_s *icutscene)

{
  float nframes;
  
  nframes = icutscene->cutscene->nframes;
  *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c & 0xbfffffff;
  icutscene->cframe = nframes + 1.0;
  return;
}


//PS2
void instNuGCutSceneSetEndCallback(struct instNUGCUTSCENE_s *icutscene,void(*fn)(void*))
{
  icutscene->endfn = fn;
  return;
}


void instNuGCutSceneClipTest(instNUGCUTSCENE_s *icutscene)

{
  uint uVar1;
  NUGCUTBBOX_s *bbox;
  int iVar2;
  numtx_s *wm;
  NUGCUTSCENE_s *cutscene;
  float fVar3;
  
  uVar1 = *(uint *)&icutscene->field_0x6c;
  cutscene = icutscene->cutscene;
  *(uint *)&icutscene->field_0x6c = uVar1 | 0x1000000;
  if (cutscene->bbox == (NUGCUTBBOX_s *)0x0) {
    return;
  }
  if (((uVar1 & 0x2000000) == 0) ||
     (fVar3 = NuCameraDistSqr(&icutscene->centre), fVar3 <= icutscene->maxdistsqr)) {
    if ((*(uint *)&icutscene->field_0x6c & 0x4000000) == 0) {
      return;
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x8000000) == 0) {
      bbox = cutscene->bbox;
      wm = &numtx_identity;
    }
    else {
      bbox = cutscene->bbox;
      wm = &icutscene->mtx;
    }
    iVar2 = NuCameraClipTestExtents(&bbox->min,&bbox->max,wm);
    if (iVar2 != 0) {
      return;
    }
  }
  *(uint *)&icutscene->field_0x6c = *(uint *)&icutscene->field_0x6c & 0xfeffffff;
  return;
}

void instNuGCutSceneUpdate(instNUGCUTSCENE_s *icutscene,int paused)

{
  uint uVar1;
  code *endfn;
  NUGCUTSCENE_s *cutscene;
  double dVar2;
  float dt;
  
  cutscene = icutscene->cutscene;
  if ((*(uint *)&icutscene->field_0x6c & 0x800000) != 0) {
    return;
  }
  if ((*(uint *)&icutscene->field_0x6c & 0x90000000) == 0x10000000) {
    instNuGCutSceneStart(icutscene);
  }
  if ((*(uint *)&icutscene->field_0x6c & 0x40000000) == 0) {
    return;
  }
  instNuGCutSceneClipTest(icutscene);
  uVar1 = *(uint *)&icutscene->field_0x6c;
  if ((uVar1 & 0x1000000) == 0) {
    return;
  }
  if ((int)uVar1 < 0) {
    if (paused != 0) goto LAB_80089dbc;
    dVar2 = 1.0;
    dt = icutscene->cframe + icutscene->rate;
    icutscene->cframe = dt;
    if (1.0 < dt) {
      icutscene->cframe = 1.0;
      goto LAB_80089dbc;
    }
    if (dt < (cutscene->nframes + 1.0) - 1.0) goto LAB_80089dbc;
    endfn = (code *)icutscene->endfn;
    if (endfn != (code *)0x0) {
      icutscene->endfn = (void *)0x0;
      (*endfn)(icutscene);
    }
    if (icutscene->next_to_play != (instNUGCUTSCENE_s *)0x0) {
      dVar2 = (double)(icutscene->cframe -
                      (float)((double)(float)((double)cutscene->nframes + dVar2) - dVar2));
      if ((double)cutscene->nframes < dVar2) {
        dVar2 = 0.0;
      }
      instNuGCutSceneEnd(icutscene);
      instNuGCutSceneStart(icutscene->next_to_play);
      icutscene->next_to_play->cframe = (float)((double)icutscene->next_to_play->cframe + dVar2);
      *(uint *)&icutscene->next_to_play->field_0x6c =
           *(uint *)&icutscene->next_to_play->field_0x6c | 0x80000000;
      instNuGCutSceneClipTest(icutscene->next_to_play);
      *(uint *)&icutscene->next_to_play->field_0x6c =
           *(uint *)&icutscene->next_to_play->field_0x6c | 0x400000;
      icutscene->next_to_play = (instNUGCUTSCENE_s *)0x0;
      return;
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x20000000) == 0) {
      instNuGCutSceneEnd(icutscene);
      goto LAB_80089dbc;
    }
    dVar2 = (double)(icutscene->cframe -
                    (float)((double)(float)((double)cutscene->nframes + dVar2) - dVar2));
    if ((double)cutscene->nframes < dVar2) {
      dVar2 = 0.0;
    }
    instNuGCutSceneStart(icutscene);
    uVar1 = *(uint *)&icutscene->field_0x6c;
    icutscene->cframe = (float)((double)icutscene->cframe + dVar2);
  }
  *(uint *)&icutscene->field_0x6c = uVar1 | 0x80000000;
LAB_80089dbc:
  if ((do_cameras != 0) && (icutscene->icameras != (instNUGCUTCAMSYS_s *)0x0)) {
    instNuGCutCamSysUpdate(icutscene,icutscene->cframe);
  }
  if (icutscene->itriggersys != (instNUGCUTTRIGGERSYS_s *)0x0) {
    instNuGCutTriggerSysUpdate(icutscene,icutscene->cframe);
  }
  if ((do_rigids != 0) && (icutscene->irigids != (instNUGCUTRIGIDSYS_s *)0x0)) {
    instNuGCutRigidSysUpdate(icutscene,icutscene->cframe);
  }
  return;
}

//PS2 (change bifields from s32 to s16 type)
static void instNuGCutSceneRender(struct instNUGCUTSCENE_s *icutscene)
{
    
    if (icutscene->is_disabled) {
        return;
    }
    
    if (!icutscene->is_playing) {
        return;
    }
    
    if (!icutscene->is_visible){
        return;
    }
    
    if ((do_rigids != 0) && (icutscene->irigids != NULL)) {
        instNuGCutRigidSysRender(icutscene, icutscene->cframe);
    }
    if ((do_chars != 0) && (icutscene->ichars != NULL)) {
        instNuGCutCharSysRender(icutscene, icutscene->cframe);
    }
    if ((do_locator != 0) && (icutscene->ilocators != NULL)) {
        instNuGCutLocatorSysUpdate(icutscene, icutscene->cframe);
    }
}

//PS2
static void NuGCutCamsSysFixPtrs(struct NUGCUTSCENE_s *cutscene, s32 address_offset)
{
    struct NUGCUTCAMSYS_s *camsys;
    
    if (cutscene->cameras != NULL) {
        camsys = (struct NUGCUTCAMSYS_s *)((s32)cutscene->cameras + address_offset);
        cutscene->cameras = camsys;
        ASSIGN_IF_SET(camsys->cutcams, (struct NUGCUTCAM_s *)((s32)camsys->cutcams + address_offset));
        
        if (camsys->camanim != NULL) {
            camsys->camanim = NuAnimData2FixPtrs(camsys->camanim, address_offset);
        }
        camsys->camchanges = StateAnimFixPtrs(camsys->camchanges, address_offset);
    }
    return;
}

void instNuGCutSceneCalculateCentre(instNUGCUTSCENE_s *icutscene,numtx_s *mtx)

{
  NUGCUTSCENE_s *cut;
  NUGCUTBBOX_s *bbox;
  
  cut = icutscene->cutscene;
  bbox = cut->bbox;
  if (bbox == (NUGCUTBBOX_s *)0x0) {
    (icutscene->centre).z = 0.0;
    (icutscene->centre).x = 0.0;
    (icutscene->centre).y = 0.0;
  }
  else {
    (icutscene->centre).x = ((bbox->max).x + (bbox->min).x) * 0.5;
    (icutscene->centre).y = ((cut->bbox->max).y + (cut->bbox->min).y) * 0.5;
    (icutscene->centre).z = ((cut->bbox->max).z + (cut->bbox->min).z) * 0.5;
  }
  if (mtx != (numtx_s *)0x0) {
    NuVecMtxTransform(&icutscene->centre,&icutscene->centre,mtx);
  }
  return;
}

//PS2
struct instNUGCUTCAMSYS_s * instNuCGutCamSysCreate(struct NUGCUTCAMSYS_s *cameras,union variptr_u *buff)
{
  struct instNUGCUTCAMSYS_s *icameras;
  
  icameras = NULL;
  if (((cameras != NULL) && (cameras->ncutcams != 0)) && (buff != NULL ))
  {
    icameras = (struct instNUGCUTCAMSYS_s *)(((int)buff->voidptr + 0xf) & 0xfffffff0);
    buff->voidptr = icameras + 1;
    memset(icameras,0,0x10);
    buff->intaddr = ((u32)buff->voidptr + 0xf) & 0xfffffff0;
    icameras->icutcams = (struct instNUGCUTCAM_s *)buff->intaddr;
    buff->voidptr = (void *)((u32)buff->voidptr + cameras->ncutcams * 4);
    memset(icameras->icutcams,0,cameras->ncutcams << 2);
  }
  return icameras;
}



void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  int state;
  NUERRORFUNC *er;
  NUGCUTCAMTGT_s *cutcamtgt;
  numtx_s *ptr3;
  nuanimdata2_s *camanim;
  numtx_s *ptrCcamMtx;
  numtx_s *ptrptr;
  instNUGCUTCAM_s *icutcam;
  NUGCUTCAM_s *cutcam2;
  byte nexttgt;
  NUGCUTCAMSYS_s *camsys;
  instNUGCUTCAMSYS_s *icamSys;
  NUGCUTCAM_s *cutcam;
  bool c;
  double dVar1;
  double dVar2;
  nuanimtime_s atime;
  numtx_s aim_mtx;
  Quat q_to;
  Quat q_result;
  Quat q_from;
  float local_58;
  float local_54;
  float local_50;
  uchar local_48 [28];
  numtx_s *cCamMtx;
  float frame;
  char remap;
  
  dVar2 = (double)current_frame;
  icamSys = icutscene->icameras;
  camsys = icutscene->cutscene->cameras;
  if ((camsys->camchanges != (NUSTATEANIM_s *)0x0) &&
     (state = StateAnimEvaluate(camsys->camchanges,&icamSys->next_switch,local_48,current_frame),
     state != 0)) {
    icamSys->current_camera = local_48[0];
  }
  nexttgt = icamSys->next_tgt_ix;
  if ((uint)nexttgt < (uint)icamSys->ntgts) {
    cutcamtgt = icamSys->tgts;
    frame = cutcamtgt[nexttgt].frame;
    while ((double)frame <= dVar2) {
      icutcam = icamSys->icutcams;
      remap = camsys->remap_table[cutcamtgt[nexttgt].camid];
      icutcam[remap].flags = icutcam[remap].flags | 2;
      icutcam[remap].tgt_ix = icamSys->next_tgt_ix;
      nexttgt = icamSys->next_tgt_ix + 1;
      icamSys->next_tgt_ix = nexttgt;
      if ((uint)icamSys->ntgts <= (uint)nexttgt) break;
      cutcamtgt = icamSys->tgts;
      frame = cutcamtgt[nexttgt].frame;
    }
  }
  nexttgt = icamSys->next_tgt_ix;
  if (nexttgt != 0) {
    cutcamtgt = icamSys->tgts;
    frame = cutcamtgt[nexttgt - 1].frame;
    while (dVar2 < (double)frame) {
      icutcam = icamSys->icutcams;
      icamSys->next_tgt_ix = (uchar)(nexttgt - 1);
      remap = camsys->remap_table[cutcamtgt->pad[((nexttgt - 1) * 0x10 & 0xff0) - 1]];
      icutcam[remap].flags = icutcam[remap].flags | 2;
      icutcam[remap].tgt_ix = icamSys->next_tgt_ix;
      nexttgt = icamSys->next_tgt_ix;
      if (nexttgt == 0) break;
      cutcamtgt = icamSys->tgts;
      frame = cutcamtgt[nexttgt - 1].frame;
    }
  }
  if (-1 < icamSys->current_camera) {
    if ((int)camsys->ncutcams <= (int)icamSys->current_camera) {
      er = NuErrorProlog("C:/source/crashwoc/code/gamelib/gcutscn.c",0x2e3);
      (*er)("instNuGCutCamSysUpdate: internal error");
    }
    icutcam = icamSys->icutcams + icamSys->current_camera;
    cutcam = camsys->cutcams + icamSys->current_camera;
    if (((cutcam->flags & 1) == 0) || (camsys->camanim->nnodes <= (short)(ushort)cutcam->anim_ix))  {
      state = 0x30;
      cCamMtx = &cutscenecammtx;
      do {
        cutcam2 = cutcam;
        ptrCcamMtx = cCamMtx;
        state = state + -0x18;
        ptrCcamMtx->_00 = (cutcam2->mtx)._00;
        ptrCcamMtx->_01 = (cutcam2->mtx)._01;
        ptrCcamMtx->_02 = (cutcam2->mtx)._02;
        ptrCcamMtx->_03 = (cutcam2->mtx)._03;
        ptrCcamMtx->_10 = (cutcam2->mtx)._10;
        cutcam = (NUGCUTCAM_s *)&(cutcam2->mtx)._12;
        ptrCcamMtx->_11 = (cutcam2->mtx)._11;
        cCamMtx = (numtx_s *)&ptrCcamMtx->_12;
      } while (state != 0);
      ptrCcamMtx->_12 = *(float *)cutcam;
      ptrCcamMtx->_13 = (cutcam2->mtx)._13;
      ptrCcamMtx->_20 = (cutcam2->mtx)._20;
      ptrCcamMtx->_21 = (cutcam2->mtx)._21;
    }
    else {
      NuAnimData2CalcTime(camsys->camanim,(float)dVar2,&atime);
      camanim = camsys->camanim;
      state = (int)camanim->ncurves * (uint)cutcam->anim_ix;
      NuAnimCurve2SetApplyToMatrix
                (camanim->curves + state,camanim->curveflags + state,
                 camanim->curvesetflags[cutcam->anim_ix],&atime,&cutscenecammtx);
    }
    if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
      NuMtxMul(&cutscenecammtx,&cutscenecammtx,&icutscene->mtx);
    }
    set_cutscenecammtx = 1;
    if ((icutcam->flags & 2) != 0) {
      cutcamtgt = icamSys->tgts + icutcam->tgt_ix;
      dVar1 = (double)cutcamtgt->nframes;
      NuFabs(cutcamtgt->nframes);
      frame = NuFsign(cutcamtgt->nframes);
      c = frame < 0.0;
      if ((!c) || (dVar2 < (double)(float)((double)cutcamtgt->frame + dVar1))) {
        local_58 = cutscenecammtx._30;
        local_54 = cutscenecammtx._31;
        local_50 = cutscenecammtx._32;
        state = 0x30;
        cCamMtx = &aim_mtx;
        ptrCcamMtx = &cutscenecammtx;
        do {
          ptrptr = ptrCcamMtx;
          ptr3 = cCamMtx;
          state = state + -0x18;
          ptr3->_00 = ptrptr->_00;
          ptr3->_01 = ptrptr->_01;
          ptr3->_02 = ptrptr->_02;
          ptr3->_03 = ptrptr->_03;
          ptr3->_10 = ptrptr->_10;
          ptr3->_11 = ptrptr->_11;
          cCamMtx = (numtx_s *)&ptr3->_12;
          ptrCcamMtx = (numtx_s *)&ptrptr->_12;
        } while (state != 0);
        ptr3->_12 = ptrptr->_12;
        ptr3->_13 = ptrptr->_13;
        ptr3->_20 = ptrptr->_20;
        ptr3->_21 = ptrptr->_21;
        NuMtxLookAtZ(&aim_mtx,cutcamtgt->tgt);
        if (dVar1 <= 0.009999999776482582) {
          dVar2 = 1.0;
        }
        else {
          dVar2 = (double)(float)((double)(float)(dVar2 - (double)cutcamtgt->frame) / dVar1);
          if (dVar2 <= 1.0) {
            if (dVar2 < 0.0) {
              c = false;
            }
          }
          else {
            c = true;
          }
        }
        if (c) {
          dVar2 = (double)(float)(1.0 - dVar2);
        }
        NuMtxToQuat(&aim_mtx,&q_result);
        NuMtxToQuat(&cutscenecammtx,&q_to);
        NuQuatSlerp((float)dVar2,&q_from,&q_to,&q_result);
        NuQuatToMtx(&q_from,&cutscenecammtx);
        cutscenecammtx._30 = local_58;
        cutscenecammtx._32 = local_50;
        cutscenecammtx._31 = local_54;
      }
      else {
        icutcam->flags = icutcam->flags & 0xfd;
      }
    }
  }
  return;
}

//PS2
void instNuGCutCamSysStart(struct instNUGCUTCAMSYS_s *icamsys,struct NUGCUTCAMSYS_s *camsys)
{
  struct instNUGCUTCAM_s *icutcam;
  u32 i;
  
  icamsys->next_switch = '\0';
  icamsys->current_camera = camsys->initial_camid;
  icamsys->next_tgt_ix = '\0';
    for (i = 0; camsys->ncutcams > i; i++)
    {
      icutcam = &icamsys->icutcams[i];
      icutcam->tgt_ix = '\0';
      icutcam->flags = icutcam->flags & 0xfd;
    }
  return;
}


//PS2
static void NuGCutLocatorSysFixPtrs(struct NUGCUTSCENE_s *cutscene,s32 address_offset)
{
  struct NUGCUTLOCATOR_s *locator;
  struct NUGCUTLOCATORTYPE_s *loctype;
  struct NUGCUTLOCATORSYS_s *locatorsys;
  s32 i;
  
  if (cutscene->locators != NULL) {
    locatorsys = (char*)cutscene->locators + address_offset;
    cutscene->locators = locatorsys;
      
    ASSIGN_IF_SET(locatorsys->locators, (struct NUGCUTLOCATOR_s *)((s32)locatorsys->locators + address_offset));
    if ((locatorsys->locators != NULL)) {
      for(i = 0; i < (s32)locatorsys->nlocators; i++)
      {
        locator = &locatorsys->locators[i];
        if (locator->anim != NULL) {
          locator->anim =  NuAnimData2FixPtrs(locator->anim,address_offset);
        }
      }
    }
    ASSIGN_IF_SET(locatorsys->locator_types, (struct NUGCUTLOCATOR_s *)((s32)locatorsys->locator_types + address_offset));
    if ((locatorsys->locator_types != NULL)) {
      for(i = 0; i < (s32)locatorsys->ntypes; i++)
      {
        loctype = &locatorsys->locator_types[i];
        loctype->name = &cutscene->name_table[(s32)(loctype->name)] - 1;
      }
    }
  }
  return;
}

//PS2
void NuGCutLocatorCalcMtx
               (struct NUGCUTLOCATOR_s *locator,float current_frame, struct numtx_s *mtx,struct nuanimtime_s *atime)
{
  char curvesetflags;
  struct nuanimcurve2_s *curves;
  char *curveflags;
  //struct numtx_s* in_t0_lo;
  struct NuVec t;
  struct NuVec r;
  struct nuangvec_s rf;
  

  if (locator->anim != NULL) {
    curves = locator->anim->curves;
    curveflags = locator->anim->curveflags;
    curvesetflags = *locator->anim->curvesetflags;
    if ((curvesetflags & 1U) != 0) {
      r.x = NuAnimCurve2CalcVal(curves + 3,atime,(int)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,atime,(int)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,atime,(int)curveflags[5]);
      rf.x = (s32)(r.x * 10430.378f);
      rf.y = (s32)(r.y * 10430.378f);
      rf.z = (s32)(r.z * 10430.378f);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    else {
      NuMtxSetIdentity(mtx);
    }
      curvesetflags = *curveflags;
    t.x = NuAnimCurve2CalcVal(curves,atime,(int)curvesetflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,atime,(int)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,atime,(int)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(struct NuVec *)&(locator->mtx)._30);
  }
  else {
     //memcpy

      //*mtx = locator->mtx;
      memcpy(mtx, &locator->mtx, sizeof(struct numtx_s));
  }
  return;
}


//PS2
s32 NuGCutLocatorIsVisble
              (struct NUGCUTLOCATOR_s *locator,float current_frame,struct nuanimtime_s *atime,float *rate)
{
  s32 is_visible;
  char *curveflags;
  struct nuanimcurve2_s *curves;
  

  if (locator->anim != NULL) {
    curveflags = locator->anim->curveflags;
    curves = locator->anim->curves;
    is_visible = (s32)NuAnimCurve2CalcVal((struct nuanimcurve2_s *)(curves + 7),atime,(s32)curveflags[7 ]);;
    if ((is_visible != 0) && (rate != NULL)) {
      *rate = NuAnimCurve2CalcVal((struct nuanimcurve2_s *)(curves + 6),atime,
                                  (s32)curveflags[6]);
    }
  }
  else {
    is_visible = locator->flags & 8;
    if (((locator->flags & 8) != 0) && (rate != NULL)) {
      *rate = locator->rate;
    }
  }
  return is_visible;
}


//PS2
struct instNUGCUTLOCATORSYS_s * NuCGutLocatorSysCreateInst(struct NUGCUTLOCATORSYS_s * locators, union variptr_u *buff)
{
  struct instNUGCUTLOCATORSYS_s *iloc;
  
  iloc = NULL;

  if (((locators != NULL) &&
      (iloc = NULL, locators->nlocators != '\0')) &&
     (buff != NULL)) {
    iloc = (struct instNUGCUTLOCATORSYS_s *)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
    buff->voidptr = iloc + 1;
    iloc->ilocators = NULL;
    buff->intaddr = (u32)(((u32)buff->voidptr + 0xf) & 0xfffffff0);
    iloc->ilocators = (struct instNUGCUTLOCATOR_s *) buff->intaddr;
    buff->voidptr = (void *)((u32)buff->voidptr + (u32)locators->nlocators * 8);
    memset(iloc->ilocators,0,((u32)locators->nlocators << 3));
  }
  return iloc;
}

//PS2
static void instNuGCutLocatorSysUpdate(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
  struct NUGCUTLOCATORSYS_s *locatorsys;
  struct instNUGCUTLOCATORSYS_s *ilocatorsys;
  struct NUGCUTLOCATOR_s *locator;
  //struct NUGCUTSCENE_s* cutscene;
  u32 i;
    
  locatorsys = icutscene->cutscene->locators;    //locatorsys = cutscene->locators;
  ilocatorsys = icutscene->ilocators;
    for (i = 0; i < locatorsys->nlocators; i++)
    {
      locator = &locatorsys->locators[i];
      if ((locator->flags & 1) == 0) {
        if ((locator->flags & 2) == 0) {
            if (icutscene->has_mtx & 0x10) {
            instNuGCutLocatorUpdate
                      (icutscene,locatorsys,ilocatorsys->ilocators + i,locator,current_frame,
                       &icutscene->mtx);
          }
          else {
            instNuGCutLocatorUpdate
                      (icutscene,locatorsys,ilocatorsys->ilocators + i,locator,current_frame,0);
          }
        }
      }
    }
  return;
}


void instNuGCutLocatorSysStart(instNUGCUTLOCATORSYS_s *ilocatorsys,NUGCUTLOCATORSYS_s *locatorsys )

{
  uint i;
  uint n;
  
  i = 0;
  if (locatorsys->nlocators != '\0') {
    do {
      n = i + 1;
      ilocatorsys->ilocators[i].time = 0.0;
      i = n;
    } while (n < locatorsys->nlocators);
    return;
  }
  return;
}

//PS2
inline static s32 LookupLocatorFn(char* name)
{
    s32 ix = 0;
      if (locatorfns != NULL) {
          while (locatorfns[ix].name) {
              if (strcasecmp(name,locatorfns[ix].name) == 0) {
                  return ix;
              }
              ix++;
          }
      }
  return -1;
}

//PS2
static void NuGCutLocatorSysFixUp(struct NUGCUTLOCATORSYS_s *locatorsys) 
{
  u32 i;
  struct NUGCUTLOCATORTYPE_s *loctype;

    for(i = 0; i < locatorsys->ntypes; i++) {
          
      loctype = &locatorsys->locator_types[i];
      if ((loctype->flags & 1) != 0) {
        loctype->ix = LookupDebrisEffect(loctype->name);
      }
      else {
        if ((loctype->flags & 2) != 0) {
          loctype->ix = LookupLocatorFn(loctype->name);
        }
      }
      if (loctype->ix == 0xFFFF) {
        NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x4FB)
            ("NuGCutLocatorSysFixUp: cannot fixup locator <%s> in cutscene", loctype->name);
      }
    }
  return;
}

//PS2
void instNuGCutLocatorUpdate
               (struct instNUGCUTSCENE_s *icutscene,struct NUGCUTLOCATORSYS_s *locatorsys,
               struct instNUGCUTLOCATOR_s *ilocator,struct NUGCUTLOCATOR_s *locator,
                float current_frame,struct numtx_s * wm)
{
    struct NUGCUTLOCATORTYPE_s* loctype;
    
    loctype = &locatorsys->locator_types[locator->locator_type];
    
    if ((loctype->flags & 1U) != 0) {
        instNuGCutDebrisLocatorUpdate(locatorsys, ilocator, locator, current_frame, wm);
    }
    else if (((loctype->flags & 2U) != 0) && (loctype->ix != 0xffff)) {
        (locatorfns[loctype->ix].fn)(icutscene, locatorsys, ilocator, locator, current_frame, wm);
    }
    
    return;
}


void instNuGCutDebrisLocatorUpdate (NUGCUTLOCATORSYS_s *locatorsys,instNUGCUTLOCATOR_s *ilocator, NUGCUTLOCATOR_s *locator,float current_frame,numtx_s *wm)

{
	//TODO
}


//PS2
static void NuGCutRigidSysFixPtrs(struct NUGCUTSCENE_s* cutscene, s32 address_offset)
{

    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct NUGCUTRIGID_s *rigid;
    s32 i;

  
    if (cutscene->rigids != NULL) {
        rigidsys = (struct NUGCUTRIGIDSYS_s *)((int)cutscene->rigids + address_offset);
        cutscene->rigids = rigidsys;
        ASSIGN_IF_SET(rigidsys->rigids, (struct NUGCUTRIGID_s *)((int)(rigidsys->rigids) + address_offset)); //rigidsys[address_offset].rigids
        
        if ((rigidsys->rigids != NULL)) {
            for(i = 0; i < rigidsys->nrigids; i++)
            {
                rigid = &rigidsys->rigids[i];
                if (rigid->name != NULL) {
                    rigid->name = &cutscene->name_table[(u32)rigid->name] - 1; // ????
                }

                if (rigid->anim != NULL) {
                    rigid->anim = NuAnimData2FixPtrs(rigid->anim, address_offset);
                }
                rigid->visibiltyanim = StateAnimFixPtrs(rigid->visibiltyanim, address_offset);
            }
        }
    }
    return;
}

//PS2
static void NuGCutRigidSysFixUp(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *scene)
{
    struct NUGCUTRIGIDSYS_s *rigidsys;
    struct NUGCUTLOCATORSYS_s *locator;
    struct NUGCUTRIGID_s *rigid;
    s32 i;
    
    
    rigidsys = cutscene->rigids;
    locator = cutscene->locators;
    if (scene != NULL) {
        for(i = 0; i < rigidsys->nrigids; i++)
        {
            rigid = &rigidsys->rigids[i];
            
            if (NuSpecialFind(scene, &rigid->special, rigid->name) != 0) {
                rigid->flags |= 4;
            } else {
                NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x5d6)
                    ("NuGCutRigidSysFixUp: cannot fixup rigid object <%s> in cutscene", rigid->name);
            }
            
            if (rigid->nlocators == 0) {
                rigid->first_locator = 0xff;
            }
            else if ((s32)rigid->locators < 0xff) {
                rigid->first_locator = rigid->locators;
                rigid->locators = &locator->locators[(s32)rigid->locators];
            }
            else {
                rigid->first_locator = 0xff;
            }
        }
    }
    return;
}

//PS2
static struct instNUGCUTRIGIDSYS_s *
instNuCGutRigidSysCreate(struct NUGCUTSCENE_s *cutscene,struct nugscn_s *gscene,union variptr_u *buff)
{
    
    struct NUGCUTRIGIDSYS_s *rigids;
    struct instNUGCUTRIGIDSYS_s *irigids;
    struct NUGCUTRIGID_s *cutrigid;
    struct instNUGCUTRIGID_s *icutrigid;
    s32 i;
    s32 iVar4;
    s32 iVar5;
    
    irigids = NULL;
    rigids = cutscene->rigids;
    if ((rigids != NULL) && (rigids->nrigids != 0) && (buff != NULL))
    {
        irigids = buff->voidptr = (struct instNUGCUTRIGIDSYS_s *)ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &irigids[1];
        
        irigids->irigids = NULL;
        icutrigid = buff->voidptr = (struct instNUGCUTRIGID_s *)ALIGN_ADDRESS(buff->voidptr, 0x10);
        irigids->irigids = icutrigid;
        
        buff->voidptr = &buff->voidptr[rigids->nrigids * sizeof(struct instNUGCUTRIGID_s)];
        memset(irigids->irigids, 0, (rigids->nrigids * sizeof(struct instNUGCUTRIGID_s)));
        
        for (i = 0; i < rigids->nrigids; i++)
        {
            cutrigid = &rigids->rigids[i];
            icutrigid = &irigids->irigids[i];
            if ((gscene != NULL) && (gscene != cutrigid->special.scene)) {
                if (cutrigid->special.special != NULL) {
                    icutrigid->special.scene = gscene;
                    iVar4 = (s32)cutrigid->special.scene->specials;
                    iVar5 = (s32)cutrigid->special.special;
                    icutrigid->special.special = &gscene->specials[FAST_DIV_20(iVar5 - iVar4)];
                } else {
                    NuDebugMsgProlog("..\\nu2.ps2\\gamelib\\gcutscn.c", 0x606)
                        ("instNuCGutRigidSysCreate: cannot reference rigid object <%s>, object was not fixed up", cutrigid->name);
                }
            }
            else {
                icutrigid->special = cutrigid->special;
            }
        }
    }
    return irigids;
}



//PS2 Match
static void NuGCutRigidCalcMtx(struct NUGCUTRIGID_s* rigid, float current_frame, struct numtx_s* mtx) 
{
  struct nuanimcurve2_s *curves;
  struct nuanimtime_s atime;
  struct NuVec t;
  struct NuVec r;
  struct nuangvec_s rf;
  struct NuVec scale;
  char *curveflags;
  char curvesetflags;
  
  if (rigid->anim != NULL) {
    curves = rigid->anim->curves;
    curvesetflags = *rigid->anim->curvesetflags;
    curveflags = rigid->anim->curveflags;
    NuAnimData2CalcTime(rigid->anim,current_frame,&atime);
    if ((curvesetflags & 1) != 0) {
      r.x = NuAnimCurve2CalcVal(curves + 3,&atime,(s32)curveflags[3]);
      r.y = NuAnimCurve2CalcVal(curves + 4,&atime,(s32)curveflags[4]);
      r.z = NuAnimCurve2CalcVal(curves + 5,&atime,(s32)curveflags[5]);
      rf.z = (s32)(r.z * 10430.378f);
      rf.x = (s32)(r.x * 10430.378f);
      rf.y = (s32)(r.y * 10430.378f);
      NuMtxSetRotateXYZ(mtx,&rf);
    }
    else {
      NuMtxSetIdentity(mtx);
    }
    if ((curvesetflags & 8) != 0) {
      scale.x = NuAnimCurve2CalcVal(curves + 6,&atime,(s32)curveflags[6]);
      scale.y = NuAnimCurve2CalcVal(curves + 7,&atime,(s32)curveflags[7]);
      scale.z = NuAnimCurve2CalcVal(curves + 8,&atime,(s32)curveflags[8]);
      NuMtxPreScale(mtx,&scale);
    }
    t.x = NuAnimCurve2CalcVal(curves,&atime,(s32)*curveflags);
    t.y = NuAnimCurve2CalcVal(curves + 1,&atime,(s32)curveflags[1]);
    t.z = NuAnimCurve2CalcVal(curves + 2,&atime,(s32)curveflags[2]);
    NuMtxTranslate(mtx,&t);
    mtx->_02 = -mtx->_02;
    mtx->_12 = -mtx->_12;
    mtx->_20 = -mtx->_20;
    mtx->_21 = -mtx->_21;
    mtx->_23 = -mtx->_23;
    mtx->_32 = -mtx->_32;
    NuMtxTranslate(mtx,(struct NuVec*)&(rigid->mtx)._30);	//CHECK param2!
  }
  else {
    //memcpy
      *mtx = rigid->mtx;
  }
  return;
}

//PS2
static void instNuGCutRigidSysStart(struct instNUGCUTRIGIDSYS_s *irigidsys,struct NUGCUTRIGIDSYS_s *rigidsys)
{
  u32 i;
    
  for(i = 0; i < rigidsys->nrigids; i++) {
      irigidsys->irigids[i].visibleframeix = '\0';
  }
}

void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  nuinstflags_s flags;
  int iVar1;
  nuinstance_s *tinst;
  instNUGCUTRIGID_s *irigid;
  numtx_s *mtmp;
  instNUGCUTRIGIDSYS_s *irigidsys;
  NUGCUTSCENE_s *cutscene;
  NUGCUTRIGIDSYS_s *rigidsys;
  instNUGCUTLOCATORSYS_s *ilocatorsys;
  NUGCUTRIGID_s *rigid;
  int i;
  NUGCUTLOCATORSYS_s *locatorsys;
  int j;
  double dVar2;
  numtx_s mtx;
  uchar local_40 [20];
  nuinstance_s *inst;
  numtx_s *m;
  
  dVar2 = (double)current_frame;
  i = 0;
  cutscene = icutscene->cutscene;
  irigidsys = icutscene->irigids;
  rigidsys = cutscene->rigids;
  if (rigidsys->nrigids != 0) {
    do {
      irigid = irigidsys->irigids;
      rigid = rigidsys->rigids + i;
      if (((rigid->flags & 2) == 0) && ((rigid->flags & 4) != 0)) {
        if ((rigid->visibiltyanim != (NUSTATEANIM_s *)0x0) &&
           (j = StateAnimEvaluate(rigid->visibiltyanim,(uchar *)&irigid[i].visibleframeix,local_40 ,
                                  (float)dVar2), j != 0)) {
          if (local_40[0] == '\0') {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags & 0x7fffffff);
          }
          else {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags | 0x80000000);
          }
          inst->flags = flags;
        }
        if ((int)(irigid[i].special.special)->instance->flags < 0) {
          NuGCutRigidCalcMtx(rigid,(float)dVar2,&mtx);
          if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
            NuMtxMul(&mtx,&mtx,&icutscene->mtx);
          }
          j = 0x30;
          inst = (irigid[i].special.special)->instance;
          m = &mtx;
          do {
            mtmp = m;
            tinst = inst;
            j = j + -0x18;
            (tinst->matrix)._00 = mtmp->_00;
            (tinst->matrix)._01 = mtmp->_01;
            (tinst->matrix)._02 = mtmp->_02;
            (tinst->matrix)._03 = mtmp->_03;
            (tinst->matrix)._10 = mtmp->_10;
            (tinst->matrix)._11 = mtmp->_11;
            inst = (nuinstance_s *)&(tinst->matrix)._12;
            m = (numtx_s *)&mtmp->_12;
          } while (j != 0);
          *(float *)inst = mtmp->_12;
          (tinst->matrix)._13 = mtmp->_13;
          (tinst->matrix)._20 = mtmp->_20;
          (tinst->matrix)._21 = mtmp->_21;
          if (((int)(irigid[i].special.special)->instance->flags < 0) &&
             (rigid->first_locator != 0xff)) {
            j = 0;
            locatorsys = cutscene->locators;
            ilocatorsys = icutscene->ilocators;
            if (rigid->nlocators != '\0') {
              do {
                iVar1 = (uint)rigid->first_locator + j;
                j = j + 1;
                instNuGCutLocatorUpdate
                          (icutscene,locatorsys,ilocatorsys->ilocators + iVar1,
                           locatorsys->locators + iVar1,(float)dVar2,&mtx);
              } while (j < (int)(uint)rigid->nlocators);
            }
          }
        }
      }
      i = i + 1;
    } while (i < (int)(uint)rigidsys->nrigids);
  }
  return;
}


void instNuGCutRigidSysRender(instNUGCUTSCENE_s *icutscene,float current_frame)

{
  nuinstflags_s flags;
  int iVar1;
  instNUGCUTRIGID_s *irigid;
  nuinstance_s *inst;
  instNUGCUTRIGIDSYS_s *irigidsys;
  NUGCUTSCENE_s *cutscene;
  NUGCUTRIGIDSYS_s *rigidsys;
  instNUGCUTLOCATORSYS_s *ilocatorsys;
  NUGCUTRIGID_s *rigid;
  int i;
  NUGCUTLOCATORSYS_s *locatorsys;
  int j;
  double dVar2;
  numtx_s mtx;
  uchar newvisibilitystate [20];
  
  dVar2 = (double)current_frame;
  i = 0;
  cutscene = icutscene->cutscene;
  irigidsys = icutscene->irigids;
  rigidsys = cutscene->rigids;
  if (rigidsys->nrigids != 0) {
    do {
      irigid = irigidsys->irigids;
      rigid = rigidsys->rigids + i;
      if (((rigid->flags & 2) != 0) && ((rigid->flags & 4) != 0)) {
        if ((rigid->visibiltyanim != (NUSTATEANIM_s *)0x0) &&
           (j = StateAnimEvaluate(rigid->visibiltyanim,(uchar *)&irigid[i].visibleframeix,
                                  newvisibilitystate,(float)dVar2), j != 0)) {
          if (newvisibilitystate[0] == '\0') {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags & 0x7fffffff);
          }
          else {
            inst = (irigid[i].special.special)->instance;
            flags = (nuinstflags_s)((uint)inst->flags | 0x80000000);
          }
          inst->flags = flags;
        }
        if ((int)(irigid[i].special.special)->instance->flags < 0) {
          NuGCutRigidCalcMtx(rigid,(float)dVar2,&mtx);
          if ((*(uint *)&icutscene->field_0x6c & 0x8000000) != 0) {
            NuMtxMul(&mtx,&mtx,&icutscene->mtx);
          }
          if (NuCutSceneRigidCollisionCheck != (NuCutSceneRigidCollisionCheck *)0x0) {
            (*NuCutSceneRigidCollisionCheck)(rigid,&mtx);
          }
          if (rigid->first_locator != 0xff) {
            j = 0;
            locatorsys = cutscene->locators;
            ilocatorsys = icutscene->ilocators;
            if (rigid->nlocators != '\0') {
              do {
                iVar1 = (uint)rigid->first_locator + j;
                j = j + 1;
                instNuGCutLocatorUpdate
                          (icutscene,locatorsys,ilocatorsys->ilocators + iVar1,
                           locatorsys->locators + iVar1,(float)dVar2,&mtx);
              } while (j < (int)(uint)rigid->nlocators);
            }
          }
        }
      }
      i = i + 1;
    } while (i < (int)(uint)rigidsys->nrigids);
  }
  return;
}


//PS2
static void NuGCutCharSysFixPtrs(struct NUGCUTSCENE_s* cutscene, s32 address_offset)
{
    struct NUGCUTCHAR_s *cutchar;
    struct NUGCUTCHARSYS_s *charsys;
    s32 i;
    
    if (cutscene->chars != NULL) {
        charsys = (struct NUGCUTCHARSYS_s *)((s32) cutscene->chars + address_offset);
        cutscene->chars = charsys;

        ASSIGN_IF_SET(charsys->chars, (struct NUGCUTCHAR_s* )((s32)charsys->chars + address_offset));
        
        if (charsys->chars != NULL)
        {
            for(i = 0; i < charsys->nchars; i++) {
                cutchar = &charsys->chars[i];
                if (cutchar->name != NULL) {
                    cutchar->name = &cutscene->name_table[(s32)cutchar->name] - 1;
                }
                
                if (cutchar->char_anim != NULL) {
                    cutchar->char_anim = NuAnimData2FixPtrs(cutchar->char_anim, address_offset);
                }
                
                if (cutchar->joint_anim != NULL) {
                    cutchar->joint_anim = NuAnimData2FixPtrs(cutchar->joint_anim, address_offset);
                }
                
                if (cutchar->face_anim != NULL) {
                    cutchar->face_anim = NuAnimData2FixPtrs(cutchar->face_anim, address_offset);
                }
            }
        }
    }
    return;
}

typedef void(*NuCutScnFindCharacters)(struct NUGCUTSCENE_s*);
NuCutScnFindCharacters NuCutSceneFindCharacters;

static void NuGCutCharSysFixUp(struct NUGCUTSCENE_s* cutscene) 
{
  if (NuCutSceneFindCharacters != NULL) {
    (*NuCutSceneFindCharacters)(cutscene);
  }
  return;
}


//PS2
static struct instNUGCUTCHARSYS_s * instNuCGutCharSysCreate(struct NUGCUTSCENE_s *cutscene,union variptr_u *buff)
{
    
    struct NUGCUTCHARSYS_s *chars;
    struct instNUGCUTCHARSYS_s *ichars;
    struct NUGCUTCHAR_s *cutchar;
    struct instNUGCUTCHAR_s *icutchar;
    s32 i;
    
    ichars = NULL;
    chars = cutscene->chars;
    if (((chars != NULL) && (chars->nchars != 0)) && (buff != NULL))
    {
        ichars = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        buff->voidptr = &ichars[1];
        
        ichars->ichars = NULL;
        
        icutchar = buff->voidptr = ALIGN_ADDRESS(buff->voidptr, 0x10);
        ichars->ichars = icutchar;
        
        buff->voidptr = buff->voidptr + (chars->nchars * sizeof(struct instNUGCUTCHAR_s));
        memset(ichars->ichars, 0, chars->nchars * sizeof(struct instNUGCUTCHAR_s));
        
        for (i = 0; i < chars->nchars; i++)
        {
            cutchar = &chars->chars[i];
            icutchar = &ichars->ichars[i];
            icutchar->current_animix = -1;
            icutchar->prev_animix = -1;
            if (cutchar->flags & 2)
            {
                if (NuCutSceneCharacterCreateData != NULL)
                {
                    (*NuCutSceneCharacterCreateData)(cutchar, icutchar, buff);
                }
            }
            else {
                icutchar->character = cutchar->character;
            }
        }
    }
    
    return ichars;
}

//PS2
void instNuGCutCharSysRender(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
  struct NUGCUTSCENE_s *cutscene;
  struct instNUGCUTCHARSYS_s *icharsys;
  struct NUGCUTCHARSYS_s *charsys;
  struct instNUGCUTCHAR_s *icutchar;
  struct NUGCUTCHAR_s* cutchar;
  s32 i;
  
  cutscene = icutscene->cutscene;
  icharsys = icutscene->ichars;
  charsys = cutscene->chars;
  if ((NuCutSceneCharacterRender != NULL))
  {
    for(i = 0; i < (s32)charsys->nchars; i++)
    {
      cutchar = &charsys->chars[i];
      icutchar = &icharsys->ichars[i];
      if (icutchar->character != NULL) {
        (*NuCutSceneCharacterRender)
                  (icutscene,cutscene,icutchar,cutchar, current_frame);
      }
    }
  }
  return;
}


//PS2
void NuGCutCharAnimProcess
               (struct NUGCUTCHAR_s *cutchar,float current_frame,struct numtx_s *mtx,s32 *is_visible,
                u32 *animix,float *animrate,float *blendframes)
{
    
    struct nuanimtime_s atime;
    struct NuVec t;
    struct NuVec r;
    struct nuangvec_s rf;
    float fanimix;
    struct nuanimcurve2_s *curves;
    char *curveflags;
    s32 curvesetflags;

    if (cutchar->char_anim != NULL) {
        NuAnimData2CalcTime(cutchar->char_anim, current_frame, &atime);
        curves = cutchar->char_anim->curves;
        curveflags = cutchar->char_anim->curveflags;
        curvesetflags = *cutchar->char_anim->curvesetflags;
        if (cutchar->char_anim->ncurves >= 7) {
            *is_visible = NuAnimCurve2CalcVal(&curves[6], &atime, (int)curveflags[6]);
        }
        else {
            *is_visible = cutchar->flags & 1;
        }
        
        if (animix != 0) {
            if (cutchar->char_anim->ncurves >= 8) {
                fanimix = NuAnimCurve2CalcVal(&curves[7], &atime, (int)curveflags[7]);
                if (fanimix < 0.0f) {
                    *animix = 0xFF;
                } else {
                    *animix = fanimix;
                }
            }
            else {
                *animix = cutchar->animix;
            }
        }
        
        if (*is_visible == 0) {
            return;
        }
        
        if (curvesetflags & 1) {
            r.x = NuAnimCurve2CalcVal(&curves[3], &atime, (int)curveflags[3]);
            r.y = NuAnimCurve2CalcVal(&curves[4], &atime, (int)curveflags[4]);
            r.z = NuAnimCurve2CalcVal(&curves[5], &atime, (int)curveflags[5]);
            rf.z = (s32)(r.z * DEG_TO_FIXED_POINT);
            rf.x = (s32)(r.x * DEG_TO_FIXED_POINT);
            rf.y = (s32)(r.y * DEG_TO_FIXED_POINT);
            NuMtxSetRotateXYZ(mtx, &rf);
        }
        else {
            NuMtxSetIdentity(mtx);
        }
        
        t.x = NuAnimCurve2CalcVal(&curves[0], &atime, (s32)curveflags[0]);
        t.y = NuAnimCurve2CalcVal(&curves[1], &atime, (s32)curveflags[1]);
        t.z = NuAnimCurve2CalcVal(&curves[2], &atime, (s32)curveflags[2]);
        NuMtxTranslate(mtx, &t);
        mtx->_02 = -mtx->_02;
        mtx->_12 = -mtx->_12;
        mtx->_20 = -mtx->_20;
        mtx->_21 = -mtx->_21;
        mtx->_23 = -mtx->_23;
        mtx->_32 = -mtx->_32;
        
        if (animrate != 0) {
            if (cutchar->char_anim->ncurves >= 10)
            {
                *animrate = NuAnimCurve2CalcVal(&curves[9], &atime, (s32)curveflags[9]);
            }
            else {
                *animrate = cutchar->animrate;
            }
        }
        
        if (blendframes == NULL) {
            return;
        }
        
        if (8 < cutchar->char_anim->ncurves) {
            *blendframes = NuAnimCurve2CalcVal(&curves[8], &atime, (s32)curveflags[8]);
            return;
        }
    } 
    else {
        *is_visible = (s32)cutchar->flags & 1;
        if (animix != 0) {
            *animix = cutchar->animix;
        }
        if (is_visible == NULL) {
            return;
        }
        
        *mtx = cutchar->mtx;  
        
        if (animrate != 0) {
            *animrate = cutchar->animrate;
        }
        if (blendframes == NULL) {
            return;
        }
    }
    
    *blendframes = cutchar->blendframes;
    return;
}

//PS2
static void NuGCutTriggerSysFixPtrs(struct NUGCUTSCENE_s *cutscene,s32 address_offset)
{
    struct NUGCUTTRIGGERSYS_s *triggersys;
    struct NUGCUTTRIGGER_s *trigger;
    s32 i;
    
    if (cutscene->triggersys != NULL) {
        triggersys = (struct NUGCUTTRIGGERSYS_s *)((int)cutscene->triggersys + address_offset);
        cutscene->triggersys = triggersys;
        
        ASSIGN_IF_SET(triggersys->triggers, (struct NUGCUTTRIGGER_s *)((s32)triggersys->triggers + address_offset));
        
        if ((triggersys->triggers != NULL))
        {
            for(i = 0; i < triggersys->ntriggers; i++)  {
                trigger = &triggersys->triggers[i];
                
                ASSIGN_IF_SET(trigger->triggername, (s8 *)trigger->triggername + address_offset);
                
                trigger->enableflag1anim = StateAnimFixPtrs(trigger->enableflag1anim, address_offset);
            }
        }
    }
    return;
}

//PS2
static void NuGCutTriggerSysFixUp(struct NUGCUTSCENE_s *cutscene,struct NUTRIGGERSYS_s *triggersys)
{
  s32 i;
  struct NUGCUTTRIGGERSYS_s *cuttriggersys;
  struct NUGCUTTRIGGER_s *cuttrigger;
  s32 j;
  
  cuttriggersys = cutscene->triggersys;
    for (i = 0; i < cuttriggersys->ntriggers; i++){
          cuttrigger = &cuttriggersys->triggers[i];
          for (j = 0; j < triggersys->ntriggers; j++) {
            cuttrigger->ix = -1;
            if (strcasecmp(cuttrigger->triggername,(&triggersys->triggers->triggername)[j * 0xd]) == 0)
            {
              cuttrigger->ix = (short)j;
              break;
            }
          }
          if (cuttrigger->ix == -1) {
            NuErrorProlog("..\\nu2.ps2\\gamelib\\gcutscn.c",0x8a5)
                ("NuGCutTriggerSysFixUp: Unable to find trigger <%s>",cuttrigger->triggername);
          }
    }
    return;
}

//PS2
struct instNUGCUTTRIGGERSYS_s *instNuCGutTriggerSysCreate(struct NUGCUTSCENE_s *cutscene,
struct instNUTRIGGERSYS_s *itriggersys,union variptr_u *buff)
{
  s32 ncuttrigs;
  struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
  
  icuttrigsys = (struct instNUGCUTTRIGGERSYS_s *)(((s32)buff->voidptr + 0xf) & 0xfffffff0);
  ncuttrigs = cutscene->triggersys->ntriggers;
  buff->voidptr = icuttrigsys + 1;
  memset(icuttrigsys,0,8);
  icuttrigsys->itriggersys = itriggersys;
  icuttrigsys->itriggers = (struct instNUGCUTTRIGGER_s *)buff->voidptr;
  buff->voidptr = (void *)((s32)buff->voidptr + ncuttrigs * 4);
  memset(icuttrigsys->itriggers,0,ncuttrigs * 4);
  return icuttrigsys;
}

//PS2
void instNuGCutTriggerSysUpdate(struct instNUGCUTSCENE_s *icutscene,float current_frame)
{
    s32 i;
    struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
    struct NUGCUTTRIGGERSYS_s *cuttrigsys;
    struct NUGCUTTRIGGER_s *cuttrig;
    struct instNUGCUTTRIGGER_s* icuttrig;
    u8 value;
    
    cuttrigsys = icutscene->cutscene->triggersys;
    icuttrigsys = icutscene->itriggersys;
    for (i = 0; i < cuttrigsys->ntriggers; i++)
    {
        icuttrig = icuttrigsys->itriggers;
        cuttrig = &cuttrigsys->triggers[i];
        if (cuttrig->enableflag1anim != NULL) {
            if (StateAnimEvaluate(cuttrig->enableflag1anim, (u8 *)(&icuttrigsys->itriggers[i]), &value, current_frame) != 0) {
                if (value != '\0') {
                    icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags = icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags | 1;
                }
                else {
                    icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags = icuttrigsys->itriggersys->itriggers[cuttrig->ix].enableflags & 0xfe;
                }
            }
        }
    }
    return;
}


//PS2
static void instNuGCutTriggerSysStart(struct instNUGCUTSCENE_s *icutscene)
{
  struct instNUGCUTTRIGGERSYS_s *icuttrigsys;
  struct NUGCUTTRIGGERSYS_s *cuttrigsys;
  struct instNUGCUTTRIGGER_s *icuttrig;
  s32 i;
  
  icuttrigsys = icutscene->itriggersys;
  cuttrigsys = icutscene->cutscene->triggersys;
    for (i = 0; i < cuttrigsys->ntriggers; i++) //while( i < cuttrigsys->ntriggers)
    {
      icuttrig = &icuttrigsys->itriggers[i];
      icuttrig->next_ix = '\0';
    }
  return;
}

void CalculateLocatorDirection(locdir ldir,numtx_s *mtx,nuvec_s *dir)

{
  bool c;
  float calc_rots;
  
  c = false;
  if (ldir == LOCATOR_NEGX) {
    calc_rots = 1.0;
LAB_8008ad5c:
    c = true;
    dir->z = 0.0;
    dir->x = calc_rots;
    dir->y = 0.0;
  }
  else {
    if (LOCATOR_NEGX < ldir) {
      if (ldir == LOCATOR_NEGY) {
        dir->z = 0.0;
        dir->y = 1.0;
      }
      else if (ldir < LOCATOR_NEGY) {
        dir->z = 0.0;
        dir->y = -1.0;
      }
      else {
        if (ldir == LOCATOR_Z) {
          calc_rots = 1.0;
        }
        else {
          if (ldir != LOCATOR_NEGZ) goto LAB_8008ade4;
          calc_rots = -1.0;
        }
        dir->y = 0.0;
        dir->z = calc_rots;
      }
      c = true;
      dir->x = 0.0;
      goto LAB_8008ade4;
    }
    if (ldir == LOCATOR_DOWN) {
      calc_rots = -1.0;
    }
    else {
      if (LOCATOR_DOWN < ldir) {
        calc_rots = -1.0;
        goto LAB_8008ad5c;
      }
      calc_rots = 1.0;
    }
    dir->x = 0.0;
    dir->y = calc_rots;
    dir->z = 0.0;
  }
LAB_8008ade4:
  if (c) {
    NuVecMtxRotate(dir,dir,mtx);
  }
  return;
}

//PS2
static int StateAnimEvaluate(struct NUSTATEANIM_s* stateanim, u8* lastix, u8* newstate, float frame)
{
    s32 statechanged;
    u8 ix;
    
    statechanged = 0;
    if (*lastix < stateanim->nchanges) {
        do 
        {
            if (stateanim->frames[*lastix] <= frame) {
                *newstate = stateanim->states[*lastix];
                statechanged = 1;
                *lastix += 1;
            } else {
                break;
            }
        } while (*lastix < stateanim->nchanges);
    }
    
    if (statechanged == 0 && *lastix > 0) {
        do
        {
            if (stateanim->frames[*lastix-1] > frame) {
                *lastix -= 1;
                *newstate = stateanim->states[*lastix];
                statechanged = 1;
            } else {
                break;
            }
        } while (*lastix > 0);
    }
    return statechanged;
}

//PS2
struct NUSTATEANIM_s * StateAnimFixPtrs(struct NUSTATEANIM_s *sanim,s32 address_offset)
{
  float *pfVar1;
  u8 *puVar2;
  struct NUSTATEANIM_s *rv;
  
  rv = NULL;
  if (sanim != NULL) {
    rv = (struct NUSTATEANIM_s *)((s32)&sanim->nchanges + address_offset);
  }
  if (rv != NULL) {
    pfVar1 = (float *)((s32)rv->frames + address_offset);
    puVar2 = rv->states + address_offset;
    if (rv->frames == NULL) {
      pfVar1 = NULL;
    }
    if (rv->states == NULL) {
      puVar2 = NULL;
    }
    rv->frames = pfVar1;
    rv->states = puVar2;
  }
  return rv;
}

typedef void(*NuCutScnCharacterRndr)(struct instNUGCUTSCENE_s*, struct NUGCUTSCENE_s*, 
                                    struct instNUGCUTCHAR_s*, struct NUGCUTCHAR_s*, float);
NuCutScnCharacterRndr NuCutSceneCharacterRender;

void NuSetCutSceneCharacterRenderFn(NuCutScnCharacterRndr fn)
{
    NuCutSceneCharacterRender = fn;
    return;
}

typedef void(*NuCutScnFindCharacters)(struct NUGCUTSCENE_s*);
NuCutScnFindCharacters NuCutSceneFindCharacters;

void NuSetCutSceneFindCharactersFn(NuCutScnFindCharacters fn)
{
  NuCutSceneFindCharacters = fn;
  return;
}