//================================================
// Result object returned from Command Execution
//================================================

#pragma once
#include "../../defs.h"

namespace boxer::commands::result {

    class CommandResult {
        private:
        bool_t execution_result;
        stringvec_t payload_vec;

        public:
        CommandResult() { };
        CommandResult(bool_t ExecutionResult, stringvec_t Payload) : execution_result(ExecutionResult), payload_vec(Payload) { };
        
        inline auto success() const       -> bool_t      { return execution_result; }
        inline auto payloadExists() const -> bool_t      { return (payload_vec.size() > 0); }
        inline auto payload() const       -> stringvec_t { return payload_vec; }
        
    };
}
