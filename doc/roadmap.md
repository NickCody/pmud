# Feature Roadmap


- Game Engine
  - Maintains clock and turns
  - Loops through actors and signals with update state and asks for input
    - Characters will get prompts ("You hear something")
    - NPC's and Monsters will have their AI called for an event-turn
  
- State Loader / Storage Features
  - Load initial state
  - Dump state for recovery

- Event Sourcing


## Tasks

- [ ] Implement user data model into redis
  - [ ] Bootstrap login data structure into redis
  - [ ] Login interaction with user, user creation
  - [ ] Object model: User, Connection
- [ ] yaml schema for magic and spells
- [ ] Allow yaml to be specified at CLI, not in `pmud.caf` file
- [ ] User Information
  -  [ ] Store ip and time of login
  -  [ ] Login, create username/password (write to data directory) 
  -  [ ] Commands to print whoami, connection info, connection history
- [ ] ncurses
  - Experiment with ncurses to see what's capable
  - Experiment with color

Done:

- [x] Implement item, place, and monster schema
- [x] Implement redis client library
- [x] Implement logging facility with timestamp
- [x] Create abstraction around I/O
  - [x] Move actors into classes (solve the initialization problem)
  - [x] Create User class that persists user information
    - [x] Make storage abstraction
- [x] Create a GameEngine class: take input from the existing asynchronous I/O layer and has abstractions around:
    - [x] New Connection
    - [x] Dropped Connection
    - [x] Login / Account Creation
    - [x] User Input
    - [x] Emit to User (both feedback from their input and async events of which user needs to be aware)


Topics:

- abstractions - user interactions
- storage layer and dependency injection
- coordinate system for positions on the map
- Yaml schema validation using JSON schema
