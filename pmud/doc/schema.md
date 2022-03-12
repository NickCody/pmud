# Documentation

## Data Files


### Simple Start

[module-forgotten-crypt](pmud/conf/module-forgotten-crypt.yaml)


### Game Archtypes

Archtypes are grouped on having similar properties.

archtype.item
archtype.item.weapon (dagger, sword)
archtype.item.armor (none, chain, plate)
archtype.item.adventuring-gear (backpack, boots)
archtype.item.coin (copper, silver, gold)
archtype.item.treasure (chalice, necklace)
archtype.item.trade-good (wheat, beans, lumber)

archtype.place.realm (material realm, astral realm, heaven, abyss)
archtype.place.planet (primordia)
archtype.place.landmass (Tarria, Isle of the Western Breeze)
archtype.place.body-of-water (East Ocean)
archtype.place.region (Tarrian Highlands)
archtype.place.nation (lumerok)
archtype.place.population-center (Lumerok)

archtype.terrain (urban, plains, hills)

archtype.habitation (dungeon, building, farmhouse, temple)

archtype.space (open-air, room, landing, cliff, mountainside)

archtype.race (player races)
archtype.class
archtype.creature
archtype.creature.character
archtype.creature.npc
archtype.creature.monster

### Game Instances

Some properties can be 'none' such as a location of a ship doesn't have a landmass location, but has body-of-water instead.

instance:location (realm, planet, continent, body-of-water, region, nation, population-center, habitation, space)

Locations are the nodes in the world-graph and the properies listed are tags, as spaces may not follow a strict hierarchy.

