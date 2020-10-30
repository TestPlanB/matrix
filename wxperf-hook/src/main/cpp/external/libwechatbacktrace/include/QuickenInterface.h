#ifndef _LIBWECHATBACKTRACE_QUICKEN_INTERFACE_H
#define _LIBWECHATBACKTRACE_QUICKEN_INTERFACE_H

#include <stdint.h>
#include <memory>
#include <mutex>

#include <unwindstack/Memory.h>
#include <unwindstack/Error.h>
#include "QuickenTableGenerator.h"
#include "Errors.h"

namespace wechat_backtrace {

class QuickenInterface {

public:
    QuickenInterface(unwindstack::Memory* memory, uint64_t load_bias, unwindstack::ArchEnum arch)
        : memory_(memory), load_bias_(load_bias), arch_(arch){}

    bool FindEntry(uptr pc, uint64_t* entry_offset);

    bool Step(uptr pc, uptr* regs, unwindstack::Memory* process_memory, bool* finished);

    template <typename AddressType>
    bool GenerateQuickenTableUltra(unwindstack::Memory* process_memory);

    bool GenerateQuickenTable(unwindstack::Memory* process_memory);

    uint64_t GetLoadBias();

    void SetArmExidxInfo(uint64_t start_offset, uint64_t total_entries) {
        arm_exidx_info_ = {start_offset, 0, total_entries};
    }

    void SetEhFrameHdrInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        eh_frame_hdr_info_ = {offset, section_bias, size};
    }

    void SetEhFrameInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        eh_frame_info_ = {offset, section_bias, size};
    }

    void SetDebugFrameInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        debug_frame_info_ = {offset, section_bias, size};
    }

    void SetGnuEhFrameHdrInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        gnu_eh_frame_hdr_info_ = {offset, section_bias, size};
    }

    void SetGnuEhFrameInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        gnu_eh_frame_info_ = {offset, section_bias, size};
    }

    void SetGnuDebugFrameInfo(uint64_t offset, int64_t section_bias, uint64_t size) {
        gnu_debug_frame_info_ = {offset, section_bias, size};
    }

    unwindstack::Memory* Memory() {
        return memory_;
    };

    QutErrorCode last_error_code_ = QUT_ERROR_NONE;
    size_t bad_entries_ = 0;

protected:

    std::mutex lock_;

    unwindstack::Memory* memory_;
    uint64_t load_bias_ = 0;

    unwindstack::ArchEnum arch_;

//    // ---- ARM exidx ----
//    uint64_t start_offset_ = 0;
//    uint64_t total_entries_ = 0;

    FrameInfo arm_exidx_info_ = {0};

    FrameInfo eh_frame_hdr_info_ = {0};
    FrameInfo eh_frame_info_ = {0};
    FrameInfo debug_frame_info_ = {0};

    FrameInfo gnu_eh_frame_hdr_info_ = {0};
    FrameInfo gnu_eh_frame_info_ = {0};
    FrameInfo gnu_debug_frame_info_ = {0};

    std::unique_ptr<QutSections> qut_sections_;
};

}  // namespace wechat_backtrace

#endif  // _LIBWECHATBACKTRACE_QUICKEN_INTERFACE_H
