#include"system/port.h"

//NGC MATCH
void NuShaderSetSkinningConstants(struct nugeomitem_s* item, struct nuprim_s* prim) {
  s32 i;
  s32 lookup;

  lookup = (s32)prim->skinmtxlookup;
  for(i = 0; i < 0x10; i++, lookup+= 2) {
    if (*(short *)lookup != -1) {
      NuMtxTranspose((struct numtx_s *)&CV_SKINMTX[i], &item->mtx[*(short *)lookup]);
    }
  }
  return;
}

void NuShaderSetBypassShaders(s32 flag) {
    bypassEffectShaders = flag;
    return;
}

void NuShaderSetGlassMix(float mix) {
    glassmix = mix;
    return;
}
