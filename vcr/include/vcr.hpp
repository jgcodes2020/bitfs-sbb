#ifndef VCR_HPP
#define VCR_HPP

#include <cstdint>
#include <string.h>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Defines bitwise operators on an enum.
 */
#define _PANCAKE_ENUM_BITFIELD_OPS(type)                         \
  constexpr inline type operator|(type lhs, type rhs) {          \
    using impltype = std::underlying_type_t<type>;               \
    using enumtype = type;                                       \
                                                                 \
    return static_cast<enumtype>(                                \
      static_cast<impltype>(lhs) | static_cast<impltype>(rhs));  \
  }                                                              \
                                                                 \
  constexpr inline type operator&(type lhs, type rhs) {          \
    using impltype = std::underlying_type_t<type>;               \
    using enumtype = type;                                       \
                                                                 \
    return static_cast<enumtype>(                                \
      static_cast<impltype>(lhs) & static_cast<impltype>(rhs));  \
  }                                                              \
                                                                 \
  constexpr inline type operator^(type lhs, type rhs) {          \
    using impltype = std::underlying_type_t<type>;               \
    using enumtype = type;                                       \
                                                                 \
    return static_cast<enumtype>(                                \
      static_cast<impltype>(lhs) ^ static_cast<impltype>(rhs));  \
  }                                                              \
                                                                 \
  constexpr inline type operator~(type param) {                  \
    using impltype = std::underlying_type_t<type>;               \
    using enumtype = type;                                       \
                                                                 \
    return static_cast<enumtype>(~static_cast<impltype>(param)); \
  }                                                              \
                                                                 \
  constexpr inline type& operator|=(type& lhs, type rhs) {       \
    return (lhs = lhs | rhs);                                    \
  }                                                              \
                                                                 \
  constexpr inline type& operator&=(type& lhs, type rhs) {       \
    return (lhs = lhs & rhs);                                    \
  }                                                              \
                                                                 \
  constexpr inline type& operator^=(type& lhs, type rhs) {       \
    return (lhs = lhs ^ rhs);                                    \
  }

namespace vcr {
  struct frame final {
    /**
     * @brief Enum representing button inputs.
     */
    enum class button : uint16_t {
      none = 0,
      /**
       * @brief The A button.
       */
      A = 0x8000,
      /**
       * @brief The B button.
       */
      B = 0x4000,
      /**
       * @brief The Z trigger.
       */
      Z = 0x2000,

      /**
       * @brief The START button.
       */
      START = 0x1000,

      /**
       * @brief The UP key on the D-pad.
       */
      D_UP = 0x0800,
      /**
       * @brief The DOWN key on the D-pad.
       */
      D_DOWN = 0x0400,
      /**
       * @brief The LEFT key on the D-pad.
       */
      D_LEFT = 0x0200,
      /**
       * @brief The RIGHT key on the D-pad.
       */
      D_RIGHT = 0x0100,
      
      U1 = 0x0080,
      U2 = 0x0040,

      /**
       * @brief The L trigger.
       */
      L = 0x0020,
      /**
       * @brief The R trigger.
       */
      R = 0x0010,

      /**
       * @brief The C-up (aka C^) button.
       */
      C_UP = 0x0008,
      /**
       * @brief The C-down (aka Cv) button.
       */
      C_DOWN = 0x0004,
      /**
       * @brief The C-left (aka C< or C) button.
       */
      C_LEFT = 0x0002,
      /**
       * @brief The C-right (aka C>) button.
       */
      C_RIGHT = 0x0001
    };
    button buttons;
    // The X/Y coordinates of the analog stick.
    int8_t stick_x, stick_y;
  };

  /**
   * @brief Indicates that a loaded M64 file is invalid.
   *
   */
  class invalid_m64 : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  class m64 {
  private:
    std::vector<frame> m_inputs;
  public:
    using iterator       = decltype(m_inputs)::iterator;
    using const_iterator = decltype(m_inputs)::const_iterator;
    
    /**
     * @brief Metadata struct, contains all the metadata.
     */
    struct metadata_t {
      friend class m64;
    private:
      uint32_t _num_input_frames = 0;
    public:
      /**
       * @brief The version of this M64. Should be 3.
       */
      uint32_t version = 3;
      /**
       * @brief The recording timestamp. Used as a UID of sorts.
       */
      uint32_t timestamp;
      
      /**
       * @brief The length of this movie in VIs.
       */
      uint32_t num_vis = 0;
      /**
       * @brief The number of rerecords (i.e. rewinding back to savestates).
       */
      uint32_t rerecords = 0;
      /**
       * @brief Essentially amounts to frames per second.
       */
      uint8_t vis_per_s = 60;
      
      /**
       * @brief The number of controllers used.
       */
      uint8_t num_controllers = 1;
      
      /**
       * @brief Returns the length of this M64.
       */
      uint32_t num_input_frames() const {
        return _num_input_frames;
      }
      
      /**
       * @brief States that an M64 can be started from.
       */
      enum class start_flags : uint16_t {
        FROM_SNAPSHOT = 0x01,
        FROM_RESET    = 0x02,
        FROM_EEPROM   = 0x04
      };
      /**
       * @brief The way this M64 should be started.
       */
      start_flags start_type = start_flags::FROM_RESET;
      
      /**
       * @brief Bit flags for controllers and their accessories.
       */
      enum class ctrl_flags : uint32_t {
        PORT1_PRESENT = 0x0001,
        PORT2_PRESENT = 0x0002,
        PORT3_PRESENT = 0x0004,
        PORT4_PRESENT = 0x0008,
        PORT1_MEMORY  = 0x0010,
        PORT2_MEMORY  = 0x0020,
        PORT3_MEMORY  = 0x0040,
        PORT4_MEMORY  = 0x0080,
        PORT1_RUMBLE  = 0x0100,
        PORT2_RUMBLE  = 0x0200,
        PORT3_RUMBLE  = 0x0400,
        PORT4_RUMBLE  = 0x0800
      };
      /**
       * @brief The current set of flags for controllers and their accessories.
       */
      ctrl_flags controllers = ctrl_flags::PORT1_PRESENT;
      
      /**
       * @brief The internal ROM name.
       * @note This will be truncated to 31 characters when serializing.
       */
      std::string rom_name;
      /**
       * @brief The CRC32 of the ROM.
       */
      uint32_t crc;
      /**
       * @brief The country code of the ROM.
       */
      uint16_t country_code;
      
      /**
       * @brief The video plugin used.
       * @note This will be truncated to 63 characters when serializing.
       */
      std::string video_plugin;
      /**
       * @brief The sound plugin used.
       * @note This will be truncated to 63 characters when serializing.
       */
      std::string sound_plugin;
      /**
       * @brief The input plugin used (likely TAS Input).
       * @note This will be truncated to 63 characters when serializing.
       */
      std::string input_plugin;
      /**
       * @brief The RSP (Reality Signal Processor) plugin used.
       * @note This will be truncated to 63 characters when serializing.
       */
      std::string rsp_plugin;
      
      /**
       * @brief A list of authors of this M64.
       * @note This will be truncated to 221 bytes when serializing.
       */
      std::string authors;
      
      /**
       * @brief A description of this M64.
       * @note This will be truncated to 255 bytes when serializing.
       */
      std::string description;
    } metadata;
    /**
     * @brief Loads an .m64 file from a path.
     *
     * @param path the path to load from
     */
    m64(std::filesystem::path path);

    /**
     * @brief Creates an M64 directly from a list of inputs and metadata.
     *
     * @tparam input_it an input iterator of m64::frame
     * @param begin the beginning iterator
     * @param end the ending iterator
     * @param mdata the metadata
     */
    template <typename input_it>
    m64(input_it begin, input_it end, const metadata_t& mdata) : m_inputs(begin, end), metadata(mdata) {
      size_t rsize = m_inputs.size();
      // if any of the upper 32 bits are set, it's too big
      if (rsize & 0xFFFFFFFF00000000) {
        throw std::domain_error("M64 files are limited to 2^32 - 1 inputs");
      }
    }

    /**
     * @brief Returns a specific input frame.
     *
     * @param frame the index of the frame
     * @return the input frame at the specified index
     */
    frame& operator[](uint32_t frame);
    /**
     * @brief Returns a specific input frame.
     *
     * @param frame the index of the frame
     * @return the input frame at the specified index
     */
    const frame& operator[](uint32_t frame) const;

    /**
     * @brief Returns a specific input frame, checking bounds.
     *
     * @param frame the index of the frame
     * @return the input frame at the specified index
     */
    frame& at(uint32_t frame);
    const frame& at(uint32_t frame) const;
    
    /**
     * @brief Returns the first frame.
     *
     * @return the first frame
     */
    frame& front();
    const frame& front() const;

    /**
     * @brief Returns the last frame.
     *
     * @return the last frame
     */
    frame& back();
    const frame& back() const;

    /**
     * @brief Returns the length of this .m64.
     *
     * @return uint32_t the length of this .m64.
     */
    uint32_t size() const;

    /**
     * @brief Returns an iterator to the beginning of the input frames.
     *
     * @return auto an iterator to the beginning of the input frames
     */
    iterator begin();

    /**
     * @brief Returns an iterator to the end of the input frames.
     *
     * @return auto an iterator to the end of the input frame
     */
    iterator end();

    /**
     * @brief Returns a const iterator to the beginning of the input frames.
     *
     * @return auto a const iterator to the beginning of the input frames
     */
    const_iterator begin() const;

    /**
     * @brief Returns a const iterator to the end of the input frames.
     *
     * @return auto a const iterator to the end of the input frames
     */
    const_iterator end() const;

    /**
     * @brief Returns a const iterator to the beginning of the input frames.
     *
     * @return auto a const iterator to the beginning of the input frames
     */
    const_iterator cbegin() const;

    /**
     * @brief Returns a const iterator to the end of the input frames.
     *
     * @return auto a const iterator to the end of the input frames
     */
    const_iterator cend() const;

    /**
     * @brief Adds a copy of this frame to the end of the inputs.
     *
     * @param frame the frame to add
     */
    void push_back(const frame& frame);

    /**
     * @brief Adds a frame to the end of the inputs.
     *
     * @param frame the frame to add
     */
    void push_back(frame&& frame);

    /**
     * @brief Removes the last frame.
     */
    void pop_back();
    
    /**
     * @brief Serializes this M64 to an output stream.
     * 
     * @param out an output stream to write to
     */
    void dump(std::filesystem::path path);
  };
  _PANCAKE_ENUM_BITFIELD_OPS(frame::button)
  _PANCAKE_ENUM_BITFIELD_OPS(m64::metadata_t::ctrl_flags)
}  // namespace pancake
#endif