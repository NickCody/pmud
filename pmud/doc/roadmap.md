# Feature Roadmap

- Create a GameEngine class: take input from the existing asynchronous I/O layer and has abstractions around:
    - New Connection
    - Dropped Connection
    - Login / Account Creation
    - User Input
    - Emit to User (both feedback from their input and async events of which user needs to be aware)

- Create abstraction around I/O
  - Move actors into classes (solve the initialization problem)
  - Create User class that persists user information
    - Make storage abstraction

- User Information
  -  Store ip and time of login
  -  Login, create username/password (write to data directory) 
  -  Commands to print whoami, connection info, connection history

- ncurses
  - Experiment with ncurses to see what's capable
  - Experiment with color
  
1. minor
  - Allow yaml to be specified at CLI, not in `.caf_pmud` file

## Tasks

- [ ] Implement user data model into redis
  - [ ] Bootstrap login data structure into redis
  - [ ] Login interaction with user, user creation
  - [ ] Object model: User, Connection
- [ ] yaml schema for magic and spells

Done:

- [x] Implement item, place, and monster schema
- [x] Implement redis client library
- [x] Implement logging facility with timestamp


Topics:

- abstractions - user interactions
- storage layer and dependency injection
- coordinate system for positions on the map
- Yaml schema validation using JSON schema
