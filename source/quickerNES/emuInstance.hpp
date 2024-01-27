#pragma once

#include <core/emu.hpp>
#include <emuInstanceBase.hpp>

typedef quickerNES::Emu emulator_t;

class EmuInstance : public EmuInstanceBase
{
  public:
  EmuInstance() : EmuInstanceBase()
  {
    // Creating new emulator
    _nes = new emulator_t;

    // Allocating video buffer
    video_buffer = (uint8_t *)malloc(image_width * image_height);

    // Setting video buffer
    _nes->set_pixels(video_buffer, image_width + 8);
  }

  ~EmuInstance() = default;

  virtual bool loadROMFileImpl(const std::string &romData) override
  {
    // Loading rom data
    _nes->load_ines((uint8_t *)romData.data());
    return true;
  }

  uint8_t *getLowMem() const override { return _nes->low_mem(); };
  uint8_t *getNametableMem() const override { return _nes->nametable_mem(); };
  uint8_t *getHighMem() const override { return _nes->high_mem(); };
  const uint8_t *getChrMem() const override { return _nes->chr_mem(); };
  size_t getChrMemSize() const override { return _nes->chr_size(); };

  void serializeState(uint8_t *state) const override {  _nes->serializeState(state); }
  void deserializeState(const uint8_t *state) override { _nes->deserializeState(state); }
  size_t getStateSize() const override { return _nes->getStateSize(); }
  void enableStateBlockImpl(const std::string& block) override { _nes->enableStateBlock(block); };
  void disableStateBlockImpl(const std::string& block) override { _nes->disableStateBlock(block); };

  void advanceStateImpl(const Controller::port_t controller1, const Controller::port_t controller2) override
  {
    if (_doRendering == true) _nes->emulate_frame(controller1, controller2);
    if (_doRendering == false) _nes->emulate_skip_frame(controller1, controller2);
  }

  std::string getCoreName() const override { return "QuickerNES"; }
  void doSoftReset() override { _nes->reset(false); }
  void doHardReset() override { _nes->reset(true); }

  void *getInternalEmulatorPointer() const override { return _nes; }

  // Video buffer
  uint8_t *video_buffer;

  // Emulator instance
  emulator_t *_nes;
};
