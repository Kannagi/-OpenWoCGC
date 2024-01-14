//MATCH NGC
void InitProbe(void) {
  probecol = 0;
  probeon = 0;
  probey = 0;
  probetime = 0;
  proberot.x = 0;
  proberot.y = 0;
  proberot.z = 0;
  return;
}

//MATCH NGC
void ResetTempCharacter(s32 character,s32 action) {
  temp_character = character;
  temp_character_action = action;
  ResetAnimPacket(&TempAnim,action);
  ResetLights(&TempLights);
  return;
}

//MATCH NGC
void ResetTempCharacter2(s32 character,s32 action) {
  temp_character2 = character;
  temp_character2_action = action;
  ResetAnimPacket(&TempAnim2,action);
  ResetLights(&TempLights2);
  return;
}

//MATCH NGC
void UpdateTempCharacter(void) {
  struct CharacterModel* model;
  s32 i;

  if (temp_character2 != -1) {
    i = CRemap[temp_character2];
    if (i == -1) {
      return;
    }
    TempAnim2.oldaction = TempAnim2.action;
    model = &CModel[i];
    if (((u16)TempAnim2.action < 0x76) && ((u16)TempAnim2.newaction < 0x76)) {
      UpdateAnimPacket(model,&TempAnim2,0.5f,0.0f);
    }
  }
  if (temp_character != -1) { 
      i = CRemap[temp_character];
    if (i != -1) {
      TempAnim.oldaction = TempAnim.action;
      if ((temp_character == 2) &&
         (((TempAnim.flags & 1) != 0 ||
          ((Pad[0] != NULL && ((Pad[0]->oldpaddata & 0x840) != 0)))))) {
        if ((Level == 0x26) && (Game.lives = '\x04', 0 <= cortex_gameover_i)) {
          if (cortex_gameover_i < 2) {
            new_level = 0x25;
            just_continued = 1;
          }
          else if (cortex_gameover_i < 7) {
            new_level = 0x23;
          }
        }
        if ((TempAnim.flags & 1) != 0) {
          if (TempAnim.action == 0x73) {
            gameover_hack = 1;
          }
          TempAnim.newaction = 0x22;
          TempAnim.flags = '\0';
        }
      }
      if ((tempanim_waitaudio != 0) && (NuSoundKeyStatus(4) == 1)) {
        TempAnim.newaction = tempanim_nextaction;
        tempanim_waitaudio = 0;
        if (Pad[0] != NULL) {
          Pad[0]->rdy = 0.0f;
        }
      }
        model = &CModel[i];
      if (((u16)TempAnim.action < 0x76) && ((u16)TempAnim.newaction < 0x76)) {
        UpdateAnimPacket(model,&TempAnim,0.5f,0.0f);
      }
    }
  }
  return;
}

//MATCH NGC
void UpdateScreenWumpas(void) {
  struct nuvec_s v[2];
  struct newwumpa_s *anew;
  struct wscr_s *scr;
  s32 i;
  
  scr = WScr;
  for (i = 0; i < 0x20; i++) {
        if (0.0f < scr->timer) {
          scr->timer = scr->timer - 0.016666668f;
              if (scr->timer < 0.0f) {
                scr->timer = 0.0f;
                    if (sw_hack != 0) {
                      mg_wumpatot++;
                      GameSfx(0x19,NULL);
                    }
                    else {
                          if (scr->bonus != '\0') {
                            plr_bonus_wumpas.count++;
                          }
                          else {
                            plr_wumpas.count++;
                          }
                    }
              }
        }
    scr++;
  }
  anew = NewWumpa;
  for (i = 0; i < 0x20; i++, anew++) {
        if (anew->active != '\0') {
              if (anew->delay == '\0') {
                if (anew->transformed == '\0') {
                  anew->transformed = '\x01';
                  NuCameraTransformScreenClip(&anew->screen_pos,&anew->world_pos,1,NULL);
                  v[0].x = (anew->world_pos).x + GameCam[0].vX.x * 0.1f;
                  v[0].y =  (anew->world_pos).y + GameCam[0].vX.y * 0.1f;
                  v[0].z = (anew->world_pos).z + GameCam[0].vX.z * 0.1f;
                  NuCameraTransformScreenClip(&v[1],v,1,NULL);
                  anew->screen_scale = (NuFabs((anew->screen_pos).x - v[1].x) * 3.636363f);
                }
                scr = &WScr[i_screenwumpa];
                scr->pos = anew->screen_pos;
                scr->scale = anew->screen_scale;
                scr->timer = 0.25f;
                    if (anew->bonus != '\0') {
                      scr->xs = BONUSWUMPAOBJSX;
                      scr->ys = BONUSPANELSY;
                      scr->bonus = '\x01';
                    }
                    else {
                      scr->xs = WUMPAOBJSX;
                      scr->ys = PANELSY;
                      scr->bonus = '\0';
                      force_panel_wumpa_update = 0x3c;
                    }
                i_screenwumpa++;
                if (i_screenwumpa == 0x20) {
                  i_screenwumpa = 0;
                }
                anew->count--;
                if (anew->count == '\0') {
                  anew->active = '\0';
                  continue;
                }
                anew->delay = '\f';
              }
              else {
                anew->delay--;
              }
        }
  }
    sw_hack = 0;
    return;
}

//NGC MATCH
void AddTempWumpa(float x,float y,float z,struct cratesarray_s *crate,s32 n) {
  struct wumpa_s* wumpa; 
  u16 a;
  float d;
  s32 i;

  if ((TimeTrial == 0)) {
    for (i = 0; i < n; i++) {
      wumpa = &Wumpa[i_tempwumpa + 0x100];
      wumpa->pos0.x = x;
      wumpa->pos0.y = y;
      wumpa->pos0.z = z;
      if (n > 1) {
        a = qrand();
        d = (qrand() * 0.000015259022f) * 0.25f;
        wumpa->pos1.x = wumpa->pos0.x + NuTrigTable[a & 0xffff] * (d);
        wumpa->pos1.y = wumpa->pos0.y;
        wumpa->pos1.z = wumpa->pos0.z + NuTrigTable[((a & 0xffff) + 0x4000) & 0x3fffc / 4] * d;
      }
      else {
        wumpa->pos1 = wumpa->pos0;
      }
      wumpa->pos = wumpa->pos0;
      wumpa->shadow = NewShadowMask(&wumpa->pos1,0.0,-1);
      wumpa->iRAIL = crate->iRAIL;
      wumpa->iALONG = crate->iALONG;
      wumpa->fALONG = crate->fALONG;
      wumpa->active = '\x01';
      wumpa->time = 0.0f;
      if (wumpa->shadow != 2000000.0f) {
        wumpa->surface_type = (char)ShadowInfo();
        FindAnglesZX(&ShadNorm);
        wumpa->surface_xrot = temp_xrot;
        wumpa->surface_zrot = temp_zrot;
      }
      else {
        wumpa->surface_type = -1;
      }
      i_tempwumpa++;
      if (i_tempwumpa == 0x40) {
        i_tempwumpa = 0;
      }
    }
  }
  return;
}

//98%
s32 Draw3DObject(s32 object,struct nuvec_s *pos,u16 xrot,u16 yrot,u16 zrot,float scalex,
                float scaley,float scalez,struct nugscn_s *scn,struct nuspecial_s *obj,s32 rot) {
  struct numtx_s m;
  struct nuvec_s s;
  s32 i;
  
  if (((scn == NULL) || (obj == NULL)) || ((scalex == 0.0f && ((scaley == 0.0f && (scalez == 0.0f))) || Level != 0x25))) {
          i = 0;
  }
  else {
    s.x = scalex;
    s.y = scaley;
    s.z = scalez;
    NuMtxSetScale(&m,&s);
    switch(rot) {
                case 0:
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
                case 1:
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
    }
    NuMtxTranslate(&m,pos);
    return NuRndrGScnObj(scn->gobjs[obj->instance->objid],&m);
  }
  return i;
}

//MATCH NGC
s32 Draw3DCharacter(struct nuvec_s *pos,u16 xrot,u16 yrot,u16 zrot,struct CharacterModel *model,
                    s32 action,float scale,float anim_time, s32 rot) {
    s32 i;
    struct numtx_s m;
    struct nuvec_s s;
    char pad[7];

    if ((model != 0) && (scale != 0.0f)) {
        if (Level != 0x25) {
        s32 iVar3;
            if (model->character == 0x75) {
                iVar3 = 0x84;
            } else if (model->character == 0x77) {
                iVar3 = 0x88;
            } else if (model->character == 0x78) {
                iVar3 = 0x89;
            } else if (model->character == 0x79) {
                iVar3 = 0x8a;
            } else if (model->character == 0x7a) {
                iVar3 = 0x8b;
            } else if (model->character == 0x7b) {
                iVar3 = 0x8c;
            } else if (model->character == 0x7c) {
                iVar3 = 0x8d;
            } else if (model->character == 0x7d) {
                iVar3 = 0x8e;
            }
        }
    
        s.z = scale;
        s.y = scale;
        s.x = scale;
        NuMtxSetScale(&m,&s);
        switch (rot) {
                case 0:
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
                case 1:
                    if (yrot != NULL) {
                        NuMtxRotateY(&m,(s32)yrot);
                    }
                    if (xrot != NULL) {
                        NuMtxRotateX(&m,(s32)xrot);
                    }
                    if (zrot != NULL) {
                        NuMtxRotateZ(&m,(s32)zrot);
                    }
                    break;
        }
        NuMtxTranslate(&m,pos);
         if (((uint)action < 0x76) && (model->anmdata[action] != NULL)) {
            NuHGobjEvalAnim(model->hobj,model->anmdata[action],anim_time,0,NULL,tmtx);
        }
        else {
            NuHGobjEval(model->hobj,0,NULL,tmtx);
        }
        i = NuHGobjRndrMtx(model->hobj,&m,1,NULL,tmtx);
    }
    else {
        i = 0;
    }
    return i;
}

//NGC MATCH
void NewMask(struct mask_s *mask,struct nuvec_s *pos) {
  
  if ((CRemap[3] != -1) && ((LBIT & 0x3e00000) == 0)) {
    if (mask->active < 2) {
      if (mask->active == 0) {
        if (pos != NULL) {
          (mask->pos) = *pos;
        }
        mask->character = 3;
        mask->scale = 0.8f;
        ResetLights(&mask->lights);
      }
      mask->active++;
    }
    else {
      mask->active = 0x296;
      GameMusic(0xa2,0);
    }
    GameSfx(0x3d,pos);
  }
  return;
}

//NGC MATCH
void MakeMaskMatrix(struct mask_s *mask,struct numtx_s *mM,struct numtx_s *mS,struct numtx_s *mLOCATOR,float scale) {
  double dVar1;
  struct nuvec_s s;
  u16 yrot;
  
  if (mM != NULL) {
    s.x = s.y = s.z = scale * mask->scale;
    NuMtxSetScale(mM,&s);
    yrot = mask->yrot - 0x8000 & 0xffff;
    if (mask->active > 2) {
         if (mLOCATOR != NULL) {
           NuMtxMulR(mM,mM,mLOCATOR);
          (mask->pos).x = mLOCATOR->_30;
          (mask->pos).y = mLOCATOR->_31;
          (mask->pos).z = mLOCATOR->_32;       
         }
         else {
              NuMtxRotateX(mM,(int)(NuTrigTable[(GameTimer.frame & 0xf) * 0x1000] * 8192.0f) & 0xffff);
              NuMtxRotateZ(mM,(int)((NuTrigTable[((GameTimer.frame & 0xf) * 0x1000 + 0x4000) & 0x3c000 / 4]) * 8192.0f) & 0xffff);
              NuMtxRotateY(mM,yrot);
          }
    }
    else {
      NuMtxRotateX(mM,(uint)mask->xrot);
      NuMtxRotateY(mM,yrot);
    }
    NuMtxTranslate(mM,&mask->pos);
    if ((mS != NULL) && (mask->shadow != 2000000.0f)) {
      ScaleFlatShadow(&s,(mask->pos).y,mask->shadow,1.0f);
      NuMtxSetScale(mS,&s);
      NuMtxRotateY(mS,yrot);
      NuMtxRotateZ(mS,mask->surface_zrot);
      NuMtxRotateX(mS,mask->surface_xrot);
      mS->_30 = (mask->pos).x;
      mS->_31 = mask->shadow + 0.025f;
      mS->_32 = (mask->pos).z;
    }
  }
  return;
}

void ResetMaskFeathers(void) {
  memset(MaskFeathers,0,0x240);
  return;
}

//NGC MATCH
void AddMaskFeathers(struct mask_s * mask) {
  struct mfeathers_s *feathers;
  s32 i;
  
  feathers = &MaskFeathers[i_maskfeathers];
    feathers->mM = mask->mM;
    feathers->mS = mask->mS;
  i = CRemap[11];
  feathers->time = 0.0f;
  feathers->shadow = mask->shadow;
  if ((i != -1) && (CModel[i].anmdata[0xe] != NULL)) {
    feathers->duration = CModel[i].anmdata[0xe]->time - 1.0f;
  }
  else {
    feathers->duration = 30.0f;
  }
  i_maskfeathers++;
  if (i_maskfeathers != 4) {
    return;
  }
  i_maskfeathers = 0;
  return;
}

//NGC MATCH
void LoseMask(struct obj_s *obj) {
  
  GameSfx(1,&obj->pos);
  AddMaskFeathers(obj->mask);
  obj->mask->active--;
  obj->invincible = 0xb4;
  (obj->mom).z = 0.0f;
  (obj->mom).x = 0.0f;
  NewBuzz(&player->rumble,0x1e);
  NewRumble(&player->rumble,0x7f);
  return;
}

//NGC MATCH
void UpdateMaskFeathers(void) {
  struct mfeathers_s *feathers;
  s32 i;
  
  feathers = MaskFeathers;
  for (i = 0; i < 4; i++) {
    if ((feathers->time < feathers->duration) && (feathers->time += 0.5f, feathers->time > feathers->duration)) {
      feathers->time = feathers->duration;
    }
    feathers++;
  }
  return;
}

//NGC MATCH
void DrawMaskFeathers(void) {
    struct CharacterModel* model;
    struct mfeathers_s *feathers;
    int i;

    i = CRemap[11];
    if (i != -1) {
        model = &CModel[i];
        if (model->anmdata[0xe] != NULL) {
            feathers = &MaskFeathers[0];
            for (i = 0; i < 4; i++) {
                if (feathers->time < feathers->duration) {
                    NuHGobjEvalAnim(model->hobj,model->anmdata[0xe],feathers->time,0,NULL,tmtx);
                    NuHGobjRndrMtx(model->hobj,&feathers->mM,1,NULL,tmtx);
                }
                feathers++;
            }
        }
    }
    return;
}

//NGC MATCH
void DrawTempCharacter(s32 render) {
    struct CharacterModel* model;
    struct numtx_s m;
    static struct numtx_s mtxLOCATOR_168[16];
    float **dwa;
    struct nuvec_s v;
    s32 i;

   
    if ((((((temp_character != -1) && (temp_character != 0x62)) && (temp_character != 0xb9)) &&
         ((temp_character != 0xb8 && (temp_character != 0xba)))) &&
        ((temp_character != 0xbc)))) {
        if (temp_character != 2 || (Level != 0x19)) {
            i = (s32)CRemap[temp_character];
            if((i != -1)) {
                        model = &CModel[i];
                        if (Level == 0x25) {
                            v.x = 5.0f;
                            v.y = 5.0f;
                            v.z = 5.0f;
                            NuMtxSetScale(&m,&v);
                            v.x = 0.0f;
                            v.y = 11.0f;
                            v.z = 6.25f;
                            NuMtxTranslate(&m,&v);
                        }              
                        else {
                            NuMtxSetIdentity(&m);
                        }
                        if (Level == 0x2b) {
                            if (model->character == 0xbb) {
                                NuMtxRotateY(&m,0x2000);
                                m._30 = DISCOXOFFSET;
                            }
                            else if (model->character == 0) {
                                NuMtxRotateY(&m,-0x2000);
                                m._30 = -DISCOXOFFSET;
                            }
                        }
                        EvalModelAnim(model,&TempAnim,&m,tmtx,&dwa,mtxLOCATOR_168);
                        if (render != 0) {
                            NuHGobjRndrMtxDwa(model->hobj,&m,1,NULL,tmtx,dwa);
                        }
            }
        }
    }
    return;
}

//NGC MATCH
void DrawTempCharacter2(s32 render) {
    struct CharacterModel* model;
    struct numtx_s m;
    static struct numtx_s mtxLOCATOR_172[16];
    float **dwa;
    s32 i;
    short layertab[2] = {0, 1};
    short* layer;
    s32 nlayers;

    layer = layertab;
    if ((temp_character2 != -1)) {
    i = CRemap[temp_character2];
        if (i != -1) {
                nlayers = 1;
                model = &CModel[i];
                if (model->character == 0) {
                    nlayers = 2;
                }
                NuMtxSetIdentity(&m);
                if (Level == 0x2b) {
                    if (model->character == 0xbb) {
                        NuMtxRotateY(&m,0x2000);
                        m._30 = DISCOXOFFSET;
                    }
                    else if (model->character == 0) {
                        NuMtxRotateY(&m,-0x2000);
                        m._30 = -DISCOXOFFSET;
                    }
                }
                EvalModelAnim(model,&TempAnim2,&m,tmtx,&dwa,mtxLOCATOR_172);
                if (render != 0) {
                    NuHGobjRndrMtxDwa(model->hobj,&m,nlayers,layer,tmtx,dwa);
                }
        }
 
    }
    return;
}

//NGC MATCH
s32 HubFromLevel(s32 level) {
  s32 j;
  s32 i;
  
  if (level == -1) {
    return -1;
  }
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 6; j++) {
      if (HData[i].level[j] == level) {
        temp_hublevel = j;
        temp_hub = i;
        return i;
      }
    }
  }
  return -1;
}

//NGC MATCH
void ResetAwards(void) {
  s32 i;
  
  for (i = 0; i < 3; i++) {
    Award[i].time = 1.0f;
  }
  return;
}

//NGC MATCH
s32 ActiveAwards(void) {
  struct award_s *awards;
  s32 i;
  
  awards = Award;
  for (i = 0; i < 3; i++) {
    if (awards->time < 1.0f) {
      return 1;
    }
    awards = awards + 1;
  }
  return 0;
}

//NGC 99%
s32 AddAward(s32 hub,s32 level,u16 got) {
  s32 i0;
  struct award_s* award;
  struct nugspline_s *spl;
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  s32 i;
  s32 j;
  s32 ang;
  
  if (((hub != -1) && (level != -1)) && (HData[hub].i_spl[1] != -1)) {
    spl = SplTab[HData[hub].i_spl[1]].spl;
    if (spl == NULL) {
      return 0;
    }
    HubFromLevel(level);
      i0 = temp_hublevel;
    if (i0 == -1) {
        return 0;
    } else{
        if ((got & 7) != 0) {
           ang = 2;
        } else if ((got & 8) != 0) {
          ang = 1;
        } else if ((got & 0x10) != 0) {
          ang = 0;
        } else{
           ang = 3;
        }
      award = &Award[i_award];
        i = ang * 2;
      i +=  (i0 * 8); 
      j = (i + 1);
      i *= spl->ptsize;
      j *= spl->ptsize;
      p0 = (struct nuvec_s*)  &spl->pts[i];
      p1 = (struct nuvec_s*) &spl->pts[j];
      award->time = 0.0f;
      award->yrot = (u16) NuAtan2D(p1->x - p0->x,p1->z - p0->z);
      award->level = (char)level;
      award->got = got;
      award->newpos = *p0;
      award->wait = 1;
      i_award++;
      if (i_award == 3) {
        i_award = 0;
      }
    }
  } else{
      return 0;
  }
  return 1;
}

//NGC 98% (float)
void DrawAwards(void) {
    struct award_s* award;
    struct nuvec_s pos;
    float f;
    float scale;
    float t;
    s32 uVar2;
    s32 i;
    s32 i_obj;
    s32 i_chr;

    award = Award;
    for (i = 0; i < 3; i++) {
        t = award->time;
        if (t < 1.0f) {
            if (award->wait != 0) {
                pos = award->oldpos0;
                t =
                    (((player->obj).anim.anim_time - (tumble_item_starttime + 1.0f))
                     / (tumble_item_addtime - (tumble_item_starttime + 1.0f)));
                if (t > 1.0f) {
                    t = 1.0f;
                }
                if (t < 0.333f) {
                    scale = 0.0f;
                } else {
                    scale = ((t - 0.333f) / 0.667f);
                }
            } else {
                if (t < 0.666f) {
                    scale = 1.0f;
                    pos = award->oldpos0;
                } else {
                    scale = 1.0f;
                    f = (t - 0.666f) / 0.33399999f;
                    pos.x = (award->newpos.x - award->oldpos0.x) * f + award->oldpos0.x;
                    pos.y = (award->newpos.y - award->oldpos0.y) * f + award->oldpos0.y;
                    pos.z = (award->newpos.z - award->oldpos0.z) * f + award->oldpos0.z;
                }
            }
            i_obj = -1;
            i_chr = -1;
            if ((award->got & 4) != 0) {
                i_obj = 1;
            } else if ((award->got & 2) != 0) {
                i_obj = 2;
            } else if ((award->got & 1) != 0) {
                i_obj = 3;
            } else {
                if (award->got == 8) {
                    i_obj = 0xc1;
                } else if (award->got == 0x10) {
                    i_obj = 0xc2;
                } else if (award->got == 0x20) {
                    i_obj = 0xc3;
                } else if (award->got == 0x40) {
                    i_obj = 0xc4;
                } else if (award->got == 0x80) {
                    i_obj = 0xc5;
                } else if (award->got == 0x100) {
                    i_obj = 0xc6;
                } else if (award->got == 0x200) {
                    i_obj = 200;
                } else if (award->got == 0x400) {
                    i_obj = 199;
                }
            }
            if (i_obj != -1) {
                f = (scale * 0.76999998f);
                if (ObjTab[i_obj].obj.special != NULL) {
                    Draw3DObject(
                        i_obj, &pos, 0, award->yrot, 0, f, f, f, ObjTab[i_obj].obj.scene, ObjTab[i_obj].obj.special, 0
                    );
                }
            } else if (i_chr != -1) {
                f = (scale * 0.76999998f);
                if (CRemap[i_chr] != -1) {
                    Draw3DCharacter(&pos, 0, award->yrot, 0, &CModel[CRemap[i_chr]], -1, f, 1.0f, 0);
                }
            }
        }
        award++;
    }
    return;
}

//NGC MATCH
s32 qrand(void) {
  qseed = qseed * 0x24cd + 1U & 0xffff;
  return qseed;
}

//NGC MATCH
s32 RotDiff(u16 a0,u16 a1) {
  s32 diff;
  
  diff = a1 - a0;
  if (0x8000 < diff) {
    return diff + -0x10000;
  }
  if (-0x8001 < diff) {
    return diff;
  }
  return diff + 0x10000;
}

//NGC MATCH
u16 SeekRot(u16 a0,u16 a1,s32 shift) {
  s32 d;
  
  d = a1 - a0;
  if (d > 0x8000) {
    d = d + -0x10000;
  }
  else {
    if (d < -0x8000) {
      d = d + 0x10000;
    }
  }
  return a0 + (d >> shift);
}

//91%
u16 TurnRot(u16 a0,u16 a1,s32 rate) {
  s32 d;
  
  if (a1 != a0) {
    d = RotDiff(a0,a1);
    if (d > 0) {
      if (d > rate) {
       return a0 + rate & 0xffff;
      }
    }
    if (d < -rate) {
        return a0 - rate & 0xffff;
    }
  }
  return a1;
}

//NGC MATCH
s32 AheadOfCheckpoint(s32 iRAIL,s32 iALONG,float fALONG) {
if ((cp_iRAIL == -1) || (cp_iALONG == -1))
{
    return 1;
}
  if (((iRAIL == -1)) ||  (((iALONG == -1 || (Rail[iRAIL].type != 0)) || (iRAIL > cp_iRAIL)))) {
      return 1;
  }
    if (iRAIL < cp_iRAIL) {
      return 0;
    }
    if (iALONG > cp_iALONG) { 
        return 1; 
    } else if (iALONG >= cp_iALONG) {
        return (fALONG > cp_fALONG);
    } else if (iALONG >= cp_iALONG) {
        // This just creates the final return expression
        return (fALONG > cp_fALONG);
    } 

    return 0;
}

//NGC MATCH
void ResetLoadSaveCharacter(void) {
  tumble_action = -1;
  tumble_duration = 0.0f;
  tumble_time = 0.0f;
  last_level = -1;
  last_hub = -1;
  Hub = -1;
  (player->obj).hdg = 0x8000;
  if (pos_START != NULL) {
    (player->obj).pos = *pos_START;
    (player->obj).shadow = NewShadowMaskPlat(&(player->obj).pos,0.0f,-1);
  }
  return;
}


void ProcMenu(Cursor *cursor,nupad_s *pad)
{
  bool bVar1;
  char cVar2;
  char cVar3;
  bool bVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  creature_s *pcVar8;
  nuvec_s *pnVar9;
  Game_s *pGVar10;
  Game_s *pGVar11;
  uchar uVar13;
  char cVar14;
  char cVar15;
  ushort uVar12;
  uint uVar16;
  uint uVar17;
  int iVar18;
  char *pcVar19;
  uint uVar20;
  int uVar21;
  float fVar21;
  int iVar22;
  int iVar23;
  float fVar24;
  Game_s *pGVar25;
  uchar *puVar26;
  mask *pmVar27;
  Game_s *pGVar28;
  int iVar29;
  int test_sfx [4];
  double local_38;
  
  uVar20 = menu_pulsate_ang & 0xffff;
  menu_pulsate_ang = menu_pulsate_ang + menu_pulsate_rate;
  menu_pulsate = NuTrigTable[uVar20] * 0.1 + 1.0;
  test_sfx[0] = 0x25;
  test_sfx[1] = 0x19;
  test_sfx[2] = 0x13;
  test_sfx[3] = 0x2c;
  if (0x10000 < menu_pulsate_ang) {
    menu_pulsate_ang = menu_pulsate_ang + -0x10000;
  }
  if ((cursor->wait != '\0') &&
     (uVar13 = cursor->wait + 0xff, cursor->wait = uVar13, uVar13 == '\0')) {
    cursor->wait_hack = '\x01';
    NewMenu(cursor,(int)cursor->new_menu,-1,-1);
  }
  if (ForceRestart == 0) {
    if (cursor->menu == -1) {
      return;
    }
    if (cursor->wait != '\0') {
      return;
    }
    if (0 < fadeval) {
      return;
    }
    if (Paused - 1U < 0x1d) {
      return;
    }
    if (cutmovie == -1) {
      if (new_mode != -1) {
        return;
      }
      if (new_level != -1) {
        return;
      }
    }
    else if (cut_on == 0) {
      return;
    }
    if ((cursor->menu == '\x14') && (gameover_hack != 1)) {
      return;
    }
  }
  iVar29 = -1;
  if (cursor->button_lock != '\0') {
    cursor->button_lock = cursor->button_lock + -1;
  }
  if ((pad == (nupad_s *)0x0) || (cursor->button_lock != '\0')) {
    uVar20 = 0;
    fVar21 = 0.0;
  }
  else {
    fVar21 = pad->r_dy;
    if (((uint)fVar21 & 0xf000) == 0) {
      fVar21 = (float)((uint)fVar21 | stick_bits);
    }
    uVar20 = pad->old_paddata;
    if ((uVar20 & 0xf000) == 0) {
      uVar20 = uVar20 | stick_bits_db;
    }
  }
  uVar16 = uVar20 & 0x840;
  uVar5 = uVar20 & 0x180;
  uVar6 = (uint)fVar21 & 0x8000;
  uVar7 = (uint)fVar21 & 0x2000;
  if (cursor->item_frame == 0) {
    uVar16 = 0;
  }
  uVar17 = uVar16;
  if (ForceRestart != 0) {
    ForceRestart = 0;
    uVar17 = 1;
    cursor->menu = '\v';
  }
  cVar14 = cursor->y_max;
  cVar15 = cursor->y_min;
  cVar2 = cursor->y;
  cVar3 = (cVar14 - cVar15) + '\x01';
  if (cVar2 < cVar15) {
    cursor->y = cVar15;
  }
  else if (cVar14 < cVar2) {
    cursor->y = cVar14;
  }
  if ((uVar20 & 0x4000) == 0) {
    if ((uVar20 & 0x1000) != 0) {
      cVar14 = cursor->y + -1;
      cursor->y = cVar14;
      if (cVar14 < cursor->y_min) {
        cVar14 = cVar14 + cVar3;
        goto LAB_80032034;
      }
    }
  }
  else {
    cVar14 = cursor->y + '\x01';
    cursor->y = cVar14;
    if (cursor->y_max < cVar14) {
      cVar14 = cVar14 - cVar3;
LAB_80032034:
      cursor->y = cVar14;
    }
  }
  cursor->remember[cursor->menu].y = cursor->y;
  if (cursor->y != cVar2) {
    iVar29 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  cVar14 = cursor->x_max;
  cVar15 = cursor->x_min;
  cVar2 = cursor->x;
  cVar3 = (cVar14 - cVar15) + '\x01';
  if (cVar2 < cVar15) {
    cursor->x = cVar15;
  }
  else if (cVar14 < cVar2) {
    cursor->x = cVar14;
  }
  if (cursor->y < cursor->y_max) {
    if ((uVar20 & 0x2000) == 0) {
      if ((uVar20 & 0x8000) != 0) {
        cVar14 = cursor->menu;
        cVar15 = cursor->x + -1;
        cursor->x = cVar15;
        if (((cVar14 == '\x16') || (cVar14 == '\x19')) || (cVar14 == '\x1d')) {
          if (cursor->x < cursor->x_min) {
            cursor->x = cursor->x_min;
          }
        }
        else if (cVar15 < cursor->x_min) {
          cVar15 = cVar15 + cVar3;
          goto LAB_800321ac;
        }
      }
    }
    else {
      cVar14 = cursor->menu;
      cVar15 = cursor->x + '\x01';
      cursor->x = cVar15;
      if (((cVar14 == '\x16') || (cVar14 == '\x19')) || (cVar14 == '\x1d')) {
        if (cursor->x_max < cursor->x) {
          cursor->x = cursor->x_max;
        }
      }
      else if (cursor->x_max < cVar15) {
        cVar15 = cVar15 - cVar3;
LAB_800321ac:
        cursor->x = cVar15;
      }
    }
  }
  cursor->remember[cursor->menu].x = cursor->x;
  if (cursor->x != cVar2) {
    iVar29 = 0x18;
    cursor->item_frame = 0;
    uVar16 = 0;
    uVar17 = 0;
  }
  bVar1 = uVar17 == 0;
  if ((!bVar1) || (uVar5 != 0)) {
    cursor->item_frame = 0;
  }
  iVar23 = Level;
  uVar17 = cursor->item_frame + 1;
  cursor->menu_frame = cursor->menu_frame + 1;
  cursor->item_frame = uVar17;
  iVar22 = cortex_quit_i;
  iVar18 = cortex_continue_i;
  if ((iVar23 == 0x23) || (cVar14 = cursor->menu, cutmovie == 0)) {
    cVar14 = cursor->menu;
    iVar23 = (int)cVar14;
    if ((iVar23 == 0) && (0x707 < uVar17)) {
      new_level = DemoLevel[i_demolevel];
      i_demolevel = i_demolevel + 1;
      if (i_demolevel == 3) {
        i_demolevel = iVar23;
      }
      if (cutmovie != 0) {
        Demo = 1;
        InvincibilityCHEAT = iVar23;
        return;
      }
      Demo = 1;
      Level = new_level;
      InvincibilityCHEAT = iVar23;
      fade_rate = 8;
      return;
    }
  }
  cVar15 = cursor->y;
  iVar23 = (int)cVar15;
  if (cVar14 == '\x17') {
    loadsaveCallEachFrame();
    if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
      NewMenu(cursor,0x26,-1,-1);
    }
    if (bVar1) {
      if (uVar5 == 0) goto LAB_800346ac;
      iVar18 = 0x16;
      iVar29 = -1;
    }
    else {
      if (cursor->y != '\x01') {
        iVar29 = 0x408;
        pGVar10 = &Game;
        pGVar11 = game;
        do {
          pGVar28 = pGVar11;
          pGVar25 = pGVar10;
          iVar29 = iVar29 + -0x18;
          *(undefined4 *)pGVar25->name = *(undefined4 *)pGVar28->name;
          *(undefined4 *)(pGVar25->name + 4) = *(undefined4 *)(pGVar28->name + 4);
          *(undefined4 *)(pGVar25->name + 8) = *(undefined4 *)(pGVar28->name + 8);
          *(undefined4 *)&pGVar25->music_volume = *(undefined4 *)&pGVar28->music_volume;
          pGVar25->hub[0] = pGVar28->hub[0];
          pGVar25->hub[1] = pGVar28->hub[1];
          pGVar10 = (Game_s *)(pGVar25->hub + 2);
          pGVar11 = (Game_s *)(pGVar28->hub + 2);
        } while (iVar29 != 0);
        iVar29 = 0x36;
        *(undefined4 *)((Game_s *)(pGVar25->hub + 2))->name =
             *(undefined4 *)((Game_s *)(pGVar28->hub + 2))->name;
        pGVar25->hub[3] = pGVar28->hub[3];
        pGVar25->hub[4] = pGVar28->hub[4];
        NewLanguage((uint)Game.language);
        NewMenu(cursor,0x18,0,-1);
        memcard_loadresult_delay = 0x28;
        plr_lives.count = (short)Game.lives;
        plr_lives.draw = plr_lives.count;
        goto LAB_800346ac;
      }
      iVar18 = 0x16;
      iVar29 = -1;
    }
    goto LAB_80034620;
  }
  if (cVar14 < '\x18') {
    if (cVar14 == '\v') {
      if (bVar1) {
        if (uVar5 == 0) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 5;
      }
      else {
        if (iVar23 == 5) {
          uVar21 = 1;
LAB_80032950:
          iVar18 = GotoCheckpoint(&(player->obj).pos,uVar21);
          if (iVar18 != 0) {
            new_mode = GameMode;
          }
          goto LAB_800346ac;
        }
        if (iVar23 < 6) {
          if (iVar23 != 2) {
            if (iVar23 < 3) {
              if (iVar23 == 0) {
                new_mode = GameMode;
                ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
                LivesLost = iVar23;
                LostLife = iVar23;
                ResetBonus();
                ResetDeath();
                ResetGemPath();
                ResetItems();
              }
              else if (iVar23 == 1) {
                ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
                LivesLost = 0;
                LostLife = 0;
                ResetBonus();
                bonus_restart = 0;
                ResetDeath();
                ResetGemPath();
                RestoreCrateTypeData();
                ResetCrates();
                ResetWumpa();
                ResetChases();
                ResetPlayerEvents();
                ResetGates();
                ResetRings();
                ResetMaskFeathers();
                ResetAI();
                ResetPlayer(0);
                ResetBug();
                ResetLevel();
                ResetProjectiles();
                NewMenu(cursor,-1,-1,-1);
                Paused = 0;
                pause_dir = 0;
                ResumeGame();
                edobjResetAnimsToZero();
              }
              goto LAB_800346ac;
            }
            if (iVar23 == 3) {
              InvincibilityCHEAT = 1 - InvincibilityCHEAT;
              ResetAI();
              goto LAB_800346ac;
            }
            if (iVar23 != 4) goto LAB_800346ac;
            uVar21 = 0;
            goto LAB_80032950;
          }
          iVar22 = -1;
          iVar18 = 0xe;
        }
        else {
          if (iVar23 == 8) {
            ShowPlayerCoordinate = 1 - ShowPlayerCoordinate;
            goto LAB_800346ac;
          }
          if (iVar23 < 9) {
            if (iVar23 != 6) {
              if (iVar23 == 7) {
                LIFTPLAYER = 1 - LIFTPLAYER;
              }
              goto LAB_800346ac;
            }
            OpenGame();
            iVar22 = -1;
            iVar18 = 5;
          }
          else {
            if (iVar23 == 9) {
              ExtraMoves = 1 - ExtraMoves;
              goto LAB_800346ac;
            }
            if (iVar23 != 10) goto LAB_800346ac;
            NewGame();
            ResetBonus();
            ResetDeath();
            ResetGemPath();
            CalculateGamePercentage(&Game);
            Hub = HubFromLevel(Level);
            iVar22 = -1;
            iVar18 = 5;
          }
        }
      }
LAB_80034684:
      NewMenu(cursor,iVar18,iVar22,-1);
      goto LAB_800346ac;
    }
    if (cVar14 < '\f') {
      if (cVar14 == '\x05') {
        if (bVar1) goto LAB_800346ac;
        iVar29 = 0x36;
        if (iVar23 == 1) {
          NewMenu(cursor,6,0,-1);
          storemenu1();
          goto LAB_800346ac;
        }
        if (iVar23 < 2) {
          if (iVar23 == 0) {
            pause_dir = 2;
          }
          goto LAB_800346ac;
        }
        if (iVar23 == 2) {
          iVar22 = 1;
          iVar18 = 10;
          goto LAB_80034684;
        }
        if ((iVar23 != 3) || (TimeTrial == 0)) goto LAB_800346ac;
        new_mode = GameMode;
        ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
        LivesLost = 0;
LAB_8003388c:
        iVar29 = 0x36;
        ResetBonus();
        ResetDeath();
        ResetGemPath();
        goto LAB_800346ac;
      }
      if ('\x05' < cVar14) {
        if (cVar14 == '\b') goto LAB_800346ac;
        if ('\b' < cVar14) {
          if (cVar14 == '\t') {
            if (!bVar1) {
              iVar29 = 0x36;
              NewLanguage(iVar23);
              iVar22 = -1;
              iVar18 = 6;
              goto LAB_80034684;
            }
            if (uVar5 == 0) goto LAB_800346ac;
            iVar29 = -1;
            iVar18 = 6;
          }
          else {
            if (cVar14 != '\n') goto LAB_800346ac;
            if (bVar1) {
              if (uVar5 == 0) goto LAB_800346ac;
              iVar29 = -1;
              iVar18 = 5;
            }
            else {
              if (iVar23 == 0) {
                new_level = 0x25;
                if (Level == 0x25) {
                  new_level = 0x23;
                }
                goto LAB_800346a8;
              }
              iVar29 = -1;
              iVar18 = 5;
            }
          }
          goto LAB_80034620;
        }
        if (cVar14 == '\x06') {
          if (!bVar1) {
            if (iVar23 == 0) {
              iVar29 = 0x36;
              Game.vibration = '\x01' - Game.vibration;
              goto LAB_800346ac;
            }
            if (iVar23 == 1) {
              storemenu2();
              iVar29 = 0x36;
              iVar18 = 7;
              goto LAB_80034680;
            }
            if ((LANGUAGEOPTION != 0) && (iVar23 == 2)) {
              iVar29 = 0x36;
              NewMenu(cursor,9,(uint)Game.language,-1);
              goto LAB_800346ac;
            }
            if (iVar23 != cursor->y_max) goto LAB_800346ac;
            iVar29 = 5;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          if (uVar5 == 0) goto LAB_800346ac;
          restoremenu1();
          iVar29 = 0x3c;
          iVar22 = -1;
          iVar18 = 5;
        }
        else {
          if (cVar14 != '\a') goto LAB_800346ac;
          if (!bVar1) {
            if (iVar23 != 2) goto LAB_800346ac;
            iVar29 = 6;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          if (uVar5 == 0) {
            if (iVar23 == 0) {
              iVar18 = qrand();
              if (iVar18 < 0x1000) {
                gamesfx_volume = (int)Game.sfx_volume;
                iVar18 = qrand();
                if (iVar18 < 0) {
                  iVar18 = iVar18 + 0x3fff;
                }
                GameSfx(test_sfx[iVar18 >> 0xe],(nuvec_s *)0x0);
              }
              if ((uVar6 == 0) || (Game.sfx_volume == '\0')) {
                if ((uVar7 != 0) && (Game.sfx_volume < 100)) {
                  Game.sfx_volume = Game.sfx_volume + '\x01';
                }
              }
              else {
                Game.sfx_volume = Game.sfx_volume + 0xff;
              }
            }
            else if (iVar23 == 1) {
              if ((uVar6 == 0) || (Game.music_volume == '\0')) {
                if ((uVar7 != 0) && (Game.music_volume < 100)) {
                  Game.music_volume = Game.music_volume + '\x01';
                }
              }
              else {
                Game.music_volume = Game.music_volume + 0xff;
              }
            }
            goto LAB_800346ac;
          }
          restoremenu2();
          iVar29 = 0x3c;
          iVar22 = -1;
          iVar18 = 6;
        }
        goto LAB_80034684;
      }
      if (cVar14 == '\x02') {
        if (memtest_done < 3) {
          if (memtest_done < 1) {
            if ((memtest_done == 0) && (loadsaveCallEachFrame(), saveload_status == 1)) {
              if (saveload_cardtype == 2) {
                memtest_done = 3;
              }
              else {
                memtest_done = saveload_status;
              }
            }
          }
          else {
            loadsaveCallEachFrame();
            if (!bVar1) {
              iVar29 = 0x36;
              memtest_done = 3;
            }
          }
        }
        else if (memtest_done == 3) {
          NewMenu(cursor,-1,-1,-1);
          if (CutAudio[cutworldix] != -1) {
            gamesfx_channel = 4;
            GameSfx(CutAudio[cutworldix],(nuvec_s *)0x0);
          }
        }
        goto LAB_800346ac;
      }
      if ('\x02' < cVar14) {
        if (cVar14 == '\x03') {
          if (bVar1) {
            if (uVar5 != 0) {
              NuSoundKillAllAudio();
              XbWaitForAllBuffersToStop();
              XbLaunchDashboardToFreeSpace();
            }
            goto LAB_800346ac;
          }
          if (NotEnoughSpace_NewGame != 0) {
            iVar29 = 4;
            goto LAB_8003469c;
          }
          new_level = 0x25;
          force_menu = 0x15;
          if (cutmovie == 0) {
            fade_rate = 8;
          }
          goto LAB_800346a8;
        }
        if (cVar14 != '\x04') goto LAB_800346ac;
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
          goto LAB_800346a0;
        }
        iVar29 = 0x36;
        iVar18 = (int)cursor->y_max;
        if (iVar23 == iVar18 + -1) {
          CleanLetters(Game.name);
          iVar18 = strcmp(Game.name,"        ");
          if (iVar18 == 0) {
            Game.name[8] = '\0';
            Game.name._0_4_ = 0x43524153;
            Game.name._4_4_ = 0x48202020;
          }
          new_level = 0x25;
          if (cutmovie == 0) {
            next_cut_movie = 1;
            NewMenu(cursor,-1,-1,-1);
            fade_rate = 8;
          }
          else {
            cutmovie = 1;
          }
          pcVar19 = strstr(Game.name,"WOMBAT");
          if (pcVar19 != (char *)0x0) {
            Game.name._0_4_ = 0x43524153;
            Game.name[8] = '\0';
            Game.name._4_4_ = 0x48202020;
            cheating = 1;
            OpenGame();
          }
          goto LAB_800346ac;
        }
        if (iVar23 == iVar18) {
          iVar22 = -1;
          iVar18 = 0;
          goto LAB_80034684;
        }
        if (Game.language == 'c') {
          iVar22 = 2;
          if ((iVar23 != iVar18 + -2) && (iVar22 = 1, iVar23 != iVar18 + -3)) {
            if (iVar23 != iVar18 + -4) {
              if ((input_alphabet == 2) && ((cVar15 == '\0' || (cVar15 == '\x04'))))
              goto LAB_800346ac;
              goto LAB_80033630;
            }
            iVar22 = 0;
          }
          input_alphabet = iVar22;
          GetMenuInfo(cursor);
        }
        else {
LAB_80033630:
          InputNewLetter(cursor,Game.name,&i_nameinput,8);
        }
        goto LAB_800346ac;
      }
      if (cVar14 != '\0') {
        if (cVar14 != '\x01') goto LAB_800346ac;
        loadsaveCallEachFrame();
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0;
          goto LAB_80034620;
        }
        iVar18 = ParseNintendoErrorCode();
        iVar29 = 0x36;
        if (iVar18 == 0) {
          if (wasloadgame != 0) {
            new_level = 0x25;
            force_menu = 0x15;
            if (cutmovie == 0) {
              fade_rate = 8;
            }
            goto LAB_800346ac;
          }
          iVar18 = 4;
          goto LAB_80034680;
        }
        if (iVar23 == 0) {
          memtest_done = iVar23;
          MemCardRetry();
          iVar29 = ParseNintendoErrorCode();
          if (iVar29 == 0) {
            if (wasloadgame == 0) {
              iVar29 = 4;
            }
            else {
              iVar29 = 0;
            }
LAB_8003469c:
            iVar18 = 0;
            goto LAB_800346a0;
          }
        }
        else {
          if (iVar23 == 1) {
            if (wasloadgame == 0) {
              iVar29 = 4;
            }
            else {
              iVar29 = 0;
            }
            goto LAB_8003469c;
          }
          if (iVar23 == 2) {
            iVar29 = ParseNintendoErrorCode();
            if (iVar29 == 6) {
              Managememcard();
            }
            else if (iVar29 < 7) {
              if ((iVar29 < 4) && (1 < iVar29)) {
                iVar29 = 0x20;
                iVar18 = 1;
                goto LAB_800346a0;
              }
            }
            else if (iVar29 == 8) {
              SaveMenu = wasloadgame;
              if (wasloadgame != 0) {
                SaveMenu = 0;
              }
              NewMenu(cursor,0x30,1,-1);
            }
          }
        }
        goto LAB_800346a8;
      }
      wasloadgame = 0;
      loadsaveCallEachFrame();
      if (bVar1) {
        if (iVar23 == 3) {
          old_lang.356 = (int)Game.language;
          if (((uVar20 & 0x2000) == 0) || (4 < (uint)old_lang.356)) {
            if (((uVar20 & 0x8000) != 0) && (Game.language != '\0')) {
              Game.language = Game.language + 0xff;
            }
          }
          else {
            Game.language = Game.language + '\x01';
          }
        }
        goto LAB_80033140;
      }
      if (iVar23 == 1) {
        wasloadgame = iVar23;
        MemCardRetry();
        iVar29 = ParseNintendoErrorCode();
        if (iVar29 != 0) {
          DestMenu = 0;
          SaveMenu = 0;
          iVar29 = 1;
          goto LAB_800330a4;
        }
        Game.name._0_4_ = 0x43524153;
        Game.name[8] = '\0';
        Game.name._4_4_ = 0x48202020;
        new_level = 0x25;
        force_menu = 0x15;
        if (cutmovie == 0) {
          fade_rate = 8;
        }
LAB_800330b0:
        iVar29 = 0x36;
      }
      else if (iVar23 < 2) {
        if (iVar23 == 0) {
          MemCardRetry();
          iVar29 = ParseNintendoErrorCode();
          if (iVar29 == 0) {
            NewMenu(cursor,4,0,-1);
            goto LAB_800330b0;
          }
          DestMenu = 4;
          iVar29 = 1;
          SaveMenu = iVar23;
LAB_800330a4:
          NewMenu(cursor,iVar29,0,-1);
          goto LAB_800330b0;
        }
      }
      else {
        if (iVar23 == 2) {
          iVar29 = 0x27;
          goto LAB_800330a4;
        }
        if (iVar23 == 3) {
          old_lang.356 = (int)Game.language;
          Game.language = (uchar)(old_lang.356 + 1U);
          if (5 < (old_lang.356 + 1U & 0xff)) {
            Game.language = '\0';
          }
        }
      }
LAB_80033140:
      if ((uint)Game.language != old_lang.356) {
        NewLanguage((uint)Game.language);
        iVar29 = 0x36;
        old_lang.356 = (int)Game.language;
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x11') {
      if (bVar1) goto LAB_800346ac;
      iVar29 = (int)cursor->y_max;
      if (iVar23 == iVar29) {
        iVar29 = newleveltime_slot * 8;
        iVar22 = Level * 0x1c + -0x7fcfdbf0;
        pcVar19 = (char *)(iVar22 + iVar29);
        CleanLetters(pcVar19);
        iVar18 = strcmp(pcVar19,"   ");
        if (iVar18 == 0) {
          *(char (*) [4])(iVar22 + iVar29) = (char  [4])0x20203f00;
        }
        if ((new_lev_flags & 7) == 0) {
          iVar29 = 0x10;
          goto LAB_8003469c;
        }
        new_level = 0x25;
        goto LAB_800346a8;
      }
      if (Game.language == 'c') {
        iVar18 = 2;
        if ((iVar23 != iVar29 + -1) && (iVar18 = 1, iVar23 != iVar29 + -2)) {
          if (iVar23 != iVar29 + -3) {
            if ((input_alphabet == 2) && ((iVar23 == 0 || (iVar23 == 4)))) goto LAB_800346a8;
            goto LAB_80033998;
          }
          iVar18 = 0;
        }
        input_alphabet = iVar18;
        GetMenuInfo(cursor);
      }
      else {
LAB_80033998:
        InputNewLetter(cursor,(char *)(Level * 0x1c + -0x7fcfdbf0 + newleveltime_slot * 8),
                       &i_nameinput,3);
      }
      goto LAB_800346a8;
    }
    if (cVar14 < '\x12') {
      if (cVar14 == '\x0e') {
        if (!bVar1) {
          iVar18 = (int)(char)(&HData[cursor->x * 3].flags)[iVar23];
          if (iVar18 != -1) {
            new_level = iVar18;
            Hub = HubFromLevel(iVar18);
          }
          goto LAB_800346ac;
        }
        if (uVar5 == 0) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if ('\x0e' < cVar14) {
        if (cVar14 != '\x0f') {
          if (cVar14 != '\x10') goto LAB_800346ac;
LAB_80033850:
          if (bVar1) goto LAB_800346ac;
          iVar29 = 0x36;
          if (iVar23 != 0) {
            if (iVar23 != 1) goto LAB_800346ac;
            goto LAB_8003389c;
          }
          new_mode = GameMode;
          ResetCheckpoint(-1,-1,0.0,(nuvec_s *)0x0);
          goto LAB_8003388c;
        }
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar22 = -1;
          iVar18 = 0xb;
          goto LAB_80034684;
        }
        if (iVar23 == 2) {
LAB_80032a44:
          iVar18 = logos_played;
          iVar22 = iVar23;
        }
        else if (iVar23 < 3) {
          iVar18 = iVar23;
          iVar22 = iVar23;
          if ((iVar23 != 0) && (iVar18 = logos_played, iVar22 = cutmovie, iVar23 == 1))
          goto LAB_80032a44;
        }
        else if ((iVar23 == 3) || (iVar18 = logos_played, iVar22 = cutmovie, iVar23 == 4))
        goto LAB_80032a44;
        cutmovie = iVar22;
        logos_played = iVar18;
        NewMenu(cursor,-1,-1,-1);
        goto LAB_8003389c;
      }
      if (cVar14 != '\f') {
        if ((cVar14 != '\r') || (uVar5 == 0)) goto LAB_800346ac;
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if (!bVar1) goto LAB_800346ac;
      if (uVar5 != 0) {
        iVar22 = -1;
        iVar18 = 0xb;
        goto LAB_80034684;
      }
      if (iVar23 == 5) {
        if ((uVar6 == 0) || (LDATA->fogb == '\0')) {
          if ((uVar7 != 0) && (LDATA->fogb != 0xff)) {
            LDATA->fogb = LDATA->fogb + '\x01';
          }
        }
        else {
          LDATA->fogb = LDATA->fogb + 0xff;
        }
        goto LAB_800346ac;
      }
      if (5 < iVar23) {
        if (iVar23 == 8) {
          if ((uVar6 == 0) || (LDATA->hazeg == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazeg != 0xff)) {
              LDATA->hazeg = LDATA->hazeg + '\x01';
            }
          }
          else {
            LDATA->hazeg = LDATA->hazeg + 0xff;
          }
        }
        else if (iVar23 < 9) {
          if (iVar23 == 6) {
            if ((uVar6 == 0) || (LDATA->foga == '\0')) {
              if ((uVar7 != 0) && (LDATA->foga < 0x7f)) {
                LDATA->foga = LDATA->foga + 1;
              }
            }
            else {
              LDATA->foga = LDATA->foga + 0xff;
            }
          }
          else if (iVar23 == 7) {
            if ((uVar6 == 0) || (LDATA->hazer == '\0')) {
              if ((uVar7 != 0) && (LDATA->hazer != 0xff)) {
                LDATA->hazer = LDATA->hazer + '\x01';
              }
            }
            else {
              LDATA->hazer = LDATA->hazer + 0xff;
            }
          }
        }
        else if (iVar23 == 9) {
          if ((uVar6 == 0) || (LDATA->hazeb == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazeb != 0xff)) {
              LDATA->hazeb = LDATA->hazeb + '\x01';
            }
          }
          else {
            LDATA->hazeb = LDATA->hazeb + 0xff;
          }
        }
        else if (iVar23 == 10) {
          if ((uVar6 == 0) || (LDATA->hazea == '\0')) {
            if ((uVar7 != 0) && (LDATA->hazea != 0xff)) {
              LDATA->hazea = LDATA->hazea + '\x01';
            }
          }
          else {
            LDATA->hazea = LDATA->hazea + 0xff;
          }
        }
        goto LAB_800346ac;
      }
      if (iVar23 == 2) {
        if (((uVar6 == 0) || (fVar21 = LDATA->fogfar, fVar21 <= 1.0)) || (fVar21 <= LDATA->fognear ))
        {
          if ((uVar7 != 0) && ((int)LDATA->fogfar < (int)(uint)LDATA->farclip)) {
            LDATA->fogfar = LDATA->fogfar + 1.0;
          }
        }
        else {
          LDATA->fogfar = fVar21 - 1.0;
        }
        goto LAB_800346ac;
      }
      if (2 < iVar23) {
        if (iVar23 == 3) {
          if ((uVar6 == 0) || (LDATA->fogr == '\0')) {
            if ((uVar7 != 0) && (LDATA->fogr != 0xff)) {
              LDATA->fogr = LDATA->fogr + '\x01';
            }
          }
          else {
            LDATA->fogr = LDATA->fogr + 0xff;
          }
        }
        else if (iVar23 == 4) {
          if ((uVar6 == 0) || (LDATA->fogg == '\0')) {
            if ((uVar7 != 0) && (LDATA->fogg != 0xff)) {
              LDATA->fogg = LDATA->fogg + '\x01';
            }
          }
          else {
            LDATA->fogg = LDATA->fogg + 0xff;
          }
        }
        goto LAB_800346ac;
      }
      if (iVar23 != 0) {
        if (iVar23 == 1) {
          if ((uVar6 == 0) || (LDATA->fognear <= 1.0)) {
            if ((uVar7 != 0) &&
               ((fVar21 = LDATA->fognear, (int)fVar21 < (int)(uint)LDATA->farclip &&
                (fVar21 < LDATA->fogfar)))) {
              LDATA->fognear = fVar21 + 1.0;
            }
          }
          else {
            LDATA->fognear = LDATA->fognear - 1.0;
          }
        }
        goto LAB_800346ac;
      }
      if ((uVar6 == 0) || (LDATA->farclip < 0xb)) {
        if ((uVar7 != 0) && (LDATA->farclip < 1000)) {
          uVar12 = LDATA->farclip + 1;
          goto LAB_80032b44;
        }
      }
      else {
        uVar12 = LDATA->farclip - 1;
LAB_80032b44:
        LDATA->farclip = uVar12;
      }
      if ((int)(uint)LDATA->farclip < (int)LDATA->fogfar) {
        local_38 = (double)CONCAT44(0x43300000,(uint)LDATA->farclip);
        LDATA->fogfar = (float)(local_38 - 4503599627370496.0);
      }
      local_38 = (double)CONCAT44(0x43300000,(int)LDATA->fognear ^ 0x80000000);
      if (LDATA->fogfar < (float)(local_38 - 4503601774854144.0)) {
        LDATA->fognear = LDATA->fogfar;
      }
      if (pNuCam != (nucamera_s *)0x0) {
        local_38 = (double)CONCAT44(0x43300000,(uint)LDATA->farclip);
        pNuCam->farclip = (float)(local_38 - 4503599627370496.0);
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x14') {
      if ((TempAnim.newaction == 0x22) && (!bVar1)) {
        if (iVar23 == 0) {
          iVar29 = cortex_continue_i + 1;
          cortex_gameover_i = cortex_continue_i;
          cortex_continue_i = iVar29;
          if (iVar29 == 2) {
            cortex_continue_i = iVar23;
          }
          tempanim_nextaction = (int)*(short *)(cortex_gameover_tab + iVar18 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == (nuAnimData_s *)0x0)) {
            Game.lives = '\x04';
            new_level = 0x25;
            just_continued = 1;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((int)*(short *)(cortex_gameover_tab + iVar18 * 4 + 2),(nuvec_s *)0x0);
            tempanim_waitaudio = 1;
          }
          iVar29 = 0x36;
          gameover_hack = 2;
        }
        else if (iVar23 == 1) {
          iVar29 = cortex_quit_i + 1;
          cortex_gameover_i = cortex_quit_i;
          cortex_quit_i = iVar29;
          if (iVar29 == 7) {
            cortex_quit_i = 2;
          }
          tempanim_nextaction = (int)*(short *)(cortex_gameover_tab + iVar22 * 4);
          if ((CRemap[2] == -1) ||
             (CModel[CRemap[2]].anmdata[tempanim_nextaction] == (nuAnimData_s *)0x0)) {
            new_level = 0x23;
          }
          else {
            gamesfx_channel = 4;
            GameSfx((int)*(short *)(cortex_gameover_tab + iVar22 * 4 + 2),(nuvec_s *)0x0);
            tempanim_waitaudio = iVar23;
          }
          iVar29 = 0x3c;
          gameover_hack = 2;
        }
      }
      goto LAB_800346ac;
    }
    if ('\x14' < cVar14) {
      if (cVar14 != '\x15') {
        if (cVar14 != '\x16') goto LAB_800346ac;
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          NewMenu(cursor,0x26,-1,-1);
        }
        UpdateSaveSlots(cursor);
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0x15;
        }
        else {
          if ((int)cursor->y < (int)cursor->y_max) {
            iVar29 = cursor->x * 2 + (int)cursor->y;
            game = SaveSlot + iVar29;
            if (SaveSlot[iVar29].empty != '\0') goto LAB_800342c4;
            iVar29 = 0x17;
            iVar18 = 1;
            goto LAB_800346a0;
          }
          iVar29 = -1;
          iVar18 = 0x15;
        }
        goto LAB_80034620;
      }
      bVar4 = false;
      loadsaveCallEachFrame();
      if (bVar1) {
        if (uVar5 != 0) {
LAB_80033a6c:
          bVar4 = true;
        }
        goto LAB_80033a70;
      }
      if (iVar23 == 1) {
        memcard_loadattempted = 0;
        UpdateSaveSlots(cursor);
        iVar18 = 0x19;
LAB_80033a54:
        iVar29 = 0x36;
        NewMenu(cursor,iVar18,-1,-1);
      }
      else if (iVar23 < 2) {
        if (iVar23 == 0) {
          memcard_loadattempted = iVar23;
          UpdateSaveSlots(cursor);
          iVar18 = 0x16;
          goto LAB_80033a54;
        }
      }
      else {
        if (iVar23 == 2) {
          memcard_loadattempted = 0;
          UpdateSaveSlots(cursor);
          iVar18 = 0x1d;
          goto LAB_80033a54;
        }
        if (iVar23 == 3) goto LAB_80033a6c;
      }
LAB_80033a70:
      if (bVar4) {
        NewMenu(cursor,-1,-1,-1);
        iVar29 = 0x3c;
        ResetLoadSaveCharacter();
      }
      goto LAB_800346ac;
    }
    if (cVar14 == '\x12') goto LAB_80033850;
    if ((cVar14 != '\x13') || ((((uint)fVar21 & 0x840) == 0 && (cut_on != 0)))) goto LAB_800346ac;
    iVar29 = (int)cursor->new_level;
    if (cursor->new_level == -1) {
      NewMenu(cursor,-1,-1,-1);
      iVar29 = new_level;
    }
  }
  else {
    if (cVar14 == '#') {
      if ((((uint)fVar21 & 0x840) == 0) && (GameTimer.ftime < credit_time)) goto LAB_800346ac;
LAB_8003389c:
      new_level = 0x25;
      goto LAB_800346ac;
    }
    iVar18 = SaveMenu;
    if ('#' < cVar14) {
      if (cVar14 == '*') {
        iVar18 = GBA_Download();
        if (iVar18 == 0) goto LAB_800346ac;
        iVar18 = 0x2b;
LAB_80034680:
        iVar22 = 0;
        goto LAB_80034684;
      }
      if ('*' < cVar14) {
        if (cVar14 != '-') {
          if ('-' < cVar14) {
            if (cVar14 == '0') {
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
              }
              else {
                if (iVar23 != 1) {
                  iVar29 = 0x36;
                  memcard_formatmessage_delay = 0x28;
                  MemCardDelete();
                  memtest_done = 0;
                  iVar18 = 0x31;
                  goto LAB_80034680;
                }
                iVar29 = -1;
              }
            }
            else {
              if ('0' < cVar14) {
                if (cVar14 != '1') goto LAB_800346ac;
                loadsaveCallEachFrame();
                UpdateSaveSlots(cursor);
                if (memcard_formatmessage_delay != 0) goto LAB_800346ac;
                iVar22 = 3;
                iVar18 = SaveMenu;
                goto LAB_80034684;
              }
              if (cVar14 != '/') goto LAB_800346ac;
              if (!bVar1) {
                iVar22 = ParseNintendoErrorCode();
                iVar18 = SaveMenu;
                if (iVar22 == 0) goto LAB_80034680;
                if (iVar23 == 0) {
                  memtest_done = iVar23;
                  MemCardRetry();
                  iVar29 = SaveMenu;
                  goto LAB_8003469c;
                }
                if (iVar23 != 1) {
                  if (iVar23 == 2) {
                    iVar29 = ParseNintendoErrorCode();
                    if (iVar29 == 6) {
                      Managememcard();
                    }
                    else if (iVar29 < 7) {
                      if ((iVar29 < 4) && (1 < iVar29)) {
                        iVar29 = 0x20;
                        iVar18 = 1;
                        goto LAB_800346a0;
                      }
                    }
                    else if (iVar29 == 8) {
                      SaveMenu = 0x15;
                      NewMenu(cursor,0x30,1,-1);
                    }
                  }
                  goto LAB_800346a8;
                }
                iVar29 = 0x15;
                iVar18 = -1;
                goto LAB_800346a0;
              }
              if (uVar5 == 0) goto LAB_800346ac;
              iVar29 = -1;
              iVar18 = BackMenu;
            }
            goto LAB_80034620;
          }
          if (cVar14 == '+') {
            iVar18 = GBA_DownloadEnd();
            if (iVar18 == -1) {
              iVar18 = 0x2d;
            }
            else {
              if (iVar18 != 0) goto LAB_800346ac;
              iVar18 = 0x2c;
            }
            goto LAB_80034680;
          }
          if (cVar14 != ',') goto LAB_800346ac;
        }
        if (bVar1) goto LAB_800346ac;
        iVar29 = 0x27;
        goto LAB_8003469c;
      }
      if (cVar14 == '\'') {
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
        }
        else {
          if (iVar23 == 0) {
            iVar18 = 0x28;
            goto LAB_8003461c;
          }
          if (iVar23 != 1) goto LAB_800346ac;
          iVar29 = 0;
          iVar18 = -1;
        }
      }
      else {
        if ('\'' < cVar14) {
          if (cVar14 == '(') {
            if (!bVar1) {
              iVar29 = 0x29;
              goto LAB_8003469c;
            }
            if (uVar5 == 0) goto LAB_800346ac;
            iVar18 = 0x27;
LAB_8003461c:
            iVar29 = 0;
            goto LAB_80034620;
          }
          if (cVar14 != ')') goto LAB_800346ac;
          GBA_DownloadStart();
          iVar18 = 0x2a;
          goto LAB_80034680;
        }
        if (cVar14 == '$') {
          if (uVar16 == 0) {
            pmVar27 = (player->obj).mask;
            if ((pmVar27 != (mask *)0x0) &&
               ((((pmVar27->anim).flags & 1) != 0 ||
                ((((pmVar27->sfx != -1 && (pmVar27->hold == '\0')) &&
                  (iVar18 = NuSoundKeyStatus(4), iVar18 != 1)) ||
                 ((0x707 < cursor->item_frame || (2 < ((player->obj).mask)->active)))))))) {
              NewMenu(cursor,-1,-1,-1);
            }
          }
          else {
            iVar29 = 0x36;
            NuSoundStopStream(4);
            NewMenu(cursor,-1,-1,-1);
          }
          if (cursor->menu != '$') {
            advice_wait = 0x1e;
          }
          goto LAB_800346ac;
        }
        if (cVar14 != '&') goto LAB_800346ac;
        loadsaveCallEachFrame();
        UpdateSaveSlots(cursor);
        if (bVar1) goto LAB_800346ac;
        if (saveload_cardchanged != 0) {
          saveload_cardchanged = 0;
        }
        saveload_error = 0;
        iVar29 = 0x15;
        iVar18 = -1;
      }
LAB_800346a0:
      NewMenu(cursor,iVar29,iVar18,-1);
LAB_800346a8:
      iVar29 = 0x36;
      goto LAB_800346ac;
    }
    if (cVar14 == '\x1d') {
      loadsaveCallEachFrame();
      if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
        if ((saveload_error == 2) || (saveload_error == 0x10)) {
          saveload_error = 8;
        }
        NewMenu(cursor,0x26,-1,-1);
      }
      UpdateSaveSlots(cursor);
      if (bVar1) {
        if (uVar5 == 0) goto LAB_800346ac;
        iVar29 = -1;
        iVar18 = 0x15;
      }
      else {
        if ((int)cursor->y < (int)cursor->y_max) {
          iVar29 = cursor->x * 2 + (int)cursor->y;
          game = SaveSlot + iVar29;
          if (SaveSlot[iVar29].empty != '\0') {
LAB_800342c4:
            iVar29 = 2;
            goto LAB_800346ac;
          }
          iVar29 = 0x1e;
          iVar18 = 1;
          goto LAB_800346a0;
        }
        iVar29 = -1;
        iVar18 = 0x15;
      }
    }
    else {
      if (cVar14 < '\x1e') {
        if (cVar14 != '\x1a') {
          if (cVar14 < '\x1b') {
            if (cVar14 != '\x18') {
              if (cVar14 != '\x19') goto LAB_800346ac;
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                saveload_error = 4;
                NewMenu(cursor,0x26,-1,-1);
              }
              UpdateSaveSlots(cursor);
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
                iVar18 = 0x15;
              }
              else {
                if (((memcard_formatting != 0) || (memcard_formatme != 0)) ||
                   (memcard_formatmessage_delay != 0)) goto LAB_800346ac;
                if ((int)cursor->y < (int)cursor->y_max) {
                  iVar29 = 0x36;
                  game = SaveSlot + cursor->x * 2 + (int)cursor->y;
                  NewMenu(cursor,0x1b,1,-1);
                  goto LAB_800346ac;
                }
                iVar29 = 1;
                iVar18 = 0x15;
              }
              goto LAB_80034620;
            }
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((saveload_cardchanged == 0) && (saveload_error == 0)) {
              if (memcard_loadresult_delay == 0) {
                NewMenu(cursor,-1,-1,-1);
                pnVar9 = pos_START;
                pcVar8 = player;
                Hub = -1;
                bVar1 = pos_START != (nuvec_s *)0x0;
                tumble_time = 0.0;
                tumble_action = -1;
                tumble_duration = 0.0;
                last_level = -1;
                last_hub = -1;
                (player->obj).hdg = 0x8000;
                if (bVar1) {
                  fVar21 = pnVar9->z;
                  fVar24 = pnVar9->y;
                  (pcVar8->obj).pos.x = pnVar9->x;
                  (pcVar8->obj).pos.z = fVar21;
                  (pcVar8->obj).pos.y = fVar24;
                }
              }
              goto LAB_800346ac;
            }
            iVar22 = 1;
            iVar18 = 0x26;
          }
          else {
            if (cVar14 == '\x1b') {
              loadsaveCallEachFrame();
              if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
                if ((saveload_error == 2) || (saveload_error == 0x10)) {
                  saveload_error = 4;
                }
                NewMenu(cursor,0x26,-1,-1);
              }
              if (bVar1) {
                if (uVar5 == 0) goto LAB_800346ac;
                iVar29 = -1;
                iVar18 = 0x19;
              }
              else {
                if (cursor->y != '\x01') {
                  XbUpdateDateStamp((int)game);
                  if (game->empty != '\0') {
                    memcard_gamesavailable = memcard_gamesavailable + 1;
                  }
                  iVar29 = 0x408;
                  pGVar10 = &Game;
                  pGVar11 = game;
                  do {
                    pGVar28 = pGVar11;
                    pGVar25 = pGVar10;
                    iVar29 = iVar29 + -0x18;
                    *(undefined4 *)pGVar28->name = *(undefined4 *)pGVar25->name;
                    *(undefined4 *)(pGVar28->name + 4) = *(undefined4 *)(pGVar25->name + 4);
                    *(undefined4 *)(pGVar28->name + 8) = *(undefined4 *)(pGVar25->name + 8);
                    *(undefined4 *)&pGVar28->music_volume = *(undefined4 *)&pGVar25->music_volume;
                    pGVar28->hub[0] = pGVar25->hub[0];
                    pGVar28->hub[1] = pGVar25->hub[1];
                    pGVar10 = (Game_s *)(pGVar25->hub + 2);
                    pGVar11 = (Game_s *)(pGVar28->hub + 2);
                  } while (iVar29 != 0);
                  *(undefined4 *)((Game_s *)(pGVar28->hub + 2))->name =
                       *(undefined4 *)((Game_s *)(pGVar25->hub + 2))->name;
                  iVar29 = 0x36;
                  pGVar28->hub[3] = pGVar25->hub[3];
                  pGVar28->hub[4] = pGVar25->hub[4];
                  memcard_saveneeded = 1;
                  NewMenu(cursor,0x1c,0,-1);
                  goto LAB_800346ac;
                }
                iVar29 = -1;
                iVar18 = 0x19;
              }
              goto LAB_80034620;
            }
            if (cVar14 != '\x1c') goto LAB_800346ac;
LAB_800340f0:
            loadsaveCallEachFrame();
            UpdateSaveSlots(cursor);
            if ((((memcard_saveneeded != 0) || (memcard_savestarted != 0)) ||
                (memcard_deleteneeded != 0)) ||
               ((memcard_deletestarted != 0 || (memcard_savemessage_delay != 0)))) {
              memcard_saveresult_delay = 0x3c;
            }
            if ((saveload_cardchanged == 0) && (saveload_error == 0)) {
              if (memcard_saveresult_delay != 0) goto LAB_800346ac;
              iVar22 = 3;
              iVar18 = 0x15;
            }
            else {
              if (((saveload_error == 2) || (saveload_error == 0x10)) &&
                 (saveload_error = 8, cursor->menu == '\x1c')) {
                saveload_error = 4;
              }
              iVar22 = -1;
              iVar18 = 0x26;
            }
          }
          goto LAB_80034684;
        }
        loadsaveCallEachFrame();
        if (saveload_cardchanged != 0) {
          NewMenu(cursor,0x26,-1,-1);
        }
        if (bVar1) {
          if (uVar5 != 0) {
            NewMenu(cursor,0x15,-1,-1);
            iVar29 = 0x3c;
          }
        }
        else {
          iVar29 = (int)cursor->y_max;
          if (iVar23 == iVar29 + -1) {
            CleanLetters(Game.name);
            iVar29 = strcmp(Game.name,"        ");
            if (iVar29 == 0) {
              Game.name._0_4_ = 0x43524153;
              Game.name[8] = '\0';
              Game.name._4_4_ = 0x48202020;
            }
            iVar29 = 0x19;
            iVar18 = 2;
LAB_80033b40:
            NewMenu(cursor,iVar29,iVar18,-1);
          }
          else {
            if (iVar23 == iVar29) {
              iVar29 = 0x15;
              iVar18 = -1;
              goto LAB_80033b40;
            }
            if (Game.language == 'c') {
              iVar18 = 2;
              if ((iVar23 != iVar29 + -2) && (iVar18 = 1, iVar23 != iVar29 + -3)) {
                if (iVar23 != iVar29 + -4) {
                  if ((input_alphabet != 2) || ((cursor->y != '\0' && (cursor->y != '\x04'))))
                  goto LAB_80033bc0;
                  goto LAB_80033bd8;
                }
                iVar18 = 0;
              }
              input_alphabet = iVar18;
              GetMenuInfo(cursor);
            }
            else {
LAB_80033bc0:
              InputNewLetter(cursor,Game.name,&i_nameinput,8);
            }
          }
LAB_80033bd8:
          iVar29 = 0x36;
        }
        if (cursor->menu == '\x15') {
          Game.name._0_4_ = 0x43524153;
          Game.name[8] = '\0';
          Game.name._4_4_ = 0x48202020;
        }
        goto LAB_800346ac;
      }
      if (cVar14 == ' ') {
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
        }
        else {
          if (iVar23 != 1) {
            memcard_formatme = 1;
            iVar29 = 0x21;
            goto LAB_8003469c;
          }
          iVar29 = -1;
        }
      }
      else {
        if (' ' < cVar14) {
          if (cVar14 != '!') {
            if ((cVar14 != '\"') || (uVar16 == 0)) goto LAB_800346ac;
            if ((Level == 0x16) || (Level == 0x18)) {
              boss_dead = 2;
            }
            iVar29 = -1;
            iVar18 = -1;
            goto LAB_800346a0;
          }
          loadsaveCallEachFrame();
          UpdateSaveSlots(cursor);
          if (memcard_formatme != 0) {
            memcard_formatmessage_delay = 0x28;
            goto LAB_800346ac;
          }
          if (memcard_formatmessage_delay != 0) goto LAB_800346ac;
          iVar22 = 3;
          iVar18 = SaveMenu;
          goto LAB_80034684;
        }
        if (cVar14 != '\x1e') {
          if (cVar14 != '\x1f') goto LAB_800346ac;
          goto LAB_800340f0;
        }
        loadsaveCallEachFrame();
        if ((saveload_cardchanged != 0) || (saveload_error != 0)) {
          if ((saveload_error == 2) || (saveload_error == 0x10)) {
            saveload_error = 8;
          }
          NewMenu(cursor,0x26,-1,-1);
        }
        if (bVar1) {
          if (uVar5 == 0) goto LAB_800346ac;
          iVar29 = -1;
          iVar18 = 0x1d;
        }
        else {
          if (cursor->y != '\x01') {
            bVar1 = true;
            iVar29 = 3;
            game->empty = '\x01';
            puVar26 = &SaveSlot[0].empty;
            do {
              uVar13 = *puVar26;
              puVar26 = puVar26 + 0x414;
              if (uVar13 == '\0') {
                bVar1 = false;
              }
              iVar29 = iVar29 + -1;
            } while (iVar29 != 0);
            memcard_gamesavailable = memcard_gamesavailable + -1;
            if (bVar1) {
              memcard_deleteneeded = 1;
            }
            else {
              memcard_saveneeded = 1;
            }
            iVar29 = 0x1f;
            goto LAB_8003469c;
          }
          iVar29 = -1;
          iVar18 = 0x1d;
        }
      }
    }
LAB_80034620:
    NewMenu(cursor,iVar18,iVar29,-1);
    iVar29 = new_level;
  }
  new_level = iVar29;
  iVar29 = 0x3c;
LAB_800346ac:
  if (iVar29 != -1) {
    GameSfx(iVar29,(nuvec_s *)0x0);
  }
  return;
}

//PS2 match
s32 StartHGobjAnim(struct nuhspecial_s *obj)
{
  
  if (((obj->special != NULL) && (obj->special->instance != NULL)) &&
     ((obj->special->instance->anim) != NULL)) 
  {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing | 1;
    return 1;
  }
  return 0;
}

//PS2 Match
s32 ResetHGobjAnim(struct nuhspecial_s *obj)
{ 
  if (((obj->special != NULL && (obj->special->instance != NULL)) &&
     ((obj->special->instance->anim) != NULL))) {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing & 0xfffffffe;
    obj->special->instance->anim->ltime = 1.0;
    return 1;
  }
  return 0;
}

//NGC MATCH
s32 FinishHGobjAnim(struct nuhspecial_s *obj) { 
  if (((obj->special != NULL) && (obj->special->instance != NULL)) &&
     (obj->special->instance->anim != NULL)) {
    (obj->special->instance->anim)->playing = (obj->special->instance->anim)->playing & 2;
    obj->special->instance->anim->ltime = world_scene[0]->instanimdata[obj->special->instance->anim->anim_ix]->time;
    return 1;
  }
  return 0;
}


//vehsupp.c
void GetLocatorMtx(struct CharacterModel *Model,struct numtx_s *LocMtx,f32 YAngOff3)
{
  /*
struct NUPOINTOFINTEREST_s *ptr;
  s32 i;
  struct NUPOINTOFINTEREST_s **pLoc;
  
  if (Model != NULL) {
    i = 0;
    pLoc = Model->pLOCATOR;
    NuMtxSetRotationY(&BaseMat,(s32)(YAngOff3 * 182.0444f));
    NuHGobjEval(Model->hobj,0,0,tmtx);
    do {
      ptr = *pLoc;
      pLoc = pLoc + 1;
      if (ptr != NULL) {
        NuHGobjPOIMtx(Model->hobj,(u8)i,&BaseMat,tmtx,LocMtx);
      }
      i = i + 1;
      LocMtx = LocMtx + 1;
    } while (i < 0x10);
  }
  return;
*/
}

//NGC MATCH
void DefaultGame(void) {
  Game.vibration = 0x1;
  Game.sfx_volume = 0x64;
  Game.music_volume = 'F';
  Game.screen_x = '\0';
  Game.screen_y = '\0';
  Game.language = '\0';
  return;
}

//NGC MATCH
void NewGame(void) {
  s32 save[7];
  
  save[0] = Game.vibration;
  save[2] = Game.sfx_volume;
  save[3] = Game.music_volume;
  save[4] = Game.screen_x;
  save[5] = Game.screen_y;
  save[6] = Game.language;
  memset(&Game,0,0x414);
  Game.vibration = save[0];
  Game.sfx_volume = save[2];
  Game.music_volume = save[3];
  Game.screen_x = save[4];
  Game.screen_y = save[5];
  Game.language = save[6];
  NewLanguage(save[6]);
  memcpy(Game.name, "MIDGET  ", 9);
  DefaultTimeTrialNames(1);
  Game.lives = '\x04';
  Game.hub[0].flags = '\x01';
  Mask.active = (u16)Game.mask;
  Hub = -1;
  return;
}

//NGC MATCH
void StartTransporter(struct obj_s *cyl,struct nuvec_s *pos) {
  cyl->oldpos = *pos;
  cyl->pos = cyl->oldpos;
  cyl->mom = v000;
  cyl->RADIUS = 0.5f;
  cyl->old_SCALE = 1.0f;
  cyl->SCALE = 1.0f;
  cyl->radius = 0.5f;
   cyl->scale = 1.0f;
   cyl->top = 0.0f;
  cyl->bot = -1.0f;
  cyl->hdg = 0;
  return;
}

//NGC MATCH
void FinishTransporter(struct obj_s *cyl,struct obj_s *obj) {
  cyl->oldpos = cyl->pos;
  cyl->mom = v000;
  cyl->dyrot = 0;
  (obj->pos).y = (obj->oldpos).y = (cyl->top * cyl->SCALE + (cyl->pos).y) - obj->bot * obj->SCALE;
  return;
}

//NGC MATCH
void TransporterGo(void) {
  GameSfx(0x82,&(player->obj).pos);
  NewBuzz(&player->rumble,6);
  return;
}

//NGC MATCH
void DrawTransporters(void) {
  s32 i;

  i = 4;
  if ((TimeTrial == 0) && (ChaseActive() == -1)) {
    if ((ObjTab[i].obj.special != NULL) && ((Rail[5].type != -1 && (VEHICLECONTROL != 1)))) {
      Draw3DObject(4,&bonus_obj[0].pos,0,bonus_obj[0].hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,
                   ObjTab[i].obj.special,0);
      Draw3DObject(4,&bonus_obj[1].pos,0,bonus_obj[1].hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,
                   ObjTab[i].obj.special,0);
    }
    if ((Level != 0x25) || ((Game.hub[5].flags & 1) != 0)) {
      i = 5;
      if (LostLife != 0) {
        i = 6;
      }
      if (((ObjTab[i].obj.special != NULL) && (Rail[6].type != -1)) &&
         ((VEHICLECONTROL != 1 || ((player->obj).vehicle == 0x44)))) {
        Draw3DObject(i,&death_obj.pos,0,death_obj.hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,
                     ObjTab[i].obj.special,0);
      }
      i = 8;
      if (gempath_open != 0) {
        i = 7;
      }
      if (((ObjTab[i].obj.special != NULL) && (Rail[7].type != -1)) &&
         ((VEHICLECONTROL != 1 || ((player->obj).vehicle == 0x44)))) {
        Draw3DObject(i,&gempath_obj.pos,0,gempath_obj.hdg,0,1.0f,1.0f,1.0f,ObjTab[i].obj.scene,
                     ObjTab[i].obj.special,0);
      }
    }
  }
  return;
}

//NGC MATCH
void ResetAkuAkuAdvice(void) {
    s32 i;

  for (i = 0; i < 0xd; i++) {
    akuaku_advice_played[i] = '\0';
  }
  return;
}

void ResetItems(void) {
  plr_items = 0;
  plr_bonusgem.count = 0;
  plr_crystal.count = 0;
  plr_crategem.draw = 0;
  plr_crategem.count = 0;
  plr_bonusgem.draw = 0;
  plr_crystal.draw = 0;
  return;
}

//NGC MATCH
void ResetGemPath(void) {
  u8 bits;
  s32 open;
  
  gempath_begun = 0;
  GemPath = 0;
    switch (Level) {
    case 0xc:
        bits = 1;
        break;
    case 0x21:
        bits = 2;
        break;
    case 0xe:
        bits = 4;
        break;
    case 4:
        bits = 8;
        break;
    case 0x14:
        bits = 0x10;
        break;
    default:
        bits = 0;
        break;
    }
    if ((Game.gembits & bits) == 0) {
        open = 0;
    } else{
        open = 1;    
    }
    gempath_open = open;
  if (ObjTab[101].obj.special != NULL) {
     (ObjTab[101].obj.special)->instance->flags.visible = ((1 - gempath_open));
  }
  return;
}

//NGC MATCH
void ResetDeath(void) {
  struct rail_s* rail;

  death_begun = 0;
  Death = 0;
  rail = &Rail[6];

  if ((((rail->type != -1) && (Level == 0x25)) && (Hub == 5)) &&
     ((last_level != 0x26 && (last_level != -1)))) {
    StartTransporter(&death_obj,(struct nuvec_s *)(&(rail->pINPLAT)->pts[((rail->pINPLAT)->len + -1) * (s32)(rail->pINPLAT)->ptsize]));
    Death = 2;
    death_begun = 1;
  }
  return;
}

void ResetBonus(void) {
    Bonus = 0;
}

//NGC MATCH
void ResetGame(void) {
  s32 i;
  
  memset(&Cursor,0,0x80);
  for(i = 0; i < 0x2c; i++) {
    if (0x3de < LData[i].farclip - 10U) {
      LData[i].farclip = 1000;
    }
  }
  for(i = 0; i < 0x23; i++) {
    LData[i].hub = HubFromLevel(i);
  }

  for(i = 0; i < 1; i++) {
    Mask[i].character = 3;
  }
  ResetTimer(&GlobalTimer);
  InitProbe();
  return;
}

void ResetKabooms(void) {
  memset(Kaboom,0,0xa20);
  i_kaboom = 0;
  return;
}

//NGC MATCH
void OpenGame(void) {
  s32 i;
  s32 j;
  s32 k;
  
  for (k = 0; k < 0x23; k++) {
    Game.level[k].flags = 0;
  }
  for (k = 0; k < 6; k++) {
    Game.hub[k].flags = 0xff;
    Game.hub[k].crystals = '\0';
    if (k < 5) {
      for (i = 0, j = 0; i < 6; i++, j++) {
        if (HData[k].level[i] != -1) {
          if (j < 5) {
            Game.level[HData[k].level[i]].flags = 0x1f;
            Game.hub[k].crystals = Game.hub[k].crystals + '\x01';
          }
          else {
            Game.level[HData[k].level[i]].flags = 0x800;
          }
        }
      }
    }
    else {
      for (i = 0; i < 5; i++) {
        if (HData[k].level[i] != -1) {
          Game.level[HData[k].level[i]].flags = 0x17;
        }
      }
    }
  }
  Game.level[1].flags = Game.level[1].flags | 0x40;
  Game.level[17].flags = Game.level[17].flags | 0x80;
  Game.level[7].flags = Game.level[7].flags | 0x100;
  Game.level[19].flags = Game.level[19].flags | 0x200;
  Game.level[10].flags = Game.level[10].flags | 0x400;
  Game.level[4].flags = Game.level[4].flags | 0x20;
  Game.level[12].flags = Game.level[12].flags | 0x20;
  Game.level[14].flags = Game.level[14].flags | 0x20;
  Game.level[5].flags = Game.level[5].flags | 0x20;
  Game.level[20].flags = Game.level[20].flags | 0x20;
  Game.level[33].flags = Game.level[33].flags | 0x20;
  Game.level[27].flags = Game.level[27].flags | 0x20;
  Game.level[28].flags = Game.level[28].flags | 0x20;
  Game.level[29].flags = Game.level[29].flags | 0x20;
  Game.level[32].flags = Game.level[32].flags | 0x20;
  Game.level[9].flags = Game.level[9].flags | 0x20;
  Game.powerbits = 0xff;
  Game.cutbits = (u32)-1;
  CalculateGamePercentage(&Game);
  ResetBonus();
  ResetDeath();
  ResetGemPath();
  return;
}

//NGC MATCH
void InitGameMode(s32 mode) {
  ResetTimer(&GameTimer);
  finish_frame = 0;
  ResetTimer(&PauseTimer);
  pausestats_frame = 0;
  ResetTimer(&MenuTimer);
  ResetTimeTrial();
  if (force_menu != -1) {
    NewMenu(&Cursor,force_menu,-1,-1);
    force_menu = -1;
  }
  else {
    if ((LDATA->flags & 1) != 0) {
      start_time = 3.0f;
      NewMenu(&Cursor,-1,-1,-1);
    }
    else {
      start_time = 0.0f;
    }
  }
  new_mode = -1;
  new_level = -1;
  ResetPlayer(1);
  ResetTubs();
  plr_lives.count = (short)Game.lives;
  plr_lives.draw = plr_lives.count;
  switch (Level) {                        
    case 0x25:
        if (GameMode == 1) {
            InitGameCut();
            return;
        }
        return;
    case 0x17:
        ResetDRAINDAMAGE();
        return;
    case 0x19:
        ResetCRUNCHTIME();
        return;
    case 0x28:
        InitGameCut();
        break;
  }
  return;
}

//NGC MATCH
void InitPositions(void) {
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  
  pos_START = NULL;
  SAFEY = -1000.0f;
  if ((SplTab[0].spl != NULL) && (0 < (SplTab[0].spl)->len)) {
    pos_START = (struct nuvec_s *)(SplTab[0].spl)->pts;
  }
  if (((Level == 0x1c) && (SplTab[29].spl != NULL)) && (SplTab[45].spl != NULL)) {
    p0 = (struct nuvec_s*)(SplTab[29].spl)->pts;
    p1 = (struct nuvec_s*)(SplTab[45].spl)->pts;
    start_pos.x = (p0->x + p1->x) * 0.5f;
    start_pos.y = (p0->y + p1->y) * 0.5f;
    start_pos.z = (p0->z + p1->z) * 0.5f;
    pos_START = &start_pos;
    return;
  }
  if ((((LDATA->vSTART).x != 0.0f) || ((LDATA->vSTART).y != 0.0f)) || ((LDATA->vSTART).z != 0.0f)) {
     pos_START = &LDATA->vSTART;
  }
  return;
}

//NGC MATCH
void InitLevel(void) {
  s32 lp;
  s32 y;
  s32 menu;
  s32 level;
  struct nuvec_s start;
  struct nuvec_s end;
  struct nuinstance_s *ipost;
  struct nuinstance_s *instgrp[24];
  
  InitPositions();
  ResetTempCharacter(-1,-1);
  menu = -1;
  ResetTempCharacter2(-1,-1);
  y = -1;
  level = -1;
  tempanim_waitaudio = 1;
    switch(Level) {
        case 0x22:
        break;
            case 0x23:
                          NewGame();
                          y = 0;
                          Hub = -1;
                          menu = 0;
                          ResetTempCharacter(0x60,0x2b);
                          Demo = 0;
            break;
            case 0x25:
                      CalculateGamePercentage(&Game);
                      ResetTempCharacter(2,0x22);
                      hub_vr_level = 0xffffffff;
                      loadsave_frame = 0x3d;
                      warp_level = 0xffffffff;
            break;
            case 0x27:
                        menu = 0x13;
                        level = 0x25;
            break;

            case 0x29:
                        menu = 0x13;
                        level = 0x2b;
            break;
            case 0x26:
                        ResetTempCharacter(2,0x22);
                        menu = 0x14;
                        tempanim_nextaction = 0x73;
                        y = 0;
                        gamesfx_channel = 4;
                        tempanim_waitaudio = 1;
                        cortex_gameover_i = -1;      
            break;
            case 0x2b:
                        InitCredits();
                        menu = 0x23;
                        ResetTempCharacter(0xbb,0x22);
                        level = 0x2b;
                        ResetTempCharacter2(0,0x22);
            break;
            case 0xd:
                ResetTempCharacter(0x62,0x22);
            break;
            case 0x1a:
                ResetTempCharacter(0xb8,0x22);
            break;
            case 0x12:
                ResetTempCharacter(0xb9,0x22);
            break;
            case 0x1e:
                        ResetTempCharacter(0xba,0x22);
            break;
            case 0x18:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x15:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x16:
                ResetTempCharacter(0xbc,0x22);
            break;
            case 0x19:
                ResetTempCharacter(2,0x22);
            break;
            case 0x13:
                        if ((ObjTab[145].obj.special != NULL) && (ObjTab[146].obj.special != NULL)) {
                          for (lp = 0; lp < 0x18; lp++) {
                            if (lp & 1) {
                                instgrp[lp] = ObjTab[145].obj.special->instance;
                            } else{
                              instgrp[lp] = ObjTab[146].obj.special->instance;
                            }
                          }
                          start.x = 36.3721f;
                          start.y = -0.1f;
                          start.z = 140.7f;
                          end.x = 36.49f;
                          end.y = -0.1f;
                          end.z = 146.84f;
                          NuBridgeCreate(instgrp,NULL,&start,&end,1.2f,-0x4200,0.3f,0.075f,
                                         -0.01f,4.0f,10,1.0f,0.5f,3,-0x7f7f7f80);
                          start.x = 206.1f;
                          start.y = 0.0f;
                          start.z = 249.8f;
                          end.x = 213.25f;
                          end.y = 0.0f;
                          end.z = 249.8f;
                          NuBridgeCreate(instgrp,NULL,&start,&end,1.2f,0,0.25f,0.08f,-0.005f, 7.0f
                                         ,0xc,1.0f,0.5f,3,-0x7f7f7f80);
                        }
            break;
            case 8:
                        if (ObjTab[155].obj.special != NULL) {
                          if (ObjTab[158].obj.special != NULL) {
                            ipost = (ObjTab[158].obj.special)->instance;
                          } else{
                              ipost = NULL;
                          }
                          for (lp = 0; lp < 0x18; lp++) {
                            instgrp[lp] = (ObjTab[155].obj.special)->instance;
                          }
                          start.x = -79.875f;
                          start.y = -14.2f;
                          start.z = 22.7f;
                          end.x = -79.875f;
                          end.y = -14.2f;
                          end.z = 26.0f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,-0x4000,0.18f,0.1f,-0.01f,4.0f,7,1.08f,
                                         0.5f,3,-0x7f7f7f80);
                          start.x = 52.5f;
                          start.y = -33.7f;
                          start.z = -73.45f;
                          end.x = 63.9f;
                          end.y = -33.7f;
                          end.z = -73.14f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,0,0.4,0.05,-0.004,8.0,0x11,1.08f,0.5f
                                         ,3,-0x7f7f7f80);
                          start.x = 70.0;
                          start.y = -33.7f;
                          start.z = -73.12f;
                          end.x = 77.0;
                          end.y = -33.7f;
                          end.z = -73.3f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,0,0.25f,0.075f,-0.008f,4.0f,10,1.08f,0.5f
                                         ,3,-0x7f7f7f80);
                        }
            break;
            case 1:
                        if (((ObjTab[159].obj.special != NULL)) && (ObjTab[160].obj.special != NULL)) {
                          ipost = NULL;
                          if (ObjTab[161].obj.special != NULL) {
                            ipost = (ObjTab[161].obj.special)->instance;
                          }
                          for (lp = 0; lp < 0x18; lp++) {
                            if (lp == (lp / 3) * 3) {
                                instgrp[lp] = ObjTab[159].obj.special->instance;
                            } else{
                              instgrp[lp] = ObjTab[160].obj.special->instance;
                            }
                          }
                          start.x = -1.31f;
                          start.y = 18.6f;
                          start.z = 16.23f;
                          end.x = -7.14f;
                          end.y = 18.6f;
                          end.z = 14.8f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,31000,0.12f,0.1f,-0.01f,4.0f,7,1.12f, 0.9f
                                         ,3,-0x7fffcfa8);
                          start.x = -8.47f;
                          start.y = 18.6f;
                          start.z = 14.4f;
                          end.x = -13.17f;
                          end.y = 18.6f;
                          end.z = 10.09f;
                          NuBridgeCreate(instgrp,ipost,&start,&end,1.2f,25000,0.12f,0.1f,-0.01f,4.0f,7,1.12f, 0.9f
                                         ,3,-0x7fffcfa8);
                        }
            break;
        }
  if ((LBIT & 0x3e00000) != 0) {
    Mask->active = 0;
  }
  ai_lookpos = v000;
  Cursor.menu = -1;
  NewMenu(&Cursor,menu,y,level);
  VEHICLECONTROL = 0;
  if ((Level == 6) || (Level == 0x1d)) {
    VEHICLECONTROL = 1;
  }
  else if (Level == 0x22) {
    VEHICLECONTROL = 2;
  }
  else {
    VEHICLECONTROL = 0;
  }
  level_part_2 = 0;
  SKELETALCRASH = 0;
  if (Level != 0x25) {
    new_hub_flags = '\0';
    new_lev_flags = 0;
  }
  ResetItems();
  boss_dead = 0;
  COMPLEXPLAYERSHADOW = (s32)(Level == 0xc);
  bonusgem_ok = (u32)(Level != 5);
  ResetGemPath();
  hubleveltext_level = -1;
  hubleveltext_pos = 0.0f;
  tumble_moveduration = 1.0f;
  tumble_duration = 1.0f;
  tumble_time = 1.0f;
  HubLevelText = NULL;
  InitBugAreas();
  bonus_restart = 0;
  plr_invisibility_time = 5.0f;
  in_finish_range = 0;
  LivesLost = 0;
  if ((LBIT & 0x3e00000) != 0) {
    Mask->active = 0;
  }
  ResetAkuAkuAdvice();
  return;
}

//NGC MATCH
void UpdateLevel(void) {
  s32 sfx;
  s32 i;

  for (i = 0; i < 5; i++) {
    lev_ambpos[i] = (player->obj).pos;
  }
  sfx = -1;
    switch ((s32) Level) {
    case 5:
        sfx = 0xB5;
        break;
    case 0x17:
        UpdateDRAINDAMAGE();
    case 0x26:
        sfx = 0xB9;
        break;
    case 0x19:
        sfx = UpdateCRUNCHTIME();
        break;
    case 2:
        if ((s32) VEHICLECONTROL == 0) {
            sfx = 0xB4;
        }
        break;
    case 8:
        gamesfx_effect_volume = 0x3FFF;
        GameSfxLoop(0xB7, 0);
    case 29:
    case 33:
    case 20:
    case 16:
    case 9:
    case 12:
    case 7:
    case 4:
    case 0:
        sfx = 0xB3;
        break;
    case 0xe:
        sfx = 0xC0;
        break;
    case 0x13:
        sfx = 0xBC;
        break;
    }
  if (sfx != -1) {
    gamesfx_effect_volume = 0x1fff;
    GameSfxLoop(sfx,NULL);
  }
  return;
}

//NGC MATCH
void DrawLevel(void) {
    switch (Level) {                          
    case 0x25:
        HubDrawItems();
        return;
    case 0x19:
        DrawCRUNCHTIME();
        return;
    case 0x17:
        DrawDRAINDAMAGE();
        return;
    }
}

void ResetLevel(void) {
  ResetVehicleLevel(1);
  return;
}

//NGC MATCH
void CleanLetters(char *txt) {
    while (*txt != '\0') {
      if (*txt == '_') {
        *txt = ' ';
      }
      *txt++;
      if (Game.language == 'c') {
        *txt++;
      }
    }
  return;
}

void DrawNameInputTable(struct Cursor *cursor,float x0,float y0)
{
  s32 align;
  s32 colour;
  s32 col;
  s32 j;
  char *tmpchar;
  float y;
  float x;
  s32 i;
  
  if (Game.language == 'c') {
    tbuf[2] = '\0';
    tbuf[1] = ' ';
  }
  else {
    tbuf[1] = '\0';
  }
  y = y0 - (float)(cursor->y_max - cursor->y_min) * MENUDY * 0.5f;
  i = 0;
  do {
    j = i + 1;
    x = x0 - (float)(cursor->x_max - cursor->x_min) * 0.06f * 0.5f;
    if (cursor->x_min <= cursor->x_max) {
      tmpchar = (char *)((s32)NameInputTable + cursor->x_min + i * 7);
      do {
        tbuf[0] = *tmpchar;
        align = 1;
        tmpchar = tmpchar + 1;
        if ((i == cursor->y) && (cursor->x_min == cursor->x)) {
          align = 0x21;
          col = 0;
          if (5 < GlobalTimer.frame % 0xc) {
            col = 3;
          }
        }
        else {
          col = 2;
        }
        cursor->x_min = cursor->x_min + 1;
        Text3D(tbuf,x,y,1.0f,0.6f,0.6f,0.6f,align,col);
        x = x + 0.06000000238418579f;
      } while (cursor->x_min <= cursor->x_max);
    }
    x = y + MENUDY;
    y = x;
    i = j;
  } while (j < 4);
  if ('\x03' < cursor->y_max) {
    i = 1;
    if (cursor->y == '\x04') {
      i = 0x21;
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
    }
    else {
      col = 2;
    }
    Text3D(tDONE[Game.language],x0,x,1.0f,0.6f,0.6f,0.6f,i,col);
    y = y + MENUDY;
  }
  if ('\x04' < cursor->y_max) {
    i = 1;
    if (cursor->y == '\x05') {
      i = 0x21;
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
    }
    else {
      col = 2;
    }
    Text3D(tCANCEL[Game.language],x0,y,1.0f,0.6f,0.6f,0.6f,i,col);
  }
  if (((cutmovie == -1) && (cursor->menu != '\x04')) && (cursor->menu != '\x1a')) {
    DrawPanel3DObject(0x81,x0,y0,1.0f,0.11875f,0.125f,0.11875f,0xc000,0,0,ObjTab[129].obj.scene,ObjTab[129].obj.special,0);
  }
  return;
}

//NGC MATCH
void DrawMenuEntry(struct cursor_s *cursor,char *txt,float *x,float *y,s32 *i) {
  s32 align;
  s32 col;
  float size;
  
  if ((*i >= cursor->y_min) && (*i <= cursor->y_max)) {
    col = *i;
    align = 1;
    size = dme_sy * 0.6f;
    if (col == cursor->y) {
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
      align = 0x21;
    }
    else {
      col = 2;
    }
    Text3D(txt,*x,*y + dme_yadj,1.0f,size * dme_sx,size * dme_symul,size,align,col);
  }
  NextMenuEntry(y,i);
  dme_sx = 1.0f;
  dme_sy = 1.0f;
  dme_symul = 1.0f;
  dme_yadj = 0.0f;
  return;
}

//NGC MATCH
void DrawMenuEntry2(struct cursor_s *cursor,char *txt0,char *txt1,float *x,float *y,s32 *i) {
  s32 align;
  s32 col;
  float sx2;
  float size;
 
  if ((*i >= cursor->y_min) && (*i <= cursor->y_max)) {
    align = 1;
    size = dme_sy * 0.6f;
    sx2 = size;
    if (*i == cursor->y) {
      col = 0;
      if (5 < GlobalTimer.frame % 0xc) {
        col = 3;
      }
      align = 0x21;
    }
    else {
      col = 2;
    }
    Text3D(txt0,*x,*y,1.0f,(sx2 * dme_sy),size,size,align,col);
    size = MENUDY * dme_sy + *y;
    *y = size;
    Text3D(txt1,*x,size,1.0f,(sx2 * dme_sx),sx2,sx2,align,col);
  }
  NextMenuEntry(y,i);
  dme_sx = 1.0f;
  dme_sy = 1.0f;
  return;
}

//NGC MATCH
void NextMenuEntry(float *y, s32 *i) {
  *y += MENUDY * dme_sy;
  *i += 1;
  return;
}

void NewMenu(Cursor *cur,s32 menu,s32 y,s32 level)
{
  s32 old_menu;
  s32 lock;
 
  cur->wait = '\0';
  lock = '\0';
  if (menu == cur->menu) {
    menu = -1;
  }
  if ((menu == cur->new_menu) && (cur->wait_hack != '\0')) goto LAB_80031bb4;
  if (menu == 0x14) {
    gameover_hack = 0;
    goto LAB_80031bb4;
  }
  if (menu < 0x15) {
    if (8 < menu) {
      if (menu == 0x11) {
        i_nameinput = 0;
      }
      goto LAB_80031bb4;
    }
    if (5 < menu) goto LAB_80031bb4;
    if (menu == -1) {
      if ((cur->menu == 0x15) || (cur->menu == 0x18)) {
        cur->wait = '<';
      }
      goto LAB_80031bb4;
    }
    if (menu != 4) goto LAB_80031bb4;
LAB_80031b54:
    y = 4;
    Game.name[0] = 'S';
    Game.name[1] = 'A';
    Game.name[2] = 'R';
    Game.name[3] = 'C';
    Game.name[8] = '\0';
    Game.name[4] = '_';
    Game.name[5] = '_';
    Game.name[6] = '_';
    Game.name[7] = 'H';
    cur->y = '\x04';
    i_nameinput = 0;
  }
  else {
    if (menu != 0x19) {
      if (menu < 0x1a) {
        if ((menu == 0x15) || (menu != 0x16)) goto LAB_80031bb4;
      }
      else if (menu != 0x1d) {
        if (0x1d < menu) {
          if (menu == 0x22) {
            lock = -0x4c;
          }
          goto LAB_80031bb4;
        }
        if (menu != 0x1a) goto LAB_80031bb4;
        goto LAB_80031b54;
      }
    }
    InitLoadSaveDeleteScreen(cur,menu);
  }
LAB_80031bb4:
  old_menu = menu;
  if (cur->wait == '\0') {
    ResetTimer(&MenuTimer);
    cur->new_level = (char)level;
    cur->new_menu = -1;
    cur->wait = '\0';
    cur->button_lock = lock;
    cur->menu = old_menu;
    if (old_menu != -1) {
      GetMenuInfo(cur);
      cur->x = cur->remember[menu].x;
      if (cur->remember[menu].x < cur->x_min) {
        cur->x = cur->x_min;
      }
      else if (cur->x_max < cur->remember[menu].x) {
        cur->x = cur->x_max;
      }
      if ((y < cur->y_min) || (cur->y_max < y)) {
        cur->y = cur->remember[menu].y;
        if (cur->remember[menu].y < cur->y_min) {
          cur->y = cur->y_min;
        }
        else if ((s32)cur->y_max < cur->remember[menu].y) {
          cur->y = cur->y_max;
        }
      }
      else {
        cur->y = (char)y;
      }
      lock = cur->menu;
      if (((lock == '\x04') || (lock == '\x1a')) || (lock == '\x11')) {
        cur->x = '\0';
      }
      cur->wait_hack = '\0';
      cur->menu_frame = 0;
      cur->item_frame = 0;
    }
  }
  else {
    cur->wait_frames = cur->wait;
    cur->new_menu = old_menu;
  }
  return;
}


void GetMenuInfo(Cursor *cur)

{
  int err;
  char cVar1;
  char menu;
  
  cVar1 = '\0';
  menu = cur->menu;
  cur->x_min = '\0';
  cur->y_min = '\0';
  cur->x_max = '\0';
  cur->y_max = '\0';
  if (menu == '\x15') {
    menu = '\x03';
    goto LAB_80031a44;
  }
  if (menu < '\x16') {
    if (menu == '\t') {
      menu = '\x05';
      goto LAB_80031a44;
    }
    if ('\t' < menu) {
      if (menu != '\x0f') {
        if ('\x0f' < menu) {
          if (menu == '\x11') {
            cur->x_max = '\x06';
            cur->y_max = '\x04';
            if (Game.language != 'c') {
              return;
            }
            menu = '\a';
          }
          else if ((menu < '\x11') || (menu == '\x12')) {
            menu = '\x01';
          }
          else {
            if (menu != '\x14') {
              return;
            }
            menu = '\x01';
          }
          goto LAB_80031a44;
        }
        if (menu == '\v') {
          menu = '\n';
          goto LAB_80031a44;
        }
        if (menu < '\v') {
          menu = '\x01';
          goto LAB_80031a44;
        }
        if (menu == '\f') {
          menu = '\n';
          goto LAB_80031a44;
        }
        if (menu != '\x0e') {
          return;
        }
        menu = '\x05';
        goto LAB_80031a24;
      }
      menu = '\x04';
      goto LAB_80031a44;
    }
    if (menu != '\x04') {
      if (menu < '\x05') {
        if (menu == '\x01') {
LAB_800318e8:
          err = ParseNintendoErrorCode();
          if (err == 6) {
            menu = '\x02';
          }
          else {
            if (err < 7) {
              if ((err < 4) && (1 < err)) {
                menu = '\x02';
                goto LAB_80031a44;
              }
            }
            else if (err == 8) {
              menu = '\x02';
              goto LAB_80031a44;
            }
            menu = '\x01';
          }
          goto LAB_80031a44;
        }
        if ('\x01' < menu) {
          return;
        }
        err = LANGUAGEOPTION;
        if (menu != '\0') {
          return;
        }
      }
      else {
        if (menu == '\x06') {
          cur->y_max = '\x02';
          if (LANGUAGEOPTION == 0) {
            return;
          }
          menu = '\x03';
          goto LAB_80031a44;
        }
        err = TimeTrial;
        if ('\x05' < menu) {
          if (menu == '\a') {
            menu = '\x02';
          }
          else {
            if (menu != '\b') {
              return;
            }
            menu = '\x02';
          }
          goto LAB_80031a44;
        }
      }
      cVar1 = '\x02';
      if (err != 0) {
        cVar1 = '\x03';
      }
LAB_80031a38:
      cur->y_max = cVar1;
      return;
    }
LAB_800319a8:
    cur->x_max = '\x06';
    cur->y_max = '\x05';
    if (Game.language != 'c') {
      return;
    }
    menu = '\b';
LAB_80031a44:
    cur->y_max = menu;
    return;
  }
  if (menu != '\x1f') {
    if (menu < ' ') {
      if (menu != '\x1a') {
        if (menu < '\x1b') {
          if (menu != '\x17') {
            if ('\x16' < menu) {
              if (menu == '\x18') goto LAB_80031a20;
              if (menu != '\x19') {
                return;
              }
            }
LAB_80031a00:
            cVar1 = '\x01';
            menu = '\x02';
            goto LAB_80031a14;
          }
        }
        else {
          if (menu == '\x1c') goto LAB_80031a20;
          if ('\x1b' < menu) {
            if (menu == '\x1d') goto LAB_80031a00;
            if (menu != '\x1e') {
              return;
            }
          }
        }
LAB_80031a0c:
        cVar1 = '\0';
        menu = '\x01';
LAB_80031a14:
        cur->y_max = menu;
        cur->x_max = cVar1;
        return;
      }
      goto LAB_800319a8;
    }
    if (menu == '(') goto LAB_80031a38;
    if (menu < ')') {
      if (menu != '!') {
        if (menu < '!') goto LAB_80031a0c;
        if (menu != '&') {
          if (menu != '\'') {
            return;
          }
          menu = '\x01';
          goto LAB_80031a44;
        }
      }
    }
    else {
      if (menu == '/') goto LAB_800318e8;
      if (menu < '0') {
        if ('-' < menu) {
          return;
        }
        menu = '\0';
        goto LAB_80031a44;
      }
      if (menu == '0') goto LAB_80031a0c;
      if (menu != '1') {
        return;
      }
    }
  }
LAB_80031a20:
  menu = '\0';
LAB_80031a24:
  cur->y_max = menu;
  cur->x_max = menu;
  return;
}

//NGC MATCH
void DrawNODATAAVAILABLE(void) {
  Text3D(tNODATAAVAILABLE[Game.language][0],0.0f,0.0f,1.0f,0.6f,0.6f,0.6f,1,0);
  return;
}













//WIP

void AddSpacesIntoText(char* txt, u32 bits);                        /* extern */
void DrawMenuEntry(struct cursor_s *cursor, char* txt, float* x, float* y, int* i);/* extern */
void DrawMenuEntry2(struct cursor_s *cursor, char* txt0, char* txt1, float* x, float* y, int* i); /* extern */
s8* GetStringIdx(s32, s32);                              /* extern */
s32 ParseNintendoErrorCode();                       /* extern */
void Text3D(char* txt, float x, float y, float z, float scalex, float scaley, float scalez, int align, int colour); /* extern */
char* MakeEditText(char* txt);
extern struct nupad_s* Pad[2];
extern struct game_s Game;
extern struct game_s* game;
extern s32 GameMode;
extern struct globaltimer_s GlobalTimer;
extern struct hdata_s HData[6];
extern char* HubName[6][6];
extern s32 LANGUAGEOPTION;
extern struct LDATA_s *LDATA;
extern char* LanguageName[6];
extern s32 Level;
extern char* LevelName[6][44];
static float MENUDY;
extern float PANELMENUX;
extern s32 TimeTrial;
static float dme_sx;
extern s32 highallocaddr;
extern s32 pause_dir;
extern union variptr_u superbuffer_base;
extern union variptr_u superbuffer_ptr;
extern char* tDONE[6]; 
extern char* tLANGUAGE[6]; 
extern char* tLOADGAME[6];
extern char* tCANTDOWNLOAD[6];
extern char* tMUSICVOLUME[6];
extern char* tCONTINUE[6];
extern char* tGAMEOVER[6];
extern char* tNEWGAME[6];
extern char* tRESTARTRACE[6];
extern char* tJCANCEL2;
extern char* tCANCEL[6];
extern char* tLOAD[6];
extern char* tSAVEGAME[6];
extern char* tSAVE[6];
extern char* tDELETEGAME[6];
extern char* tDELETE[6];
extern char* tEXIT[6];
extern char* tOFF[6];
extern char* tON[6];
extern char* tOPTIONS[6];
extern char* tPLAYCRASHBLAST[6];
extern char* tQUIT[6];
extern char* tRELICS[6];
extern char* tRESTARTTRIAL[6];
extern char* tRESUME[6];
extern char* tSFXVOLUME[6];
extern char* tSOUNDOPTIONS[6];
extern char* tGAMEINST[3][6];
extern char* tSUPERBELLYFLOPTEXT[6];
extern char* tTCR_ABANDONTHISGAME[6]; 
extern char* tTCR_ABANDONTHISLEVEL[6]; 
extern char* tVIBRATION[6]; 
extern char* tWARPROOM[6]; 
extern char* tYES[6]; 
extern char* tMEMCARDCONTINUE[6]; 
extern char* tMEMCARDRETRY[6]; 
extern char* tMEMCARDMANAGE[6]; 
extern char* tFORMAT[6]; 
extern char* tGEMS[6]; 
extern char* tNO[6];
extern char* tPRESSxTOCONTINUE[6];
extern char* tCONFIRMOVERWRITE[2][6];
extern char* tMEMCARDACCESS[2][6];
extern char* tCONFIRMDELETE[6];
extern char* tCONFIRMSAVE[6];
extern char* tCONFIRMFORMAT[6];
extern char* tFAILEDTOSAVE[6];
extern char* tFAILEDTODELETE[6];
extern char* tCONNECTGBA[3][6];
extern char* tCONNECTING[6];
extern char* tPLEASEWAIT[6];
extern char* tDOWNLOADING[6];
extern char* tLOADCRASHBLAST[6];
extern char* tPLAYCRASHBLAST[6];
extern char* tTCR_CONTENTPROTECTIONFAULT[6];
extern int new_power;
extern float dme_sy;
extern s32 InvincibilityCHEAT;
extern s32 LIFTPLAYER;
extern s32 ShowPlayerCoordinate;
extern s32 ExtraMoves;
float TT_INPUTY;
int new_level;
struct anim_s TempAnim;
int gameover_hack;
int fadeval;
int memcard_gamesavailable;
int memcard_saveneeded;
int memcard_savestarted;
int memcard_savemessage_delay;
int memcard_formatme;
int memcard_formatting;
int memcard_formatmessage_delay;
int saveload_error;
int saveload_cardchanged; 
int lost_controller;
float GAMENAMEY;
float POWERTEXTY;
extern struct numtl_s * GBABG_mtl;
int VersionDisplayFlag;
int SaveMenu;
int BackMenu;



/*
    s32 i; // 0x38(r1)
    s32 j; // r29
    s32 k; // r0
    s32 col; // r5
    float f; // f29
    float size; // f29
    float x; // 0x30(r1)
    float y; // 0x34(r1)
    float dy; // f31
    float xnew; // f31
    // Size: 0xC, DWARF: 0x2739C2
    struct pdata_s* PData; // r31
    unsigned char c; // 
*/

void DrawMenu(struct cursor_s *cursor,s32 paused) {
    float fVar1;
    float scalex;
    UNKTYPE* uVar2;
    s32 col;
    //uint uVar3;
    UNKTYPE* uVar4;
    char **txt;
    s32 iVar5;
    s32 unaff_r27;
    //char *(*papcVar6) [6];
    struct pdata_s *pdat;
    s32 iVar7;
    //double dVar8;
    double dVar9;
    double dVar10;
    //float local_68;
    float x;
    float y;
    s32 i;
    char *c;
    float dy;
    
    iVar7 = new_power;
    if (cursor->wait != '\0') {
        return;
    }
    if (cursor->menu == -1) {
        return;
    }
    if (GameMode == 1) {
        return;
    }
    x = 0.0;
    if (pause_dir != 0) {
        //local_68 = 0x1eU - paused;
        x = 0x1eU - paused * 0.06666667;
        if (pause_dir == 1) {
            x = 0.0 - x;
        }
        else {
            x = x + 0.0;
        }
    }
    col = (s32)cursor->y_min;
    //uVar3 = cursor->y_max - col;
    dVar9 = 0.5;
    iVar5 = (s32)cursor->menu;
    i = 0;
    dVar10 = 1.0;
    fVar1 = (cursor->y_max - col) * MENUDY;
    //dVar8 = (double)(fVar1 * 0.5);
    if (iVar5 != 0x17) {
        if (iVar5 < 0x18) {
            if (iVar5 == 0xb) {
                if (paused < 0x1e) {
                    return;
                }
                dme_sy = 0.8333333;
                y = 0.0 - fVar1 * 0.8333333 * 0.5;
                if (Game.language == 'c') {
                    c = "R E S T A R T   L E V E L ";
                }
                else {
                    c = "RESTART LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "R E S E T   L E V E L ";
                }
                else {
                    c = "RESET LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   L E V E L ";
                }
                else {
                    c = "GOTO LEVEL";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "I N V I N C I B I L I T Y :   ";
                }
                else {
                    c = "INVINCIBILITY: ";
                }
                if (InvincibilityCHEAT == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   N E X T   C H E C K P O I N T ";
                }
                else {
                    c = "GOTO NEXT CHECKPOINT";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "G O T O   L A S T   C H E C K P O I N T ";
                }
                else {
                    c = "GOTO LAST CHECKPOINT";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "O P E N   G A M E ";
                }
                else {
                    c = "OPEN GAME";
                }
                DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "L I F T   P L A Y E R :   ";
                }
                else {
                    c = "LIFT PLAYER: ";
                }
                if (LIFTPLAYER == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "P L A Y E R   C O O R D I N A T E :   ";
                }
                else {
                    c = "PLAYER COORDINATE: ";
                }
                if (ShowPlayerCoordinate == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "E X T R A   M O V E S :   ";
                }
                else {
                    c = "EXTRA MOVES: ";
                }
                if (ExtraMoves == 0) {
                    txt = tOFF;
                }
                else {
                    txt = tON;
                }
                sprintf(tbuf,"%s%s",c,txt[Game.language]);
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sy = 0.8333333;
                if (Game.language == 'c') {
                    c = "R E S E T   G A M E ";
                }
                else {
                    c = "RESET GAME";
                }
                goto LAB_80036f1c;
            }
            if (0xb < iVar5) {
                if (iVar5 == 0x11) {
                    DrawNameInputTable(cursor,0.0,TT_INPUTY);
                    return;
                }
                if (0x11 < iVar5) {
                    if (iVar5 == 0x14) {
                        if (TempAnim.newaction == 0x75) {
                            return;
                        }
                        if (TempAnim.newaction == 0x33) {
                            return;
                        }
                        if (new_level != -1) {
                            return;
                        }
                        if (gameover_hack != 1) {
                            return;
                        }
                        if ((TempAnim.newaction == 0x22) && (fadeval == 0)) {
                            y = (float)(0.5 - fVar1 * 0.5);
                            x = 0.4;
                            DrawMenuEntry(cursor,tCONTINUE[Game.language],&x,&y,(int*)i);
                            DrawMenuEntry(cursor,tQUIT[Game.language],&x,&y,(int*)i);
                        }
                        if (Game.language - 2 < 4) {
                            fVar1 = 0.0;
                            dy = -0.6;
                        }
                        else {
                            fVar1 = 0.4;
                            dy = -0.5;
                        }
                        scalex = 0.8;
                        c = tGAMEOVER[Game.language];
                        dVar10 = 1.0;
                        x = fVar1;
                        y = dy;
                        goto LAB_80037ad8;
                    }
                    if (iVar5 < 0x15) {
                        if (iVar5 != 0x12) {
                            return;
                        }
                        y = (float)(-0.699999988079071 - fVar1 * 0.5);
                        DrawMenuEntry(cursor,tRESTARTRACE[Game.language],&x,&y,(int*)i);
                        c = tWARPROOM[Game.language];
                        goto LAB_80036da0;
                    }
                    if (iVar5 != 0x15) {
                        if (iVar5 != 0x16) {
                            return;
                        }
                        iVar7 = ParseNintendoErrorCode();
                        if (iVar7 != 0) goto LAB_80036b44;
                        iVar7 = 1;
                        Text3D(tLOADGAME[Game.language],0.0,0.81,1.0,0.6,0.6,0.6,1,0);
                        if (memcard_gamesavailable == 0) {
                            DrawNODATAAVAILABLE();
                            cursor->y = cursor->y_max;
                        }
                        else {
                            DrawGameSlots(cursor);
                        }
                        if (cursor->y == cursor->y_max) {
                            iVar7 = 0x21;
                            col = 0;
                            if (5 < GlobalTimer.frame % 0xc) {
                                col = 3;
                            }
                        }
                        else {
                            col = 2;
                        }
                        c = tJCANCEL2;
                        if (Game.language != 99) {
                            c = tCANCEL[Game.language];
                        }
                        goto LAB_80036c8c;
                    }
                    if (Game.language == 'c') {
                        c = "%s/ %s";
                    }
                    else {
                        c = "%s/%s";
                    }
                    sprintf(tbuf,c,tLOAD[Game.language],tSAVE[Game.language]);
                    Text3D(tbuf,0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    XbUpdateDateStamp(&Game);
                    DrawGameSlot(&Game,0.0,0.25,4,0.6);
                    y = (float)(-0.5 - fVar1 * 0.5);
                    if (Game.language == 2) {
                        y = y - (float)(fVar1 * 0.5 * 0.2000000029802322);
                    }
                    DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + y);
                    }
                    DrawMenuEntry(cursor,tSAVEGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + (double)y);
                    }
                    DrawMenuEntry(cursor,tDELETEGAME[Game.language],&x,&y,(int*)i);
                    if (Game.language == 2) {
                        y = (float)(fVar1 * 0.5 * 0.119999997317791 + (double)y);
                    }
                    c = tEXIT[Game.language];
                    goto LAB_80037638;
                }
                if (iVar5 == 0xe) {
                    if (paused < 0x1e) {
                        return;
                    }
                    //local_68 = cursor->y_max - col + 2;
                    y = cursor->y_max - col + 2 * 0.1f * 0.5f - 0.2f;
                    i = col;
                    if (cursor->y_max < col) {
                        return;
                    }
                    do {
                        iVar7 = (s32)cursor->x_min;
                        dVar10 = 0.5;
                        //local_68 = cursor->x - iVar7;
                        fVar1 = x - cursor->x - iVar7 * 0.5;
                        if (iVar7 <= cursor->x_max) {
                            col = iVar7 * 0xc;
                            //papcVar6 = HubName[iVar7];
                            do {
                                //dVar8 = (double)fVar1;
                                if (i == 0) {
                                    if (unaff_r27 == -1) {
                                        c = "?";
                                    }
                                    else {
                                        c = HubName[iVar7];
                                    }
                                    strcpy(tbuf,c);
                                    Text3D(tbuf,(float)fVar1,y + 0.2f,1.0f,(float)(dVar9 * dVar10),
                                           (float)dVar9,(float)dVar9,1,4);
                                }
                                //unaff_r27 = (s32)(char)(&HData)[i + col];
                                if (unaff_r27 == -1) {
                                    c = "?";
                                }
                                else {
                                   // c = LevelName[unaff_r27 * 6];
                                }
                                strcpy(tbuf,c);
                                if ((i == cursor->y) && (iVar7 == cursor->x)) {
                                    iVar5 = 3;
                                    if (GlobalTimer.frame % 0xc < 6) {
                                        iVar5 = 4;
                                    }
                                }
                                else {
                                    iVar5 = 0;
                                }
                                iVar7 = iVar7 + 1;
                                col = col + 0xc;
                                Text3D(tbuf,(float)fVar1,y,1.0f,(float)(dVar9 * dVar10),(float)dVar9,
                                       (float)dVar9,1,iVar5);
                            //    papcVar6 = papcVar6[1];
                                fVar1 = (float)(fVar1 + dVar10);
                            } while (iVar7 <= cursor->x_max);
                        }
                        i = i + 1;
                        y = y - 0.1;
                    } while (i <= cursor->y_max);
                    return;
                }
                if (iVar5 < 0xf) {
                    if (iVar5 != 0xc) {
                        if (iVar5 != 0xd) {
                            return;
                        }
                        if (paused < 0x1e) {
                            return;
                        }
                        //dVar8 = -0.1000000014901161;
                        dVar9 = 9.5367431640625e-07; //35800000h
                        //dVar10 = 0.5;
                        y = 0.05;
                       // sprintf(tbuf,"SUPERBUFFER USED: %.2fMB/%.2fMB",
                        //        (((s32)superbuffer_ptr.voidptr -(s32)superbuffer_base * dVar9),7.0);
                        if (Game.language == 'c') {
                            AddSpacesIntoText(tbuf,1);
                        }
                        Text3D(tbuf,x,y,1.0,0.5,0.5,0.5,1,0);
                        y = (float)((double)y + -0.1000000014901161);
                        //local_68 = highallocaddr;
                        sprintf(tbuf,"HIGHALLOCADDR: %.2fMB/32.00MB",highallocaddr * dVar9);
                        if (Game.language == 'c') {
                            AddSpacesIntoText(tbuf,1);
                        }
                        Text3D(tbuf,x,y,1.0,0.5,0.5,0.5,1,0);
                        return;
                    }
                    if (paused < 0x1e) {
                        return;
                    }
                    y = (float)(0.0 - -0.1000000014901161);
                    sprintf(tbuf,"DRAW DISTANCE: %i/%i",(uint)LDATA->farclip,1000);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    //local_68 = (double)(long long)(s32)LDATA->fognear;
                    sprintf(tbuf,"FOG NEAR: %i/%i",(s32)LDATA->fognear,(uint)LDATA->farclip);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    //local_68 = (double)(long long)(s32)LDATA->fogfar;
                    sprintf(tbuf,"FOG FAR: %i/%i",(s32)LDATA->fogfar,(uint)LDATA->farclip);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG RED: %i/255",(uint)LDATA->fogr);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG GREEN: %i/255",(uint)LDATA->fogg);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG BLUE: %i/255",(uint)LDATA->fogb);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"FOG OPACITY: %i/127",(uint)LDATA->foga);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE RED: %i/255",(uint)LDATA->hazer);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE GREEN: %i/255",(uint)LDATA->hazeg);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE BLUE: %i/255",(uint)LDATA->hazeb);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                    sprintf(tbuf,"HAZE WOBBLE: %i/255",(uint)LDATA->hazea);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    c = tbuf;
                    goto LAB_80036f1c;
                }
                if (iVar5 == 0xf) {
                    y = (float)(0.0 - -0.1000000014901161);
                    if (Game.language == 'c') {
                        c = "L O G O S ";
                    }
                    else {
                        c = "LOGOS";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "I N T R O 1 ";
                    }
                    else {
                        c = "INTRO1";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "I N T R O 2 ";
                    }
                    else {
                        c = "INTRO2";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "O U T R O ";
                    }
                    else {
                        c = "OUTRO";
                    }
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    if (Game.language == 'c') {
                        c = "O U T R O 2 ";
                    }
                    else {
                        c = "OUTRO2";
                    }
                    goto LAB_80036f1c;
                }
                if (iVar5 != 0x10) {
                    return;
                }
                y = (float)(-0.699999988079071 - -0.1000000014901161);
                DrawMenuEntry(cursor,tRESTARTTRIAL[Game.language],&x,&y,(int*)i);
                c = tWARPROOM[Game.language];
                goto LAB_80036da0;
            }
         /*   if (iVar5 == 5) {
                if (paused < 0x1e) {
                    return;
                }
                y = (float)(-0.5 - -0.1000000014901161);
                x = PANELMENUX;
                DrawMenuEntry(cursor,tRESUME[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tOPTIONS[Game.language],&x,&y,(int*)i);
                if (Level == 0x25) {
                    txt = tQUIT;
                }
                else {
                    txt = tWARPROOM;
                }
                DrawMenuEntry(cursor,txt[Game.language],&x,&y,(int*)i);
                if (TimeTrial == 0) {
                    return;
                }
                c = tRESTARTTRIAL[Game.language];
                goto LAB_80036f1c;
            }*/
        /*    if (iVar5 < 6) {
                if (2 < iVar5) {
                    if (iVar5 == 3) {
                        return;
                    }
                    if (iVar5 != 4) {
                        return;
                    }
                    goto LAB_80034ee8;
                }
                if (iVar5 < 1) {
                    if (iVar5 != 0) {
                        return;
                    }
                    y = (float)(-0.6899999976158142 - -0.1000000014901161);
                    VersionDisplayFlag = 1;
                    SaveMenu = iVar5;
                    BackMenu = iVar5;
                    DrawMenuEntry(cursor,tNEWGAME[Game.language],&x,&y,(int*)i);
                    DrawMenuEntry(cursor,tLOADGAME[Game.language],&x,&y,(int*)i);
                    DrawMenuEntry(cursor,tPLAYCRASHBLAST[Game.language],&x,&y,(int*)i);
                    sprintf(tbuf,"%s: %s",tLANGUAGE[Game.language],LanguageName[Game.language]);
                    c = tbuf;
                    goto LAB_80036f1c;
                }
                y = 0.2;
                for (iVar7 = 0; iVar7 < 4; iVar7 = iVar7 + 1) {
                    uVar2 = ParseNintendoErrorCode();
                    c = (char *)GetStringIdx(uVar2,iVar7);
                    if (c == (char *)0x0) break;
                    Text3D(c,0.0,y,1.0,0.4,0.4,0.4,1,0);
                    y = y - 0.1;
                }
                y = (float)(-0.6899999976158142 - -0.1000000014901161);
                DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,(int*)i);
                iVar7 = ParseNintendoErrorCode();
                if (iVar7 == 6) {
                    uVar3 = (uint)Game.language;
                    txt = tMEMCARDMANAGE;
                    goto LAB_80037c10;
                }
                if (iVar7 < 7) {
                    if (3 < iVar7) {
                        return;
                    }
                    if (iVar7 < 2) {
                        return;
                    }
                    uVar3 = (uint)Game.language;
                    txt = tFORMAT;
                    goto LAB_80037c10;
                }
                goto LAB_80037be8;
            }*/
            if (iVar5 == 8) {
                return;
            }
            if (iVar5 < 9) {
                if (iVar5 != 6) {
                    if (iVar5 != 7) {
                        return;
                    }
                    if (paused < 0x1e) {
                        return;
                    }
                    //uVar3 = (uint)Game.sfx_volume;
                    y = (float)(-0.4600000083446503 - -0.1000000014901161) - (float)(-0.1000000014901161 * 0.5);
                    dme_sx = 1.0;
                    x = PANELMENUX;
                    //uVar2 = 0x20;
                    if (Game.sfx_volume != 0) {
                       // uVar2 = 0x3c;
                    }
                  //  uVar4 = 0x20;
                    if (Game.sfx_volume < 100) {
                     //   uVar4 = 0x3e;
                    }
                    sprintf(tbuf,"%c %i%% %c",uVar2,Game.sfx_volume,uVar4);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    dme_sx = (float)dVar10;
                    DrawMenuEntry2(cursor,tSFXVOLUME[Game.language],tbuf,&x,&y,(int*)i);
                    //uVar3 = (uint)Game.music_volume;
                   // uVar2 = 0x20;
                    if (Game.music_volume != 0) {
                    //    uVar2 = 0x3c;
                    }
                   // uVar4 = 0x20;
                    if (Game.music_volume < 100) {
                    //    uVar4 = 0x3e;
                    }
                    sprintf(tbuf,"%c %i%% %c",uVar2,Game.music_volume,uVar4);
                    if (Game.language == 'c') {
                        AddSpacesIntoText(tbuf,1);
                    }
                    dme_sx = (float)dVar10;
                    DrawMenuEntry2(cursor,tMUSICVOLUME[Game.language],tbuf,&x,&y,(int*)i);
                    dme_sx = (float)dVar10;
                    DrawMenuEntry(cursor,tDONE[Game.language],&x,&y,(int*)i);
                    return;
                }
                if (paused < 0x1e) {
                    return;
                }
                //uVar3 = (uint)Game.language;
                y = (float)(-0.5 - -0.1000000014901161);
                x = PANELMENUX;
                if (Game.language == 99) {
                    c = tRELICS[3];
                    if (Game.vibration != '\0') {
                        c = tGEMS[3];
                    }
                    sprintf(tbuf,"%s:   %s",tSUPERBELLYFLOPTEXT[2][3],c);
                }
                else {
                    if (Game.vibration == '\0') {
                        txt = tOFF;
                    }
                    else {
                        txt = tON;
                    }
                    sprintf(tbuf,"%s: %s",tVIBRATION[Game.language],txt[Game.language]);
                }
                dme_sx = (float)dVar10;
                DrawMenuEntry(cursor,tbuf,&x,&y,(int*)i);
                dme_sx = (float)dVar10;
                DrawMenuEntry(cursor,tSOUNDOPTIONS[Game.language],&x,&y,(int*)i);
                if (LANGUAGEOPTION != 0) {
                    dme_sx = (float)dVar10;
                    DrawMenuEntry(cursor,tLANGUAGE[Game.language],&x,&y,(int*)i);
                }
                dme_sx = (float)dVar10;
                c = tDONE[Game.language];
                goto LAB_80036f1c;
            }
            if (iVar5 == 9) {
                if (paused < 0x1e) {
                    return;
                }
             //   txt = LanguageName;
                x = PANELMENUX;
                y = (float)(-0.5 - -0.1000000014901161);
             /*   do {
                    c = *txt;
                    txt = txt + 1;
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                } while ((s32)txt < *LanguageName);    */
                return;
            }
            if (iVar5 != 10) {
                return;
            }
            if (paused < 0x1e) {
                return;
            }
            y = (float)(-0.5 - -0.1000000014901161);
            x = PANELMENUX;
            if (Level == 0x25) {
                txt = tTCR_ABANDONTHISGAME;
            }
            else {
                txt = tTCR_ABANDONTHISLEVEL;
            }
            Text3D(txt[Game.language],PANELMENUX,y + 0.25,1.0,0.5,0.5,0.5,1,0);
            c = tYES[Game.language];
        }
        else {
            if (iVar5 == 0x23) {
                DrawCredits();
                return;
            }
            if (iVar5 < 0x24) {
                if (iVar5 == 0x1d) {
                    iVar7 = ParseNintendoErrorCode();
                    if (iVar7 != 0) {
LAB_80036b44:
                        BackMenu = 0x15;
                        SaveMenu = iVar5;
                        NewMenu(cursor,0x2f,0,-1);
                        return;
                    }
                    iVar7 = 1;
                    Text3D(tDELETEGAME[Game.language],0.0,0.81,(float)dVar10,0.6,0.6,0.6,1,0);
                    if (memcard_gamesavailable == 0) {
                        DrawNODATAAVAILABLE();
                        cursor->y = cursor->y_max;
                    }
                    else {
                        DrawGameSlots(cursor);
                    }
                    if (cursor->y == cursor->y_max) {
                        col = 0;
                        if (5 < GlobalTimer.frame % 0xc) {
                            col = 3;
                        }
                        iVar7 = 0x21;
                    }
                    else {
                        col = 2;
                    }
                    if (memcard_saveneeded != 0) {
                        return;
                    }
                    if (memcard_savestarted != 0) {
                        return;
                    }
                    if (memcard_savemessage_delay != 0) {
                        return;
                    }
                    c = tCANCEL[Game.language];
LAB_80036c8c:
                    Text3D(c,0.0,-0.81,1.0,0.6,0.6,0.6,iVar7,col);
                    return;
                }
                if (iVar5 < 0x1e) {
                    if (iVar5 == 0x1a) {
LAB_80034ee8:
                        VersionDisplayFlag = 1;
                        DrawNameInputTable(cursor,0.0,-0.5);
                        c = MakeEditText(Game.name);
                        Text3D(c,0.0,GAMENAMEY,1.0,1.0,1.0,1.0,1,4);
                        return;
                    }
                    if (iVar5 < 0x1b) {
                        if (iVar5 != 0x18) {
                            if (iVar5 != 0x19) {
                                return;
                            }
                            iVar7 = ParseNintendoErrorCode();
                            if (iVar7 != 0) goto LAB_80036b44;
                            iVar7 = 1;
                            Text3D(tSAVEGAME[Game.language],0.0,0.81,1.0,0.6,0.6,0.6,1,0);
                            DrawGameSlots(cursor);
                            if (cursor->y == cursor->y_max) {
                                iVar7 = 0x21;
                                col = 0;
                                if (5 < GlobalTimer.frame % 0xc) {
                                    col = 3;
                                }
                            }
                            else {
                                col = 2;
                            }
                            if (memcard_formatting != 0) {
                                return;
                            }
                            if (memcard_formatme != 0) {
                                return;
                            }
                            if (memcard_formatmessage_delay != 0) {
                                return;
                            }
                            c = tJCANCEL2;
                            if (Game.language != 99) {
                                c = tCANCEL[Game.language];
                            }
                            goto LAB_80036c8c;
                        }
                        Text3D(tLOADGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                        Text3D((char*)tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar10,0.6,
                               0.6,0.6,1,0);
                        scalex = 0.6;
                        fVar1 = 0.0;
                        c = (char*) tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                        dy = -0.45;
                    }
                    else {
                        if (iVar5 == 0x1b) {
                            Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                            DrawGameSlot(&Game,0.0,0.25,4,0.6);
                            if (game->empty == '\0') {
                                Text3D((char*) tCONFIRMOVERWRITE[(uint)Game.language * 2],0.0,-0.3,
                                       (float)dVar10,0.6,0.6,0.6,1,0);
                                Text3D((char*) tCONFIRMOVERWRITE[(uint)Game.language * 2 + 1],0.0,-0.45,
                                       (float)dVar10,0.6,0.6,0.6,1,0);
                            }
                            else {
                                Text3D(tCONFIRMSAVE[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,
                                       0.6,1,0);
                            }
                            y = (float)(-0.699999988079071 - -0.1000000014901161);
                            DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
                            c = tNO[Game.language];
                            goto LAB_80036da0;
                        }
                        if (iVar5 != 0x1c) {
                            return;
                        }
                        Text3D(tSAVEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                        Text3D((char*) tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)dVar10,0.6,
                               0.6,0.6,1,0);
                        scalex = 0.6;
                        fVar1 = 0.0;
                        c = (char*) tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                        dy = -0.45;
                    }
                    goto LAB_80037ad8;
                }
                if (iVar5 == 0x20) {
                    Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tCONFIRMFORMAT[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,0.6,1,0);
                    y = (float)(-0.699999988079071 - -0.1000000014901161);
                    DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
                    txt = tNO;
LAB_80037624:
                    c = txt[Game.language];
LAB_80037638:
                    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
                    return;
                }
          /*      if (0x20 < iVar5) {
                    if (iVar5 != 0x21) {
                        if (iVar5 != 0x22) {
                            return;
                        }
                        pdat = struct pdata_s + new_power;
                        if ((new_power == 0) && (Game.language == '\x01')) {
                            fVar1 = 0.55;
                        }
                        else {
                            fVar1 = 0.6;
                        }
                        dVar10 = 0.6000000238418579;
                        Text3D(struct pdata_s[new_power].name,0.0,POWERTEXTY + 0.6,1.0,fVar1,0.6,0.6,1,1);
                        if (cursor->button_lock == '\0') {
                            y = (float)((double)(float)((double)POWERTEXTY - 0.6000000238418579) - -0.1000000014901161);
                            DrawMenuEntry(cursor,tPRESSxTOCONTINUE[Game.language],&x,&y,(int*)i);
                        }
                        uVar3 = struct pdata_s[iVar7].font3d_letter - 0x61 & 0xff;
                        fVar1 = Font3DObjTab[uVar3].scale * 2.5;
                        col = 0;
                        DrawPanel3DCharacter
                                  ((uint)pdat->character,0.0,-0.8,1.0,fVar1,fVar1,fVar1,0,0,0,
                                   (s32)Font3DObjTab[uVar3].action,Font3DObjTab[uVar3].anim_time,0 );
                        while ((col < 7 &&
                               (iVar5 = strcmp((char *)struct pdata_s[iVar7].description[col],""), iVar5 !=  0
                               ))) {
                            col = col + 1;
                        }
                        //local_68 = col - 1U;
                        dVar10 = 0.5;
                        dVar8 = (double)(MENUDY * 1.2);
                        i = 0;
                        y = POWERTEXTY -
                            (float)((double)(float)(col - 1U -
                                                   (double)(variptr_u  [2])0x4330000080000000) *
                                   MENUDY * 1.2) * 0.5;
                        if (col < 1) {
                            return;
                        }
                        do {
                            Text3D((char *)struct pdata_s[iVar7].description[i],0.0,y,1.0,(float)0.5,
                                   (float)0.5,0.6,1,0);
                            i = i + 1;
                            y = (float)((double)y + MENUDY * 1.2);
                        } while (i < col);
                        return;
                    }
                    Text3D(tFORMAT[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,(float)0.5,0.6,0.6,
                           0.6,1,0);
                    scalex = 0.6;
                    fVar1 = 0.0;
                    c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                    dy = -0.45;
                    goto LAB_80037ad8;
                }*/
        /*        if (iVar5 != 0x1e) {
                    if (iVar5 != 0x1f) {
                        return;
                    }
                    Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                    Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
                    scalex = 0.6;
                    fVar1 = 0.0;
                    c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                    dy = -0.45;
                    dVar10 = 1.0;
                    goto LAB_80037ad8;
                }*/
                Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                DrawGameSlot(game,0.0,0.25,4,0.6);
                txt = tCONFIRMDELETE;
                goto LAB_80036d1c;
            }
          if (iVar5 == 0x2b) {
                NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                return;
            }
            if (iVar5 < 0x2c) {
                if (iVar5 == 0x28) {
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3],0.0,-0.35,(float)dVar10,0.4,0.4,0.4,
                           1,0);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    Text3D((char*) tCONNECTGBA[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = -0.77;
LAB_800378c4:
                    Text3D(c,0.0,fVar1,(float)dVar10,0.4,0.4,0.4,1,0);
                    GBA_Draw();
                    return;
                }
                if (0x28 < iVar5) {
                  /*  if (iVar5 == 0x29) {
                        NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                        Text3D(tCONNECTING[Game.language],0.0,-0.35,(float)dVar10,0.4,0.4,0.4,1,0) ;
                        c = tPLEASEWAIT[Game.language];
                        fVar1 = -0.49;
                    }*/
                  /*  else {
                        if (iVar5 != 0x2a) {
                            return;
                        }
                        NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                        Text3D(tDOWNLOADING[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
                        c = tPLEASEWAIT[Game.language];
                        fVar1 = -0.49;
                        dVar10 = 1.0;
                    }*/
                    //goto LAB_800378c4;
                }
           /*     if (iVar5 != 0x26) {
                    if (iVar5 != 0x27) {
                        return;
                    }
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    y = -0.55;
                    DrawMenuEntry(cursor,tLOADCRASHBLAST[Game.language],&x,&y,(int*)i);
                    txt = tEXIT;
                    goto LAB_80037624;
                }*/
          /*      iVar7 = XbGetNumControllers();
                if (iVar7 == 0) {
                    Pad[0] = NULL;
                    return;
                }
                if (lost_controller != 0) {
                    return;
                }
        /*        if (((saveload_error & 0x20) == 0) && (saveload_cardchanged == 0)) {
                    if (saveload_error == 4) {
LAB_800371cc:
                        txt = tFAILEDTOSAVE;
                    }
                    else {
                        if ((s32)saveload_error < 5) {
LAB_80037218:
                            Text3D(tTCR_CONTENTPROTECTIONFAULT[Game.language],0.0,0.0,1.0,0.5,0.5,
                                   0.5,1,0);
                            goto LAB_80037260;
                        }
                        if (saveload_error == 8) {
                            txt = tFAILEDTODELETE;
                        }
                        else {
                            if (saveload_error != 0x10) goto LAB_80037218;
                            txt = tTCR_CONTENTPROTECTIONFAULT;
                        }
                    }
LAB_800371e8:
                    Text3D(txt[Game.language],0.0,0.0,1.0,0.5,0.5,0.5,1,0);
                }*/
      /*          else {
                    iVar7 = saveload_error - 0x20;
                    if (iVar7 == 4) goto LAB_800371cc;
                    if (iVar7 < 5) {
                        if (iVar7 == 2) {
LAB_80037150:
                            txt = tTCR_CONTENTPROTECTIONFAULT;
                            goto LAB_800371e8;
                        }
                    }
                    else {
                        if (iVar7 == 8) {
                            txt = tFAILEDTODELETE;
                            goto LAB_800371e8;
                        }
                        if (iVar7 == 0x10) goto LAB_80037150;
                    }
                }*/
/*LAB_80037260:
                scalex = 0.5;
                c = tPRESSxTOCONTINUE[Game.language];
                fVar1 = 0.0;
                dy = -0.15;
                dVar10 = 1.0;*/
LAB_80037ad8:
                 Text3D(c,fVar1,dy,(float)dVar10,scalex,scalex,scalex,1,0);
                return;
            }
       /*     if (iVar5 == 0x2f) {
                y = 0.2;
                for (iVar7 = 0; iVar7 < 4; iVar7 = iVar7 + 1) {
                    uVar2 = ParseNintendoErrorCode();
                    c = (char *)GetStringIdx(uVar2,iVar7);
                    if (c == (char *)0x0) break;
                    Text3D(c,0.0,y,1.0,0.4,0.4,0.4,1,0);
                    y = y - 0.1;
                }
                y = (float)(-0.6899999976158142 - MENUDY * 1.2);
                DrawMenuEntry(cursor,tMEMCARDRETRY[Game.language],&x,&y,(int*)i);
                DrawMenuEntry(cursor,tMEMCARDCONTINUE[Game.language],&x,&y,(int*)i);
                iVar7 = ParseNintendoErrorCode();
                if (iVar7 < 2) {
                    return;
                }
                if (iVar7 < 4) {
                    uVar3 = (uint)Game.language;
                    txt = tFORMAT;
                    goto LAB_80037c10;
                }
LAB_80037be8:
                if (iVar7 != 8) {
                    return;
                }
                uVar3 = (uint)Game.language;
                txt = tDELETE;
LAB_80037c10:
                DrawMenuEntry(cursor,txt[uVar3],&x,&y,(int*)i);
                cursor->y_max = '\x02';
                return;
            }*/
      /*      if (iVar5 < 0x30) {
                if (iVar5 == 0x2c) {
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D(tGAMEINST[(uint)Game.language * 3],0.0,-0.35,(float)dVar10,0.4,0.4,0.4, 1,
                           0);
                    Text3D(tGAMEINST[(uint)Game.language * 3 + 1],0.0,-0.49,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    Text3D(tGAMEINST[(uint)Game.language * 3 + 2],0.0,-0.63,(float)dVar10,0.4,0.4,
                           0.4,1,0);
                    scalex = 0.4;
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = 0.0;
                    dy = -0.77;
                }
                else {
                    if (iVar5 != 0x2d) {
                        return;
                    }
                    NuRndrRectUV2di(0,0,0x280,0x1e0,0.0,0.0,1.0,1.0,-1,GBABG_mtl);
                    Text3D(tCANTDOWNLOAD[Game.language],0.0,-0.35,1.0,0.4,0.4,0.4,1,0);
                    scalex = 0.4;
                    c = tPRESSxTOCONTINUE[Game.language];
                    fVar1 = 0.0;
                    dy = -0.77;
                    dVar10 = 1.0;
                }
                goto LAB_80037ad8;
            }*/
       /*     if (iVar5 != 0x30) {
                if (iVar5 != 0x31) {
                    return;
                }
                Text3D(tDELETEGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
                Text3D(tMEMCARDACCESS[(uint)Game.language * 2],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
                scalex = 0.6;
                fVar1 = 0.0;
                c = tMEMCARDACCESS[(uint)Game.language * 2 + 1];
                dy = -0.45;
                dVar10 = 1.0;
                goto LAB_80037ad8;
            }
            Text3D(tCONFIRMDELETE[Game.language],0.0,-0.3,1.0,0.6,0.6,0.6,1,0);
            c = tYES[Game.language];
            y = (float)(-0.699999988079071 - MENUDY * 1.2);*/
        }
    //    DrawMenuEntry(cursor,c,&x,&y,(int*)i);
      //  c = tNO[Game.language];
LAB_80036f1c:
     //   DrawMenuEntry(cursor,c,&x,&y,(int*)i);
        return;
    }
/*    Text3D(tLOADGAME[Game.language],0.0,0.75,1.0,0.6,0.6,0.6,1,0);
    DrawGameSlot(game,0.0,0.25,4,0.6);
    txt = tCONFIRMLOAD;    */
LAB_80036d1c:
 /*   Text3D(txt[Game.language],0.0,-0.3,(float)dVar10,0.6,0.6,0.6,1,0);
    y = (float)(-0.699999988079071 - MENUDY * 1.2);
    DrawMenuEntry(cursor,tYES[Game.language],&x,&y,(int*)i);
    c = tNO[Game.language];    */
LAB_80036da0:
    //DrawMenuEntry(cursor,c,&x,&y,(int*)i);
  //  return;
}

//NGC MATCH
void MakeTimeI(s32 time,s32 hours,char *txt) {
  s32 t;
  
  if (time < 0) {
    time = 0;
  }
  t = time / 3;
  if (hours != 0) {
    temp_hours = t / 360000;
    temp_minutes = t / 6000 - temp_hours * 0x3c;
  }
  else {
    temp_minutes = t / 6000;

  }
  temp_hundredths = t % 100;
  temp_tenths = temp_hundredths / 10;
  temp_seconds = (t / 100) % 0x3c;
  if ((txt != NULL) && (sprintf(txt,"%i:%c%c.%c%c",temp_minutes,temp_seconds / 10 + 0x30,temp_seconds % 10 + 0x30,
              temp_tenths + 0x30,temp_hundredths % 10 + 0x30), Game.language == 'c')) {
    AddSpacesIntoText(txt,1);
  }
  return;
}

//NGC MATCH
void MakeLevelTimeString(struct time_s *time,char *txt) {
  char *__format;
  char time_string [64];
  
  MakeTimeI(time->itime,0,time_string);
  if (Game.language == 'c') {
    __format = "%s  %s";
  }
  else {
    __format = "%s %s";
  }
  sprintf(txt,__format,time,time_string);
  return;
}

//NGC MATCH
void InitSplineTable(void) {
  s32 i;
  
  if (world_scene[0] != NULL) {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
      if (((SplTab[i].levbits >> Level) & 1) != 0) {
        SplTab[i].spl = NuSplineFind(world_scene[0],SplTab[i].name);
        if (SplTab[i].spl != NULL) {
          if ((SplTab[i].min > 0) && (SplTab[i].spl->len < SplTab[i].min)) {
            SplTab[i].spl = NULL;
          }
          else {
            if ((SplTab[i].max > 0) && ((SplTab[i].max >= SplTab[i].min  && (SplTab[i].spl->len > SplTab[i].max)))) {
              SplTab[i].spl = 0;
            }
          }
        }
      }
    }
  }
  else {
    for (i = 0; i < 0x49; i++) {
      SplTab[i].spl = NULL;
    }
  }
  return;
}

//NGC MATCH
float DistanceToLine(struct nuvec_s* pos, struct nuvec_s* p0, struct nuvec_s* p1) {
  s32 a;
  
  a = NuAtan2D(p1->x - p0->x,p1->z - p0->z);
  return NuFabs(((pos->x - p0->x) * (NuTrigTable[(((-a & 0xffff) + 0x4000) & 0x3fffc  / 4)]) +
                                 (pos->z - p0->z) * NuTrigTable[-a & 0xffff]));
}

//NGC MATCH
s32 LineCrossed(float xold,float zold,float xnew,float znew,float x0,float z0,float x1,float z1) {

s32 rv;

  if (!(0.0f <= (xnew - x0) * (z1 - z0) + (znew - z0) * (x0 - x1)) &&
     ((xold - x0) * (z1 - z0) + (zold - z0) * (x0 - x1) >= 0.0f)) {
      rv = 1;
    if (!((xnew - xold) * (z0 - zold) + (znew - zold) * (xold - x0) >= 0.0f && ((xnew - x1) * (zold - z1) + (znew - z1) * (x1 - xold) >= 0.0f))) {
    }else {
        rv = 2;
    }
      return rv;
  }
   else 
  return 0;
}

//NGC MATCH
float RatioBetweenEdges(struct nuvec_s *pos,struct nuvec_s *pL0,struct nuvec_s *pL1,struct nuvec_s *pR0,struct nuvec_s *pR1) {
  float dL;
  float dR;
  
  dL = DistanceToLine(pos,pL0,pL1);
  dR = DistanceToLine(pos,pR0,pR1);
  return (dL / (dL + dR));
}

//NGC MATCH
float RatioAlongLine(struct nuvec_s* pos, struct nuvec_s* p0, struct nuvec_s* p1) {
    float z;
    float z1;
    float dx;
    float dz;
    float siny;
    float cosy;
    u16 yrot;

    dx = p1->x - p0->x;
    dz = p1->z - p0->z;
    yrot = -NuAtan2D(dx, dz);
    siny = NuTrigTable[yrot];
    cosy = NuTrigTable[(yrot + 0x4000) & 0xffff];
    z = -((pos->x - p0->x) * siny) + ((pos->z - p0->z) * cosy);
    if (z <= 0.0f) {
        return 0.0f;
    }
    z1 = -(dx * siny) + (dz * cosy);
    if (z >= z1) {
        return 1.0f;
    } else {
        return z / z1;
    }
}

//NGC MATCH
u16 SplinePointAngle(struct nugspline_s *spl,int i) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  float dx;
  float dz;
    
  p0 = (struct nuvec_s *)(spl->pts + i * spl->ptsize);
  dx = dz = 0.0f;
  if (i > 0) {
    p1 = (struct nuvec_s *)(spl->pts + (i - 1) * spl->ptsize);
    dx = (p0->x - p1->x) + 0.0f;
    dz = (p0->z - p1->z) + 0.0f;
  }
  if (i < spl->len - 1) {
    p1 = (struct nuvec_s *)(spl->pts + (i + 1) * spl->ptsize);
    dx = dx + (p1->x - p0->x);
    dz = dz + (p1->z - p0->z);
  }
  return (u16)NuAtan2D(dx,dz);
}

//NGC MATCH
s32 NearestSplinePoint(struct nuvec_s *pos,struct nugspline_s *spl) {
  s32 index;
  s32 i;
  s32 d;
  s32 d0;
  
  index = -1;
  if ((spl != NULL)) {
    for (i = 0; i < spl->len; i++) {
      d = NuVecDistSqr(pos,(spl->pts + i * spl->ptsize),NULL);
      if ((index == -1) || (d < d0)) {
        index = i;
        d0 = d;
      }
    }
  }
  return index;
}

//NGC MATCH
void PointAlongSpline(struct nugspline_s *spl,float ratio,struct nuvec_s *dst,u16 *angle,u16 *tilt) {
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  u32 i;
  u32 j;
  u32 l;
  u16 a;
  float f;
  s32 d;
  
  if (angle != NULL) {
    *angle = 0;
  }
  if (tilt != NULL) {
    *tilt = 0;
  }
  if (spl != NULL) {
    if ((1.0f < ratio)) {
        ratio = 1.0f;
    } else if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    l = (s32)(ratio * (float)((u32)(spl->len + -1) * 0x10000));
    i = l >> 0x10;
    iTEMP = i;
    p0 = (struct nuvec_s *)(spl->pts + i * (int)spl->ptsize);
    *dst = *p0;
    if (angle != NULL) {
      a = SplinePointAngle(spl,i);
      *angle = a;
    }
    if (tilt != NULL) {
      a = SplinePointTilt(spl,i);
      *tilt = a;
    }
    if ((i < spl->len - 1U) && (l &= 0xffff, l != 0)) {
      j = i + 1;
      p1 = (struct nuvec_s *)(spl->pts + j * spl->ptsize);
      f = (float)l * 0.000015258789f;
      dst->x = (((p1->x) - p0->x) * f + dst->x);
      dst->y = (((p1->y) - p0->y) * f + dst->y);
      dst->z = (((p1->z) - p0->z) * f + dst->z);
      if (((angle == NULL) & 1) == 0) {
        a = SplinePointAngle(spl,j);
        d = RotDiff(*angle,a);
        *angle += (u16)(f * d);
      }
      if (tilt != NULL) {
        a = SplinePointTilt(spl,j);
        d = RotDiff(*tilt,a);
        *tilt += (u16)(f * d);
      }
    }
  }
  return;
}

//NGC MATCH
void DrawParallax(void) {
  static struct numtx_s m_513;
  struct nuvec_s s;
  
  s.x = s.y = s.z = pNuCam->farclip * 0.1f * 0.5f;
  NuMtxSetScale(&m_513,&s);
  m_513._30 = (pCam->m)._30;
  m_513._31 = (pCam->m)._31;
  m_513._32 = (pCam->m)._32;
  if (ObjTab[9].obj.special != NULL) {
    NuRndrGScnObj((ObjTab[9].obj.scene)->gobjs[(ObjTab[9].obj.special)->instance->objid],&m_513);
  }
  if (ObjTab[10].obj.special != NULL) {
    NuRndrGScnObj((ObjTab[10].obj.scene)->gobjs[(ObjTab[10].obj.special)->instance->objid],&m_513);
  }
  return;
}

//NGC MATCH
void GameRayCast(struct nuvec_s *src,struct nuvec_s *dir,float d,struct nuvec_s *dst) {
  struct nuvec_s end;
  struct nuvec_s v;
  float ratio;
  float length;
  
  ratio = 1.0f;
  end.x = dir->x * d + src->x;
  end.y = dir->y * d + src->y;
  end.z = dir->z * d + src->z;
  length = NuVecDist(&end,src,NULL);
  if ((RayIntersectSphere(src,&end,&GameCam->pos,ratio) != 0) && (temp_ratio < ratio)) {
    temp_ratio -= (ratio / length);
    if (0.0f <= temp_ratio) {
      plr_target_found = 0;
      ratio = temp_ratio;
    }
  }
  if ((CrateRayCast(src,&end) != 0) && (temp_ratio < ratio)) {
    plr_target_found = 1;
    ratio = temp_ratio;
  }
  if ((CreatureRayCast(src,&end) != 0) && (temp_ratio < ratio)) {
    plr_target_found = 1;
    ratio = temp_ratio;
  }
  if ((WumpaRayCast(src,&end,ratio) != 0) && (temp_ratio < ratio)) {
    temp_ratio = (temp_ratio - (0.1f / length));
    if (0.0f <= temp_ratio) {
      plr_target_found = 1;
      ratio = temp_ratio;
    }
  }
  if (1.0f > ratio) {
    dst->x = (end.x - src->x) * ratio + src->x;
    dst->y = (end.y - src->y) * ratio + src->y;
    dst->z = (end.z - src->z) * ratio + src->z;
  }
  else {
    *dst = end;
  }
  NuVecSub(&v,dst,src);
  if (NewRayCast(src,&v,0.0f) != 0) {
    NuVecAdd(plr_target_pos + 1,plr_target_pos,&v);
    plr_target_found = 0;
  }
  return;
}

//NGC MATCH
s32 InLoadSaveZone(struct creature_s *plr) {
  float x;
  float z;
  
  if ((((plr->obj).ground != '\0') && ((plr->obj).idle_gametime > 0.0f)) && (NuVecDistSqr(&(plr->obj).pos,&loadsavepos,NULL) < 9.0f)) {
    x = (plr->obj).pos.x;
    z = (plr->obj).pos.z;
    if (((0.0f <= ((x - vLSZ[0].x) * (vLSZ[1].z - vLSZ[0].z) + (z - vLSZ[0].z) * (vLSZ[0].x - vLSZ[1].x))) &&
        (0.0f <= ((x - vLSZ[1].x) * (vLSZ[2].z - vLSZ[1].z) + (z - vLSZ[1].z) * (vLSZ[1].x - vLSZ[2].x)))) &&
       ((0.0f <= ((x - vLSZ[2].x) * (vLSZ[3].z - vLSZ[2].z) + (z - vLSZ[2].z) * (vLSZ[2].x - vLSZ[3].x)) &&
        (0.0f <= ((x - vLSZ[3].x) * (vLSZ[0].z - vLSZ[3].z) + (z - vLSZ[3].z) * (vLSZ[3].x - vLSZ[0].x)))))) {
      return 1;
    }
  }
  return 0;
}
