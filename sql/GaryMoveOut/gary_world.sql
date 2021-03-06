-- Hunterpet vendor
REPLACE INTO `creature_template` (`entry`, `modelid1`, `modelid3`, `name`, `gossip_menu_id`, `minlevel`, `faction_A`, `faction_H`, `npcflag`, `unit_class`, `ScriptName`) VALUES 
('100003', '25103', '25106', 'Beastmaster', '40009', '80', '35', '35', '1', '1', 'npc_hunterpetvendor');

REPLACE INTO `gossip_menu` (entry, text_id)
VALUES ('40009', '40009');

REPLACE INTO `npc_text` (ID, text0_0, text0_1, lang0, prob0, em0_0, em0_1, em0_2, em0_3, em0_4, em0_5, text1_0, text1_1, lang1, prob1, em1_0, em1_1, em1_2, em1_3, em1_4, em1_5, text2_0, text2_1, lang2, prob2, em2_0, em2_1, em2_2, em2_3, em2_4, em2_5, text3_0, text3_1, lang3, prob3, em3_0, em3_1, em3_2, em3_3, em3_4, em3_5, text4_0, text4_1, lang4, prob4, em4_0, em4_1, em4_2, em4_3, em4_4, em4_5, text5_0, text5_1, lang5, prob5, em5_0, em5_1, em5_2, em5_3, em5_4, em5_5, text6_0, text6_1, lang6, prob6, em6_0, em6_1, em6_2, em6_3, em6_4, em6_5, text7_0, text7_1, lang7, prob7, em7_0, em7_1, em7_2, em7_3, em7_4, em7_5, WDBVerified)
VALUES ('40009', 'Hi $N, Please choose a Category:', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

REPLACE INTO `gossip_menu` (entry, text_id)
VALUES ('40010', '40010');

REPLACE INTO `npc_text` (ID, text0_0, text0_1, lang0, prob0, em0_0, em0_1, em0_2, em0_3, em0_4, em0_5, text1_0, text1_1, lang1, prob1, em1_0, em1_1, em1_2, em1_3, em1_4, em1_5, text2_0, text2_1, lang2, prob2, em2_0, em2_1, em2_2, em2_3, em2_4, em2_5, text3_0, text3_1, lang3, prob3, em3_0, em3_1, em3_2, em3_3, em3_4, em3_5, text4_0, text4_1, lang4, prob4, em4_0, em4_1, em4_2, em4_3, em4_4, em4_5, text5_0, text5_1, lang5, prob5, em5_0, em5_1, em5_2, em5_3, em5_4, em5_5, text6_0, text6_1, lang6, prob6, em6_0, em6_1, em6_2, em6_3, em6_4, em6_5, text7_0, text7_1, lang7, prob7, em7_0, em7_1, em7_2, em7_3, em7_4, em7_5, WDBVerified)
VALUES ('40010', 'Sorry, you must be a Hunter to summon a creature.', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

REPLACE INTO `gossip_menu` (entry, text_id)
VALUES ('40011', '40011');

REPLACE INTO `npc_text` (ID, text0_0, text0_1, lang0, prob0, em0_0, em0_1, em0_2, em0_3, em0_4, em0_5, text1_0, text1_1, lang1, prob1, em1_0, em1_1, em1_2, em1_3, em1_4, em1_5, text2_0, text2_1, lang2, prob2, em2_0, em2_1, em2_2, em2_3, em2_4, em2_5, text3_0, text3_1, lang3, prob3, em3_0, em3_1, em3_2, em3_3, em3_4, em3_5, text4_0, text4_1, lang4, prob4, em4_0, em4_1, em4_2, em4_3, em4_4, em4_5, text5_0, text5_1, lang5, prob5, em5_0, em5_1, em5_2, em5_3, em5_4, em5_5, text6_0, text6_1, lang6, prob6, em6_0, em6_1, em6_2, em6_3, em6_4, em6_5, text7_0, text7_1, lang7, prob7, em7_0, em7_1, em7_2, em7_3, em7_4, em7_5, WDBVerified)
VALUES ('40011', 'Please choose a creature:', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

-- Elemental focus(shaman talent) correct proc.
DELETE FROM spell_proc_event WHERE entry = 16164;
INSERT INTO spell_proc_event (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown) VALUES (16164, 0, 11, 2416967683, 4096, 0, 0, 2, 0, 0, 0);

-- Fix Dancing Rune Weapon.
DELETE FROM `spell_proc_event` where `entry` = 49028;
INSERT INTO `spell_proc_event` (entry, procFlags) VALUES
('49028','69652');

-- Fix Lifebloom.
DELETE FROM spell_bonus_data WHERE entry = 33778;
DELETE FROM spell_bonus_data WHERE entry = 33763;
INSERT INTO spell_bonus_data (entry, direct_bonus, dot_bonus, ap_bonus, ap_dot_bonus, comments) VALUES
(33778, 0, 0, 0, 0, 'Druid - Lifebloom final heal'),
(33763, 0.516, 0.0952, 0, 0, 'Druid - Lifebloom HoT(rank 1)');

-- ICC fixs...

   -- update immunity
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36612', '37957', '37958', '37959'); -- Lod Mark'gar
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36855', '38106', '38296', '38297'); -- Lady Deathwhisper
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37813', '38402', '38582', '38583'); -- Deathbringer Saurfang
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36627', '38390', '38549', '38550'); -- Rotface
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36626', '37504', '37505', '37506'); -- Festergut
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('36678', '38431', '38585', '38586'); -- Professor Putricide
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37562', '38602', '38760', '38761'); -- Gas Cloud
   UPDATE `creature_template` set `mechanic_immune_mask` = 617299803 where entry IN ('37697', '38604', '38758', '38759'); -- Volatile Ooze
   UPDATE `creature_template` SET `mechanic_immune_mask` = 617299803 where entry IN ('36980', '38320', '38321', '38322'); -- Ice Tombs / Sindragosa

   -- Rotface:
   -- remove conditions (target selection changed from TARGET_UNIT_NEARBY_ENTRY to TARGET_UNIT_TARGET_ANY so doesn't need it anymore)
    DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=69508;
   /* backup data
   INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
   (13, 0, 69508, 0, 0, 18, 0, 1, 37986, 0, 0, 0, '', 'Rotface - Slime Spray');
   */

   -- Add script
   DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rotface_slime_spray';
   INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
   (69507, 'spell_rotface_slime_spray'),
   (71213, 'spell_rotface_slime_spray'),
   (73189, 'spell_rotface_slime_spray'),
   (73190, 'spell_rotface_slime_spray');

   -- Sindragosa:
   -- Fix spell 69762 Unchained Magic 
   -- Add internal cooldown with 1 seconds, so multi-spell spells will only apply one stack of triggered spell 69766 (i.e. Chain Heal)
   DELETE FROM `spell_proc_event` WHERE `entry` = 69762;
   INSERT INTO `spell_proc_event` (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown) VALUES
   (69762, 0, 0, 0, 0, 0, 81920, 0, 0, 0, 1);

-- Project Astranaar!

   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 6087; -- Astranaar Sentinel (79)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3894; -- Pelturas Whitemoon (124)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3959; -- Nantar (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3961; -- Malyinn (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3915; -- Dagri (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3888; -- Korra (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3691; -- Raene Wolfhunter (124)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3969; -- Fahran Silentblade (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 6738; -- Inkeeper Kimlya (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 10052; -- Maluressian (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3967; -- Aayandia Floralwind (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 11137; -- Xai'ander (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3962; -- Haljan Oakheart (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3954; -- Dalria (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3996; -- Faldreas Goeth'Shael (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 17106; -- Vindicator Palanaar (1638)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 4079; -- Sentinel Thenysil (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3952; -- Aeolynn (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3953; -- Tandaan Lightmane (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3958; -- Lardan (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3970; -- Llana (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3845; -- Sindrell Swiftfire (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 4267; -- Daelyshia (80)
   UPDATE creature_template SET faction_A = 35, faction_H = 35 WHERE entry = 3892;
   UPDATE gameobject_template SET faction = 0 WHERE entry = 142117; -- Astranaar mailbox

-- Transmogrification.
SET
@Entry = 190010,
@Name = "Warpweaver";

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
(@Entry, 0, 0, 0, 0, 0, 19646, 0, 0, 0, @Name, 'Transmogrifier', NULL, 0, 80, 80, 2, 35, 35, 1, 1, 1.14286, 1, 0, 500, 500, 0, 350, 1, 2000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'NPC_Transmogrify', 0);

REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11100, 'Transmogrifications removed from equipped items', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11101, 'You have no transmogrified items equipped', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11102, '%s transmogrification removed', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11103, 'No transmogrification on %s slot', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11104, '%s transmogrified', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11105, 'Selected items are not suitable', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11106, 'Selected item does not exist', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11107, 'Equipment slot is empty', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11108, 'Head', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11109, 'Shoulders', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11110, 'Shirt', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11111, 'Chest', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11112, 'Waist', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11113, 'Legs', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11114, 'Feet', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11115, 'Wrists', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11116, 'Hands', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11117, 'Back', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11118, 'Main hand', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11119, 'Off hand', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11120, 'Ranged', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11121, 'Tabard', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11122, 'Back..', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11123, 'Remove all transmogrifications', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11124, 'Remove transmogrifications from all equipped items?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11125, 'Update menu', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11126, 'Remove transmogrification', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11127, 'Remove transmogrification from %s?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11128, 'Using this item for transmogrify will bind it to you and make it non-refundable and non-tradeable.\r\nDo you wish to continue?\r\n\r\n', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (11129, 'You don\'t have enough %ss', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

-- Announces Administrator / GameMaster
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(787, '|cffffff00[|c1f4DF620GameMaster|r |c100FFFF0%s|c1f4DF620 Announces|cffffff00]:|r %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(11666, '|cffffff00[|c1f4DF620Administrator|r |c100FFFF0%s|c1f4DF620 Announces|cffffff00]:|r %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

REPLACE INTO `command` (`name`, `security`, `help`) VALUES
('msg', 2, 'Syntax: .msg $announcement.\nSend an announcement to all online players, displaying the name of the sender.'),
('adm', 4, 'Syntax: .adm $announcement.\nSend an announcement to all online players, displaying the name of the sender.');

-- Add AutoBanned Passive Anticheat System!
REPLACE INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(11002, '|cFFFFFC00[ANTICHEAT]|cFF00FFFF[|cFF60FF00%s|cFF00FFFF] Banned for cheating!|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

-- Fix [Warrior] T10 4P Bonus.
DELETE FROM spell_script_names WHERE spell_id = 46916;
INSERT INTO spell_script_names VALUES (46916,'spell_warr_slam');

-- [Argent Tournament] Fix Npc Lake Frog.
UPDATE `creature_template` SET `ScriptName` = 'npc_lake_frog' WHERE `entry` IN (33211,33224);
UPDATE `creature_template` SET `gossip_menu_id` = 33220 WHERE `entry` = 33220;
REPLACE INTO `gossip_menu_option` (`menu_id`, `option_text`, `option_id`, `npc_option_npcflag`, `action_script_id`) VALUES ('33220', 'Do you know, where I can find Ashwood Brand Sword?', '1', '1', '33220');
REPLACE INTO `gossip_scripts` (`id`, `command`, `datalong`, `datalong2`) VALUES ('33220', '15', '62554', '3');
