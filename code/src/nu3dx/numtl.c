#include "numtl.h"

struct nusysmtl_s* smlist;
struct numtl_s* numtl_white;
s32 nummtls = 0;
s32 wateritem_cnt;
s32 stenitem_cnt;
s32 otitem_cnt;
s32 faceonmtl_cnt;
s32 faceonitem_cnt;
s32 dynamic_glass_item_cnt;
f32 sinetime_246;
struct nustenitem_s stenitem[512];
struct nufaceonitem_s* faceonmtllist[50];
struct nufaceonitem_s faceonitem[512];
struct nuotitem_s dynamic_glass_items[64];
struct nuwateritem_s wateritem[512];
s32 IsStencil = 0;

enum nustencilmode_e stencil_mode = NUSTENCIL_NOSTENCIL;

//NGC MATCH
static void NuMtlCreateDefault(void) {
    struct numtl_s* mtl;

    mtl = NuMtlCreate(1);
    
    numtl_white = mtl;
    
    mtl->attrib.cull = 2;
    mtl->attrib.zmode = 0;
    mtl->attrib.alpha = 0;
    
    mtl->diffuse.r = 1.0f;
    mtl->diffuse.g = 1.0f;
    mtl->diffuse.b = 1.0f;
    mtl->alpha = 1.0f;
    
    mtl->K = 0;
    mtl->L = 0;
    NuMtlUpdate(numtl_white);
}

//NGC MATCH
void NuMtlInit(void) {
  if (initialised != 0) {
    NuMtlClose();
  }
  smlist = NULL;
  initialised = 1;
  NuMtlCreateDefault();
  return;
}

void NuMtlClose(void)

{
  struct numtl_s *list;
  struct numtl_s *next;

  if (initialised != 0) {
    list = &smlist->mtl;
    if (smlist != NULL) {
      do {
        next = list->next;
        NuMtlDestroy(list);
        list = next;
      } while (next != NULL);
    }
    initialised = 0;
  }
  return;
}

//NGC MATCH
struct numtl_s* NuMtlCreate(s32 mode) {
  struct nusysmtl_s *sm;
  struct nusysmtl_s *ret;
  s32 i;
    char pad [3];

    sm = NULL;
  if (0 < mode) { 
    for (i = mode; i != 0; i--) {
      ret = (struct nusysmtl_s *)malloc_x(0x6c);
      memset(ret,0,0x6c);
      (ret->mtl).alpha = 1.0f;

      //(ret->mtl).attrib = ((ret->mtl).attrib & 0xcfffffff | 0x20000000);
      (ret->mtl).attrib.filter = 2;

      NuMtlInsert(ret);
      (ret->mtl).next = &sm->mtl;
      nummtls++;
      sm = ret;
    }
  }
  return &ret->mtl;
}


//NGC MATCH
void NuMtlDestroy(struct numtl_s* mtl) {
    struct nusysmtl_s* sm = (struct nusysmtl_s*)mtl; // r31
    struct nusysmtl_s* next; //unused?

    if (sm->mtl.tid != 0) {
        NuTexUnRef(sm->mtl.tid);
    }
    if (sm->geom2d != NULL) {
        NuGeomDestroy(sm->geom2d->geom);
    }
    if (sm->geom3d != NULL) {
        NuGeomDestroy(sm->geom3d->geom);
    }
    NuMtlRemove(sm);
    free_x(mtl);
    nummtls -= 1;
    return;
}




int NuMtlNum(void)

{
  return nummtls;
}

struct numtl_s * NuMtlGet(int id)
{
  struct nusysmtl_s *ret;

  ret = NULL;
  if ((id < nummtls) && (ret = smlist, 0 < id)) {
    do {
      ret = ret->next;
      id = id + -1;
    } while (id != 0);
  }
  return &ret->mtl;
}

//NGC MATCH
struct numtl_s * NuMtlRead(s32 fh) {
  struct numtl_s *mtl;

  mtl = NuMtlCreate(1);
  NuFileRead(fh,mtl,0x54);
  return mtl;
}

//NGC MATCH
static void NuMtlInsert(struct nusysmtl_s* sm) {
  s32 sid;
  s32 lsid;
  struct nusysmtl_s *last;
  struct nusysmtl_s *list;

  last = NULL;
  list = smlist;
    
  sid = (sm->mtl).alpha_sort * 0x10000 + (sm->mtl).tid;
  sm->last = sm->next = NULL;

  for(; list != NULL; last = list, list = list->next) {      
    lsid = (list->mtl).alpha_sort * 0x10000 + (list->mtl).tid;
    if(lsid <= sid) 
        break;
  }         

  if (last != NULL) {
    sm->last = last;
    sm->next = last->next;
    last->next = sm;
  }
  else {
    sm->next = smlist;
    smlist = sm;
  }
  if (sm->next != NULL) {
    sm->next->last = sm;
  }

  return;
}

//NGC MATCH
void NuMtlRemove(struct nusysmtl_s *sm) {
  if (sm->last != NULL) {
    sm->last->next = sm->next;
  }
  else {

    smlist = sm->next;
  }
  if (sm->next == NULL) {
    return;
  }
  sm->next->last = sm->last;
  return;
}

void NuMtlUpdate(struct numtl_s *mtl)

{
  return;
}

//NGC MATCH
void NuMtlSetStencilRender(enum nustencilmode_e mode) {
  stencil_mode = mode;
  return;
}




void NuMtlAddRndrItem(struct nusysmtl_s *mtl,struct nugeomitem_s *item)

{
  struct nustenitem_s *steni;
  struct nuwateritem_s *wateri;
  float fVar1;
  bool check;
  int cnt;
  short hShader;
  enum nustencilmode_e stmode;
  int watercnt;

  stmode = stencil_mode;
  watercnt = wateritem_cnt;
  cnt = stenitem_cnt;
  if (stencil_mode == NUSTENCIL_NOSTENCIL) {
    hShader = item->hShader;
    if ((hShader == 3) || (hShader == 0x21)) {
      NuMtlAddGlassItem(&mtl->mtl,&item->hdr);
    }
    else {
      if ((hShader != 1) && (hShader != 0x1b)) {
        /*if (((u32)(mtl->mtl).attrib >> 0x1e == 0) && (item->geom->mtls->fxid != '\x04')) {
          (item->hdr).next = mtl->rndrlist;
          mtl->rndrlist = &item->hdr;
          return;
        }*/
        cnt = otitem_cnt + -1;
        otitem_cnt = cnt;
        otitem[cnt].mtl = mtl;
        otitem[cnt].hdr = &item->hdr;
        //fVar1 = NuRndrItemDist(&item->hdr);
        otitem[cnt].dist = fVar1;
        NuMtlOTInsert(otitem + cnt);
        return;
      }
      if (wateritem_cnt < 0x200) {
        wateri = wateritem + wateritem_cnt;
        wateritem[wateritem_cnt].hdr = &item->hdr;
        wateritem[watercnt].mtl = mtl;
      }
      else {
        //error = NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x2a7);
        //(*error)("NuMtlAddRndrItem: Exceeded maximum number of water items in render queue!");
      }
      if (wateritem_cnt != 0) {
        wateritem[wateritem_cnt + -1].next = wateri;
      }
      wateritem_cnt = wateritem_cnt + 1;
    }
  }
  else {
    check = stenitem_cnt != 0;
    steni = stenitem + stenitem_cnt;
    stenitem[stenitem_cnt].hdr = &item->hdr;
    stenitem[cnt].mtl = mtl;
    stenitem[cnt].type = stmode;
    if (check) {
      stenitem[cnt + -1].next = steni;
    }
    stenitem_cnt = stenitem_cnt + 1;
  }
  return;
}


//NGC MATCH
void NuMtlAddFaceonItem(struct numtl_s *mtl,struct nurndritem_s *item) {
    struct nusysmtl_s* sm; // unused
    struct nufaceonitem_s *faceoni;
    s32 i;
    
    faceoni = &faceonitem[--faceonitem_cnt];
    faceoni->hdr = item;
    faceoni->mtl = (struct nusysmtl_s*)mtl;
        for(i = 0; faceonmtllist[i] && faceonmtllist[i]->mtl != faceoni->mtl; i++) {
            if (i > 0x31) {
                NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x2f5)("Out of room for face-ons in the list");
               // i++;
            }
            //if (faceonmtllist[++i] == NULL) goto LAB_800b3084;
        }
        if (faceonmtllist[i] == NULL) {
            faceonmtllist[i] = faceoni;
            faceoni->next = NULL;
            faceonmtl_cnt++;
        } else {
            faceoni->next = faceonmtllist[i];
            faceonmtllist[i] = faceoni;
        }
    return;
}


//NGC MATCH
static void NuMtlOTInsert(struct nuotitem_s *oti) {
  s32 ix;

  ix = (s32)(oti->mtl->mtl).alpha_sort;
  if (0x100 < ix) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x3e0)("assert");
  }
  if (ot[ix] != NULL) {
    oti->next = ot[ix];
    ot[ix] = oti;
  }
  else {
    ot[ix] = oti;
    oti->next = NULL;
  }
  return;
}

//NGC MATCH
void NuMtlGet2dBuffer(struct numtl_s *mtl,enum nuprimtype_e pt,struct nugeom_s **geomptr,struct nuprim_s **primptr, union variptr_u *ptr,union variptr_u *end) {
  struct nugeom_s *g;
  struct nuprim_s *prim;
    struct nusysmtl_s* sm;

  sm = (struct nusysmtl_s *)mtl;
  NuMemSetExternal(ptr,end);
  if (sm->geom2d == NULL) {
    sm->geom2d = (struct nugeomitem_s *)NuMemAlloc(0x24);
    memset(sm->geom2d,0,0x24);
    (sm->geom2d->hdr).type = NURNDRITEM_GEOM2D;
    (sm->geom2d->hdr).flags = 0;
    g = NuGeomCreate();
    sm->geom2d->geom = g;
    NuGeomCreateVB(sm->geom2d->geom,0x400,NUVT_TLTC1,1);
    sm->geom2d->geom->vtxcnt = 0;
  }
  g = sm->geom2d->geom;
  prim = g->prims;
  if (prim != NULL) {
    while (prim->type != pt) {
      prim = prim->next;
      if (prim == NULL) goto LAB_800b3234;
    }
    if (prim != NULL) goto LAB_800b325c;
  }
LAB_800b3234:
  prim = NuPrimCreate(0x400,pt);
  NuGeomAddPrim(g,prim);
  prim->cnt = 0;
LAB_800b325c:
  if (g != NULL) {
    *geomptr = g;
  }
  if (prim != NULL) {
    *primptr = prim;
  }
  NuMemSetExternal(NULL,NULL);
  return;
}

//NGC MATCH
void NuMtlGet3dBuffer(struct numtl_s *mtl,enum nuprimtype_e pt,struct nugeom_s **geomptr,struct nuprim_s **primptr, union variptr_u *ptr,union variptr_u *end) {
  struct nugeom_s *geom;
  struct nuprim_s *prim;
    struct nusysmtl_s* sm; //

    sm = (struct nusysmtl_s *)mtl;

  NuMemSetExternal(ptr,end);
  if (sm->geom3d == NULL) {
    sm->geom3d = NuMemAlloc(0x24);;
    memset(sm->geom3d,0,0x24);
    (sm->geom3d->hdr).type = NURNDRITEM_GEOM3D;
    (sm->geom3d->hdr).flags = 0;
    geom = NuGeomCreate();
    sm->geom3d->geom = geom;
    NuGeomCreateVB(sm->geom3d->geom,0xc00,NUVT_TC1,1);
    sm->geom3d->geom->vtxcnt = 0;
  }
  geom = sm->geom3d->geom;
  prim = geom->prims;
  if (prim != NULL) {
    while (prim->type != pt) {
      prim = prim->next;
      if (prim == NULL) goto LAB_800b336c;
    }
    if (prim != NULL) goto LAB_800b3394;
  }
LAB_800b336c:
  prim = NuPrimCreate(0xc00,pt);
  NuGeomAddPrim(geom,prim);
  prim->cnt = 0;
LAB_800b3394:
  if (geom != NULL) {
    *geomptr = geom;
  }
  if (prim != NULL) {
    *primptr = prim;
  }
  NuMemSetExternal(NULL,NULL);
  return;
}

//NGC MATCH
void NuMtlRender(void) {
  DBTimerStart(0x13);
  NuMtlRender3d();
  DBTimerEnd(0x13);
  DBTimerStart(0x12);
  DBTimerEnd(0x12);
  DBTimerStart(0x11);
  DBTimerEnd(0x11);
  DBTimerStart(0x10);
  NuMtlRenderDynamic2d3d();
  DBTimerEnd(0x10);
  DBTimerStart(0xf);
  NuMtlRenderOT(0,10);
  DBTimerEnd(0xf);
  DBTimerStart(0xe);
  NuMtlRenderFaceOn();
  DBTimerEnd(0xe);
  DBTimerStart(0xc);
  NuMtlRenderSten();
  DBTimerEnd(0xc);
  DBTimerStart(0xb);
  NuMtlRenderGlass();
  DBTimerEnd(0xb);
  DBTimerStart(10);
  NuMtlRenderWater();
  DBTimerEnd(10);
  DBTimerStart(9);
  NuMtlRenderOT(0xb,0x100);
  DBTimerEnd(9);
  DBTimerStart(8);
  DrawStencilShadowQuad();
  DBTimerEnd(8);
  DBTimerStart(7);
  NuMtlRenderUpd();
  DBTimerEnd(7);
  return;
}

//NGC MATCH
static void NuMtlRender3d(void) {
  struct nusysmtl_s *sm;
  struct nurndritem_s *ri;

  GS_SetAlphaCompare(7,0);
  for (sm = smlist; sm != NULL; sm = sm->next) {
    if (((sm->mtl).L == '\0') && (sm->rndrlist != NULL)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      for (ri = sm->rndrlist; ri != NULL; ri = ri->next) {
        NuRndrItem(ri);
      }
      sm->rndrlist = NULL;
    }
  }
  GS_SetAlphaCompare(7,0);
  return;
}

//NGC MATCH
static void NuMtlRenderDynamic2d3d(void) {
  struct nuprim_s *prim;
  struct nusysmtl_s *sm;
  struct nugeomitem_s *item;

  GS_SetAlphaCompare(7,0);
  sm = smlist;
  GS_SetZCompare(1,0,GX_LEQUAL);
  for (; sm != NULL; sm = sm->next) {
    if ((sm->geom3d != NULL) && (sm->geom3d->geom->vtxcnt != 0)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      item = sm->geom3d;
      NuRndrItem(&item->hdr);
      item->geom->vtxcnt = 0;
      for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
        prim->cnt = 0;
      }
    }
    if ((sm->geom2d != NULL) && (sm->geom2d->geom->vtxcnt != 0)) {
      NuTexSetTexture(0,(sm->mtl).tid);
      NuMtlSetRenderStates(&sm->mtl);
      NuTexSetTextureStates(&sm->mtl);
      item = sm->geom2d;
      GS_EnableLighting(0);
      GS_SetZCompare(1,0,GX_LEQUAL);
      GS_SetAlphaCompare(7,0);
      NuRndrItem(&item->hdr);
      item->geom->vtxcnt = 0;
      for (prim = item->geom->prim; prim != NULL; prim = prim->next) {
        prim->cnt = 0;
      }
    }
  }
  return;
}

//NGC MATCH
static void NuMtlRenderOT(s32 begin,s32 end) {
  struct nuotitem_s *oti;
  s32 i;

  GS_SetAlphaCompare(7,0);
    for (i = begin; i <= end; i++) {
      for (oti = ot[i]; oti != NULL; oti = oti->next) {
        if ((oti->mtl->mtl).L == '\0') {
          NuTexSetTexture(0,(oti->mtl->mtl).tid);
          NuMtlSetRenderStates(&oti->mtl->mtl);
          NuTexSetTextureStates(&oti->mtl->mtl);
          NuRndrItem(oti->hdr);
        }
        else {
          NuTexSetTexture(0,(oti->mtl->mtl).tid);
          NuMtlSetRenderStates(&oti->mtl->mtl);
          NuTexSetTextureStates(&oti->mtl->mtl);
          GS_SetZCompare(1,1,GX_LEQUAL);
          GS_SetAlphaCompare(3,0xf7);
          NuRndrItem(oti->hdr);
        }
      }
      ot[i] = NULL;
    }
  return;
}

//NGC MATCH
static void NuMtlRenderFaceOn() {
    s32 processed_count = 0;
    struct nufaceonitem_s* cur_list;
    struct numtx_s rotation;
    struct numtx_s itemmtx;
    struct numtx_s faceonmtx;
    float w;
    float h;
    float r;
    float g;
    float b;
    float a;
    struct NuVec vertex_transformed;
    struct NuVec faceon_point_rotated;
    struct NuVec faceon_world_pos;
    struct NuVec vtxpos;
    struct nugeomitem_s* list_geomitem;
    struct NuFaceOnGeom* fop;
    s32 faceon_count;
    struct nufaceon_s* faceon_list;
    struct nugeomitem_s* item;
    char stack_padding[0x34];
    
    ResetShaders();
    SetVertexShader(0x142);
    GS_LoadWorldMatrixIdentity();
    
    for (; processed_count < faceonmtl_cnt; processed_count++) {
        cur_list = faceonmtllist[processed_count];
        
        if (cur_list->mtl->mtl.L == 0) {
            NuTexSetTexture(0, cur_list->mtl->mtl.tid);
            NuMtlSetRenderStates(&cur_list->mtl->mtl);
            NuTexSetTextureStates(&cur_list->mtl->mtl);
            GS_SetAlphaCompare(4, 0);
            GS_SetZCompare(1, 1, 3);
        } else {
            NuTexSetTexture(0, cur_list->mtl->mtl.tid);
            NuMtlSetRenderStates(&cur_list->mtl->mtl);
            NuTexSetTextureStates(&cur_list->mtl->mtl);
            GS_SetZCompare(1, 0, 3);
            GS_SetAlphaCompare(3, 0xf7);
        }

         //nufaceonitem_s::hdr is probably a nugeomitem_s*
         //TODO: remove this cast
        itemmtx = *((struct nugeomitem_s*)cur_list->hdr)->mtx;

        for (; cur_list != NULL; cur_list = cur_list->next) {
            item = ((struct nugeomitem_s*)cur_list->hdr);
            // get rotation matrix and zero out translation
            rotation = *item->mtx;
            rotation._30 = 0.0f;
            rotation._31 = 0.0f;
            rotation._32 = 0.0f;
            list_geomitem = (struct nugeomitem_s*)cur_list->hdr;
            fop = (struct NuFaceOnGeom*)list_geomitem->geom;
        
        
            for (; fop != NULL; fop = fop->next) {
                int i;
                faceon_count = fop->nfaceons;
                faceon_list = fop->faceons;
                GS_DrawQuadListBeginBlock(faceon_count * 4, 0);
            
                for (i = 0; i < faceon_count; i++, faceon_list += 1) {
                    w = faceon_list->width * 0.5f;  //30
                    h = faceon_list->height * 0.5f; //31
                    
                    
                    NuVecMtxRotate(&faceon_point_rotated, &faceon_list->point, &rotation);
                    
                    NuVecAdd(&faceon_world_pos, &faceon_point_rotated, 
                        (struct NuVec*)&item->mtx->_30);
                    
                    
                    NuMtxCalcCheapFaceOn(&faceonmtx, &faceon_world_pos);
                    
                    vtxpos.x = -w;
                    vtxpos.y = h;
                    vtxpos.z = 0.0f;
                    
                    NuVecMtxTransform(&vertex_transformed, &vtxpos, &faceonmtx);
                    
                    //Color is defined ARGB?
                    GS_SetQuadListRGBA((faceon_list->colour >> 16) & 0xff,
                             (faceon_list->colour >> 8) & 0xff,
                             faceon_list->colour & 0xff,
                             (faceon_list->colour >> 24) & 0xff);
                    GS_DrawQuadListSetVert(&vertex_transformed, 0.0f, 0.0f);
                    
                    vtxpos.x = w;
                    vtxpos.y = h;
                    vtxpos.z = 0.0f;
                    NuVecMtxTransform(&vertex_transformed, &vtxpos, &faceonmtx);
                    GS_DrawQuadListSetVert(&vertex_transformed, 1.0f, 0.0f);
                    
                    vtxpos.x = w;
                    vtxpos.y = -h;
                    vtxpos.z = 0.0f;
                    NuVecMtxTransform(&vertex_transformed, &vtxpos, &faceonmtx);
                    GS_DrawQuadListSetVert(&vertex_transformed, 1.0f, 1.0f);
                    
                    vtxpos.x = -w;
                    vtxpos.y = -h;
                    vtxpos.z = 0.0f;
                    NuVecMtxTransform(&vertex_transformed, &vtxpos, &faceonmtx);        
                    
                    GS_DrawQuadListSetVert(&vertex_transformed, 0.0f, 1.0f);
                }   
                GS_DrawQuadListEndBlock();
            }
        }
    
        faceonmtllist[processed_count] = 0;
    }
}


//NGC MATCH
static void NuMtlRenderGlass(void) {
  s32 i;
  struct nuotitem_s *oti;

  oti = dynamic_glass_items;
  i = 0;
  if (dynamic_glass_item_cnt > 0) {
    NudxFw_MakeBackBufferCopy(force_glass_screencopy_enable);
      for (; i < dynamic_glass_item_cnt; i++) {
        NuTexSetTexture(0,(oti->mtl->mtl).tid);
        NuMtlSetRenderStates(&oti->mtl->mtl);
        NuTexSetTextureStates(&oti->mtl->mtl);
        NuRndrItem(oti->hdr);
        oti = oti->next;
      }
    NuMtlClearGlassList();
  }
  return;
}

//NGC MATCH
static void NuMtlRenderWater(void) {
  s32 i;
  struct nustenitem_s *wateri;

  wateri = (struct nustenitem_s *)wateritem;
  if (0 < wateritem_cnt) {
    i = 0;
    NudxFw_MakeBackBufferCopy(1);
      for (; i < wateritem_cnt; i++) {
        NuTexSetTexture(0,(wateri->mtl->mtl).tid);
        NuMtlSetRenderStates(&wateri->mtl->mtl);
        NuTexSetTextureStates(&wateri->mtl->mtl);
        NuRndrItem(wateri->hdr);
        wateri = wateri->next;
      }
  }
  wateritem_cnt = 0;
  return;
}


//NGC MATCH
static void NuMtlRenderSten(void) {
  struct nustenitem_s *steni;
  s32 i;

   steni = stenitem;
  GS_SetAlphaCompare(4,0);
  i = 0;
  NudxFw_SetRenderState(0x7d,1);
  NudxFw_SetRenderState(0x46,0x207);
  NudxFw_SetRenderState(0x44,0x1e00);
  NudxFw_SetRenderState(0x7e,0x1e00);
  NudxFw_SetRenderState(0x47,1);
  NudxFw_SetRenderState(0x48,0xffffffff);
  NudxFw_SetRenderState(0x49,0xffffffff);
  IsStencil = 1;
    for (; i < stenitem_cnt; i++) {
          if (steni->type == 1) {
            GS_EnableLighting(0);
            GS_SetMaterialSourceEmissive(1);
            NuTexSetTexture(0,0);
            GS_SetBlendSrc(1,1,0);
          }
          else {
            NuTexSetTexture(0,(steni->mtl->mtl).tid);
            NuMtlSetRenderStates(&steni->mtl->mtl);
            NuTexSetTextureStates(&steni->mtl->mtl);
          }
          if (steni->type == 1) {
            NudxFw_SetRenderState(0x45,0x1e01);
            NuRndrItem(steni->hdr);
          }
          steni = steni->next;
    }
  stenitem_cnt = 0;
  GS_EnableLighting(1);
  NudxFw_SetRenderState(0x7d,0);
  IsStencil = 0;
  return;
}

//NGC MATCH
static void NuMtlAddGlassItem(struct numtl_s *mtl,struct nurndritem_s *item) {
  struct nuotitem_s *tail;

  if (0x40 < dynamic_glass_item_cnt) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c",0x980)("Too many dynamic glass items");
  }
  if (dynamic_glass_item_cnt < 0x40) {
    tail = &dynamic_glass_items[dynamic_glass_item_cnt];
    tail->hdr = item;
    tail->mtl = (struct nusysmtl_s *)mtl;
    tail->dist = 0.0f;
    if (0 < dynamic_glass_item_cnt) {
      dynamic_glass_items[dynamic_glass_item_cnt + -1].next = tail;
    }
    dynamic_glass_item_cnt++;
  }
  return;
}


//NGC MATCH
static void NuMtlClearGlassList(void) {
  memset(dynamic_glass_items,0,0x400);
  dynamic_glass_item_cnt = 0;
  return;
}

//NGC MATCH
static void NuMtlRenderUpd(void) {
  faceonitem_cnt = 0x200;
  faceonmtl_cnt = 0;
  otitem_cnt = 0x7e8;
  NuTexSetTexture(0,0);
  return;
}

//NGC MATCH
void NuMtlSetRenderStates(struct numtl_s *mtl) {
    struct _D3DMATERIAL8 d3dmtl;
    
    d3dmtl.Diffuse.r = (mtl->diffuse).r;
    d3dmtl.Diffuse.g = (mtl->diffuse).g;
    d3dmtl.Diffuse.b = (mtl->diffuse).b;
    d3dmtl.Diffuse.a = mtl->alpha;
    
    d3dmtl.Ambient.r = (mtl->ambient).r;
    d3dmtl.Ambient.g = (mtl->ambient).g;
    d3dmtl.Ambient.b = (mtl->ambient).b;
    d3dmtl.Ambient.a = d3dmtl.Diffuse.a;
    
    if (mtl->attrib.lighting == 2) {
        d3dmtl.Emissive.r = mtl->diffuse.r;
        d3dmtl.Emissive.g = mtl->diffuse.g;
        d3dmtl.Emissive.b = mtl->diffuse.b;
        d3dmtl.Emissive.a = d3dmtl.Diffuse.a;
    } else {
        d3dmtl.Emissive.r = mtl->ambient.r;
        d3dmtl.Emissive.g = mtl->ambient.g;
        d3dmtl.Emissive.b = mtl->ambient.b;
        d3dmtl.Emissive.a = d3dmtl.Diffuse.a;
    }
    d3dmtl.Power = mtl->power;
    GS_SetMaterial(&d3dmtl);
    
    if (mtl->attrib.alpha == 0) {
        GS_SetBlendSrc(1,1,0);
        GS_SetAlphaCompare(7,0);
    } else if (mtl->attrib.alpha == 1) {
        GS_SetBlendSrc(1,4,5);
        GS_SetAlphaCompare(4,0);
    } else if (mtl->attrib.alpha == 2) {
        GS_SetBlendSrc(1,4,1);
        GS_SetAlphaCompare(4,0);
    } else if (mtl->attrib.alpha == 3) {
        GS_SetBlendSrc(1,0,3);
        GS_SetAlphaCompare(4,0);
    }
    
    GS_SetAlphaCompare(4,0);
    NudxFw_SetRenderState(0x80,0);

    if (mtl->attrib.zmode == 0) {
        GS_SetZCompare(1,1,GX_LEQUAL);
    }
    
    if (mtl->attrib.zmode == 1) {
        GS_SetZCompare(1,0,GX_LEQUAL);
    }
    
    if (mtl->attrib.zmode == 2) {
        GS_SetZCompare(1,1,GX_ALWAYS);
    }
    
    if (mtl->attrib.zmode == 3) {
        GS_SetZCompare(0,0,GX_ALWAYS);
    }
    
    IsObjLit = 0;
    if (mtl->attrib.lighting == 0) {
        GS_EnableLighting(1);
        GS_EnableSpecular(0);
        IsObjLit = 1;
    }
    
    if (mtl->attrib.lighting == 1) {
        GS_EnableLighting(1);
        GS_EnableSpecular(1);
        IsObjLit = 2;
    }
    
    if (mtl->attrib.lighting == 2) {
        GS_EnableLighting(0);
    }
    
    if (mtl->attrib.colour == 0) {
        GS_EnableColorVertex(0);
        GS_SetMaterialSourceAmbient(0);
        GS_SetMaterialSourceEmissive(0);
    }
    
    if (mtl->attrib.colour == 1) {
        GS_EnableColorVertex(1);
        GS_SetMaterialSourceAmbient(1);
        if (mtl->attrib.lighting == 2) {
            GS_SetMaterialSourceEmissive(1);
        } else {
            GS_SetMaterialSourceEmissive(0);
        }
    }
    return;
}


void NuMtlAnimate(float timestep)

{
  struct nusysmtl_s *sm;
  u32 unk;

  sinetime_246 = sinetime_246 + timestep;
  sm = smlist;
  if (smlist == NULL) {
    return;
  }
  do {
    unk = *(u32 *)&(sm->mtl).K;
    if ((unk & 0xff) != 0) {
      if ((unk & 0xf0) == 0x20) {
        (sm->mtl).du = (sm->mtl).su * timestep;
      }
      if ((*(u32 *)&(sm->mtl).K & 0xf) == 2) {
        (sm->mtl).dv = (sm->mtl).sv * timestep;
      }
    }
    sm = sm->next;
  } while (sm != NULL);
  return;
}

/*

void NuMtlUVAnimation(struct nugobj_s *gobj)

{
  s32 numvts;
  struct numtl_s *mtls;
  struct nuvtx_tc1_s* vt1;
  struct nuvtx_tc1_s *vt3;
  struct nuvtx_tc1_s *vt1;
  struct nuvtx_tc1_s *vt2;
  struct nugeom_s *geoms;
  f32 du;
  f32 dv;
  f32 fVar1;


  geoms = gobj->geom;
  if (geoms == NULL) {
    return;
  }
  do {
    mtls = geoms->mtls;
    if ((*(u32 *)&mtls->K & 0xf0) == 0x20) {
      du = mtls->du;
LAB_800b4334:
      dv = 0.0;
      if ((*(u32 *)&mtls->K & 0xf) == 2) {
        dv = mtls->dv;
      }
      if ((dv == 0.0) || (du == 0.0)) {
        if ((dv == 0.0) && (du != 0.0)) {
          if (geoms->vertex_type == NUVT_TC1) {
            numvts = geoms->vtxcount;
            vt1 = (struct nuvtx_tc1_s *)geoms->hVB;
            if (0 < numvts) {
              do {
                vt1->tc[0] = vt1->tc[0] + du + du;
                vt1 = vt1 + 1;
                numvts = numvts + -1;
              } while (numvts != 0);
            }
          }
        }
        else if (((dv != 0.0) && (du == 0.0)) && (geoms->vertex_type == NUVT_TC1)) {
          numvts = geoms->vtxcount;
          vt2 = (struct nuvtx_tc1_s *)geoms->hVB;
          if (0 < numvts) {
            do {
              vt2->tc[1] = vt2->tc[1] + dv + dv;
              vt2 = vt2 + 1;
              numvts = numvts + -1;
            } while (numvts != 0);
          }
        }
      }
      else if (geoms->vertex_type == NUVT_TC1) {
        numvts = geoms->vtxcount;
        vt3 = (struct nuvtx_tc1_s *)geoms->hVB;
        if (0 < numvts) {
          do {
            fVar1 = vt3->tc[1];
            vt3->tc[0] = vt3->tc[0] + du + du;
            vt3->tc[1] = fVar1 + dv + dv;
            vt3 = vt3 + 1;
            numvts = numvts + -1;
          } while (numvts != 0);
        }
      }
    }
    else {
      du = 0.0;
      if ((*(u32*)&mtls->K & 0xf) == 2) goto LAB_800b4334;
    }
    geoms = geoms->next;
    if (geoms == NULL) {
      return;
    }
  } while( true );
}
*/

//NGC MATCH
void NuMtlClearOt(void) {
    memset(&ot, 0, 0x404);
}
