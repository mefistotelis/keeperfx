#ifndef DK_SPRITES_H
#define DK_SPRITES_H

// Based on gui1-64/filelist_gui1.txt
enum GUIButtonSprite {
	GBS_frontend_button_std_l = 1,
	GBS_frontend_button_std_c = 2,
	GBS_frontend_button_std_r = 3,
	GBS_borders_bar_std_l = 4,
	GBS_borders_bar_std_c = 5,
	GBS_borders_bar_std_r = 6,
	GBS_frontend_button_sta_l = 7,
	GBS_frontend_button_sta_c = 8,
	GBS_frontend_button_sta_r = 9,
	GBS_parchment_map_frame_deco_a_tl = 10,
	GBS_parchment_map_frame_deco_a_bl = 11,
	GBS_borders_frame_thin_tl = 12,
	GBS_borders_frame_thin_tc = 13,
	GBS_borders_frame_thin_tr = 14,
	GBS_borders_frame_thin_ml = 15,
	GBS_borders_frame_thin_mr = 16,
	GBS_borders_frame_thin_bl = 17,
	GBS_borders_frame_thin_bc = 18,
	GBS_borders_frame_thin_br = 19,
	GBS_guisymbols_jewel_off = 20,
	GBS_guisymbols_jewel_on = 21,
	GBS_options_button_save = 22,
	GBS_options_button_load = 23,
	GBS_options_button_sound = 24,
	GBS_options_button_graphc = 25,
	GBS_options_button_exit = 26,
	GBS_options_button_grph_shadow0 = 27,
	GBS_options_button_grph_shadow1 = 28,
	GBS_options_button_grph_shadow2 = 29,
	GBS_options_button_grph_shadow3 = 30,
	GBS_options_button_grph_shadow4 = 31,

	GBS_options_button_grph_textr_lq = 34,
	GBS_options_button_grph_textr_hq = 35,
	GBS_options_button_grph_range0 = 36,
	GBS_options_button_grph_range1 = 37,
	GBS_options_button_grph_range2 = 38,
	GBS_options_button_grph_range3 = 39,
	GBS_options_button_snd_sounds = 40,
	GBS_options_button_snd_music = 41,
	GBS_options_button_grph_wall_hi = 42,
	GBS_options_button_grph_wall_lo = 43,
	GBS_options_button_grph_gamma = 44,
	GBS_options_button_smd_generic = 45,
	GBS_options_button_smd_no = 46,
	GBS_options_button_sma_no = 47,
	GBS_options_button_smd_yes = 48,
	GBS_options_button_sma_yes = 49,
	GBS_creature_states_defend = 50,
	GBS_creature_states_attack = 51,
	GBS_creature_states_getgold = 52,
	GBS_creature_states_retreat = 53,
	GBS_creature_states_sleep = 54,
	GBS_creature_states_angry = 55,
	GBS_creature_states_resrch = 56,
	GBS_creature_states_train = 57,
	GBS_creature_states_lair = 58,
	GBS_creature_states_hungry = 59,
	GBS_creature_states_unk059 = 60,
	GBS_creature_states_livid = 61,
	GBS_creature_states_cta = 62,
	GBS_creature_states_fight = 63,
	GBS_creatr_state_scavngd = 64,
	GBS_creatr_state_tortr = 65,
	GBS_creature_states_prison = 66,
	GBS_creature_states_manufct = 67,
	GBS_creature_states_psycho = 68,
	GBS_creature_states_scavng = 69,
	GBS_creature_states_cloud = 70,
	GBS_fontchars_number_dig0 = 71,
	GBS_fontchars_number_dig1 = 72,
	GBS_fontchars_number_dig2 = 73,
	GBS_fontchars_number_dig3 = 74,
	GBS_fontchars_number_dig4 = 75,
	GBS_fontchars_number_dig5 = 76,
	GBS_fontchars_number_dig6 = 77,
	GBS_fontchars_number_dig7 = 78,
	GBS_fontchars_number_dig8 = 79,
	GBS_fontchars_number_dig9 = 80,
	GBS_creature_flower_health_r8 = 81,
	GBS_creature_flower_health_r7 = 82,
	GBS_creature_flower_health_r6 = 83,
	GBS_creature_flower_health_r5 = 84,
	GBS_creature_flower_health_r4 = 85,
	GBS_creature_flower_health_r3 = 86,
	GBS_creature_flower_health_r2 = 87,
	GBS_creature_flower_health_r1 = 88,
	GBS_creature_flower_health_b8 = 89,
	GBS_creature_flower_health_b7 = 90,
	GBS_creature_flower_health_b6 = 91,
	GBS_creature_flower_health_b5 = 92,
	GBS_creature_flower_health_b4 = 93,
	GBS_creature_flower_health_b3 = 94,
	GBS_creature_flower_health_b2 = 95,
	GBS_creature_flower_health_b1 = 96,
	GBS_creature_flower_health_g8 = 97,
	GBS_creature_flower_health_g7 = 98,
	GBS_creature_flower_health_g6 = 99,
	GBS_creature_flower_health_g5 = 100,
	GBS_creature_flower_health_g4 = 101,
	GBS_creature_flower_health_g3 = 102,
	GBS_creature_flower_health_g2 = 103,
	GBS_creature_flower_health_g1 = 104,
	GBS_creature_flower_health_y8 = 105,
	GBS_creature_flower_health_y7 = 106,
	GBS_creature_flower_health_y6 = 107,
	GBS_creature_flower_health_y5 = 108,
	GBS_creature_flower_health_y4 = 109,
	GBS_creature_flower_health_y3 = 110,
	GBS_creature_flower_health_y2 = 111,
	GBS_creature_flower_health_y1 = 112,
	GBS_creature_flower_health_w8 = 113,
	GBS_creature_flower_health_w7 = 114,
	GBS_creature_flower_health_w6 = 115,
	GBS_creature_flower_health_w5 = 116,
	GBS_creature_flower_health_w4 = 117,
	GBS_creature_flower_health_w3 = 118,
	GBS_creature_flower_health_w2 = 119,
	GBS_creature_flower_health_w1 = 120,

	GBS_optionsbutton_resolution = 122,
	GBS_optionsbutton_snd_voice = 123,
	GBS_options_button_grph_pers_rot = 124,
	GBS_options_button_grph_pers_neat = 125,
	GBS_options_button_grph_pers_straight = 126,

	GBS_creature_portrait_horny = 145,
	GBS_creature_portrait_skelt = 146,
	GBS_creature_portrait_troll = 147,
	GBS_creature_portrait_dragn = 148,
	GBS_creature_portrait_spawn = 149,
	GBS_creature_portrait_fly = 150,
	GBS_creature_portrait_mistr = 151,
	GBS_creature_portrait_warlk = 152,
	GBS_creature_portrait_biled = 153,
	GBS_creature_portrait_imp = 154,
	GBS_creature_portrait_bug = 155,
	GBS_creature_portrait_vampr = 156,
	GBS_creature_portrait_spidr = 157,
	GBS_creature_portrait_tentc = 158,
	GBS_creature_portrait_hound = 159,
	GBS_creature_portrait_ghost = 160,
	GBS_creature_portrait_fairy = 161,
	GBS_creature_portrait_knigh = 162,
	GBS_creature_portrait_thief = 163,
	GBS_creature_portrait_wizrd = 164,
	GBS_creature_portrait_barbr = 165,
	GBS_creature_portrait_dwrf1 = 166,
	GBS_creature_portrait_dwrf2 = 167,
	GBS_creature_portrait_archr = 168,
	GBS_creature_portrait_giant = 169,
	GBS_creature_portrait_witch = 170,
	GBS_creature_portrait_samur = 171,
	GBS_creature_portrait_monk = 172,
	GBS_creature_portrait_orc = 173,
	GBS_guisymbols_sym_crown = 174,
	GBS_guisymbols_sym_fight = 175,
	GBS_guisymbols_new_function_1 = 176,
	GBS_guisymbols_new_function_2 = 177,
	GBS_guisymbols_new_function_3 = 178,
	GBS_guisymbols_new_function_4 = 179,
	GBS_guisymbols_new_function_5 = 180,
	GBS_guisymbols_new_function_6 = 181,
	GBS_guisymbols_new_function_7 = 182,
	GBS_guisymbols_new_function_8 = 183,
	GBS_creature_flower_level_01 = 184,
	GBS_creature_flower_level_02 = 185,
	GBS_creature_flower_level_03 = 186,
	GBS_creature_flower_level_04 = 187,
	GBS_creature_flower_level_05 = 188,
	GBS_creature_flower_level_06 = 189,
	GBS_creature_flower_level_07 = 190,
	GBS_creature_flower_level_08 = 191,
	GBS_creature_flower_level_09 = 192,
	GBS_creature_flower_level_10 = 193,
	GBS_parchment_map_frame_deco_b_tl = 194,
	GBS_parchment_map_frame_deco_b_tr = 195,
	GBS_parchment_map_frame_deco_b_bl = 196,
	GBS_parchment_map_frame_deco_b_br = 197,
	GBS_parchment_map_screen_flame_1 = 198,
	GBS_parchment_map_screen_flame_2 = 199,
	GBS_parchment_map_screen_flame_3 = 200,
	GBS_parchment_map_screen_flame_4 = 201,
	GBS_parchment_map_screen_flame_5 = 202,
	GBS_parchment_map_screen_flame_6 = 203,
	GBS_parchment_map_screen_flame_7 = 204,
	GBS_parchment_map_screen_flame_8 = 205,
	GBS_borders_frame_thck_tl = 206,
	GBS_borders_frame_thck_tr = 207,
	GBS_borders_frame_thck_bl = 208,
	GBS_borders_frame_thck_br = 209,
	GBS_borders_frame_thck_tc = 210,
	GBS_borders_frame_thck_bc = 211,
	GBS_borders_frame_thck_ml = 212,
	GBS_borders_frame_thck_mr = 213,
	GBS_creature_portrait_avatr = 214,

	GUI_BUTTON_SPRITES_COUNT = 215,
};

// Based on gui2-64/filelist_gui2.txt
enum GUIPanelSprite {
	GPS_rpanel_rpanel_full_tl = 1,
	GPS_rpanel_rpanel_full_tr = 2,
	GPS_rpanel_rpanel_full_bl = 3,
	GPS_rpanel_rpanel_full_br = 4,
	GPS_rpanel_rpanel_full_a_l = 5,
	GPS_rpanel_rpanel_full_a_r = 6,
	GPS_rpanel_rpanel_tab_infoa = 7,
	GPS_rpanel_rpanel_tab_infod = 8,
	GPS_rpanel_rpanel_tab_rooma = 9,
	GPS_rpanel_rpanel_tab_roomd = 10,
	GPS_rpanel_rpanel_tab_spela = 11,
	GPS_rpanel_rpanel_tab_speld = 12,
	GPS_rpanel_rpanel_tab_wrksha = 13,
	GPS_rpanel_rpanel_tab_wrkshd = 14,
	GPS_rpanel_rpanel_tab_crtra = 15,
	GPS_rpanel_rpanel_tab_crtrd = 16,
	GPS_rpanel_rpanel_mapdrir_n = 17,
	GPS_rpanel_rpanel_mapdrir_s = 18,
	GPS_rpanel_rpanel_mapdrir_e = 19,
	GPS_rpanel_rpanel_mapdrir_w = 20,
	GPS_rpanel_rpanel_full_b_l = 21,
	GPS_rpanel_rpanel_full_b_r = 22,
	GPS_rpanel_frame_wide_wbar = 23,
	GPS_rpanel_frame_portrt_empty = 24,
	GPS_rpanel_frame_portrt_qmark = 25,
	GPS_rpanel_frame_wide_empty = 26,
	GPS_rpanel_frame_portrt_light = 27,
	GPS_rpanel_frame_portrt_active = 28,
	GPS_room_treasury_std_l = 29,
	GPS_room_treasury_dis_l = 30,
	GPS_room_hatchery_std_l = 31,
	GPS_room_hatchery_dis_l = 32,
	GPS_room_research_std_l = 33,
	GPS_room_research_dis_l = 34,
	GPS_room_torture_std_l = 35,
	GPS_room_torture_dis_l = 36,
	GPS_room_prison_std_l = 37,
	GPS_room_prison_dis_l = 38,
	GPS_room_training_std_l = 39,
	GPS_room_training_dis_l = 40,
	GPS_room_armory_std_l = 41,
	GPS_room_armory_dis_l = 42,
	GPS_room_graveyard_std_l = 43,
	GPS_room_graveyard_dis_l = 44,
	GPS_room_temple_std_l = 45,
	GPS_room_temple_dis_l = 46,
	GPS_room_workshop_std_l = 47,
	GPS_room_workshop_dis_l = 48,
	GPS_room_scavenge_std_l = 49,
	GPS_room_scavenge_dis_l = 50,
	GPS_room_lair_std_l = 51,
	GPS_room_lair_dis_l = 52,
	GPS_room_bridge_std_l = 53,
	GPS_room_bridge_dis_l = 54,
	GPS_room_grdpost_std_l = 55,
	GPS_room_grdpost_dis_l = 56,
	GPS_room_treasury_std_s = 57,
	GPS_room_treasury_dis_s = 58,
	GPS_room_hatchery_std_s = 59,
	GPS_room_hatchery_dis_s = 60,
	GPS_room_research_std_s = 61,
	GPS_room_research_dis_s = 62,
	GPS_room_torture_std_s = 63,
	GPS_room_torture_dis_s = 64,
	GPS_room_prison_std_s = 65,
	GPS_room_prison_dis_s = 66,
	GPS_room_training_std_s = 67,
	GPS_room_training_dis_s = 68,
	GPS_room_armory_std_s = 69,
	GPS_room_armory_dis_s = 70,
	GPS_room_graveyard_std_s = 71,
	GPS_room_graveyard_dis_s = 72,
	GPS_room_temple_std_s = 73,
	GPS_room_temple_dis_s = 74,
	GPS_room_workshop_std_s = 75,
	GPS_room_workshop_dis_s = 76,
	GPS_room_scavenge_std_s = 77,
	GPS_room_scavenge_dis_s = 78,
	GPS_room_lair_std_s = 79,
	GPS_room_lair_dis_s = 80,
	GPS_room_bridge_std_s = 81,
	GPS_room_bridge_dis_s = 82,
	GPS_room_grdpost_std_s = 83,
	GPS_room_grdpost_dis_s = 84,
	GPS_keepower_sight_std_l = 85,
	GPS_keepower_sight_dis_l = 86,
	GPS_keepower_heal_std_l = 87,
	GPS_keepower_heal_dis_l = 88,
	GPS_keepower_holdaud_std_l = 89,
	GPS_keepower_holdaud_dis_l = 90,
	GPS_keepower_possess_std_l = 91,
	GPS_keepower_possess_dis_l = 92,
	GPS_keepower_cta_std_l = 93,
	GPS_keepower_cta_dis_l = 94,
	GPS_keepower_imp_std_l = 95,
	GPS_keepower_imp_dis_l = 96,
	GPS_keepower_cavein_std_l = 97,
	GPS_keepower_cavein_dis_l = 98,
	GPS_keepower_speed_std_l = 99,
	GPS_keepower_speed_dis_l = 100,
	GPS_keepower_lightng_std_l = 101,
	GPS_keepower_lightng_dis_l = 102,
	GPS_keepower_armor_std_l = 103,
	GPS_keepower_armor_dis_l = 104,
	GPS_keepower_conceal_std_l = 105,
	GPS_keepower_conceal_dis_l = 106,
	GPS_rpanel_frame_portrt_sell = 107,
	GPS_keepower_sight_std_s = 108,
	GPS_keepower_sight_dis_s = 109,
	GPS_keepower_heal_std_s = 110,
	GPS_keepower_heal_dis_s = 111,
	GPS_keepower_holdaud_std_s = 112,
	GPS_keepower_holdaud_dis_s = 113,
	GPS_keepower_possess_std_s = 114,
	GPS_keepower_possess_dis_s = 115,
	GPS_keepower_cta_std_s = 116,
	GPS_keepower_cta_dis_s = 117,
	GPS_keepower_imp_std_s = 118,
	GPS_keepower_imp_dis_s = 119,
	GPS_keepower_cavein_std_s = 120,
	GPS_keepower_cavein_dis_s = 121,
	GPS_keepower_speed_std_s = 122,
	GPS_keepower_speed_dis_s = 123,
	GPS_keepower_lightng_std_s = 124,
	GPS_keepower_lightng_dis_s = 125,
	GPS_keepower_armor_std_s = 126,
	GPS_keepower_armor_dis_s = 127,
	GPS_keepower_conceal_std_s = 128,
	GPS_keepower_conceal_dis_s = 129,
	GPS_trapdoor_trap_boulder_std_l = 130,
	GPS_trapdoor_trap_boulder_dis_l = 131,
	GPS_trapdoor_trap_alarm_std_l = 132,
	GPS_trapdoor_trap_alarm_dis_l = 133,
	GPS_trapdoor_trap_gas_std_l = 134,
	GPS_trapdoor_trap_gas_dis_l = 135,
	GPS_trapdoor_trap_lightning_std_l = 136,
	GPS_trapdoor_trap_lightning_dis_l = 137,
	GPS_trapdoor_trap_wop_std_l = 138,
	GPS_trapdoor_trap_wop_dis_l = 139,
	GPS_trapdoor_trap_lava_std_l = 140,
	GPS_trapdoor_trap_lava_dis_l = 141,
	GPS_trapdoor_bonus_box_std_l = 142,
	GPS_trapdoor_bonus_box_dis_l = 143,
	GPS_trapdoor_door_pers_wood_std_l = 144,
	GPS_trapdoor_door_pers_wood_dis_l = 145,
	GPS_trapdoor_door_pers_braced_std_l = 146,
	GPS_trapdoor_door_pers_braced_dis_l = 147,
	GPS_trapdoor_door_pers_iron_std_l = 148,
	GPS_trapdoor_door_pers_iron_dis_l = 149,
	GPS_trapdoor_door_pers_magic_std_l = 150,
	GPS_trapdoor_door_pers_magic_dis_l = 151,
	GPS_trapdoor_trap_boulder_std_s = 152,
	GPS_trapdoor_trap_boulder_dis_s = 153,
	GPS_trapdoor_trap_alarm_std_s = 154,
	GPS_trapdoor_trap_alarm_dis_s = 155,
	GPS_trapdoor_trap_gas_std_s = 156,
	GPS_trapdoor_trap_gas_dis_s = 157,
	GPS_trapdoor_trap_lightning_std_s = 158,
	GPS_trapdoor_trap_lightning_dis_s = 159,
	GPS_trapdoor_trap_wop_std_s = 160,
	GPS_trapdoor_trap_wop_dis_s = 161,
	GPS_trapdoor_trap_lava_std_s = 162,
	GPS_trapdoor_trap_lava_dis_s = 163,
	GPS_trapdoor_bonus_box_std_s = 164,
	GPS_trapdoor_bonus_box_dis_s = 165,
	GPS_trapdoor_door_wood_std_s = 166,
	GPS_trapdoor_door_wood_dis_s = 167,
	GPS_trapdoor_door_braced_std_s = 168,
	GPS_trapdoor_door_braced_dis_s = 169,
	GPS_trapdoor_door_iron_std_s = 170,
	GPS_trapdoor_door_iron_dis_s = 171,
	GPS_trapdoor_door_magic_std_s = 172,
	GPS_trapdoor_door_magic_dis_s = 173,
	GPS_rpanel_frame_wide_up = 174,
	GPS_rpanel_frame_wide_down = 175,
	GPS_rpanel_frame_hex_narrow_a = 176,
	GPS_rpanel_frame_hex_narrow_b = 177,
	GPS_creatr_icon_wizrd_std = 178,
	GPS_creatr_icon_wizrd_dis = 179,
	GPS_creatr_icon_barbr_std = 180,
	GPS_creatr_icon_barbr_dis = 181,
	GPS_creatr_icon_archr_std = 182,
	GPS_creatr_icon_archr_dis = 183,
	GPS_creatr_icon_monk_std = 184,
	GPS_creatr_icon_monk_dis = 185,
	GPS_creatr_icon_dwarf_std = 186,
	GPS_creatr_icon_dwarf_dis = 187,
	GPS_creatr_icon_knght_std = 188,
	GPS_creatr_icon_knght_dis = 189,
	GPS_creatr_icon_avatr_std = 190,
	GPS_creatr_icon_avatr_dis = 191,
	GPS_creatr_icon_tunlr_std = 192,
	GPS_creatr_icon_tunlr_dis = 193,
	GPS_creatr_icon_prsts_std = 194,
	GPS_creatr_icon_prsts_dis = 195,
	GPS_creatr_icon_giant_std = 196,
	GPS_creatr_icon_giant_dis = 197,
	GPS_creatr_icon_fairy_std = 198,
	GPS_creatr_icon_fairy_dis = 199,
	GPS_creatr_icon_thief_std = 200,
	GPS_creatr_icon_thief_dis = 201,
	GPS_creatr_icon_samur_std = 202,
	GPS_creatr_icon_samur_dis = 203,
	GPS_creatr_icon_hornd_std = 204,
	GPS_creatr_icon_hornd_dis = 205,
	GPS_creatr_icon_skelt_std = 206,
	GPS_creatr_icon_skelt_dis = 207,
	GPS_creatr_icon_troll_std = 208,
	GPS_creatr_icon_troll_dis = 209,
	GPS_creatr_icon_dragn_std = 210,
	GPS_creatr_icon_dragn_dis = 211,
	GPS_creatr_icon_dspwn_std = 212,
	GPS_creatr_icon_dspwn_dis = 213,
	GPS_creatr_icon_fly_std = 214,
	GPS_creatr_icon_fly_dis = 215,
	GPS_creatr_icon_dkmis_std = 216,
	GPS_creatr_icon_dkmis_dis = 217,
	GPS_creatr_icon_warlk_std = 218,
	GPS_creatr_icon_warlk_dis = 219,
	GPS_creatr_icon_biled_std = 220,
	GPS_creatr_icon_biled_dis = 221,
	GPS_creatr_icon_imp_std = 222,
	GPS_creatr_icon_imp_dis = 223,
	GPS_creatr_icon_bug_std = 224,
	GPS_creatr_icon_bug_dis = 225,
	GPS_creatr_icon_vampr_std = 226,
	GPS_creatr_icon_vampr_dis = 227,
	GPS_creatr_icon_spidr_std = 228,
	GPS_creatr_icon_spidr_dis = 229,
	GPS_creatr_icon_hound_std = 230,
	GPS_creatr_icon_hound_dis = 231,
	GPS_creatr_icon_ghost_std = 232,
	GPS_creatr_icon_ghost_dis = 233,
	GPS_creatr_icon_tentc_std = 234,
	GPS_creatr_icon_tentc_dis = 235,
	GPS_rpanel_rpanel_full_c = 236,
	GPS_rpanel_rpanel_mapbt2a = 237,
	GPS_rpanel_rpanel_mapbt2d = 238,
	GPS_rpanel_rpanel_mapbt3a = 239,
	GPS_rpanel_rpanel_mapbt3d = 240,
	GPS_message_frame_thin_hex_tl = 241,
	GPS_message_frame_thin_hex_ct = 242,
	GPS_message_frame_thin_hex_tr = 243,
	GPS_message_frame_thin_hex_cr = 244,
	GPS_message_frame_thin_hex_fill = 245,
	GPS_message_frame_thin_hex_cl = 246,
	GPS_message_frame_thin_hex_bl = 247,
	GPS_message_frame_thin_hex_cb = 248,
	GPS_message_frame_thin_hex_br = 249,
	GPS_message_rpanel_msg_room_act = 250,
	GPS_message_rpanel_msg_room_std = 251,
	GPS_message_rpanel_msg_spell_act = 252,
	GPS_message_rpanel_msg_spell_std = 253,
	GPS_message_rpanel_msg_manufct_act = 254,
	GPS_message_rpanel_msg_manufct_std = 255,
	GPS_message_rpanel_msg_creatr_act = 256,
	GPS_message_rpanel_msg_creatr_std = 257,
	GPS_message_rpanel_msg_questn_act = 258,
	GPS_message_rpanel_msg_questn_std = 259,
	GPS_message_rpanel_msg_exclam_act = 260,
	GPS_message_rpanel_msg_exclam_std = 261,
	GPS_message_rpanel_msg_battle_act = 262,
	GPS_message_rpanel_msg_battle_std = 263,
	GPS_message_rpanel_msg_battlr_act = 264,
	GPS_message_rpanel_msg_battlr_std = 265,
	GPS_message_rpanel_msg_inforb_act = 266,
	GPS_message_rpanel_msg_inforb_std = 267,
	GPS_message_rpanel_msg_inforg_act = 268,
	GPS_message_rpanel_msg_inforg_std = 269,
	GPS_message_rpanel_msg_infolg_act = 270,
	GPS_message_rpanel_msg_infolg_std = 271,
	GPS_message_message_btn_down_act = 272,
	GPS_message_message_btn_down_std = 273,
	GPS_message_message_btn_accept_act = 274,
	GPS_message_message_btn_accept_std = 275,
	GPS_message_message_btn_show_act = 276,
	GPS_message_message_btn_show_std = 277,
	GPS_rpanel_rpanel_btn_up_act = 278,
	GPS_rpanel_rpanel_btn_up_std = 279,
	GPS_rpanel_rpanel_btn_down_act = 280,
	GPS_rpanel_rpanel_btn_down_std = 281,
	GPS_rpanel_tab_crtr_work_act = 282,
	GPS_rpanel_tab_crtr_work_std = 283,
	GPS_rpanel_tab_crtr_wandr_act = 284,
	GPS_rpanel_tab_crtr_wandr_std = 285,
	GPS_rpanel_tab_crtr_fight_act = 286,
	GPS_rpanel_tab_crtr_fight_std = 287,
	GPS_rpanel_tab_crtr_annoy_lv00 = 288,
	GPS_rpanel_tab_crtr_annoy_lv01 = 289,
	GPS_rpanel_tab_crtr_annoy_lv02 = 290,
	GPS_rpanel_tab_crtr_annoy_lv03 = 291,
	GPS_rpanel_tab_crtr_annoy_lv04 = 292,
	GPS_rpanel_tab_crtr_annoy_lv05 = 293,
	GPS_rpanel_tab_crtr_annoy_lv06 = 294,
	GPS_rpanel_tab_crtr_annoy_lv07 = 295,
	GPS_rpanel_tab_crtr_annoy_lv08 = 296,
	GPS_rpanel_tab_crtr_annoy_lv09 = 297,
	GPS_rpanel_tab_crtr_annoy_lv10 = 298,
	GPS_rpanel_tab_crtr_annoy_lv11 = 299,
	GPS_rpanel_tab_crtr_annoy_lv12 = 300,
	GPS_rpanel_tab_crtr_annoy_lv13 = 301,
	GPS_rpanel_tab_crtr_annoy_lv14 = 302,
	GPS_rpanel_room_ensign_filled = 303,
	GPS_rpanel_rpanel_btn_bigmap_act = 304,
	GPS_rpanel_rpanel_btn_bigmap_std = 305,
	GPS_keepower_chicken_std_l = 306,
	GPS_keepower_chicken_dis_l = 307,
	GPS_keepower_dstwall_std_l = 308,
	GPS_keepower_dstwall_dis_l = 309,
	GPS_keepower_disease_std_l = 310,
	GPS_keepower_disease_dis_l = 311,
	GPS_keepower_armagedn_std_l = 312,
	GPS_keepower_armagedn_dis_l = 313,
	GPS_keepower_chicken_std_s = 314,
	GPS_keepower_chicken_dis_s = 315,

	GPS_keepower_dstwall_std_s = 317,
	GPS_keepower_dstwall_dis_s = 318,
	GPS_keepower_disease_std_s = 319,
	GPS_keepower_disease_dis_s = 320,
	GPS_keepower_armagedn_std_s = 321,
	GPS_keepower_armagedn_dis_s = 322,
	GPS_plyrsym_symbol_player_red_std_a = 323,
	GPS_plyrsym_symbol_room_red_std_a = 324,
	GPS_plyrsym_symbol_player_blue_std_a = 325,
	GPS_plyrsym_symbol_room_blue_std_a = 326,
	GPS_plyrsym_symbol_player_green_std_a = 327,
	GPS_plyrsym_symbol_room_green_std_a = 328,
	GPS_plyrsym_symbol_player_yellow_std_a = 329,
	GPS_plyrsym_symbol_room_yellow_std_a = 330,
	GPS_symbols_creatr_stat_kills_std = 331,
	GPS_symbols_creatr_stat_strength_std = 332,
	GPS_symbols_creatr_stat_gold_std = 333,
	GPS_symbols_creatr_stat_wage_std = 334,
	GPS_symbols_creatr_stat_armor_std = 335,
	GPS_symbols_creatr_stat_age_std = 336,
	GPS_symbols_creatr_stat_dexterity_std = 337,
	GPS_symbols_creatr_stat_defense_std = 338,
	GPS_symbols_creatr_stat_luck_std = 339,
	GPS_symbols_creatr_stat_blood_std = 340,
	GPS_rpanel_rpanel_payday_counter = 341,
	GPS_rpanel_tendency_attacke_act = 342,
	GPS_rpanel_tendency_attacke_std = 343,
	GPS_rpanel_tendency_attackd_act = 344,
	GPS_rpanel_tendency_attackd_std = 345,
	GPS_rpanel_tendency_fleee_act = 346,
	GPS_rpanel_tendency_fleee_std = 347,
	GPS_rpanel_tendency_fleed_act = 348,
	GPS_rpanel_tendency_fleed_std = 349,
	GPS_rpanel_tendency_prisne_act = 350,
	GPS_rpanel_tendency_prisne_std = 351,
	GPS_rpanel_tendency_prisnd_act = 352,
	GPS_rpanel_tendency_prisnd_std = 353,
	GPS_rpanel_tendency_prisnu_dis = 354,
	GPS_crspell_rebound_std_l = 355,
	GPS_crspell_heal_std_l = 356,
	GPS_crspell_heal_dis_l = 357,
	GPS_crspell_poisncld_std_l = 358,
	GPS_crspell_poisncld_dis_l = 359,
	GPS_crspell_conceal_std_l = 360,
	GPS_crspell_conceal_dis_l = 361,
	GPS_crspell_teleport_std_l = 362,
	GPS_crspell_teleport_dis_l = 363,
	GPS_crspell_speedup_std_l = 364,
	GPS_crspell_speedup_dis_l = 365,
	GPS_crspell_slowdn_std_l = 366,
	GPS_crspell_slowdn_dis_l = 367,
	GPS_crspell_missle_std_l = 368,
	GPS_crspell_missle_dis_l = 369,
	GPS_crspell_navmissl_std_l = 370,
	GPS_crspell_navmissl_dis_l = 371,
	GPS_crspell_flambrth_std_l = 372,
	GPS_crspell_flambrth_dis_l = 373,
	GPS_crspell_wind_std_l = 374,
	GPS_crspell_wind_dis_l = 375,
	GPS_crspell_light_std_l = 376,
	GPS_crspell_light_dis_l = 377,
	GPS_crspell_flight_std_l = 378,
	GPS_crspell_flight_dis_l = 379,
	GPS_crspell_sight_std_l = 380,
	GPS_crspell_sight_dis_l = 381,
	GPS_crspell_grenade_std_l = 382,
	GPS_crspell_grenade_dis_l = 383,
	GPS_crspell_hailstrm_std_l = 384,
	GPS_crspell_hailstrm_dis_l = 385,
	GPS_crspell_wop_std_l = 386,
	GPS_crspell_wop_dis_l = 387,
	GPS_crspell_dedlycld_std_l = 388,
	GPS_crspell_dedlycld_dis_l = 389,
	GPS_crspell_dig_std_l = 390,
	GPS_crspell_dig_dis_l = 391,
	GPS_crspell_sword_std_l = 392,
	GPS_crspell_sword_dis_l = 393,
	GPS_crspell_whip_std_l = 394,
	GPS_crspell_whip_dis_l = 395,

	GPS_crspell_hand_std_s = 400,
	GPS_crspell_hand_dis_s = 401,
	GPS_crspell_fireball_std_s = 402,
	GPS_crspell_fireball_dis_s = 403,
	GPS_crspell_meteor_std_s = 404,
	GPS_crspell_meteor_dis_s = 405,
	GPS_crspell_freeze_std_s = 406,
	GPS_crspell_freeze_dis_s = 407,
	GPS_crspell_armor_std_s = 408,
	GPS_crspell_armor_dis_s = 409,
	GPS_crspell_lightng_std_s = 410,
	GPS_crspell_lightng_dis_s = 411,
	GPS_crspell_rebound_std_s = 412,
	GPS_crspell_rebound_dis_s = 413,
	GPS_crspell_heal_std_s = 414,
	GPS_crspell_heal_dis_s = 415,
	GPS_crspell_poisncld_std_s = 416,
	GPS_crspell_poisncld_dis_s = 417,
	GPS_crspell_conceal_std_s = 418,
	GPS_crspell_conceal_dis_s = 419,
	GPS_crspell_teleport_std_s = 420,
	GPS_crspell_teleport_dis_s = 421,
	GPS_crspell_speedup_std_s = 422,
	GPS_crspell_speedup_dis_s = 423,
	GPS_crspell_slowdn_std_s = 424,
	GPS_crspell_slowdn_dis_s = 425,
	GPS_crspell_missle_std_s = 426,
	GPS_crspell_missle_dis_s = 427,
	GPS_crspell_navmissl_std_s = 428,
	GPS_crspell_navmissl_dis_s = 429,
	GPS_crspell_flambrth_std_s = 430,
	GPS_crspell_flambrth_dis_s = 431,
	GPS_crspell_wind_std_s = 432,
	GPS_crspell_wind_dis_s = 433,
	GPS_crspell_light_std_s = 434,
	GPS_crspell_light_dis_s = 435,
	GPS_crspell_flight_std_s = 436,
	GPS_crspell_flight_dis_s = 437,
	GPS_crspell_sight_std_s = 438,
	GPS_crspell_sight_dis_s = 439,
	GPS_crspell_grenade_std_s = 440,
	GPS_crspell_grenade_dis_s = 441,
	GPS_crspell_hailstrm_std_s = 442,
	GPS_crspell_hailstrm_dis_s = 443,
	GPS_crspell_wop_std_s = 444,
	GPS_crspell_wop_dis_s = 445,
	GPS_crspell_dedlycld_std_s = 446,
	GPS_crspell_dedlycld_dis_s = 447,
	GPS_crspell_dig_std_s = 448,
	GPS_crspell_dig_dis_s = 449,
	GPS_crspell_sword_std_s = 450,
	GPS_crspell_sword_dis_s = 451,
	GPS_crspell_whip_std_s = 452,
	GPS_crspell_whip_dis_s = 453,
	GPS_crspell_arrow_std_s = 454,
	GPS_crspell_arrow_dis_s = 455,

	GPS_rpanel_bar_long_full = 458,
	GPS_rpanel_frame_rect_wide_up = 459,
	GPS_rpanel_frame_rect_wide_down = 460,
	GPS_rpanel_bar_with_pic_full_red_up = 461,
	GPS_rpanel_bar_with_pic_full_blue_up = 462,
	GPS_rpanel_bar_with_pic_full_blue_down = 463,
	GPS_rpanel_frame_double_hex_med = 464,
	GPS_rpanel_frame_double_hex_lrg = 465,
	GPS_rpanel_bar_rounded_empty = 466,
	GPS_rpanel_bar_rounded_full = 467,
	GPS_symbols_creatr_mood_vhappy_std = 468,
	GPS_symbols_creatr_mood_happy_std = 469,
	GPS_symbols_creatr_mood_content_std = 470,
	GPS_symbols_creatr_mood_angry_std = 471,
	GPS_symbols_creatr_mood_livid_std = 472,
	GPS_rpanel_rpanel_btn_nxpage_act = 473,
	GPS_rpanel_rpanel_btn_nxpage_std = 474,
	GPS_rpanel_rpanel_btn_crinfo_act = 475,
	GPS_rpanel_rpanel_btn_crinfo_std = 476,

	GPS_message_message_btn_up_act = 486,
	GPS_message_message_btn_up_std = 487,
	GPS_plyrsym_symbol_player_red_std_b = 488,
	GPS_plyrsym_symbol_player_blue_std_b = 489,
	GPS_plyrsym_symbol_player_green_std_b = 490,
	GPS_plyrsym_symbol_player_yellow_std_b = 491,
	GPS_rpanel_rpanel_btn_cassisti_act = 492,
	GPS_rpanel_rpanel_btn_cassisti_std = 493,
	GPS_rpanel_rpanel_btn_cassistw_act = 494,
	GPS_rpanel_rpanel_btn_cassistw_std = 495,
	GPS_creatr_icon_orc_std = 496,
	GPS_creatr_icon_orc_dis = 497,
	GPS_plyrsym_symbol_player_any_dis = 498,
	GPS_rpanel_rpanel_btn_quit_act = 499,
	GPS_rpanel_rpanel_btn_quit_std = 500,
	GPS_options_cassist_btn_black_a = 501,
	GPS_options_cassist_btn_white = 502,
	GPS_options_cassist_btn_orange = 503,
	GPS_options_cassist_btn_black_b = 504,
	GPS_options_cassist_btn_yellow = 505,
	GPS_options_cassist_btn_black_c = 506,
	GPS_options_cassist_btn_pink = 507,
	GPS_options_cassist_btn_black_d = 508,
	GPS_options_cassist_btn_green = 509,
	GPS_options_cassist_btn_black_e = 510,
	GPS_symbols_goldpot_sml = 511,
	GPS_symbols_heartcircle = 512,
	GPS_symbols_progress_red = 513,
	GPS_symbols_progress_blue = 514,
	GPS_symbols_creatr_stat_traingcst_std = 515,
	GPS_symbols_creatr_stat_scavngcst_std = 516,
	GPS_crspell_arrow_std = 517,
	GPS_crspell_arrow_dis = 518,
	GPS_crspell_drain_std = 519,
	GPS_crspell_drain_dis = 520,
	GPS_crspell_fireball_std = 521,
	GPS_crspell_fireball_dis = 522,
	GPS_crspell_freeze_std = 523,
	GPS_crspell_freeze_dis = 524,
	GPS_crspell_meteor_std = 525,
	GPS_crspell_meteor_dis = 526,

	GPS_plyrsym_symbol_player_white_std = 533,
	GPS_plyrsym_symbol_player_white_good_std = 534,
	GPS_plyrsym_symbol_player_red_dead = 535,
	GPS_plyrsym_symbol_player_blue_dead = 536,
	GPS_plyrsym_symbol_player_green_dead = 537,
	GPS_plyrsym_symbol_player_yellow_dead = 538,
	GPS_plyrsym_symbol_player_white_dead = 539,
	GPS_plyrsym_symbol_player_white_good_dead = 540,
	GPS_plyrsym_symbol_player_any_dead = 541,
	GPS_plyrsym_symbol_room_white_std = 542,
    
    GPS_keepower_timebomb_std_s = 550,
    GPS_keepower_timebomb_dis_s = 551,
    GPS_keepower_timebomb_std_l = 566,
    GPS_keepower_timebomb_dis_l = 567,

	GPS_rpanel_rpanel_extra = 547,

	GUI_PANEL_SPRITES_COUNT = 600,
	GUI_PANEL_SPRITES_NEW = 256,
	NEW_GUI_PANEL_SPRITES_COUNT = GUI_PANEL_SPRITES_COUNT + GUI_PANEL_SPRITES_NEW,
};

// Based on frontend-64/filelist_frontbit.txt
enum GUIFrontendSprite {
	GFS_cursor_horny = 1,
	GFS_hugebutton_a01l = 2,
	GFS_hugebutton_a01c = 3,
	GFS_hugebutton_a01r = 4,
	GFS_hugebutton_a02l = 5,
	GFS_hugebutton_a02c = 6,
	GFS_hugebutton_a02r = 7,
	GFS_hugebutton_a03l = 8,
	GFS_hugebutton_a03c = 9,
	GFS_hugebutton_a03r = 10,
	GFS_hugebutton_a04l = 11,
	GFS_hugebutton_a04c = 12,
	GFS_hugebutton_a04r = 13,
	GFS_hugebutton_a05l = 14,
	GFS_hugebutton_a05c = 15,
	GFS_hugebutton_a05r = 16,
	GFS_bullfrog_red_lrg = 17,
	GFS_bullfrog_blue_lrg = 18,
	GFS_bullfrog_green_lrg = 19,
	GFS_bullfrog_yellow_lrg = 20,
	GFS_bullfrog_red_med = 21,
	GFS_bullfrog_blue_med = 22,
	GFS_bullfrog_green_med = 23,
	GFS_bullfrog_yellow_med = 24,
	GFS_hugearea_thn_cor_tl = 25,
	GFS_hugearea_thn_tx1_tc = 26,
	GFS_hugearea_thn_tx2_tc = 27,
	GFS_hugearea_thn_tx3_tc = 28,
	GFS_hugearea_thn_tx4_tc = 29,
	GFS_hugearea_thn_cor_tr = 30,
	GFS_scrollbar_toparrow_std = 31,
	GFS_scrollbar_toparrow_act = 32,
	GFS_hugearea_thn_cor_ml = 33,
	GFS_hugearea_thn_tx1_mc = 34,
	GFS_hugearea_thn_tx2_mc = 35,
	GFS_hugearea_thn_tx3_mc = 36,
	GFS_hugearea_thn_tx4_mc = 37,
	GFS_hugearea_thn_cor_mr = 38,
	GFS_scrollbar_vert_ct_short = 39,
	GFS_hugearea_thc_cor_ml = 40,
	GFS_hugearea_thc_tx1_mc = 41,
	GFS_hugearea_thc_tx2_mc = 42,
	GFS_hugearea_thc_tx3_mc = 43,
	GFS_hugearea_thc_tx4_mc = 44,
	GFS_hugearea_thc_cor_mr = 45,
	GFS_scrollbar_vert_ct_long = 46,
	GFS_hugearea_thn_cor_bl = 47,
	GFS_hugearea_thn_tx1_bc = 48,
	GFS_hugearea_thn_tx2_bc = 49,
	GFS_hugearea_thn_tx3_bc = 50,
	GFS_hugearea_thn_tx4_bc = 51,
	GFS_hugearea_thn_cor_br = 52,
	GFS_scrollbar_btmarrow_std = 53,
	GFS_scrollbar_btmarrow_act = 54,
	GFS_largearea_xts_cor_l = 55,
	GFS_largearea_xts_tx1_c = 56,
	GFS_largearea_xts_tx2_c = 57,
	GFS_largearea_xts_tx3_c = 58,
	GFS_largearea_xts_tx4_c = 59,
	GFS_largearea_xts_cor_r = 60,
	GFS_largebutton_a01l = 61,
	GFS_largebutton_a01r = 62,
	GFS_largebutton_a02l = 63,
	GFS_largebutton_a02r = 64,
	GFS_largebutton_a03l = 65,
	GFS_largebutton_a03r = 66,
	GFS_largebutton_a04l = 67,
	GFS_largebutton_a04r = 68,
	GFS_largebutton_a05l = 69,
	GFS_largebutton_a05r = 70,
	GFS_largearea_nx1_cor_l = 71,
	GFS_largearea_nx1_tx5_c = 72,
	GFS_largearea_nx1_cor_r = 73,
	GFS_hugearea_thc_cor_tl = 74,
	GFS_hugearea_thc_tx1_tc = 75,
	GFS_hugearea_thc_cor_tr = 76,
	GFS_hugearea_thc_tx2_tc = 77,
	GFS_slider_indicator_std = 78,
	GFS_hugearea_thc_tx1_bc = 79,
	GFS_largearea_nx2_cor_l = 80,
	GFS_largearea_nx2_tx5_c = 81,
	GFS_largearea_nx2_cor_r = 82,
	GFS_slidrect_indicator_std0 = 83,
	GFS_slidrect_indicator_std1 = 84,
	GFS_slidrect_indicator_std2 = 85,
	GFS_slidrect_indicator_act = 86,
	GFS_scrollbar_indicator_std = 87,
	GFS_scrollbar_indicator_act = 88,
	GFS_specicon_music = 89,
	GFS_specicon_sound = 90,
	GFS_slider_indicator_act = 91,
	GFS_slider_horiz_l = 92,
	GFS_slider_horiz_c = 93,
	GFS_slider_horiz_r = 94,
	GFS_specicon_voice = 95,
};

#endif
