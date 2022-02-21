# Primordia MUD

## Introduction

This is the Primordia MUD, a multiplayer text-based adventure game.

### To Build and Run

Build this way:

```
bazel build //primordia-mud:pmud
```

Run this way:

```
bazel-bin/primordia-mud/pmud --config-file=.caf_pmud 
```


## Feature Roadmap

1. Create a GameEngine class
  Take input from the existing asynchronous I/O layer and has abstractions around:
    - New Connection
    - Dropped Connection
    - Login / Account Creation
    - User Input
    - Emit to User (both feedback from their input and async events of which user needs to be aware)
1. Create abstraction around I/O
  - Move actors into classes (solve the initialization problem)
  - Create User class that persists user information
    - Make storage abstraction
1. User Information
  -  Store ip and time of login
  -  Login, create username/password (write to data directory) 
  -  Commands to print whoami, connection info, connection history
1. ncurses
  - Experiment with ncurses to see what's capable
  - Experiment with color
1. minor
  - Allow yaml to be specified at CLI, not in `.caf_pmud` file

## Tasks

- [ ] Implement user data model into redis
  - [ ] Bootstrap login data structure into redis
  - [ ] Login interaction with user, user creation
  - [ ] Object model: User, Connection

Done:

- [x] Implement redis client library
- [x] Implement logging facility with timestamp

## Data Files



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


### Simple Start

```yaml
---
module:
  name: Forgotten Crypt
maps:
  -
    name: Forgotten Crypt
    place.realm: Material
    place.planet: Primordia
    place.landmass: Tarria
    archtype.place.region: Tarrian Highlands
    archtype.place.nation: Lumerok
    archtype.place.population-center: Trulum
...
```
