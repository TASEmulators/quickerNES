#pragma once

#include "core/emu.hpp"
#include "../nesInstanceBase.hpp"

typedef quickerNES::Emu emulator_t;

class NESInstance final : public NESInstanceBase
{
  public:

  uint8_t *getLowMem() const override { return _nes.get_low_mem(); };
  size_t getLowMemSize() const override { return _nes.get_low_mem_size(); };

  uint8_t *getWorkMem() const { return _nes.high_mem(); };
  size_t getWorkMemSize() const { return _nes.get_high_mem_size(); };

  uint8_t *getNametableMem() const { return _nes.nametable_mem(); };
  size_t getNametableMemSize() const { return _nes.nametable_size(); };

  uint8_t *getSpriteMem() const { return _nes.spr_mem(); };
  size_t getSpriteMemSize() const { return _nes.spr_mem_size(); };

  uint8_t *getCHRMem() const { return _nes.chr_mem(); };
  size_t getCHRMemSize() const { return _nes.chr_size(); };

  void serializeState(jaffarCommon::serializer::Base& serializer) const override { _nes.serializeState(serializer); }
  void deserializeState(jaffarCommon::deserializer::Base& deserializer) override { _nes.deserializeState(deserializer); }

  std::string getCoreName() const override { return "QuickerNES"; }
  
  void doSoftReset() override { _nes.reset(false); }
  void doHardReset() override { _nes.reset(true); }
  
  void *getInternalEmulatorPointer() override { return &_nes; }
  
  inline size_t getFullStateSize() const override
  {
    jaffarCommon::serializer::Contiguous serializer;
    serializeState(serializer);
    return serializer.getOutputSize();
  }

  inline size_t getDifferentialStateSize() const override
  {
    jaffarCommon::serializer::Differential serializer;
    serializeState(serializer);
    return serializer.getOutputSize();
  }

  void setNTABBlockSize(const size_t size) override { _nes.setNTABBlockSize(size); }
  
  protected:

  bool loadROMImpl(const uint8_t* romData, const size_t romSize) override
  {
    // Loading rom data
    _nes.load_ines(romData);
    return true;
  }

  void enableStateBlockImpl(const std::string& block) override { _nes.enableStateBlock(block); };
  void disableStateBlockImpl(const std::string& block) override { _nes.disableStateBlock(block); };
  void advanceStateImpl(const quickNES::Controller::port_t controller1, const quickNES::Controller::port_t controller2) override
  {
    if (_doRendering == true) _nes.emulate_frame(controller1, controller2);
    if (_doRendering == false) _nes.emulate_skip_frame(controller1, controller2);
  }

  private:

  // Emulator instance
  emulator_t _nes;
};
