ALTER TABLE `spell_areatrigger` ADD `AnimId` INT(11) NOT NULL DEFAULT '0' AFTER `FacingCurveId`;
ALTER TABLE `spell_areatrigger`	ADD `AnimKitId` INT(11) NOT NULL DEFAULT '0' AFTER `AnimId`;