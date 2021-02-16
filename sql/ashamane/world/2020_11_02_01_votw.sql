DELETE FROM `phase_area` WHERE `AreaId`=7873 AND `PhaseId` IN (181, 182);
INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`) VALUES
(7873, 181, 'PHASE - CONDITION_QUESTSTATE'),
(7873, 182, 'PHASE - CONDITION_QUESTSTATE');
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 26) AND (`ConditionTypeOrReference` =47) AND (`SourceGroup` IN (181, 182)) AND (`SourceEntry` =7873) AND (`ConditionValue1` =38672); 
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(26,181,7873,0,0,47,0,38672,8,0,0,0,0,'','Phase 181 when quest 38672 taken'),
(26,182,7873,0,0,47,0,38672,2,0,0,0,0,'','Phase 182 when quest 38672 completed');
UPDATE `creature` SET `PhaseId`=181 WHERE `guid` IN (20542616, 20542615);
UPDATE `creature_template` SET `AIName`="", `ScriptName`='npc_maiev_shadowsong_welcome' WHERE `entry`=92718;
DELETE FROM `waypoint_data` WHERE `id` IN (1092718);
INSERT INTO `waypoint_data`(`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES 
(1092718,1,4325.83,-585.576,-281.745,0,0,1,0,100,0),
(1092718,2,4325.83,-582.826,-281.495,0,0,1,0,100,0),
(1092718,3,4325.83,-582.826,-281.495,0,0,1,0,100,0),
(1092718,4,4325.83,-579.576,-281.495,0,0,1,336,100,0),
(1092718,5,4325.83,-576.826,-281.495,0,0,1,0,100,0);
DELETE FROM `gameobject` WHERE `guid` IN (210120185, 210120186, 210120187, 210120189, 210120190, 210120191, 210120192, 210120193, 210120194, 210120195, 210120196);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnDifficulties`,`phaseUseFlags`,`PhaseId`,`PhaseGroup`,`terrainSwapMap`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`ScriptName`,`VerifiedBuild`) VALUES
(210120185, 244944, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4318.35, -587.293, -281.11, 6.07018, 0, 0, 0.106301, -0.994334, 7200, 255, 1, 0, '', 22423),
(210120186, 244944, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4318.39, -586.839, -281.127, 6.07018, 0, 0, 0.106301, -0.994334, 7200, 255, 1, 0, '', 22423),
(210120187, 244582, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4317.79, -587.068, -281.922, 6.05931, 0, 0, 0.111704, -0.993742, 7200, 255, 1, 0, '', 22423),
(210120189, 244925, 1468, 7814, 7814, 0, 0, 181, 0, -1, 4325.66, -576.49, -281.783, 4.65537, -0, -0, -0.726975, 0.686664, 300, 255, 1, 0, '', 22423),
(210120190, 244943, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4333.72, -587.326, -281.146, 0.598127, 0, 0, 0.294625, 0.955613, 7200, 255, 1, 0, '', 22423),
(210120191, 244943, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4334.09, -587.684, -281.203, 0.598127, 0, 0, 0.294625, 0.955613, 7200, 255, 1, 0, '', 22423),
(210120192, 244582, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4334.39, -587.222, -281.922, 3.73722, 0, 0, 0.95598, -0.293431, 7200, 255, 1, 0, '', 22423),
(210120193, 247411, 1468, 7814, 7814, 0, 0, 181, 0, -1, 4338.13, -590.41, -281.766, 0.143833, 0, 0, 0.0718545, 0.997415, 7200, 255, 1, 0, '', 22423),
(210120194, 244923, 1468, 7814, 7873, 0, 0, 181, 0, -1, 4332.15, -582.543, -281.923, 4.96083, 0, 0, 0.614047, -0.78927, 7200, 255, 1, 0, '', 22423),
(210120195, 244923, 1468, 7814, 7873, 0, 0, 182, 0, -1, 4332.15, -582.543, -281.923, 4.96083, 0, 0, 0.614047, -0.78927, 7200, 255, 1, 0, '', 22423),
(210120196, 244925, 1468, 7814, 7814, 0, 0, 182, 0, -1, 4325.66, -576.49, -281.783, 4.65537, 0, 0, -0.726975, 0.686664, 300, 255, 1, 0, '', 22423);
UPDATE `creature` SEt `PhaseId`=182 WHERE `guid` IN (20542658, 20542674);
-- Other SQL fixes from TrinityCore
UPDATE `creature_addon` SET `auras`="" WHERE `auras`="51714"; -- DB/Creature: Remove wrong aura from some spawns
UPDATE `gameobject` SET `spawntimesecs` = 2 WHERE `id` IN
(20691,  -- Cozzle's Footlocker
 20925,  -- Captain's Footlocker
181110,  -- Soaked Tome
181133,  -- Rathis Tomber's Supplies
181238,  -- Dented Chest
181239,  -- Worn Chest
182011,  -- Crate of Ingots
182804,  -- Mysteries of the Light
183050,  -- The Saga of Terokk
187980, -- First Aid Supplies
181138,181139,181140, -- Night Elf Plans: An'daroth, An'owyn, Scrying on the Sin'dorei
182542,181626); -- Sealed Box (Investigate Tuurem), Dawn Runner Cargo/Warped Crates
-- Duskhaven fixes - should be moved to the separate file
DELETE FROM `creature` WHERE `guid` = 801338; -- removed a copy of Prince Liam Greymane 36140
UPDATE `creature` SET `AreaId`=4786 WHERE `guid`= 801588; -- update AreaId to 4786
UPDATE `gameobject` SET `PhaseId`=182 WHERE `id`= 196403; -- Meke Gunpowder Keg visible during quest
DELETE FROM `creature` WHERE `guid` = 801668; -- removed a copy of 36290
DELETE FROM `creature` WHERE `guid` = 801664; -- removed a copy of 36291
DELETE FROM `creature` WHERE `guid` = 801670; -- removed a copy of 36779
DELETE FROM `creature` WHERE `guid` IN (801662, 801660, 801657, 801671); -- removed a copy of Duskhaven Watchman 36690
DELETE FROM `creature` WHERE `guid` IN (801565, 801580, 801628, 801569, 801564, 801573, 801619, 801612, 801647, 801659, 801661); -- removed a copy of Duskhaven Watchmen 36211
DELETE FROM `creature` WHERE `guid` IN (801658, 801665, 804911, 801810, 801809, 804910, 801806, 804912, 801669, 801675); -- removed a copy of Gilnean Mastiff 36713
DELETE FROM `creature` WHERE `guid` IN (801384, 801460, 801389, 801601, 801404); -- removed a copy of 36231
DELETE FROM `creature` WHERE `guid` IN (801387, 801566, 801363, 801435, 801391, 801374, 801492, 801614, 801355, 801367, 801623, 801579, 801372); -- removed a copy of 36283
DELETE FROM `creature` WHERE `guid` IN (801631, 801629, 801558, 801557, 801527, 801524, 801518, 801491, 801667, 801501, 801497, 801483, 801697); -- removed a copy of 36236
DELETE FROM `creature` WHERE `guid` IN (801699, 801700); -- removed a copy of Dark Ranger Thyala 36312
DELETE FROM `creature` WHERE `guid` IN (801539, 801543, 801547); -- removed a copy of Captain Anson 36397
DELETE FROM `creature` WHERE `guid` IN (801625, 801544, 801627); -- removed a copy of Captain Morris 36399
DELETE FROM `creature` WHERE `guid` IN (801505, 801677, 801678); -- removed a copy of Attack Mastiff 36405
UPDATE `npc_spellclick_spells` SET `cast_flags`= 1 WHERE `npc_entry`= 36440 AND `spell_id`= 68735;
UPDATE `creature_template` SET `npcflag`= 16777216, `IconName`= 'questinteract' WHERE `entry`= 36440; -- Make Drowning Watchmane clickable
DELETE FROM `gameobject` WHERE `id`= 196473 AND `guid`= 20406565; -- object=196473/linen-wrapped-book
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES 
(20406565, 196473, 654, 4714, 4806, 0, 183, 0, -2156.35, 2371.38, 10.8817, 1.53765, 0, 0, -0.69529, -0.718729, 30, 255, 1, 0, '', 0);
UPDATE `gameobject` SET `PhaseId`= '183' WHERE `guid`= '51003260'; -- object=196472/grandmas-good-clothes
UPDATE `creature_template` SET `maxlevel`= 12 WHERE `entry` IN (36461, 36671, 36488);
DELETE FROM `creature` WHERE `guid`= 801903; -- removed a copy of Grandma Wahl 36458
DELETE FROM `creature` WHERE `guid` IN (801836, 801901); -- removed a copy of Lucius the Cruel 36461
DELETE FROM `creature` WHERE `guid` IN (801929, 801941, 801937, 801925, 801922, 801944, 801939, 801921, 801931, 801919, 801924, 801926, 801943, 801923); -- removed a copy of 36488
-- Barrel of Coal Tar (196808)
DELETE FROM  `gameobject` WHERE `id` = 196808;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES
('1533744', '196808', '654', '4714', '0', '0', '0', '183', '0', '-1', '-2337.03', '2257.33', '0.328134', '0', '0', '0', '0', '1', '5', '255', '1', '0', '', '26365');
UPDATE `gameobject` SET `spawntimesecs`='5' WHERE  `id`=196808;

DELETE FROM `gameobject_template_locale` WHERE `entry`=196808 AND `locale`='esES';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196808, 'esES', 'Barrica de alquitran de carbon', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196808 AND `locale`='esMX';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196808, 'esMX', 'Barrica de alquitran de carbon', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196808 AND `locale`='ruRU';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196808, 'ruRU', 'Бочка угольной смолы', '', NULL, 26365);
-- Planks of Wood (196809)
DELETE FROM  `gameobject` WHERE `id` = 196809;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES
('1533746', '196809', '654', '4714', '0', '0', '0', '183', '0', '-1', '-2347.02', '2304.01', '0.37202', '0', '0', '0', '0', '1', '5', '255', '1', '0', '', '26365');
UPDATE `gameobject` SET `spawntimesecs`='5' WHERE  `id`=196809;
DELETE FROM `gameobject_template_locale` WHERE `entry`=196809 AND `locale`='esES';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196809, 'esES', 'Tablones de madera', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196809 AND `locale`='esMX';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196809, 'esMX', 'Tablones de madera', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196809 AND `locale`='ruRU';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196809, 'ruRU', 'Доски', '', NULL, 26365);
-- Shipwright's Tools (196810)
DELETE FROM  `gameobject` WHERE `id` = 196810;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`) VALUES
('1533747', '196810', '654', '4714', '4808', '0', '0', '183', '0', '-1', '-2361.65', '2261.07', '2.32001', '0.837758', '0', '0', '0.406736', '0.913545', '5', '255', '1', '0', '', '26365');

UPDATE `gameobject` SET `spawntimesecs`='5' WHERE  `id`=196810;

DELETE FROM `gameobject_template_locale` WHERE `entry`=196810 AND `locale`='esES';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196810, 'esES', 'Herramientas de carpinteria de barcos', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196810 AND `locale`='esMX';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196810, 'esMX', 'Herramientas de carpinteria de barcos', '', NULL, 26365);

DELETE FROM `gameobject_template_locale` WHERE `entry`=196810 AND `locale`='ruRU';
INSERT INTO `gameobject_template_locale` (`entry`, `locale`, `name`, `castBarCaption`, `unk1`, `VerifiedBuild`) VALUES 
(196810, 'ruRU', 'Инструменты корабела', '', NULL, 26365);

DELETE FROM `waypoint_data` WHERE `id`= 3674101;
INSERT INTO `waypoint_data`(`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES 
(3674101,0,-1897.25,2260.34,42.3233,0,0,1,0,100,0),
(3674101,1,-1856.48,2298.407,42.2751,0,0,1,0,100,0),
(3674101,2,-1828.07,2326.007,37.2874,0,0,1,0,100,0),
(3674101,3,-1806.25,2341.49,35.8007,0,0,1,336,100,0),
(3674101,4,-1789.14,2356.74,37.4380,0,0,1,0,100,0),
(3674101,5,-1781.66,2383.98,43.5483,0,0,1,337,100,0),
(3674101,6,-1779.509,2417.72,53.8031,0,0,1,0,100,0),
(3674101,7,-1769.45,2447.63,62.3517,0,0,1,0,100,0),
(3674101,8,-1759.62,2459.43,67.1536,0,0,1,0,100,0),
(3674101,9,-1756.905,2462.21,68.4139,0,0,1,0,100,0),
(3674101,10,-1749.53,2463.18,70.5280,0,0,1,0,100,0),
(3674101,11,-1712.74,2467.504,82.5859,0,0,1,336,100,0),
(3674101,12,-1707.58,2469.71,84.6752,0,0,1,0,100,0),
(3674101,13,-1704.76,2472.15,86.1085,0,0,1,337,100,0),
(3674101,14,-1702.96,2474.909,87.3626,0,0,1,0,100,0),
(3674101,15,-1687.96,2499.91,96.8781,0,0,1,0,100,0),
(3674101,16,-1686.606,2501.53,97.2075,0,0,1,0,100,0),
(3674101,17,-1671.16,2519.94,97.8956,0,0,1,0,100,0);

-- Phase 184
DELETE FROM `spell_area` WHERE `spell`=69077 AND `area`=4714 AND `quest_start`=14465;
-- INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `teamId`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
-- (69077, 4714, 14465, 14466, 0, -1, 0, 2, 3, 74, 11);

DELETE FROM `spell_area` WHERE `spell`=69077 AND `area`=4817 AND `quest_start`=14465;
-- INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `teamId`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
-- (69077, 4817, 14465, 14466, 0, -1, 0, 2, 3, 74, 11);

DELETE FROM `spell_area` WHERE `spell`=69077 AND `area`=4714 AND `quest_start`=14466;
-- INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `teamId`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
-- (69077, 4714, 14466, 24438, 0, -1, 0, 2, 3, 74, 11);

DELETE FROM `spell_area` WHERE `spell`=69077 AND `area`=4817 AND `quest_start`=14466;
-- INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `teamId`, `racemask`, `gender`, `flags`, `quest_start_status`, `quest_end_status`) VALUES 
-- (69077, 4817, 14466, 24438, 0, -1, 0, 2, 3, 74, 11);


-- TrinityCore SAI/DB fixes
-- Eldara Dawnrunner
DELETE FROM `creature_text` WHERE `CreatureID`=25032;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(25032,0,0,"$p, it's very good to see you again!",                                                                                             12,0,100,0,0,0,24086,0,'Eldara Dawnrunner'),
(25032,0,1,"Everyone look, it's $p!",                                                                                                          12,0,100,0,0,0,24087,0,'Eldara Dawnrunner'),
(25032,0,2,"Dawnblade, demons and naga beware, $p is back.",                                                                                   12,0,100,0,0,0,24088,0,'Eldara Dawnrunner'),
(25032,0,3,"$G Milord : Milady;, $p, we are fortunate to have you here with us.",                                                              12,0,100,0,0,0,24089,0,'Eldara Dawnrunner'),
(25032,0,4,"The Shattered Sun Offensive is lucky to have $n on our side.",                                                                     12,0,100,0,0,0,24098,0,'Eldara Dawnrunner'),
(25032,0,5,"If it weren't for $p, we'd all be long dead.",                                                                                     12,0,100,0,0,0,24099,0,'Eldara Dawnrunner'),
(25032,0,6,"$p, thank you again for helping us. We couldn't have achieved any of this without you.",                                           12,0,100,0,0,0,24100,0,'Eldara Dawnrunner'),
(25032,0,7,"$Three cheers for $p!",                                                                                                            12,0,100,0,0,0,24102,0,'Eldara Dawnrunner'),
(25032,0,8,"It's an honor to serve with you, $p.",                                                                                             12,0,100,0,0,0,24103,0,'Eldara Dawnrunner'),
(25032,0,9,"If I were one of those Dawnblade blood elves, I'd be swimming away from the island now that $p is here.",                          12,0,100,0,0,0,24106,0,'Eldara Dawnrunner'),
(25032,0,10,"How many wretched do you think $n has put out of their misery?",                                                                  12,0,100,0,0,0,24107,0,'Eldara Dawnrunner'),
(25032,0,11,"Snap to attention, $p, one of our finest, is here!",                                                                              12,0,100,0,0,0,24108,0,'Eldara Dawnrunner'),
(25032,0,12,"Welcome back, $p.",                                                                                                               12,0,100,0,0,0,24110,0,'Eldara Dawnrunner'),
(25032,0,13,"There's our $g hero : heroine; right there! How are you doing, $p?",                                                              12,0,100,0,0,0,24111,0,'Eldara Dawnrunner'),
(25032,0,14,"I guarantee that without $p, this whole offensive would have been a failure.",                                                    12,0,100,0,0,0,24112,0,'Eldara Dawnrunner'),
(25032,0,15,"No matter what, your efforts will always be exalted here, $p!",                                                                   12,0,100,0,0,0,24113,0,'Eldara Dawnrunner'),
(25032,0,16,"Lay waste to the demons on the Dead Scar lately, $p?",                                                                            12,0,100,0,0,0,24115,0,'Eldara Dawnrunner'),
(25032,0,17,"$p, one of these days I want to buy you a drink.",                                                                                12,0,100,0,0,0,24116,0,'Eldara Dawnrunner'),
(25032,0,18,"$p, you have no idea what a relief it is to see you!",                                                                            12,0,100,0,0,0,24119,0,'Eldara Dawnrunner'),
(25032,0,19,"What a coincidence... we were just talking about you, $p!",                                                                       12,0,100,0,0,0,24120,0,'Eldara Dawnrunner'),
(25032,0,20,"Who needs sentries? $p is here!",                                                                                                 12,0,100,0,0,0,24121,0,'Eldara Dawnrunner'),
(25032,0,21,"Hey, $p, try to leave some of the glory for us, would ya?",                                                                       12,0,100,0,0,0,24122,0,'Eldara Dawnrunner'),
(25032,0,22,"Do not fight the enemy unprepared, $n.  I can provide you with very powerful items... provided you have proven yourself to us.",  12,0,100,0,0,0,24302,0,'Eldara Dawnrunner'),
(25032,0,23,"I don't even want to think about what'll happen here if $p ever decides not to come back.",                                       12,0,100,0,0,0,24117,0,'Eldara Dawnrunner');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry`=25032;
DELETE FROM `smart_scripts` WHERE (`source_type`=0 AND `entryorguid`=25032);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(25032,0,0,0,10,0,100,0,1,20,120000,120000,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Eldara Dawnrunner - OOC LOS - Say Line');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=25032;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 25032, 0, 0, 5, 0, 1077, 128, 0, 0, 0, 0, '', 'Smart event 0 for Eldara Dawnrunner will execute if the payer is exalted with Shattered Sun');

-- Crimson Crysal Shard is a guaranteed drop
UPDATE `creature_loot_template` SET `Chance` = 100 WHERE `Entry` = 19188 AND `Item` = 29476;

-- Diaphanous wing droprate
UPDATE `creature_loot_template` SET `Chance` = 40 WHERE `Item` = 24372 AND `Entry` IN (18132, 18133, 20197, 20198, 18283);

-- Greater and Young Sporebat also drop eyes.
DELETE FROM `creature_loot_template` WHERE `Entry` IN (20387, 18129) AND `Item` = 24426;
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Chance`) VALUES
(18129,24426,20),
(20387,24426,20);

-- Drop chance howling wind
UPDATE `creature_loot_template` SET `Chance` = 20 WHERE `Item` = 24504 AND `Entry` IN (17158,17159,17160);

-- Scripts/Northrend: talk event for quest A Suitable Test Subject (#20462)
-- When using the quest item for A Suitable Test Subject and the spell aura vanishes,
-- Bloodmage Laurith should turn toward the player and whisper a line.
-- This is handled via spell event for Bloodspore Ruination (45997)
DELETE FROM `spell_script_names` WHERE `ScriptName`="spell_q11719_bloodspore_ruination_45997";
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(45997, "spell_q11719_bloodspore_ruination_45997");

UPDATE `creature_template` SET `ScriptName`="npc_bloodmage_laurith" WHERE `entry`=25381;

DELETE FROM `creature_text` WHERE `CreatureID`=25381;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(25381, 0, 0, "How positively awful! You were totally incapacitated? Weak? Hot flashes?", 15, 0, 100, 21, 0, 0, 24992, 0, "Bloodmage Laurith");

-- DB/Creature: Set correct unit flag to Rin'ji
UPDATE `creature_template` SET `unit_flags`=`unit_flags`|768 WHERE `entry`=7780;

-- Underlight Mines
SET @PATHID_1 := 826050;
SET @PATHID_2 := 825820;

UPDATE `creature` SET `MovementType`=2 WHERE `guid` IN (82605, 82582);
DELETE FROM `creature_addon` WHERE `guid` IN (82605, 82582);
INSERT INTO `creature_addon` (`guid`, `path_id`, `bytes2`) VALUES 
(82605, @PATHID_1, 4097),
(82582, @PATHID_2, 4097);

-- Add waypoints
DELETE FROM `waypoint_data` WHERE `id` IN (@PATHID_1, @PATHID_2);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`) VALUES
-- Blackpaw Shaman #1
(@PATHID_1,  1, 7126.29, -6221.997, 21.801),
(@PATHID_1,  2, 7110.97, -6227.314, 21.194),
(@PATHID_1,  3, 7105.31, -6209.855, 21.509),
(@PATHID_1,  4, 7081.72, -6205.795, 21.952),
(@PATHID_1,  5, 7071.54, -6199.641, 22.810),
(@PATHID_1,  6, 7071.65, -6189.511, 21.062),
(@PATHID_1,  7, 7071.54, -6199.641, 22.810),
(@PATHID_1,  8, 7077.39, -6207.805, 22.138),
(@PATHID_1,  9, 7076.12, -6218.441, 23.908),
(@PATHID_1, 10, 7068.51, -6227.06,  23.479),
(@PATHID_1, 11, 7061.94, -6227.88,  22.943),
(@PATHID_1, 12, 7068.51, -6227.06,  23.479),
(@PATHID_1, 13, 7076.12, -6218.44,  23.908),
(@PATHID_1, 14, 7077.39, -6207.80,  22.138),
(@PATHID_1, 15, 7071.54, -6199.64,  22.810),
(@PATHID_1, 16, 7071.65, -6189.51,  21.062),
(@PATHID_1, 17, 7071.54, -6199.64,  22.810),
(@PATHID_1, 18, 7081.72, -6205.79,  21.952),
(@PATHID_1, 19, 7105.31, -6209.85,  21.509),
(@PATHID_1, 20, 7110.97, -6227.31,  21.194),
(@PATHID_1, 21, 7126.29, -6221.99,  21.801),
-- Blackpaw Shaman #2
(@PATHID_2,  1, 7001.19, -6262.55,  6.511),
(@PATHID_2,  2, 7009.89, -6270.94,  5.008),
(@PATHID_2,  3, 7022.81, -6275.65,  7.483),
(@PATHID_2,  4, 7039.36, -6277.72,  8.733),
(@PATHID_2,  5, 7054.01, -6276.99, 12.318),
(@PATHID_2,  6, 7069.66, -6269.71, 15.899),
(@PATHID_2,  7, 7076.94, -6260.30, 17.073),
(@PATHID_2,  8, 7072.61, -6244.68, 19.408),
(@PATHID_2,  9, 7068.06, -6234.58, 22.018),
(@PATHID_2, 10, 7072.61, -6244.68, 19.408),
(@PATHID_2, 11, 7076.94, -6260.30, 17.073),
(@PATHID_2, 12, 7069.66, -6269.71, 15.899),
(@PATHID_2, 13, 7054.01, -6276.99, 12.318),
(@PATHID_2, 14, 7039.36, -6277.72,  8.733),
(@PATHID_2, 15, 7022.81, -6275.65,  7.483),
(@PATHID_2, 16, 7009.89, -6270.94,  5.008),
(@PATHID_2, 17, 7001.19, -6262.55,  6.511),
(@PATHID_2, 18, 7006.29, -6247.67,  6.112);

-- Add correct weapons to specific Blackpaw Gnolls and Blackpaw Scavengers
DELETE FROM `creature_equip_template` WHERE `CreatureID` IN (16334, 16335) AND `id`=2;
INSERT INTO `creature_equip_template` (`CreatureID`, `ID`, `ItemID1`) VALUES
(16334, 2, 2901), -- This weapon isn't sniffed, but looks exactly like the one used in retail. If someone has the sniffed weapon I can update the SQL
(16335, 2, 2901);

UPDATE `creature` SET `equipment_id`=2 WHERE `guid` IN (82576, 82580, 82583, 82584, 82586, 82587, 82591, 82593, 82594, 82595, 82596, 82598, 82601, 82644);

-- Blackpaw Shaman SAI
SET @ENTRY := 16337;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,0,0,3000,4500,11,20802,64,0,0,0,0,2,0,0,0,0,0,0,0,"Blackpaw Shaman - In Combat CMC - Cast 'Lightning Bolt'"),
(@ENTRY,0,1,0,2,0,100,1,0,30,0,0,11,28902,1,0,0,0,0,1,0,0,0,0,0,0,0,"Blackpaw Shaman - Between 0-30% Health - Cast 'Bloodlust' (No Repeat)"),
(@ENTRY,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,0,0,0,0,0,0,0,0,"Blackpaw Shaman - Between 0-15% Health - Flee For Assist (No Repeat)");

-- DB/Creature: Commander Hobb model fix
UPDATE `creature_model_info` SET `DisplayID_Other_Gender`=0 WHERE `DisplayID` IN (21505);

-- DB/SAI: Miles Sidney trigger los event only for players
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=28347;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 2, 28347, 0, 0, 31, 0, 4, 0, 0, 0, 0, 0, "", "Execute SAI only if invoker is a player");

-- Attach touch of the magi aura script
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_touch_of_the_magi_aura';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(210824,'spell_mage_touch_of_the_magi_aura');

-- Core/Scripts Fix issues with Midsummer's Ribbon Pole
DELETE FROM `spell_script_names` WhERE `ScriptName` IN
('spell_midsummer_torch_target_picker',
'spell_midsummer_torch_toss_land',
'spell_midsummer_test_ribbon_pole_channel',
'spell_gen_ribbon_pole_dancer_check',
'spell_midsummer_ribbon_pole_periodic_visual');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(45907,'spell_midsummer_torch_target_picker'),
(46054,'spell_midsummer_torch_toss_land'),
(29705,'spell_midsummer_test_ribbon_pole_channel'),
(29726,'spell_midsummer_test_ribbon_pole_channel'),
(29727,'spell_midsummer_test_ribbon_pole_channel'),
(45406,'spell_midsummer_ribbon_pole_periodic_visual');

-- UPDATE `spell_dbc` SET `Effect1`=3, `EffectImplicitTargetA1`=1 WHERE `Id`=58934;
DELETE FROM `spelleffect_dbc` WHERE `Id`=155919;
INSERT INTO `spelleffect_dbc` (`Id`, `Effect`, `EffectValueMultiplier`, `EffectApplyAuraName`, `EffectAmplitude`, `EffectBasePoints`, `EffectBonusMultiplier`, `EffectDamageMultiplier`, `EffectChainTarget`, `EffectDieSides`, `EffectItemType`, `EffectMechanic`, `EffectMiscValue`, `EffectMiscValueB`, `EffectRadiusIndex`, `EffectRadiusIndexMax`, `EffectRealPointsPerLevel`, `EffectSpellClassMaskA`, `EffectSpellClassMaskB`, `EffectSpellClassMaskC`, `EffectTriggerSpell`, `EffectImplicitTargetA`, `EffectImplicitTargetB`, `EffectSpellId`, `EffectIndex`) VALUES
(155919, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 58934, 0);

UPDATE `creature_template` SET `AIName`='' WHERE `entry`=25535;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=25535;

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=29726;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=45723 AND `spell_effect`=43313;
DELETE FROM `disables` WHERE `sourceType`=0 AND `entry` IN(45724);
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(0,45724,64,0,0,'Disable LoS for spell Braziers Hit');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (45907,46054,45732,29705,29727);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,45907,0,0,31,0,3,25535,0,0,0,'','Spell \'Torch Target Picker\' targets [DNT] Torch Tossing Target Bunny'),
(13,1,45907,0,0,1,0,45723,0,0,1,0,'','Spell \'Torch Target Picker\' can not hit targets with Aura \'Target Indicator (Rank 1)\''),
(13,1,45732,0,0,1,0,45723,0,0,0,0,'','Spell \'Torch Toss\' can only hit targets with Aura \'Target Indicator\''),
(13,1,46054,0,0,1,0,45723,0,0,0,0,'','Spell \'Torch Toss\' can only hit targets with Aura \'Target Indicator\''),
(13,1,46054,0,0,31,0,3,25535,0,0,0,'','Spell \'Torch Target Picker\' targets [DNT] Torch Tossing Target Bunny'),
(13,1,45732,0,0,31,0,3,25535,0,0,0,'','Spell \'Torch Target Picker\' targets [DNT] Torch Tossing Target Bunny'),
(13,1,29705,0,0,31,0,3,17066,0,0,0,'','Spell \'Test Ribbon Pole Channel\' targets Ribbon Pole Debug Target'),
(13,1,29727,0,0,31,0,3,17066,0,0,0,'','Spell \'Test Ribbon Pole Channel\' targets Ribbon Pole Debug Target');