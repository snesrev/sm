// Samus animations

#include "ida_types.h"
#include "variables.h"
#include "funcs.h"

#define kSamus_AnimationDefinitionPtrs ((uint16*)RomPtr(0x92d94e))
#define kSamus_TileDefs_TopHalf ((uint16*)RomPtr(0x92d91e))
#define kSamus_TileDefs_BottomHalf ((uint16*)RomPtr(0x92d938))

void SetSamusTilesDefsForCurAnim(void) {  // 0x928000
  int16 v2;

  R18_ = 4 * samus_anim_frame;
  R20_ = 4 * samus_anim_frame + 2;
  uint16 v0 = 4 * samus_anim_frame + kSamus_AnimationDefinitionPtrs[samus_pose];
  R22_ = get_SamusTileAnimationDefs(v0)->top_half_idx;
  tempB24 = get_SamusTileAnimationDefs(v0 + 1)->top_half_idx;
  R18_ = 7 * tempB24;
  nmi_copy_samus_top_half_src = 7 * tempB24 + kSamus_TileDefs_TopHalf[R22_];
  LOBYTE(nmi_copy_samus_halves) = 1;
  uint16 v1 = R20_ + kSamus_AnimationDefinitionPtrs[samus_pose];
  v2 = *RomPtr_92(v1);
  if (v2 != 255) {
    tempB26 = *RomPtr_92(v1 + 1);
    R20_ = 7 * tempB26;
    nmi_copy_samus_bottom_half_src = 7 * tempB26 + kSamus_TileDefs_BottomHalf[v2];
    HIBYTE(nmi_copy_samus_halves) = 1;
  }
}

uint8 PlaySamusFanfare(void) {  // 0x92ED24
  if (substate) {
    if (substate == 5)
      PlayRoomMusicTrackAfterAFrames(0x168u);
  } else {
    QueueMusic_DelayedY(1u, 0xE);
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
  if (samus_pose_x_dir == 4)
    R18_ = g_word_7E0DE4 + 2085;
  else
    R18_ = g_word_7E0DE4 + 2076;
  DrawSamusSpritemap(R18_, samus_x_pos, samus_y_pos);
}
