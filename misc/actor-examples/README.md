# Actor Framework Notes

# function-based stateful_actor

This pattern is hot shit.

```c++
struct cell_state {
  int32_t value = 0;
  static inline const char* name = "cell";
};

caf::behavior cell_fun(caf::stateful_actor<cell_state>* self, int32_t value) {
  self->state.value = value;
  return {
    [self](caf::get_atom) {
      return self->state.value;
    },
    [self](caf::put_atom, int32_t new_value) {
      self->state.value = new_value;
    },
  };
}
```
