#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Each Soul ability adds its described type")
{
    enum Ability ability;
    enum Type type;
    u32 species;
    enum Type types[3];

    PARAMETRIZE { ability = ABILITY_PLAIN_SOUL;        type = TYPE_NORMAL;   species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_FIGHTER_SOUL;      type = TYPE_FIGHTING; species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_WIND_SOUL;         type = TYPE_FLYING;   species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_VENOM_SOUL;        type = TYPE_POISON;   species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_EARTH_SOUL;        type = TYPE_GROUND;   species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_STONE_SOUL;        type = TYPE_ROCK;     species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_HIVE_SOUL;         type = TYPE_BUG;      species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_PHANTOM_SOUL;      type = TYPE_GHOST;    species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_STEEL_SOUL;        type = TYPE_STEEL;    species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_FLAME_SOUL;        type = TYPE_FIRE;     species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_SEA_SOUL;          type = TYPE_WATER;    species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_VERDANT_SOUL_TYPE; type = TYPE_GRASS;    species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_THUNDER_SOUL;      type = TYPE_ELECTRIC; species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_MIND_SOUL;         type = TYPE_PSYCHIC;  species = SPECIES_SNORLAX; }
    PARAMETRIZE { ability = ABILITY_FROST_SOUL;        type = TYPE_ICE;      species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_DRAGON_SOUL;       type = TYPE_DRAGON;   species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_NIGHT_SOUL;        type = TYPE_DARK;     species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { ability = ABILITY_FAE_SOUL;          type = TYPE_FAIRY;    species = SPECIES_WOBBUFFET; }
    GIVEN {
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        GetBattlerTypes((enum BattlerId)B_POSITION_PLAYER_LEFT, FALSE, types);
        EXPECT_EQ(types[2], type);
    }
}

SINGLE_BATTLE_TEST("Omega adds Normal typing")
{
    enum Type types[3];

    GIVEN {
        PLAYER(SPECIES_ARCEUS_FIRE);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        GetBattlerTypes((enum BattlerId)B_POSITION_PLAYER_LEFT, FALSE, types);
        EXPECT_EQ(types[2], TYPE_NORMAL);
    }
}

SINGLE_BATTLE_TEST("Omega grants its additional abilities")
{
    enum Ability ability;
    enum Ability battlerTraits[MAX_MON_TRAITS];
    enum BattlerId battler = (enum BattlerId)B_POSITION_PLAYER_LEFT;

    PARAMETRIZE { ability = ABILITY_NORMALIZE; }
    PARAMETRIZE { ability = ABILITY_PLAIN_SOUL; }
    PARAMETRIZE { ability = ABILITY_SIMPLE; }
    PARAMETRIZE { ability = ABILITY_REBORN; }
    PARAMETRIZE { ability = ABILITY_MINDS_EYE; }
    PARAMETRIZE { ability = ABILITY_TRUE_GRIT; }
    PARAMETRIZE { ability = ABILITY_ADAPTABILITY; }
    PARAMETRIZE { ability = ABILITY_NEUROFORCE; }
    PARAMETRIZE { ability = ABILITY_TECHNICIAN; }
    PARAMETRIZE { ability = ABILITY_MANEUVERABILITY; }
    PARAMETRIZE { ability = ABILITY_PARENTAL_BOND; }
    PARAMETRIZE { ability = ABILITY_LONG_REACH; }
    PARAMETRIZE { ability = ABILITY_LIFESTEAL; }
    PARAMETRIZE { ability = ABILITY_INFILTRATOR; }
    PARAMETRIZE { ability = ABILITY_MOLD_BREAKER; }
    PARAMETRIZE { ability = ABILITY_DAZZLING; }
    PARAMETRIZE { ability = ABILITY_UNAWARE; }
    PARAMETRIZE { ability = ABILITY_GOOD_AS_GOLD; }
    PARAMETRIZE { ability = ABILITY_FULL_METAL_BODY; }
    PARAMETRIZE { ability = ABILITY_MAGIC_GUARD; }
    PARAMETRIZE { ability = ABILITY_PHALANX; }
    PARAMETRIZE { ability = ABILITY_SPECTRAL; }
    PARAMETRIZE { ability = ABILITY_MENDING; }

    GIVEN {
        PLAYER(SPECIES_ARCEUS_NORMAL);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        STORE_BATTLER_TRAITS(battler);
        EXPECT_NE(BattlerHasTrait(battler, ability), FALSE);
        EXPECT_NE(SearchTraits(battlerTraits, ability), 0);
    }
}

SINGLE_BATTLE_TEST("Fighter Soul adds Fighting STAB to non-Fighting Pokemon", s16 damage)
{
    enum Ability ability;

    PARAMETRIZE { ability = ABILITY_BIG_PECKS; }
    PARAMETRIZE { ability = ABILITY_FIGHTER_SOUL; }

    GIVEN {
        ASSUME(GetMoveType(MOVE_KARATE_CHOP) == TYPE_FIGHTING);
        ASSUME(!IsSpeciesOfType(SPECIES_SNORLAX, TYPE_FIGHTING));
        PLAYER(SPECIES_SNORLAX) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_KARATE_CHOP); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Fighter Soul adds Fighting strengths and weaknesses", s16 damage)
{
    enum Ability ability;
    enum Move move;

    PARAMETRIZE { ability = ABILITY_BIG_PECKS;     move = MOVE_GUST; }
    PARAMETRIZE { ability = ABILITY_FIGHTER_SOUL; move = MOVE_GUST; }
    PARAMETRIZE { ability = ABILITY_BIG_PECKS;     move = MOVE_BITE; }
    PARAMETRIZE { ability = ABILITY_FIGHTER_SOUL; move = MOVE_BITE; }

    GIVEN {
        ASSUME(GetMoveType(MOVE_GUST) == TYPE_FLYING);
        ASSUME(GetMoveType(MOVE_BITE) == TYPE_DARK);
        ASSUME(!IsSpeciesOfType(SPECIES_SNORLAX, TYPE_FIGHTING));
        PLAYER(SPECIES_SNORLAX) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(0.5), results[3].damage);
    }
}

#if MAX_MON_TRAITS > 1
SINGLE_BATTLE_TEST("Fighter Soul adds Fighting STAB to non-Fighting Pokemon (Traits)", s16 damage)
{
    enum Ability ability;

    PARAMETRIZE { ability = ABILITY_BIG_PECKS; }
    PARAMETRIZE { ability = ABILITY_FIGHTER_SOUL; }

    GIVEN {
        ASSUME(GetMoveType(MOVE_KARATE_CHOP) == TYPE_FIGHTING);
        ASSUME(!IsSpeciesOfType(SPECIES_SNORLAX, TYPE_FIGHTING));
        PLAYER(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Innates(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_KARATE_CHOP); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.5), results[1].damage);
    }
}
#endif
