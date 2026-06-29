#ifndef SM64_GLOBALS_HPP_INCLUDED
#define SM64_GLOBALS_HPP_INCLUDED

#include "sm64/types.hpp"
#include "sm64/camera.hpp"
#include "sm64/constants.hpp"

namespace sm64 {
  // Special tag type representing a valid global.
  template <class T>
  class global_tag {
    friend class globals;
  public:
    const char* name() const {
      return m_name;
    }
  private:
    constexpr global_tag(const char* name) : m_name(name) {}
    const char* m_name;
  };

  // A directory of usable globals from libsm64.
  struct globals {
    globals() = delete;
    #define SM64_GLOBAL(type, name) static constexpr global_tag<type> name {#name}
    
    // Mario's state.
    SM64_GLOBAL(MarioState[1], gMarioStates);
    // The state of the controllers.
    SM64_GLOBAL(OSContPad[4], gControllerPads);
    // The state of the camera.
    SM64_GLOBAL(Camera*, gCamera);
    // The pool to which all objects are allocated.
    SM64_GLOBAL(Object[OBJECT_POOL_CAPACITY], gObjectPool);

    #undef SM64_GLOBAL
  };
}

#endif