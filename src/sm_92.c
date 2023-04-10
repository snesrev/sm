// Samus animations

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

#define kSamus_AnimationDefinitionPtrs ((uint16*)RomFixedPtr(0x92d94e))
#define kSamus_TileDefs_TopHalf ((uint16*)RomFixedPtr(0x92d91e))
#define kSamus_TileDefs_BottomHalf ((uint16*)RomFixedPtr(0x92d938))

void SetSamusTilesDefsForCurAnim(void) {  // 0x928000
  uint16 v0 = 4 * samus_anim_frame + kSamus_AnimationDefinitionPtrs[samus_pose];
  SamusTileAnimationDefs *AD = get_SamusTileAnimationDefs(v0);
  nmi_copy_samus_top_half_src = 7 * AD->top_half_pos + kSamus_TileDefs_TopHalf[AD->top_half_idx];
  LOBYTE(nmi_copy_samus_halves) = 1;
  if (AD->bottom_half_idx != 255) {
    nmi_copy_samus_bottom_half_src = 7 * AD->bottom_half_pos + kSamus_TileDefs_BottomHalf[AD->bottom_half_idx];
    HIBYTE(nmi_copy_samus_halves) = 1;
  }
}

uint8 PlaySamusFanfare(void) {  // 0x92ED24
  if (substate) {
    if (substate == 5)
      PlayRoomMusicTrackAfterAFrames(0x168);
  } else {
    QueueMusic_DelayedY(1, 0xE);
  }
  if (sign16(substate - 359)) {
    ++substate;
    return 0;
  } else {
    samus_last_different_pose = samus_prev_pose;
    *(uint16 *)&samus_last_different_pose_x_dir = *(uint16 *)&samus_prev_pose_x_dir;
    samus_prev_pose = samus_pose;
    *(uint16 *)&samus_prev_pose_x_dir = *(uint16 *)&samus_pose_x_dir;
    substate = 0;
    return 1;
  }
}

void Unused_SamusTileViewer(void) {  // 0x92ED7A
  DrawSamusSpritemap(0x182, 0x40, 0x40);
  DrawSamusSpritemap(0x183, 0xC0, 0x40);
  DrawSamusSpritemap(0x184, 0x80, 0x60);
  DrawSamusSpritemap(0x185, 0x80, 0x50);
}

void DrawSamusSuitExploding(void) {  // 0x92EDBE
  uint16 r18 = (samus_pose_x_dir == 4) ? g_word_7E0DE4 + 2085 : g_word_7E0DE4 + 2076;
  DrawSamusSpritemap(r18, samus_x_pos, samus_y_pos);
}
